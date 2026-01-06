/* Armageddon Framework v3 - logger.h
 * Système de logging centralisé avec timestamps
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "framework.h"
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_DEBUG(tag, fmt, ...) logger_log(LOG_DEBUG, tag, fmt, __VA_ARGS__)
#define LOG_INFO(tag, fmt, ...) logger_log(LOG_INFO, tag, fmt, __VA_ARGS__)
#define LOG_WARN(tag, fmt, ...) logger_log(LOG_WARN, tag, fmt, __VA_ARGS__)
#define LOG_ERROR(tag, fmt, ...) logger_log(LOG_ERROR, tag, fmt, __VA_ARGS__)

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
