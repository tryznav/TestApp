cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Apf_flt")
set(_TYPE "Flt")

set(_TYPE "Flt")
set(BASE_PATH "../../../../..")
set(EFFECTS_PATH "../../../../../Effects")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/apf_calculate.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/apf_flt_control.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/apf_flt_process.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/src/apf_settings.c)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)
target_link_libraries(${LIB_NAME} PRIVATE json-c)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/inc)


target_include_directories(${LIB_NAME} PRIVATE ../json-c)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc_addition)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc/${_TYPE})
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/inc_close)