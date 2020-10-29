# Install script for directory: D:/TestApp/TestApp/build

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/WholeSolution")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/TestApp/TestApp/build/tmp/Libs/Arithmetic/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Flt/Fir_flt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Flt/Iir_flt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Flt/Gain_flt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Flt/Crossover_flt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Flt/Apf_flt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Fxd/Fir_fxd/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Fxd/Iir_fxd/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Fxd/Apf_fxd/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/Effects/Fxd/Crossover_fxd/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/TestSigGen/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/Libs/GetOpt/cmake_install.cmake")
  include("D:/TestApp/TestApp/build/tmp/TestApp/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/TestApp/TestApp/build/tmp/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
