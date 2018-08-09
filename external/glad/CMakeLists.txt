project(glad)

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${PROJECT_NAME} STATIC src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib/${LN_TARGET_ARCH_NAME})

install(
	EXPORT ${PROJECT_NAME}Targets
	DESTINATION "lib/${LN_TARGET_ARCH_NAME}/cmake")
