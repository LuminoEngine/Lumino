#pragma once

namespace ln {
namespace detail {
class LinearAllocator;
class MeshGenerater;
}
class Mesh;

class MeshGeometryBuilder
	: public Object
{
public:
	void setTransform(const AttitudeTransform& value);
	void setPosition(const Vector3& value);
	void setRotation(const Vector3& value);
	void setScale(const Vector3& value);

	void setColor(const Color& value);

	void beginSection();

	// Y+
	void addPlane(const Vector2& size, int sliceH, int sliceV);
	
	void addBox(const Vector3& sizes);

	void addSphere(float radius, int sliceH, int sliceV);

	void addCylinder(float radius, float height, int slices, int stacks);

	void endSection();

	Ref<Mesh> buildMesh();
	
LN_CONSTRUCT_ACCESS:
	MeshGeometryBuilder();
	virtual ~MeshGeometryBuilder();
	void init();

private:
	template<class T>
	T* newMeshGenerater()
	{
		void* buffer = m_allocator->allocate(sizeof(T));
		T* data = new (buffer)T();
		data->setTransform(Matrix(m_transform));
		data->setColor(m_color);
		m_generators.add(Geometry{ data });
		//allocateNextTransform();
		return data;
	}

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

