
#pragma once
#include "VisualObject.hpp"

namespace ln {
class StaticMeshComponent;

class StaticMesh
	: public VisualObject
{
public:
    static Ref<StaticMesh> create();
    static Ref<StaticMesh> create(const StringRef& filePath);

    StaticMeshComponent* staticMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void initialize();
    void initialize(const StringRef& filePath);

private:
    Ref<StaticMeshComponent> m_component;
};

} // namespace ln
