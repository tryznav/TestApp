cmake_minimum_required(VERSION 3.10)

set(LIB_NAME "Crossover_flt")
set(_TYPE "Flt")

set(_TYPE "Flt")
set(BASE_PATH "../../../../..")
set(EFFECTS_PATH "../../../../../Effects")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC
    # all pass filter
    ${EFFECTS_PATH}/${_TYPE}/Apf_flt/apf/src/apf_dbl_structs.c
    ${EFFECTS_PATH}/${_TYPE}/Apf_flt/apf/src/apf_flt_structs.c
    ${EFFECTS_PATH}/${_TYPE}/Apf_flt/apf/src/apf_flt_coef.c

    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/cross/src/cross_flt_coef.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/cross/src/cross_flt.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/cross/src/cross_dbl.c

    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/api/src/cross_flt_control.c
    ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/api/src/cross_flt_process.c)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/inc)
# all pass filter
target_include_directories(${LIB_NAME} PRIVATE  ${EFFECTS_PATH}/Flt/Apf_flt/apf/inc)

target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc_addition)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/inc/${_TYPE})
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/api/inc)
target_include_directories(${LIB_NAME} PRIVATE ${EFFECTS_PATH}/${_TYPE}/${LIB_NAME}/cross/inc)