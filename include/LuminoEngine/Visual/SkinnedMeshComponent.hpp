
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
	virtual void onUpdate(float elapsedSeconds) override;
	virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();
	void init();

private:
    Ref<SkinnedMeshModel> m_model;
};

} // namespace ln
