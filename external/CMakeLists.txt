
cmake_minimum_required(VERSION 3.0.0)
project (ExternalProjects)

include(../src/LuminoCommon.cmake)
include(ExternalProject)

# notify global values to external projects.
set(
    LN_FLAGS_ARGS
    -DCMAKE_C_FLAGS=${DCMAKE_C_FLAGS}
    -DCMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}
    -DCMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
    -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
)

ExternalProject_Add(
    external_zlib
    PREFIX          ${LN_EXTERNAL_BUILD_DIR}/zlib
    INSTALL_DIR     ${LN_EXTERNAL_INSTALL_DIR}/zlib
    GIT_REPOSITORY  https://github.com/madler/zlib.git
    GIT_TAG         v1.2.11
    CMAKE_ARGS      -DASM686=OFF
                    -DAMD64=OFF
                    -DCMAKE_INSTALL_PREFIX=${LN_EXTERNAL_INSTALL_DIR}/zlib
                    -DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                    ${LN_FLAGS_ARGS}
)

ExternalProject_Add(
    external_libpng
    DEPENDS         external_zlib
    PREFIX          ${LN_EXTERNAL_BUILD_DIR}/libpng
    INSTALL_DIR     ${LN_EXTERNAL_INSTALL_DIR}/libpng
    GIT_REPOSITORY  git://git.code.sf.net/p/libpng/code
    GIT_TAG         v1.6.9
    CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${LN_EXTERNAL_INSTALL_DIR}/libpng
                    -DZLIB_ROOT=${LN_EXTERNAL_INSTALL_DIR}/zlib
                    -DPNG_SHARED=OFF
                    -DPNG_STATIC=ON
                    -DPNG_TESTS=OFF
                    -DPNG_DEBUG=OFF
                    -DPNGARG=OFF
                    ${LN_FLAGS_ARGS}
                    #-DBUILD_SHARED_LIBS=FALSE
                    #-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    #-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                    #-DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
                    #-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                    #-DCMAKE_C_FLAGS=${GL_BINDING_C_FLAGS}
)

ExternalProject_Add(
    external_glslang
    PREFIX          ${LN_EXTERNAL_BUILD_DIR}/glslang
    INSTALL_DIR     ${LN_EXTERNAL_INSTALL_DIR}/glslang
    GIT_REPOSITORY  https://github.com/KhronosGroup/glslang.git
    GIT_TAG         6.2.2596
    CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${LN_EXTERNAL_INSTALL_DIR}/glslang
                    ${LN_FLAGS_ARGS}
)

ExternalProject_Add(
    external_SPIRV-Cross
    PREFIX          ${LN_EXTERNAL_BUILD_DIR}/SPIRV-Cross
    INSTALL_DIR     ${LN_EXTERNAL_INSTALL_DIR}/SPIRV-Cross
    GIT_REPOSITORY  https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG         be7425ef70231ab82930331959ab487d605d0482
    CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${LN_EXTERNAL_INSTALL_DIR}/SPIRV-Cross
                    -DCMAKE_DEBUG_POSTFIX=d
                    ${LN_FLAGS_ARGS}
)




