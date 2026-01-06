/* Armageddon Framework v3 - tusb_config.h
 * Configuration TinyUSB pour HID Keyboard (BadUSB)
 * RHPORT0 Device HS, HID-only mode
 */

#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// ============ DEVICE CONFIG ============
#define CFG_TUSB_RHPORT0_MODE           OPT_MODE_DEVICE
#define CFG_TUSB_RHPORT0_SPEED          OPT_SPEED_HIGH  // High-speed USB 2.0

#define CFG_TUSB_OS                     OPT_OS_FREERTOS

// ============ DESCRIPTOR CONFIG ============
#define CFG_TUD_HID                     1    // Enable HID
#define CFG_TUD_HID_EP_BUFSIZE          64   // HID report buffer

// ============ MEMORY CONFIG ============
// EP0 Buffer size - doit être 64 pour HS
#define CFG_TUD_ENDPOINT0_SIZE          64

// ============ CONTROLLER CONFIG ============
#if defined(ESP_PLATFORM)
  #define CFG_TUSB_MEM_SECTION                __attribute__((section(".iram_data")))
  #define CFG_TUSB_MEM_ALIGN                  __attribute__((aligned(4)))
#endif

// ============ DEBUG CONFIG ============
#define CFG_TUSB_DEBUG                  0    // Pas de debug pour performance

// ============ CLASS DRIVER CONFIG ============
// HID Report Descriptor pour Keyboard
#define CFG_TUD_HID_EP_BUFSIZE          64
#define CFG_TUD_HID_KBD_REPORT_ID       1

#ifdef __cplusplus
}
#endif

#endif /* TUSB_CONFIG_H */
