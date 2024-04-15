#pragma once

namespace ln {
namespace detail {
class LinearAllocator;
class MeshGenerater;
}
class MeshPrimitive;

class MeshGeometryBuilder
	: public Object
{
public:
	void setTransform(const AttitudeTransform& value);
	void setPosition(const Vector3& value);
	void setPosition(float x, float y, float z) { setPosition(Vector3(x, y, z)); }
	void setRotation(const Vector3& value);
	void setRotation(float x, float y, float z) { setRotation(Vector3(x, y, z)); }
	void setScale(const Vector3& value);

	void setColor(const Color& value);

	void beginSection();

	// Y+
	void addPlane(const Vector2& size, int sliceH, int sliceV);
	
	void addBox(const Vector3& sizes);

	void addSphere(float radius, int sliceH, int sliceV);

	void addCylinder(float radius, float height, int slices, int stacks);

	void addCone(float radius, float height, int slices);

	void addArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices);

	void endSection();

	Ref<MeshPrimitive> buildMesh();
	
LN_CONSTRUCT_ACCESS:
	MeshGeometryBuilder();
	virtual ~MeshGeometryBuilder();
	void init();

private:
	template<class T>
	T* newMeshGenerater();

	//void allocateNextTransform();

	struct Section
	{
		int startGenerator;
		int generatorCount;
	};

	struct Geometry
	{
		//const Matrix* transform;
		/*const*/ detail::MeshGenerater* generator;
	};

	Ref<detail::LinearAllocator> m_allocator;
	AttitudeTransform m_transform;
	Color m_color;
	List<Geometry> m_generators;
	List<Section> m_currentMeshSections;
};

} // namespace ln

