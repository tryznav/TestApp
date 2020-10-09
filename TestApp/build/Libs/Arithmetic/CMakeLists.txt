cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Arithmetic")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/flt_arithmetic.c
    ../../../${LIB_NAME}/src/fxd_arithmetic.c
    ../../../${LIB_NAME}/src/tables.c)
    # ../../src/flt_arithmetic.c
    # ../../src/fxd_arithmetic.c
    # ../../src/tables.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)

# target_include_directories(${LIB_NAME} PRIVATE ../../inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()