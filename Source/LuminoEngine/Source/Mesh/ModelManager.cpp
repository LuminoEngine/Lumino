
/*

	[2014/3/14]
	MMDX から読み取れる更新順序
	・アニメーションをボーンへ適用	MMDModel.BoneUpdate()
	・ボーンのグローバル行列更新	MMDModel.BoneUpdate()
	・<ボーン位置合わせ>			MMDXでは対応していない	http://www10.atwiki.jp/mmdphysics/pages/14.html
	・IK更新						MMDModel.BoneUpdate()
	・物理更新(相互反映)			MMDModel.SkinUpdate()	PhysicsManager.update() -> motionState.Flush()
	・物理更新						stepSimulation()
	・物理更新(※フレーム落ち対策)	PhysicsManager.DropFrame() -> motionState.FlushFD()
	・表情更新						MMDModel.SkinUpdate()	MMDFaceManager.update()
	・表情頂点更新					MMDModel.SkinUpdate()
	・スキン行列更新				MMDModel.SkinUpdate()
	・頂点更新						MMDModel.SkinUpdate()
	・描画

	※フレーム落ち対策…
		現在は常に有効。最後に Flush() された時のボーン行列を覚えておいて、
		ボーンを剛体位置に移動させる処理で使用する。
		Flash() が割と重い処理なので、それをスキップするための処置だと思われる。

	現行LNoteは ボーン→剛体 と 剛体→ボーン を別のタイミングに分けていたけど、その必要は無さそう。
	むしろ　IK更新 → 剛体をボーン位置へ  の方が自然になるはず。

	そして LNote 的にまとめなおすと、
	・アニメーションをボーンへ適用
	・ボーンのグローバル行列更新
	・剛体更新① (ボーンを剛体[物理]と[物理+位置合わせ]へ移動)
	・ボーン位置合わせ											※ここまでで、ボーン行列の更新はIK以外完了しておくこと
	★ユーザー任意のボーン更新
	・IK更新													※ボーン行列に対する最終更新。これ以降はボーン行列は変わらない
	・物理更新② (剛体[ボーン追従]をボーン位置へ移動)			
	・表情スキニング
	・opt頂点スキニング
	◆描画情報更新
	◆描画


	[2014/3/21]
	これまで全てのボーンはひとつの配列で管理していて、参照はインデックスで行っていた。
	(ParentIndex や ChildIndexList 等)

	コピーは高速だが、追加削除(しないと思うけど)等に柔軟に対応できないし、
	階層更新するときは for で回せるからシンプルではあるけど
	配列の先頭から順に、ルートボーンから下っていく並びが必須となる。

	今後はアニメーションXファイルや他のモデルファイルのサポートするだろうし、
	変に高速化とか気を使うよりも汎用的な形にした方が良いと思う。

	というわけで、ポインタ管理に変更。

	[2014/3/21]
	・・・が、やっぱり挿入・削除の対応がつらい。
	例えばIKリンクの真ん中のボーンを消したときは前後はどうするの？とか、
	考えなければならないことがどんどん出てきそう。
	それに、実際ゲーム内でボーン等を挿入・削除することがあるのかって言うと疑問。
	できればライブラリとしてはいい感じだけど、普通使わない。

	インデックス管理ならコピーも高速だし、処理もシンプル。
	ということでこのまま・・・。

	[2014/11/11] MMDX
	MMDMotionState.Flash()
		ボーンの種類に応じて、同期を行っている。ボーン→剛体も、剛体→ボーンも、この関数の中。
		剛体の姿勢は m_graphicsWorldTrans。
		これは virtual メソッド setWorldTransform getWorldTransform により Bullet に渡る。

	MMDCore.update()
		PhysicsThreadManager.update()	メインスレッド。マルチスレッド有効の時は、物理スレッドの更新完了を待ってから Sync を呼んでいる。
										もし遅れていた場合はDropFrameを呼ぶ。
			PhysicsThreadManager.Sync()
				PhysicsThreadManager.Instanse.Synchronize event
					PhysicsManager.update()
						MMDMotionState.Flash()

	物理スレッドは PhysicsThreadManager.threadFunc()。
	無限ループ。
	描画と並列に行われる。
	つまり、
	・アニメ更新			・物理更新
	・剛体とボーン同期
	・スキニング行列生成	・物理更新
	・描画					・物理更新
	なので、物理ボーン(髪とか)の状態が1フレーム遅れる。
	キャラが高速で直線移動すると、なんか服だけちょっとずれで見えたりする。
	正解動作は以下のようになる。
	・アニメ更新
	・ボーン追従の剛体位置をボーンに合わせる
	・物理更新
	・物理ボーンの位置を剛体に合わせる
	・スキニング行列生成
	・描画
	非同期処理が難しい。

	または、物理同期時に剛体追従ボーンと剛体のグローバル空間でのずれ(オフセット)を覚えておいて、
	物理ボーンのスキニング行列を作るときに、このオフセットで補正する。
	これができれば描画と非同期で更新できる。


	MikuMikuFlex は…
		BulletManager.stepSimulation() が呼ばれているのはメインスレッド。
		しかもこれ、BulletManager は PMXPhysicsTransformManager 単位。
		つまり、PMXモデル1つに付き1つの BulletWorld が用意されている。

	ちなみにゲームロジックとしての物理は非同期更新でいいの？
	RayCastができないので論外。
*/
#include "../Internal.h"
#include <Lumino/IO/FileManager.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Mesh/SkinnedMeshModel.h>
#include "PmxSkinnedMesh.h"
#if defined(LN_OS_WIN32)
#include "XFileLoader.h"
#endif
#include "MqoImporter.h"
#include "PmxLoader.h"
//#include "VMDLoader.h"
#include "ModelManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// ModelManager
//==============================================================================

static const unsigned char toon01Data[] =
{
#include "Resource/toon01.png.h"
};
static const size_t toon01DataLen = LN_ARRAY_SIZE_OF(toon01Data);

static const unsigned char toon02Data[] =
{
#include "Resource/toon02.png.h"
};
static const size_t toon02DataLen = LN_ARRAY_SIZE_OF(toon02Data);

static const unsigned char toon03Data[] =
{
#include "Resource/toon03.png.h"
};
static const size_t toon03DataLen = LN_ARRAY_SIZE_OF(toon03Data);

static const unsigned char toon04Data[] =
{
#include "Resource/toon04.png.h"
};
static const size_t toon04DataLen = LN_ARRAY_SIZE_OF(toon04Data);

static const unsigned char toon05Data[] =
{
#include "Resource/toon05.png.h"
};
static const size_t toon05DataLen = LN_ARRAY_SIZE_OF(toon05Data);

static const unsigned char toon06Data[] =
{
#include "Resource/toon06.png.h"
};
static const size_t toon06DataLen = LN_ARRAY_SIZE_OF(toon06Data);

static const unsigned char toon07Data[] =
{
#include "Resource/toon07.png.h"
};
static const size_t toon07DataLen = LN_ARRAY_SIZE_OF(toon07Data);

static const unsigned char toon08Data[] =
{
#include "Resource/toon08.png.h"
};
static const size_t toon08DataLen = LN_ARRAY_SIZE_OF(toon08Data);

static const unsigned char toon09Data[] =
{
#include "Resource/toon09.png.h"
};
static const size_t toon09DataLen = LN_ARRAY_SIZE_OF(toon09Data);

static const unsigned char toon10Data[] =
{
#include "Resource/toon10.png.h"
};
static const size_t toon10DataLen = LN_ARRAY_SIZE_OF(toon10Data);

static ModelManager* g_modelManagerInstance = nullptr;

//------------------------------------------------------------------------------
ModelManager* ModelManager::getInstance(ModelManager* priority)
{
	if (priority != nullptr) return priority;
	return g_modelManagerInstance;
}

//------------------------------------------------------------------------------
ModelManager::ModelManager()
{
}

//------------------------------------------------------------------------------
ModelManager::~ModelManager()
{
}

//------------------------------------------------------------------------------
void ModelManager::initialize(const ConfigData& configData)
{
	m_fileManager = configData.fileManager;
	m_physicsManager = configData.physicsManager;
	m_graphicsManager = configData.graphicsManager;

	MemoryStream data1(toon01Data, toon01DataLen);
	m_mmdDefaultToonTexture[0] = newObject<Texture2D>(&data1, TextureFormat::R8G8B8A8, false);
	MemoryStream data2(toon02Data, toon02DataLen);
	m_mmdDefaultToonTexture[1] = newObject<Texture2D>(&data2, TextureFormat::R8G8B8A8, false);
	MemoryStream data3(toon03Data, toon03DataLen);
	m_mmdDefaultToonTexture[2] = newObject<Texture2D>(&data3, TextureFormat::R8G8B8A8, false);
	MemoryStream data4(toon04Data, toon04DataLen);
	m_mmdDefaultToonTexture[3] = newObject<Texture2D>(&data4, TextureFormat::R8G8B8A8, false);
	MemoryStream data5(toon05Data, toon05DataLen);
	m_mmdDefaultToonTexture[4] = newObject<Texture2D>(&data5, TextureFormat::R8G8B8A8, false);
	MemoryStream data6(toon06Data, toon06DataLen);
	m_mmdDefaultToonTexture[5] = newObject<Texture2D>(&data6, TextureFormat::R8G8B8A8, false);
	MemoryStream data7(toon07Data, toon07DataLen);
	m_mmdDefaultToonTexture[6] = newObject<Texture2D>(&data7, TextureFormat::R8G8B8A8, false);
	MemoryStream data8(toon08Data, toon08DataLen);
	m_mmdDefaultToonTexture[7] = newObject<Texture2D>(&data8, TextureFormat::R8G8B8A8, false);
	MemoryStream data9(toon09Data, toon09DataLen);
	m_mmdDefaultToonTexture[8] = newObject<Texture2D>(&data9, TextureFormat::R8G8B8A8, false);
	MemoryStream data10(toon10Data, toon10DataLen);
	m_mmdDefaultToonTexture[9] = newObject<Texture2D>(&data10, TextureFormat::R8G8B8A8, false);

	m_defaultMaterial = newObject<Material>();

	m_unitBoxMeshResource = Ref<MeshResource>::makeRef();
	m_unitBoxMeshResource->initialize(m_graphicsManager, MeshCreationFlags::None);
	m_unitBoxMeshResource->addBox(Vector3(1, 1, 1));

	m_unitBoxMeshResourceReverseFaces = Ref<MeshResource>::makeRef();
	m_unitBoxMeshResourceReverseFaces->initialize(m_graphicsManager, MeshCreationFlags::None);
	m_unitBoxMeshResourceReverseFaces->addBox(Vector3(1, 1, 1));
	m_unitBoxMeshResourceReverseFaces->reverseFaces();

	m_unitSphereMeshResource = Ref<MeshResource>::makeRef();
	m_unitSphereMeshResource->initialize(m_graphicsManager, MeshCreationFlags::None);
	m_unitSphereMeshResource->addSphere(0.5, 16, 16);

	m_unitSphereMeshResourceReverseFaces = Ref<MeshResource>::makeRef();
	m_unitSphereMeshResourceReverseFaces->initialize(m_graphicsManager, MeshCreationFlags::None);
	m_unitSphereMeshResourceReverseFaces->addSphere(0.5, 16, 16);
	m_unitSphereMeshResourceReverseFaces->reverseFaces();

	m_unitTeapotMeshResource = Ref<MeshResource>::makeRef();
	m_unitTeapotMeshResource->initialize(m_graphicsManager, MeshCreationFlags::None);
	m_unitTeapotMeshResource->addTeapot(1.0f, 8);


	if (g_modelManagerInstance == nullptr)
	{
		g_modelManagerInstance = this;
	}
}

//------------------------------------------------------------------------------
void ModelManager::dispose()
{
	for (auto tex : m_mmdDefaultToonTexture)
	{
		tex.safeRelease();
	}

	if (g_modelManagerInstance == this)
	{
		g_modelManagerInstance = nullptr;
	}
}

//------------------------------------------------------------------------------
Material* ModelManager::getDefaultMaterial() const
{
	return m_defaultMaterial;
}

//------------------------------------------------------------------------------
MeshResource* ModelManager::getUnitBoxMeshResource(bool reverseFaces) const
{
	return (reverseFaces) ? m_unitBoxMeshResourceReverseFaces : m_unitBoxMeshResource;
}

//------------------------------------------------------------------------------
MeshResource* ModelManager::getUnitSphereMeshResource(bool reverseFaces) const
{
	return (reverseFaces) ? m_unitSphereMeshResourceReverseFaces : m_unitSphereMeshResource;
}

//------------------------------------------------------------------------------
MeshResource* ModelManager::getUnitTeapotMeshResource() const
{
	return m_unitTeapotMeshResource;
}

//------------------------------------------------------------------------------
Texture2D* ModelManager::getMMDDefaultToonTexture(int index)
{ 
	return m_mmdDefaultToonTexture[index];
}

//------------------------------------------------------------------------------
Ref<PmxSkinnedMeshResource> ModelManager::createSkinnedMeshResource(const PathName& filePath)
{
	Ref<Stream> stream(m_fileManager->createFileStream(filePath), false);
	PmxLoader loader;
	Ref<PmxSkinnedMeshResource> mesh = loader.load(this, stream, filePath.getParent(), true, ModelCreationFlag::None);
	mesh->refreshInitialValues();
	return mesh;
}

//------------------------------------------------------------------------------
Ref<StaticMeshModel> ModelManager::createStaticMeshModel(const PathName& filePath)
{

#if defined(LN_OS_WIN32)
	Ref<StaticMeshModel> mesh;
	Ref<Stream> stream(m_fileManager->createFileStream(filePath), false);

	PathName parentDir = filePath.getParent();
	{
		MqoImporter importer;
		mesh = importer.import(this, parentDir, stream);
		if (mesh != nullptr) return mesh;
		stream->seek(0, SeekOrigin_Begin);
	}

	//PMXLoader loader;
	//Ref<ModelCore> modelCore(loader.Load(this, stream, filePath.GetParent(), true));

	XFileLoader loader;
	mesh = loader.load(this, stream, parentDir, true, ModelCreationFlag::None);

	//modelCore->refreshInitialValues();
	//modelCore.SafeAddRef();
	return mesh;
#else
	LN_NOTIMPLEMENTED();
	return nullptr;
#endif

	//auto meshResource = CreateModelCore(filePath);
	//auto mesh = Ref<StaticMeshModel>::MakeRef();
	//mesh->initialize(m_graphicsManager, meshResource);
	//return mesh;
}

//------------------------------------------------------------------------------
Ref<SkinnedMeshModel> ModelManager::createSkinnedMeshModel(const PathName& filePath)
{
	auto meshResource = createSkinnedMeshResource(filePath);
	auto mesh = Ref<SkinnedMeshModel>::makeRef();
	mesh->initialize(m_graphicsManager, meshResource);
	return mesh;
}

#if 0
//------------------------------------------------------------------------------
Animation::AnimationClip* ModelManager::CreateMotion(const PathName& filePath)
{
	Ref<Stream> stream(m_fileManager->CreateFileStream(filePath));
	VMDLoader loader;
	if (!loader.Load(stream)) {
		LN_THROW(0, InvalidFormatException);
	}

	// TODO: ボーンと表情のアニメーションは分けた方が良い気がする…
	Ref<Animation::AnimationClip> clip(LN_NEW Animation::AnimationClip());
	LN_FOREACH(VMDLoader::BoneAnimation& anim, loader.GetBoneAnimationList())
	{
		clip->AddAnimationCurve(anim.TargetBoneName, anim.AnimationCurve);
	}
	LN_FOREACH(VMDLoader::FaceAnimation& anim, loader.GetFaceAnimationList())
	{
		clip->AddAnimationCurve(anim.TargetFaceName, anim.AnimationCurve);
	}
	clip.safeAddRef();
	return clip;
}
#endif

//------------------------------------------------------------------------------
Ref<Texture> ModelManager::createTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags)
{
	PathName path(parentDir, filePath.getBegin());	// TODO GetBegin

	// FileNotFound を無視する場合
	if (flags.TestFlag(ModelCreationFlag::IgnoreTextureNotFound))
	{
		if (!m_fileManager->existsFile(path)) {	// TODO: PathName へ変換するときメモリ確保が走る
			return nullptr;
		}
	}

	// TODO: キャッシュが効かない
	auto tex = newObject<Texture2D>(path, TextureFormat::B8G8R8A8, false);	// TODO: mipmap
	return tex;
}

} // namespace detail
LN_NAMESPACE_END
