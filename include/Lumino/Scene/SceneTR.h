#if 0
#pragma once
#include "Common.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class HugePlane;
using HugePlanePtr = RefPtr<HugePlane>;

class HugePlane
	: public VisualNode
{
public:
	static HugePlanePtr Create(const Vector3& direction);

public:

	void SetTexture(Texture* texture);
	
LN_PROTECTED_INTERNAL_ACCESS:
	HugePlane();
	virtual ~HugePlane();
	void Initialize(SceneGraph* sceneGraph, const Vector3& direction);

	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	RefPtr<MeshResource>	m_mesh;
	Vector3					m_direction;
};

} // namespace tr
LN_NAMESPACE_END
#endif
