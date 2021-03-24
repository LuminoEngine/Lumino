
#pragma once
#include "../VisualObject.hpp"
#include "../Mesh/StaticMesh.hpp"

namespace ln {

/**
 * A cylinder mesh entity.
 */
LN_CLASS()
class CylinderMesh
	: public StaticMesh
{
	LN_BUILDER;
public:

LN_CONSTRUCT_ACCESS:
	CylinderMesh();
	virtual ~CylinderMesh();

	/** $Constructor */
	bool init(float radiusTop, float radiusBottom, float height, float segments, Material* material);
};

struct CylinderMesh::BuilderDetails : public StaticMesh::BuilderDetails
{
	float radiusTop;
	float radiusBottom;
	float height;
	int segments;
	Ref<Material> material;

	BuilderDetails();
	Ref<Object> create() const;
};

template<class T, class B, class D>
struct CylinderMesh::BuilderCore : public StaticMesh::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(StaticMesh::BuilderCore);

	/** Set radiusTop and radiusBottom. */
	B& radius(float radiusTop, float radiusBottom) { d()->radiusTop = radiusTop; d()->radiusBottom = radiusBottom; return self(); }

	/** Radius of the cylinder at the top. (Default: 0.5) */
	B& radiusTop(float value) { d()->radiusTop = value; return self(); }

	/** Radius of the cylinder at the bottom. (Default: 0.5) */
	B& radiusBottom(float value) { d()->radiusBottom = value; return self(); }

	/** Height of the cylinder. (Default: 1.0) */
	B& height(float value) { d()->height = value; return self(); }

	/** Number of segmented faces around the circumference of the cylinder. (Default: 16) */
	B& segments(int value) { d()->segments = value; return self(); }

	/** Material. (Default: Material::Default) */
	B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(CylinderMesh);

} // namespace ln
