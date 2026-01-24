cmake_minimum_required(VERSION 3.13)

include ($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
# if you have pico_sdk_import.cmake in the project and you did not 
# set PICO_SDK_PATH, uncomment the following and comment above
# include(pico_sdk_import.cmake)

project(power-relay-control C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS "-mcpu=cortex-m0plus -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m0plus -mthumb")

add_subdirectory(
	include/crc8
)

add_executable(power-relay-control
    src/main.c
    src/pico-serial-message.c
    src/power-relay-control.c
)
pico_enable_stdio_usb(power-relay-control 1)
pico_enable_stdio_uart(power-relay-control 0)

pico_add_extra_outputs(power-relay-control)

target_link_libraries(power-relay-control pico_stdlib hardware_gpio hardware_irq hardware_i2c hardware_spi crc8)

