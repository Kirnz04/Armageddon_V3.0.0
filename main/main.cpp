/* Armageddon Framework v3 - main.cpp
 * Point d'entrée - Orchestration complète du firmware
 */

#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_event.h>
#include <esp_netif.h>

// Armageddon modules
#include "framework.h"
#include "logger.h"
#include "config.h"
#include "wifi_init.h"
#include "wifi_attacks.h"
#include "badusb_keyboard.h"
#include "tinyusb_hid_callbacks.h"
#include "web_ui.h"

// TinyUSB
#include "tinyusb.h"

static const char *TAG = "MAIN";

// ============ TINYUSB TASK ============
static void tinyusb_task(void *pvParameters) {
    ESP_LOGI(TAG, "TinyUSB task started");
    
    while (1) {
        // TinyUSB device task
        tud_task();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ============ WATCHDOG RESET TASK ============
static void watchdog_task(void *pvParameters) {
    while (1) {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// ============ HEARTBEAT TASK ============
static void heartbeat_task(void *pvParameters) {
    uint32_t tick_count = 0;
    
    while (1) {
        tick_count++;
        
        // Log heartbeat every 10 seconds
        if (tick_count % 10 == 0) {
            ESP_LOGI(TAG, "❤ Armageddon v3 running - Uptime: %lu seconds", 
                     tick_count);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ============ APP MAIN ============
void app_main(void) {
    ESP_LOGI(TAG, "\n\n");
    ESP_LOGI(TAG, "╔════════════════════════════════════╗");
    ESP_LOGI(TAG, "║   ☠ ARMAGEDDON v3.0.0 ☠           ║");
    ESP_LOGI(TAG, "║   ESP32-S3-N16R8 Pentest Firmware  ║");
    ESP_LOGI(TAG, "║   BadUSB + WiFi Attacks            ║");
    ESP_LOGI(TAG, "╚════════════════════════════════════╝");
    ESP_LOGI(TAG, "\n");
    
    // ============ STEP 1: INITIALIZE NVS (Configuration) ============
    ESP_LOGI(TAG, "[1/8] Initializing NVS (Flash persistence)");
    ESP_ERROR_CHECK(config_init());
    
    // Load configuration
    config_t cfg;
    ESP_ERROR_CHECK(config_load(&cfg));
    ESP_LOGI(TAG, "     ✓ Config loaded (Layout: %d)", cfg.keyboard_layout);
    
    // ============ STEP 2: INITIALIZE LOGGER ============
    ESP_LOGI(TAG, "[2/8] Initializing Logger");
    logger_init();
    ESP_LOGI(TAG, "     ✓ Logger ready");
    
    // ============ STEP 3: INITIALIZE FRAMEWORK ============
    ESP_LOGI(TAG, "[3/8] Initializing Framework");
    ESP_ERROR_CHECK(framework_init());
    ESP_LOGI(TAG, "     ✓ Framework initialized");
    
    // ============ STEP 4: INITIALIZE TINYUSB (HID) ============
    ESP_LOGI(TAG, "[4/8] Initializing TinyUSB HID");
    tinyusb_descriptors_init();
    tinyusb_hid_init();
    
    // Start TinyUSB task on core 1
    xTaskCreatePinnedToCore(tinyusb_task, "tinyusb", 4096, NULL, 5, NULL, 1);
    ESP_LOGI(TAG, "     ✓ TinyUSB HID ready");
    
    // ============ STEP 5: INITIALIZE KEYBOARD MODULE ============
    ESP_LOGI(TAG, "[5/8] Initializing Keyboard Module");
    badusb_keyboard_init();
    badusb_keyboard_set_layout((keyboard_layout_t)cfg.keyboard_layout);
    ESP_LOGI(TAG, "     ✓ Keyboard module ready");
    
    // ============ STEP 6: INITIALIZE WIFI (AP+STA) ============
    ESP_LOGI(TAG, "[6/8] Initializing WiFi (APSTA mode)");
    ESP_ERROR_CHECK(wifi_init());
    ESP_ERROR_CHECK(wifi_start_ap());
    ESP_LOGI(TAG, "     ✓ WiFi AP started: %s", cfg.ap_ssid);
    
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // ============ STEP 7: INITIALIZE WIFI ATTACKS ============
    ESP_LOGI(TAG, "[7/8] Initializing WiFi Attacks");
    ESP_ERROR_CHECK(wifi_attacks_init());
    
    // Perform initial scan
    ESP_LOGI(TAG, "     → Scanning WiFi networks...");
    ESP_ERROR_CHECK(wifi_scan_networks());
    uint16_t scan_count = wifi_scan_get_count();
    ESP_LOGI(TAG, "     ✓ Found %d networks", scan_count);
    
    // ============ STEP 8: START WEB SERVER ============
    ESP_LOGI(TAG, "[8/8] Starting HTTP Web Server");
    ESP_ERROR_CHECK(web_ui_init());
    ESP_LOGI(TAG, "     ✓ Web UI running at http://192.168.4.1/");
    
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔════════════════════════════════════╗");
    ESP_LOGI(TAG, "║  ✓ ALL SYSTEMS ONLINE              ║");
    ESP_LOGI(TAG, "║  AP: HackESP32 / redteam123        ║");
    ESP_LOGI(TAG, "║  Web: 192.168.4.1                  ║");
    ESP_LOGI(TAG, "║  Ready for engagement              ║");
    ESP_LOGI(TAG, "╚════════════════════════════════════╝");
    ESP_LOGI(TAG, "\n");
    
    // ============ BACKGROUND TASKS ============
    
    // Watchdog reset task (core 0)
    xTaskCreatePinnedToCore(watchdog_task, "watchdog", 2048, NULL, 1, NULL, 0);
    
    // Heartbeat / monitoring task
    xTaskCreatePinnedToCore(heartbeat_task, "heartbeat", 2048, NULL, 1, NULL, 0);
    
    // ============ MAIN LOOP ============
    // Application loop continues on core 0 (scheduler)
    // TinyUSB runs on core 1
    // All other tasks run in background
    
    ESP_LOGI(TAG, "Application ready. Waiting for commands...");
    
    // Idle - let FreeRTOS scheduler handle everything
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        
        // Periodic status check
        bool usb_ready = tinyusb_hid_is_ready();
        bool ap_running = wifi_is_ap_running();
        
        ESP_LOGI(TAG, "[Status] USB: %s | AP: %s", 
                 usb_ready ? "Ready" : "N/A",
                 ap_running ? "Online" : "Offline");
    }
}
