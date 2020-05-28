
#pragma once
#include "../../Graphics/GeometryStructs.hpp"
#include "../../Visual/VisualComponent.hpp"

namespace ln {

class PlaneMeshComponent
    : public VisualComponent
{
public:
	void setSize(const Vector2& size) { m_size = size; }
	void setSize(float sizeX, float sizeY) { setSize(Vector2(sizeX, sizeY)); }

    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    PlaneMeshComponent();
    virtual ~PlaneMeshComponent();
    void init();

private:
	Vector2 m_size;
    float m_sizeX;
    float m_sizeY;
    Ref<Material> m_material;
};

class SphereMeshComponent
	: public VisualComponent
{
public:
    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SphereMeshComponent();
    virtual ~SphereMeshComponent();
	void init();

private:
    Ref<Material> m_material;
};

class BoxMeshComponent
	: public VisualComponent
{
public:
	void setMaterial(Material* material);

protected:
	void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	BoxMeshComponent();
	virtual ~BoxMeshComponent();
	void init();

private:
	Ref<Material> m_material;
	Box m_box;
};

} // namespace ln
