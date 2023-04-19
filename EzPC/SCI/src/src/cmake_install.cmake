# Install script for directory: /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-OT.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-HE.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-FloatingPoint.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-FloatML.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-FloatBeacon.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-BuildingBlocks.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-LinearOT.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-LinearHE.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-Math.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/lib/libSCI-GC.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI/SCITargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI/SCITargets.cmake"
         "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/CMakeFiles/Export/lib/cmake/SCI/SCITargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI/SCITargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI/SCITargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI" TYPE FILE FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/CMakeFiles/Export/lib/cmake/SCI/SCITargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI" TYPE FILE FILES "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/CMakeFiles/Export/lib/cmake/SCI/SCITargets-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/utils"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/OT"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/GC"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/Millionaire"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/NonLinear"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/BuildingBlocks"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/LinearOT"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/LinearHE"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/Math"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint"
    FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hpp$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/defines_float.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/defines.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/defines_uniform.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/globals_float.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/globals.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/library_float.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/library_fixed.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/library_fixed_uniform.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/cleartext_library_float.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/cleartext_library_fixed.h"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/cleartext_library_fixed_uniform.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SCI" TYPE FILE FILES
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/utils/cmake/FindGMP.cmake"
    "/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/utils/cmake/source_of_randomness.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/utils/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/OT/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/GC/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/Millionaire/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/BuildingBlocks/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/LinearOT/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/LinearHE/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/NonLinear/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/Math/cmake_install.cmake")
  include("/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint/cmake_install.cmake")

endif()

