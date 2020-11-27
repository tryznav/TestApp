cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Arithmetic")
set(_TYPE "Flt")
add_compile_options(-O2 -Wmultichar -mavx 
-mfma
-mavx2
-mavx
-msse4.2
-msse4.1
-mssse3
-msse3
-msse2
-msse
-pg)

SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ../../../../${LIB_NAME}/${_TYPE}/src/flt_arithmetic.c)
    # ../../src/flt_arithmetic.c
    # ../../src/fxd_arithmetic.c
    # ../../src/tables.c)

target_include_directories(${LIB_NAME} PRIVATE ../../../../${LIB_NAME}/${_TYPE}/inc)

# target_include_directories(${LIB_NAME} PRIVATE ../../inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# add_compile_options(-O2 -Wmultichar -mavx 
# -mfma         
# -mavx2        
# -mavx         
# -msse4.2      
# -msse4.1      
# -mssse3       
# -msse3        
# -msse2        
# -m64          
# -msse)
