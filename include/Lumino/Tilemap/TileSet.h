
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class Texture;

/**
	@brief	
*/
class TileSet
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	TileSet();
	virtual ~TileSet();
	void CreateCore();

	void SetImageSource(Texture* texture);
	Texture* GetImageSource() const;
	
	//const PathName& GetImageFilePath();
	//void SetImageFilePath(const PathName& filePath);

private:
	struct TileInfo
	{
		int	priority;
	};
	
	Texture*	m_imageSource;
	//PathName	m_imageFilePath;
};

LN_NAMESPACE_END
