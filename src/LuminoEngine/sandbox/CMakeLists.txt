cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-sandbox)

file(GLOB LOCAL_SOURCES *.cpp *.hpp)

include_directories(
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
