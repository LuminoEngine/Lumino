
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
	SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRendererImpl();

public:

	/// 座標変換行列の設定
    void SetTransform( const Matrix& matrix );

	/// ビュー、プロジェクション行列の設定 (view はビルボードの計算で必要になるので、view proj で分ける)
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/// ビューサイズの設定
	void SetViewPixelSize(const Size& size);

	/// レンダリングスレートの設定
	void SetRenderState(const RenderState& state);

	/// ソート方法の設定
	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);

	/// 描画リクエスト
    void DrawRequest2D(
        const Vector3& position,
		const Vector3& center,
		const Vector2& size,
        Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable);

	/// 描画リクエスト
	void DrawRequest3D(
        const Vector3& position,
        const Vector3& center,
        const Vector2& size,
		Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,    // 4 頂点分。NULL の場合は白
		AxisDirection front);

	/// バッチ処理されているスプライトの描画
	void Flash();

	/// 描画せずにバッチをすべてクリアする
	void Clear();

private:

	/// 描画リクエスト
	void DrawRequest3DInternal(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,
		AxisDirection front,
		bool is3D);

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
	ArrayList<int>					m_spriteIndexList;			///< Flash() 内で使用する。m_spriteRequestList をソートするのは構造体コピーを伴うため速度面で心配。なのでインデックスをソートする。
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
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varViewPixelSize;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;

    } m_shader;

public:
	class SpriteRendererCommand : public RenderingCommand
	{
	protected:
		SpriteRendererImpl*	m_renderer;
		
		virtual ~SpriteRendererCommand()
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetTransformCommand : public SpriteRendererCommand
	{
		Matrix	m_transform;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Matrix& transform)
		{
			HandleCast<SetTransformCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetTransformCommand>(cmd)->m_transform = transform;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetTransform(m_transform);
		}
	};

	class SetViewProjMatrixCommand : public SpriteRendererCommand
	{
		Matrix	m_view;
		Matrix	m_proj;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Matrix& view, const Matrix& proj)
		{
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_view = view;
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_proj = proj;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewProjMatrix(m_view, m_proj);
		}
	};

	class SetViewPixelSizeCommand : public SpriteRendererCommand
	{
		Size	m_size;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Size& size)
		{
			HandleCast<SetViewPixelSizeCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewPixelSizeCommand>(cmd)->m_size = size;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewPixelSize(m_size);
		}
	};

	class SetRenderStateCommand : public SpriteRendererCommand
	{
		RenderState	m_state;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const RenderState& state)
		{
			HandleCast<SetRenderStateCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetRenderStateCommand>(cmd)->m_state = state;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetRenderState(m_state);
		}
	};

	class SetSortModeCommand : public SpriteRendererCommand
	{
		uint32_t				m_flags;
		SortingDistanceBasis	m_basis;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, uint32_t flags, SortingDistanceBasis basis)
		{
			HandleCast<SetSortModeCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetSortModeCommand>(cmd)->m_flags = flags;
			HandleCast<SetSortModeCommand>(cmd)->m_basis = basis;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetSortMode(m_flags, m_basis);
		}
	};

	class DrawRequest2DCommand : public SpriteRendererCommand
	{
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];
	public:
		static void Create(
			CmdInfo& cmd,
			SpriteRendererImpl* renderer, 
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable)
		{
			HandleCast<DrawRequest2DCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawRequest2DCommand>(cmd)->m_position = position;
			HandleCast<DrawRequest2DCommand>(cmd)->m_center = center;
			HandleCast<DrawRequest2DCommand>(cmd)->m_size = size;
			HandleCast<DrawRequest2DCommand>(cmd)->m_texture = texture;
			HandleCast<DrawRequest2DCommand>(cmd)->m_srcRect = srcRect;
			memcpy(HandleCast<DrawRequest2DCommand>(cmd)->m_colorTable, colorTable, sizeof(ColorF));
			LN_SAFE_ADDREF(renderer);
			LN_SAFE_ADDREF(texture);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawRequest2D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable);
		}
		virtual ~DrawRequest2DCommand()
		{
			LN_SAFE_RELEASE(m_texture);
		}
	};

	class DrawRequest3DCommand : public SpriteRendererCommand
	{
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];
		AxisDirection m_front;
	public:
		static void Create(
			CmdInfo& cmd,
			SpriteRendererImpl* renderer,
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable,
			AxisDirection front)
		{
			HandleCast<DrawRequest3DCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawRequest3DCommand>(cmd)->m_position = position;
			HandleCast<DrawRequest3DCommand>(cmd)->m_center = center;
			HandleCast<DrawRequest3DCommand>(cmd)->m_size = size;
			HandleCast<DrawRequest3DCommand>(cmd)->m_texture = texture;
			HandleCast<DrawRequest3DCommand>(cmd)->m_srcRect = srcRect;
			memcpy(HandleCast<DrawRequest3DCommand>(cmd)->m_colorTable, colorTable, sizeof(ColorF));
			HandleCast<DrawRequest3DCommand>(cmd)->m_front = front;
			LN_SAFE_ADDREF(renderer);
			LN_SAFE_ADDREF(texture);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawRequest3D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable, m_front);
		}
		virtual ~DrawRequest3DCommand()
		{
			LN_SAFE_RELEASE(m_texture);
		}
	};

	class FlashCommand : public SpriteRendererCommand
	{
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer)
		{
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->Flash();
		}
	};

	class ClearCommand : public SpriteRendererCommand
	{
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer)
		{
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->Clear();
		}
	};
};

} // namespace Graphics
} // namespace Lumino
