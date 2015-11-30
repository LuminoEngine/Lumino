
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class TileLayer;

/**
	@brief	
*/
class TileMap
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TileMap();
	virtual ~TileMap();
	void CreateCore(SceneGraphManager* manager);

private:
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
