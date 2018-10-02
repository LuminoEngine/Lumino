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

