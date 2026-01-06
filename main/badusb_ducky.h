/* Armageddon Framework v3 - badusb_ducky.h
 * DuckyScript Parser - Support complet des commandes
 */

#ifndef BADUSB_DUCKY_H
#define BADUSB_DUCKY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ DUCKY COMMAND TYPES ============
typedef enum {
    DUCKY_CMD_NONE = 0,
    DUCKY_CMD_STRING = 1,
    DUCKY_CMD_DELAY = 2,
    DUCKY_CMD_KEY = 3,
    DUCKY_CMD_MODIFIER = 4,
    DUCKY_CMD_ENTER = 5,
    DUCKY_CMD_REM = 6,
    DUCKY_CMD_TAB = 7,
    DUCKY_CMD_SPACE = 8
} ducky_cmd_type_t;

// ============ MODIFIER COMMANDS ============
typedef enum {
    MOD_CMD_NONE = 0,
    MOD_CMD_CTRL = 1,
    MOD_CMD_ALT = 2,
    MOD_CMD_SHIFT = 4,
    MOD_CMD_GUI = 8,
    MOD_CMD_CTRL_ALT = 3,      // CTRL + ALT
    MOD_CMD_CTRL_SHIFT = 5,    // CTRL + SHIFT
    MOD_CMD_CTRL_GUI = 9,      // CTRL + GUI
    MOD_CMD_ALT_SHIFT = 6,     // ALT + SHIFT
    MOD_CMD_ALT_GUI = 10,      // ALT + GUI
    MOD_CMD_SHIFT_GUI = 12,    // SHIFT + GUI
    MOD_CMD_CTRL_ALT_SHIFT = 7, // CTRL + ALT + SHIFT
    MOD_CMD_CTRL_ALT_GUI = 11,  // CTRL + ALT + GUI
} ducky_modifier_t;

// ============ PARSED COMMAND STRUCTURE ============
typedef struct {
    ducky_cmd_type_t type;
    union {
        struct {
            char *text;
            uint16_t length;
        } string;
        struct {
            uint32_t delay_ms;
        } delay;
        struct {
            char *key_name;
            uint16_t key_length;
        } key;
        struct {
            ducky_modifier_t modifier;
            char *key_name;
            uint16_t key_length;
        } modifier_key;
    } data;
} ducky_cmd_t;

// ============ PARSER STATE ============
typedef struct {
    const char *script;         // DuckyScript source
    uint16_t script_length;     // Script length
    uint16_t current_line;      // Current line number
    uint16_t line_pos;          // Position in current line
    ducky_cmd_t parsed_cmd;     // Last parsed command
    bool parsing;               // Parsing in progress
} ducky_parser_t;

// ============ FUNCTION DECLARATIONS ============

/* Initialize parser with DuckyScript source */
esp_err_t ducky_parser_init(ducky_parser_t *parser, const char *script, uint16_t length);

/* Parse next command from script */
esp_err_t ducky_parser_next(ducky_parser_t *parser, ducky_cmd_t *cmd);

/* Get command string representation (for debugging) */
const char *ducky_cmd_to_string(const ducky_cmd_t *cmd);

/* Helper: Parse modifier string (CTRL, ALT, SHIFT, GUI, etc.) */
ducky_modifier_t ducky_parse_modifier(const char *mod_str);

/* Helper: Parse key name to keycode */
uint8_t ducky_parse_key(const char *key_name);

/* Reset parser to beginning */
void ducky_parser_reset(ducky_parser_t *parser);

/* Get total number of commands */
uint16_t ducky_parser_get_command_count(ducky_parser_t *parser);

#ifdef __cplusplus
}
#endif

#endif /* BADUSB_DUCKY_H */
