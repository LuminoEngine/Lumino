#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"

namespace ln {
class MeshResource;
namespace detail {

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class PrimitiveRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
    PrimitiveRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("PrimitiveRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

// 単純なメッシュ形状を描画する。
// MeshRenderFeature が Mesh(VertexBuffer, IndexBuffer) を受け取って描画するのに対し、
// こちらは形状の情報（球なら中心位置と半径）を受け取って描画する。そのためデータサイズを非常に小さく抑えることができる。
// また、スプライトのようなバッファリングによるドローコール削減も狙う。
class PrimitiveRenderFeature
	: public RenderFeature
{
public:
    PrimitiveRenderFeature();
	~PrimitiveRenderFeature();
	void initialize(RenderingManager* manager);




    void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);



	virtual void flush(GraphicsContext* context) override;

private:
	RenderingManager* m_manager;
};

} // namespace detail
} // namespace ln

