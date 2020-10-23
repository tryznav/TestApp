cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Iir_fxd")
set(_TYPE "Fxd")

set(BASE_PATH "../../../../..")
set(EFFECTS_PATH "../../../../../Effects")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/iir_fxd_control.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/iir_fxd_process.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/coeff_calculate.c)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/inc)

target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc_addition)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc/${_TYPE})
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/inc_close)