cmake_minimum_required(VERSION 3.10)
project(hello-arm C)

add_executable(hello-arm main.c)

target_compile_definitions(hello-arm PRIVATE ARCH_ARM)
