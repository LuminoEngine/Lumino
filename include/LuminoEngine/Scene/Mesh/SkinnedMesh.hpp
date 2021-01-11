
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class MeshComponent;

class SkinnedMesh
	: public VisualObject
{
public:
    static Ref<SkinnedMesh> create();
    static Ref<SkinnedMesh> load(const StringRef& filePath, float scale = 1.0f);

    MeshComponent* meshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SkinnedMesh();
	virtual ~SkinnedMesh();
	void init();
    void init(const StringRef& filePath, float scale);

private:
    Ref<MeshComponent> m_component;
};

} // namespace ln
