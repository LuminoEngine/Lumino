
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class SkinnedMeshComponent;

class SkinnedMesh
	: public VisualObject
{
public:
    static Ref<SkinnedMesh> create();
    static Ref<SkinnedMesh> load(const StringRef& filePath, float scale = 1.0f);

    SkinnedMeshComponent* skinnedMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SkinnedMesh();
	virtual ~SkinnedMesh();
	void init();
    void init(const StringRef& filePath, float scale);

private:
    Ref<SkinnedMeshComponent> m_component;
};

} // namespace ln
