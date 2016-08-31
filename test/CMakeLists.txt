
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/TestConfig.h
	UnitTest/Base/Test_Base_Point.cpp
	UnitTest/Base/Test_Base_Rect.cpp
	UnitTest/Base/Test_Base_Size.cpp
	UnitTest/Base/Test_Base_Version.cpp
	UnitTest/IO/Test_IO_ArchiveMaker.cpp
	UnitTest/IO/Test_IO_ASync.cpp
	UnitTest/Platform/Test_Platform_Clipboard.cpp
	UnitTest/Imaging/Test_Imaging_Bitmap.cpp
	UnitTest/Graphics/Test_Graphics_Color.cpp
	UnitTest/Graphics/Test_Graphics_DrawingContext.cpp
	UnitTest/Graphics/Test_Graphics_RenderingContext.cpp
	#UnitTest/Graphics/Test_Graphics_IndexBuffer.cpp
	UnitTest/Graphics/Test_Graphics_Pmx.cpp
	#UnitTest/Graphics/Test_Graphics_Renderer.cpp
	#UnitTest/Graphics/Test_Graphics_Shader.cpp
	#UnitTest/Graphics/Test_Graphics_SpriteRenderer.cpp
	UnitTest/Graphics/Test_Graphics_StaticMesh.cpp
	UnitTest/Graphics/Test_Graphics_Texture.cpp
	#UnitTest/Graphics/Test_Graphics_VertexBuffer.cpp
	UnitTest/Graphics/Test_Graphics_XFile.cpp
	UnitTest/Scene/Test_Scene_Sprite.cpp
	UnitTest/Scene/Test_Scene_SpriteParticle.cpp
	UnitTest/Scene/Test_Scene_StaticMesh.cpp
	UnitTest/Scene/Test_Scene_SceneNode.cpp
	UnitTest/UI/Test_UI_Button.cpp
	UnitTest/UI/Test_UI_TextBlock.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${PROJECT_SOURCE_DIR}/test/UnitTest"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include"
)
	
link_libraries(Lumino_Static)

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
add_executable(Lumino_UnitTest ${LN_TEST_SOURCES})
