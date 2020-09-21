cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "TestSigGen")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/tsig_chirp.c
    ../../../${LIB_NAME}/src/tsig_delta.c
    ../../../${LIB_NAME}/src/tsig_lsw_sin.c
    ../../../${LIB_NAME}/src/tsig_sin.c
    ../../../${LIB_NAME}/src/tsig_square.c 
    ../../../${LIB_NAME}/src/tsig_step.c 
    ../../../${LIB_NAME}/src/tsig_wnoise.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()