cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Arithmetic")
set(_TYPE "Fxd")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../../${LIB_NAME}/${_TYPE}/src/fxd_arithmetic.c
    ../../../../${LIB_NAME}/${_TYPE}/src/tables.c)
    # ../../src/flt_arithmetic.c
    # ../../src/fxd_arithmetic.c
    # ../../src/tables.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../../${LIB_NAME}/${_TYPE}/inc)

# target_include_directories(${LIB_NAME} PRIVATE ../../inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()