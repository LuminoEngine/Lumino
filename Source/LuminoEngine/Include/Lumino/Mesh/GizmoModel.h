
#pragma once

LN_NAMESPACE_BEGIN
class DrawList;

namespace tr {
class GizmoModel;
using GizmoModelPtr = Ref<GizmoModel>;

enum class GizmoType
{
	Translation,
	Rotation,
	Scaling,
};

class GizmoModel
	: public RefObject
{
public:
	static GizmoModelPtr create();

public:
	void setGizmoType(GizmoType type);

	void setup(const Matrix& parentSpaceTransform, const AttitudeTransform& targetInitialTransform);
	
	//void SetTargetTransform(const Matrix& transform);
	const AttitudeTransform& GetTargetTransform() const;
	
	void setViewInfo(const Vector3& viewPosition, const Matrix& view, const Matrix& proj, const SizeI& viewPixelSize);
	void setDisplayScale(float scale);
	
	bool injectMouseDown(int x, int y);
	bool injectMouseMove(int x, int y);
	bool injectMouseUp(int x, int y);
	//bool injectKeyDown(Keys keyCode, ModifierKeys modifierKeys);

	void render(DrawList* context);

	void addOnTargetTransformChanged(const std::function<void(GizmoModel*)>& handler)
	{
		m_onTargetTransformChanged.connect(handler);
	}
	void addOnSubmitEditing(const std::function<void(GizmoModel*)>& handler)
	{
		m_onSubmitEditing.connect(handler);
	}
	
LN_INTERNAL_ACCESS:
	
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
	
	GizmoModel();
	virtual ~GizmoModel();
	void initialize(ln::detail::GraphicsManager* manager);

	void submitEditing();

	void makeScreenFactor();
	
	OperationType getDirectionOperationType(int x, int y, Plane* outLocalPlane = nullptr);	// translation or scaling
	OperationType getRotationOperationType(int x, int y);
	
	Ray makeLocalRay(int x, int y);
	void intersectsLocalPlanes(int x, int y, bool* xz, Vector3* ptXZ, bool* xy, Vector3* ptXY, bool* yz, Vector3* ptYZ, Ray* localViewRay);

private:
	static const float CenterBoxSize;
	static const float BarRadius;
	static const float BarLendth;
	static const float OnPlaneBoxSize;
	static const float RotationRingInner;
	static const float RotationRingOuter;
	static const float RotationViewZRingInner;
	static const float RotationViewZRingOuter;
	static const float BaseOpacity;

	//Ref<MeshResource>	m_mesh;
	GizmoType				m_gizmoType;
	//Matrix					m_parentSpaceTransform;
	AttitudeTransform					m_targetInitialTransform;
	AttitudeTransform					m_targetTransform;	// Gizmo によって操作される transform
	Matrix					m_gizmoInitialTransform;
	Matrix					m_gizmoTransform;	// Gizmo 自体の transform (視点距離によるスケーリングは含まれない)
	Vector3					m_viewPosition;
	Matrix					m_view;
	Matrix					m_proj;
	float					m_displayScale;
	float					m_screenFactor;
	SizeI					m_viewPixelSize;

	OperationType			m_operationType;
	Ref<CommonMaterial>		m_tmat;

	bool					m_dragging;
	Plane					m_draggingLocalPlane;
	Vector3					m_draggingStartLocalPosition;
	Matrix					m_draggingStartGizmoTransform;
	PointI					m_draggingStartViewPixelPoint;

	Event<GizmoModel*>	m_onTargetTransformChanged;
	Event<GizmoModel*>	m_onSubmitEditing;
};

} // namespace tr
LN_NAMESPACE_END

