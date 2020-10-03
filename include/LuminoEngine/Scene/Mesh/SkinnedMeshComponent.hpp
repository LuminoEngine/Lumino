
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {
class SkinnedMeshModel;

/**
 * SkinnedMeshComponent
 */
LN_CLASS()
class SkinnedMeshComponent
	: public VisualComponent
{
public:
	/** setModel */
	LN_METHOD()
    void setModel(SkinnedMeshModel* model);

    SkinnedMeshModel* model() const;

protected:
	virtual void onUpdate(float elapsedSeconds) override;
	virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();

	/** init */
	LN_METHOD()
	void init();

private:
    Ref<SkinnedMeshModel> m_model;
};

} // namespace ln
