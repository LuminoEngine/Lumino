
#pragma once
#include "../VisualObject.hpp"
#include "../Mesh/StaticMesh.hpp"

namespace ln {

/**
 * A cone mesh entity.
 */
LN_CLASS()
class ConeMesh
	: public StaticMesh
{
	LN_BUILDER;
public:

LN_CONSTRUCT_ACCESS:
	ConeMesh();
	virtual ~ConeMesh();

	/** $Constructor */
	bool init(float radius, float height, float segments, Material* material);
};

struct ConeMesh::BuilderDetails : public StaticMesh::BuilderDetails
{
	float radius;
	float height;
	int segments;
	Ref<Material> material;

	BuilderDetails();
	Ref<Object> create() const;
};

template<class T, class B, class D>
struct ConeMesh::BuilderCore : public StaticMesh::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(StaticMesh::BuilderCore);

	/** Radius of the cone base. (Default: 1.0) */
	B& radius(float value) { d()->radius = value; return self(); }

	/** Height of the cone. (Default: 1.0) */
	B& height(float value) { d()->height = value; return self(); }

	/** Number of segmented faces around the circumference of the cone. (Default: 16) */
	B& segments(int value) { d()->segments = value; return self(); }

	/** Material. (Default: Material::Default) */
	B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(ConeMesh);

} // namespace ln
