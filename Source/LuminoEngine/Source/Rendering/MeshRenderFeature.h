
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
class MeshResource;

namespace detail {

// いまのところ、streamIndex の仕組みをユーザーに公開しないために用意している。
// メッシュが使う streamIndex 関係の処理は全部この中。
class MeshRenderFeature
	: public RefObject
	, public detail::IRenderFeature
{
public:
	MeshRenderFeature();
	~MeshRenderFeature();
	void initialize(GraphicsManager* manager);

	void DrawMesh(MeshResource* mesh, int startIndex, int primitiveCount, PrimitiveType primitiveType);

protected:
	virtual bool IsStandaloneShader() const { return false; }
	virtual void flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override {}

private:
	struct DrawMeshCommandData
	{
		RefPtr<Driver::IVertexDeclaration>	vertexDeclaration;
		RefPtr<Driver::IVertexBuffer>		vertexBuffers[Driver::MaxVertexStreams];
		int									vertexBuffersCount;
		RefPtr<Driver::IIndexBuffer>		indexBuffer;
		int									startIndex;
		int									primitiveCount;
		PrimitiveType						primitiveType;
	};

	void DrawMeshImpl(const DrawMeshCommandData& data);	// Threading

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
};

} // namespace detail
LN_NAMESPACE_END
