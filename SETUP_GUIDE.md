# 🚀 Armageddon Framework v3.0.0 - Setup & Build Guide

## ✅ Prerequisites

### System Requirements
- **Linux/macOS/Windows (with WSL2)**
- **ESP-IDF v5.3+** - [Installation Guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/index.html)
- **Python 3.7+** (for ESP-IDF tools)
- **USB-C cable** for flashing

### Hardware
- **ESP32-S3-N16R8** development board
- USB-C port for flashing & HID

---

## 📦 Installation Steps

### 1. **Set Up ESP-IDF** (if not already done)

```bash
# Clone ESP-IDF
git clone -b v5.3 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
source export.sh
cd ..
```

### 2. **Create Project Directory**

```bash
mkdir armageddon_framework_v3
cd armageddon_framework_v3
git init
```

### 3. **Create Directory Structure**

```bash
mkdir -p main
```

### 4. **Place Files**

Copy these files to root directory:
```
CMakeLists.txt          (rename from CMakeLists_ROOT.txt)
sdkconfig
idf_component.yml
.gitignore
README.md
SETUP_GUIDE.md (this file)
FILE_ORGANIZATION.md
```

Copy all 27 source files to `main/` directory:
```
main/CMakeLists.txt     (rename from CMakeLists_MAIN.txt)
main/main.cpp
main/framework.h
main/framework.cpp
main/tusb_config.h
main/tinyusb_descriptors.h
main/tinyusb_descriptors.cpp
main/tinyusb_hid_callbacks.h
main/tinyusb_hid_callbacks.cpp
main/badusb_ducky.h
main/badusb_ducky.cpp
main/badusb_keyboard.h
main/badusb_keyboard.cpp
main/wifi_init.h
main/wifi_init.cpp
main/wifi_attacks.h
main/wifi_attacks.cpp
main/web_ui.h
main/web_ui.cpp
main/web_ui_html.h
main/logger.h
main/logger.cpp
main/config.h
main/config.cpp
```

### 5. **Verify Directory Structure**

```bash
tree -L 2
# Should show:
# armageddon_framework_v3/
# ├── CMakeLists.txt
# ├── sdkconfig
# ├── idf_component.yml
# ├── .gitignore
# ├── README.md
# └── main/
#     ├── CMakeLists.txt
#     ├── main.cpp
#     ├── ... (26 more files)
```

---

## 🔨 Building

### Step 1: Set Target

```bash
idf.py set-target esp32s3
```

### Step 2: Configure (Optional)

```bash
idf.py menuconfig
# Navigate to:
# Component config → USB Device (TinyUSB) → verify enabled
# Component config → ESP32S3 specific → verify PSRAM settings
# Then save & exit
```

### Step 3: Build

```bash
idf.py build
```

**Expected output:**
```
...
[100%] Built target app.elf
esptool.py esp32s3 image_info build/esp-idf/app.bin
Checking image...
...
To flash, run:
  esptool.py --chip esp32s3 -p PORT ...
```

---

## 💾 Flashing

### Step 1: Connect Board

```bash
# Find USB device
ls /dev/tty.USB* or /dev/ttyUSB*
```

### Step 2: Flash & Monitor

```bash
idf.py -p /dev/ttyUSB0 flash monitor
# Or on macOS:
idf.py -p /dev/tty.SLAB_USBtoUART flash monitor
```

**Expected boot sequence:**
```
rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
...
[1/8] Initializing NVS (Flash persistence)
[2/8] Initializing Logger
[3/8] Initializing Framework
[4/8] Initializing TinyUSB HID
[5/8] Initializing Keyboard Module
[6/8] Initializing WiFi (APSTA mode)
[7/8] Initializing WiFi Attacks
[8/8] Starting HTTP Web Server

╔════════════════════════════════════╗
║  ✓ ALL SYSTEMS ONLINE              ║
║  AP: HackESP32 / redteam123        ║
║  Web: 192.168.4.1                  ║
║  Ready for engagement              ║
╚════════════════════════════════════╝
```

**Exit monitor:** `Ctrl+]`

---

## ✅ Post-Flash Verification

### 1. **Check Serial Output**

```bash
idf.py monitor
# Should see initialization messages
# Press Ctrl+] to exit
```

### 2. **Verify WiFi AP**

```bash
# From another device:
# WiFi networks → Look for "HackESP32"
# Connect with password: redteam123
```

### 3. **Test Web UI**

```bash
# Open browser:
# http://192.168.4.1
# Should see dark mode cyberpunk interface with 3 tabs
```

### 4. **Test BadUSB**

```bash
# Connect USB-C to target machine
# ESP32-S3 should enumerate as HID Keyboard
# (Windows: Device Manager → Human Interface Devices → HID Keyboard)
```

### 5. **Test WiFi Scan**

```bash
# In Web UI, WiFi tab:
# Click "SCAN NETWORKS"
# Should list available WiFi networks in ~3 seconds
```

---

## 🐛 Troubleshooting

### **Build Fails with "tusb.h not found"**
```bash
# Verify tinyusb component is available
idf.py add-dependency esp_tinyusb
idf.py fullclean
idf.py build
```

### **ESP32 Not Detected**
```bash
# Check connection
ls -la /dev/ttyUSB*

# Install USB drivers (CH340/FTDI depending on board)
# Try with explicit baud rate
idf.py -p /dev/ttyUSB0 -b 921600 flash monitor
```

### **Web UI Blank/404**
```bash
# Check logs in serial monitor
idf.py monitor

# Should show:
# [WEB_UI] HTTP server started at http://192.168.4.1/
```

### **WiFi AP Won't Start**
```bash
# Verify WiFi credentials in framework.h
#define AP_SSID "HackESP32"
#define AP_PASSWORD "redteam123"

# Check logs for WiFi initialization errors
```

### **Accents Not Working**
```bash
# Verify layout is FR AZERTY (index 1)
# Check web UI dropdown selection
# Verify badusb_keyboard.cpp has FR layout mappings
```

---

## 📊 Build Output

Expected file sizes:
```
build/esp-idf/app.elf      ~500-600 KB
build/esp-idf/app.bin      ~300-350 KB
```

Expected memory usage:
- **Flash:** ~40% (6-7 MB of 16 MB)
- **PSRAM:** ~20-30% during operation

---

## 🔄 Clean Build

If you encounter issues:

```bash
idf.py fullclean
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## 📝 Next Steps

1. ✅ Build & flash successful?
2. ✅ Connect to HackESP32 WiFi AP
3. ✅ Open http://192.168.4.1 in browser
4. ✅ Test BadUSB by typing strings
5. ✅ Test WiFi scan
6. ✅ Test Deauth attack on target network

---

## 🎯 Quick Command Reference

```bash
# View logs
idf.py monitor

# Build only (no flash)
idf.py build

# Flash only (no build)
idf.py -p /dev/ttyUSB0 flash

# Build + Flash + Monitor
idf.py -p /dev/ttyUSB0 flash monitor

# Erase flash memory
idf.py -p /dev/ttyUSB0 erase_flash

# Configure via menuconfig
idf.py menuconfig

# Full clean rebuild
idf.py fullclean && idf.py build

# Size report
idf.py size
```

---

## ✨ Success Checklist

- [ ] Project builds without errors
- [ ] Flash completes successfully
- [ ] Serial monitor shows initialization
- [ ] WiFi AP "HackESP32" appears
- [ ] Web UI loads at 192.168.4.1
- [ ] Network scan finds targets
- [ ] DuckyScript execution works
- [ ] USB HID enumeration on target
- [ ] Deauth attack disconnects clients

---

**Ready to engage! 💀🖤🔥**

For issues, check logs:
```bash
idf.py monitor
```
