/* Armageddon Framework v3 - logger.h
 * Système de logging centralisé avec timestamps
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============ LOG LEVELS ============
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3
} log_level_t;

#define LOG_DEBUG(...) logger_log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) logger_log(LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_WARN(...) logger_log(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) logger_log(LOG_LEVEL_ERROR, __VA_ARGS__)

// ============ FUNCTION DECLARATIONS ============

/* Initialize logger */
void logger_init(void);

/* Log message with level */
void logger_log(log_level_t level, const char *tag, const char *fmt, ...);

/* Get log buffer (for web UI streaming) */
const char *logger_get_buffer(void);

/* Get current log buffer size */
uint16_t logger_get_buffer_size(void);

/* Clear log buffer */
void logger_clear_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H */
