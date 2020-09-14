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
		archive.close();
	}

	detail::EngineDomain::assetManager()->addAssetArchive(assetFile, u"pass");


	{
		auto obj = Texture2D::load(u"x/Numbers1.png");
		ASSERT_EQ(true, obj != nullptr);
	}
}
