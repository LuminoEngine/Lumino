
#pragma once
#if 0
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Tilemap/TileMapRenderer.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class TileMapRenderer;

/**
	@brief	
*/
class TileMap
	: public VisualComponent
{
	LN_OBJECT();
public:

	/**
		@brief		
	*/
	static TileMapPtr create();

	/**
		@brief
	*/
	static TileMapPtr Create3D();

public:
	void SetTileMap(TileMapModel* tileMap);

protected:
	TileMap();
	virtual ~TileMap();
	void Create3DCore(SceneGraph* owner);
	virtual void OnRender2(DrawList* renderer) override;
	//virtual void drawSubset(SceneGraphRenderingContext* dc, int subsetIndex);

private:
	TileMapRenderer*	m_renderer;
	TileMapModel*		m_tileMap;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END


#if 0
//==============================================================================
// Tilemap 
//------------------------------------------------------------------------------
///**
//  @file       Tilemap.h
//  @brief      Tilemap
//*/
//==============================================================================

#pragma once

#include "../../Base/Table.h"
#include "VisualNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Tilemap
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Tilemap
    : public VisualComponent
{
public:
	static const int DefaultTilePixelSize = 32;

public:
    Tilemap( SceneGraph* scene );
    virtual ~Tilemap();

public:

	/// 作成
    void create(/* lnU32 width, lnU32 height, LNTilemapDirection frontDir */);

	/// 表示位置の設定
	//virtual void setPosition( int x_, int y_ );

	/// タイルセットテクスチャの設定
    void setTilesetTexture( Graphics::Texture* texture );

	/// タイルセットテクスチャの横方向のタイル数の設定
	void setTilesetHorizontalTileCount(size_t count) { mTilesetHorizontalTileCount = count; }

	/// タイルひとつ分のピクセルサイズを設定する
	void setTileSize(size_t width, size_t height);



    //virtual void setMapData( int* mapdata ) { mMapData = mapdata; }

	/// マップデータの設定
	void setMapData(Base::IntTable* mapData) { LN_REFOBJ_SET(mMapData, mapData); }


public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }
    virtual void update( float elapsedTime );

private:
	Base::IntTable*		mMapData;
	LNTilemapDirection	mDirection;
    Graphics::Texture*	mTilesetTexture;
	size_t				mTilesetHorizontalTileCount;
	lnFloat     mTileWidth;     ///< タイルひとつ分の幅
	lnFloat     mTileHeight;    ///< タイルひとつ分の高さ

    //lnU32         mXSize;         ///< X 方向のタイルの数
    //lnU32         mYSize;         ///< Y 方向のタイルの数
    //lnU32         mFaceNum;       ///< 面数


    //lnFloat     mUVWidth;       ///< マップチップひとつ分のピクセルサイズをテクスチャ座標系に変換したときの幅
    //lnFloat     mUVHeight;      ///< マップチップひとつ分のピクセルサイズをテクスチャ座標系に変換したときの幅

    ////int*        mMapData;

    //lnU32         mSrcXNum;       ///< タイルセット画像の横方向のチップ数 (RGSS の場合は 8)
    
	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
#endif
#endif
