
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/Test.PCH.cpp
	UnitTest/Test.PCH.h
	UnitTest/TestConfig.h
	UnitTest/Scene/Test_Scene_Sprite.cpp
	UnitTest/Scene/Test_Scene_TextBlock.cpp
)

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
ln_add_pch(LuminoEngine_Test "UnitTest/Test.PCH.h" "UnitTest/Test.PCH.cpp")

#===============================================================================
set(LN_SANDBOX_SOURCES
	Sandbox/SandboxMain.cpp
	Sandbox/UIControlsGallery.cpp
)

add_executable(LuminoSandbox ${LN_SANDBOX_SOURCES})
target_link_libraries(LuminoSandbox LuminoEngine LuminoLaunch)
add_dependencies(LuminoSandbox LuminoEngine)

