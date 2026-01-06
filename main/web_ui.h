/* Armageddon Framework v3 - web_ui.h
 * Routes HTTP et API endpoints
 */

#ifndef WEB_UI_H
#define WEB_UI_H

#include <esp_err.h>
#include <esp_http_server.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ FUNCTION DECLARATIONS ============

/* Initialize HTTP server */
esp_err_t web_ui_init(void);

/* Start HTTP server */
esp_err_t web_ui_start(void);

/* Stop HTTP server */
esp_err_t web_ui_stop(void);

/* Get HTTP server handle */
httpd_handle_t web_ui_get_server(void);

#ifdef __cplusplus
}
#endif

#endif /* WEB_UI_H */
