
set(_LN_REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

include("${_LN_REPO_ROOT}/external/ImportExternalLibraries.cmake")
include("${_LN_REPO_ROOT}/src/LuminoCommon.cmake")	# for ln_add_pch

set(_LN_INCLUDE_DIR "${_LN_REPO_ROOT}/include")
set(_LN_CMAKE_BUILD_DIR "${_LN_REPO_ROOT}/build/MSVC2017-x86-MD")

foreach(LIB IN LISTS LN_EXTERNAL_LIBS)
    list(APPEND _LN_LINK_ONLY_LIBS "\$<LINK_ONLY:${LIB}>")
endforeach()

#message("_LN_LINK_ONLY_LIBS:${_LN_LINK_ONLY_LIBS}")

# LuminoCore
find_library(LuminoCore_LIBRARY_RELEASE NAMES LuminoCore libLuminoCore PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoCore/Release" NO_CMAKE_SYSTEM_PATH)
find_library(LuminoCore_LIBRARY_DEBUG NAMES LuminoCored libLuminoCored PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoCore/Debug" NO_CMAKE_SYSTEM_PATH)
add_library(LuminoCore STATIC IMPORTED)
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoCore_LIBRARY_RELEASE}")
set_target_properties(LuminoCore PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoCore_LIBRARY_DEBUG}")
set_target_properties(LuminoCore PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})

# LuminoEngine
find_library(LuminoEngine_LIBRARY_RELEASE NAMES LuminoEngine libLuminoEngine PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEngine/Release" NO_CMAKE_SYSTEM_PATH)
find_library(LuminoEngine_LIBRARY_DEBUG NAMES LuminoEngined libLuminoEngined PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEngine/Debug" NO_CMAKE_SYSTEM_PATH)
add_library(LuminoEngine STATIC IMPORTED)
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoEngine_LIBRARY_RELEASE}")
set_target_properties(LuminoEngine PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoEngine_LIBRARY_DEBUG}")
set_target_properties(LuminoEngine PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${_LN_INCLUDE_DIR})
set_target_properties(LuminoEngine PROPERTIES INTERFACE_LINK_LIBRARIES "LuminoCore;${_LN_LINK_ONLY_LIBS}")

# LuminoEditor
find_library(LuminoEditor_LIBRARY_RELEASE NAMES EditorCore libEditorCore PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEditor/Core/Release" NO_CMAKE_SYSTEM_PATH)
find_library(LuminoEditor_LIBRARY_DEBUG NAMES EditorCored libEditorCored PATHS "${_LN_CMAKE_BUILD_DIR}/src/LuminoEditor/Core/Debug" NO_CMAKE_SYSTEM_PATH)
add_library(LuminoEditor STATIC IMPORTED)
set_target_properties(LuminoEditor PROPERTIES IMPORTED_LOCATION_RELEASE "${LuminoEditor_LIBRARY_RELEASE}")
set_target_properties(LuminoEditor PROPERTIES IMPORTED_LOCATION_DEBUG "${LuminoEditor_LIBRARY_DEBUG}")
set_target_properties(LuminoEditor PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${_LN_REPO_ROOT}/src/LuminoEditor/Core")
