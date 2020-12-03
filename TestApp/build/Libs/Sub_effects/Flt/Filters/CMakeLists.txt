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
# -msse
# -pg)

# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(LIB_NAME "Filters")
set(_TYPE "Flt")

set(_TYPE "Flt")
set(BASE_PATH "../../../../..")
set(SUB_EFFECT_PATH "../../../../../Sub_effects")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC

    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_process.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_cascade2_process.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_cascade4_process.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/apf2_band2_process.c


    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_control.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_cascade2_control.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/biquad_cascade4_control.c
    ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/src/apf2_band2_control.c

)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)


target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/${_TYPE}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/${LIB_NAME}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/${_TYPE}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${SUB_EFFECT_PATH}/inc_addition)