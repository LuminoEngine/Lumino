project(luminosc)
find_package(CLI11 CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("" FILES ${LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    .
    "../LuminoFFI/include"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    CLI11::CLI11
    LuminoCore
    LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "tools")
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.hpp")

if(LN_DESKTOP_WEBGPU)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

