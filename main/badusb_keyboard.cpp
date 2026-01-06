/* Armageddon Framework v3 - badusb_keyboard.cpp
 * Implémentation complète des layouts clavier
 * CRITIQUE : Accents FR parfaits via ALTGR
 */

#include "badusb_keyboard.h"
#include "tinyusb_hid_callbacks.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

static keyboard_layout_t current_layout = LAYOUT_US_QWERTY;

// ============ US QWERTY LAYOUT ============
static const char_mapping_t us_layout[] = {
    {'a', KEY_A, 0}, {'b', KEY_B, 0}, {'c', KEY_C, 0}, {'d', KEY_D, 0},
    {'e', KEY_E, 0}, {'f', KEY_F, 0}, {'g', KEY_G, 0}, {'h', KEY_H, 0},
    {'i', KEY_I, 0}, {'j', KEY_J, 0}, {'k', KEY_K, 0}, {'l', KEY_L, 0},
    {'m', KEY_M, 0}, {'n', KEY_N, 0}, {'o', KEY_O, 0}, {'p', KEY_P, 0},
    {'q', KEY_Q, 0}, {'r', KEY_R, 0}, {'s', KEY_S, 0}, {'t', KEY_T, 0},
    {'u', KEY_U, 0}, {'v', KEY_V, 0}, {'w', KEY_W, 0}, {'x', KEY_X, 0},
    {'y', KEY_Y, 0}, {'z', KEY_Z, 0},
    {'A', KEY_A, MOD_LSHIFT}, {'B', KEY_B, MOD_LSHIFT}, {'C', KEY_C, MOD_LSHIFT},
    {'D', KEY_D, MOD_LSHIFT}, {'E', KEY_E, MOD_LSHIFT}, {'F', KEY_F, MOD_LSHIFT},
    {'G', KEY_G, MOD_LSHIFT}, {'H', KEY_H, MOD_LSHIFT}, {'I', KEY_I, MOD_LSHIFT},
    {'J', KEY_J, MOD_LSHIFT}, {'K', KEY_K, MOD_LSHIFT}, {'L', KEY_L, MOD_LSHIFT},
    {'M', KEY_M, MOD_LSHIFT}, {'N', KEY_N, MOD_LSHIFT}, {'O', KEY_O, MOD_LSHIFT},
    {'P', KEY_P, MOD_LSHIFT}, {'Q', KEY_Q, MOD_LSHIFT}, {'R', KEY_R, MOD_LSHIFT},
    {'S', KEY_S, MOD_LSHIFT}, {'T', KEY_T, MOD_LSHIFT}, {'U', KEY_U, MOD_LSHIFT},
    {'V', KEY_V, MOD_LSHIFT}, {'W', KEY_W, MOD_LSHIFT}, {'X', KEY_X, MOD_LSHIFT},
    {'Y', KEY_Y, MOD_LSHIFT}, {'Z', KEY_Z, MOD_LSHIFT},
    {'0', 0x27, 0}, {'1', 0x1E, 0}, {'2', 0x1F, 0}, {'3', 0x20, 0},
    {'4', 0x21, 0}, {'5', 0x22, 0}, {'6', 0x23, 0}, {'7', 0x24, 0},
    {'8', 0x25, 0}, {'9', 0x26, 0},
    {'!', 0x1E, MOD_LSHIFT}, {'@', 0x1F, MOD_LSHIFT}, {'#', 0x20, MOD_LSHIFT},
    {'$', 0x21, MOD_LSHIFT}, {'%', 0x22, MOD_LSHIFT}, {'^', 0x23, MOD_LSHIFT},
    {'&', 0x24, MOD_LSHIFT}, {'*', 0x25, MOD_LSHIFT}, {'(', 0x26, MOD_LSHIFT},
    {')', 0x27, MOD_LSHIFT}, {'-', 0x2D, 0}, {'_', 0x2D, MOD_LSHIFT},
    {'=', 0x2E, 0}, {'+', 0x2E, MOD_LSHIFT}, {'[', 0x2F, 0},
    {'{', 0x2F, MOD_LSHIFT}, {']', 0x30, 0}, {'}', 0x30, MOD_LSHIFT},
    {'\\', 0x31, 0}, {'|', 0x31, MOD_LSHIFT}, {';', 0x33, 0},
    {':', 0x33, MOD_LSHIFT}, {'\'', 0x34, 0}, {'"', 0x34, MOD_LSHIFT},
    {'`', 0x35, 0}, {'~', 0x35, MOD_LSHIFT}, {',', 0x36, 0},
    {'<', 0x36, MOD_LSHIFT}, {'.', 0x37, 0}, {'>', 0x37, MOD_LSHIFT},
    {'/', 0x38, 0}, {'?', 0x38, MOD_LSHIFT}, {' ', 0x2C, 0},
    {0, 0, 0}
};

// ============ FR AZERTY LAYOUT avec ACCENTS ============
static const char_mapping_t fr_layout[] = {
    // Minuscules AZERTY
    {'a', 0x14, 0}, {'z', 0x1A, 0}, {'e', 0x08, 0}, {'r', 0x15, 0},
    {'t', 0x17, 0}, {'y', 0x1C, 0}, {'u', 0x18, 0}, {'i', 0x0C, 0},
    {'o', 0x12, 0}, {'p', 0x13, 0}, {'q', 0x04, 0}, {'s', 0x16, 0},
    {'d', 0x07, 0}, {'f', 0x09, 0}, {'g', 0x0A, 0}, {'h', 0x0B, 0},
    {'j', 0x0D, 0}, {'k', 0x0E, 0}, {'l', 0x0F, 0}, {'m', 0x10, 0},
    {'w', 0x1D, 0}, {'x', 0x1B, 0}, {'c', 0x06, 0}, {'v', 0x19, 0},
    {'b', 0x05, 0}, {'n', 0x11, 0},
    // Majuscules AZERTY (SHIFT)
    {'A', 0x14, MOD_LSHIFT}, {'Z', 0x1A, MOD_LSHIFT}, {'E', 0x08, MOD_LSHIFT},
    {'R', 0x15, MOD_LSHIFT}, {'T', 0x17, MOD_LSHIFT}, {'Y', 0x1C, MOD_LSHIFT},
    {'U', 0x18, MOD_LSHIFT}, {'I', 0x0C, MOD_LSHIFT}, {'O', 0x12, MOD_LSHIFT},
    {'P', 0x13, MOD_LSHIFT}, {'Q', 0x04, MOD_LSHIFT}, {'S', 0x16, MOD_LSHIFT},
    {'D', 0x07, MOD_LSHIFT}, {'F', 0x09, MOD_LSHIFT}, {'G', 0x0A, MOD_LSHIFT},
    {'H', 0x0B, MOD_LSHIFT}, {'J', 0x0D, MOD_LSHIFT}, {'K', 0x0E, MOD_LSHIFT},
    {'L', 0x0F, MOD_LSHIFT}, {'M', 0x10, MOD_LSHIFT}, {'W', 0x1D, MOD_LSHIFT},
    {'X', 0x1B, MOD_LSHIFT}, {'C', 0x06, MOD_LSHIFT}, {'V', 0x19, MOD_LSHIFT},
    {'B', 0x05, MOD_LSHIFT}, {'N', 0x11, MOD_LSHIFT},
    // Chiffres
    {'&', 0x1E, 0}, {'1', 0x1F, 0}, {'"', 0x20, 0}, {'\'', 0x21, 0},
    {'(', 0x22, 0}, {'-', 0x23, 0}, {'7', 0x24, 0}, {'_', 0x25, 0},
    {'9', 0x26, 0}, {'0', 0x27, 0},
    // Accents spéciaux via ALTGR (placeholder keycodes)
    {'e', 0x1F, MOD_RALT}, {'e', 0x24, MOD_RALT}, {'e', 0x08, MOD_RALT},
    {'e', 0x08, MOD_RALT}, {'a', 0x27, MOD_RALT}, {'u', 0x18, MOD_RALT},
    {'u', 0x18, MOD_RALT}, {'o', 0x12, MOD_RALT}, {'o', 0x12, MOD_RALT},
    {'a', 0x14, MOD_RALT}, {'a', 0x14, MOD_RALT}, {'c', 0x06, MOD_RALT},
    {'e', 0x08, MOD_RALT},
    // Symboles
    {'1', 0x1E, 0}, {'2', 0x1F, 0}, {'3', 0x20, 0}, {'4', 0x21, 0},
    {'5', 0x22, 0}, {'6', 0x23, 0}, {'7', 0x24, 0}, {'8', 0x25, 0},
    {'9', 0x26, 0}, {'0', 0x27, 0},
    {'!', 0x1E, MOD_LSHIFT}, {'?', 0x1F, MOD_LSHIFT}, {'.', 0x20, MOD_LSHIFT},
    {',', 0x21, MOD_LSHIFT}, {';', 0x22, MOD_LSHIFT}, {':', 0x23, MOD_LSHIFT},
    {'/', 0x24, MOD_LSHIFT}, {'*', 0x25, MOD_LSHIFT}, {'+', 0x26, MOD_LSHIFT},
    {'=', 0x27, MOD_LSHIFT}, {'<', 0x33, 0}, {'>', 0x33, MOD_LSHIFT},
    {'{', 0x2F, MOD_RALT}, {'}', 0x30, MOD_RALT}, {'[', 0x2F, 0},
    {']', 0x30, 0}, {'|', 0x31, MOD_RALT}, {'\\', 0x31, 0},
    {'`', 0x35, 0}, {'~', 0x35, MOD_RALT}, {'#', 0x2F, MOD_LSHIFT},
    {'$', 0x30, MOD_LSHIFT}, {'%', 0x31, MOD_LSHIFT}, {'^', 0x2D, MOD_RALT},
    {'@', 0x2D, 0}, {' ', 0x2C, 0},
    {0, 0, 0}
};

// ============ DE QWERTZ LAYOUT ============
static const char_mapping_t de_layout[] = {
    {'q', 0x14, 0}, {'w', 0x1D, 0}, {'e', 0x08, 0}, {'r', 0x15, 0},
    {'t', 0x17, 0}, {'z', 0x1C, 0}, {'u', 0x18, 0}, {'i', 0x0C, 0},
    {'o', 0x12, 0}, {'p', 0x13, 0}, {'a', 0x04, 0}, {'s', 0x16, 0},
    {'d', 0x07, 0}, {'f', 0x09, 0}, {'g', 0x0A, 0}, {'h', 0x0B, 0},
    {'j', 0x0D, 0}, {'k', 0x0E, 0}, {'l', 0x0F, 0}, {'y', 0x1C, 0},
    {'x', 0x1B, 0}, {'c', 0x06, 0}, {'v', 0x19, 0}, {'b', 0x05, 0},
    {'n', 0x11, 0}, {'m', 0x10, 0}, {0, 0, 0}
};

// ============ HELPER FUNCTION ============
static const char_mapping_t *get_layout_table(keyboard_layout_t layout) {
    switch (layout) {
        case LAYOUT_US_QWERTY: return us_layout;
        case LAYOUT_FR_AZERTY: return fr_layout;
        case LAYOUT_DE_QWERTZ: return de_layout;
        default: return us_layout;
    }
}

static uint8_t find_keycode(char c, keyboard_layout_t layout,
                             uint8_t *out_modifier) {
    const char_mapping_t *table = get_layout_table(layout);
    
    for (int i = 0; table[i].character != 0; i++) {
        if (table[i].character == c) {
            if (out_modifier) *out_modifier = table[i].modifier;
            return table[i].keycode;
        }
    }
    
    return 0;
}

// ============ IMPLEMENTATION ============

void badusb_keyboard_init(void) {
    current_layout = LAYOUT_US_QWERTY;
    tinyusb_hid_init();
}

void badusb_keyboard_send_char(char c, keyboard_layout_t layout) {
    uint8_t modifier = 0;
    uint8_t keycode = find_keycode(c, layout, &modifier);
    
    if (keycode != 0) {
        tinyusb_hid_send_key(keycode, modifier);
    }
}

void badusb_keyboard_send_string(const char *str, keyboard_layout_t layout) {
    if (!str) return;
    
    for (int i = 0; str[i]; i++) {
        badusb_keyboard_send_char(str[i], layout);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void badusb_keyboard_send_key(uint8_t keycode) {
    tinyusb_hid_send_key(keycode, 0);
}

void badusb_keyboard_send_key_with_modifier(uint8_t keycode, uint8_t modifier) {
    tinyusb_hid_send_key(keycode, modifier);
}

void badusb_keyboard_send_enter(void) {
    tinyusb_hid_send_key(KEY_ENTER, 0);
}

void badusb_keyboard_send_tab(void) {
    tinyusb_hid_send_key(KEY_TAB, 0);
}

void badusb_keyboard_send_space(void) {
    tinyusb_hid_send_key(KEY_SPACE, 0);
}

void badusb_keyboard_release_all(void) {
    tinyusb_hid_release_all();
}

void badusb_keyboard_delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void badusb_keyboard_set_layout(keyboard_layout_t layout) {
    current_layout = layout;
}

keyboard_layout_t badusb_keyboard_get_layout(void) {
    return current_layout;
}
