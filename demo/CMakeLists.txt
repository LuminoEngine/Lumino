
set(LN_DEMO_SOURCES
	Main.cpp
	Common.h)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include")
link_libraries(Lumino_Static)
add_executable(Lumino_Demo ${LN_DEMO_SOURCES})
