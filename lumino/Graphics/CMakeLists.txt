project(LuminoGraphics)
include(cmake/LuminoConfig-LuminoGraphics.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LN_SOURCES "include/LuminoGraphics/*.hpp" "include/LuminoGraphics/detail/*.hpp" "include/*.hpp" "src/*.cpp" "src/*.hpp")









source_group("" FILES ${LN_SOURCES})

set(LN_SOURCES
    ${LN_SOURCES}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


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
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH

target_compile_definitions(${PROJECT_NAME} PRIVATE ${LN_DEFINITIONS})

target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoRuntime)


set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(
        TARGETS     ${PROJECT_NAME}
        EXPORT      LuminoExport
    )
    install(DIRECTORY include DESTINATION .)
    install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
endif()

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
    add_subdirectory(sandbox)
endif()

if (LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
