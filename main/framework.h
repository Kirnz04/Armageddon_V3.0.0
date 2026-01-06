/* Armageddon Framework v3 - framework.h
 * Structures globales, enums et constantes
 */

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ VERSION ============
#define FRAMEWORK_VERSION           "3.0.0"
#define FRAMEWORK_NAME              "Armageddon"

// ============ AP CONFIG ============
#define AP_SSID                     "HackESP32"
#define AP_PASSWORD                 "redteam123"
#define AP_CHANNEL                  6
#define AP_MAX_CONN                 4
#define AP_BEACON_INTERVAL          100

// ============ ENUMS ============

/* États du système */
typedef enum {
    STATE_IDLE = 0,
    STATE_SCANNING = 1,
    STATE_DEAUTH = 2,
    STATE_BADUSB = 3,
    STATE_ERROR = 4
} system_state_t;

/* Layouts clavier */
typedef enum {
    LAYOUT_US_QWERTY = 0,
    LAYOUT_FR_AZERTY = 1,
    LAYOUT_DE_QWERTZ = 2
} keyboard_layout_t;

/* Niveaux de log */
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARN = 2,
    LOG_ERROR = 3
} log_level_t;

// ============ STRUCTURES ============

/* WiFi Network Info */
typedef struct {
    char ssid[33];              // SSID (max 32 chars + null)
    uint8_t bssid[6];           // BSSID (MAC address)
    int8_t rssi;                // Signal strength
    uint8_t channel;            // WiFi channel
    bool is_secure;             // True si WPA/WPA2/WPA3
    uint8_t auth_mode;          // Type de sécurité
} wifi_network_t;

/* Scan WiFi Result */
typedef struct {
    uint16_t count;             // Nombre de réseaux trouvés
    wifi_network_t networks[50]; // Max 50 réseaux
    bool scanning;              // En cours de scan
    uint32_t last_scan_ms;      // Timestamp du dernier scan
} wifi_scan_result_t;

/* Deauth Attack Status */
typedef struct {
    bool active;                // Attaque en cours
    uint8_t target_bssid[6];    // BSSID cible
    char target_ssid[33];       // SSID cible
    uint8_t target_channel;     // Channel de la cible
    uint32_t packets_sent;      // Nombre de paquets envoyés
    uint32_t start_time_ms;     // Timestamp de début
    bool success;               // Succès de l'attaque
} deauth_status_t;

/* BadUSB/DuckyScript Status */
typedef struct {
    bool executing;             // Exécution en cours
    char payload[4096];         // Payload DuckyScript
    uint16_t current_line;      // Ligne actuelle
    uint16_t total_lines;       // Total de lignes
    keyboard_layout_t layout;   // Layout actuel
    uint32_t start_time_ms;     // Timestamp de début
    bool success;               // Succès de l'exécution
    char last_error[256];       // Dernier message d'erreur
} badusb_status_t;

/* Global Framework State */
typedef struct {
    system_state_t state;
    wifi_scan_result_t wifi_scan;
    deauth_status_t deauth_status;
    badusb_status_t badusb_status;
    keyboard_layout_t current_layout;
    char log_buffer[8192];      // Ring buffer pour logs
    uint16_t log_pos;
    uint32_t uptime_ms;
    bool usb_connected;
} framework_state_t;

// ============ GLOBAL INSTANCE ============
extern framework_state_t g_framework;

// ============ FUNCTION DECLARATIONS ============

/* Framework Init */
esp_err_t framework_init(void);

/* State Management */
void framework_set_state(system_state_t state);
system_state_t framework_get_state(void);

/* Layout Management */
void framework_set_layout(keyboard_layout_t layout);
keyboard_layout_t framework_get_layout(void);

/* Logging */
void framework_log(log_level_t level, const char *tag, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* FRAMEWORK_H */
