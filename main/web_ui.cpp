/* Armageddon Framework v3 - web_ui.cpp
 * Implémentation des routes HTTP et API
 */

#include "web_ui.h"
#include "web_ui_html.h"
#include "framework.h"
#include "wifi_attacks.h"
#include "badusb_ducky.h"
#include "badusb_keyboard.h"
#include "logger.h"
#include <cjson/cJSON.h>
#include <string.h>
#include <stdio.h>

static const char *TAG = "WEB_UI";
static httpd_handle_t server = NULL;

// ============ HTTP HANDLERS ============

/* GET / - Main page */
static esp_err_t handler_get_index(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, INDEX_HTML, strlen(INDEX_HTML));
    return ESP_OK;
}

/* GET /scan - WiFi scan results (JSON) */
static esp_err_t handler_get_scan(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    
    uint16_t count = wifi_scan_get_count();
    cJSON *root = cJSON_CreateObject();
    cJSON *networks = cJSON_CreateArray();
    
    for (uint16_t i = 0; i < count; i++) {
        char ssid[33] = {0};
        uint8_t bssid[6] = {0};
        int8_t rssi = 0;
        uint8_t channel = 0;
        bool secure = false;
        
        wifi_scan_get_network(i, ssid, bssid, &rssi, &channel, &secure);
        
        cJSON *net = cJSON_CreateObject();
        cJSON_AddStringToObject(net, "ssid", ssid);
        cJSON_AddNumberToObject(net, "rssi", rssi);
        cJSON_AddNumberToObject(net, "channel", channel);
        cJSON_AddBoolToObject(net, "secure", secure);
        
        char bssid_str[18];
        snprintf(bssid_str, sizeof(bssid_str), "%02x:%02x:%02x:%02x:%02x:%02x",
                 bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
        cJSON_AddStringToObject(net, "bssid", bssid_str);
        
        cJSON_AddItemToArray(networks, net);
    }
    
    cJSON_AddItemToObject(root, "networks", networks);
    cJSON_AddNumberToObject(root, "count", count);
    
    const char *json_str = cJSON_Print(root);
    httpd_resp_send(req, json_str, strlen(json_str));
    
    free((void *)json_str);
    cJSON_Delete(root);
    
    return ESP_OK;
}

/* POST /badusb - Execute DuckyScript */
static esp_err_t handler_post_badusb(httpd_req_t *req) {
    char buffer[4096] = {0};
    int total_len = httpd_req_recv(req, buffer, sizeof(buffer) - 1);
    
    if (total_len <= 0) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    
    buffer[total_len] = '\0';
    
    // Parse JSON
    cJSON *root = cJSON_Parse(buffer);
    if (!root) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    
    cJSON *script_item = cJSON_GetObjectItem(root, "script");
    cJSON *layout_item = cJSON_GetObjectItem(root, "layout");
    
    const char *script = script_item ? script_item->valuestring : "";
    int layout = layout_item ? layout_item->valueint : 0;
    
    // Initialize parser
    ducky_parser_t parser;
    ducky_parser_init(&parser, script, strlen(script));
    
    // Set keyboard layout
    badusb_keyboard_set_layout((keyboard_layout_t)layout);
    
    // Execute script
    ducky_cmd_t cmd;
    while (ducky_parser_next(&parser, &cmd) == ESP_OK) {
        switch (cmd.type) {
            case DUCKY_CMD_STRING:
                badusb_keyboard_send_string(cmd.data.string.text, (keyboard_layout_t)layout);
                break;
            case DUCKY_CMD_DELAY:
                badusb_keyboard_delay(cmd.data.delay.delay_ms);
                break;
            case DUCKY_CMD_ENTER:
                badusb_keyboard_send_enter();
                break;
            case DUCKY_CMD_KEY: {
                uint8_t keycode = ducky_parse_key(cmd.data.key.key_name);
                if (keycode) badusb_keyboard_send_key(keycode);
                break;
            }
            case DUCKY_CMD_MODIFIER: {
                uint8_t keycode = ducky_parse_key(cmd.data.modifier_key.key_name);
                if (keycode) {
                    uint8_t modifier = 0;
                    if (cmd.data.modifier_key.modifier & MOD_CMD_CTRL) modifier |= MOD_LCTRL;
                    if (cmd.data.modifier_key.modifier & MOD_CMD_ALT) modifier |= MOD_LALT;
                    if (cmd.data.modifier_key.modifier & MOD_CMD_SHIFT) modifier |= MOD_LSHIFT;
                    if (cmd.data.modifier_key.modifier & MOD_CMD_GUI) modifier |= MOD_LGUI;
                    badusb_keyboard_send_key_with_modifier(keycode, modifier);
                }
                break;
            }
            case DUCKY_CMD_TAB:
                badusb_keyboard_send_tab();
                break;
            case DUCKY_CMD_SPACE:
                badusb_keyboard_send_space();
                break;
            case DUCKY_CMD_REM:
            case DUCKY_CMD_NONE:
            default:
                break;
        }
    }
    
    // Response
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddBoolToObject(resp, "success", true);
    
    const char *resp_str = cJSON_Print(resp);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp_str, strlen(resp_str));
    
    free((void *)resp_str);
    cJSON_Delete(resp);
    cJSON_Delete(root);
    
    return ESP_OK;
}

/* POST /deauth - Launch deauth attack */
static esp_err_t handler_post_deauth(httpd_req_t *req) {
    char buffer[512] = {0};
    int total_len = httpd_req_recv(req, buffer, sizeof(buffer) - 1);
    
    if (total_len <= 0) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    
    buffer[total_len] = '\0';
    
    // Parse JSON
    cJSON *root = cJSON_Parse(buffer);
    if (!root) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    
    cJSON *idx_item = cJSON_GetObjectItem(root, "idx");
    int idx = idx_item ? idx_item->valueint : -1;
    
    if (idx < 0 || idx >= wifi_scan_get_count()) {
        cJSON *resp = cJSON_CreateObject();
        cJSON_AddBoolToObject(resp, "success", false);
        cJSON_AddStringToObject(resp, "error", "Invalid target");
        
        const char *resp_str = cJSON_Print(resp);
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, resp_str, strlen(resp_str));
        
        free((void *)resp_str);
        cJSON_Delete(resp);
        cJSON_Delete(root);
        return ESP_OK;
    }
    
    // Get target network info
    char ssid[33] = {0};
    uint8_t bssid[6] = {0};
    int8_t rssi = 0;
    uint8_t channel = 0;
    bool secure = false;
    
    wifi_scan_get_network(idx, ssid, bssid, &rssi, &channel, &secure);
    
    // Start deauth attack
    deauth_params_t params = {
        .channel = channel,
        .duration_ms = 5000
    };
    memcpy(params.target_bssid, bssid, 6);
    
    wifi_deauth_start(&params);
    
    // Wait for attack to complete
    while (wifi_deauth_is_active()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    // Get results
    uint32_t packets_sent = 0;
    bool active = false;
    wifi_deauth_get_status(&packets_sent, &active);
    
    // Response
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddBoolToObject(resp, "success", true);
    cJSON_AddNumberToObject(resp, "packets", packets_sent);
    cJSON_AddStringToObject(resp, "target", ssid);
    
    const char *resp_str = cJSON_Print(resp);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp_str, strlen(resp_str));
    
    free((void *)resp_str);
    cJSON_Delete(resp);
    cJSON_Delete(root);
    
    return ESP_OK;
}

/* GET /logs - Get system logs */
static esp_err_t handler_get_logs(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "logs", logger_get_buffer());
    
    const char *json_str = cJSON_Print(root);
    httpd_resp_send(req, json_str, strlen(json_str));
    
    free((void *)json_str);
    cJSON_Delete(root);
    
    return ESP_OK;
}

// ============ HTTP SERVER ============

esp_err_t web_ui_init(void) {
    ESP_LOGI(TAG, "Initializing HTTP server");
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.max_uri_handlers = 10;
    
    ESP_ERROR_CHECK(httpd_start(&server, &config));
    
    // Register handlers
    httpd_uri_t uri_get_index = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = handler_get_index,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &uri_get_index);
    
    httpd_uri_t uri_scan = {
        .uri = "/scan",
        .method = HTTP_GET,
        .handler = handler_get_scan,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &uri_scan);
    
    httpd_uri_t uri_badusb = {
        .uri = "/badusb",
        .method = HTTP_POST,
        .handler = handler_post_badusb,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &uri_badusb);
    
    httpd_uri_t uri_deauth = {
        .uri = "/deauth",
        .method = HTTP_POST,
        .handler = handler_post_deauth,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &uri_deauth);
    
    httpd_uri_t uri_logs = {
        .uri = "/logs",
        .method = HTTP_GET,
        .handler = handler_get_logs,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &uri_logs);
    
    ESP_LOGI(TAG, "HTTP server started at http://192.168.4.1/");
    
    return ESP_OK;
}

esp_err_t web_ui_start(void) {
    if (server) return ESP_OK;
    return web_ui_init();
}

esp_err_t web_ui_stop(void) {
    if (server) {
        httpd_stop(server);
        server = NULL;
    }
    return ESP_OK;
}

httpd_handle_t web_ui_get_server(void) {
    return server;
}
