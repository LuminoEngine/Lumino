﻿
#include "../Internal.h"
#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Graphics/Mesh/GizmoModel.h>
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace tr {

//------------------------------------------------------------------------------
GizmoModelPtr GizmoModel::Create()
{
	auto ptr = GizmoModelPtr::MakeRef();
	ptr->Initialize(ln::detail::GraphicsManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
GizmoModel::GizmoModel()
	: /*m_mesh(nullptr)
	, */m_gizmoType(GizmoType::Translation)
	, m_displayScale(1.0f)
	, m_operationType(OperationType::None)
{
}

//------------------------------------------------------------------------------
GizmoModel::~GizmoModel()
{
}

//------------------------------------------------------------------------------
void GizmoModel::Initialize(ln::detail::GraphicsManager* manager)
{
	m_tmat = RefPtr<Material>::MakeRef();
	m_tmat->Initialize();
	m_tmat->blendMode = BlendMode::Alpha;
	m_tmat->cullingMode = CullingMode::None;
}

//------------------------------------------------------------------------------
void GizmoModel::SetGizmoType(GizmoType type)
{
	m_gizmoType = type;
}

//------------------------------------------------------------------------------
void GizmoModel::Setup(const Matrix& parentSpaceTransform, const Matrix& targetInitialTransform)
{
	Matrix targetWorld = parentSpaceTransform * targetInitialTransform;
	m_gizmoTransform = targetWorld;
}

////------------------------------------------------------------------------------
//void GizmoModel::SetTargetTransform(const Matrix& transform)
//{
//	m_targetTransform = transform;
//}

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
bool GizmoModel::InjectMouseDown(int x, int y)
{
	return false;
}

//------------------------------------------------------------------------------
bool GizmoModel::InjectMouseMove(int x, int y)
{
	m_operationType = GetDirectionOperationType(x, y);
	return false;
}

//------------------------------------------------------------------------------
bool GizmoModel::InjectMouseUp(int x, int y)
{
	return false;
}

//------------------------------------------------------------------------------
void GizmoModel::Render(DrawList* context)
{
	Matrix gizmoMat;
	gizmoMat.Scale(m_screenFactor);
	gizmoMat *= m_gizmoTransform;
	context->SetTransform(gizmoMat);	// TODO: old

	float r = 0.025f;
	float d = 1.0f;

	Color c;

	// X axis
	c = (m_operationType == OperationType::X) ? Color::White : Color::Red;
	context->DrawCylinder(r, d, 8, 1, c, Matrix::MakeRotationZ(-Math::PIDiv2) * Matrix::MakeTranslation(d / 2, 0, 0));
	context->DrawCone(r * 3, r * 6, 8, c, Matrix::MakeRotationZ(-Math::PIDiv2) * Matrix::MakeTranslation(d, 0, 0));

	// Y axis
	c = (m_operationType == OperationType::Y) ? Color::White : Color::Green;
	context->DrawCylinder(r, d, 8, 1, c, Matrix::MakeTranslation( 0, d / 2,0));
	context->DrawCone(r * 3, r * 6, 8, c, Matrix::MakeTranslation(0, d, 0));

	// Z axis
	c = (m_operationType == OperationType::Z) ? Color::White : Color::Blue;
	context->DrawCylinder(r, d, 8, 1, c, Matrix::MakeRotationX(Math::PIDiv2) * Matrix::MakeTranslation(0, 0, d / 2));
	context->DrawCone(r * 3, r * 6, 8, c, Matrix::MakeRotationX(Math::PIDiv2) * Matrix::MakeTranslation(0, 0, d));

	// center
	c = (m_operationType == OperationType::XYZ) ? Color::White : Color::Yellow;
	context->DrawSphere(0.1f, 8, 8, c);

	float s = 0.3f;
	float s2 = s / 2;
	context->DrawSquare(s, s, 1, 1, Color(0, 1, 1, 0.5), Matrix::MakeRotationZ(-Math::PIDiv2) * Matrix::MakeTranslation(0, s2, s2), m_tmat);
	context->DrawSquare(s, s, 1, 1, Color(1, 0, 1, 0.5), Matrix::MakeTranslation(s2, 0, s2), m_tmat);
	context->DrawSquare(s, s, 1, 1, Color(1, 1, 0, 0.5), Matrix::MakeRotationX(Math::PIDiv2) * Matrix::MakeTranslation(s2, s2, 0), m_tmat);


	context->SetTransform(Matrix::Identity);
}

//------------------------------------------------------------------------------
void GizmoModel::MakeScreenFactor()
{
	Matrix viewproj = m_view * m_proj;
	Vector4 trf = Vector4(m_gizmoTransform.GetPosition(), 1.0f);
	trf = Vector4::Transform(trf, viewproj);
	m_screenFactor = m_displayScale * 0.15f * trf.w;
}

//------------------------------------------------------------------------------
GizmoModel::OperationType GizmoModel::GetDirectionOperationType(int x, int y)
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
GizmoModel::OperationType GizmoModel::GetRotationOperationType(int x, int y)
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
	////gizmoMat.Scale(m_screenFactor);
	
	Matrix viewproj = m_view * m_proj;
	
	Ray ray;
	ray.origin = m_viewPosition;
	ray.direction = Vector3::Unproject(Vector3(x, y, 0), viewproj, 0, 0, m_viewPixelSize.width, m_viewPixelSize.height);
	ray.direction = ray.direction - ray.origin;
	
	// ray を gizmo のローカル空間へ変換する
	Matrix gizmoInv = Matrix::MakeInverse(gizmoMat);
	{
		//ray.TransformCoord(gizmoInv);
		ray.direction += ray.origin;
		ray.origin.TransformCoord(gizmoInv);
		ray.direction.TransformCoord(gizmoInv);
		ray.direction -= ray.origin;
		ray.direction.Normalize();
	}
	
	// 各軸平面と衝突判定
	*xz = Plane(Vector3::UnitY).Intersects(ray, ptXZ);
	*xy = Plane(Vector3::UnitZ).Intersects(ray, ptXY);
	*yz = Plane(Vector3::UnitX).Intersects(ray, ptYZ);
	
	*localViewRay = ray;
}

} // namespace tr
LN_NAMESPACE_END

