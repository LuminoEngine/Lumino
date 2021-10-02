
#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoGraphics/Common.hpp>
#include "../Mesh/Common.hpp"

namespace ln {
	
class InstancedMeshList
	: public Object
{
public:
	void reset();
	void setTransform(const Matrix& transform);
	void setUVOffset(const Vector4& value);
	void setColorScale(const Color& value);
	void drawMesh();


	const Ref<MeshPrimitive>& mesh() const { return m_mesh; }
	int sectionIndex() const { return m_sectionIndex; }
	int instanceCount() const { return m_instanceCount; }
	void commitRenderData(MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB);

LN_CONSTRUCT_ACCESS:
	InstancedMeshList();
	virtual ~InstancedMeshList();
	bool init(MeshPrimitive* mesh, int sectionIndex);

private:
	struct InstanceData
	{
		Vector4 transform0;
		Vector4 transform1;
		Vector4 transform2;
		Vector4 transform3;
		Vector4 uvOffset;	// .xy=pos, .zw=scale
		Vector4 colorScale;
	};

	Ref<MeshPrimitive> m_mesh;
	int m_sectionIndex;
	InstanceData m_stagingData;
	std::vector<InstanceData> m_instanceData;
	int m_instanceCount;
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_instanceBuffer;
	VertexLayout* m_sourceVertexLayout;
	bool m_dirty;
};

} // namespace ln
