
#pragma once
#include "../../Visual/VisualComponent.hpp"
#include "Common.hpp"

namespace ln {

class MeshTilemapComponent
	: public VisualComponent
{
    LN_OBJECT;
public:
    MeshTilemapModel* model() const;
    void setModel(MeshTilemapModel* value);

protected:
    virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    MeshTilemapComponent();
	void init();

private:
    Ref<MeshTilemapModel> m_model;
};

} // namespace ln
