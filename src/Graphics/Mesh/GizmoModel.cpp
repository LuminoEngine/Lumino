
#include "GizmoModel.h"

LN_NAMESPACE_BEGIN
namespace detail {

//------------------------------------------------------------------------------
GizmoModel::GizmoModel()
	: m_mesh(nullptr)
	, m_gizmoType(GizmoType::Translation)
	, m_displayScale(1.0f)
{
}

//------------------------------------------------------------------------------
GizmoModel::~GizmoModel()
{
}

//------------------------------------------------------------------------------
void GizmoModel::Initialize(GraphicsManager* manager)
{
}

//------------------------------------------------------------------------------
void GizmoModel::SetGizmoType(GizmoType type)
{
	m_gizmoType = type;
}

//------------------------------------------------------------------------------
void GizmoModel::SetTargetTransform(const Matrix& transform)
{
	m_targetTransform = transform;
}

//------------------------------------------------------------------------------
const Matrix& GizmoModel::GetTargetTransform() const
{
	return m_targetTransform;
}

//------------------------------------------------------------------------------
void GizmoModel::SetViewInfo(const Vector3& viewPosition, const Matrix& view, const Matrix& proj, const SizeI& viewPixelSize)
{
	m_viewPosition = viewPosition;
	m_view = view;
	m_proj = proj;
	m_viewPixelSize = viewPixelSize;
	
	MakeScreenFactor();
}

//------------------------------------------------------------------------------
void GizmoModel::SetDisplayScale(float scale)
{
	m_displayScale = scale;
}

//------------------------------------------------------------------------------
bool GizmoModel::InjectMouseDown(unsigned int x, unsigned int y)
{
}

//------------------------------------------------------------------------------
bool GizmoModel::InjectMouseMove(unsigned int x, unsigned int y)
{
}

//------------------------------------------------------------------------------
bool GizmoModel::InjectMouseUp(unsigned int x, unsigned int y)
{
}

//------------------------------------------------------------------------------
void GizmoModel::Render(DrawList* context)
{
}

//------------------------------------------------------------------------------
void GizmoModel::MakeScreenFactor()
{
	Matrix viewproj = m_view * m_proj;
	Vector4 trf = Vector4(m_transform.GetPosition(), 1.0f);
	trf.Transform(viewproj);
	m_screenFactor = m_displayScale * 0.15f * trf.w;
}

//------------------------------------------------------------------------------
OperationType GizmoModel::GetDirectionOperationType(int x, int y)
{
	const float MoveXYZBoxSize = 0.25;
	const float MoveOnPlaneBoxSize = 0.5;
	
 	bool xz, xy, yz;
	Vector3 ptXZ, ptXY, ptYZ;
	Ray ray;
	IntersectsLocalPlanes(x, y, &xz, &ptXZ, &xy, &ptXY, &yz, &ptYZ, &ray);
	
	if (xz && xy && yz &&
		(abs(ptXZ.x) <= MoveXYZBoxSize && (ptXZ.z) <= MoveXYZBoxSize) &&
		(abs(ptXY.x) <= MoveXYZBoxSize && (ptXY.y) <= MoveXYZBoxSize) &&
		(abs(ptYZ.y) <= MoveXYZBoxSize && (ptYZ.z) <= MoveXYZBoxSize))
	{
		return OperationType::XYZ;
	}
	if (xz)
	{
		if ((ptXZ.x >= 0) && (ptXZ.x <= 1) && ( fabs(ptXZ.z) < 0.1f)) return OperationType::X;
		if ((ptXZ.z >= 0) && (ptXZ.z <= 1) && ( fabs(ptXZ.x) < 0.1f)) return OperationType::Z;
		if ((ptXZ.x < MoveOnPlaneBoxSize) && (ptXZ.z < MoveOnPlaneBoxSize) && (ptXZ.x > 0) && (ptXZ.z > 0))  return OperationType::XZ;
	}
	if (xy)
	{
		if ((ptXY.x >= 0 ) && (ptXY.x <= 1) && (fabs(ptXY.y) < 0.1f)) return OperationType::X;
		if ((ptXY.y >= 0 ) && (ptXY.y <= 1) && (fabs(ptXY.x) < 0.1f)) return OperationType::Y;
		if ((ptXY.x < MoveOnPlaneBoxSize) && (ptXY.y < MoveOnPlaneBoxSize) && (ptXY.x > 0) && (ptXY.y > 0)) return OperationType::XY;
	}
	if (yz)
	{
		if ((ptYZ.y >= 0) && (ptYZ.y <= 1) && (fabs(ptYZ.z) < 0.1f)) return OperationType::Y;
		if ((ptYZ.z >= 0) && (ptYZ.z <= 1) && (fabs(ptYZ.y) < 0.1f)) return OperationType::Z;
		if ((ptYZ.y < MoveOnPlaneBoxSize) && (ptYZ.z < MoveOnPlaneBoxSize) && (ptYZ.y > 0) && (ptYZ.z > 0)) return OperationType::YZ;
	}
	return OperationType::None;
}

//------------------------------------------------------------------------------
OperationType GizmoModel::GetRotationOperationType(int x, int y)
{
 	bool xz, xy, yz;
	Vector3 ptXZ, ptXY, ptYZ;
	Ray ray;
	IntersectsLocalPlanes(x, y, &xz, &ptXZ, &xy, &ptXY, &yz, &ptYZ, &ray);
	
	LN_NOTIMPLEMENTED();
	
	return OperationType::None;
}

//------------------------------------------------------------------------------
void GizmoModel::IntersectsLocalPlanes(int x, int y, bool* xz, Vector3* ptXZ, bool* xy, Vector3* ptXY, bool* yz, Vector3* ptYZ, Ray* localViewRay)
{
	Matrix gizmoMat = m_gizmoTransform;
	gizmoMat.Scaling(m_screenFactor);
	
	Matrix viewproj = m_view * m_proj;
	
	Ray ray;
	ray.origin = m_viewPosition;
	ray.direction = Vector3::Unproject(Vector3(x, y, 0), viewproj, 0, 0, m_viewPixelSize.width, m_viewPixelSize.height);
	ray.direction.Normalize();
	
	// ray Ç gizmo ÇÃÉçÅ[ÉJÉããÛä‘Ç÷ïœä∑Ç∑ÇÈ
	Matrix gizmoInv = Matrix::MakeInverse(gizmoMat);
	{
		//ray.TransformCoord(gizmoInv);
		ray.direction += ray.origin;
		ray.origin.TransformCoord(gizmoInv);
		ray.direction.TransformCoord(gizmoInv);
		ray.direction -= ray.origin;
	}
	
	// äeé≤ïΩñ Ç∆è’ìÀîªíË
	*xz = Plane(Vector3::UnitY).Intersects(ray, ptXZ);
	*xy = Plane(Vector3::UnitZ).Intersects(ray, ptXY);
	*yz = Plane(Vector3::UnitX).Intersects(ray, ptYZ);
	
	*localViewRay = ray;
}

} // namespace detail
LN_NAMESPACE_END

