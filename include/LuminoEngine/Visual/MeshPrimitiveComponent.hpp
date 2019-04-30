
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "VisualComponent.hpp"

namespace ln {

class PlaneComponent
    : public VisualComponent
{
public:
	void setSize(const Vector2& size) { m_size = size; }
	void setSize(float sizeX, float sizeY) { setSize(Vector2(sizeX, sizeY)); }

    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    PlaneComponent();
    virtual ~PlaneComponent();
    void init();

private:
	Vector2 m_size;
    float m_sizeX;
    float m_sizeY;
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

class BoxComponent
	: public VisualComponent
{
public:
	void setMaterial(Material* material);

protected:
	void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	BoxComponent();
	virtual ~BoxComponent();
	void init();

private:
	Ref<Material> m_material;
};

} // namespace ln
