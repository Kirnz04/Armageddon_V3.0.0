/* Armageddon Framework v3 - tinyusb_descriptors.cpp
 * Implémentation des descripteurs USB HID Keyboard
 * CRITIQUE : Stabilité HID maximale
 */

#include "tinyusb_descriptors.h"
#include <string.h>

// ============ USB DEVICE DESCRIPTOR ============
const uint8_t usb_device_descriptor[18] = {
    0x12,                          // bLength
    0x01,                          // bDescriptorType: Device
    0x10, 0x02,                    // bcdUSB: 2.10 (USB 2.0)
    0x00,                          // bDeviceClass: 0 (defined at interface)
    0x00,                          // bDeviceSubClass
    0x00,                          // bDeviceProtocol
    0x40,                          // bMaxPacketSize0: 64
    0x34, 0x12,                    // idVendor: 0x1234 (Test)
    0x78, 0x56,                    // idProduct: 0x5678 (Test)
    USB_BCD_DEVICE & 0xFF, 
    (USB_BCD_DEVICE >> 8) & 0xFF, // bcdDevice: 1.00
    STRID_MANUFACTURER,            // iManufacturer
    STRID_PRODUCT,                 // iProduct
    STRID_SERIAL,                  // iSerialNumber
    0x01                           // bNumConfigurations: 1
};

// ============ USB CONFIGURATION DESCRIPTOR ============
const uint8_t usb_config_descriptor[34] = {
    // Configuration Descriptor
    0x09,                          // bLength
    0x02,                          // bDescriptorType: Configuration
    0x22, 0x00,                    // wTotalLength: 34 bytes
    0x01,                          // bNumInterfaces: 1
    0x01,                          // bConfigurationValue: 1
    0x00,                          // iConfiguration: 0
    0xA0,                          // bmAttributes: Bus powered + Remote wakeup
    0x64,                          // bMaxPower: 200mA (100 * 2mA)

    // Interface Descriptor - HID Keyboard
    0x09,                          // bLength
    0x04,                          // bDescriptorType: Interface
    0x00,                          // bInterfaceNumber: 0
    0x00,                          // bAlternateSetting: 0
    0x01,                          // bNumEndpoints: 1 (EP IN)
    0x03,                          // bInterfaceClass: HID
    0x01,                          // bInterfaceSubClass: Boot Interface
    0x01,                          // bInterfaceProtocol: Keyboard
    STRID_HID_KEYBOARD,            // iInterface: String index

    // HID Descriptor
    0x09,                          // bLength
    0x21,                          // bDescriptorType: HID
    0x11, 0x01,                    // bcdHID: 1.11
    0x00,                          // bCountryCode: 0 (not localized)
    0x01,                          // bNumDescriptors: 1
    0x22,                          // bDescriptorType: Report
    0x3F, 0x00,                    // wDescriptorLength: 63 (size of report descriptor)

    // Endpoint Descriptor - HID IN
    0x07,                          // bLength
    0x05,                          // bDescriptorType: Endpoint
    0x81,                          // bEndpointAddress: IN endpoint 1
    0x03,                          // bmAttributes: Interrupt
    0x40, 0x00,                    // wMaxPacketSize: 64
    0x0A                           // bInterval: 10ms
};

// ============ HID REPORT DESCRIPTOR ============
// Standard Keyboard Report Descriptor
const uint8_t usb_hid_report_descriptor[] = {
    0x05, 0x01,                    // Usage Page (Generic Desktop)
    0x09, 0x06,                    // Usage (Keyboard)
    0xA1, 0x01,                    // Collection (Application)
    0x85, 0x01,                    // Report ID (1)
    0x75, 0x01,                    // Report Size (1)
    0x95, 0x08,                    // Report Count (8)
    0x05, 0x07,                    // Usage Page (Keyboard/Keypad)
    0x19, 0xE0,                    // Usage Minimum (Left Control)
    0x29, 0xE7,                    // Usage Maximum (Right GUI)
    0x15, 0x00,                    // Logical Minimum (0)
    0x25, 0x01,                    // Logical Maximum (1)
    0x81, 0x02,                    // Input (Data, Variable, Absolute)
    0x95, 0x01,                    // Report Count (1)
    0x75, 0x08,                    // Report Size (8)
    0x81, 0x03,                    // Input (Const, Variable, Absolute)
    0x95, 0x05,                    // Report Count (5)
    0x75, 0x01,                    // Report Size (1)
    0x05, 0x08,                    // Usage Page (LEDs)
    0x19, 0x01,                    // Usage Minimum (Num Lock)
    0x29, 0x05,                    // Usage Maximum (Kana)
    0x91, 0x02,                    // Output (Data, Variable, Absolute)
    0x95, 0x01,                    // Report Count (1)
    0x75, 0x03,                    // Report Size (3)
    0x91, 0x03,                    // Output (Const, Variable, Absolute)
    0x95, 0x06,                    // Report Count (6)
    0x75, 0x08,                    // Report Size (8)
    0x15, 0x00,                    // Logical Minimum (0)
    0x25, 0x65,                    // Logical Maximum (101)
    0x05, 0x07,                    // Usage Page (Keyboard/Keypad)
    0x19, 0x00,                    // Usage Minimum (Reserved/No Event)
    0x29, 0x65,                    // Usage Maximum (Keyboard Application)
    0x81, 0x00,                    // Input (Data, Array, Absolute)
    0xC0                           // End Collection
};

// ============ USB STRING DESCRIPTORS ============

// String 0 - Languages
static const uint16_t usb_string_lanid[] = {
    (0x03 << 8) | 0x04,  // wLANGID: English (US)
    0x0409
};

// String 1 - Manufacturer
static const uint16_t usb_string_manufacturer[] = {
    (0x03 << 8) | 22,    // bString: "Armageddon"
    'A', 'r', 'm', 'a', 'g', 'e', 'd', 'd', 'o', 'n'
};

// String 2 - Product
static const uint16_t usb_string_product[] = {
    (0x03 << 8) | 26,    // bString: "BadUSB Framework"
    'B', 'a', 'd', 'U', 'S', 'B', ' ', 'F', 'r', 'a', 'm', 'e', 'w', 'o', 'r', 'k'
};

// String 3 - Serial
static const uint16_t usb_string_serial[] = {
    (0x03 << 8) | 14,    // bString: "123456789ABC"
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C'
};

// String 4 - HID Keyboard Interface
static const uint16_t usb_string_hid_keyboard[] = {
    (0x03 << 8) | 18,    // bString: "Keyboard"
    'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd'
};

// Array of string pointers
const uint16_t *usb_string_descriptors[] = {
    usb_string_lanid,
    usb_string_manufacturer,
    usb_string_product,
    usb_string_serial,
    usb_string_hid_keyboard
};

const uint8_t usb_string_descriptors_len = 5;

// ============ INITIALIZATION ============
void tinyusb_descriptors_init(void) {
    // Descriptors are statically defined and ready to use
    // This function is a placeholder for future initialization if needed
}
