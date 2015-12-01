
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class SpriteRendererImpl;

/**
	@brief		スプライトの描画を行うクラスです。
*/
class SpriteRenderer
	: public RefObject
{
public:

	/**
		@brief		SpriteRenderer のインスタンスを作成します。
		@param[in]	maxSpriteCount	: 一度のバッチ描画で描画できる最大スプライト数
		@param[in]	manager			: 作成に使用する GraphicsManager
	*/
	static SpriteRenderer* Create(int maxSpriteCount, GraphicsManager* manager);

public:

	/**
		@brief		座標変換行列を設定します。
		@param[in]	matrix		: 座標変換行列
		@details	次の DrawRequest2D または DrawRequest3D で描画要求されるスプライトに対して適用する座標変換行列です。
	*/
	void SetTransform(const Matrix& matrix);

	/**
		@brief		ビュー及びプロジェクション行列を設定します。
	*/
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/**
		@brief		描画先のビューサイズを設定します。
	*/
	void SetViewPixelSize(const Size& size);

	/**
		@brief		レンダリングステートを設定します。
		@details	次の DrawRequest2D または DrawRequest3D で描画要求されるスプライトに対して適用するレンダリングステートです。
	*/
	void SetRenderState(const RenderState& state);

	/**
		@brief		スプライトのソートに関する情報を設定します。
		@param[in]	flags		: 
		@param[in]	basis		: ソートの基準
		@details	この設定は次の Flash で使用します。
	*/
	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);
	
	/**
		@brief		スプライトの描画を要求します。
		@param[in]	position	: 
		@param[in]	center		: 
		@param[in]	size		: 
		@param[in]	texture		: 
		@param[in]	srcRect		: (ピクセル単位)
		@param[in]	colorTable	: 
	*/
	void DrawRequest2D(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Texture* texture,
		const RectF& srcRect,		// TODO: Rect のほうがピクセル単位であることがわかりやすい
		const ColorF* colorTable);

	/**
		@brief		スプライトの描画を要求します。
		@param[in]	position	: 
		@param[in]	center		: 
		@param[in]	size		: 
		@param[in]	texture		: 
		@param[in]	srcRect		: 
		@param[in]	color		: 
	*/
	void DrawRequest2D(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Texture* texture,
		const RectF& srcRect,
		const ColorF& color);

	/**
		@brief		スプライトの描画を要求します。
		@param[in]	position	: 
		@param[in]	center		: 
		@param[in]	size		: 
		@param[in]	texture		: 
		@param[in]	srcRect		: 
		@param[in]	colorTable	: 
		@param[in]	front		:
	*/
	void DrawRequest3D(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Texture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,
		AxisDirection front);

	/**
		@brief		要求されているスプライトを全て描画します。
	*/
	void Flush();

	GraphicsManager* GetManager() const { return m_manager; }

private:
	SpriteRenderer(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRenderer();
	GraphicsManager*	m_manager;
	SpriteRendererImpl*	m_internal;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
