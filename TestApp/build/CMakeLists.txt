# set(CMAKE_C_COMPILER "MinGW")
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
# -msse)

# SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

project(WholeSolution)

set(_TYPE "Flt")

add_subdirectory(Libs/Arithmetic/${_TYPE})


add_subdirectory(Libs/Sub_effects/${_TYPE}/Crossovers)
add_subdirectory(Libs/Sub_effects/${_TYPE}/Filters)
add_subdirectory(Libs/Sub_effects/${_TYPE}/Compressors)
add_subdirectory(Libs/Sub_effects/${_TYPE}/EQ)
add_subdirectory(Libs/Sub_effects/${_TYPE}/Chain)

add_subdirectory(Libs/Effect)
add_subdirectory(Libs/TestSigGen)
add_subdirectory(Libs/GetOpt)
add_subdirectory(Libs/cJSON)

add_subdirectory(TestApp)