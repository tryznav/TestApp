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

set(LIB_NAME "Effect")
set(_TYPE "Flt")

set(_TYPE "Flt")
set(BASE_PATH "../../..")
set(EFFECTS_PATH "../../../Effect")
# set(ALGORITHM_PATH "../../../../Algorithm")
set(SUB_EFFECT_PATH "../../../Sub_effects")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC

    ${EFFECTS_PATH}/src/effect_control.c
    ${EFFECTS_PATH}/src/effect_process.c)

target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/inc_addition)


target_link_libraries(${LIB_NAME} PRIVATE Chain)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/${_TYPE}/inc)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/${_TYPE}/inc)

target_include_directories(${LIB_NAME} PRIVATE  ${EFFECTS_PATH}/inc)

target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/Crossovers/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/Compressors/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/Chain/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/EQ/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/Filters/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/inc)


