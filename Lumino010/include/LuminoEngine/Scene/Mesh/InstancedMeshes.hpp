
#pragma once
#include "../../Visual/VisualComponent.hpp"
#include "../VisualObject.hpp"

namespace ln {
class InstancedMeshList;

class InstancedMeshesComponent
    : public VisualComponent
{
public:
    void setModel(InstancedMeshList* value);

    InstancedMeshList* model() const;

protected:
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    InstancedMeshesComponent();
    virtual ~InstancedMeshesComponent();
    bool init();

private:
    Ref<InstancedMeshList> m_model;
    Ref<Material> m_material;
};

/**
 */
class InstancedMeshes
	: public VisualObject
{
public:
    InstancedMeshesComponent* instancedMeshesComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	InstancedMeshes();
	virtual ~InstancedMeshes();
	bool init();

private:
    Ref<InstancedMeshesComponent> m_component;
};

} // namespace ln
