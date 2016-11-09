#pragma once

#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public

#include <gtest/gtest.h>
#include <LuminoCore.h>
#include <LuminoEngine.h>
#include <Lumino/Testing/TestUtils.h>

//#include "../../../src/Scene/SceneGraphManager.h"
//#include "../../../src/Scene/ViewPane.h"
//#include <Lumino/Scene/Sprite.h>
//#include "../../../src/Scene/MME/MMEShader.h"
//#include "../../../src/Scene/MME/MMEShaderBuilder.h"
//#include "../../../src/Scene/MME/MMEShaderErrorInfo.h"
//


using namespace ln;

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
	static void SaveScreenShot(const TCHAR* filePath);
	static bool EqualsScreenShot(const TCHAR* filePath, int passRate = 90);
	static bool EqualsTexture(Texture* texture, const TCHAR* filePath);
	static bool EqualsBitmapFile(Bitmap* bmp1, const TCHAR* filePath, int passRate);
	static bool CheckScreenShot(const TCHAR* filePath, int passRate = 99, bool save = false);	// 基本的に 99% 一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある (Radeon HD8490)
	static void WaitRendering();

protected:
	virtual void SetUp();
	virtual void TearDown();
};


inline PathName Test_GetTempFilePath(const TCHAR* fileName)
{
	PathName base(__FILE__);
	PathName tempDir(base.GetParent(), _T("../../"));
	tempDir.Append(_T("tmp"));
	PathName path(tempDir, fileName);
	FileSystem::CreateDirectory(path.GetParent());
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
