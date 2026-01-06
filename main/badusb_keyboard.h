/* Armageddon Framework v3 - badusb_keyboard.h
 * Keyboard layouts et envoi HID
 * Support US QWERTY, FR AZERTY (accents via ALTGR), DE QWERTZ
 */

#ifndef BADUSB_KEYBOARD_H
#define BADUSB_KEYBOARD_H

#include "tinyusb_hid_callbacks.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ KEYBOARD LAYOUT ============
typedef enum {
    KB_LAYOUT_US = 0,
    KB_LAYOUT_FR = 1,
    KB_LAYOUT_DE = 2
} keyboard_layout_t;

// ============ CHARACTER MAPPING ============
typedef struct {
    char character;
    uint8_t keycode;
    uint8_t modifier;
} char_mapping_t;

// ============ FUNCTION DECLARATIONS ============

/* Initialize keyboard module */
void badusb_keyboard_init(void);

/* Send a single character with layout support */
void badusb_keyboard_send_char(char c, keyboard_layout_t layout);

/* Send a string with layout support */
void badusb_keyboard_send_string(const char *str, keyboard_layout_t layout);

/* Send special key (ENTER, TAB, etc.) */
void badusb_keyboard_send_key(uint8_t keycode);

/* Send key with modifier (CTRL+C, etc.) */
void badusb_keyboard_send_key_with_modifier(uint8_t keycode, uint8_t modifier);

/* Send ENTER key */
void badusb_keyboard_send_enter(void);

/* Send TAB key */
void badusb_keyboard_send_tab(void);

/* Send SPACE key */
void badusb_keyboard_send_space(void);

/* Release all keys (send empty report) */
void badusb_keyboard_release_all(void);

/* Delay in milliseconds */
void badusb_keyboard_delay(uint32_t ms);

/* Set active keyboard layout */
void badusb_keyboard_set_layout(keyboard_layout_t layout);

/* Get active keyboard layout */
keyboard_layout_t badusb_keyboard_get_layout(void);

#ifdef __cplusplus
}
#endif

#endif /* BADUSB_KEYBOARD_H */
