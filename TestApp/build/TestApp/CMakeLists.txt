cmake_minimum_required(VERSION 3.10)

set(PR_NAME "TestApp")

project(${PR_NAME})

add_executable(${PR_NAME}
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

target_link_libraries(${PR_NAME} PRIVATE EffectGain)
target_link_libraries(${PR_NAME} PRIVATE GetOpt)
target_link_libraries(${PR_NAME} PRIVATE TestSigGen)
target_link_libraries(${PR_NAME} PRIVATE EffectFirFlt)


target_include_directories(${PR_NAME} PRIVATE ../../inc)
target_include_directories(${PR_NAME} PRIVATE ../../EffectFirFlt/inc)
target_include_directories(${PR_NAME} PRIVATE ../../Effects/inc)
target_include_directories(${PR_NAME} PRIVATE ../../GetOpt/inc)
target_include_directories(${PR_NAME} PRIVATE ../../TestSigGen/inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()