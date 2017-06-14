#pragma once



//#include "../../../src/Scene/SceneGraphManager.h"
//#include "../../../src/Scene/ViewPane.h"
//#include <Lumino/Scene/Sprite.h>
//#include "../../../src/Scene/MME/MMEShader.h"
//#include "../../../src/Scene/MME/MMEShaderBuilder.h"
//#include "../../../src/Scene/MME/MMEShaderErrorInfo.h"
//


using namespace ln;

#define SS_SAVE	0
#define SS_LOAD	1

/// スクリーンショットの作成マクロ
//#define SS_SAVE(fileName)	TestEnv::saveScreenShot(LN_LOCALFILE(TestEnv::MakeScreenShotPath(fileName)))

/// スクリーンショットの比較マクロ
#define SS_CHECK(mode, fileName) if (mode == SS_SAVE) { TestEnv::saveScreenShot(LN_LOCALFILE(TestEnv::MakeScreenShotPath(fileName))); } else { ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE(TestEnv::MakeScreenShotPath(fileName)))); }

class TestEnv : public ::testing::Environment
{
public:
	static void saveScreenShot(const TCHAR* filePath);
	static bool EqualsScreenShot(const TCHAR* filePath, int passRate = 90);
	//static bool equalsTexture(Texture* texture, const TCHAR* filePath);
	static bool EqualsBitmapFile(Bitmap* bmp1, const TCHAR* filePath, int passRate);
	static bool CheckScreenShot(const TCHAR* filePath, int passRate = 99, bool save = false);	// 基本的に 99% 一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある (Radeon HD8490)
	static void WaitRendering();


	static void BeginFrame()
	{
		Engine::updateFrame();
	}

	static void EndFrame()
	{
		Engine::presentFrame();
	}

protected:
	virtual void SetUp();
	virtual void TearDown();
};

// テスト。あえてマクロにしたほうが処理の境界がわかりやすい？
#define LN_TEST_BEGIN_FRAME		TestEnv::BeginFrame()
#define LN_TEST_END_FRAME		TestEnv::EndFrame()


inline PathName Test_GetTempFilePath(const TCHAR* fileName)
{
	PathName base(__FILE__);
	PathName tempDir(base.getParent(), _T("../../"));
	tempDir.append(_T("tmp"));
	PathName path(tempDir, fileName);
	FileSystem::createDirectory(path.getParent());
	return PathName(path.c_str());
}

#define LN_TEMPFILE(fileName)	Test_GetTempFilePath(_T(fileName)).c_str()


/// 頂点データ (位置+頂点カラー)
class PosColorVertex
{
public:
	Vector3		Position;
	Color		Color;

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

class ScopedCameraPosition
{
public:
	ScopedCameraPosition(float x, float y, float z)
	{
		camera = CameraComponent::getMain3DCamera();
		oldPos = camera->getPosition();
		camera->setPosition(x, y, z);
	}

	~ScopedCameraPosition()
	{
		camera->setPosition(oldPos);
	}

	CameraComponent*	camera;
	Vector3 oldPos;
};

