
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class BoxMeshComponent;
class SphereMeshComponent;

class BoxMesh
	: public VisualObject
{
public:
	static Ref<BoxMesh> create();

	BoxMeshComponent* boxMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	BoxMesh();
	virtual ~BoxMesh();
	void init();

private:
    Ref<BoxMeshComponent> m_component;
};

class SphereMesh
	: public VisualObject
{
public:
	static Ref<SphereMesh> create();

	SphereMeshComponent* sphereMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SphereMesh();
	void init();

private:
	Ref<SphereMeshComponent> m_component;
};

} // namespace ln
