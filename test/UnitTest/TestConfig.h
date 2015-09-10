#pragma once

#include <gtest/gtest.h>
#include <LuminoCore.h>
#include <Lumino.h>
#include <Lumino/Testing/TestUtils.h>

#include "../../../src/Scene/SceneGraphManager.h"
#include "../../../src/Scene/ViewPane.h"
#include <Lumino/Scene/Sprite.h>
#include "../../../src/Scene/MME/MMEShader.h"
#include "../../../src/Scene/MME/MMEShaderBuilder.h"
#include "../../../src/Scene/MME/MMEShaderErrorInfo.h"



using namespace Lumino;
using namespace Lumino::Graphics;

#define LOCALFILE(fileName) TestUtils::GetFilePath(__FILE__, fileName).GetCStr()
#define LOCALFILEA(fileName) TestUtils::GetFilePathA(__FILE__, fileName).GetCStr()
#define LOCALFILEW(fileName) TestUtils::GetFilePathW(__FILE__, fileName).GetCStr()

#define SS_SAVE	0
#define SS_LOAD	1

/// スクリーンショットの作成マクロ
//#define SS_SAVE(fileName)	TestEnv::SaveScreenShot(LOCALFILE(TestEnv::MakeScreenShotPath(fileName)))

/// スクリーンショットの比較マクロ
#define SS_CHECK(mode, fileName) if (mode == SS_SAVE) { TestEnv::SaveScreenShot(LOCALFILE(TestEnv::MakeScreenShotPath(fileName))); } else { ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE(TestEnv::MakeScreenShotPath(fileName)))); }

class TestEnv : public ::testing::Environment
{
public:
	static RefPtr<Lumino::Application>	Application;
	static Platform::PlatformManager*	Platform;
	static Physics::PhysicsManager*	PhysicsManager;
	static GraphicsManagerPtr			Manager;
	static Graphics::Renderer*		Renderer;
	static Graphics::SwapChain*		MainSwapChain;
	static SceneGraphManager*		MMDSceneGraph;

public:
	static Graphics::Renderer* BeginRendering();
	static void EndRendering();
	static PathName MakeScreenShotPath(const char* fileName);
	static void SaveScreenShot(const TCHAR* filePath);
	static bool EqualsScreenShot(const TCHAR* filePath);
	static bool EqualsBitmapFile(Bitmap* bmp1, const TCHAR* filePath);
	static RefPtr<Shader> CreateShader(const TCHAR* filePath);

protected:
	virtual void SetUp();
	virtual void TearDown();
};

/// 頂点データ (位置+頂点カラー)
class PosColorVertex
{
public:
	Vector3		Position;
	ColorF		Color;

	static VertexElement* GetLayout()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
		};
		return elements;
	}
	static const int LayoutCount = 2;
};

/// 頂点データ (位置+テクスチャUV)
class PosUVVertex
{
public:
	Vector3		Position;
	Vector2		TexUV;

	static VertexElement* GetLayout()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		return elements;
	}
};
