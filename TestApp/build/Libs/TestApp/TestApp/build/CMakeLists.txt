cmake_minimum_required(VERSION 3.10)

project(WholeSolution)

set(_TYPE "Flt")

add_subdirectory(Libs/Arithmetic/${_TYPE})
add_subdirectory(Libs/json-c)

add_subdirectory(Libs/Effect/${_TYPE})

add_subdirectory(Libs/TestSigGen)
add_subdirectory(Libs/GetOpt)

add_subdirectory(TestApp)