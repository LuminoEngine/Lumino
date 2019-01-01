
#pragma once
#include "VisualComponent.hpp"

namespace ln {
class StaticMeshModel;

class StaticMeshComponent
	: public VisualComponent
{
public:
    void setModel(StaticMeshModel* model);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    StaticMeshComponent();
	virtual ~StaticMeshComponent();
	void initialize();

private:
    Ref<StaticMeshModel> m_model;
};

} // namespace ln
