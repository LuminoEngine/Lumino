
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/DeviceInterface.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

class SpriteRendererImpl
    : public RefObject
{
public:
	SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount, bool is3D);
	virtual ~SpriteRendererImpl();

public:

	/// 座標変換行列の設定
    void SetTransform( const Matrix& matrix );

	/// ビュー、プロジェクション行列の設定 (view はビルボードの計算で必要になるので、view proj で分ける)
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	void SetViewPixelSize(const Size& size);
	//void setViewProjMatrix( const LMatrix& view, const LMatrix& proj, const LVector2& viewPixelSize );
	
	/// ビューサイズの設定( 2D 用のプロジェクション行列を作って、setViewProjMatrix() に設定する)
	//void setViewSize(const LVector2& viewSize);

	void SetRenderState( const RenderState& state );

	/// ソート方法の設定(SpriteSortMode  enable_view_pos_depth_:カメラからの距離をソート用Z値にする場合 true)
    void SetSortMode( uint32_t flags, bool enable_view_pos_depth_ );

	/// 描画リクエスト
    void DrawRequest2D(
        const Vector3& position,
        const Vector3& center,
        Texture* texture,
        const RectF& srcRect,
        ColorF color );

	/// 描画リクエスト
	void DrawRequest3D(
        const Vector3& position,
        const Vector3& center,
        const Vector2& size,
        Texture* texture,
        const RectF& src_rect,
		const ColorF* colorTable,    // 4 頂点分。NULL の場合は白
		AxisDirection front);

	/// バッチ処理されているスプライトの描画
	void Flash();

	/// 描画せずにバッチをすべてクリアする
	void Clear();

private:

	/// バッチ処理用スプライト頂点
	struct BatchSpriteVertex
	{
		Vector3		Position;   ///< 座標
		ColorF		Color;      ///< 頂点カラー
		Vector2		TexUV;      ///< テクスチャ座標

		/// 頂点レイアウト
		static Graphics::VertexElement* Elements()
		{
			static Graphics::VertexElement elements[] =
			{
				{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_Color, 0 },
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	struct BatchSpriteData
	{
		BatchSpriteVertex	Vertices[4];  ///< 座標変換済み頂点 ([0].Position.z がソートの基準になる)
		Device::ITexture*	Texture;
		int					RenderStateIndex;
		int					Priority;       ///< 優先度 (大きい方が後から描画される =手前)
		float				Depth;          ///< ソートに使われる Z 値 (大きいほど遠い)
		bool				Visible;
		bool				Assigned;       ///< SpriteRenderer::getBatchSpriteData() で取得された場合に true になる

	};

    struct Attribute
    {
        int StartIndex;
        int PrimitiveNum;
		Device::ITexture*   Texture;
		int RenderStateIndex;
    };

	class SpriteCmpDepthBackToFront;		///< Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthFrontToBack;		///< Z 値の小さい方から大きい方へソートする比較
	class SpriteCmpTexDepthBackToFront;		///< Z 値の大きい方から小さい方へソートする比較 (テクスチャ優先)
	class SpriteCmpTexDepthFrontToBack;		///< Z 値の小さい方から大きい方へソートする比較 (テクスチャ優先)

private:
	typedef ArrayList<BatchSpriteData>	SpriteRequestList;
	typedef ArrayList<RenderState>		RenderStateList;
	typedef ArrayList<Attribute>		AttributeList;

	RefPtr<GraphicsManager>			m_manager;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
    int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	ArrayList<int>					m_spriteIndexList;		///< Flash() 内で使用する。m_spriteRequestList をソートするのは構造体コピーを伴うため速度面で心配。なのでインデックスをソートする。
	RenderStateList					m_renderStateList;
	int								m_currentRenderStateIndex;	///< 次の Draw でに適用される RenderState がある m_renderStateList 内のインデックス

	AttributeList					m_attributeList;

	Matrix							m_transformMatrix;

	Vector3							m_viewDirection;		///< ビルボードの計算に使用する
	Matrix							m_viewInverseMatrix;	///< ビルボードの計算に使用する
	Vector3							m_viewPosition;			///< カメラ位置を基準としたソートで使用する
	Matrix							m_viewProjMatrix;
	Vector2							m_viewPixelSize;

	uint32_t						m_spriteSortMode;		///< ソート方法 (LNSpriteSortMode)
	bool							m_enableViewPosDepth;	///< TODO:これは enum の方がわかりやすい気がする・・・

	bool							m_3DSystem;				///< true の場合、スプライトの中心は四角形の中心で、Y+ 方向が上になる




    //BatchSpriteData*                    mSpriteRequests;
    //int                                 mNextIndex;         ///< mSpriteRequests の空いているインデックスを示す
    //int                                 mLastSpriteNum;
    //Matrix                             mTransformMatrix;
    //Matrix                             mViewInverseMatrix;
    //Matrix                             mViewProjMatrix;
    //Vector3                            mViewPosition;
    //Vector3                            mViewDirection;

    uint16_t*                                mSpriteIndexArray;
	uint16_t*                                mSpriteIndexArraySource;
	ArrayList<Attribute>		mAttributeList;

	bool(*mComFunc)(const uint16_t& l_, const uint16_t& r_);
    bool                                mIs3DSystem;        ///< true の場合、スプライトの中心は四角形の中心で、Y+ 方向が上になる


	typedef ArrayList<RenderState>	RenderStateArray;
	RenderStateArray					mRenderStateArray;
	int									mCurrentRenderStateIndex;

    
	Vector2 mViewPixelSize;
   
	/// シェーダ関係の変数をまとめた構造体
    struct 
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varViewPixelSize;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;

    } m_shader;


    static BatchSpriteData* sSpriteRequests;
};

} // namespace Graphics
} // namespace Lumino
