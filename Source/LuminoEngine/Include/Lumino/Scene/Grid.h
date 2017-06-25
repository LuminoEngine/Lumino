
#pragma once
#if 0
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class Grid;
using GridPtr = RefPtr<Grid>;

/**
	@brief
*/
class Grid
	: public VisualComponent		// TODO: Visual である必要ない気もするが・・・
{
	LN_OBJECT();
public:
	static GridPtr Create3D();

protected:
	Grid();
	virtual ~Grid();
	void initialize(SceneGraph* owner);

	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
#endif
