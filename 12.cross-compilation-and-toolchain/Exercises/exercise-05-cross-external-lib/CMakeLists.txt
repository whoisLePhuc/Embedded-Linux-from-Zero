cmake_minimum_required(VERSION 3.10)
project(hello-arm C)

# TODO: Create arm-cross.cmake toolchain file, then build with:
#   cmake -DCMAKE_TOOLCHAIN_FILE=../arm-cross.cmake -B build
#   make -C build

set(SOURCES main.c)
add_executable(hello-arm ${SOURCES})
