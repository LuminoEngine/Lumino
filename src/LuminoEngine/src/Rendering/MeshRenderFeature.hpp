#pragma once
//#include <LuminoCore/Base/EnumFlags.hpp>
//#include <LuminoEngine/Graphics/GeometryStructs.hpp>
//#include <LuminoEngine/Graphics/ColorStructs.hpp>
//#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
//#include <LuminoEngine/Rendering/RenderFeature.hpp>
//#include <LuminoEngine/Rendering/Vertex.hpp>
//#include "../Graphics/GraphicsDeviceContext.hpp"
#include "RenderStage.hpp"

namespace ln {
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

//class MeshRenderFeature
//	: public RefObject
//	, public detail::IRenderFeature
//{
//public:
//	MeshRenderFeature();
//	~MeshRenderFeature();
//	void initialize(GraphicsManager* manager);
//
//	void drawMesh(MeshResource* mesh, int startIndex, int primitiveCount, PrimitiveType primitiveType);
//
//protected:
//	virtual bool isStandaloneShader() const { return false; }
//	virtual void flush() override {}
//	virtual void onActivated() override {}
//	virtual void onDeactivated() override {}
//
//private:
//	struct DrawMeshCommandData
//	{
//		Ref<Driver::IVertexDeclaration>	vertexDeclaration;
//		Ref<Driver::IVertexBuffer>		vertexBuffers[Driver::MaxVertexStreams];
//		int									vertexBuffersCount;
//		Ref<Driver::IIndexBuffer>		indexBuffer;
//		int									startIndex;
//		int									primitiveCount;
//		PrimitiveType						primitiveType;
//	};
//
//	void drawMeshImpl(const DrawMeshCommandData& data);	// Threading
//
//	GraphicsManager*		m_manager;
//	Driver::IRenderer*		m_renderer;
//};
//

} // namespace detail
} // namespace ln

