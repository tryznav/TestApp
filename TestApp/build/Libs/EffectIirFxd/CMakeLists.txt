cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "EffectIirFxd")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/iir_fxd_control.c
    ../../../${LIB_NAME}/src/iir_fxd_process.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../Arithmetic/inc)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)