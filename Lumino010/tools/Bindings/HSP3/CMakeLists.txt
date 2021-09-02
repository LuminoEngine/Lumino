cmake_minimum_required (VERSION 3.0.0)
project(LuminoHSP3)

file(GLOB LOCAL_SOURCES "*.h" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

file(GLOB LOCAL_SOURCES_hsp3plugin "hsp3plugin/*.h" "hsp3plugin/*.cpp")
source_group("hsp3plugin" FILES ${LOCAL_SOURCES_hsp3plugin})

add_library(${PROJECT_NAME} SHARED ${LOCAL_SOURCES} ${LOCAL_SOURCES_hsp3plugin})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
#ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")

# Install shared library
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME     DESTINATION bin
    ARCHIVE     DESTINATION lib
    LIBRARY     DESTINATION lib)
