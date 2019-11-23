
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)

#------------------------------------------------------------------------------
# Files

file(GLOB LOCAL_HEADERS include/*.h)
file(GLOB LOCAL_SOURCES src/*.cpp)
source_group("Sources" FILES ${LOCAL_SOURCES} ${LOCAL_HEADERS})

#------------------------------------------------------------------------------
# Targets

# Shared lib
add_library(${PROJECT_NAME} SHARED ${LOCAL_SOURCES} ${LOCAL_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${LOCAL_HEADERS}")

# Static lib
add_library(${PROJECT_NAME}-Static STATIC ${LOCAL_SOURCES} ${LOCAL_HEADERS})
target_include_directories(${PROJECT_NAME}-Static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
target_link_libraries(${PROJECT_NAME}-Static LuminoEngine-static)
set_target_properties(${PROJECT_NAME}-Static PROPERTIES FOLDER "Lumino")

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME     DESTINATION lib
    ARCHIVE     DESTINATION lib
    LIBRARY     DESTINATION lib)

install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
	DESTINATION "lib/cmake")

install(
    FILES "${PROJECT_NAME}Config.cmake"
    DESTINATION "lib/cmake")

#------------------------------------------------------------------------------
# Test

add_subdirectory(test)
