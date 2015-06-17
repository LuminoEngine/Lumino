
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/Test.cpp
	UnitTest/Imaging/Test_Imaging_Bitmap.cpp
	UnitTest/Graphics/Test_Graphics_GeometryRenderer.cpp
	UnitTest/Graphics/Test_Graphics_IndexBuffer.cpp
	UnitTest/Graphics/Test_Graphics_Shader.cpp
	UnitTest/Graphics/Test_Graphics_SpriteRenderer.cpp
	UnitTest/Graphics/Test_Graphics_Texture.cpp
	UnitTest/Graphics/Test_Graphics_VertexBuffer.cpp
	UnitTest/Scene/Test_Scene_Sprite.cpp)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${PROJECT_SOURCE_DIR}/test/UnitTest"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include")
	
link_libraries(Lumino_Static)

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
add_executable(Lumino_UnitTest ${LN_TEST_SOURCES})
