/* Armageddon Framework v3 - wifi_attacks.h
 * WiFi Scan + Deauth attacks
 * CRITIQUE : Gestion conflit AP/channel
 */

#ifndef WIFI_ATTACKS_H
#define WIFI_ATTACKS_H

#include <esp_err.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ CONSTANTS ============
#define DEAUTH_FRAMES_PER_BURST     20  // Nombre de frames par burst
#define DEAUTH_BURST_DELAY_MS       50  // Délai entre bursts (ms)
#define DEAUTH_DURATION_MS          5000 // Durée totale d'attaque (5s)

// ============ STRUCTURES ============

/* Deauth attack parameters */
typedef struct {
    uint8_t target_bssid[6];    // BSSID de la cible
    uint8_t channel;            // Canal WiFi
    uint16_t duration_ms;       // Durée de l'attaque (ms)
} deauth_params_t;

// ============ FUNCTION DECLARATIONS ============

/* Initialize WiFi attacks subsystem */
esp_err_t wifi_attacks_init(void);

/* Scan WiFi networks (blocking, ~3 secondes) */
esp_err_t wifi_scan_networks(void);

/* Get number of networks found */
uint16_t wifi_scan_get_count(void);

/* Get network info by index */
esp_err_t wifi_scan_get_network(uint16_t idx, 
                                 char *ssid, uint8_t *bssid, 
                                 int8_t *rssi, uint8_t *channel,
                                 bool *is_secure);

/* Launch deauth attack on target (with AP restart handling) */
esp_err_t wifi_deauth_start(const deauth_params_t *params);

/* Stop deauth attack */
esp_err_t wifi_deauth_stop(void);

/* Get deauth attack status */
esp_err_t wifi_deauth_get_status(uint32_t *packets_sent, bool *active);

/* Check if deauth is running */
bool wifi_deauth_is_active(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_ATTACKS_H */
