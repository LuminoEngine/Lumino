
#pragma once
#if 0
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class GpuParticle
	: public VisualComponent
{
public:

protected:
	GpuParticle();
	virtual ~GpuParticle();
	void Initialize(SceneGraphManager* manager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

#endif
