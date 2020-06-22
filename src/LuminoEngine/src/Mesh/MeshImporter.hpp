
#pragma once
#include <LuminoEngine/Mesh/Common.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>

namespace ln {
class Stream;
class DiagnosticsManager;
class AnimationClip;
namespace detail {

class MeshImporter
{
public:
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
