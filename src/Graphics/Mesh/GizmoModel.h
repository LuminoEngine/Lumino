
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

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
	void SetGizmoType(GizmoType type);
	
	void SetTargetTransform(const Matrix& transform);
	const Matrix& GetTargetTransform() const;
	
	void SetViewInfo(const Vector3& viewPosition, const Matrix& view, const Matrix& proj, const SizeI& viewPixelSize);
	void SetDisplayScale(float scale);
	
	bool InjectMouseDown(int x, int y);
	bool InjectMouseMove(int x, int y);
	bool InjectMouseUp(int x, int y);
	
	void Render(DrawList* context);
	
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
	};
	
	GizmoModel();
	virtual ~GizmoModel();
	void Initialize(GraphicsManager* manager);
	void MakeScreenFactor();
	
	OperationType GetDirectionOperationType(int x, int y);	// translation or scaling
	OperationType GetRotationOperationType(int x, int y);
	
	void IntersectsLocalPlanes(int x, int y, bool* xz, Vector3* ptXZ, bool* xy, Vector3* ptXY, bool* yz, Vector3* ptYZ, Ray* localViewRay);

private:
	RefPtr<MeshResource>	m_mesh;
	GizmoType				m_gizmoType;
	Matrix					m_targetTransform;	// Gizmo によって操作される Transform
	Matrix					m_gizmoTransform;	// Gizmo 自体の Transform (視点距離によるスケーリングは含まれない)
	Vector3					m_viewPosition;
	Matrix					m_view;
	Matrix					m_proj;
	float					m_displayScale;
	float					m_screenFactor;
	SizeI					m_viewPixelSize;
};

} // namespace detail
LN_NAMESPACE_END

