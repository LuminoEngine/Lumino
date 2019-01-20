
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "VisualComponent.hpp"

namespace ln {

class PlaneComponent
    : public VisualComponent
{
public:
    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    PlaneComponent();
    virtual ~PlaneComponent();
    void init();

private:
    float m_width;
    float m_depth;
    Ref<Material> m_material;
};

class SphereComponent
	: public VisualComponent
{
public:
    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SphereComponent();
    virtual ~SphereComponent();
	void init();

private:
    Ref<Material> m_material;
};

} // namespace ln
