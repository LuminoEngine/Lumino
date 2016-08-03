
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class Texture;
class TileSet;
typedef RefPtr<TileSet> TileSetPtr;

/**
	@brief	
*/
class TileSet
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static TileSetPtr Create();

public:
	TileSet();
	virtual ~TileSet();

	void SetImageSource(Texture* texture);
	Texture* GetImageSource() const;

	void SetTileSize(const SizeI& tileSize) { m_tileSize = tileSize; }
	const SizeI& GetTileSize() const { return m_tileSize; }

	
	
	//const PathName& GetImageFilePath();
	//void SetImageFilePath(const PathName& filePath);

LN_PROTECTED_INTERNAL_ACCESS:
	void Initialize();
	virtual void LookupTileImage(int id, Texture** outTexture, Rect* outSrcRect);

private:
	struct TileInfo
	{
		int	priority;
	};
	
	Texture*	m_imageSource;
	SizeI		m_tileSize;
	int			m_tileCountH;
	//PathName	m_imageFilePath;
};

/**
	@brief	
*/
//class RGSS1TileSet
//	: public tr::ReflectionObject
//{
//	LN_TR_REFLECTION_TYPEINFO_DECLARE();
//public:
//	RGSS1TileSet();
//	virtual ~RGSS1TileSet();
//	void CreateCore();
//
//
//protected:
//	virtual void LookupTileImage(int id, Texture** outTexture, Rect* outSrcRect);
//
//private:
//};

LN_NAMESPACE_END
