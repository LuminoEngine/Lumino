
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class TileLayer
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TileLayer();
	virtual ~TileLayer();
	void CreateCore(SceneGraphManager* manager);
	
	void SetTileId(int x, int y, int id);
	int GetTileId(int x, int y);

private:
	Array<int>	m_data;		// マップデータ本体。m_size.X*m_size.Y のサイズを持つ。
	Size		m_size;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
