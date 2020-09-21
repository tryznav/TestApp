cmake_minimum_required(VERSION 3.10)

set(PR_NAME "TestApp")

project(${PR_NAME})

add_executable(${PR_NAME}
    ../../src/main.c
    ../../src/app_handler.c
    ../../src/app_gain.c)

set(LIBS_PATH_1 "../Libs/")
set(LIBS_PATH_2 "/tmp/Debug/")

target_link_libraries(${PR_NAME} PRIVATE EffectGain)
target_link_libraries(${PR_NAME} PRIVATE FileHandler)
target_link_libraries(${PR_NAME} PRIVATE GetOpt)


target_include_directories(${PR_NAME} PRIVATE ../../inc)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()