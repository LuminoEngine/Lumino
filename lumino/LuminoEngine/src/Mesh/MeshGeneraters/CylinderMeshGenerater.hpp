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

class RegularCylinderMeshGenerater
	: public MeshGenerater
{
public:
	RegularCylinderMeshGenerater();
	bool init(float radiusTop, float radiusBottom, float height, int slices, int stacks);
	int vertexCount() const override { return (m_slices + 1) * (m_stacks + 5); }
	int indexCount() const override { return (m_slices * (m_stacks + 2)) * 6; }
	PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
	void copyFrom(const RegularCylinderMeshGenerater* other);
	LN_MESHGENERATOR_CLONE_IMPLEMENT(RegularCylinderMeshGenerater);

protected:
	void onGenerate(MeshGeneraterBuffer* buf) override;

private:
	inline void setV(MeshGeneraterBuffer* buf, int s, int t, const Vector3& pos, const Vector2& uv, const Vector3& normal) { buf->setV(s + ((m_slices + 1) * t), pos, uv, normal); }
	void addFace(MeshGeneraterBuffer* buf, int p0, int p1, int p2, int p3);

	float m_radiusTop;
	float m_radiusBottom;
	float m_height;
	int m_slices;
	int m_stacks;
	int m_indexPos;
};

} // namespace detail
} // namespace ln

