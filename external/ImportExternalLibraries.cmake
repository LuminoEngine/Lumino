
message("LN_TARGET_ARCH: ${LN_TARGET_ARCH}")

# FIXME: Emscripten と Android のビルド時、find_library が sysroot 以外を探せるように強制。
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

if (DEFINED LN_DEPENDENCIES_ROOT)
    message("LN_DEPENDENCIES_ROOT:${LN_DEPENDENCIES_ROOT}")
endif()

if (DEFINED EMSCRIPTEN)

elseif (LN_ANDROID)

elseif (APPLE AND DEFINED PLATFORM) # PLATFORM is ios-cmake variable.

elseif(WIN32 OR APPLE OR UNIX)
    set(LN_OS_DESKTOP ON)

endif()


if (NOT DEFINED LN_BUILD_DIRECTORY)
    # 外部プロジェクトからこの .cmake が呼ばれること備えて、CMAKE_CURRENT_BINARY_DIR は使わない
    set(LN_BUILD_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../build/${LN_TARGET_ARCH})
endif()

#-------------------------------------------------------------------------------
# Visual Studio ソリューションフォルダを作るようにする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    add_definitions("/wd4996")        # pragma warning disable
endif()

#-------------------------------------------------------------------------------
macro(ln_make_external_find_path varName projectDirName)
    if (DEFINED LN_DEPENDENCIES_ROOT)
        set(${varName} ${LN_DEPENDENCIES_ROOT}/${projectDirName})
    elseif(DEFINED LN_EXTERNAL_FIND_PATH_MODE)
        if (${LN_EXTERNAL_FIND_PATH_MODE} STREQUAL "build")
            set(${varName} ${LN_BUILD_DIRECTORY}/ExternalInstall/${projectDirName})
        elseif (${LN_EXTERNAL_FIND_PATH_MODE} STREQUAL "config")
            set(${varName} ${CMAKE_CURRENT_LIST_DIR}/lib)
        endif()
    elseif(DEFINED CMAKE_BUILD_TYPE)
        set(${varName} ${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}-${CMAKE_BUILD_TYPE})
    else()
        set(${varName} ${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH})
    endif()
endmacro()

#--------------------------------------
# glfw
ln_make_external_find_path(GLFW_ROOT glfw)

find_library(GLFW_LIBRARY_RELEASE NAMES glfw3 libglfw3 PATHS ${GLFW_ROOT} PATH_SUFFIXES lib)
find_library(GLFW_LIBRARY_DEBUG NAMES glfw3d libglfw3d glfw3 libglfw3 PATHS ${GLFW_ROOT} PATH_SUFFIXES lib)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION_RELEASE "${GLFW_LIBRARY_RELEASE}")
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION_DEBUG "${GLFW_LIBRARY_DEBUG}")
set_target_properties(glfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLFW_ROOT}/include)
if (GLFW_LIBRARY_RELEASE)
    list(APPEND LN_EXTERNAL_LIBS glfw)
endif()

#--------------------------------------
# glad
ln_make_external_find_path(GLAD_ROOT glad)

find_library(GLAD_LIBRARY_RELEASE NAMES glad libglad PATHS ${GLAD_ROOT} PATH_SUFFIXES lib)
find_library(GLAD_LIBRARY_DEBUG NAMES gladd libgladd glad libglad PATHS ${GLAD_ROOT} PATH_SUFFIXES lib)

set(LIB_NAME GLAD)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${LIB_NAME}_ROOT}/include)
if(GLAD_LIBRARY_RELEASE)
    list(APPEND LN_EXTERNAL_LIBS GLAD)
endif()

#--------------------------------------
# zlib
# if (ANDROID_ABI)
#     # zlib を "ZLIB" 変数で参照できるようにする
#     find_package(ZLIB REQUIRED)
#     add_library(ZLIB STATIC IMPORTED)
#     set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION "${ZLIB_LIBRARIES}")
#     set_target_properties(ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS})
#     list(APPEND LN_EXTERNAL_LIBS ZLIB)
    
# elseif (DEFINED EMSCRIPTEN OR LN_IOS)
#     ln_make_external_find_path(ZLIB_ROOT zlib)
#     find_library(ZLIB_LIBRARY_RELEASE NAMES zlib PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
#     find_library(ZLIB_LIBRARY_DEBUG NAMES zlibd zlib PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)

#     set(LIB_NAME ZLIB)
#     add_library(${LIB_NAME} STATIC IMPORTED)
#     set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
#     set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
#     set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_ROOT}/include)
#     list(APPEND LN_EXTERNAL_LIBS ZLIB)
    
# else()
#     ln_make_external_find_path(ZLIB_ROOT zlib)
#     find_library(ZLIB_LIBRARY_RELEASE NAMES zlib PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
#     find_library(ZLIB_LIBRARY_DEBUG NAMES zlibd zlib PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)

#     set(LIB_NAME ZLIB)
#     add_library(${LIB_NAME} STATIC IMPORTED)
#     set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
#     set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
#     set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_ROOT}/include)
#     list(APPEND LN_EXTERNAL_LIBS ZLIB)

# #elseif (LN_IOS)
#     # use system zlib.
# #    find_package(ZLIB REQUIRED)
# #    add_library(ZLIB STATIC IMPORTED)
# #    set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION_RELEASE ${ZLIB_LIBRARIES})
# #    set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION_DEBUG ${ZLIB_LIBRARIES})
# #    set_target_properties(ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS})
# #    list(APPEND LN_EXTERNAL_LIBS ZLIB)

# #else()
# endif()

#--------------------------------------
# libpng
# ln_make_external_find_path(PNG_ROOT libpng)
# find_library(PNG_LIBRARY_RELEASE NAMES libpng16 png16 PATHS ${PNG_ROOT} PATH_SUFFIXES lib)
# find_library(PNG_LIBRARY_DEBUG NAMES libpng16d png16d libpng16 png16 PATHS ${PNG_ROOT} PATH_SUFFIXES lib)

# set(LIB_NAME PNG)
# add_library(${LIB_NAME} STATIC IMPORTED)
# set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
# set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
# set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${PNG_ROOT}/include)
# list(APPEND LN_EXTERNAL_LIBS PNG)


#--------------------------------------
# glslang

ln_make_external_find_path(GLSLANG_ROOT glslang)

find_library(glslang_LIBRARY_RELEASE NAMES libglslang glslang PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(glslang_LIBRARY_DEBUG NAMES libglslangd glslangd libglslang glslang PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(HLSL_LIBRARY_RELEASE NAMES libHLSL HLSL PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(HLSL_LIBRARY_DEBUG NAMES libHLSLd HLSLd libHLSL HLSL PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OGLCompiler_LIBRARY_RELEASE NAMES libOGLCompiler OGLCompiler PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OGLCompiler_LIBRARY_DEBUG NAMES libOGLCompilerd OGLCompilerd libOGLCompiler OGLCompiler PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OSDependent_LIBRARY_RELEASE NAMES libOSDependent OSDependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(OSDependent_LIBRARY_DEBUG NAMES libOSDependentd OSDependentd libOSDependent OSDependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPIRV_LIBRARY_RELEASE NAMES libSPIRV SPIRV PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPIRV_LIBRARY_DEBUG NAMES libSPIRVd SPIRVd libSPIRV SPIRV PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPVRemapper_LIBRARY_RELEASE NAMES libSPVRemapper SPVRemapper PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(SPVRemapper_LIBRARY_DEBUG NAMES libSPVRemapperd SPVRemapperd libSPVRemapper SPVRemapper PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(GenericCodeGen_LIBRARY_RELEASE NAMES libGenericCodeGen GenericCodeGen PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(GenericCodeGen_LIBRARY_DEBUG NAMES libGenericCodeGend GenericCodeGend libGenericCodeGen GenericCodeGen PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(MachineIndependent_LIBRARY_RELEASE NAMES libMachineIndependent MachineIndependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
find_library(MachineIndependent_LIBRARY_DEBUG NAMES libMachineIndependentd MachineIndependentd libMachineIndependent MachineIndependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)

set(glslang_INCLUDE_DIRS "${GLSLANG_ROOT}/include")

set(LIB_NAME glslang)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME HLSL)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME OGLCompiler)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME OSDependent)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME SPIRV)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME SPVRemapper)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME GenericCodeGen)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

set(LIB_NAME MachineIndependent)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set(glslang_LIBRARIES ${glslang_LIBRARIES} ${LIB_NAME})

if(glslang_LIBRARY_RELEASE)
    list(APPEND LN_EXTERNAL_LIBS glslang)
    list(APPEND LN_EXTERNAL_LIBS HLSL)
    list(APPEND LN_EXTERNAL_LIBS OGLCompiler)
    list(APPEND LN_EXTERNAL_LIBS OSDependent)
    list(APPEND LN_EXTERNAL_LIBS SPIRV)
    list(APPEND LN_EXTERNAL_LIBS SPVRemapper)
    list(APPEND LN_EXTERNAL_LIBS GenericCodeGen)
    list(APPEND LN_EXTERNAL_LIBS MachineIndependent)
endif()

#--------------------------------------
# SPIRV-Cross

ln_make_external_find_path(SPIRV-Cross_ROOT SPIRV-Cross)

find_library(spirv-cross-core_LIBRARY_RELEASE NAMES spirv-cross-core PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-core_LIBRARY_DEBUG NAMES spirv-cross-cored spirv-cross-core PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-glsl_LIBRARY_RELEASE NAMES spirv-cross-glsl PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
find_library(spirv-cross-glsl_LIBRARY_DEBUG NAMES spirv-cross-glsld spirv-cross-glsl PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)

set(LIB_NAME spirv-cross-core)
add_library(${LIB_NAME} STATIC IMPORTED)
#set_property(TARGET spirv-cross-core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")

add_library(spirv-cross-glsl STATIC IMPORTED)
#set_property(TARGET spirv-cross-glsl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-glsl PROPERTIES IMPORTED_LOCATION_RELEASE "${spirv-cross-glsl_LIBRARY_RELEASE}")
set_target_properties(spirv-cross-glsl PROPERTIES IMPORTED_LOCATION_DEBUG "${spirv-cross-glsl_LIBRARY_DEBUG}")

set(spirv-cross_INCLUDE_DIRS "${SPIRV-Cross_ROOT}/include")
set(spirv-cross_LIBRARIES spirv-cross-core spirv-cross-glsl)

if(spirv-cross-core_LIBRARY_RELEASE)
    list(APPEND LN_EXTERNAL_LIBS spirv-cross-core)
    list(APPEND LN_EXTERNAL_LIBS spirv-cross-glsl)
endif()

#--------------------------------------
# openal-soft
if (LN_EMSCRIPTEN)

elseif (LN_ANDROID)

elseif(APPLE)
    # Use OpenAL.framework
else()
    ln_make_external_find_path(OpenAL_ROOT openal-soft)

    find_library(OpenAL_LIBRARY_RELEASE NAMES OpenAL32 PATHS ${OpenAL_ROOT} PATH_SUFFIXES lib)
    find_library(OpenAL_LIBRARY_DEBUG NAMES OpenAL32d OpenAL32 PATHS ${OpenAL_ROOT} PATH_SUFFIXES lib)

    add_library(OpenAL STATIC IMPORTED)
    set_target_properties(OpenAL PROPERTIES IMPORTED_LOCATION_RELEASE "${OpenAL_LIBRARY_RELEASE}")
    set_target_properties(OpenAL PROPERTIES IMPORTED_LOCATION_DEBUG "${OpenAL_LIBRARY_DEBUG}")
    set_target_properties(OpenAL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OpenAL_ROOT}/include)
    list(APPEND LN_EXTERNAL_LIBS OpenAL)
endif()

#--------------------------------------
# SDL2
if (LN_USE_SDL)
    ln_make_external_find_path(SDL2_ROOT SDL2)
    find_library(SDL2_LIBRARY_RELEASE NAMES SDL2 PATHS ${SDL2_ROOT} PATH_SUFFIXES lib)
    find_library(SDL2_LIBRARY_DEBUG NAMES SDL2d SDL2 PATHS ${SDL2_ROOT} PATH_SUFFIXES lib)

    set(LIB_NAME SDL2)
    add_library(${LIB_NAME} STATIC IMPORTED)
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
    set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SDL2_ROOT}/include)
    list(APPEND LN_EXTERNAL_LIBS SDL2)
endif()

# #--------------------------------------
# # freetype
# ln_make_external_find_path(FreeType_ROOT "freetype2")

# find_library(FreeType_LIBRARY_RELEASE NAMES freetype libfreetype PATHS ${FreeType_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(FreeType_LIBRARY_DEBUG NAMES freetyped libfreetyped freetype libfreetype PATHS ${FreeType_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

# add_library(FreeType STATIC IMPORTED)
# set_target_properties(FreeType PROPERTIES IMPORTED_LOCATION_RELEASE "${FreeType_LIBRARY_RELEASE}")
# set_target_properties(FreeType PROPERTIES IMPORTED_LOCATION_DEBUG "${FreeType_LIBRARY_DEBUG}")
# set_target_properties(FreeType PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${FreeType_ROOT}/include/freetype2)
# list(APPEND LN_EXTERNAL_LIBS FreeType)

#--------------------------------------
# ogg
ln_make_external_find_path(ogg_ROOT "ogg")

find_library(ogg_LIBRARY_RELEASE NAMES ogg libogg PATHS ${ogg_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(ogg_LIBRARY_DEBUG NAMES oggd liboggd ogg libogg PATHS ${ogg_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(ogg STATIC IMPORTED)
set_target_properties(ogg PROPERTIES IMPORTED_LOCATION_RELEASE "${ogg_LIBRARY_RELEASE}")
set_target_properties(ogg PROPERTIES IMPORTED_LOCATION_DEBUG "${ogg_LIBRARY_DEBUG}")
set_target_properties(ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ogg_ROOT}/include)
list(APPEND LN_EXTERNAL_LIBS ogg)

#--------------------------------------
# vorbis
ln_make_external_find_path(vorbis_ROOT "vorbis")

find_library(vorbis_LIBRARY_RELEASE NAMES vorbis libvorbis PATHS ${vorbis_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(vorbis_LIBRARY_DEBUG NAMES vorbisd libvorbisd vorbis libvorbis PATHS ${vorbis_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(vorbisfile_LIBRARY_RELEASE NAMES vorbisfile libvorbisfile PATHS ${vorbis_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(vorbisfile_LIBRARY_DEBUG NAMES vorbisfiled libvorbisfiled vorbisfile libvorbisfile PATHS ${vorbis_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(vorbis STATIC IMPORTED)
set_target_properties(vorbis PROPERTIES IMPORTED_LOCATION_RELEASE "${vorbis_LIBRARY_RELEASE}")
set_target_properties(vorbis PROPERTIES IMPORTED_LOCATION_DEBUG "${vorbis_LIBRARY_DEBUG}")
set_target_properties(vorbis PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${vorbis_ROOT}/include)
list(APPEND LN_EXTERNAL_LIBS vorbis)

add_library(vorbisfile STATIC IMPORTED)
set_target_properties(vorbisfile PROPERTIES IMPORTED_LOCATION_RELEASE "${vorbisfile_LIBRARY_RELEASE}")
set_target_properties(vorbisfile PROPERTIES IMPORTED_LOCATION_DEBUG "${vorbisfile_LIBRARY_DEBUG}")
list(APPEND LN_EXTERNAL_LIBS vorbisfile)

# #--------------------------------------
# # bullet
# ln_make_external_find_path(bullet3_ROOT "bullet3")

# find_library(LinearMath_LIBRARY_RELEASE NAMES LinearMath PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(LinearMath_LIBRARY_DEBUG NAMES LinearMathd LinearMath PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletCollision_LIBRARY_RELEASE NAMES BulletCollision PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletCollision_LIBRARY_DEBUG NAMES BulletCollisiond BulletCollision PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletDynamics_LIBRARY_RELEASE NAMES BulletDynamics PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletDynamics_LIBRARY_DEBUG NAMES BulletDynamicsd BulletDynamics PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletSoftBody_LIBRARY_RELEASE NAMES BulletSoftBody PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(BulletSoftBody_LIBRARY_DEBUG NAMES BulletSoftBodyd BulletSoftBody PATHS ${bullet3_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

# add_library(LinearMath STATIC IMPORTED)
# set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION_RELEASE "${LinearMath_LIBRARY_RELEASE}")
# set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION_DEBUG "${LinearMath_LIBRARY_DEBUG}")
# set_target_properties(LinearMath PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${bullet3_ROOT}/include/bullet)
# list(APPEND LN_EXTERNAL_LIBS LinearMath)

# add_library(BulletCollision STATIC IMPORTED)
# set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION_RELEASE "${BulletCollision_LIBRARY_RELEASE}")
# set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION_DEBUG "${BulletCollision_LIBRARY_DEBUG}")
# list(APPEND LN_EXTERNAL_LIBS BulletCollision)

# add_library(BulletDynamics STATIC IMPORTED)
# set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION_RELEASE "${BulletDynamics_LIBRARY_RELEASE}")
# set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION_DEBUG "${BulletDynamics_LIBRARY_DEBUG}")
# list(APPEND LN_EXTERNAL_LIBS BulletDynamics)

# add_library(BulletSoftBody STATIC IMPORTED)
# set_target_properties(BulletSoftBody PROPERTIES IMPORTED_LOCATION_RELEASE "${BulletSoftBody_LIBRARY_RELEASE}")
# set_target_properties(BulletSoftBody PROPERTIES IMPORTED_LOCATION_DEBUG "${BulletSoftBody_LIBRARY_DEBUG}")
# list(APPEND LN_EXTERNAL_LIBS BulletSoftBody)

# #--------------------------------------
# # pcre
# ln_make_external_find_path(pcre_ROOT "pcre")

# find_library(pcre_LIBRARY_RELEASE NAMES pcre2-16 PATHS ${pcre_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(pcre_LIBRARY_DEBUG NAMES pcre2-16d pcre2-16 PATHS ${pcre_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

# add_library(pcre STATIC IMPORTED)
# set_target_properties(pcre PROPERTIES IMPORTED_LOCATION_RELEASE "${pcre_LIBRARY_RELEASE}")
# set_target_properties(pcre PROPERTIES IMPORTED_LOCATION_DEBUG "${pcre_LIBRARY_DEBUG}")
# set_target_properties(pcre PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${pcre_ROOT}/include)
# list(APPEND LN_EXTERNAL_LIBS pcre)

#--------------------------------------
# tmxlite
ln_make_external_find_path(tmxlite_ROOT "tmxlite")

find_library(tmxlite_LIBRARY_RELEASE NAMES tmxlite-s PATHS ${tmxlite_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(tmxlite_LIBRARY_DEBUG NAMES tmxlite-s-d tmxlite-s PATHS ${tmxlite_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(tmxlite STATIC IMPORTED)
set_target_properties(tmxlite PROPERTIES IMPORTED_LOCATION_RELEASE "${tmxlite_LIBRARY_RELEASE}")
set_target_properties(tmxlite PROPERTIES IMPORTED_LOCATION_DEBUG "${tmxlite_LIBRARY_DEBUG}")
set_target_properties(tmxlite PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${tmxlite_ROOT}/include)
#list(APPEND LN_EXTERNAL_LIBS tmxlite)

# #--------------------------------------
# # Box2D
# ln_make_external_find_path(Box2D_ROOT "Box2D")

# find_library(Box2D_LIBRARY_RELEASE NAMES Box2D PATHS ${Box2D_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
# find_library(Box2D_LIBRARY_DEBUG NAMES Box2Dd Box2D PATHS ${Box2D_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

# add_library(Box2D STATIC IMPORTED)
# set_target_properties(Box2D PROPERTIES IMPORTED_LOCATION_RELEASE "${Box2D_LIBRARY_RELEASE}")
# set_target_properties(Box2D PROPERTIES IMPORTED_LOCATION_DEBUG "${Box2D_LIBRARY_DEBUG}")
# set_target_properties(Box2D PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Box2D_ROOT}/include)
# list(APPEND LN_EXTERNAL_LIBS Box2D)

#--------------------------------------
# Vulkan

if (${LN_TARGET_ARCH} STREQUAL "Emscripten")
else()
    #if (LN_USE_VULKAN)
    ##    if (WIN32)
    ln_make_external_find_path(VulkanHeaders_ROOT "Vulkan-Headers")

    add_library(VulkanHeaders IMPORTED INTERFACE)   # IMPORTED がないと install で Target not found になる
    set_target_properties(VulkanHeaders PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${VulkanHeaders_ROOT}/include)
    list(APPEND LN_EXTERNAL_LIBS VulkanHeaders)

    #find_package(Vulkan REQUIRED)
    #list(APPEND LN_EXTERNAL_LIBS Vulkan::Vulkan)


    #add_library(VulkanImported IMPORTED INTERFACE)
    #set_target_properties(VulkanImported PROPERTIES IMPORTED_LOCATION_RELEASE ${Vulkan_LIBRARIES})
    #set_target_properties(VulkanImported PROPERTIES IMPORTED_LOCATION_DEBUG ${Vulkan_LIBRARIES})
    # dynamic load としたため、include だけでよい
    #set_target_properties(VulkanImported PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Vulkan_INCLUDE_DIRS})
    #list(APPEND LN_EXTERNAL_LIBS VulkanImported)
    #    endif()
    #endif()
endif()

#--------------------------------------
# yaml-cpp
ln_make_external_find_path(yamlcpp_ROOT "yaml-cpp")

find_library(yamlcpp_LIBRARY_RELEASE NAMES yaml-cpp PATHS ${yamlcpp_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(yamlcpp_LIBRARY_DEBUG NAMES yaml-cppd yaml-cpp PATHS ${yamlcpp_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(yamlcpp STATIC IMPORTED)
set_target_properties(yamlcpp PROPERTIES IMPORTED_LOCATION_RELEASE "${yamlcpp_LIBRARY_RELEASE}")
set_target_properties(yamlcpp PROPERTIES IMPORTED_LOCATION_DEBUG "${yamlcpp_LIBRARY_DEBUG}")
set_target_properties(yamlcpp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${yamlcpp_ROOT}/include)
list(APPEND LN_EXTERNAL_LIBS yamlcpp)

#--------------------------------------
# lua
ln_make_external_find_path(lua_ROOT "lua")

find_library(lua_LIBRARY_RELEASE NAMES lua PATHS ${lua_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(lua_LIBRARY_DEBUG NAMES luad lua PATHS ${lua_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(lua STATIC IMPORTED)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION_RELEASE "${lua_LIBRARY_RELEASE}")
set_target_properties(lua PROPERTIES IMPORTED_LOCATION_DEBUG "${lua_LIBRARY_DEBUG}")
set_target_properties(lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${lua_ROOT}/include)
list(APPEND LN_EXTERNAL_LIBS lua)

#--------------------------------------
# nanovg
ln_make_external_find_path(nanovg_ROOT "nanovg")

find_library(nanovg_LIBRARY_RELEASE NAMES nanovg PATHS ${nanovg_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(nanovg_LIBRARY_DEBUG NAMES nanovgd nanovg PATHS ${nanovg_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

add_library(nanovg STATIC IMPORTED)
set_target_properties(nanovg PROPERTIES IMPORTED_LOCATION_RELEASE "${nanovg_LIBRARY_RELEASE}")
set_target_properties(nanovg PROPERTIES IMPORTED_LOCATION_DEBUG "${nanovg_LIBRARY_DEBUG}")
set_target_properties(nanovg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${nanovg_ROOT}/include)
list(APPEND LN_EXTERNAL_LIBS nanovg)

#--------------------------------------
# FBX SDK

if (LN_USE_FBX_IMPORTER)
    if (WIN32)
        set(FBX_ROOT "${LN_FBX_SDK_PATH}")
        add_library(LN::FBX STATIC IMPORTED)
        set_target_properties(LN::FBX PROPERTIES IMPORTED_LOCATION_RELEASE "${FBX_ROOT}/lib/vs2017/x64/release/libfbxsdk-mt.lib")
        set_target_properties(LN::FBX PROPERTIES IMPORTED_LOCATION_DEBUG "${FBX_ROOT}/lib/vs2017/x64/debug/libfbxsdk-mt.lib")
        set_target_properties(LN::FBX PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FBX_ROOT}/include")
        list(APPEND LN_EXTERNAL_LIBS LN::FBX)
    endif()
endif()

#--------------------------------------
# Effekseer

set(TEST_Effekseer_IncludeDirs
    #"${Effekseer_ROOT}/include"
    "D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/Effekseer;D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/EffekseerRendererVulkan;D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/EffekseerRendererLLGI;D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/3rdParty/LLGI/src"
    #"D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/EffekseerRendererDX12"
    #"D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/EffekseerRendererLLGI"
    #"D:/Proj/LN/Lumino/build/ExternalSource/Effekseer/Dev/Cpp/3rdParty/LLGI/src"
    )

ln_make_external_find_path(Effekseer_ROOT "Effekseer")
find_library(Effekseer_LIBRARY_RELEASE NAMES Effekseer PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(Effekseer_LIBRARY_DEBUG NAMES Effekseerd Effekseer PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(EffekseerRendererDX12_LIBRARY_RELEASE NAMES EffekseerRendererDX12 PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(EffekseerRendererDX12_LIBRARY_DEBUG NAMES EffekseerRendererDX12d EffekseerRendererDX12 PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(EffekseerRendererVulkan_LIBRARY_RELEASE NAMES EffekseerRendererVulkan PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(EffekseerRendererVulkan_LIBRARY_DEBUG NAMES EffekseerRendererVulkand EffekseerRendererVulkan PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(LLGI_LIBRARY_RELEASE NAMES LLGI PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)
find_library(LLGI_LIBRARY_DEBUG NAMES LLGId LLGI PATHS ${Effekseer_ROOT} PATH_SUFFIXES lib NO_CMAKE_SYSTEM_PATH)

# TODO: test
#set(EffekseerRendererVulkan_LIBRARY_DEBUG "D:/Proj/LN/Lumino/build/MSVC2017-x86-MD/ExternalBuild/Effekseer/Dev/Cpp/EffekseerRendererVulkan/Debug/EffekseerRendererVulkan.lib")
#set(EffekseerRendererVulkan_LIBRARY_RELEASE "D:/Proj/LN/Lumino/build/MSVC2017-x86-MD/ExternalBuild/Effekseer/Dev/Cpp/EffekseerRendererVulkan/Release/EffekseerRendererVulkan.lib")
#set(LLGI_LIBRARY_DEBUG "D:/Proj/LN/Lumino/build/MSVC2017-x86-MD/ExternalBuild/Effekseer/Dev/Cpp/3rdParty/LLGI/src/Debug/LLGI.lib")
#set(LLGI_LIBRARY_RELEASE "D:/Proj/LN/Lumino/build/MSVC2017-x86-MD/ExternalBuild/Effekseer/Dev/Cpp/3rdParty/LLGI/src/Release/LLGI.lib")

add_library(Effekseer STATIC IMPORTED)
set_target_properties(Effekseer PROPERTIES IMPORTED_LOCATION_RELEASE ${Effekseer_LIBRARY_RELEASE})
set_target_properties(Effekseer PROPERTIES IMPORTED_LOCATION_DEBUG ${Effekseer_LIBRARY_DEBUG})
set_target_properties(Effekseer PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${TEST_Effekseer_IncludeDirs}")

add_library(EffekseerRendererDX12 STATIC IMPORTED)
set_target_properties(EffekseerRendererDX12 PROPERTIES IMPORTED_LOCATION_RELEASE ${EffekseerRendererDX12_LIBRARY_RELEASE})
set_target_properties(EffekseerRendererDX12 PROPERTIES IMPORTED_LOCATION_DEBUG ${EffekseerRendererDX12_LIBRARY_DEBUG})

add_library(EffekseerRendererVulkan STATIC IMPORTED)
set_target_properties(EffekseerRendererVulkan PROPERTIES IMPORTED_LOCATION_RELEASE ${EffekseerRendererVulkan_LIBRARY_RELEASE})
set_target_properties(EffekseerRendererVulkan PROPERTIES IMPORTED_LOCATION_DEBUG ${EffekseerRendererVulkan_LIBRARY_DEBUG})

add_library(LLGI STATIC IMPORTED)
set_target_properties(LLGI PROPERTIES IMPORTED_LOCATION_RELEASE ${LLGI_LIBRARY_RELEASE})
set_target_properties(LLGI PROPERTIES IMPORTED_LOCATION_DEBUG ${LLGI_LIBRARY_DEBUG})
