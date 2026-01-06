/* Armageddon Framework v3 - config.h
 * Persistence NVS - Configuration système
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <esp_err.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ NVS NAMESPACE ============
#define CONFIG_NVS_NAMESPACE "armageddon"

// ============ CONFIGURATION STRUCTURE ============
typedef struct {
    char ap_ssid[33];
    char ap_password[64];
    uint8_t keyboard_layout;
    uint16_t deauth_packet_count;
    uint16_t deauth_duration_ms;
    uint8_t ducky_autoplay;
} config_t;

// ============ FUNCTION DECLARATIONS ============

/* Initialize NVS */
esp_err_t config_init(void);

/* Load configuration from NVS */
esp_err_t config_load(config_t *cfg);

/* Save configuration to NVS */
esp_err_t config_save(const config_t *cfg);

/* Get default configuration */
void config_get_defaults(config_t *cfg);

/* Reset configuration to defaults */
esp_err_t config_reset_to_defaults(void);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */
