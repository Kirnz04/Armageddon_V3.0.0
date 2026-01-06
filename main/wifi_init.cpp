/* Armageddon Framework v3 - wifi_init.cpp
 * Implémentation WiFi AP/STA avec gestion de conflits
 * CRITIQUE : Gestion propre des changements de mode
 */

#include "wifi_init.h"
#include "framework.h"
#include <esp_wifi.h>
#include <esp_netif.h>
#include <esp_event.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "WIFI_INIT";

// ============ STATIC VARIABLES ============
static bool wifi_initialized = false;
static wifi_config_t saved_ap_config = {0};
static bool ap_running = false;
static bool sta_running = false;

// ============ EVENT HANDLERS ============
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_AP_START:
                ESP_LOGI(TAG, "AP started");
                ap_running = true;
                break;
            
            case WIFI_EVENT_AP_STOP:
                ESP_LOGI(TAG, "AP stopped");
                ap_running = false;
                break;
            
            case WIFI_EVENT_AP_STACONNECTED: {
                wifi_event_ap_staconnected_t *event = 
                    (wifi_event_ap_staconnected_t *)event_data;
                ESP_LOGI(TAG, "STA connected, MAC: " MACSTR,
                         MAC2STR(event->mac));
                break;
            }
            
            case WIFI_EVENT_AP_STADISCONNECTED: {
                wifi_event_ap_stadisconnected_t *event =
                    (wifi_event_ap_stadisconnected_t *)event_data;
                ESP_LOGI(TAG, "STA disconnected, MAC: " MACSTR,
                         MAC2STR(event->mac));
                break;
            }
            
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "STA started");
                sta_running = true;
                break;
            
            case WIFI_EVENT_STA_STOP:
                ESP_LOGI(TAG, "STA stopped");
                sta_running = false;
                break;
            
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "STA connected to AP");
                break;
            
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "STA disconnected from AP");
                break;
            
            default:
                break;
        }
    }
}

// ============ INITIALIZATION ============
esp_err_t wifi_init(void) {
    if (wifi_initialized) {
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Initializing WiFi (APSTA mode)");
    
    // Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Create event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Create network interfaces
    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();
    
    // Initialize WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Register event handlers
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                               &wifi_event_handler, NULL));
    
    // Set WiFi mode to APSTA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    
    wifi_initialized = true;
    ESP_LOGI(TAG, "WiFi initialized successfully");
    
    return ESP_OK;
}

// ============ AP MANAGEMENT ============
esp_err_t wifi_start_ap(void) {
    if (!wifi_initialized) {
        ESP_LOGE(TAG, "WiFi not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Starting AP: %s", AP_SSID);
    
    wifi_config_t ap_config = {
        .ap = {
            .ssid = AP_SSID,
            .ssid_len = strlen(AP_SSID),
            .password = AP_PASSWORD,
            .channel = AP_CHANNEL,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .max_connection = AP_MAX_CONN,
            .beacon_interval = AP_BEACON_INTERVAL,
        }
    };
    
    // Save config for potential restart
    memcpy(&saved_ap_config, &ap_config, sizeof(wifi_config_t));
    
    // Set and start AP
    esp_err_t ret = esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set AP config: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "AP started successfully");
    
    return ESP_OK;
}

esp_err_t wifi_stop_ap(void) {
    if (!wifi_initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Stopping AP");
    
    esp_err_t ret = esp_wifi_stop();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop WiFi: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
    ap_running = false;
    
    return ESP_OK;
}

esp_err_t wifi_restart_ap(void) {
    ESP_LOGI(TAG, "Restarting AP");
    
    esp_err_t ret = wifi_stop_ap();
    if (ret != ESP_OK) {
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(200));
    
    ret = esp_wifi_set_mode(WIFI_MODE_APSTA);
    if (ret != ESP_OK) {
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
    
    ret = esp_wifi_set_config(WIFI_IF_AP, &saved_ap_config);
    if (ret != ESP_OK) {
        return ret;
    }
    
    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "AP restarted successfully");
    
    return ESP_OK;
}

// ============ STA MANAGEMENT ============
esp_err_t wifi_start_sta(uint8_t channel) {
    if (!wifi_initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Starting STA on channel %d", channel);
    
    wifi_config_t sta_config = {
        .sta = {
            .channel = channel,
            .scan_method = WIFI_ALL_CHANNEL_SCAN,
        }
    };
    
    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA, &sta_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set STA config: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));
    sta_running = true;
    
    return ESP_OK;
}

esp_err_t wifi_stop_sta(void) {
    if (!wifi_initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Stopping STA");
    sta_running = false;
    
    return ESP_OK;
}

// ============ QUERY FUNCTIONS ============
esp_wifi_mode_t wifi_get_mode(void) {
    if (!wifi_initialized) {
        return WIFI_MODE_NULL;
    }
    
    esp_wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    return mode;
}

bool wifi_is_ap_running(void) {
    return ap_running;
}

bool wifi_is_sta_running(void) {
    return sta_running;
}

esp_err_t wifi_set_promiscuous(bool enable) {
    if (!wifi_initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Setting promiscuous mode: %d", enable);
    
    return esp_wifi_set_promiscuous(enable);
}

esp_err_t wifi_get_ap_config(wifi_config_t *config) {
    if (!config) {
        return ESP_ERR_INVALID_ARG;
    }
    
    memcpy(config, &saved_ap_config, sizeof(wifi_config_t));
    return ESP_OK;
}
