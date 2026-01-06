/* Armageddon Framework v3 - logger.cpp
 * Implémentation du système de logging
 */

#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// ============ LOG BUFFER ============
#define LOG_BUFFER_SIZE 8192
static char log_buffer[LOG_BUFFER_SIZE] = {0};
static uint16_t log_buffer_pos = 0;
static SemaphoreHandle_t log_mutex = NULL;

// ============ INITIALIZATION ============
void logger_init(void) {
    log_mutex = xSemaphoreCreateMutex();
    memset(log_buffer, 0, sizeof(log_buffer));
    log_buffer_pos = 0;
}

// ============ LOGGING FUNCTION ============
void logger_log(log_level_t level, const char *tag, const char *fmt, ...) {
    if (!log_mutex) {
        return;
    }
    
    // Take mutex
    if (xSemaphoreTake(log_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return;
    }
    
    // Build timestamp
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    // Level string
    const char *level_str[] = {"[D]", "[I]", "[W]", "[E]"};
    const char *lstr = (level < 4) ? level_str[level] : "[?]";
    
    // Format message
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d %s [%s] ",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, lstr, tag);
    
    // Prepare formatted message
    char message[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    
    // Print to UART
    printf("%s%s\n", timestamp, message);
    
    // Add to buffer (with ring buffer wrap)
    uint16_t total_len = strlen(timestamp) + strlen(message) + 1;
    
    if (log_buffer_pos + total_len >= LOG_BUFFER_SIZE) {
        // Wrap around
        memset(log_buffer, 0, sizeof(log_buffer));
        log_buffer_pos = 0;
    }
    
    // Append to buffer
    snprintf(log_buffer + log_buffer_pos, 
             LOG_BUFFER_SIZE - log_buffer_pos,
             "%s%s\n", timestamp, message);
    log_buffer_pos += total_len;
    
    // Release mutex
    xSemaphoreGive(log_mutex);
}

// ============ BUFFER ACCESS ============
const char *logger_get_buffer(void) {
    return log_buffer;
}

uint16_t logger_get_buffer_size(void) {
    return log_buffer_pos;
}

void logger_clear_buffer(void) {
    if (xSemaphoreTake(log_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        memset(log_buffer, 0, sizeof(log_buffer));
        log_buffer_pos = 0;
        xSemaphoreGive(log_mutex);
    }
}
