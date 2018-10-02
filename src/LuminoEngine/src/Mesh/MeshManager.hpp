#pragma once

namespace ln {
class VertexDeclaration;
namespace detail {

enum PredefinedVertexLayout
{
	PredefinedVertexLayout_Standard,			// struct Vertex
	PredefinedVertexLayout_Standard_BlendWeight,	// struct Vertex, struct VertexBlendWeight

	PredefinedVertexLayout_Count,
};

class MeshManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	MeshManager();
	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
	const Ref<VertexDeclaration>& predefinedVertexLayout(PredefinedVertexLayout kind) const { return m_predefinedVertexLayouts[kind]; }

private:
	GraphicsManager* m_graphicsManager;
	std::array<Ref<VertexDeclaration>, PredefinedVertexLayout_Count> m_predefinedVertexLayouts;
};

} // namespace detail
} // namespace ln

