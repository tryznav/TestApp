cmake_minimum_required(VERSION 3.10)

# add_compile_options(-O2 -Wmultichar -mavx 
# -mfma
# -mavx2
# -mavx
# -msse4.2
# -msse4.1
# -mssse3
# -msse3
# -msse2
# -msse)

# SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")


set(LIB_NAME "GetOpt")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../${LIB_NAME}/src/getopt.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../${LIB_NAME}/inc)