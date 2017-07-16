
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class Texture;
class Material;
class TileSet;
typedef Ref<TileSet> TileSetPtr;

/**
	@brief	
*/
class TileSet
	: public tr::ReflectionObject
{
	LN_OBJECT();
public:
	static TileSetPtr create();

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
	void initialize(detail::GraphicsManager* manager);
	virtual void LookupTileImage(int id, Texture** outTexture, RectI* outSrcRect);
	Material* getMaterial() const;

private:
	struct TileInfo
	{
		int	priority;
	};
	
	Texture*	m_imageSource;
	Ref<Material>	m_material;
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
//	LN_OBJECT();
//public:
//	RGSS1TileSet();
//	virtual ~RGSS1TileSet();
//	void createCore();
//
//
//protected:
//	virtual void LookupTileImage(int id, Texture** outTexture, Rect* outSrcRect);
//
//private:
//};

LN_NAMESPACE_END
