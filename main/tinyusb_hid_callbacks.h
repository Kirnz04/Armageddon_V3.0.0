/* Armageddon Framework v3 - tinyusb_hid_callbacks.h
 * Callbacks HID pour TinyUSB (set_report, get_report)
 */

#ifndef TINYUSB_HID_CALLBACKS_H
#define TINYUSB_HID_CALLBACKS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ HID KEYBOARD REPORT STRUCTURE ============
typedef struct {
    uint8_t modifier;           // Modifier keys (CTRL, SHIFT, ALT, GUI)
    uint8_t reserved;           // Reserved byte
    uint8_t keycode[6];         // Up to 6 keycodes simultaneously
} hid_keyboard_report_t;

// ============ MODIFIER MASKS ============
#define MOD_LCTRL                   0x01
#define MOD_LSHIFT                  0x02
#define MOD_LALT                    0x04
#define MOD_LGUI                    0x08
#define MOD_RCTRL                   0x10
#define MOD_RSHIFT                  0x20
#define MOD_RALT                    0x40  // ALT GR for FR keyboard
#define MOD_RGUI                    0x80

// ============ SPECIAL KEYCODES ============
#define KEY_NONE                    0x00
#define KEY_A                       0x04
#define KEY_B                       0x05
#define KEY_C                       0x06
#define KEY_D                       0x07
#define KEY_E                       0x08
#define KEY_F                       0x09
#define KEY_G                       0x0A
#define KEY_H                       0x0B
#define KEY_I                       0x0C
#define KEY_J                       0x0D
#define KEY_K                       0x0E
#define KEY_L                       0x0F
#define KEY_M                       0x10
#define KEY_N                       0x11
#define KEY_O                       0x12
#define KEY_P                       0x13
#define KEY_Q                       0x14
#define KEY_R                       0x15
#define KEY_S                       0x16
#define KEY_T                       0x17
#define KEY_U                       0x18
#define KEY_V                       0x19
#define KEY_W                       0x1A
#define KEY_X                       0x1B
#define KEY_Y                       0x1C
#define KEY_Z                       0x1D

#define KEY_1                       0x1E
#define KEY_2                       0x1F
#define KEY_3                       0x20
#define KEY_4                       0x21
#define KEY_5                       0x22
#define KEY_6                       0x23
#define KEY_7                       0x24
#define KEY_8                       0x25
#define KEY_9                       0x26
#define KEY_0                       0x27

#define KEY_ENTER                   0x28
#define KEY_ESC                     0x29
#define KEY_BACKSPACE               0x2A
#define KEY_TAB                     0x2B
#define KEY_SPACE                   0x2C
#define KEY_MINUS                   0x2D
#define KEY_EQUAL                   0x2E
#define KEY_LBRACKET                0x2F
#define KEY_RBRACKET                0x30
#define KEY_BACKSLASH               0x31
#define KEY_SEMICOLON               0x33
#define KEY_APOSTROPHE              0x34
#define KEY_GRAVE                   0x35
#define KEY_COMMA                   0x36
#define KEY_DOT                     0x37
#define KEY_SLASH                   0x38

#define KEY_CAPSLOCK                0x39
#define KEY_F1                      0x3A
#define KEY_F2                      0x3B
#define KEY_F3                      0x3C
#define KEY_F4                      0x3D
#define KEY_F5                      0x3E
#define KEY_F6                      0x3F
#define KEY_F7                      0x40
#define KEY_F8                      0x41
#define KEY_F9                      0x42
#define KEY_F10                     0x43
#define KEY_F11                     0x44
#define KEY_F12                     0x45

#define KEY_DELETE                  0x4C
#define KEY_END                     0x4D
#define KEY_PAGEDOWN                0x4E
#define KEY_HOME                    0x4F
#define KEY_PAGEUP                  0x4B

#define KEY_RIGHT                   0x4F
#define KEY_LEFT                    0x50
#define KEY_DOWN                    0x51
#define KEY_UP                      0x52

// ============ CALLBACK FUNCTIONS ============

/* Initialize TinyUSB HID subsystem */
void tinyusb_hid_init(void);

/* Send HID keyboard report */
void tinyusb_hid_send_report(const hid_keyboard_report_t *report);

/* Send key with modifiers */
void tinyusb_hid_send_key(uint8_t keycode, uint8_t modifier);

/* Send character (handles shift for uppercase) */
void tinyusb_hid_send_char(char c, uint8_t layout);

/* Release all keys (send empty report) */
void tinyusb_hid_release_all(void);

/* Check if USB is ready to send */
bool tinyusb_hid_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* TINYUSB_HID_CALLBACKS_H */
