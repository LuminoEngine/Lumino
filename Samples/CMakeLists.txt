
set(LN_SOURCES_SampleBrowser
	Sample_HelloWorld.cpp
	Sample_DebugAndTestSupport.cpp
	Sample_Sprite2D.cpp
	SampleBrowser.cpp
	SampleBrowser.PCH.cpp
	SampleBrowser.PCH.h
)

include_directories(
	"${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include"
	"${PROJECT_SOURCE_DIR}/Source/LuminoEngine/Include"
)

add_executable(SampleBrowser ${LN_SOURCES_SampleBrowser})
target_link_libraries(SampleBrowser LuminoEngine LuminoLaunch)
add_dependencies(SampleBrowser LuminoEngine LuminoLaunch)
ln_add_pch(SampleBrowser "SampleBrowser.PCH.h" "SampleBrowser.PCH.cpp")

