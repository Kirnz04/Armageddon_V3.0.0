/* Armageddon Framework v3 - config.cpp
 * Implémentation NVS persistence
 */

#include "config.h"
#include "framework.h"
#include "wifi_attacks.h"
#include <nvs_flash.h>
#include <nvs.h>
#include <string.h>

static const char *TAG = "CONFIG";

// ============ INITIALIZATION ============
esp_err_t config_init(void) {
    ESP_LOGI(TAG, "Initializing NVS");
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition was truncated, erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    return ret;
}

// ============ LOAD/SAVE ============
esp_err_t config_load(config_t *cfg) {
    if (!cfg) {
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Loading configuration from NVS");
    
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open(CONFIG_NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "NVS namespace not found, using defaults");
        config_get_defaults(cfg);
        return ESP_OK;
    }
    
    // Load SSID
    size_t ssid_len = sizeof(cfg->ap_ssid);
    ret = nvs_get_str(nvs_handle, "ap_ssid", cfg->ap_ssid, &ssid_len);
    if (ret != ESP_OK) {
        strncpy(cfg->ap_ssid, AP_SSID, sizeof(cfg->ap_ssid) - 1);
    }
    
    // Load password
    size_t pwd_len = sizeof(cfg->ap_password);
    ret = nvs_get_str(nvs_handle, "ap_pwd", cfg->ap_password, &pwd_len);
    if (ret != ESP_OK) {
        strncpy(cfg->ap_password, AP_PASSWORD, sizeof(cfg->ap_password) - 1);
    }
    
    // Load keyboard layout
    ret = nvs_get_u8(nvs_handle, "kb_layout", &cfg->keyboard_layout);
    if (ret != ESP_OK) {
        cfg->keyboard_layout = LAYOUT_US_QWERTY;
    }
    
    // Load deauth settings
    ret = nvs_get_u16(nvs_handle, "deauth_pkts", &cfg->deauth_packet_count);
    if (ret != ESP_OK) {
        cfg->deauth_packet_count = DEAUTH_FRAMES_PER_BURST;
    }
    
    ret = nvs_get_u16(nvs_handle, "deauth_dur", &cfg->deauth_duration_ms);
    if (ret != ESP_OK) {
        cfg->deauth_duration_ms = DEAUTH_DURATION_MS;
    }
    
    // Load autoplay setting
    ret = nvs_get_u8(nvs_handle, "autoplay", &cfg->ducky_autoplay);
    if (ret != ESP_OK) {
        cfg->ducky_autoplay = 0;
    }
    
    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "Configuration loaded successfully");
    
    return ESP_OK;
}

esp_err_t config_save(const config_t *cfg) {
    if (!cfg) {
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Saving configuration to NVS");
    
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open(CONFIG_NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Save SSID
    ret |= nvs_set_str(nvs_handle, "ap_ssid", cfg->ap_ssid);
    
    // Save password
    ret |= nvs_set_str(nvs_handle, "ap_pwd", cfg->ap_password);
    
    // Save keyboard layout
    ret |= nvs_set_u8(nvs_handle, "kb_layout", cfg->keyboard_layout);
    
    // Save deauth settings
    ret |= nvs_set_u16(nvs_handle, "deauth_pkts", cfg->deauth_packet_count);
    ret |= nvs_set_u16(nvs_handle, "deauth_dur", cfg->deauth_duration_ms);
    
    // Save autoplay
    ret |= nvs_set_u8(nvs_handle, "autoplay", cfg->ducky_autoplay);
    
    ret |= nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save configuration");
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "Configuration saved successfully");
    return ESP_OK;
}

void config_get_defaults(config_t *cfg) {
    strncpy(cfg->ap_ssid, AP_SSID, sizeof(cfg->ap_ssid) - 1);
    strncpy(cfg->ap_password, AP_PASSWORD, sizeof(cfg->ap_password) - 1);
    cfg->keyboard_layout = LAYOUT_US_QWERTY;
    cfg->deauth_packet_count = DEAUTH_FRAMES_PER_BURST;
    cfg->deauth_duration_ms = DEAUTH_DURATION_MS;
    cfg->ducky_autoplay = 0;
}

esp_err_t config_reset_to_defaults(void) {
    config_t cfg;
    config_get_defaults(&cfg);
    return config_save(&cfg);
}
