

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
#set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(_LN_LIBRARY_DIR "${LUMINO_ENGINE_ROOT}/lib")
set(_LN_INCLUDE_DIR "${LUMINO_ENGINE_ROOT}/include")
#set(LN_TARGET_ARCH "MSVC2017-x86-MD")
set(LN_EXTERNAL_FIND_PATH_MODE "config")

message("_LN_LIBRARY_DIR: ${_LN_LIBRARY_DIR}")
message("_LN_INCLUDE_DIR: ${_LN_INCLUDE_DIR}")

#set ( LUMINO_FOUND 1 )
#set ( LUMINO_INCLUDE_DIR  "${LUMINO_ENGINE_ROOT}/include" )
#set ( LUMINO_INCLUDE_DIRS "${LUMINO_ENGINE_ROOT}/include" )
#set ( LUMINO_LIBRARIES    "" )
#set ( LUMINO_LIBRARY_DIRS "${LUMINO_ENGINE_ROOT}/lib/${LN_TARGET_ARCH}" )
#set ( LUMINO_ROOT_DIR     ${LUMINO_ENGINE_ROOT} )
#set ( Lumino_DIR     ${LUMINO_ENGINE_ROOT} )


include(${CMAKE_CURRENT_LIST_DIR}/LuminoCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ImportExternalLibraries.cmake)

list(REVERSE LN_EXTERNAL_LIBS)
foreach(LIB IN LISTS LN_EXTERNAL_LIBS)
    list(APPEND _LN_LINK_ONLY_LIBS "\$<LINK_ONLY:${LIB}>")
endforeach()

#------------------------------------------------------------------------------
# LuminoCore
add_library(LuminoCore STATIC IMPORTED)
find_library(LuminoCore_LIBRARY_RELEASE NAMES LuminoCore PATHS ${_LN_LIBRARY_DIR})
find_library(LuminoCore_LIBRARY_DEBUG NAMES LuminoCored PATHS ${_LN_LIBRARY_DIR})
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_RELEASE ${LuminoCore_LIBRARY_RELEASE})
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_DEBUG ${LuminoCore_LIBRARY_DEBUG})
set_target_properties(LuminoCore PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})

#------------------------------------------------------------------------------
# LuminoEngine
add_library(LuminoEngine STATIC IMPORTED)
find_library(LuminoEngine_LIBRARY_RELEASE NAMES LuminoEngine libLuminoEngine PATHS ${_LN_LIBRARY_DIR})
find_library(LuminoEngine_LIBRARY_DEBUG NAMES LuminoEngined libLuminoEngined PATHS ${_LN_LIBRARY_DIR})
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_RELEASE ${LuminoEngine_LIBRARY_RELEASE})
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_DEBUG ${LuminoEngine_LIBRARY_DEBUG})
set_target_properties(LuminoEngine PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})
set_target_properties(LuminoEngine PROPERTIES INTERFACE_LINK_LIBRARIES "LuminoCore;${_LN_LINK_ONLY_LIBS}")

#------------------------------------------------------------------------------
# LuminoEditor
#find_library(LuminoEditor_LIBRARY_RELEASE NAMES LuminoEditor libLuminoEditor PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEditor/Core/Release" NO_CMAKE_SYSTEM_PATH)
#find_library(LuminoEditor_LIBRARY_DEBUG NAMES LuminoEditord libLuminoEditord PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEditor/Core/Debug" NO_CMAKE_SYSTEM_PATH)
#add_library(LuminoEditor STATIC IMPORTED)
#set_target_properties(LuminoEditor PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoEditor_LIBRARY_RELEASE}")
#set_target_properties(LuminoEditor PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoEditor_LIBRARY_DEBUG}")
#set_target_properties(LuminoEditor PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${_LN_REPO_ROOT}/src/LuminoEditor/Core")

#------------------------------------------------------------------------------
# Lumino
set(_LN_LIBS LuminoCore LuminoEngine)

foreach(lib IN LISTS LN_EXTERNAL_LIBS)
    message("llll:${lib}")
    list(APPEND _LN_LIBS "\$<LINK_ONLY:${lib}>")
endforeach()
message("_LN_LIBS:${_LN_LIBS}")


