project(lumino)

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
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.hpp")

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

