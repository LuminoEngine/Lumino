
set(LN_DEMO_SOURCES
	Main.cpp
	Common.h
)

set(LN_DEMO_SOURCES_Core
	Core/Demo_Core_String.cpp
)

#--------------------------------------
# Grouping.
source_group("Core"			FILES ${LN_DEMO_SOURCES_Core})


set(LN_DEMO_SOURCES
	${LN_DEMO_SOURCES}
	${LN_DEMO_SOURCES_Core}
)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/demo"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include")
link_libraries(Lumino_Static)
add_executable(Lumino_Demo ${LN_DEMO_SOURCES})
