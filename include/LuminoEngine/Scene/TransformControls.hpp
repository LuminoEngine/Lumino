
#pragma once
#include "Common.hpp"
//#include "WorldObject.hpp"

namespace ln {
class RenderingContext;
class UIEventArgs;
class WorldObject;
class MeshPrimitive;

enum class TransformControlsType
{
    Translation,
    Rotation,
    Scaling,
};

class TransformControls
	: public Object
{
public:
    void setTarget(WorldObject* value);


    void setViewInfo(const Vector3& viewPosition, const Matrix& view, const Matrix& proj, const Size& viewPixelSize);

    void setTransformType(TransformControlsType value);

    virtual void onRender(RenderingContext* context) /*override*/;
    virtual void onRoutedEvent(UIEventArgs* e) /*override*/;

protected:

LN_CONSTRUCT_ACCESS:
	TransformControls();
	virtual ~TransformControls() = default;
	void init();

private:
    enum class OperationType
    {
        None,
        X,
        Y,
        Z,
        XY,
        XZ,
        YZ,
        XYZ,

        //ViewXY,
        ViewZ,
    };

    static const float CenterBoxSize;
    static const float BarRadius;
    static const float BarLendth;
    static const float OnPlaneBoxSize;
    static const float RotationRingInner;
    static const float RotationRingOuter;
    static const float RotationViewZRingInner;
    static const float RotationViewZRingOuter;
    static const float BaseOpacity;

    bool enabled() const { return m_target != nullptr; }
    bool handleMouseDown(const Point& point);
    bool handleMouseUp(const Point& point);
    bool handleMouseMove(const Point& point);
    void onTargetTransformChanged();

    void startEditing();
    void submitEditing();
    void makeScreenFactor();
    OperationType getDirectionOperationType(float x, float y, Plane* outLocalPlane) const;
    void intersectsLocalAAPlanes(float x, float y, bool* xz, Vector3* ptXZ, bool* xy, Vector3* ptXY, bool* yz, Vector3* ptYZ, Ray* localViewRay) const;
    Ray makeLocalRay(float x, float y) const;

    Ref<WorldObject> m_target;

    Matrix m_view;
    Matrix m_proj;
    Vector3 m_viewPosition;
    Size m_viewPixelSize;

    TransformControlsType m_transformType;
    Matrix m_gizmoInitialTransform;
    Matrix m_gizmoTransform;	// Gizmo 自体の transform (視点距離によるスケーリングは含まれない)
    float m_displayScale;
    float m_screenFactor;

    OperationType m_operationType;

    // TransformControls によって操作される transform
    AttitudeTransform m_targetInitialTransform;
    AttitudeTransform m_targetTransform;

    // Dragging infomation
    bool m_dragging;
    Plane m_draggingLocalPlane;
    Vector3 m_draggingStartLocalPosition;
    Matrix m_draggingStartGizmoTransform;
    Point m_draggingStartViewPixelPoint;

    // Rendering resources
    Ref<Material> m_xColorMaterial;
    Ref<Material> m_yColorMaterial;
    Ref<Material> m_zColorMaterial;
    Ref<Material> m_activeColorMaterial;

    Ref<MeshPrimitive> m_translationGizmoMesh;

};

} // namespace ln
