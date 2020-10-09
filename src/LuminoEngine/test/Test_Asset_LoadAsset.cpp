#include "Common.hpp"
#include "../src/Asset/AssetManager.hpp"
#include "../src/Asset/AssetArchive.hpp"

class Test_Asset_LoadAsset : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Asset_LoadAsset, Basic)
{
	const Path assetFile = LN_TEMPFILE("LoadAssetTest.lca");
	{
		detail::ShaderHelper::buildShader(
			LN_ASSETFILE("Shader/UniformBufferTest-1.fx"),
			LN_TEMPFILE("UniformBufferTest-1.lcfx"),
			String::Empty);

		detail::CryptedAssetArchiveWriter archive;
		archive.open(assetFile, u"pass");
		archive.addFile(LN_ASSETFILE("Graphics/Numbers1.png"), u"x/Numbers1.png");
		archive.addFile(LN_TEMPFILE("UniformBufferTest-1.lcfx"), u"x/Shaders/UniformBufferTest-1.lcfx");
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/BoxTextured.gltf"), u"x/BoxTextured/BoxTextured.gltf");
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/BoxTextured0.bin"), u"x/BoxTextured/BoxTextured0.bin");
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/CesiumLogoFlat.png"), u"x/BoxTextured/CesiumLogoFlat.png");
		archive.close();
	}

	detail::EngineDomain::assetManager()->addAssetArchive(assetFile, u"pass");

	
	// 絶対パスで読み込み
	auto obj1 = Texture2D::load(LN_ASSETFILE("Graphics/Numbers1.png"));
	ASSERT_EQ(true, obj1 != nullptr);
	
	// 相対パスで読み込み
	auto obj2 = Texture2D::load(u"Graphics/Numbers1.png");
	ASSERT_EQ(true, obj2 != nullptr);
	
	// 相対パスで Archive 内のファイルを読み込み
	auto obj3 = Texture2D::load(u"x/Numbers1.png");
	ASSERT_EQ(true, obj3 != nullptr);

	// キャッシュから
	auto obj4 = Texture2D::load(u"Graphics/Numbers1.png");
	ASSERT_EQ(true, obj4 == obj2);

	// キャッシュから
	auto obj5 = Texture2D::load(u"x/Numbers1.png");
	ASSERT_EQ(true, obj5 == obj3);
	ASSERT_EQ(true, obj5 != obj4);


	
	{
		auto obj4 = StaticMeshModel::load(u"x/BoxTextured/BoxTextured.gltf");
		ASSERT_EQ(true, obj4 != nullptr);
	}
}
