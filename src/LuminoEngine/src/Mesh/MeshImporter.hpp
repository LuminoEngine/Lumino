
#pragma once
#include <LuminoEngine/Mesh/Common.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>

namespace ln {
class Stream;
class DiagnosticsManager;
class AnimationClip;
class MeshImportSettings;
namespace detail {

class MeshImporter
{
public:
	MeshImporter();
	void prepare(MeshManager* meshManager, DiagnosticsManager* diag);
	void applySettings(const MeshImportSettings* settings);

	virtual bool onImportAsStaticMesh(StaticMeshModel* model, const AssetPath& assetPath) = 0;
	virtual bool onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath) = 0;

	MeshManager* m_meshManager;
	AssetManager* m_assetManager;
	DiagnosticsManager* m_diag;

	bool m_applyBoneTransformationsEnabled;
	bool m_flipZCoordinate;
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
	Ref<StaticMeshModel> staticMesh;
	Ref<SkinnedMeshModel> skinnedMesh;

	List<Ref<AnimationClip>> animationClips;

//LN_CONSTRUCT_ACCESS:
//	MeshSceneModel();
//	void init();

private:
};

} // namespace ln
