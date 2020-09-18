cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "EffectGain")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/effect_control.c
    ../../../${LIB_NAME}/src/effect_process.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)