
#pragma once
#include "../../Visual/VisualComponent.hpp"
#include "Common.hpp"

namespace ln {

class MeshVoxelmapComponent
	: public VisualComponent
{
    LN_OBJECT;
public:
    MeshVoxelmapModel* model() const;
    void setModel(MeshVoxelmapModel* value);

protected:
    virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    MeshVoxelmapComponent();
	void init();

private:
    Ref<MeshVoxelmapModel> m_model;
};

} // namespace ln
