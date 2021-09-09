#include <future>
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
		archive.open(assetFile, _TT("pass"));
		archive.addFile(LN_ASSETFILE("Graphics/Numbers1.png"), _TT("x/Numbers1.png"));
		archive.addFile(LN_TEMPFILE("UniformBufferTest-1.lcfx"), _TT("x/Shaders/UniformBufferTest-1.lcfx"));
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/BoxTextured.gltf"), _TT("x/BoxTextured/BoxTextured.gltf"));
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/BoxTextured0.bin"), _TT("x/BoxTextured/BoxTextured0.bin"));
		archive.addFile(LN_ASSETFILE("Mesh/BoxTextured/CesiumLogoFlat.png"), _TT("x/BoxTextured/CesiumLogoFlat.png"));
		archive.addFile(LN_ASSETFILE("Mesh/SkinnedMesh2.glb"), _TT("x/SkinnedMesh2.glb"));
		archive.close();
	}

	detail::EngineDomain::assetManager()->addAssetArchive(assetFile, _TT("pass"));

	
	// 絶対パスで読み込み
	auto obj1 = Texture2D::load(LN_ASSETFILE("Graphics/Numbers1.png"));
	ASSERT_EQ(true, obj1 != nullptr);
	
	// 相対パスで読み込み
	auto obj2 = Texture2D::load(_TT("Graphics/Numbers1.png"));
	ASSERT_EQ(true, obj2 != nullptr);
	
	// 相対パスで Archive 内のファイルを読み込み
	auto obj3 = Texture2D::load(_TT("x/Numbers1.png"));
	ASSERT_EQ(true, obj3 != nullptr);

	// キャッシュから
	auto obj4 = Texture2D::load(_TT("Graphics/Numbers1.png"));
	ASSERT_EQ(true, obj4 == obj2);

	// キャッシュから
	auto obj5 = Texture2D::load(_TT("x/Numbers1.png"));
	ASSERT_EQ(true, obj5 == obj3);
	ASSERT_EQ(true, obj5 != obj4);

	{
		// モデルファイルから相対パス指定されたテクスチャの読み込み
		auto mesh1 = MeshModel::load(_TT("x/BoxTextured/BoxTextured.gltf"));
		ASSERT_EQ(true, mesh1 != nullptr);
		ASSERT_EQ(true, mesh1->materials()[0]->mainTexture() != nullptr);

		auto texture1 = Texture2D::load(_TT("x/BoxTextured/CesiumLogoFlat.png"));
		ASSERT_EQ(true, mesh1->materials()[0]->mainTexture() == texture1);
	}

	// Shader
	{
		auto shader1 = Shader::load(_TT("x/Shaders/UniformBufferTest-1.lcfx"));
		ASSERT_EQ(true, shader1 != nullptr);

		auto shader2 = Shader::load(_TT("x/Shaders/UniformBufferTest-1.lcfx"));
		ASSERT_EQ(true, shader1 == shader2);
	}

	// SkinnedMeshModel
	{
		auto skinnedmesh1 = SkinnedMeshModel::load(_TT("x/SkinnedMesh2.glb"));
		ASSERT_EQ(true, skinnedmesh1 != nullptr);

		// TODO: キャッシュ未対応。HC4 ではひとつしか使わないので、#156 対応後に合わせこむ
		//auto skinnedmesh2 = SkinnedMeshModel::load(_TT("x/SkinnedMesh2.glb");
		//ASSERT_EQ(true, skinnedmesh1 == skinnedmesh2);
	}
}

//------------------------------------------------------------------------------
// 音声ファイルは基本的に別スレッドで読み取られるので、
// メインスレッドでの読み取りと競合すると音声が遅延してしまうことがある。
// そのため Archive ファイルを分けることで同時読み取りを避けたい。
TEST_F(Test_Asset_LoadAsset, MultiArchive)
{
	const Path assetFile1 = LN_TEMPFILE("LoadAssetTest1.lca");
	uint32_t hash1 = 0;
	{
		detail::CryptedAssetArchiveWriter archive;
		archive.open(assetFile1, _TT("pass1"));
		archive.addFile(LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_2ch.wav"), _TT("Audio/sin_440_3s_S16L_96000_2ch.wav"));
		archive.close();

		auto data = FileSystem::readAllBytes(LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_2ch.wav"));
		hash1 = CRCHash::compute((const char*)data.data(), data.size());
	}

	const Path assetFile2 = LN_TEMPFILE("LoadAssetTest2.lca");
	uint32_t hash2 = 0;
	{
		detail::CryptedAssetArchiveWriter archive;
		archive.open(assetFile2, _TT("pass2"));
		archive.addFile(LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_2ch.wav"), _TT("Audio/sin_440_3s_S16L_88200_2ch.wav"));
		archive.close();

		auto data = FileSystem::readAllBytes(LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_2ch.wav"));
		hash2 = CRCHash::compute((const char*)data.data(), data.size());
	}

	detail::EngineDomain::assetManager()->addAssetArchive(assetFile1, _TT("pass1"));
	detail::EngineDomain::assetManager()->addAssetArchive(assetFile2, _TT("pass2"));

	auto stream1 = Assets::openFileStream(_TT("Audio/sin_440_3s_S16L_96000_2ch.wav"));
	auto stream2 = Assets::openFileStream(_TT("Audio/sin_440_3s_S16L_88200_2ch.wav"));
	ASSERT_EQ(true, stream1 != nullptr);
	ASSERT_EQ(true, stream2 != nullptr);

	// 複数スレッドから同時にファイル内容を読み取って、ハッシュを計算
	auto result1 = std::async(std::launch::async, [&] { 
		auto data1 = stream1->readToEnd();
		return CRCHash::compute((const char*)data1.data(), data1.size());
	});
	auto result2 = std::async(std::launch::async, [&] {
		auto data2 = stream2->readToEnd();
		return CRCHash::compute((const char*)data2.data(), data2.size());
	});

	// ハッシュの一致で、ファイル破損をチェック
	uint32_t r1 = result1.get();
	uint32_t r2 = result2.get();
	ASSERT_EQ(hash1, r1);
	ASSERT_EQ(hash2, r2);
}
