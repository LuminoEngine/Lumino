
#pragma once
#include "../../Graphics/GeometryStructs.hpp"
#include "../../Visual/VisualComponent.hpp"

namespace ln {
    
class ShapeComponent
    : public VisualComponent
{
    LN_OBJECT;
public:
    void setMaterial(Material* material);
    Material* material() const;

protected:
    void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
    ShapeComponent();
    bool init();

private:
    Ref<Material> m_material;
};

class PlaneMeshComponent
    : public ShapeComponent
{
    LN_OBJECT;
public:
    void setSize(const Vector2& size) { m_size = size; }
    void setSize(float sizeX, float sizeY) { setSize(Vector2(sizeX, sizeY)); }

    void setUVParUnit(const Vector2& value) { m_uvParUnit = value; }

protected:
    void serialize(Serializer2& ar) override;
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    PlaneMeshComponent();
    virtual ~PlaneMeshComponent();
    void init();

private:
    Vector2 m_size;
    Vector2 m_uvParUnit;
};

class SphereMeshComponent
    : public ShapeComponent
{
public:

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SphereMeshComponent();
    virtual ~SphereMeshComponent();
    bool init();

private:
};

//class BoxMeshComponent
//	: public ShapeComponent
//{
//public:
//    /** @copydoc BoxMesh::setSize */
//    void setSize(const Vector3& size);
//
//protected:
//	void onRender(RenderingContext* context);
//
//LN_CONSTRUCT_ACCESS:
//	BoxMeshComponent();
//	virtual ~BoxMeshComponent();
//    bool init();
//    bool init(const Vector3& size);
//
//private:
//	//Box m_box;
//    Ref<MeshModel> m_model;
//};

} // namespace ln
