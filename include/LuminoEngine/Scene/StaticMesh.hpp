
#pragma once
#include "VisualObject.hpp"

namespace ln {
class StaticMeshComponent;

class StaticMesh
	: public VisualObject
{
public:
    static Ref<StaticMesh> create();

    StaticMeshComponent* staticMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void initialize();

private:
    Ref<StaticMeshComponent> m_component;
};

} // namespace ln
