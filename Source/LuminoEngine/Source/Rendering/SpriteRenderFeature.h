
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/ContextInterface.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

struct SpriteColorTable
{
	Color	colors[4];
};

class SpriteRendererImpl
	: public RefObject
{
public:
	SpriteRendererImpl();
	virtual ~SpriteRendererImpl();
	void initialize(GraphicsManager* manager, int maxSpriteCount);

public:
	void SetTransform(const Matrix& matrix);
	void setViewProjMatrix(const Matrix& view, const Matrix& proj);
	void SetViewPixelSize(const Size& size);

	void setRenderState(const RenderState& state);

	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);

	void flush(SpriteSortMode sortFlags);

	void clear();

	void DrawRequestInternal(
		const Vector3& position,
		const Vector2& size,
		const Vector2& anchorRatio,
		Driver::ITexture* texture,
		const Rect& srcRect,
		const SpriteColorTable& colorTable,
		SpriteBaseDirection baseDir,
		BillboardType billboardType);


private:

	/// バッチ処理用スプライト頂点
	struct BatchSpriteVertex
	{
		Vector3		Position;   ///< 座標
		Color		Color;      ///< 頂点カラー
		Vector2		TexUV;      ///< テクスチャ座標

								/// 頂点レイアウト
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	struct BatchSpriteData
	{
		BatchSpriteVertex	Vertices[4];  ///< 座標変換済み頂点 ([0].Position.z がソートの基準になる)
		Driver::ITexture*	Texture;
		int					RenderStateIndex;
		int					Priority;       ///< 優先度 (大きい方が後から描画される =手前)
		float				Depth;          ///< ソートに使われる Z 値 (大きいほど遠い)
		bool				Visible;
		bool				Assigned;       ///< SpriteRenderFeature::getBatchSpriteData() で取得された場合に true になる

	};

	struct Attribute
	{
		int StartIndex;
		int PrimitiveNum;
		Driver::ITexture*   Texture;
		int RenderStateIndex;
	};

	class SpriteCmpDepthBackToFront;		///< Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthFrontToBack;		///< Z 値の小さい方から大きい方へソートする比較
	class SpriteCmpTexDepthBackToFront;		///< Z 値の大きい方から小さい方へソートする比較 (テクスチャ優先)
	class SpriteCmpTexDepthFrontToBack;		///< Z 値の小さい方から大きい方へソートする比較 (テクスチャ優先)

private:
	typedef List<BatchSpriteData>	SpriteRequestList;
	typedef List<RenderState>		RenderStateList;
	typedef List<Attribute>		AttributeList;

	GraphicsManager*					m_manager;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
	RefPtr<Driver::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Driver::IIndexBuffer>	m_indexBuffer;
	int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	List<int>						m_spriteIndexList;			///< Flash() 内で使用する。m_spriteRequestList をソートするのは構造体コピーを伴うため速度面で心配。なのでインデックスをソートする。
	RenderStateList					m_renderStateList;
	int								m_currentRenderStateIndex;	///< 次の Draw でに適用される RenderState がある m_renderStateList 内のインデックス
	AttributeList					m_attributeList;

	Matrix							m_transformMatrix;
	Vector3							m_viewDirection;			///< ビルボードの計算に使用する
	Matrix							m_viewInverseMatrix;		///< ビルボードの計算に使用する
	Vector3							m_viewPosition;				///< カメラ位置を基準としたソートで使用する
	Matrix							m_viewProjMatrix;
	Vector2							m_viewPixelSize;

	SortingDistanceBasis			m_sortingBasis;				///< ソート基準

																/// シェーダ関係の変数をまとめた構造体
	struct
	{
		RefPtr<Driver::IShader>		Shader;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varViewPixelSize;
		Driver::IShaderVariable*    varTexture;
		Driver::IShaderTechnique*   techMainDraw;

	} m_shader;
};

/**
	@brief		スプライトの描画を行うクラスです。
*/
class SpriteRenderFeature
	: public RefObject
	, public detail::IRenderFeature
{
public:

	static SpriteRenderFeature* create(int maxSpriteCount, GraphicsManager* manager);

public:

	/**
		@brief		座標変換行列を設定します。
		@param[in]	matrix		: 座標変換行列
		@details	次の DrawRequest2D または DrawRequest3D で描画要求されるスプライトに対して適用する座標変換行列です。
	*/
	void SetTransform(const Matrix& matrix);

	void SetState(const RenderState& renderState);

	void SetViewInfo(const Size& size, const Matrix& view, const Matrix& proj);

	/**
		@brief		レンダリングステートを設定します。
		@details	次の DrawRequest2D または DrawRequest3D で描画要求されるスプライトに対して適用するレンダリングステートです。
	*/
	//void SetRenderState(const RenderState& state);

	/**
		@brief		スプライトのソートに関する情報を設定します。
		@param[in]	flags		: 
		@param[in]	basis		: ソートの基準
		@details	この設定は次の Flash で使用します。
	*/
	void SetSortMode(SpriteSortMode flags, SortingDistanceBasis basis);

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
		const Vector2& size,
		const Vector2& anchorRatio,
		Texture* texture,
		const Rect& srcRect,
		const Color& color,
		BillboardType billboardType);

	void DrawRequest(
		const Vector3& position,
		const Vector2& anchorRatio,
		const Vector2& size,
		Texture* texture,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType);

	virtual bool isStandaloneShader() const { return true; }
	virtual void flush() override;
	virtual void onActivated() override {}
	virtual void onDeactivated() override { flush(); }

	GraphicsManager* getManager() const { return m_manager; }

	static void MakeBoundingSphere(const Vector2& size, SpriteBaseDirection baseDir, detail::Sphere* sphere);

LN_INTERNAL_ACCESS:
	SpriteRenderFeature(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRenderFeature();
	GraphicsManager*	m_manager;
	SpriteRendererImpl*	m_internal;
	SpriteSortMode		m_spriteSortMode;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
