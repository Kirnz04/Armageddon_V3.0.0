/* Armageddon Framework v3 - tinyusb_descriptors.h
 * Descripteurs USB pour HID Keyboard
 */

#ifndef TINYUSB_DESCRIPTORS_H
#define TINYUSB_DESCRIPTORS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ USB IDS ============
#define USB_VID                    0x1234  // Test VID (change si needed)
#define USB_PID_KEYBOARD           0x5678  // Test PID (change si needed)
#define USB_BCD_DEVICE             0x0100

// ============ STRING INDICES ============
#define STRID_LANGID               0
#define STRID_MANUFACTURER         1
#define STRID_PRODUCT              2
#define STRID_SERIAL               3
#define STRID_HID_KEYBOARD         4

// ============ DESCRIPTOR FUNCTIONS ============

/* USB Device Descriptor */
extern const uint8_t usb_device_descriptor[18];

/* USB Configuration Descriptor (complete with interface and endpoints) */
extern const uint8_t usb_config_descriptor[34];

/* HID Report Descriptor pour Keyboard */
extern const uint8_t usb_hid_report_descriptor[63];

/* String Descriptors */
extern const uint16_t *usb_string_descriptors[];
extern const uint8_t usb_string_descriptors_len;

// ============ DESCRIPTORS INIT ============
void tinyusb_descriptors_init(void);

#ifdef __cplusplus
}
#endif

#endif /* TINYUSB_DESCRIPTORS_H */
