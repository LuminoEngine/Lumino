cmake_minimum_required(VERSION 3.9)
project(glad)

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${PROJECT_NAME} STATIC src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include DESTINATION .)

install(
	EXPORT ${PROJECT_NAME}Targets
	DESTINATION "lib/cmake")
