
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class GizmoModel
	: public RefObject
{
public:
	void SetTransform(const Matrix& transform);
	const Matrix& GetTransform() const;
	
	void SetViewInfo(const Matrix& view, const Matrix& proj, const Size& viewSize);
	void SetDisplayScale(float scale);
	
	bool InjectMouseDown(unsigned int x, unsigned int y);
	bool InjectMouseMove(unsigned int x, unsigned int y);
	bool InjectMouseUp(unsigned int x, unsigned int y);
	
	void Render(DrawList* context);
	
LN_INTERNAL_ACCESS:
	GizmoModel();
	virtual ~GizmoModel();
	void Initialize(GraphicsManager* manager);
	void MakeScreenFactor();

private:
	Matrix					m_transform;
	RefPtr<MeshResource>	m_mesh;
	Matrix					m_view;
	Matrix					m_proj;
	float					m_displayScale;
	float					m_screenFactor;
};

} // namespace detail
LN_NAMESPACE_END

