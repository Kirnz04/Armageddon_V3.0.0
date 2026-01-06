/* Armageddon Framework v3 - badusb_ducky.cpp
 * Implémentation DuckyScript Parser complet
 */

#include "badusb_ducky.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static const char *TAG = "DUCKY";

// ============ KEY NAME TO KEYCODE MAPPING ============
typedef struct {
    const char *name;
    uint8_t keycode;
} keycode_mapping_t;

static const keycode_mapping_t keycode_map[] = {
    {"ENTER", 0x28},
    {"RETURN", 0x28},
    {"BACKSPACE", 0x2A},
    {"DELETE", 0x4C},
    {"TAB", 0x2B},
    {"SPACE", 0x2C},
    {"ESC", 0x29},
    {"ESCAPE", 0x29},
    {"HOME", 0x4A},
    {"END", 0x4D},
    {"PAGEUP", 0x4B},
    {"PAGEDOWN", 0x4E},
    {"UP", 0x52},
    {"DOWN", 0x51},
    {"LEFT", 0x50},
    {"RIGHT", 0x4F},
    {"INSERT", 0x49},
    {"CAPSLOCK", 0x39},
    {"F1", 0x3A},
    {"F2", 0x3B},
    {"F3", 0x3C},
    {"F4", 0x3D},
    {"F5", 0x3E},
    {"F6", 0x3F},
    {"F7", 0x40},
    {"F8", 0x41},
    {"F9", 0x42},
    {"F10", 0x43},
    {"F11", 0x44},
    {"F12", 0x45},
    {NULL, 0}
};

// ============ HELPER FUNCTIONS ============

static void trim_line(char *line) {
    while (*line && isspace(*line)) line++;
    char *end = line + strlen(line) - 1;
    while (end > line && isspace(*end)) *(end--) = '\0';
}

static bool is_comment_line(const char *line) {
    while (*line && isspace(*line)) line++;
    return (*line == '#' || (line[0] == 'R' && line[1] == 'E' && line[2] == 'M'));
}

static bool is_empty_line(const char *line) {
    while (*line && isspace(*line)) line++;
    return (*line == '\0');
}

// ============ MODIFIER PARSING ============
ducky_modifier_t ducky_parse_modifier(const char *mod_str) {
    if (!mod_str) return MOD_CMD_NONE;
    
    int modifier = MOD_CMD_NONE;
    char mod_copy[256];
    strncpy(mod_copy, mod_str, sizeof(mod_copy) - 1);
    
    // Convert to uppercase
    for (int i = 0; mod_copy[i]; i++) {
        mod_copy[i] = toupper(mod_copy[i]);
    }
    
    if (strstr(mod_copy, "CTRL")) modifier |= MOD_CMD_CTRL;
    if (strstr(mod_copy, "ALT")) modifier |= MOD_CMD_ALT;
    if (strstr(mod_copy, "SHIFT")) modifier |= MOD_CMD_SHIFT;
    if (strstr(mod_copy, "GUI")) modifier |= MOD_CMD_GUI;
    
    return (ducky_modifier_t)modifier;
}

// ============ KEY PARSING ============
uint8_t ducky_parse_key(const char *key_name) {
    if (!key_name) return 0;
    
    char key_upper[64];
    strncpy(key_upper, key_name, sizeof(key_upper) - 1);
    for (int i = 0; key_upper[i]; i++) {
        key_upper[i] = toupper(key_upper[i]);
    }
    
    // Search in keycode map
    for (int i = 0; keycode_map[i].name != NULL; i++) {
        if (strcmp(keycode_map[i].name, key_upper) == 0) {
            return keycode_map[i].keycode;
        }
    }
    
    return 0;
}

// ============ PARSER IMPLEMENTATION ============

esp_err_t ducky_parser_init(ducky_parser_t *parser, const char *script, uint16_t length) {
    if (!parser || !script) {
        return ESP_ERR_INVALID_ARG;
    }
    
    parser->script = script;
    parser->script_length = length;
    parser->current_line = 0;
    parser->line_pos = 0;
    parser->parsing = true;
    
    memset(&parser->parsed_cmd, 0, sizeof(ducky_cmd_t));
    
    return ESP_OK;
}

esp_err_t ducky_parser_next(ducky_parser_t *parser, ducky_cmd_t *cmd) {
    if (!parser || !cmd || !parser->parsing) {
        return ESP_ERR_INVALID_STATE;
    }
    
    memset(cmd, 0, sizeof(ducky_cmd_t));
    cmd->type = DUCKY_CMD_NONE;
    
    // Extract next line
    static char line_buffer[512];
    uint16_t line_idx = 0;
    const char *pos = parser->script + parser->line_pos;
    
    // Skip to start of next line
    while (parser->line_pos < parser->script_length && 
           *pos != '\n' && *pos != '\r') {
        line_buffer[line_idx++] = *pos++;
        parser->line_pos++;
    }
    
    // Skip newline characters
    while (parser->line_pos < parser->script_length && 
           (*pos == '\n' || *pos == '\r')) {
        pos++;
        parser->line_pos++;
    }
    
    line_buffer[line_idx] = '\0';
    trim_line(line_buffer);
    parser->current_line++;
    
    // Check end of script
    if (parser->line_pos >= parser->script_length) {
        parser->parsing = false;
        return ESP_ERR_NOT_FINISHED;
    }
    
    // Skip empty and comment lines
    if (is_empty_line(line_buffer) || is_comment_line(line_buffer)) {
        return ducky_parser_next(parser, cmd);  // Recursive: get next valid line
    }
    
    // Parse command
    char cmd_name[256];
    char cmd_arg[256];
    sscanf(line_buffer, "%255s %255[^\n]", cmd_name, cmd_arg);
    
    // Convert to uppercase
    for (int i = 0; cmd_name[i]; i++) {
        cmd_name[i] = toupper(cmd_name[i]);
    }
    
    // ============ COMMAND PARSING ============
    
    if (strcmp(cmd_name, "STRING") == 0) {
        cmd->type = DUCKY_CMD_STRING;
        cmd->data.string.text = (char *)malloc(strlen(cmd_arg) + 1);
        strcpy(cmd->data.string.text, cmd_arg);
        cmd->data.string.length = strlen(cmd_arg);
    }
    else if (strcmp(cmd_name, "DELAY") == 0) {
        cmd->type = DUCKY_CMD_DELAY;
        cmd->data.delay.delay_ms = atoi(cmd_arg);
    }
    else if (strcmp(cmd_name, "ENTER") == 0 || strcmp(cmd_name, "RETURN") == 0) {
        cmd->type = DUCKY_CMD_ENTER;
    }
    else if (strcmp(cmd_name, "TAB") == 0) {
        cmd->type = DUCKY_CMD_TAB;
    }
    else if (strcmp(cmd_name, "SPACE") == 0) {
        cmd->type = DUCKY_CMD_SPACE;
    }
    else if (strcmp(cmd_name, "REM") == 0) {
        cmd->type = DUCKY_CMD_REM;
    }
    else if (strcmp(cmd_name, "CTRL") == 0 || strcmp(cmd_name, "CONTROL") == 0 ||
             strcmp(cmd_name, "ALT") == 0 || strcmp(cmd_name, "SHIFT") == 0 ||
             strcmp(cmd_name, "GUI") == 0 || strcmp(cmd_name, "WINDOWS") == 0) {
        // Modifier key command
        cmd->type = DUCKY_CMD_MODIFIER;
        cmd->data.modifier_key.modifier = ducky_parse_modifier(line_buffer);
        
        // Extract key name from remaining line
        char *key_pos = strstr(line_buffer, " ");
        if (key_pos) {
            while (*key_pos && isspace(*key_pos)) key_pos++;
            cmd->data.modifier_key.key_name = (char *)malloc(strlen(key_pos) + 1);
            strcpy(cmd->data.modifier_key.key_name, key_pos);
            cmd->data.modifier_key.key_length = strlen(key_pos);
        }
    }
    else {
        // Try as single key command
        cmd->type = DUCKY_CMD_KEY;
        cmd->data.key.key_name = (char *)malloc(strlen(cmd_name) + 1);
        strcpy(cmd->data.key.key_name, cmd_name);
        cmd->data.key.key_length = strlen(cmd_name);
    }
    
    return ESP_OK;
}

const char *ducky_cmd_to_string(const ducky_cmd_t *cmd) {
    static char buffer[256];
    
    switch (cmd->type) {
        case DUCKY_CMD_STRING:
            snprintf(buffer, sizeof(buffer), "STRING: %s", cmd->data.string.text);
            break;
        case DUCKY_CMD_DELAY:
            snprintf(buffer, sizeof(buffer), "DELAY: %ld ms", cmd->data.delay.delay_ms);
            break;
        case DUCKY_CMD_ENTER:
            snprintf(buffer, sizeof(buffer), "ENTER");
            break;
        case DUCKY_CMD_KEY:
            snprintf(buffer, sizeof(buffer), "KEY: %s", cmd->data.key.key_name);
            break;
        case DUCKY_CMD_MODIFIER:
            snprintf(buffer, sizeof(buffer), "MOD+KEY: 0x%02X %s", 
                     cmd->data.modifier_key.modifier, cmd->data.modifier_key.key_name);
            break;
        case DUCKY_CMD_TAB:
            snprintf(buffer, sizeof(buffer), "TAB");
            break;
        case DUCKY_CMD_SPACE:
            snprintf(buffer, sizeof(buffer), "SPACE");
            break;
        case DUCKY_CMD_REM:
            snprintf(buffer, sizeof(buffer), "REM");
            break;
        default:
            snprintf(buffer, sizeof(buffer), "UNKNOWN");
            break;
    }
    
    return buffer;
}

void ducky_parser_reset(ducky_parser_t *parser) {
    if (parser) {
        parser->current_line = 0;
        parser->line_pos = 0;
        parser->parsing = true;
        memset(&parser->parsed_cmd, 0, sizeof(ducky_cmd_t));
    }
}

uint16_t ducky_parser_get_command_count(ducky_parser_t *parser) {
    if (!parser) return 0;
    
    uint16_t count = 0;
    uint16_t saved_pos = parser->line_pos;
    bool saved_parsing = parser->parsing;
    
    ducky_parser_reset(parser);
    
    ducky_cmd_t dummy;
    while (ducky_parser_next(parser, &dummy) == ESP_OK) {
        count++;
    }
    
    parser->line_pos = saved_pos;
    parser->parsing = saved_parsing;
    
    return count;
}
