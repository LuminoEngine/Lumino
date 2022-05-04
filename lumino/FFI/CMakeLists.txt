project(LuminoFFI)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_FFI include/LuminoFFI/*.h include/LuminoFFI/*.hpp src/*.cpp src/*.hpp)

source_group("FFI" FILES ${LN_SOURCES_FFI})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_FFI}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoEngine)
target_link_libraries(${PROJECT_NAME} PUBLIC LuminoRuntime)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------

set(LN_TARGETS_NAME "${PROJECT_NAME}Targets")
set(LN_CONFIG_NAME "${PROJECT_NAME}Config")

install(
    TARGETS ${PROJECT_NAME}
    EXPORT LuminoExport
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include DESTINATION .)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()
