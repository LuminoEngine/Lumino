
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class Grid;
using GridPtr = RefPtr<Grid>;

/**
	@brief
*/
class Grid
	: public VisualNode		// TODO: Visual である必要ない気もするが・・・
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static GridPtr Create3D();

protected:
	Grid();
	virtual ~Grid();
	void Initialize(SceneGraphManager* manager);

	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
