cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-Test)

set(LN_SOURCES
	${LN_SOURCES_gtest}
	Common.hpp
	Main.cpp
	Test_Asset_AssetArchive.cpp
	Test_Engine_Object.cpp
	Test_Graphics_Bitmap.cpp
	Test_Graphics_HlslEffect.cpp
	Test_Graphics_LowLevelRendering.cpp
	Test_Scene_Sprite.cpp
	Test_Scene_WorldObject.cpp
	Test_Shader_Shader.cpp
	Test_UI_UIElement.cpp
	Test_UI_UISprite.cpp
	Test_Visual_VisualComponent.cpp
	TestEnv.cpp
	TestEnv.hpp
)

include_directories(
	${LN_INCLUDES_gtest}
	#${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
