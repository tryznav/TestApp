cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "EffectFirFxd")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/fir_fxd_control.c
    ../../../${LIB_NAME}/src/fir_fxd_process.c
    ../../../${LIB_NAME}/src/coefs.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../Arithmetic/inc)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)