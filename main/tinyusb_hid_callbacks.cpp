/* Armageddon Framework v3 - tinyusb_hid_callbacks.cpp
 * Implémentation des callbacks HID TinyUSB
 * CRITIQUE : Stabilité maximale pour envoi fiable
 */

#include "tinyusb_hid_callbacks.h"
#include "framework.h"
// #include "tusb.h"  // TinyUSB not available in standard ESP-IDF v5.3
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "HID";

// ============ HID STATE ============
static bool hid_ready = false;
static hid_keyboard_report_t last_report = {0};

// ============ FORWARD DECLARATIONS - TinyUSB Callbacks ============

// Callback called when host requests HID report descriptor
uint8_t const *tud_descriptor_device_cb(void);
uint8_t const *tud_descriptor_configuration_cb(uint8_t index);
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid);

// Callback called when USB host sets a report via control endpoint (host to device)
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, 
                            uint8_t const *buffer, uint16_t bufsize);

// Callback called when USB host gets a report via control endpoint (device to host)
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, 
                                uint8_t *buffer, uint16_t reqlen);

// ============ TinyUSB DESCRIPTOR CALLBACKS ============

uint8_t const *tud_descriptor_device_cb(void) {
    extern const uint8_t usb_device_descriptor[];
    return usb_device_descriptor;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    extern const uint8_t usb_config_descriptor[];
    return usb_config_descriptor;
}

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    
    extern const uint16_t *usb_string_descriptors[];
    extern const uint8_t usb_string_descriptors_len;
    
    if (index < usb_string_descriptors_len) {
        return usb_string_descriptors[index];
    }
    return NULL;
}

// ============ TinyUSB HID CALLBACKS ============

void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type,
                            uint8_t const *buffer, uint16_t bufsize) {
    // Called when host sets output/feature report (e.g., keyboard LEDs)
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
    // For keyboard, typically LED status (NumLock, CapsLock, ScrollLock)
}

uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type,
                                uint8_t *buffer, uint16_t reqlen) {
    // Called when host requests input report via control endpoint
    (void)report_id;
    
    if (report_type == HID_REPORT_TYPE_INPUT) {
        memcpy(buffer, &last_report, sizeof(hid_keyboard_report_t));
        return sizeof(hid_keyboard_report_t);
    }
    return 0;
}

// ============ IMPLEMENTATION ============

void tinyusb_hid_init(void) {
    ESP_LOGI(TAG, "Initializing TinyUSB HID");
    hid_ready = true;
    memset(&last_report, 0, sizeof(hid_keyboard_report_t));
}

void tinyusb_hid_send_report(const hid_keyboard_report_t *report) {
    if (!hid_ready || !tud_hid_ready()) {
        return;
    }
    
    // Store for get_report callback
    memcpy(&last_report, report, sizeof(hid_keyboard_report_t));
    
    // Send via interrupt endpoint
    if (tud_hid_keyboard_report(1, report->modifier, (uint8_t *)report->keycode)) {
        esp_task_wdt_reset();  // Reset watchdog
    }
}

void tinyusb_hid_send_key(uint8_t keycode, uint8_t modifier) {
    if (!hid_ready || !tud_hid_ready()) {
        return;
    }
    
    hid_keyboard_report_t report = {0};
    report.modifier = modifier;
    report.keycode[0] = keycode;
    
    tinyusb_hid_send_report(&report);
    
    // Delay for stability
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // Release key
    tinyusb_hid_release_all();
    vTaskDelay(pdMS_TO_TICKS(5));
}

void tinyusb_hid_send_char(char c, uint8_t layout) {
    if (!hid_ready || !tud_hid_ready()) {
        return;
    }
    
    uint8_t keycode = 0;
    uint8_t modifier = 0;
    bool found = false;
    
    // ============ LAYOUT: US QWERTY ============
    if (layout == 0) {
        if (c >= 'a' && c <= 'z') {
            keycode = 0x04 + (c - 'a');
            found = true;
        } else if (c >= 'A' && c <= 'Z') {
            keycode = 0x04 + (c - 'A');
            modifier = MOD_LSHIFT;
            found = true;
        } else if (c >= '0' && c <= '9') {
            keycode = 0x27 - (c - '0');
            if (c == '0') keycode = 0x27;
            found = true;
        } else {
            // Symbols
            switch (c) {
                case ' ': keycode = KEY_SPACE; break;
                case '!': keycode = 0x1E; modifier = MOD_LSHIFT; break;  // Shift+1
                case '@': keycode = 0x1F; modifier = MOD_LSHIFT; break;  // Shift+2
                case '#': keycode = 0x20; modifier = MOD_LSHIFT; break;  // Shift+3
                case '$': keycode = 0x21; modifier = MOD_LSHIFT; break;  // Shift+4
                case '%': keycode = 0x22; modifier = MOD_LSHIFT; break;  // Shift+5
                case '^': keycode = 0x23; modifier = MOD_LSHIFT; break;  // Shift+6
                case '&': keycode = 0x24; modifier = MOD_LSHIFT; break;  // Shift+7
                case '*': keycode = 0x25; modifier = MOD_LSHIFT; break;  // Shift+8
                case '(': keycode = 0x26; modifier = MOD_LSHIFT; break;  // Shift+9
                case ')': keycode = 0x27; modifier = MOD_LSHIFT; break;  // Shift+0
                case '-': keycode = KEY_MINUS; break;
                case '_': keycode = KEY_MINUS; modifier = MOD_LSHIFT; break;
                case '=': keycode = KEY_EQUAL; break;
                case '+': keycode = KEY_EQUAL; modifier = MOD_LSHIFT; break;
                case '[': keycode = KEY_LBRACKET; break;
                case '{': keycode = KEY_LBRACKET; modifier = MOD_LSHIFT; break;
                case ']': keycode = KEY_RBRACKET; break;
                case '}': keycode = KEY_RBRACKET; modifier = MOD_LSHIFT; break;
                case '\\': keycode = KEY_BACKSLASH; break;
                case '|': keycode = KEY_BACKSLASH; modifier = MOD_LSHIFT; break;
                case ';': keycode = KEY_SEMICOLON; break;
                case ':': keycode = KEY_SEMICOLON; modifier = MOD_LSHIFT; break;
                case '\'': keycode = KEY_APOSTROPHE; break;
                case '"': keycode = KEY_APOSTROPHE; modifier = MOD_LSHIFT; break;
                case '`': keycode = KEY_GRAVE; break;
                case '~': keycode = KEY_GRAVE; modifier = MOD_LSHIFT; break;
                case ',': keycode = KEY_COMMA; break;
                case '<': keycode = KEY_COMMA; modifier = MOD_LSHIFT; break;
                case '.': keycode = KEY_DOT; break;
                case '>': keycode = KEY_DOT; modifier = MOD_LSHIFT; break;
                case '/': keycode = KEY_SLASH; break;
                case '?': keycode = KEY_SLASH; modifier = MOD_LSHIFT; break;
                case '\n':
                case '\r': keycode = KEY_ENTER; break;
                case '\t': keycode = KEY_TAB; break;
                default: break;
            }
            if (keycode != 0) found = true;
        }
    }
    // ============ LAYOUT: FR AZERTY ============
    else if (layout == 1) {
        // FR AZERTY mapping
        if (c >= 'a' && c <= 'z') {
            // AZERTY layout (a=q, z=w, etc.)
            const uint8_t fr_keycodes[] = {
                KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, 
                KEY_O, KEY_P, 0x2F, 0x30, 0x31, KEY_N, KEY_M, 0x3B,
                0x34, 0x35, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K
            };
            keycode = fr_keycodes[c - 'a'];
            found = true;
        } else if (c >= 'A' && c <= 'Z') {
            const uint8_t fr_keycodes[] = {
                KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I,
                KEY_O, KEY_P, 0x2F, 0x30, 0x31, KEY_N, KEY_M, 0x3B,
                0x34, 0x35, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K
            };
            keycode = fr_keycodes[c - 'A'];
            modifier = MOD_LSHIFT;
            found = true;
        } else if (c == 'é') {
            keycode = 0x08;  // E
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == 'è') {
            keycode = 0x08;  // E
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == 'ê') {
            keycode = 0x08;  // E
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == 'ç') {
            keycode = 0x06;  // C
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == 'à') {
            keycode = 0x04;  // A
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == 'ù') {
            keycode = 0x18;  // U
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == '€') {
            keycode = KEY_E;
            modifier = MOD_RALT;  // ALT GR
            found = true;
        } else if (c == '_') {
            keycode = 0x2D;  // Minus
            modifier = MOD_RALT;  // ALT GR
            found = true;
        }
    }
    
    if (found && keycode != 0) {
        tinyusb_hid_send_key(keycode, modifier);
    }
}

void tinyusb_hid_release_all(void) {
    if (!hid_ready || !tud_hid_ready()) {
        return;
    }
    
    hid_keyboard_report_t report = {0};
    tinyusb_hid_send_report(&report);
}

bool tinyusb_hid_is_ready(void) {
    return hid_ready && tud_hid_ready();
}
