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
        tcpip_adapter
        tinyusb
        lwip
        freertos
        driver
    
    LDFRAGMENTS linker.lf
)

# Add compilation flags
target_compile_options(${COMPONENT_LIB} PRIVATE 
    -Wall 
    -Wextra 
    -Werror=format 
    -O2
)

# Suppress warnings for specific issues (optional)
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-format
)
