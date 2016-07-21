
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Mesh.h>
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"
#include "GraphicsManager.h"
#include "MeshRendererProxy.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//------------------------------------------------------------------------------
MeshRendererProxy::MeshRendererProxy()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_transform()
	, m_viewProj()
	, m_stateModified(true)
{
}

//------------------------------------------------------------------------------
MeshRendererProxy::~MeshRendererProxy()
{
}

//------------------------------------------------------------------------------
void MeshRendererProxy::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;

	Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();

	// シェーダ
	static const byte_t codeData[] =
	{
#include "Resource/MeshRenderer.fx.h"
	};
	static const size_t codeLen = LN_ARRAY_SIZE_OF(codeData);

	ShaderCompileResult result;
	m_shader.shader.Attach(device->CreateShader(codeData, codeLen, &result), false);
	LN_THROW(result.Level != ShaderCompileResultLevel_Error, CompilationException, result);
	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
}

//------------------------------------------------------------------------------
void MeshRendererProxy::SetTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void MeshRendererProxy::SetViewProjMatrix(const Matrix& matrix)
{
	m_viewProj = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void MeshRendererProxy::DrawMesh(StaticMeshModel* mesh)
{
	LN_CHECK_ARG(mesh != nullptr);
	auto* _this = this;

	if (m_stateModified)
	{
		LN_ENQUEUE_RENDER_COMMAND_3(
			FlushState, m_manager,
			MeshRendererProxy*, _this,
			Matrix, m_transform,
			Matrix, m_viewProj,
			{
				_this->FlushStateImpl(m_transform, m_viewProj);
			});
		m_stateModified = false;
	}
	
	

	DrawMeshCommandData data;
	data.vertexDeclaration = mesh->m_vertexDeclaration->GetDeviceObject();
	data.vertexBuffers[0] = mesh->m_vertexBuffer->GetDeviceObject();		// TODO: まだ1つだけ
	data.indexBuffer = mesh->m_indexBuffer->GetDeviceObject();
	data.triangleCount = mesh->m_triangleCount;
	LN_ENQUEUE_RENDER_COMMAND_2(
		FlushState, m_manager,
		MeshRendererProxy*, _this,
		DrawMeshCommandData, data,
		{
			_this->DrawMeshImpl(data);
		});
}

//------------------------------------------------------------------------------
void MeshRendererProxy::FlushStateImpl(const Matrix& world, const Matrix& viewProj)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
}

//------------------------------------------------------------------------------
void MeshRendererProxy::DrawMeshImpl(const DrawMeshCommandData& data)
{
	//auto* deserializer = m_manager->GetShaderVariableCommitSerializeHelper();
	//deserializer->Deserialize(variablesData.GetData(), variablesData.GetSize());

	//m_shader.pass->Apply();

	m_renderer->SetVertexDeclaration(data.vertexDeclaration);
	m_renderer->SetVertexBuffer(0, data.vertexBuffers[0]);		// TODO: まだ1つだけ
	m_renderer->SetIndexBuffer(data.indexBuffer);
	m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, data.triangleCount);
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
