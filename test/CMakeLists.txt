
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/TestConfig.h
	UnitTest/Base/Test_Base_GeometryStructs.cpp
	UnitTest/Base/Test_Base_Uuid.cpp
	UnitTest/Base/Test_Base_Version.cpp
	UnitTest/IO/Test_IO_ArchiveMaker.cpp
	UnitTest/IO/Test_IO_ASync.cpp
	UnitTest/Platform/Test_Platform_Clipboard.cpp
	UnitTest/Imaging/Test_Imaging_Bitmap.cpp
	UnitTest/Graphics/Test_Graphics.cpp
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

if (MSVC_VERSION GREATER 1800)		# (MSVC > vs2013)
	
	set(LN_TEST_SOURCES
		${LN_TEST_SOURCES}
		UnitTest/Scripting/Test_Scripting.cpp
	)
	
endif()

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${PROJECT_SOURCE_DIR}/test/UnitTest"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include"
)


#set(LN_TEST_LIBS Lumino_static)
#link_libraries(${LN_TEST_LIBS})


add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
add_executable(Lumino_UnitTest ${LN_TEST_SOURCES})
target_link_libraries(Lumino_UnitTest Lumino_Static)
add_dependencies(Lumino_UnitTest Lumino_Static)

#===============================================================================
set(LN_SANDBOX_SOURCES
	Sandbox/SandboxMain.cpp
)

add_executable(LuminoSandbox ${LN_SANDBOX_SOURCES})
target_link_libraries(LuminoSandbox LuminoFoundation Lumino_Static)
add_dependencies(LuminoSandbox Lumino_Static)

