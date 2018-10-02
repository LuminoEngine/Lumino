
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include "MeshManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshManager
	
MeshManager::MeshManager()
	: m_graphicsManager(nullptr)
	, m_predefinedVertexLayouts{}
{
}

void MeshManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;

	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
			{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
			{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
			{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
		};
		m_predefinedVertexLayouts[PredefinedVertexLayout_Standard] = newObject<VertexDeclaration>(elements, 4);
	}

	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
			{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
			{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
			{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
			{ 1, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0 },
			{ 1, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0 },
		};
		m_predefinedVertexLayouts[PredefinedVertexLayout_Standard_BlendWeight] = newObject<VertexDeclaration>(elements, 4);
	}
}

void MeshManager::dispose()
{
	for (int i = 0; i < m_predefinedVertexLayouts.size(); i++) {
		m_predefinedVertexLayouts[i] = nullptr;
	}
}

} // namespace detail
} // namespace ln

