cmake_minimum_required(VERSION 3.10)

set(PR_NAME "TestApp")

project(${PR_NAME})

add_executable(${PR_NAME}
    ../../src/opt_parse/app_parse_gen_opt.c
    ../../src/opt_parse/app_parse_iir_opt.c
    ../../src/app_efect_init.c
    ../../src/app_generator_init.c
    ../../src/app_handl.c
    ../../src/app_task_check.c
    ../../src/fhand_newav.c
    ../../src/fhand_parse_wav_file_fptr.c
    ../../src/fhand.c
    ../../src/main.c)

set(LIBS_PATH_1 "../Libs/")
set(LIBS_PATH_2 "/tmp/Debug/")

# find_package(json-c CONFIG REQUIRED)
# target_link_libraries(${PR_NAME} PRIVATE json-c)

target_link_libraries(${PR_NAME} PRIVATE json-c)

target_link_libraries(${PR_NAME} PRIVATE GetOpt)
target_link_libraries(${PR_NAME} PRIVATE TestSigGen)
#fxd
target_link_libraries(${PR_NAME} PRIVATE Fir_fxd)
target_link_libraries(${PR_NAME} PRIVATE Iir_fxd)
target_link_libraries(${PR_NAME} PRIVATE Crossover_fxd)
target_link_libraries(${PR_NAME} PRIVATE Apf_fxd)
#flt
target_link_libraries(${PR_NAME} PRIVATE Gain_flt)
target_link_libraries(${PR_NAME} PRIVATE Fir_flt)
target_link_libraries(${PR_NAME} PRIVATE Iir_flt)
# target_link_libraries(${PR_NAME} PRIVATE Crossover_flt)
target_link_libraries(${PR_NAME} PRIVATE Apf_flt)
target_link_libraries(${PR_NAME} PRIVATE Chain_flt)



target_include_directories(${PR_NAME} PRIVATE ../Libs/json-c)



target_include_directories(${PR_NAME} PRIVATE ../../inc)

# target_include_directories(${LIB_NAME} PRIVATE ../../Arithmetic/inc)


set(EFFECTS_PATH "../../Effects")


# target_include_directories(${LIB_NAME} PRIVATE ../json-c)

target_include_directories(${PR_NAME} PRIVATE ${EFFECTS_PATH}/inc)
target_include_directories(${PR_NAME} PRIVATE ${EFFECTS_PATH}/inc/Flt)
target_include_directories(${PR_NAME} PRIVATE ${EFFECTS_PATH}/inc/Fxd)

target_include_directories(${PR_NAME} PRIVATE ../../GetOpt/inc)
target_include_directories(${PR_NAME} PRIVATE ../../TestSigGen/inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()