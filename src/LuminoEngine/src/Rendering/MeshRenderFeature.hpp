#pragma once
//#include <LuminoCore/Base/EnumFlags.hpp>
//#include <LuminoEngine/Graphics/GeometryStructs.hpp>
//#include <LuminoEngine/Graphics/ColorStructs.hpp>
//#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
//#include <LuminoEngine/Rendering/Vertex.hpp>
//#include "../Graphics/GraphicsDeviceContext.hpp"
#include "RenderStage.hpp"

namespace ln {
class MeshResource;
namespace detail {

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class MeshRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	MeshRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("MeshRenderFeatureStageParameters"))
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

// MeshSection 単位で描画する
class MeshRenderFeature
	: public RenderFeature
{
public:
	MeshRenderFeature();
	~MeshRenderFeature();
	void initialize(RenderingManager* manager);

	void drawMesh(GraphicsContext* context, MeshResource* mesh, int sectionIndex);
	//void drawMesh(MeshResource* mesh, int startIndex, int primitiveCount, PrimitiveType primitiveType);

	virtual void flush(GraphicsContext* context) override;

private:
	struct DrawMeshCommandData
	{
		Ref<IVertexDeclaration>	vertexDeclaration;
		Ref<IVertexBuffer>		vertexBuffers[MaxVertexStreams];
		int									vertexBuffersCount;
		Ref<IIndexBuffer>		indexBuffer;
		int									startIndex;
		int									primitiveCount;
		PrimitiveType						primitiveType;
	};

	void drawMeshImplOnRenderThread(IGraphicsDeviceContext* context, const DrawMeshCommandData& data);

	RenderingManager* m_manager;

	//GraphicsManager*		m_manager;
	//Driver::IRenderer*		m_renderer;
};


} // namespace detail
} // namespace ln

