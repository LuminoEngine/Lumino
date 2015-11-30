
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class TileSet
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TileSet();
	virtual ~TileSet();
	void CreateCore(SceneGraphManager* manager);
	
	const PathName& GetImageFilePath();
	void SetImageFilePath(const PathName& filePath);

private:
	struct TileInfo
	{
		int	priority;
	};
	
	PathName	m_imageFilePath;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
