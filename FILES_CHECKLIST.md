# 📋 ARMAGEDDON Framework v3.0.0 - Complete Files Checklist

## ✅ FILE PLACEMENT VERIFICATION

Before building, check off each file as you place it in the correct location.

---

## 📁 ROOT DIRECTORY (7 files)

Copy these files from generated folder to **project root**:

```
armageddon_framework_v3/
```

- [ ] **CMakeLists.txt**
  - Source: `CMakeLists_ROOT.txt`
  - Destination: `armageddon_framework_v3/CMakeLists.txt`
  - Purpose: Root project configuration
  - Size: ~150 bytes

- [ ] **sdkconfig**
  - Source: `sdkconfig` (provided)
  - Destination: `armageddon_framework_v3/sdkconfig`
  - Purpose: ESP32-S3-N16R8 hardware configuration
  - Size: ~3 KB

- [ ] **idf_component.yml**
  - Source: `idf_component.yml` (provided)
  - Destination: `armageddon_framework_v3/idf_component.yml`
  - Purpose: Component dependencies manifest
  - Size: ~500 bytes

- [ ] **.gitignore**
  - Source: `.gitignore` (provided)
  - Destination: `armageddon_framework_v3/.gitignore`
  - Purpose: Git ignore patterns
  - Size: ~800 bytes

- [ ] **README.md**
  - Source: `README.md` (provided)
  - Destination: `armageddon_framework_v3/README.md`
  - Purpose: Full project documentation
  - Size: ~15 KB

- [ ] **FILE_ORGANIZATION.md**
  - Source: `FILE_ORGANIZATION.md` (provided)
  - Destination: `armageddon_framework_v3/FILE_ORGANIZATION.md`
  - Purpose: File placement guide
  - Size: ~3 KB

- [ ] **SETUP_GUIDE.md**
  - Source: `SETUP_GUIDE.md` (provided)
  - Destination: `armageddon_framework_v3/SETUP_GUIDE.md`
  - Purpose: Build & installation instructions
  - Size: ~10 KB

---

## 📁 MAIN/ DIRECTORY (24 files)

Copy these files from generated folder to **main/** subdirectory:

```
armageddon_framework_v3/main/
```

### Configuration (1 file)

- [ ] **CMakeLists.txt**
  - Source: `CMakeLists_MAIN.txt`
  - Destination: `armageddon_framework_v3/main/CMakeLists.txt`
  - Purpose: Main component configuration
  - Size: ~700 bytes

### Application Entry Point (1 file)

- [ ] **main.cpp**
  - Source: `main.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/main.cpp`
  - Purpose: app_main() entry point, orchestration
  - Size: ~4 KB

### Framework Core (2 files)

- [ ] **framework.h**
  - Source: `framework.h` (provided)
  - Destination: `armageddon_framework_v3/main/framework.h`
  - Purpose: Global structures & definitions
  - Size: ~2 KB

- [ ] **framework.cpp**
  - Source: `framework.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/framework.cpp`
  - Purpose: Framework initialization
  - Size: ~1 KB

### BadUSB Module (4 files)

- [ ] **badusb_ducky.h**
  - Source: `badusb_ducky.h` (provided)
  - Destination: `armageddon_framework_v3/main/badusb_ducky.h`
  - Purpose: DuckyScript parser declarations
  - Size: ~3 KB

- [ ] **badusb_ducky.cpp**
  - Source: `badusb_ducky.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/badusb_ducky.cpp`
  - Purpose: DuckyScript parser implementation
  - Size: ~8 KB

- [ ] **badusb_keyboard.h**
  - Source: `badusb_keyboard.h` (provided)
  - Destination: `armageddon_framework_v3/main/badusb_keyboard.h`
  - Purpose: Keyboard layout declarations
  - Size: ~2 KB

- [ ] **badusb_keyboard.cpp**
  - Source: `badusb_keyboard.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/badusb_keyboard.cpp`
  - Purpose: US/FR/DE keyboard layouts with accents
  - Size: ~12 KB

### TinyUSB HID Module (5 files)

- [ ] **tusb_config.h**
  - Source: `tusb_config.h` (provided)
  - Destination: `armageddon_framework_v3/main/tusb_config.h`
  - Purpose: TinyUSB device configuration
  - Size: ~3 KB

- [ ] **tinyusb_descriptors.h**
  - Source: `tinyusb_descriptors.h` (provided)
  - Destination: `armageddon_framework_v3/main/tinyusb_descriptors.h`
  - Purpose: USB descriptors declarations
  - Size: ~2 KB

- [ ] **tinyusb_descriptors.cpp**
  - Source: `tinyusb_descriptors.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/tinyusb_descriptors.cpp`
  - Purpose: USB device, config, HID descriptors
  - Size: ~6 KB

- [ ] **tinyusb_hid_callbacks.h**
  - Source: `tinyusb_hid_callbacks.h` (provided)
  - Destination: `armageddon_framework_v3/main/tinyusb_hid_callbacks.h`
  - Purpose: HID callback declarations
  - Size: ~1.5 KB

- [ ] **tinyusb_hid_callbacks.cpp**
  - Source: `tinyusb_hid_callbacks.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/tinyusb_hid_callbacks.cpp`
  - Purpose: HID initialization, mount/unmount, report send
  - Size: ~4 KB

### WiFi Module (4 files)

- [ ] **wifi_init.h**
  - Source: `wifi_init.h` (provided)
  - Destination: `armageddon_framework_v3/main/wifi_init.h`
  - Purpose: WiFi initialization declarations
  - Size: ~2 KB

- [ ] **wifi_init.cpp**
  - Source: `wifi_init.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/wifi_init.cpp`
  - Purpose: WiFi AP/STA setup, event handlers
  - Size: ~8 KB

- [ ] **wifi_attacks.h**
  - Source: `wifi_attacks.h` (provided)
  - Destination: `armageddon_framework_v3/main/wifi_attacks.h`
  - Purpose: WiFi scan & deauth declarations
  - Size: ~2 KB

- [ ] **wifi_attacks.cpp**
  - Source: `wifi_attacks.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/wifi_attacks.cpp`
  - Purpose: Scan networks, launch deauth attacks
  - Size: ~10 KB

### Web UI Module (3 files)

- [ ] **web_ui.h**
  - Source: `web_ui.h` (provided)
  - Destination: `armageddon_framework_v3/main/web_ui.h`
  - Purpose: HTTP server declarations
  - Size: ~1 KB

- [ ] **web_ui.cpp**
  - Source: `web_ui.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/web_ui.cpp`
  - Purpose: HTTP routes, API endpoints
  - Size: ~15 KB

- [ ] **web_ui_html.h**
  - Source: `web_ui_html.h` (provided)
  - Destination: `armageddon_framework_v3/main/web_ui_html.h`
  - Purpose: Embedded HTML/CSS/JS interface
  - Size: ~25 KB

### Utilities (4 files)

- [ ] **logger.h**
  - Source: `logger.h` (provided)
  - Destination: `armageddon_framework_v3/main/logger.h`
  - Purpose: Logging system declarations
  - Size: ~2 KB

- [ ] **logger.cpp**
  - Source: `logger.cpp` (provided - from earlier note)
  - Destination: `armageddon_framework_v3/main/logger.cpp`
  - Purpose: Logging with timestamps, buffer
  - Size: ~5 KB

- [ ] **config.h**
  - Source: `config.h` (provided)
  - Destination: `armageddon_framework_v3/main/config.h`
  - Purpose: Configuration & NVS declarations
  - Size: ~2 KB

- [ ] **config.cpp**
  - Source: `config.cpp` (provided)
  - Destination: `armageddon_framework_v3/main/config.cpp`
  - Purpose: Configuration load/save from NVS
  - Size: ~4 KB

---

## 📊 SUMMARY TABLE

| Category | Files | Location | Total Size |
|----------|-------|----------|-----------|
| Root config | 1 | root/ | ~150 B |
| Root data | 1 | root/ | ~3 KB |
| Root manifests | 2 | root/ | ~1.3 KB |
| Root docs | 3 | root/ | ~28 KB |
| Main config | 1 | main/ | ~700 B |
| Main entry | 1 | main/ | ~4 KB |
| Framework | 2 | main/ | ~3 KB |
| BadUSB | 4 | main/ | ~25 KB |
| TinyUSB | 5 | main/ | ~16.5 KB |
| WiFi | 4 | main/ | ~20 KB |
| Web UI | 3 | main/ | ~40 KB |
| Utils | 4 | main/ | ~13 KB |
| **TOTAL** | **31** | **root + main/** | **~154 KB** |

---

## ✅ VERIFICATION STEPS

### Step 1: Count Files
```bash
# Root directory should have 7 files
ls -la armageddon_framework_v3/ | grep -v "^d" | wc -l
# Expected: ~9 (7 files + . + ..)

# main directory should have 24 files
ls -la armageddon_framework_v3/main/ | grep -v "^d" | wc -l
# Expected: ~26 (24 files + . + ..)
```

### Step 2: Check File Names
```bash
# List all source files
ls armageddon_framework_v3/main/*.cpp armageddon_framework_v3/main/*.h
# Should show 24 files
```

### Step 3: Verify CMakeLists.txt
```bash
# Check root CMakeLists.txt
grep "project(armageddon_framework_v3" armageddon_framework_v3/CMakeLists.txt
# Should match

# Check main CMakeLists.txt
grep "idf_component_register" armageddon_framework_v3/main/CMakeLists.txt
# Should match
```

### Step 4: Pre-build Test
```bash
cd armageddon_framework_v3
idf.py validate
# Should show no errors
```

---

## 📋 FINAL CHECKLIST

Before building, verify:

- [ ] All 7 root files present
- [ ] All 24 main/ files present
- [ ] CMakeLists.txt files renamed correctly
- [ ] File structure matches ESP-IDF standard
- [ ] sdkconfig is valid (not corrupted)
- [ ] idf_component.yml has correct syntax
- [ ] .gitignore is present
- [ ] Documentation files readable
- [ ] No duplicate files
- [ ] File permissions are correct (644 for text)

---

## 🚀 READY TO BUILD!

Once all files are checked off, run:

```bash
cd armageddon_framework_v3
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## 📞 TROUBLESHOOTING

**Files not found during build?**
- Verify file names match exactly (case-sensitive)
- Check CMakeLists.txt SRCS list
- Run `idf.py fullclean` and retry

**Compilation errors?**
- Ensure all .h files are present
- Check #include paths in .cpp files
- Verify idf_component.yml dependencies

**Flash fails?**
- Verify USB connection
- Check `/dev/ttyUSB*` exists
- Try different baud rate: `idf.py -p /dev/ttyUSB0 -b 921600 flash`

---

**All 31 files organized and ready!** ✅
