cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME ssd1306)
set(${CMAKE_PROJECT_NAME}_BOARD pro)
set(ARDUINO_CPU 8MHzatmega328)

project(${PROJECT_NAME})

# Define the source code for cpp files or default arduino sketch files
set(${PROJECT_NAME}_SRCS ssd1306_test.cpp.bak ssd1306.c ssd1306_gfx.c ssd1306_max.cpp twi.c)
set(${PROJECT_NAME}_HDRS ssd1306.h twi.h ssd1306_display.h ssd1306_cmd.h Ssd1306Display.h)
### Additional static libraries to include in the target.
#set(${CMAKE_PROJECT_NAME}_LIBS/..)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

#set(${CMAKE_PROJECT_NAME}_SKETCH ssd1306_test.ino)

#### Additional settings for programmer. From programmers.txt
set(${CMAKE_PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${CMAKE_PROJECT_NAME}_PORT /dev/cu.usbserial-00000000)
# set(pro.upload.speed 57600)

## Verbose build process
set(${CMAKE_PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
