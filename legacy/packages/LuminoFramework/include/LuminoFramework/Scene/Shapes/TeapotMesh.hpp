
#pragma once
#include "../Mesh/StaticMesh.hpp"

namespace ln {

/**
 * A teapot mesh entity.
 */
LN_CLASS()
class TeapotMesh
	: public StaticMesh
{
	LN_BUILDER;
public:

LN_CONSTRUCT_ACCESS:
	TeapotMesh();
	virtual ~TeapotMesh();

	/** $Constructor */
	bool init(Material* material);
};

struct TeapotMesh::BuilderDetails : public StaticMesh::BuilderDetails
{
	Ref<Material> material;

	BuilderDetails();
	Ref<Object> create() const;
};

template<class T, class B, class D>
struct TeapotMesh::BuilderCore : public StaticMesh::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(StaticMesh::BuilderCore);

	/** Material. (Default: Material::Default) */
	B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(TeapotMesh);

} // namespace ln
