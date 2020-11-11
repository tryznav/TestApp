cmake_minimum_required(VERSION 3.10)

project(WholeSolution)


add_subdirectory(Libs/Arithmetic)
add_subdirectory(Libs/json-c)
# FLT
# add_subdirectory(Libs/Effects/Flt/Fir_flt)
# add_subdirectory(Libs/Effects/Flt/Iir_flt)
# add_subdirectory(Libs/Effects/Flt/Gain_flt)
# add_subdirectory(Libs/Effects/Flt/Crossover_flt)
add_subdirectory(Libs/Effects/Flt/Chain_flt)
# add_subdirectory(Libs/Effects/Flt/Apf_flt)

# add_subdirectory(Libs/Effects/Fxd/Fir_fxd)
# add_subdirectory(Libs/Effects/Fxd/Iir_fxd)
# add_subdirectory(Libs/Effects/Fxd/Apf_fxd)
# add_subdirectory(Libs/Effects/Fxd/Crossover_fxd)

# GAIN

# Crossover
# add_subdirectory(Libs/TestSigGen)
# add_subdirectory(Libs/GetOpt)

# add_subdirectory(TestApp)