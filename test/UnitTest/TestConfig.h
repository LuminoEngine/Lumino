#pragma once

#include <gtest/gtest.h>
#include <LuminoCore.h>
#include <Lumino.h>
#include <Lumino/Testing/TestUtils.h>

#include "../../../src/Scene/SceneGraphManager.h"
#include "../../../src/Scene/ViewPane.h"
#include "../../../src/Scene/Sprite.h"
#include "../../../src/Scene/MME/MMEShader.h"
#include "../../../src/Scene/MME/MMEShaderBuilder.h"
#include "../../../src/Scene/MME/MMEShaderErrorInfo.h"



using namespace Lumino;
using namespace Lumino::Graphics;
using namespace Lumino::Imaging;
using namespace Lumino::Scene;

#define LOCALFILE(fileName) TestUtils::GetFilePath(__FILE__, fileName).GetCStr()
#define LOCALFILEA(fileName) TestUtils::GetFilePathA(__FILE__, fileName).GetCStr()
#define LOCALFILEW(fileName) TestUtils::GetFilePathW(__FILE__, fileName).GetCStr()

class TestEnv : public ::testing::Environment
{
public:
	static Platform::PlatformManager*	Application;
	static Physics::PhysicsManager*	PhysicsManager;
	static GraphicsManager*			Manager;
	static Graphics::Renderer*		Renderer;
	static Graphics::SwapChain*		MainSwapChain;
	static SceneGraphManager*		MMDSceneGraph;

public:
	static void SaveScreenShot(const TCHAR* filePath);
	static bool EqualsScreenShot(const TCHAR* filePath);
	static bool EqualsBitmapFile(Imaging::Bitmap* bmp1, const TCHAR* filePath);

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
