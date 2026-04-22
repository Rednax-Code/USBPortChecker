cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(usb_valve_custom C CXX ASM)

pico_sdk_init()

add_subdirectory(lib/pico-ssd1306)

add_executable(usbvalve
    src/main.cpp src/globals.c src/usb_descriptors.c src/ramdisk.c src/msc_callbacks.c src/xxhash.c
)

target_include_directories(usbvalve PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_definitions(usbvalve PRIVATE
    PICO_STDIO_USB_CONTAINER_PCIE=0
    CFG_TUSB_DEBUG=0
)

# 2. Link the library to your executable
# The name 'pico_ssd1306' is defined inside the library's own CMakeLists.txt
target_link_libraries(usbvalve 
    pico_stdlib
    pico_ssd1306
    pico_bootrom
    pico_multicore
    hardware_i2c
    tinyusb_device
)

pico_enable_stdio_usb(usbvalve 1)
pico_enable_stdio_uart(usbvalve 0)
pico_add_extra_outputs(usbvalve)