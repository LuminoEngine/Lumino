
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/Test.PCH.cpp
	UnitTest/Test.PCH.h
	UnitTest/TestConfig.h
	UnitTest/Animation/Test_Animation.cpp
	UnitTest/Base/Test_Base_GeometryStructs.cpp
	UnitTest/Base/Test_Base_Uuid.cpp
	UnitTest/IO/Test_IO_ArchiveMaker.cpp
	UnitTest/IO/Test_IO_ASync.cpp
	UnitTest/Physics/Test_Physics_3D.cpp
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
	UnitTest/UI/Test_UI_Layout.cpp
	UnitTest/UI/Test_UI_Primitive.cpp
	UnitTest/UI/Test_UI_System.cpp
)

if (MSVC_VERSION GREATER 1800)		# (MSVC > vs2013)
	
	set(LN_TEST_SOURCES
		${LN_TEST_SOURCES}
		UnitTest/Scripting/Test_Scripting.cpp
	)
	
endif()

include_directories(
	"${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include"
	"${PROJECT_SOURCE_DIR}/Include"
	"${PROJECT_SOURCE_DIR}/Source/LuminoExperimental/Include"
	"${PROJECT_SOURCE_DIR}/Test/gtest/include"
	"${PROJECT_SOURCE_DIR}/Test/gtest"
	"${PROJECT_SOURCE_DIR}/Test/gtest/src"
	"${PROJECT_SOURCE_DIR}/Test/UnitTest"
)


add_executable(LuminoEngine_Test ${LN_TEST_SOURCES})
target_link_libraries(LuminoEngine_Test LuminoEngine)
add_dependencies(LuminoEngine_Test LuminoEngine)
ln_add_pch(LuminoEngine_Test "Test.PCH.h" "UnitTest/Test.PCH.cpp")

#===============================================================================
set(LN_SANDBOX_SOURCES
	Sandbox/SandboxMain.cpp
	Sandbox/UIControlsGallery.cpp
)

add_executable(LuminoSandbox ${LN_SANDBOX_SOURCES})
target_link_libraries(LuminoSandbox LuminoEngine LuminoLaunch)
add_dependencies(LuminoSandbox LuminoEngine)

