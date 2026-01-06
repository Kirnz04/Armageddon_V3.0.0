# Armageddon Framework v3.0.0 - Build Status & Fixes

## Current Build Status
**Status**: Compilation errors found, requires code fixes

## Issues Found

### 1. **TinyUSB Component Integration** ⚠️
- **Problem**: TinyUSB is not available as a standard ESP-IDF v5.3 component
- **Solution**: Added git dependency to `idf_component.yml`:
  ```yaml
  tinyusb:
    version: "1.4.1"
    git: https://github.com/espressif/esp-tinyusb.git
  ```

### 2. **badusb_keyboard.cpp - Type Narrowing** 
- **Lines 70-78**: Integer literals (50089, 50088, etc.) cannot be converted to `char`
- **Fix**: Use `uint16_t` for these values instead of `char`

### 3. **web_ui_html.h - UTF-8 Encoding**
- **Problem**: Special characters (✓, ✗, ⚡, ▶) in C++ string literals cause compiler errors
- **Fix**: Replace with ASCII equivalents or use proper Unicode escapes
  - `✓` → `[OK]`
  - `✗` → `[FAIL]`
  - `⚡` → `[!]`
  - `▶` → `[>]`

### 4. **tinyusb_descriptors.cpp - Array Size**
- **Line 105**: Array has too many initializers for declared size
- **Fix**: Check descriptor array size matches number of initializers

## Build Commands

### Environment Setup
```bash
cd "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0"
source ~/esp/esp-idf/export.sh
```

### Clean Build
```bash
idf.py fullclean
idf.py set-target esp32s3
idf.py build
```

### Flash to Device
```bash
idf.py -p /dev/ttyUSB0 -b 921600 flash monitor
```

## Next Steps

1. **Fix code issues** (narrowing conversions, UTF-8 characters, array sizes)
2. **Rebuild** with `idf.py build`
3. **Flash** to ESP32-S3-N16R8 board
4. **Verify** WiFi AP comes online: `HackESP32` / `redteam123`
5. **Test** web UI at `http://192.168.4.1`

## Tools Available

- **ESP-IDF**: `/Users/sousou04/esp/esp-idf/`
- **Compiler**: `xtensa-esp32s3-elf-g++`
- **Python**: `3.14.2` (via IDF env)

## Troubleshooting

**If compilation fails**:
```bash
idf.py fullclean
rm -rf build sdkconfig
idf.py set-target esp32s3
idf.py build
```

**If tinyusb won't download**:
Check internet connection and git availability:
```bash
git --version
ping github.com
```
