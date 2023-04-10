cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE /usr/local/etc/arduino-cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(PROJECT_NAME ssd1306_gfx)

# use UART with interrupts if set to non-zero
set(UART_INT 0)

# use DEBUG_MODE dev version of options
set(DEBUG_MODE 1)
IF (DEBUG_MODE)
    set(ARDUINO_CPU 16MHzatmega328)
    add_compile_definitions(DEBUG_MODE)
ELSE ()
    set(ARDUINO_CPU 8MHzatmega328)
ENDIF ()

set(${PROJECT_NAME}_BOARD pro)
project(${PROJECT_NAME})

add_compile_definitions(INCLUDE_TWI_INT)
set(TWI_C_FILES src/twi.c)
set(TWI_H_FILES src/twi.h)

# Define additional source and header files or default arduino sketch files
set(${PROJECT_NAME}_SRCS
        src/print.c
        src/Ssd1306Display.cpp
        src/ssd1306.c
        src/ssd1306_gfx.c
        src/ssd1306_print.c
        ssd1306_test.ino
        ${TWI_C_FILES}
        )
set(${PROJECT_NAME}_HDRS
        src/print.h
        src/Ssd1306Display.h
        src/ssd1306.h
        src/ssd1306_cmd.h
        src/ssd1306_gfx.h
        ${TWI_H_FILES}
        )
# set(${PROJECT_NAME}_LIBS)

### Additional static libraries to include in the target.
#set(${PROJECT_NAME}_LIBS/..)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
# add_subdirectory()
# set(${PROJECT_NAME}_SKETCH ssd1306_gfx.ino)

# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

set(${CMAKE_PROJECT_NAME}_SKETCH ssd1306_test.ino)

#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT /dev/cu.usbserial-00000000)
set(pro.upload.speed 57600)

# CAUTION: Need this for TWI & UART, Configure ATmega328p Library options, it knows nothing of the Arduino CMake options.
add_compile_definitions(BAUD=${pro.upload.speed})
add_compile_definitions(TWI_FREQUENCY=400000)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

print_board_list()
print_programmer_list()
