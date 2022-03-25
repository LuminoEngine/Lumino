
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)
include(cmake/LuminoConfig-LuminoRuntime.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_SOURCES
    include/LuminoEngine/RuntimeModule.hpp
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    src/RegisterTypes.cpp
    src/RuntimeModule.cpp
)
file(GLOB LN_SOURCES_Runtime include/LuminoEngine/Runtime/*.hpp include/LuminoEngine/Runtime/detail/*.hpp src/Runtime/*.cpp src/Runtime/*.hpp)
file(GLOB LN_SOURCES_Base include/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)
file(GLOB LN_SOURCES_Reflection include/LuminoEngine/Reflection/*.hpp src/Reflection/*.cpp src/Reflection/*.hpp)
file(GLOB LN_SOURCES_Engine include/LuminoEngine/Engine/*.hpp src/Engine/*.cpp src/Engine/*.hpp)
file(GLOB LN_SOURCES_Asset include/LuminoEngine/Asset/*.hpp src/Asset/*.cpp src/Asset/*.hpp)
file(GLOB LN_SOURCES_Graphics include/LuminoEngine/Graphics/*.hpp src/Graphics/*.cpp src/Graphics/*.hpp)

# Grouping.
source_group("" FILES ${LN_SOURCES})
source_group("Runtime" FILES ${LN_SOURCES_Runtime})
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Reflection" FILES ${LN_SOURCES_Reflection})
source_group("Engine" FILES ${LN_SOURCES_Engine})
source_group("Asset" FILES ${LN_SOURCES_Asset})
source_group("Graphics" FILES ${LN_SOURCES_Graphics})

set(LN_SOURCES
    ${LN_SOURCES}
    ${LN_SOURCES_Runtime}
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Reflection}
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Graphics}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore yaml-cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------

install(
    TARGETS     ${PROJECT_NAME}
    EXPORT      LuminoExport
    ARCHIVE     DESTINATION ./lib
    INCLUDES    DESTINATION ./include
)

install(DIRECTORY include DESTINATION .)

# find_package(LuminoCore CONFIG REQUIRED) で ln_add_pch を使えるようにする。
# install(EXPORT) で出力される LuminoCoreConfig.cmake は、同一フォルダの
# LuminoCoreConfig-*.cmake を include しようとするため、それにマッチするファイル名にする。
install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION lib/cmake/Lumino)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------
if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()

