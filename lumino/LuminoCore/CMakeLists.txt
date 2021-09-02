cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/LuminoCommon.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LOCAL_SOURCES "src/*.hpp" "src/*.cpp")
file(GLOB LOCAL_SOURCES_Base "include/LuminoCore/Base/*.hpp" "src/Base/*.cpp" "src/Base/*.hpp")
file(GLOB LOCAL_SOURCES_Base_TextEncoding "include/LuminoCore/Base/TextEncoding/*.hpp" "src/Base/TextEncoding/*.cpp" "src/Base/TextEncoding/*.hpp")

source_group("" FILES ${LOCAL_SOURCES})
source_group("Base" FILES ${LOCAL_SOURCES_Base})
source_group("Base/TextEncoding" FILES ${LOCAL_SOURCES_Base_TextEncoding})

set(LOCAL_SOURCES
    ${LOCAL_SOURCES}
    ${LOCAL_SOURCES_Base}
    ${LOCAL_SOURCES_Base_TextEncoding}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

if (WIN32)
    list(APPEND LOCAL_LIBS "imagehlp.lib")
    list(APPEND LOCAL_LIBS "Shlwapi.lib")

elseif (APPLE)
    set(APPEND LOCAL_LIBS pthread)
    set(APPEND LOCAL_LIBS "-framework Foundation")
    
elseif (UNIX)
    set(APPEND LOCAL_LIBS pthread)

endif()

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LOCAL_LIBS} fmt::fmt)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

if (LN_EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread")
endif()


#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoCore-config
    ARCHIVE     DESTINATION ./lib       # Library installation location (Copied at "INSTALL" build time)
    INCLUDES    DESTINATION ./include   # Include directory (File is not copied)
    )

install(DIRECTORY include DESTINATION .)
#install(FILES include/LuminoCore.hpp DESTINATION include)

install(
    EXPORT LuminoCore-config
    FILE LuminoCore-config.cmake
    DESTINATION lib/cmake/LuminoCore
    EXPORT_LINK_INTERFACE_LIBRARIES)

#install(
#    FILES LuminoCore-config.cmake
#    DESTINATION lib/cmake/LuminoCore)


#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

add_subdirectory(test)
