
#include "GizmoModel.h"

LN_NAMESPACE_BEGIN
namespace detail {

//------------------------------------------------------------------------------
GizmoModel::GizmoModel()
	: m_mesh(nullptr)
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
void GizmoModel::SetTransform(const Matrix& transform)
{
	m_transform = transform;
}

//------------------------------------------------------------------------------
const Matrix& GizmoModel::GetTransform() const
{
	return m_transform;
}

//------------------------------------------------------------------------------
void GizmoModel::SetViewInfo(const Matrix& view, const Matrix& proj, const Size& viewSize)
{
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

} // namespace detail
LN_NAMESPACE_END

