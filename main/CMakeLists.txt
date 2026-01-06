idf_component_register(
    SRCS 
        main.cpp
        framework.cpp
        badusb_ducky.cpp
        badusb_keyboard.cpp
        tinyusb_descriptors.cpp
        tinyusb_hid_callbacks.cpp
        wifi_init.cpp
        wifi_attacks.cpp
        web_ui.cpp
        logger.cpp
        config.cpp
    
    INCLUDE_DIRS 
        .
    
    REQUIRES 
        esp_hw_support
        esp_wifi
        esp_http_server
        nvs_flash
        esp_netif
        esp_event
        lwip
        freertos
        driver
        esp_tinyusb
)

# Integrate tusb config and descriptors into the local `esp_tinyusb` wrapper to avoid reverse-dependency issues
idf_component_get_property(tusb_lib esp_tinyusb COMPONENT_LIB)
if (tusb_lib)
    # Provide our local tusb/tusb_config.h to the esp_tinyusb component
    target_include_directories(${tusb_lib} PRIVATE "${COMPONENT_DIR}/tusb")
    # Also make our descriptor header available when needed
    target_include_directories(${tusb_lib} PUBLIC "${COMPONENT_DIR}")
    # Compile our descriptors as part of the esp_tinyusb component to avoid reverse deps
    target_sources(${tusb_lib} PRIVATE "${COMPONENT_DIR}/tinyusb_descriptors.cpp")
endif()

# Options de compilation
target_compile_options(${COMPONENT_LIB} PRIVATE 
    -Wall 
    -Wextra 
    -O2
)
