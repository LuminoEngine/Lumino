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
	void beginSection();
	void addBox(const Vector3& sizes);
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
		m_generators.add(data);
		return data;
	}

	struct Section
	{
		int startGenerator;
		int generatorCount;
	};

	Ref<detail::LinearAllocator> m_allocator;
	List<detail::MeshGenerater*> m_generators;
	List<Section> m_currentMeshSections;
};

} // namespace ln

