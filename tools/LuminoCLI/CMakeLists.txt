cmake_minimum_required (VERSION 3.0.0)
project(lumino)

set(LN_SOURCES
	LuminoCLI.PCH.cpp
	LuminoCLI.PCH.hpp
	EnvironmentSettings.cpp
	EnvironmentSettings.hpp
	Main.cpp
	Project.cpp
	Project.hpp
	Workspace.cpp
	Workspace.hpp
)

source_group("Sources" FILES ${LN_SOURCES})

include_directories(
	.
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "LuminoCLI.PCH.hpp" "LuminoCLI.PCH.cpp")



