
#pragma once
#include <LuminoGraphics/Mesh/Common.hpp>
#include <LuminoGraphics/Mesh/MeshPrimitive.hpp>

namespace ln {
class Stream;
class DiagnosticsManager;
class AnimationClip;
class MeshImportSettings;
namespace detail {

/*
	[2020/10/10] 座標系変換の方針
	----------
	大方針
	- SkinnedMesh は人型であると想定できるファイルは、Z+ を向くようにインポートする。
	- StaticMesh は基本的に外部エディタで「見たまま」インポートする。

	SkinnedMesh はキャラクターなど動的なオブジェクトとして使用することがほとんどで、
	Lumino の多くの機能は Z+ を正面＝進行方向と考えている。
	それに合わせておくことで、いちいち 90度回転するような微調整が不要となるし、
	インポートした後すぐ想定通りの動きができるようにしておきたい。

	一方 StaticMesh は地形や背景オブジェクトなど静的なオブジェクトとして使用する。
	こちらは Lumino が気を利かせるよりは Blender など外部エディタで編集した見た目のまま表示できた方がよい。

	### .gltf

	モデル自体は [Y-Up,R-Hand] である。


	### .vrm

	.gltf の拡張であるため [Y-Up,R-Hand] であるが、キャラクターとしての正面は Z- を向いている。
*/
class MeshImporter
{
public:
	MeshImporter();
	void prepare(MeshManager* meshManager, DiagnosticsManager* diag, const MeshImportSettings* settings);

	virtual bool onImportAsStaticMesh(MeshModel* model, const AssetPath& assetPath) = 0;
	virtual bool onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath) = 0;

	MeshManager* m_meshManager;
	AssetManager* m_assetManager;
	DiagnosticsManager* m_diag;

	bool m_applyBoneTransformationsEnabled;
	bool m_flipZCoordinate;

	bool m_isCharacterModelFormat = false;

protected:
	// モデルファイルの座標系が既知の場合、正面が Lumino の座標系で Z+ を向くように処理する
	bool isCharacterModelFormat() const { return m_isCharacterModelFormat; }

	bool isSkeletonImport() const;

private:
	const MeshImportSettings* m_settings;
};

} // namespace detail


// glTF などメッシュデータ以外にも Animation や Metadata, Camera, Light などを持っているデータファイルに対して、
// それらをユーザープログラムへ公開するインターフェイスとなるクラス。
// 主に Animation を取り出すために使用する。
class MeshSceneModel final
	//: public Object
{
public:
	// TODO: ↓まとめちゃっていいかもしれない。
	Ref<MeshModel> staticMesh;
	Ref<SkinnedMeshModel> skinnedMesh;

	List<Ref<AnimationClip>> animationClips;

//LN_CONSTRUCT_ACCESS:
//	MeshSceneModel();
//	void init();

private:
};

} // namespace ln
