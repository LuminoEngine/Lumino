cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(LN_TARGET_ARCH Android-${ANDROID_ABI})
set(LUMINO_ENGINE_ROOT $ENV{LUMINO_PATH}/Engine/${LN_TARGET_ARCH})

list(APPEND CMAKE_FIND_ROOT_PATH ${LUMINO_ENGINE_ROOT}/lib/Android-${ANDROID_ABI})

include(${LUMINO_ENGINE_ROOT}/LuminoConfig.cmake)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/../../../src/*.cpp)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             ${SOURCES}
             src/main/cpp/native-lib.cpp
             src/main/cpp/PCH.cpp
             src/main/cpp/PCH.h
             )
ln_add_pch(native-lib src/main/cpp/PCH.h src/main/cpp/PCH.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       EGL
                       GLESv3
                       m
                       z
                       LuminoEngine
                       )
