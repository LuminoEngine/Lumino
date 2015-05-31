
#pragma once

#include "Texture.h"

namespace Lumino
{
namespace Graphics
{

enum BrushType
{
	BrushType_Unknown = 0,
	BrushType_Texture,
};

enum BrushWrapMode
{
	BrushWrapMode_Stretch = 0,	///< イメージ全体を引き延ばす
	BrushWrapMode_Tile,			///< イメージを並べて表示する

};

class Brush
	: public CoreObject
{
public:
	Brush();
	virtual ~Brush();

public:
	virtual BrushType GetType() const = 0;
};

/// Bitmap はソフト的な描画処理を行うため、BitmapBrush という名前はつかわない
class TextureBrush
	: public Brush
{
public:
	TextureBrush();
	virtual ~TextureBrush();

public:
	void Create(const TCHAR* filePath, GraphicsManager* manager);
	void SetTexture(Texture* texture) { m_texture = texture; }
	Texture* GetTexture() { return m_texture; }

	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	const Rect& GetSourceRect() const { return m_srcRect; }

	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }


	virtual BrushType GetType() const { return BrushType_Texture; }

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< 初期値は (0, 0, INT_MAX, INT_MAX) で、全体を転送することを表す
	BrushWrapMode		m_wrapMode;
};

/**
	@brief		
*/
class Painter
	: public RefObject
{
public:
	Painter(GraphicsManager* manager);
	virtual ~Painter();

public:
	void Begin();
	void End();

	void SetProjection(const Size& viewSize, float nearZ, float farZ);

	void SetBrush(Brush* brush);
	void DrawRectangle(const RectF& rect);
	void DrawFrameRectangle(const RectF& rect, float frameWidth);

private:
	GraphicsManager*	m_manager;
	RefPtr<Brush>		m_currentBrush;
};

} // namespace Graphics
} // namespace Lumino
