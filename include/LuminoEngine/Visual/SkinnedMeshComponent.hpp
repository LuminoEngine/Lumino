
#pragma once
#include "VisualComponent.hpp"

namespace ln {
class SkinnedMeshModel;

class SkinnedMeshComponent
	: public VisualComponent
{
public:
    void setModel(SkinnedMeshModel* model);
    SkinnedMeshModel* model() const;

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();
	void init();

private:
    Ref<SkinnedMeshModel> m_model;
};

} // namespace ln
