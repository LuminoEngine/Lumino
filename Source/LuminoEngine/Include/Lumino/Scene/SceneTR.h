#if 0
#pragma once
#include "Common.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class HugePlane;
using HugePlanePtr = Ref<HugePlane>;

class HugePlane
	: public VisualComponent
{
public:
	static HugePlanePtr create(const Vector3& direction);

public:

	void SetTexture(Texture* texture);
	
LN_PROTECTED_INTERNAL_ACCESS:
	HugePlane();
	virtual ~HugePlane();
	void initialize(SceneGraph* sceneGraph, const Vector3& direction);

	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	Ref<MeshResource>	m_mesh;
	Vector3					m_direction;
};

} // namespace tr
LN_NAMESPACE_END
#endif
