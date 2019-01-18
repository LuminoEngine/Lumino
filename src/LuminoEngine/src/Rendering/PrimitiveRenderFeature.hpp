#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Mesh/MeshGenerater.hpp"
#include "RenderingManager.hpp"

namespace ln {
class MeshResource;
namespace detail {

class InternalPrimitiveRenderer
    : public RefObject
{
public:
    InternalPrimitiveRenderer();
    virtual ~InternalPrimitiveRenderer();
    void initialize(RenderingManager* manager);

    void drawMeshGenerater(const MeshGenerater* generator);

    void flush(IGraphicsDeviceContext* context);

private:
    void prepareBuffers(IGraphicsDeviceContext* context, int vertexCount, int indexCount);

    RenderingManager* m_manager;
    Ref<LinearAllocator> m_linearAllocator;
    List<MeshGenerater*> m_generators;
    PrimitiveType m_primitiveType;
    Ref<IVertexDeclaration> m_vertexDeclaration;
    Ref<IVertexBuffer> m_vertexBuffer;
    Ref<IIndexBuffer> m_indexBuffer;

    /*
     0.4.0 までは CacheBuffer という、内部実装は vector なバッファを使っていたが、
     これだと常に最大使用量のメモリが確保されたままになるので、メモリ効率が良くない。
     LinearAllocator を使うことで平均化できるが、一方こちらはバッファがひとつながりではないので、
     CacheBuffer の時のように頂点バッファデータをキャッシュすることはできなくなる。
     そのため Sprite と同じように、まずは描画情報 (MeshGenerater) をリストで持って、
     flush 時に頂点データ化するという流れで描画を行う。
    */
};

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



    //void drawMeshGenerater(const MeshGenerater* generator);
    template<class TFactory>
    void drawMeshGenerater(const TFactory& generator)
    {
        if (m_lastPrimitiveType.hasValue() && m_lastPrimitiveType != generator.primitiveType()) {
            flush(m_manager->graphicsManager()->graphicsContext());
        }
        m_lastPrimitiveType = generator.primitiveType();

        GraphicsManager* manager = m_manager->graphicsManager();
        LN_ENQUEUE_RENDER_COMMAND_2(
            PrimitiveRenderFeature_drawMeshGenerater, manager,
            InternalPrimitiveRenderer*, m_internal,
            TFactory, generator,
            {
                m_internal->drawMeshGenerater(&generator);
            });
    }


    //void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);



	virtual void flush(GraphicsContext* context) override;
    virtual bool drawElementTransformNegate() const override { return true; }

private:
    Optional<PrimitiveType> m_lastPrimitiveType;
	RenderingManager* m_manager;
    Ref<InternalPrimitiveRenderer> m_internal;
};

} // namespace detail
} // namespace ln

