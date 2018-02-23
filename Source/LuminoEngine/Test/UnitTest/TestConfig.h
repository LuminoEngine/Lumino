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
	static void saveScreenShot(const Char* filePath);
	static bool EqualsScreenShot(const Char* filePath, int passRate = 90);
	static bool EqualsBitmapFile(RawBitmap* bmp1, const Char* filePath, int passRate);
	static bool CheckScreenShot(const Char* filePath, int passRate = 99, bool save = false);	// 基本的に 99% 一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある (Radeon HD8490)
	static void WaitRendering();

	// Engine::getWorld... を使った追加ユーティリティ。この辺のインターフェイスの変化が激しいので用意した。
	template<class T>
	static T addWorld2D(T obj)
	{
		Engine::defaultWorld2D()->add(obj);
		return obj;
	}
	template<class T>
	static T addWorld3D(T obj)
	{
		Engine::defaultWorld3D()->add(obj);
		return obj;
	}

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

#define LN_TEST_CLEAN_SCENE \
	Engine::defaultWorld2D()->removeAllObjects(); \
	Engine::defaultWorld3D()->removeAllObjects();


inline PathName Test_GetTempFilePath(const Char* fileName)
{
	PathName base(__FILE__);
	PathName tempDir(base.getParent(), _LT("../../"));
	tempDir.append(_LT("tmp"));
	PathName path(tempDir, fileName);
	FileSystem::createDirectory(path.getParent());
	return PathName(path.c_str());
}

#define LN_TEMPFILE(fileName)	Test_GetTempFilePath(_LT(fileName)).c_str()


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
		camera = Engine::defaultWorld3D()->getMainCamera();
		oldPos = camera->getPosition();
		camera->setPosition(x, y, z);
	}

	ScopedCameraPosition(const Vector3& pos, const Vector3& lookAt)
	{
		camera = Engine::defaultWorld3D()->getMainCamera();
		oldPos = camera->getPosition();
		camera->setPosition(pos);
		camera->getCameraComponent()->setLookAt(lookAt);
	}

	~ScopedCameraPosition()
	{
		camera->setPosition(oldPos);
		camera->getCameraComponent()->setLookAt(Vector3::Zero);
	}

	Camera*	camera;
	Vector3 oldPos;
};

// テストフィクスチャ終了時のクリーンアップ
class LuminoGraphicsTest : public ::testing::Test
{
	virtual void TearDown() override
	{
		LN_TEST_CLEAN_SCENE;
	}
};

