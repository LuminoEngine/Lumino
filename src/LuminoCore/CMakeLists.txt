#
# Required variables:
#

cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

include(${PROJECT_SOURCE_DIR}/../LuminoCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
include(LuminoCoreSources.cmake)

set(LN_SOURCES
    ${LN_SOURCES_LuminoCore}
    src/LuminoCore.PCH.cpp
    src/LuminoCore.PCH.h)

set(LN_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/include)

#------------------------------------------------------------------------------
# Apply options

if (WIN32)
    list(APPEND LN_LINK_LIBS "imagehlp.lib")
    list(APPEND LN_LINK_LIBS "Shlwapi.lib")

elseif (APPLE)
    set(APPEND LN_LINK_LIBS pthread)
    
elseif (UNIX)
    set(APPEND LN_LINK_LIBS pthread)
    
endif()

#------------------------------------------------------------------------------
# Targets

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} INTERFACE ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoCore.PCH.h" "src/LuminoCore.PCH.cpp")

# Unit test.
if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoCoreTargets
    ARCHIVE     DESTINATION lib    # Library installation location (Copied at "INSTALL" build time)
    INCLUDES    DESTINATION include                    # include directory (File is not copied)
    FRAMEWORK DESTINATION .)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)

install(
    EXPORT LuminoCoreTargets
    FILE LuminoCoreTargets.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION "lib/cmake")

install(
    FILES "LuminoCoreConfig.cmake"
    DESTINATION "lib/cmake")

