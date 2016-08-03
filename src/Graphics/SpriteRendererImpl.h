
#pragma once
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class SpriteRendererImpl
	: public GraphicsResourceObject
{
public:
	SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRendererImpl();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);
	void CreateInternal();

public:

	/// 座標変換行列の設定
    void SetTransform( const Matrix& matrix );

	/// ビュー、プロジェクション行列の設定 (view はビルボードの計算で必要になるので、view proj で分ける)
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/// ビューサイズの設定
	void SetViewPixelSize(const SizeI& size);

	/// レンダリングスレートの設定
	void SetRenderState(const RenderState& state);

	/// ソート方法の設定
	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);

	/// 描画リクエスト
    void DrawRequest2D(
        const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Driver::ITexture* texture,
		const RectF& srcRect,
		const Color* colorTable);

	/// 描画リクエスト
	void DrawRequest3D(
        const Vector3& position,
        const Vector3& center,
        const Vector2& size,
		Driver::ITexture* texture,
		const RectF& srcRect,
		const Color* colorTable,    // 4 頂点分。NULL の場合は白
		AxisDirection front);

	/// バッチ処理されているスプライトの描画
	void Flush();

	/// 描画せずにバッチをすべてクリアする
	void Clear();

	GraphicsManager* GetManager() const { return m_manager; }

private:

	/// 描画リクエスト
	void DrawRequest3DInternal(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Driver::ITexture* texture,
		const RectF& srcRect,
		const Color* colorTable,
		AxisDirection front,
		bool is3D);

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
		bool				Assigned;       ///< SpriteRenderer::getBatchSpriteData() で取得された場合に true になる

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
	typedef Array<BatchSpriteData>	SpriteRequestList;
	typedef Array<RenderState>		RenderStateList;
	typedef Array<Attribute>		AttributeList;

	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
	RefPtr<Driver::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Driver::IIndexBuffer>	m_indexBuffer;
    int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	Array<int>						m_spriteIndexList;			///< Flash() 内で使用する。m_spriteRequestList をソートするのは構造体コピーを伴うため速度面で心配。なのでインデックスをソートする。
	RenderStateList					m_renderStateList;
	int								m_currentRenderStateIndex;	///< 次の Draw でに適用される RenderState がある m_renderStateList 内のインデックス
	AttributeList					m_attributeList;

	Matrix							m_transformMatrix;
	Vector3							m_viewDirection;			///< ビルボードの計算に使用する
	Matrix							m_viewInverseMatrix;		///< ビルボードの計算に使用する
	Vector3							m_viewPosition;				///< カメラ位置を基準としたソートで使用する
	Matrix							m_viewProjMatrix;
	Vector2							m_viewPixelSize;

	uint32_t						m_spriteSortMode;			///< ソート方法 (SpriteSortMode)
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

public:

	struct SetTransformCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Matrix	m_transform;

		void Create(SpriteRendererImpl* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetTransform(m_transform); }
	};

	struct SetViewProjMatrixCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Matrix	m_view;
		Matrix	m_proj;

		void Create(SpriteRendererImpl* renderer, const Matrix& view, const Matrix& proj)
		{
			m_renderer = renderer;
			m_view = view;
			m_proj = proj;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetViewProjMatrix(m_view, m_proj); }
	};

	struct SetViewPixelSizeCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		SizeI	m_size;

		void Create(SpriteRendererImpl* renderer, const SizeI& size)
		{
			m_renderer = renderer;
			m_size = size;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetViewPixelSize(m_size); }
	};

	struct SetRenderStateCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		RenderState	m_state;

		void Create(SpriteRendererImpl* renderer, const RenderState& state)
		{
			m_renderer = renderer;
			m_state = state;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetRenderState(m_state); }
	};

	struct SetSortModeCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		uint32_t				m_flags;
		SortingDistanceBasis	m_basis;

		void Create(SpriteRendererImpl* renderer, uint32_t flags, SortingDistanceBasis basis)
		{
			m_renderer = renderer;
			m_flags = flags;
			m_basis = basis;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->SetSortMode(m_flags, m_basis);
		}
	};

	struct DrawRequest2DCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Driver::ITexture* m_texture;
		RectF m_srcRect;
		Color m_colorTable[4];

		void Create(
			SpriteRendererImpl* renderer, 
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Driver::ITexture* texture,
			const RectF& srcRect,
			const Color* colorTable)
		{
			m_renderer = renderer;
			m_position = position;
			m_center = center;
			m_size = size;
			m_texture = texture;
			m_srcRect = srcRect;
			memcpy(m_colorTable, colorTable, sizeof(Color) * 4);
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute()
		{
			m_renderer->DrawRequest2D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable);
		}
	};

	struct DrawRequest3DCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Driver::ITexture* m_texture;
		RectF m_srcRect;
		Color m_colorTable[4];
		AxisDirection m_front;

		void Create(
			SpriteRendererImpl* renderer,
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Driver::ITexture* texture,
			const RectF& srcRect,
			const Color* colorTable,
			AxisDirection front)
		{
			m_renderer = renderer;
			m_position = position;
			m_center = center;
			m_size = size;
			m_texture = texture;
			m_srcRect = srcRect;
			memcpy(m_colorTable, colorTable, sizeof(Color) * 4);
			m_front = front;
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute()
		{
			m_renderer->DrawRequest3D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable, m_front);
		}
	};

	struct FlushCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;

		void Create(SpriteRendererImpl* renderer)
		{
			m_renderer = renderer;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->Flush(); }
	};

	struct ClearCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;

		void Create(SpriteRendererImpl* renderer)
		{
			m_renderer = renderer;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->Clear(); }
	};
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
