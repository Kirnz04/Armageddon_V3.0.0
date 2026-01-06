/* Armageddon Framework v3 - wifi_init.h
 * Initialisation WiFi AP/STA et gestion des modes
 */

#ifndef WIFI_INIT_H
#define WIFI_INIT_H

#include <esp_err.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ WiFi MODE MANAGEMENT ============

/* Initialize WiFi subsystem (AP+STA) */
esp_err_t wifi_init(void);

/* Start AP (HackESP32 / redteam123) */
esp_err_t wifi_start_ap(void);

/* Stop AP */
esp_err_t wifi_stop_ap(void);

/* Restart AP (utility for channel switching) */
esp_err_t wifi_restart_ap(void);

/* Start STA mode on specific channel */
esp_err_t wifi_start_sta(uint8_t channel);

/* Stop STA mode */
esp_err_t wifi_stop_sta(void);

/* Get current WiFi mode */
esp_wifi_mode_t wifi_get_mode(void);

/* Check if AP is running */
bool wifi_is_ap_running(void);

/* Check if STA is running */
bool wifi_is_sta_running(void);

/* Set WiFi to promiscuous mode (required for raw frame capture) */
esp_err_t wifi_set_promiscuous(bool enable);

/* Get AP configuration */
esp_err_t wifi_get_ap_config(wifi_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_INIT_H */
