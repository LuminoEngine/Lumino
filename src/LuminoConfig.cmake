

set(LUMINO_ENGINE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")
set(LN_TARGET_ARCH "MSVC2017-x86-MD")
#set( Lumino_ROOT ${LUMINO_ENGINE_ROOT})
set(LN_LIBRARY_DIR "${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}")



set ( LUMINO_FOUND 1 )
set ( LUMINO_INCLUDE_DIR  "${LUMINO_ENGINE_ROOT}/include" )
set ( LUMINO_INCLUDE_DIRS "${LUMINO_ENGINE_ROOT}/include" )
set ( LUMINO_LIBRARIES    "" )
set ( LUMINO_LIBRARY_DIRS "${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}" )
set ( LUMINO_ROOT_DIR     ${LUMINO_ENGINE_ROOT} )
set ( Lumino_DIR     ${LUMINO_ENGINE_ROOT} )


include(${CMAKE_CURRENT_LIST_DIR}/LuminoCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ImportExternalLibraries.cmake)

#include("${CMAKE_CURRENT_LIST_DIR}/../../LuminoSetup.cmake")

message("LN_LIBRARY_DIR:${LN_LIBRARY_DIR}")

#------------------------------------------------------------------------------
# LuminoCore
add_library(LuminoCore STATIC IMPORTED)
find_library(LuminoCore_LIBRARY_RELEASE NAMES LuminoCore PATHS ${LN_LIBRARY_DIR})
find_library(LuminoCore_LIBRARY_DEBUG NAMES LuminoCored PATHS ${LN_LIBRARY_DIR})
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_RELEASE ${LuminoCore_LIBRARY_RELEASE})
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_DEBUG ${LuminoCore_LIBRARY_DEBUG})

#------------------------------------------------------------------------------
# LuminoEngine
add_library(LuminoEngine STATIC IMPORTED)
find_library(LuminoEngine_LIBRARY_RELEASE NAMES LuminoEngine PATHS ${LN_LIBRARY_DIR})
find_library(LuminoEngine_LIBRARY_DEBUG NAMES LuminoEngined PATHS ${LN_LIBRARY_DIR})
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_RELEASE ${LuminoEngine_LIBRARY_RELEASE})
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_DEBUG ${LuminoEngine_LIBRARY_DEBUG})

#------------------------------------------------------------------------------
# Lumino
set(_LN_LIBS LuminoCore LuminoEngine)

foreach(lib IN LISTS LN_EXTERNAL_LIBS)
    message("llll:${lib}")
    list(APPEND _LN_LIBS "\$<LINK_ONLY:${lib}>")
endforeach()
message("_LN_LIBS:${_LN_LIBS}")




add_library(Lumino INTERFACE IMPORTED)
set_target_properties(Lumino PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LUMINO_INCLUDE_DIR}"
  INTERFACE_LINK_LIBRARIES "${_LN_LIBS}"
#  INTERFACE_LINK_LIBRARIES "LuminoCore;LuminoEngine;opengl32;glu32;glslang;HLSL;OGLCompiler;OSDependent;SPIRV;SPVRemapper;spirv-cross-core;spirv-cross-glsl;\$<LINK_ONLY:LuminoCore>;\$<LINK_ONLY:pcre>;\$<LINK_ONLY:ZLIB>;\$<LINK_ONLY:PNG>;\$<LINK_ONLY:FreeType>;\$<LINK_ONLY:vorbisfile>;\$<LINK_ONLY:vorbis>;\$<LINK_ONLY:ogg>;\$<LINK_ONLY:BulletSoftBody>;\$<LINK_ONLY:BulletDynamics>;\$<LINK_ONLY:BulletCollision>;\$<LINK_ONLY:LinearMath>;\$<LINK_ONLY:tmxlite>;\$<LINK_ONLY:glfw>;\$<LINK_ONLY:GLAD>;\$<LINK_ONLY:winmm>;\$<LINK_ONLY:imm32>;\$<LINK_ONLY:version>;\$<LINK_ONLY:OpenAL>"
)


