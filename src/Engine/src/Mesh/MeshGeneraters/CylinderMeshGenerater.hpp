#pragma once
#include "MeshGenerater.hpp"

namespace ln {
namespace detail {

class CylinderMeshFactory
	: public MeshGenerater
{
public:	// MeshGenerater interface
	int vertexCount() const override { return (m_slices + 1) * (m_stacks + 3); }
	int indexCount() const override { return m_slices * (m_stacks + 2) * 6; }
	PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
    void copyFrom(const CylinderMeshFactory* other);
	LN_MESHGENERATOR_CLONE_IMPLEMENT(CylinderMeshFactory);

public:
	bool init(float radiusTop, float radiusBottom, float height, int slices, int stacks);
	void onGenerate(MeshGeneraterBuffer* buf) override;

private:
    float m_radiusTop;
	float m_radiusBottom;
	float m_height;
	int m_slices;
	int m_stacks;
};

} // namespace detail
} // namespace ln

