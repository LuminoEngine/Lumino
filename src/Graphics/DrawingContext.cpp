/*
	[2015/12/3]
		Arc
		Polygon
		PolyLine
		Ellipse
		Pie
		Point
		Rect
		RoundRect
		Text
		Image
		Path

		・Path 以外は汎用的な面張りを行うよりも自前で頂点作ってしまったほうが高速。
		・ストロークとアンチエイリアスは面張りしなければならないので、外周の頂点は素早く参照できるようにする必要がある。

		アウトラインのポイントリスト作成
		(Path以外)頂点リスト作成
		(Path)面張り・頂点リスト作成
		Fill頂点バッファ作成
		描画
		Stroke頂点バッファ作成
		描画

	[2015/12/2] タイリング描画について
		デフォルトでは有効にしない。シェーダは分ける。
		タイル配置するブラシが指定された場合、描画モードを切り替える。
		常に有効にするにはあまりにも複雑すぎる。


	[2015/12/2] コマンドのキャッシュについて
		同一ステート間の Primitive 描画コマンドは一括で実行できる。
		まとめてコマンドリストに送るといいのだが、バッファは誰が持つの？
		→ ステートの切り替えは割りと頻繁だと思われる。
		むしろ new の数が増えてしまうかもしれない。
		memcpy でコマンドリストの一時バッファへコピーするほうが全体的に見てパフォーマンス良いかも。
		sizeof(Matrix) を1コマンド分の平均サイズとして 10000 子 memcpy するときは 30us くらいかかる。
		1000 個なら 3us。
		実際にはこんなに使わないだろうし、memcpy で問題ないはず。


	[2015/12/2] クラス名
		しばらく間空いて、やっぱり DrawingContext のほうがいいと思う。
		このライブラリはかなり低レベルなAPIも公開する。
		そうなってくると DirectX11 の RenderingContext の意味が強くなってくる。
		高レベルってことで差別化するなら DrawingContext。
		


	名前の元ネタはこのあたりから。
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	このモジュールは Scene と GUI で共有されるが、
	DrawingContext は Scene よりも GUI 寄り。
	ライブラリとしては Scene の方が意味が大きい。Draw より Render かな、と。


	Scene では、あらかじめ SceneNode のシェーダをユーザーシェーダとして
	設定したものを OnRender() に渡す。



	ユーザーシェーダに要求する頂点宣言は、
	・Pos0
	・TexUV0
	・Color0

	ユーザーシェーダに要求するパラメータは、
	・VIEWPORTPIXELSIZE
	・LNRC_TONE
		UI_TONE、NODE_TONE とかのほうがいいかも？
	↑Tone の計算はユーザーシェーダで色が決まった後に行わなければならない。
	（乗算だけで表現できないので頂点宣言だけでは不足）
	#include "lumino.fx" とかして、ピクセルシェーダで LNUI_PostPixelShader みたいな関数を
	ユーザーに呼んでもらう必要がある。

	描画方式は3つ。
	・プリミティブのみ
	・組み込みシェーダを使う
	・ユーザーシェーダを使う

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/

#include "Internal.h"
#include <algorithm>
#include "PainterEngine.h"	// CacheBuffer
#include "RenderingCommand.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>


LN_NAMESPACE_BEGIN


//class DrawingPrimitiveCommandList
//{
//public:
//
//protected:
//	DrawingContext();
//	virtual ~DrawingContext();
//	void Initialize(GraphicsManager* manager);
//
//private:
//	GraphicsManager*	m_manager;
//};

struct DrawingBasicVertex
{
public:

	Vector3	Position;			///< 位置
	Vector4	Color;				///< 頂点カラー
	Vector4	UVOffset;		///< テクスチャUV (転送元のUV情報)
	Vector2	UVTileUnit;		///< テクスチャUV (タイリング空間のどこにいるか)

							/// 頂点レイアウト
	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 1 },
		};
		return elements;
	}
	static const int ElementCount = 4;
};

//=============================================================================
// DrawingCommands
//=============================================================================
enum class DrawingCommandType : uint32_t
{
	DrawLine = 0,
	DrawRectangle,
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};
struct DrawingCommands_DrawRectangle
{
	DrawingCommandType	type;
	RectF				rect;
	ColorF				color;
};


//=============================================================================
// PrimitiveCache
//=============================================================================
class PrimitiveCache
{
public:
	PrimitiveCache();
	virtual ~PrimitiveCache();

	void Clear();
	void DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);
	void DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color);

	void ApplyBuffers(Driver::IVertexBuffer* vb, Driver::IIndexBuffer* ib);
	int GetIndexCount() const { return m_indexCache.GetCount(); }
private:

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveCache::PrimitiveCache()
{
	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveCache::~PrimitiveCache()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::Clear()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	DrawingBasicVertex v;
	v.Position = from;
	v.Color = fromColor;
	m_vertexCache.Add(v);
	v.Position = to;
	v.Color = toColor;
	m_vertexCache.Add(v);

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color)
{
	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float uvWidth = srcUVRect.Width;
	float uvHeight = srcUVRect.Height;

	DrawingBasicVertex v;
	v.Color = color;
	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 2);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 2);	// 右下
	m_vertexCache.Add(v);

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::ApplyBuffers(Driver::IVertexBuffer* vb, Driver::IIndexBuffer* ib)
{
	vb->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	ib->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
}



//=============================================================================
// DrawingContextImpl
//=============================================================================
class DrawingContextImpl
	: public RefObject
{
public:

	struct BrushData
	{
		BrushType	Type;

		union
		{
			struct
			{
				float				Color[4];	///< RGBA

			} SolidColorBrush;

			struct
			{
				Driver::ITexture*	Texture;
				int					SourceRect[4];	///< XYWH
				BrushWrapMode		WrapMode;

			} TextureBrush;

			struct
			{
				Driver::ITexture*	Texture;
				int					SourceRect[4];	///< XYWH
				int					InnerSourceRect[4];	///< XYWH
				BrushWrapMode		WrapMode;
				int					FrameThicness;

			} FrameTextureBrush;
		};

		void Set(ln::Brush* brush)
		{
			if (brush == nullptr)
			{
				Type = BrushType_Unknown;	// no set
			}
			else
			{
				Type = brush->GetType();
				if (Type == BrushType_SolidColor)
				{
					auto t = static_cast<ColorBrush*>(brush);
					const ColorF& c = t->GetColor();
					SolidColorBrush.Color[0] = c.R;		// TODO: POD 型をまとめて定義したほうがいい気がする
					SolidColorBrush.Color[1] = c.G;
					SolidColorBrush.Color[2] = c.B;
					SolidColorBrush.Color[3] = c.A;
				}
				else if (Type == BrushType_Texture)
				{
					auto t = static_cast<ln::TextureBrush*>(brush);
					TextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
					TextureBrush.SourceRect[1] = r.Y;
					TextureBrush.SourceRect[2] = r.Width;
					TextureBrush.SourceRect[3] = r.Height;
					TextureBrush.WrapMode = t->GetWrapMode();
				}
				else if (Type == BrushType_FrameTexture)
				{
					auto t = static_cast<ln::FrameTextureBrush*>(brush);
					FrameTextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					const Rect& r2 = t->GetInnerAreaSourceRect();
					FrameTextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
					FrameTextureBrush.SourceRect[1] = r.Y;
					FrameTextureBrush.SourceRect[2] = r.Width;
					FrameTextureBrush.SourceRect[3] = r.Height;
					FrameTextureBrush.InnerSourceRect[0] = r2.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
					FrameTextureBrush.InnerSourceRect[1] = r2.Y;
					FrameTextureBrush.InnerSourceRect[2] = r2.Width;
					FrameTextureBrush.InnerSourceRect[3] = r2.Height;
					FrameTextureBrush.WrapMode = t->GetWrapMode();
					FrameTextureBrush.FrameThicness = t->GetThickness();
				}
				else {
					LN_THROW(0, NotImplementedException);
				}
			}
		}

		void Copy(const BrushData& data)
		{
			memcpy(this, &data, sizeof(BrushData));
			if (Type == BrushType_SolidColor) {
				//ForeColor.R = Brush.SolidColorBrush.Color[0];
				//ForeColor.G = Brush.SolidColorBrush.Color[1];
				//ForeColor.B = Brush.SolidColorBrush.Color[2];
				//ForeColor.A = Brush.SolidColorBrush.Color[3];
			}
			else if (Type == BrushType_Texture) {
				LN_SAFE_ADDREF(TextureBrush.Texture);
			}
			else if (Type == BrushType_FrameTexture) {
				LN_SAFE_ADDREF(FrameTextureBrush.Texture);
			}
		}
	};

	struct PenData
	{
		BrushData	brush;
		float		thickness;

		void Set(ln::Pen* pen)
		{
			if (pen != nullptr)
			{
				brush.Set(pen->GetBrush());
				thickness = pen->GetThickness();
			}
			else
			{
				brush.Set(nullptr);
				thickness = 0.0f;
			}
		}
		void Copy(const PenData& data)
		{
			brush.Copy(data.brush);
			thickness = data.thickness;
		}
	};

	// このクラスは描画コマンドの引数となるクラス。RefPtr とかはメンバに置かないこと。
	struct DrawingState
	{
		BrushData	Brush;
		PenData		pen;
		float		Opacity;
		//ColorF		ForeColor;		///< 乗算する色。SolidColorBrush の時はその色になる。それと Opacity の乗算結果。
		ToneF		Tone;

		DrawingState()
		{
			pen.brush.Type = BrushType_Unknown;
		}

		// Painter 側で作り、PainterEngineState をコマンドリストに乗せるときに使う。
		void Create(const detail::DrawingState& state)
		{
			// ブラシデータ
			Brush.Set(state.brush);
			pen.Set(state.pen);

			// 不透明度
			Opacity = state.opacity;

			// 色調
			Tone = state.tone;
		}

		void Copy(const DrawingState& state)
		{
			ReleaseObjects();

			// ブラシデータ
			Brush.Copy(state.Brush);
			pen.Copy(state.pen);

			// 不透明度
			Opacity = state.Opacity;

			// 色調
			Tone = state.Tone;
		}

		void ReleaseObjects()
		{
			if (Brush.Type == BrushType_Texture) {
				LN_SAFE_RELEASE(Brush.TextureBrush.Texture);
			}
			else if (Brush.Type == BrushType_FrameTexture) {
				LN_SAFE_RELEASE(Brush.FrameTextureBrush.Texture);
			}
		}
	};

public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void SetState(const DrawingState& state);
	void DoCommandList(const void* commandBuffer, size_t size);
	void Flush();


public:
	struct SetViewProjectionCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		Matrix m_view;
		Matrix m_proj;

		void Create(DrawingContextImpl* impl, const Matrix& view, const Matrix& proj)
		{
			m_impl = impl;
			m_view = view;
			m_proj = proj;
		}
		void Execute() { m_impl->SetViewProjection(m_view, m_proj); }
	};

	struct SetStateCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DrawingState m_state;

		void Create(DrawingContextImpl* impl, const DrawingState& state)
		{
			m_impl = impl;
			m_state.Copy(state);

			if (m_state.Brush.Type == BrushType_Texture) {
				MarkGC(m_state.Brush.TextureBrush.Texture);
			}
			if (m_state.Brush.Type == BrushType_FrameTexture) {
				MarkGC(m_state.Brush.FrameTextureBrush.Texture);
			}
		}
		void Execute() { m_impl->SetState(m_state); }
	};

	struct DoCommandListCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DataHandle m_commandBuffer;
		size_t	m_size;
		void Create(DrawingContextImpl* impl, const void* commandBuffer, size_t size)
		{
			m_impl = impl;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* impl) { m_impl = impl; }
		void Execute() { m_impl->Flush(); }
	};

private:

	enum class PathPointAttr
	{
		MoveTo,
		LineTo,
	};

	enum class PathType
	{
		Line,
		Rectangle,
		Path,	// ExpandFill などを実行して面張りを行う
	};

	struct PathPoint
	{
		PathPointAttr	attr;
		Vector3			point;
		ColorF			color;
	};

	struct StrokePoint
	{
		Vector3			point;
		ColorF			color;
	};

	struct Path
	{
		PathType		type;
		int				firstIndex;		// このパスの開始点を示す m_pathPoints 上のインデックス
		int				pointCount;		// このパスを構成する m_pathPoints 上の点の数
		bool			closed;			// パスが閉じているか (false は線分)
	};

	void AddPath(PathType type);
	Path* GetCurrentPath();
	void AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color);
	void ClosePath();

	void PutMoveTo(const Vector3& pt, const ColorF& color);
	void PutLineTo(const Vector3& pt, const ColorF& color);

	void ExpandStroke();
	void AddVertex(const Vector3& point, const ColorF& color);

	GraphicsManager*		m_manager;
	//PrimitiveCache			m_primitiveCache;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	Matrix					m_view;
	Matrix					m_proj;
	DrawingState			m_currentState;

	CacheBuffer<PathPoint>	m_pathPoints;
	CacheBuffer<Path>		m_pathes;

	CacheBuffer<StrokePoint>		m_strokePoints;

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;	// 頂点バッファ本体をリサイズすることになると余計に時間がかかるので、まずはここに作ってからコピーする
	CacheBuffer<uint16_t>			m_indexCache;

	// DrawingContext3D
	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varTexture;
		Driver::IShaderTechnique*   techMainDraw;
		Driver::IShaderPass*        passP0;

	} m_shader3D;
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::DrawingContextImpl(GraphicsManager* manager)
	: m_manager(manager)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
	m_pathPoints.Reserve(1024);
	m_pathes.Reserve(1024);
	m_strokePoints.Reserve(1024);
	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);

	Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();

	const int DefaultFaceCount = 1024;

	m_vertexBuffer = device->CreateVertexBuffer(DrawingBasicVertex::Elements(), DrawingBasicVertex::ElementCount, DefaultFaceCount * 4, NULL, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

	//-----------------------------------------------------
	// シェーダ (DrawingContext3D)
	const unsigned char code[] =
	{
#include "Resource/DrawingContext3D.fx.h"
	};
	const size_t codeLen = LN_ARRAY_SIZE_OF(code);

	ShaderCompileResult result;
	m_shader3D.shader = device->CreateShader(code, codeLen, &result);
	if (result.Level != ShaderCompileResultLevel_Success) {
		printf(result.Message.c_str());	// TODO:仮
	}

	m_shader3D.varWorldMatrix = m_shader3D.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader3D.varViewProjMatrix = m_shader3D.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader3D.varTexture = m_shader3D.shader->GetVariableByName(_T("g_texture"));
	m_shader3D.techMainDraw = m_shader3D.shader->GetTechnique(0);
	m_shader3D.passP0 = m_shader3D.techMainDraw->GetPass(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::~DrawingContextImpl()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_shader3D.shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	m_view = view;
	m_proj = proj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::SetState(const DrawingState& state)
{
	m_currentState.Copy(state);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	detail::DrawingClass c = *((const detail::DrawingClass*)pos);
	pos += sizeof(detail::DrawingClass);

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);
		switch (type)
		{
			case DrawingCommandType::DrawLine:
			{
				const DrawingCommands_DrawLine* cmd = (const DrawingCommands_DrawLine*)pos;

				// パスを作る (Stroke を作るために使う)
				AddPath(PathType::Line);

				StrokePoint pt;
				pt.point = cmd->from; pt.color = cmd->fromColor;
				m_strokePoints.Add(pt);
				pt.point = cmd->to; pt.color = cmd->toColor;
				m_strokePoints.Add(pt);

				GetCurrentPath()->pointCount = 2;
				ClosePath();

				//PutMoveTo(cmd->from, cmd->fromColor);
				//PutLineTo(cmd->to, cmd->toColor);
				//ClosePath();

				//// 頂点バッファ
				//DrawingBasicVertex v;
				//v.Position = cmd->from; v.Color = cmd->fromColor;
				//m_vertexCache.Add(v);
				//v.Position = cmd->to; v.Color = cmd->toColor;
				//m_vertexCache.Add(v);
				//
				//// インデックスバッファ
				//uint16_t i = m_vertexCache.GetCount();
				//m_indexCache.Add(i + 0);
				//m_indexCache.Add(i + 1);

				pos += sizeof(DrawingCommands_DrawLine);
				break;
			}
			case DrawingCommandType::DrawRectangle:
			{
				const DrawingCommands_DrawRectangle* cmd = (const DrawingCommands_DrawRectangle*)pos;

				AddPath(PathType::Rectangle);

				const RectF& rect = cmd->rect;
				StrokePoint pt;
				pt.color = cmd->color;
				pt.point.Set(rect.GetLeft(), rect.GetTop(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetRight(), rect.GetTop(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetLeft(), rect.GetBottom(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetRight(), rect.GetBottom(), 0); m_strokePoints.Add(pt);

				GetCurrentPath()->pointCount = 4;
				ClosePath();

				pos += sizeof(DrawingCommands_DrawRectangle);
				break;
			}
			default:
				LN_THROW(0, InvalidOperationException);
				return;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::Flush()
{
	if (m_pathes.GetCount() == 0) { return; }


	Driver::IRenderer* renderer = m_manager->GetGraphicsDevice()->GetRenderer();
	m_shader3D.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader3D.varViewProjMatrix->SetMatrix(m_view * m_proj);

	
	if (m_currentState.pen.brush.Type != BrushType_Unknown && m_currentState.pen.thickness == 0.0f)
	{
		ExpandStroke();

		//m_primitiveCache.ApplyBuffers(m_vertexBuffer, m_indexBuffer);
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

		renderer->SetVertexBuffer(m_vertexBuffer);
		renderer->SetIndexBuffer(m_indexBuffer);
		m_shader3D.varTexture->SetTexture(m_manager->GetDummyTexture());
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitiveIndexed(PrimitiveType_LineList, 0, m_indexCache.GetCount() / 2);

		m_pathes.Clear();
		m_pathPoints.Clear();
		m_strokePoints.Clear();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPath(PathType type)
{
	Path path;
	path.type = type;
	path.firstIndex = m_pathPoints.GetCount();
	path.pointCount = 0;
	path.closed = false;
	m_pathes.Add(path);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color)
{
	PathPoint pt;
	pt.attr = attr;
	pt.point = point;
	pt.color = color;
	m_pathPoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ClosePath()
{
	GetCurrentPath()->closed = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::Path* DrawingContextImpl::GetCurrentPath()
{
	return &m_pathes.GetLast();
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::PutMoveTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::MoveTo, pt, color);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::PutLineTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::LineTo, pt, color);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ExpandStroke()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		switch (path.type)
		{
		case PathType::Line:
			for (int iPt = path.firstIndex; iPt < path.firstIndex+path.pointCount; ++iPt)
			{
				const StrokePoint& pt = m_strokePoints.GetAt(iPt);

				DrawingBasicVertex v;
				v.Position = pt.point;
				v.Color = pt.color;
				m_vertexCache.Add(v);

				uint16_t i = m_indexCache.GetCount();
				m_indexCache.Add(i);
			}
			break;
		case PathType::Rectangle:
		{
			const StrokePoint* pts = &m_strokePoints.GetAt(path.firstIndex);

			DrawingBasicVertex v;
			v.Position = pts[0].point; v.Color = pts[0].color; m_vertexCache.Add(v);
			v.Position = pts[1].point; v.Color = pts[1].color; m_vertexCache.Add(v);
			v.Position = pts[2].point; v.Color = pts[2].color; m_vertexCache.Add(v);
			v.Position = pts[3].point; v.Color = pts[3].color; m_vertexCache.Add(v);

			uint16_t i = m_indexCache.GetCount();
			m_indexCache.Add(i + 0);
			m_indexCache.Add(i + 1);
			m_indexCache.Add(i + 1);
			m_indexCache.Add(i + 3);
			m_indexCache.Add(i + 3);
			m_indexCache.Add(i + 2);
			m_indexCache.Add(i + 2);
			m_indexCache.Add(i + 0);
			break;
		}

		case PathType::Path:
			LN_THROW(0, NotImplementedException);
			break;
		}
	}
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::AddVertex(const Vector3& point, const ColorF& color)
//{
//	DrawingBasicVertex v;
//	v.Position = point;
//	v.Color = color;
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i);
//}


//=============================================================================
// DrawingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::DrawingContext()
	: m_manager(nullptr)
	, m_commandsUsingByte(0)
	, m_internal(nullptr)
	, m_currentDrawingClass(detail::DrawingClass::Unknown)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_internal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_internal = LN_NEW	DrawingContextImpl(m_manager);
	m_commandsBuffer.Resize(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	LN_CALL_COMMAND(SetViewProjection, DrawingContextImpl::SetViewProjectionCommand, view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	m_currentState.transform = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	m_currentState.brush = brush;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetPen(Pen* pen)
{
	m_currentState.pen = pen;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	m_currentState.opacity = opacity;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTone(const ToneF& tone)
{
	m_currentState.tone = tone;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	m_currentState.font = font;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	SetDrawingClassInternal(detail::DrawingClass::LineList);
	CheckFlush();

	DrawingCommands_DrawLine cmd;
	cmd.type = DrawingCommandType::DrawLine;
	cmd.from = from;
	cmd.to = to;
	cmd.fromColor = fromColor;
	cmd.toColor = toColor;
	AddCommand(&cmd, sizeof(cmd));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& color)
{
	DrawLine(from, to, color, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	CheckFlush();

	DrawingCommands_DrawRectangle cmd;
	cmd.type = DrawingCommandType::DrawRectangle;
	cmd.rect = rect;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Flush()
{
	FlushInternal();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::AddCommand(const void* command, size_t size)
{
	// バッファが足りなければ拡張する
	if (m_commandsUsingByte + size > m_commandsBuffer.GetSize()) {
		size_t newSize = m_commandsBuffer.GetSize() + std::max(m_commandsBuffer.GetSize(), size);	// 最低でも size 分を拡張する
		m_commandsBuffer.Resize(newSize, false);
	}

	// データコピー
	if (command != NULL)
	{
		byte_t* ptr = &(m_commandsBuffer.GetData()[m_commandsUsingByte]);
		size_t bufSize = m_commandsBuffer.GetSize() - m_commandsUsingByte;
		memcpy_s(ptr, bufSize, command, size);
	}

	// バッファ使用量を増やす
	m_commandsUsingByte += size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::FlushInternal()
{
	DrawingContextImpl::DrawingState state;
	state.Create(m_currentState);
	LN_CALL_COMMAND(SetState, DrawingContextImpl::SetStateCommand, state);

	LN_CALL_COMMAND(DoCommandList, DrawingContextImpl::DoCommandListCommand, m_commandsBuffer.GetConstData(), m_commandsUsingByte);
	m_commandsUsingByte = 0;

	LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetDrawingClassInternal(detail::DrawingClass dc)
{
	if (m_currentDrawingClass != dc) {
		m_currentDrawingClass = dc;
		m_stateModified = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::CheckFlush()
{
	if (m_stateModified) {
		FlushInternal();
		m_stateModified = false;
	}

	// コマンドバッファの先頭に区分を入れておく
	AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
}




//=============================================================================
// GraphicsContext
/*		GraphicsContext は JavaFX のクラス。
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::GraphicsContext(GraphicsManager* manager)
	: m_currentRenderer(RendererType::None)
	, m_spriteRenderer(nullptr)
{
	Renderer = manager->GetRenderer();
	GeometryRenderer = GeometryRenderer::Create(manager);
	m_drawingContext.Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(GeometryRenderer);
	LN_SAFE_RELEASE(m_spriteRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext* GraphicsContext::BeginDrawingContext()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::DrawingContext;
	}
	return &m_drawingContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer* GraphicsContext::BeginSpriteRendering()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::SpriteRenderer;
	}
	return m_spriteRenderer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_drawingContext.Flush();
	m_spriteRenderer->Flush();
}


LN_NAMESPACE_END
