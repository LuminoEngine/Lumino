
#pragma once
#include "VisualObject.hpp"

namespace ln {
class SkinnedMeshComponent;

class SkinnedMesh
	: public VisualObject
{
public:
    static Ref<SkinnedMesh> create();
    static Ref<SkinnedMesh> create(const StringRef& filePath, float scale = 1.0f);

    SkinnedMeshComponent* staticMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SkinnedMesh();
	virtual ~SkinnedMesh();
	void initialize();
    void initialize(const StringRef& filePath, float scale);

private:
    Ref<SkinnedMeshComponent> m_component;
};

} // namespace ln
