
# FIXME: Emscripten と Android のビルド時、find_library が sysroot 以外を探せるように強制。
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

#-------------------------------------------------------------------------------

# Visual Studio ソリューションフォルダを作るようにする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# サードパーティライブラリ用の共通設定
macro(ln_add_dependencies_common_property_full projectName includeDir)
    set_target_properties(${projectName} PROPERTIES PREFIX "")
    set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")    # Visual Studio solution folder
    set(LN_DEPENDENCIES_LIBRARIES ${LN_DEPENDENCIES_LIBRARIES} ${projectName})
    set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${includeDir})
endmacro()
macro(ln_add_dependencies_common_property projectName)
    set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")    # Visual Studio solution folder
endmacro()
macro(ln_mark_non_dependencies projectName)
    set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")    # Visual Studio solution folder
endmacro()

if (MSVC)
    add_definitions("/wd4996")        # pragma warning disable
endif()


#-------------------------------------------------------------------------------

message("EMSCRIPTEN_ROOT_PATH: ${EMSCRIPTEN_ROOT_PATH}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")




macro(ln_make_external_find_path varName projectDirName)
    #if (LN_EMSCRIPTEN)
        # reference to installed libs by "build.csproj"
        #set(${varName} ${EMSCRIPTEN_ROOT_PATH}/system)
    #else
    if(DEFINED LN_EXTERNAL_FIND_PATH_MODE)
        if (${LN_EXTERNAL_FIND_PATH_MODE} STREQUAL "build")
            set(${varName} ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/${projectDirName})
        endif()
    else()
    set(${varName} ${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH})
    endif()
endmacro()



#--------------------------------------
# glfw
if (LN_OS_DESKTOP)
    set(GLFW_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/glfw)
    find_library(GLFW_LIBRARY_RELEASE NAMES glfw3 libglfw3 PATHS ${GLFW_ROOT} PATH_SUFFIXES lib)
    find_library(GLFW_LIBRARY_DEBUG NAMES glfw3d libglfw3d PATHS ${GLFW_ROOT} PATH_SUFFIXES lib)

    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION_RELEASE "${GLFW_LIBRARY_RELEASE}")
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION_DEBUG "${GLFW_LIBRARY_DEBUG}")
    set_target_properties(glfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLFW_ROOT}/include)
endif()

#--------------------------------------
# glad
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glad)
#set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} "${CMAKE_CURRENT_LIST_DIR}/glad/include")
#ln_add_dependencies_common_property(glad)

if (LN_OS_DESKTOP)# OR LN_EMSCRIPTEN)

    set(LIB_NAME GLAD)
    add_library(${LIB_NAME} STATIC IMPORTED)

    #if (LN_EMSCRIPTEN)
        # reference to installed libs by "build.csproj"
    #    set(GLAD_ROOT ${EMSCRIPTEN_ROOT_PATH}/system)
    #else()
        set(GLAD_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/glad)
        find_library(GLAD_LIBRARY_RELEASE NAMES glad libglad PATHS ${GLAD_ROOT} PATH_SUFFIXES lib)
        find_library(GLAD_LIBRARY_DEBUG NAMES gladd libgladd PATHS ${GLAD_ROOT} PATH_SUFFIXES lib)
        set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
        set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
    #endif()

    set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${LIB_NAME}_ROOT}/include)
endif()

#--------------------------------------
# zlib

if (ANDROID_ABI)
    # zlib を "ZLIB" 変数で参照できるようにする
    find_package(ZLIB REQUIRED)
    add_library(ZLIB STATIC IMPORTED)
    set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION "${ZLIB_LIBRARIES}")
    set_target_properties(ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS})
    
elseif (LN_OS_DESKTOP OR DEFINED EMSCRIPTEN)
    ln_make_external_find_path(ZLIB_ROOT zlib)
    find_library(ZLIB_LIBRARY_RELEASE NAMES zlib PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib)
    find_library(ZLIB_LIBRARY_DEBUG NAMES zlibd PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib)

    set(LIB_NAME ZLIB)
    add_library(${LIB_NAME} STATIC IMPORTED)
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
    set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_ROOT}/include)

elseif (LN_IOS)
    # use system zlib.
    find_package(ZLIB REQUIRED)
    add_library(ZLIB STATIC IMPORTED)
    set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION_RELEASE ${ZLIB_LIBRARIES})
    set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION_DEBUG ${ZLIB_LIBRARIES})
    set_target_properties(ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS})

else()
endif()

#--------------------------------------
# libpng

ln_make_external_find_path(PNG_ROOT libpng)
find_library(PNG_LIBRARY_RELEASE NAMES libpng16 png16 PATHS ${PNG_ROOT} PATH_SUFFIXES lib)
find_library(PNG_LIBRARY_DEBUG NAMES libpng16d png16d PATHS ${PNG_ROOT} PATH_SUFFIXES lib)

set(LIB_NAME PNG)
add_library(${LIB_NAME} STATIC IMPORTED)
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${PNG_ROOT}/include)


#--------------------------------------
# glslang

if (LN_OS_DESKTOP)

    ln_make_external_find_path(GLSLANG_ROOT glslang)
    #set(GLSLANG_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/glslang)
    #set(CMAKE_PREFIX_PATH ${GLSLANG_ROOT})        # for Linux

    find_library(glslang_LIBRARY_RELEASE NAMES libglslang glslang PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(glslang_LIBRARY_DEBUG NAMES libglslangd glslangd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(HLSL_LIBRARY_RELEASE NAMES libHLSL HLSL PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(HLSL_LIBRARY_DEBUG NAMES libHLSLd HLSLd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(OGLCompiler_LIBRARY_RELEASE NAMES libOGLCompiler OGLCompiler PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(OGLCompiler_LIBRARY_DEBUG NAMES libOGLCompilerd OGLCompilerd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(OSDependent_LIBRARY_RELEASE NAMES libOSDependent OSDependent PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(OSDependent_LIBRARY_DEBUG NAMES libOSDependentd OSDependentd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_LIBRARY_RELEASE NAMES libSPIRV SPIRV PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_LIBRARY_DEBUG NAMES libSPIRVd SPIRVd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(SPVRemapper_LIBRARY_RELEASE NAMES libSPVRemapper SPVRemapper PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)
    find_library(SPVRemapper_LIBRARY_DEBUG NAMES libSPVRemapperd SPVRemapperd PATHS ${GLSLANG_ROOT} PATH_SUFFIXES lib)

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
endif()

#--------------------------------------
# SPIRV-Cross

if (LN_OS_DESKTOP)
    ln_make_external_find_path(SPIRV-Cross_ROOT SPIRV-Cross)
    #set(SPIRV-Cross_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalInstall/SPIRV-Cross)

    find_library(spirv-cross-core_LIBRARY_RELEASE NAMES spirv-cross-core PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
    find_library(spirv-cross-core_LIBRARY_DEBUG NAMES spirv-cross-cored PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
    find_library(spirv-cross-glsl_LIBRARY_RELEASE NAMES spirv-cross-glsl PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)
    find_library(spirv-cross-glsl_LIBRARY_DEBUG NAMES spirv-cross-glsld PATHS ${SPIRV-Cross_ROOT} PATH_SUFFIXES lib)

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
endif()

#--------------------------------------
# openal-soft

if (LN_EMSCRIPTEN)

else()
    ln_make_external_find_path(OpenAL_ROOT openal-soft)

    find_library(OpenAL_LIBRARY_RELEASE NAMES OpenAL32 PATHS ${OpenAL_ROOT} PATH_SUFFIXES lib)
    find_library(OpenAL_LIBRARY_DEBUG NAMES OpenAL32d PATHS ${OpenAL_ROOT} PATH_SUFFIXES lib)

    add_library(OpenAL STATIC IMPORTED)
    set_target_properties(OpenAL PROPERTIES IMPORTED_LOCATION_RELEASE "${OpenAL_LIBRARY_RELEASE}")
    set_target_properties(OpenAL PROPERTIES IMPORTED_LOCATION_DEBUG "${OpenAL_LIBRARY_DEBUG}")
    set_target_properties(OpenAL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OpenAL_ROOT}/include)
endif()

#--------------------------------------
# SDL2

if (LN_USE_SDL)
    ln_make_external_find_path(SDL2_ROOT SDL2)
    find_library(SDL2_LIBRARY_RELEASE NAMES SDL2 PATHS ${SDL2_ROOT} PATH_SUFFIXES lib)
    find_library(SDL2_LIBRARY_DEBUG NAMES SDL2d PATHS ${SDL2_ROOT} PATH_SUFFIXES lib)

    set(LIB_NAME SDL2)
    add_library(${LIB_NAME} STATIC IMPORTED)
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_RELEASE "${${LIB_NAME}_LIBRARY_RELEASE}")
    set_target_properties(${LIB_NAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${${LIB_NAME}_LIBRARY_DEBUG}")
    set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SDL2_ROOT}/include)
endif()

#--------------------------------------
# freetype

ln_make_external_find_path(FreeType_ROOT "freetype2")

find_library(FreeType_LIBRARY_RELEASE NAMES freetype libfreetype PATHS ${FreeType_ROOT} PATH_SUFFIXES lib)
find_library(FreeType_LIBRARY_DEBUG NAMES freetyped libfreetyped PATHS ${FreeType_ROOT} PATH_SUFFIXES lib)

add_library(FreeType STATIC IMPORTED)
set_target_properties(FreeType PROPERTIES IMPORTED_LOCATION_RELEASE "${FreeType_LIBRARY_RELEASE}")
set_target_properties(FreeType PROPERTIES IMPORTED_LOCATION_DEBUG "${FreeType_LIBRARY_DEBUG}")
set_target_properties(FreeType PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${FreeType_ROOT}/include/freetype2)
