#!/bin/bash

# Create a build directory
mkdir -p build
cd build

# Run CMake to configure the project
cmake ..

# Build the project using make
cmake --build .

# If you want to flash the firmware to your STM32 device, you can add the flashing command here.
# Example (replace /dev/ttyUSB0 and your_hex_file.hex with your actual values):
# stm32flash -w ${PROJECT_NAME}.hex -v -g 0x0 -R -i 23,-22,22:-23,23 -b 115200 /dev/ttyUSB0

# Optionally, you can install the firmware to a specified directory (e.g., out)
# make install DESTDIR=out

# Optionally, you can clean the build directory after the build
# make clean