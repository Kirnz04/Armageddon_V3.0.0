#pragma once

/* Minimal TinyUSB configuration for esp-idf projects. Add more macros if needed. */

#include "sdkconfig.h"

/* Operate as USB device on RHPORT0 */
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE)

/* Use FreeRTOS as OS layer */
#define CFG_TUSB_OS OPT_OS_FREERTOS

/* Enable debug logs from TinyUSB (0 = off) */
#define CFG_TUSB_DEBUG 0

/* Enable device stack */
#define CFG_TUD_ENABLED 1

/* Enable HID class (keyboard) */
#define CFG_TUD_HID 1

/* Optional: configure HID endpoint buffer size if necessary */
#define CFG_TUD_HID_EP_BUFSIZE 64
