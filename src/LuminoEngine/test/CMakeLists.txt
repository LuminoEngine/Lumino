cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-Test)

set(LN_SOURCES
	${LN_SOURCES_gtest}
	Common.hpp
	Main.cpp
	Test_Asset_AssetArchive.cpp
	Test_Asset_AssetObject.cpp
	Test_Base_ObjectCache.cpp
	Test_Base_Regex.cpp
	Test_Base_Variant.cpp
	Test_Engine_Object.cpp
	Test_Graphics_Bitmap.cpp
	Test_Graphics_ColorStructs.cpp
	Test_Graphics_DeviceContext.cpp
	Test_Graphics_LowLevelRendering.cpp
	Test_Graphics_Texture.cpp
	Test_Physics_2D.cpp
	Test_Rendering_ClusteredShading.cpp
	Test_Scene_MeshPrimitive.cpp
	Test_Scene_Sprite.cpp
	Test_Scene_WorldObject.cpp
	Test_Shader_HlslEffect.cpp
	Test_Shader_Shader.cpp
	Test_UI_UIElement.cpp
	Test_UI_UILayout.cpp
	Test_UI_UISprite.cpp
	Test_UI_UIStyle.cpp
	Test_UI_UITextBlock.cpp
	Test_Visual_VisualComponent.cpp
	TestEnv.cpp
	TestEnv.hpp
)

include_directories(
	${LN_INCLUDES_gtest}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

#------------------------------------------------------------------------------
# ctest

add_test(NAME LuminoEngine-Test-all COMMAND $<TARGET_FILE:LuminoEngine-Test>)
