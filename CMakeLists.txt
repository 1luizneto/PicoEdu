# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(demo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(demo 
    main.c 
    src/icons.c 
    src/fonts.c 
    src/display.c 
    src/menu.c
    src/wifi.c
    src/neopixel.c
    src/joystick.c
    src/buzzer.c
    src/notes.c
    src/microfone.c
    )

pico_set_program_name(demo "demo")
pico_set_program_version(demo "0.1")

# Generate PIO header
pico_generate_pio_header(demo ${CMAKE_CURRENT_LIST_DIR}//ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(demo 0)
pico_enable_stdio_usb(demo 1)

# Add the standard library to the build
target_link_libraries(demo
        pico_stdlib
        pico_multicore
        hardware_i2c
        hardware_adc
        hardware_pwm
        pico_cyw43_arch_lwip_threadsafe_background
        )

# Add the standard include files to the build
target_include_directories(demo PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(demo 
    pico_cyw43_arch_lwip_threadsafe_background
        )

pico_add_extra_outputs(demo)

