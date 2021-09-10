
cmake_minimum_required(VERSION 3.0)
project(LuminoEngineCore)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_Base include/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)
file(GLOB LN_SOURCES_Reflection include/LuminoEngine/Reflection/*.hpp src/Reflection/*.cpp src/Reflection/*.hpp)

# Grouping.
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Reflection" FILES ${LN_SOURCES_Reflection})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Reflection}
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
target_include_directories(${PROJECT_NAME} PRIVATE include srce)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../LuminoEngine/include)   # TODO: 消したい
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../Platform/include)   # TODO: 消したい
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------

set(LN_TARGETS_NAME "${PROJECT_NAME}Targets")
set(LN_CONFIG_NAME "${PROJECT_NAME}Config")

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${LN_TARGETS_NAME}
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include DESTINATION .)

install(
    EXPORT ${LN_TARGETS_NAME}
    FILE ${LN_TARGETS_NAME}.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
    EXPORT_LINK_INTERFACE_LIBRARIES
)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

