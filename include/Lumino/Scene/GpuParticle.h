
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class GpuParticle
	: public VisualNode
{
public:

protected:
	GpuParticle();
	virtual ~GpuParticle();
	void Initialize(SceneGraphManager* manager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
