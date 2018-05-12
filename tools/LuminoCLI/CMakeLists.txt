cmake_minimum_required (VERSION 3.0.0)
project(lumino)

set(LN_SOURCES
	LuminoCLI.PCH.cpp
	LuminoCLI.PCH.hpp
	Main.cpp
)

include_directories(
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "LuminoCLI.PCH.h" "LuminoCLI.PCH.cpp")



