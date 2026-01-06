/* Armageddon Framework v3 - framework.cpp
 * Implémentation des fonctions de gestion de base
 */

#include "framework.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// ============ GLOBAL INSTANCE ============
framework_state_t g_framework = {
    .state = STATE_IDLE,
    .current_layout = LAYOUT_US_QWERTY,
    .log_pos = 0,
    .uptime_ms = 0,
    .usb_connected = false
};

static const char *TAG = "FRAMEWORK";

// ============ INITIALIZATION ============
esp_err_t framework_init(void) {
    ESP_LOGI(TAG, "Initializing Armageddon Framework v%s", FRAMEWORK_VERSION);
    
    // Initialize state
    memset(&g_framework.wifi_scan, 0, sizeof(wifi_scan_result_t));
    memset(&g_framework.deauth_status, 0, sizeof(deauth_status_t));
    memset(&g_framework.badusb_status, 0, sizeof(badusb_status_t));
    memset(g_framework.log_buffer, 0, sizeof(g_framework.log_buffer));
    
    g_framework.state = STATE_IDLE;
    g_framework.current_layout = LAYOUT_US_QWERTY;
    
    ESP_LOGI(TAG, "Framework initialized successfully");
    return ESP_OK;
}

// ============ STATE MANAGEMENT ============
void framework_set_state(system_state_t state) {
    if (g_framework.state != state) {
        system_state_t old_state = g_framework.state;
        g_framework.state = state;
        
        const char *state_names[] = {"IDLE", "SCANNING", "DEAUTH", "BADUSB", "ERROR"};
        if (state < 5) {
            ESP_LOGI(TAG, "State change: %s -> %s", 
                     state_names[old_state], 
                     state_names[state]);
        }
    }
}

system_state_t framework_get_state(void) {
    return g_framework.state;
}

// ============ LAYOUT MANAGEMENT ============
void framework_set_layout(keyboard_layout_t layout) {
    if (g_framework.current_layout != layout) {
        const char *layout_names[] = {"US QWERTY", "FR AZERTY", "DE QWERTZ"};
        if (layout < 3) {
            ESP_LOGI(TAG, "Keyboard layout changed to: %s", layout_names[layout]);
        }
        g_framework.current_layout = layout;
    }
}

keyboard_layout_t framework_get_layout(void) {
    return g_framework.current_layout;
}

// ============ LOGGING ============
void framework_log(log_level_t level, const char *tag, const char *fmt, ...) {
    // Get timestamp
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    // Format log message
    va_list args;
    va_start(args, fmt);
    
    // Log to UART
    const char *level_str[] = {"[D]", "[I]", "[W]", "[E]"};
    printf("%02d:%02d:%02d %s [%s] ", 
           timeinfo->tm_hour, 
           timeinfo->tm_min, 
           timeinfo->tm_sec,
           (level < 4) ? level_str[level] : "[?]",
           tag);
    vprintf(fmt, args);
    printf("\n");
    
    va_end(args);
}
