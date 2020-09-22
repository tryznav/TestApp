cmake_minimum_required(VERSION 3.10)

project(WholeSolution)

add_subdirectory(Libs/TestSigGen)
add_subdirectory(Libs/EffectGain)
add_subdirectory(Libs/FileHandler)
add_subdirectory(Libs/GetOpt)

add_subdirectory(TestApp)