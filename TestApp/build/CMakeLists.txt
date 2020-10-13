cmake_minimum_required(VERSION 3.10)

project(WholeSolution)

add_subdirectory(Libs/Arithmetic)
#FIR
add_subdirectory(Libs/EffectFirFlt)
add_subdirectory(Libs/EffectFirFxd)
# #IIR
add_subdirectory(Libs/EffectIirFlt)
add_subdirectory(Libs/EffectIirFxd)
# #GAIN
add_subdirectory(Libs/EffectGain)

add_subdirectory(Libs/TestSigGen)
add_subdirectory(Libs/GetOpt)

add_subdirectory(TestApp)