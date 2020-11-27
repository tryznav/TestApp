cmake_minimum_required(VERSION 3.10)

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

set(LIB_NAME "Effect")
set(_TYPE "Flt")

set(_TYPE "Flt")
set(BASE_PATH "../../../..")
set(EFFECTS_PATH "../../../../Effect")
set(ALGORITHM_PATH "../../../../Algorithm")

project(${LIB_NAME})

add_library(${LIB_NAME} STATIC


    ${ALGORITHM_PATH}/${_TYPE}/cross/src/cross_calculate_coef.c
    ${ALGORITHM_PATH}/${_TYPE}/cross/src/cross_process.c
    ${ALGORITHM_PATH}/${_TYPE}/cross/src/cross_set.c

    ${ALGORITHM_PATH}/${_TYPE}/comp/src/comp_calculate_coef.c
    ${ALGORITHM_PATH}/${_TYPE}/comp/src/comp_process.c
    ${ALGORITHM_PATH}/${_TYPE}/comp/src/comp_set.c

    ${ALGORITHM_PATH}/${_TYPE}/biquad/src/biquad_calculate_coef.c
    ${ALGORITHM_PATH}/${_TYPE}/biquad/src/biquad_process.c
    ${ALGORITHM_PATH}/${_TYPE}/biquad/src/biquad_set.c

    ${ALGORITHM_PATH}/${_TYPE}/eq/src/eq_calculate_coef.c
    ${ALGORITHM_PATH}/${_TYPE}/eq/src/eq_process.c
    ${ALGORITHM_PATH}/${_TYPE}/eq/src/eq_set.c

    ${EFFECTS_PATH}/${_TYPE}/src/effect_control.c
    ${EFFECTS_PATH}/${_TYPE}/src/effect_process.c)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_link_libraries(${LIB_NAME} PRIVATE Arithmetic)

target_include_directories(${LIB_NAME} PRIVATE ${BASE_PATH}/Arithmetic/${_TYPE}/inc)



target_include_directories(${LIB_NAME} PRIVATE ../../../../Effect/inc)
target_include_directories(${LIB_NAME} PRIVATE ../../../../Effect/Flt/inc)


target_include_directories(${LIB_NAME} PRIVATE ${ALGORITHM_PATH}/inc_addition)



target_include_directories(${LIB_NAME} PRIVATE ${ALGORITHM_PATH}/${_TYPE}/comp/inc)
target_include_directories(${LIB_NAME} PRIVATE ${ALGORITHM_PATH}/${_TYPE}/eq/inc)
target_include_directories(${LIB_NAME} PRIVATE ${ALGORITHM_PATH}/${_TYPE}/cross/inc)
target_include_directories(${LIB_NAME} PRIVATE ${ALGORITHM_PATH}/${_TYPE}/biquad/inc)

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
