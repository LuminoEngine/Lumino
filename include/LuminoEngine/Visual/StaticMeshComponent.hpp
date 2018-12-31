
#pragma once
#include "VisualComponent.hpp"

namespace ln {
class MeshModel;

class StaticMeshComponent
	: public VisualComponent
{
public:
    void setModel(MeshModel* model);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    StaticMeshComponent();
	virtual ~StaticMeshComponent();
	void initialize();

private:
    Ref<MeshModel> m_model;
};

} // namespace ln
