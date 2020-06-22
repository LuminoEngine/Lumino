#pragma once

namespace ln {
class SkinnedMeshModel;

//enum class MeshCreationFlags
//{
//    None = 0x0000,
//    //StrictResourceLoading = 0x04,		// マテリアルに含まれるテクスチャファイルが見つからなくても無視する
//    //ModelCreateFlag_CopyMaterials = 0x08,		// マテリアルを共有せずにコピーする
//};
//LN_FLAGS_OPERATORS(BoneType);

class RenderFeature
	: public Object
{
public:
	virtual void flush() = 0;

private:
};

} // namespace ln

