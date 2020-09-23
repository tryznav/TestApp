cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "FileHandler")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/file_handler.c
    ../../../${LIB_NAME}/src/fhand_wav_process.c
    ../../../${LIB_NAME}/src/fhand_parse_wav_file_fptr.c
    ../../../${LIB_NAME}/src/fhand_newav.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()