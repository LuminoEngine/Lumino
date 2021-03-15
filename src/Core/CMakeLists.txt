#
# Required variables:
#

cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

#include(${PROJECT_SOURCE_DIR}/../LuminoCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
include(LuminoCoreSources.cmake)

set(LN_SOURCES
    ${LN_SOURCES_LuminoCore}
    src/LuminoCore.PCH.cpp
    src/LuminoCore.PCH.h)

#------------------------------------------------------------------------------
# Apply options

if (WIN32)
    list(APPEND LN_LINK_LIBS "imagehlp.lib")
    list(APPEND LN_LINK_LIBS "Shlwapi.lib")

elseif (APPLE)
    set(APPEND LN_LINK_LIBS pthread)
    set(APPEND LN_LINK_LIBS "-framework Foundation")
    
elseif (UNIX)
    set(APPEND LN_LINK_LIBS pthread)

endif()

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC  
    $<BUILD_INTERFACE:${LN_CORE_INCLUDE_DIR}>  
    #$<INSTALL_INTERFACE:../../include>  # <prefix>/include
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoCore.PCH.h" "src/LuminoCore.PCH.cpp")

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

# Unit test.
if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoCoreTargets
    ARCHIVE     DESTINATION ./lib       # Library installation location (Copied at "INSTALL" build time)
    INCLUDES    DESTINATION ./include   # Include directory (File is not copied)
    FRAMEWORK DESTINATION .)

install(DIRECTORY ../../include/LuminoCore DESTINATION ./include)
install(FILES ../../include/LuminoCore.hpp DESTINATION include)

install(
    EXPORT LuminoCoreTargets
    FILE LuminoCoreTargets.cmake
    DESTINATION lib/cmake/LuminoCore
    EXPORT_LINK_INTERFACE_LIBRARIES)

install(
    FILES LuminoCoreConfig.cmake
    DESTINATION lib/cmake/LuminoCore)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../../../../lib/${LN_TARGET_ARCH})
