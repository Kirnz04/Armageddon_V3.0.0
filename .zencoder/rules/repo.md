---
description: Repository Information Overview
alwaysApply: true
---

# Armageddon Framework v3.0.0 Information

## Summary
Armageddon Framework v3.0.0 is an ESP32-S3-N16R8 pentest firmware featuring BadUSB HID keyboard emulation, WiFi attack capabilities (network scanning and deauthentication), and a dark mode cyberpunk web interface. It implements a modular architecture with FreeRTOS task coordination and configuration persistence using NVS Flash.

## Structure
```
armageddon_framework_v3/
├── main/                           # Primary application component
│   ├── main.cpp                    # Entry point & system orchestration
│   ├── framework.{h,cpp}           # Core framework & global structures
│   ├── badusb/                     # DuckyScript parsing & keyboard emulation
│   │   ├── badusb_ducky.{h,cpp}
│   │   └── badusb_keyboard.{h,cpp}
│   ├── tinyusb/                    # USB HID device implementation
│   │   ├── tinyusb_descriptors.{h,cpp}
│   │   ├── tinyusb_hid_callbacks.{h,cpp}
│   │   └── tusb_config.h
│   ├── wifi/                       # WiFi AP & attack operations
│   │   ├── wifi_init.{h,cpp}
│   │   └── wifi_attacks.{h,cpp}
│   ├── webui/                      # HTTP server & dark mode interface
│   │   ├── web_ui.{h,cpp}
│   │   └── web_ui_html.h
│   └── utils/                      # Logging & configuration management
│       ├── logger.{h,cpp}
│       └── config.{h,cpp}
├── CMakeLists.txt                  # Root ESP-IDF project config
├── main/CMakeLists.txt             # Main component build configuration
├── idf_component.yml               # IDF component dependencies
├── sdkconfig                       # ESP-IDF build configuration
├── tusb_config.h                   # TinyUSB configuration
└── README.md                       # Comprehensive documentation
```

## Language & Runtime
**Language**: C++ (embedded)  
**Target Platform**: ESP32-S3-N16R8  
**Runtime**: FreeRTOS dual-core (Xtensa 32-bit @ 240MHz)  
**ESP-IDF Version**: v5.3+  
**Build System**: CMake (ESP-IDF toolchain)  
**Package Manager**: ESP-IDF component registry

## Dependencies
**Core ESP-IDF Components**:
- `esp_wifi` - WiFi 2.4GHz 802.11 b/g/n functionality
- `esp_http_server` - HTTP/1.0 web server
- `tinyusb` - USB device HID implementation
- `nvs_flash` - Non-volatile storage for configuration persistence
- `esp_netif` - Network interface abstraction
- `esp_event` - Event loop system
- `lwip` - TCP/IP stack
- `freertos` - Real-time OS kernel
- `driver` - ESP32 peripheral drivers
- `esp_hw_support` - Hardware support utilities

## Build & Installation
```bash
# Set target to ESP32-S3
idf.py set-target esp32s3

# Build firmware
idf.py build

# Flash to device (adjust port as needed)
idf.py -p /dev/ttyUSB0 flash monitor

# Full rebuild if needed
idf.py fullclean && idf.py build
```

**Post-Flash Setup**:
1. Device enumerates as WiFi AP: `HackESP32` / `redteam123`
2. Web UI accessible at `http://192.168.4.1`
3. USB HID device ready for payload injection on connection

## Main Files & Resources

**Entry Point**: `main/main.cpp:1` - Orchestrates firmware initialization and task creation

**Key Configuration**: `main/framework.h` - Global system definitions:
- AP credentials and channel settings
- Deauth attack parameters (burst count, duration)
- Keyboard layout definitions (US QWERTY, FR AZERTY, DE QWERTZ)
- System state enumerations

**Application Entry**: System initialization sequence in main.cpp:
1. NVS Flash initialization for persistent config
2. Logger system setup with ring buffer
3. Framework core initialization
4. TinyUSB HID device enumeration
5. Keyboard module with 3 keyboard layouts
6. WiFi AP + STA mode initialization
7. HTTP web server startup with embedded HTML/CSS/JS

**API Endpoints**:
- `GET /` - Web UI interface
- `GET /scan` - WiFi network discovery (JSON)
- `POST /badusb` - Execute DuckyScript payload
- `POST /deauth` - Launch deauthentication attack
- `GET /logs` - System console logs

## Testing & Validation
**Verification Checklist** (documented in README.md):
- USB HID enumeration on host machine
- DuckyScript execution across 3 keyboard layouts
- WiFi AP connectivity and client association
- Web UI dark mode rendering at 192.168.4.1
- Network scanning finds target SSIDs
- Deauth attack disconnects connected clients
- System logs display all initialization and attack events
- No watchdog timeouts or task starvation
- PSRAM (8MB) properly initialized and utilized

**Validation Method**: Serial monitor logs via `idf.py monitor` confirm initialization sequence and runtime events
