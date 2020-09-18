cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "GetOpt")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/getopt.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)