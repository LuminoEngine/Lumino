#pragma once
#include <LuminoEngine/Mesh/detail/MeshGenerater.hpp>

namespace ln {
struct Vertex;
namespace detail {
struct TeapotPatch;

class TeapotMeshGenerater
	: public MeshGenerater
{
public:
	float m_size;
	int m_tessellation;

public:	// MeshGenerater interface
	int vertexCount() const override;
	int indexCount() const override;
	PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
	void copyFrom(const TeapotMeshGenerater* other)
	{
		MeshGenerater::copyFrom(other);
		m_size = other->m_size;
		m_tessellation = other->m_tessellation;
	}
	LN_MESHGENERATOR_CLONE_IMPLEMENT(TeapotMeshGenerater);

protected:
	void onGenerate(MeshGeneraterBuffer* buf) override;

private:
	static int getPatchBaseCalls();
	void computeTeapot(float size, size_t tessellation, bool rhcoords);
	void tessellatePatch(const TeapotPatch& patch, size_t tessellation, const Vector3& scale, bool isMirrored);
	void addVertex(const Vector3& pos, const Vector3& normal, const Vector2& texUV);
	void addIndex(uint32_t index);

	MeshGeneraterBuffer* m_buffer;
	//Vertex* m_vbBegin;
	//Vertex* m_vbPos;
	uint32_t m_vbPos;
	uint32_t m_ibPos;
};

} // namespace detail
} // namespace ln

