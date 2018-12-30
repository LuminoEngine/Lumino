
#pragma once
#include "VisualObject.hpp"

namespace ln {
class StaticMeshComponent;

class StaticMesh
	: public VisualObject
{
public:
    static Ref<StaticMesh> create();

protected:

LN_CONSTRUCT_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void initialize();

private:
    Ref<StaticMeshComponent> m_component;
};

} // namespace ln
