
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class StaticMeshModel;
class Material3;

namespace detail
{

// いまのところ、streamIndex の仕組みをユーザーに公開しないために用意している。
// メッシュが使う streamIndex 関係の処理は全部この中。
class MeshRendererProxy
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	MeshRendererProxy();
	~MeshRendererProxy();
	void Initialize(GraphicsManager* manager);

	void SetTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);

	void DrawMesh(StaticMeshModel* mesh);

protected:
	virtual void Flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override {}

private:
	struct DrawMeshCommandData
	{
		RefPtr<Driver::IVertexDeclaration>	vertexDeclaration;
		RefPtr<Driver::IVertexBuffer>		vertexBuffers[Driver::MaxVertexStreams];
		RefPtr<Driver::IIndexBuffer>		indexBuffer;
		int									triangleCount;
	};

	void FlushStateImpl(const Matrix& world, const Matrix& viewProj);	// Threading
	void DrawMeshImpl(const DrawMeshCommandData& data);	// Threading

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;

	Matrix					m_transform;
	Matrix					m_viewProj;
	bool					m_stateModified;

	struct
	{
		RefPtr<Driver::IShader>		shader;
		Driver::IShaderTechnique*	technique = nullptr;
		Driver::IShaderPass*		pass = nullptr;
		Driver::IShaderVariable*	varWorldMatrix = nullptr;
		Driver::IShaderVariable*	varViewProjMatrix = nullptr;
		Driver::IShaderVariable*	varTexture = nullptr;

	} m_shader;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
