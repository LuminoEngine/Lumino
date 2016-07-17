
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class StaticMeshModel;

namespace detail
{

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

private:
	void FlushStateImpl(const Matrix& world, const Matrix& viewProj);
	void DrawMeshImpl(StaticMeshModel* mesh);

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
