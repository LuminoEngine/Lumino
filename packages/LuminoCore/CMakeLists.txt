cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)
include(cmake/LuminoConfig-LuminoCore.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

include(LuminoCoreSources.cmake)

file(GLOB LN_LOCAL_SOURCES_Geometries include/LuminoCore/Geometries/*.hpp src/Geometries/*.cpp src/Geometries/*.hpp)
file(GLOB LN_LOCAL_SOURCES_Runtime include/LuminoCore/Runtime/*.hpp src/Runtime/*.cpp src/Runtime/*.hpp)

source_group("Geometries" FILES ${LN_LOCAL_SOURCES_Geometries})
source_group("Runtime" FILES ${LN_LOCAL_SOURCES_Runtime})

set(LN_SOURCES
    ${LN_SOURCES_LuminoCore}
    ${LN_LOCAL_SOURCES_Geometries}
    ${LN_LOCAL_SOURCES_Runtime}
    "src/LuminoCore.PCH.h"
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

if (WIN32)
    list(APPEND LN_LINK_LIBS "imagehlp.lib")
    list(APPEND LN_LINK_LIBS "Shlwapi.lib")

elseif (APPLE)
    set(APPEND LN_LINK_LIBS pthread)
    set(APPEND LN_LINK_LIBS "-framework Foundation")
    
elseif (UNIX)
    set(APPEND LN_LINK_LIBS pthread)

endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "include"
    "src"
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LN_LINK_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/LuminoCore.PCH.h")

if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread")
endif()

if (LN_BUILD_XCODE_FRAMEWORK)
    # Build as framework.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        FRAMEWORK TRUE
        #VERSION 1.0.0
        #SOVERSION 1.0.0
        MACOSX_FRAMEWORK_IDENTIFIER lib.Lumino
        BUILD_WITH_INSTALL_RPATH 1
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMIRY "1,2"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    # Do not use PUBLIC_HEADERS.
    # Since PUBLIC_HEADERS does not maintain the folder tree,
    # install the header file with the install command.
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/Lumino DESTINATION "LuminoCore.framework/Headers")
    install(FILES ${PROJECT_SOURCE_DIR}/include/LuminoCore.hpp DESTINATION "LuminoCore.framework/Headers")
endif()

# add debug view definition file for msvc (natvis)
if (MSVC)
    set(LN_ADD_NATVIS TRUE)
    set(LN_NATVIS_FILE "LuminoCore.natvis")
    set(LN_NATVIS_INSTALL_DIR "share/lumino")
    set(LN_NATVIS_INSTALL_PATH "share/lumino/LuminoCore.natvis")
    target_sources(
        ${PROJECT_NAME}
        INTERFACE
            $<INSTALL_INTERFACE:${LN_NATVIS_INSTALL_PATH}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LN_NATVIS_FILE}>
    )
endif()

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    set(LN_TARGETS_NAME "${PROJECT_NAME}Targets")
    set(LN_CONFIG_NAME "${PROJECT_NAME}Config")

    install(
        TARGETS     ${PROJECT_NAME}
        EXPORT      LuminoExport
    )

    install(DIRECTORY include/LuminoCore DESTINATION ./include)
    install(FILES include/LuminoCore.hpp DESTINATION include)

    # install(EXPORT) で出力される LuminoCoreConfig.cmake は、同一フォルダの
    # LuminoCoreConfig-*.cmake を include しようとするため、それにマッチするファイル名にする。
    install(FILES cmake/LuminoConfig-Common.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
    install(FILES cmake/LuminoConfig-LuminoCore.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../../../../lib/${LN_TARGET_ARCH})
        
    if (LN_ADD_NATVIS)
        install(
            FILES ${LN_NATVIS_FILE}
            DESTINATION ${LN_NATVIS_INSTALL_DIR}
        )
    endif()
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()
