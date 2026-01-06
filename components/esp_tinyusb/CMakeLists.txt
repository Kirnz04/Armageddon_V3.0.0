cmake_minimum_required(VERSION 3.20)

# Simple wrapper component that integrates the vendored TinyUSB sources
# into an IDF component so other components can `REQUIRES esp_tinyusb`.

idf_component_register()

# Include the tinyusb src CMake helper which exposes tinyusb_target_add()
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../tinyusb/src ${CMAKE_CURRENT_BINARY_DIR}/tinyusb_src)

if (TARGET ${COMPONENT_LIB})
    tinyusb_target_add(${COMPONENT_LIB})
    target_include_directories(${COMPONENT_LIB} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../tinyusb/src)
endif()
