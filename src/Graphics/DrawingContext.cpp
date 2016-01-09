/*
	[2015/1/3] 面張りについて
		とりあえずねじれパスは想定しない。これは軽量な組み込みモードとする。
		本当にちゃんとしたのが使いたい要望あれば、ExpandFill だけを poly2tri で置き換えるモードを作る。

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
#include "RendererImpl.h"

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
	Vector2	UVOffset;		///< テクスチャUV (転送元のUV情報)
	Vector4	UVTileUnit;		///< テクスチャUV (タイリング空間のどこにいるか)

							/// 頂点レイアウト
	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 1 },
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
	DrawPoint = 0,
	DrawLine,
	DrawTriangle,
	DrawRectangle,
};
struct DrawingCommands_DrawPoint
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};
struct DrawingCommands_DrawTriangle
{
	DrawingCommandType	type;
	Vector3				p1;
	Vector3				p2;
	Vector3				p3;
	ColorF				p1Color;
	ColorF				p2Color;
	ColorF				p3Color;
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

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void PrimitiveCache::DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
//{
//	DrawingBasicVertex v;
//	v.Position = from;
//	v.Color = fromColor;
//	m_vertexCache.Add(v);
//	v.Position = to;
//	v.Color = toColor;
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i + 0);
//	m_indexCache.Add(i + 1);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void PrimitiveCache::DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color)
//{
//	float lu = srcUVRect.GetLeft();
//	float tv = srcUVRect.GetTop();
//	float uvWidth = srcUVRect.Width;
//	float uvHeight = srcUVRect.Height;
//
//	DrawingBasicVertex v;
//	v.Color = color;
//	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 1);	// 左上
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 1);	// 右上
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 2);	// 左下
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 2);	// 右下
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i + 0);
//	m_indexCache.Add(i + 1);
//	m_indexCache.Add(i + 2);
//	m_indexCache.Add(i + 2);
//	m_indexCache.Add(i + 1);
//	m_indexCache.Add(i + 3);
//}

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

		Driver::ITexture* SelectTexutre(Driver::ITexture* defaultTexture)
		{
			if (Type == BrushType_Texture) {
				return TextureBrush.Texture;
			}
			else if (Type == BrushType_FrameTexture) {
				return FrameTextureBrush.Texture;
			}
			return defaultTexture;
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
		detail::DrawingClass	drawingClass;
		BrushData	Brush;
		PenData		pen;
		float		Opacity;
		ColorF		ForeColor;		///< 乗算する色。SolidColorBrush の時はその色になる。それと Opacity の乗算結果。
		ToneF		Tone;

		DrawingState()
			: drawingClass(detail::DrawingClass::PointList)
			, ForeColor(ColorF::White)
			, Opacity(1.0f)
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
			drawingClass = state.drawingClass;

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

		void UpdateCurrentForeColor()
		{
			if (Brush.Type == BrushType_SolidColor)
			{
				ForeColor.R = Brush.SolidColorBrush.Color[0];
				ForeColor.G = Brush.SolidColorBrush.Color[1];
				ForeColor.B = Brush.SolidColorBrush.Color[2];
				ForeColor.A = Opacity * Brush.SolidColorBrush.Color[3];
			}
			else
			{
				ForeColor.Set(1, 1, 1, Opacity);
			}
		}
	};

public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void SetState(const DrawingState& state);
	void DoCommandList(const void* commandBuffer, size_t size, detail::DrawingClass drawingClass);
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
		detail::DrawingClass m_dc;
		void Create(DrawingContextImpl* impl, const void* commandBuffer, size_t size, detail::DrawingClass dc)
		{
			m_impl = impl;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
			m_dc = dc;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size, m_dc); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* impl) { m_impl = impl; }
		void Execute() { m_impl->Flush(); }
	};

private:

	//enum class PathPointAttr
	//{
	//	MoveTo,
	//	LineTo,
	//};

	
	enum class PathType
	{
		Point,
		Line,		// Stroke のみ
		Rectangle,	// TODO: いらないかも
		Path,	// ExpandFill などを実行して面張りを行う
	};

	//struct PathPoint
	//{
	//	PathPointAttr	attr;
	//	Vector3			point;
	//	ColorF			color;
	//};

	struct BasePoint
	{
		Vector3			point;
		ColorF			color;
		BasePoint(const Vector3& p, const ColorF& c) : point(p), color(c) {}
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
	void AddBasePoint(const Vector3& point, const ColorF& color);
	//void AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color);
	void ClosePath();

	void ExpandFill();

	//void PutMoveTo(const Vector3& pt, const ColorF& color);
	//void PutLineTo(const Vector3& pt, const ColorF& color);

	void ExpandStroke(bool vertexOnly);
	void AddVertex(const Vector3& point, const ColorF& color);

	GraphicsManager*		m_manager;
	//PrimitiveCache			m_primitiveCache;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	Matrix					m_view;
	Matrix					m_proj;
	DrawingState			m_currentState;

	//CacheBuffer<PathPoint>	m_pathPoints;
	CacheBuffer<Path>		m_pathes;

	CacheBuffer<BasePoint>	m_basePoints;

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;	// 頂点バッファ本体をリサイズすることになると余計に時間がかかるので、まずはここに作ってからコピーする
	CacheBuffer<uint16_t>			m_indexCache;

	// DrawingContext3D
	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varPixelStep;
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
	//m_pathPoints.Reserve(1024);
	m_pathes.Reserve(1024);
	m_basePoints.Reserve(1024);
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
	m_shader3D.varPixelStep = m_shader3D.shader->GetVariableByName(_T("g_pixelStep"));
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
	m_currentState.UpdateCurrentForeColor();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size, detail::DrawingClass drawingClass)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	m_currentState.drawingClass = drawingClass;
	//detail::DrawingClass c = *((const detail::DrawingClass*)pos);
	//pos += sizeof(detail::DrawingClass);

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);
		switch (type)
		{
			////////////////////////////////////////////////////////////
			case DrawingCommandType::DrawPoint:
			{
				// Point は連続する DrawingCommandType::DrawPoint を全て1つの Path にまとめる
				AddPath(PathType::Point);
				do
				{
					auto* cmd = (const DrawingCommands_DrawPoint*)pos;
					AddBasePoint(cmd->point, cmd->color);
					pos += sizeof(DrawingCommands_DrawLine);

				} while (pos < end && *((const DrawingCommandType*)pos) == DrawingCommandType::DrawPoint);

				ClosePath();
				break;
			}
			////////////////////////////////////////////////////////////
			case DrawingCommandType::DrawLine:
			{
				auto* cmd = (const DrawingCommands_DrawLine*)pos;

				AddPath(PathType::Line);
				AddBasePoint(cmd->from, cmd->fromColor);
				AddBasePoint(cmd->to, cmd->toColor);
				ClosePath();

				pos += sizeof(DrawingCommands_DrawLine);
				break;
			}
			////////////////////////////////////////////////////////////
			case DrawingCommandType::DrawTriangle:
			{
				auto* cmd = (const DrawingCommands_DrawTriangle*)pos;
				AddPath(PathType::Path);
				AddBasePoint(cmd->p1, cmd->p1Color);
				AddBasePoint(cmd->p2, cmd->p2Color);
				AddBasePoint(cmd->p3, cmd->p3Color);
				ClosePath();
				pos += sizeof(DrawingCommands_DrawTriangle);
				break;
			}
			////////////////////////////////////////////////////////////
			case DrawingCommandType::DrawRectangle:
			{
				auto* cmd = (const DrawingCommands_DrawRectangle*)pos;
				const RectF& rect = cmd->rect;

				AddPath(PathType::Rectangle);
				AddBasePoint(Vector3(rect.GetLeft(), rect.GetTop(), 0), cmd->color);
				AddBasePoint(Vector3(rect.GetRight(), rect.GetTop(), 0), cmd->color);
				AddBasePoint(Vector3(rect.GetRight(), rect.GetBottom(), 0), cmd->color);
				AddBasePoint(Vector3(rect.GetLeft(), rect.GetBottom(), 0), cmd->color);
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
	if (m_shader3D.varPixelStep != nullptr)
	{
		const Size& size = m_manager->GetRenderer()->GetRenderTarget(0)->GetSize();
		m_shader3D.varPixelStep->SetVector(Vector4(0.5f / size.Width, 0.5f / size.Height, 0, 0));
	}

	if (m_currentState.drawingClass == detail::DrawingClass::PointList)
	{
		ExpandStroke(true);	// 頂点バッファだけ作る
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		renderer->SetVertexBuffer(m_vertexBuffer);
		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitive(PrimitiveType_PointList, 0, m_vertexCache.GetCount());
	}
	else if (1)
	{
		ExpandFill();

		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

		renderer->SetVertexBuffer(m_vertexBuffer);
		renderer->SetIndexBuffer(m_indexBuffer);
		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
	}

#if 0
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

	}
#endif
	m_pathes.Clear();
	//m_pathPoints.Clear();
	m_basePoints.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPath(PathType type)
{
	Path path;
	path.type = type;
	path.firstIndex = m_basePoints.GetCount();//m_pathPoints.GetCount();
	path.pointCount = 0;
	path.closed = false;
	m_pathes.Add(path);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void DrawingContextImpl::AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color)
//{
//	PathPoint pt;
//	pt.attr = attr;
//	pt.point = point;
//	pt.color = color;
//	m_pathPoints.Add(pt);
//	GetCurrentPath()->pointCount++;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddBasePoint(const Vector3& point, const ColorF& color)
{
	BasePoint pt(point, color);
	pt.color.R *= m_currentState.ForeColor.R;
	pt.color.G *= m_currentState.ForeColor.G;
	pt.color.B *= m_currentState.ForeColor.B;
	pt.color.A *= m_currentState.ForeColor.A;
	m_basePoints.Add(pt);
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
void DrawingContextImpl::ExpandFill()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		int count = path.pointCount;
		if (count < 3) continue;

		// 境界の矩形を求めておく
		Vector2 posMin(FLT_MAX, FLT_MAX);
		Vector2 posMax(FLT_MIN, FLT_MIN);
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			posMin.X = std::min(posMin.X, pt.point.X);
			posMin.Y = std::min(posMin.Y, pt.point.Y);
			posMax.X = std::max(posMax.X, pt.point.X);
			posMax.Y = std::max(posMax.Y, pt.point.Y);
		}
		Vector2 uvSpan = posMax - posMin;
		if (uvSpan.X == 0.0f || uvSpan.Y == 0.0) continue;	// 潰れたパス。描く必要はない
		uvSpan = 1.0f / uvSpan;

		// 頂点バッファを作る
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex+i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			v.UVOffset.X = (pt.point.X - posMin.X) * uvSpan.X;
			v.UVOffset.Y = (pt.point.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);
		}

		// インデックスバッファを作る (時計回り)
		int ib = path.firstIndex;
		int i0 = 0;
		int i1 = 1;
		int i2 = count - 1;
		for (int iPt = 0; iPt < count - 2; ++iPt)
		{
			m_indexCache.Add(ib + i0);
			m_indexCache.Add(ib + i1);
			m_indexCache.Add(ib + i2);

			if (iPt & 1) {	// 奇数回
				i0 = i2;
				--i2;
			}
			else {	// 偶数回
				i0 = i1;
				++i1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ExpandStroke(bool vertexOnly)
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);
		int count = path.pointCount;

		// 境界の矩形を求めておく
		Vector2 posMin(FLT_MAX, FLT_MAX);
		Vector2 posMax(FLT_MIN, FLT_MIN);
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			posMin.X = std::min(posMin.X, pt.point.X);
			posMin.Y = std::min(posMin.Y, pt.point.Y);
			posMax.X = std::max(posMax.X, pt.point.X);
			posMax.Y = std::max(posMax.Y, pt.point.Y);
		}
		Vector2 uvSpan = posMax - posMin;
		if (uvSpan.X == 0.0f || uvSpan.Y == 0.0) {
			// パスがつぶれている。が、1点だけ描くとかあり得る。その場合uvは(0,0) とする。
		}
		else {
			uvSpan = 1.0f / uvSpan;
		}

		// 頂点バッファを作る
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			v.UVOffset.X = (pt.point.X - posMin.X) * uvSpan.X;
			v.UVOffset.Y = (pt.point.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);
		}
	}

	//for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	//{
	//	const Path& path = m_pathes.GetAt(iPath);

	//	switch (path.type)
	//	{
	//	case PathType::Line:
	//		for (int iPt = path.firstIndex; iPt < path.firstIndex+path.pointCount; ++iPt)
	//		{
	//			const BasePoint& pt = m_basePoints.GetAt(iPt);

	//			DrawingBasicVertex v;
	//			v.Position = pt.point;
	//			v.Color = pt.color;
	//			m_vertexCache.Add(v);

	//			uint16_t i = m_indexCache.GetCount();
	//			m_indexCache.Add(i);
	//		}
	//		break;
	//	case PathType::Rectangle:
	//	{
	//		const BasePoint* pts = &m_basePoints.GetAt(path.firstIndex);

	//		DrawingBasicVertex v;
	//		v.Position = pts[0].point; v.Color = pts[0].color; m_vertexCache.Add(v);
	//		v.Position = pts[1].point; v.Color = pts[1].color; m_vertexCache.Add(v);
	//		v.Position = pts[2].point; v.Color = pts[2].color; m_vertexCache.Add(v);
	//		v.Position = pts[3].point; v.Color = pts[3].color; m_vertexCache.Add(v);

	//		uint16_t i = m_indexCache.GetCount();
	//		m_indexCache.Add(i + 0);
	//		m_indexCache.Add(i + 1);
	//		m_indexCache.Add(i + 1);
	//		m_indexCache.Add(i + 3);
	//		m_indexCache.Add(i + 3);
	//		m_indexCache.Add(i + 2);
	//		m_indexCache.Add(i + 2);
	//		m_indexCache.Add(i + 0);
	//		break;
	//	}

	//	case PathType::Path:
	//		LN_THROW(0, NotImplementedException);
	//		break;
	//	}
	//}
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
	, m_flushRequested(false)
	, m_internalTextureBrush(nullptr)
{
	m_internalTextureBrush = LN_NEW TextureBrush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_internalTextureBrush);
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
void DrawingContext::SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize)
{
	m_uvParPixel.X = 1.0f / viewPixelSize.Width;
	m_uvParPixel.Y = 1.0f / viewPixelSize.Height;
	m_invViewProj = Matrix::Inverse(view * proj);
	m_viewOnePixelOffset = Vector2::TransformCoord(m_uvParPixel, m_invViewProj);
	LN_CALL_COMMAND(SetViewProjection, DrawingContextImpl::SetViewProjectionCommand, view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	if (m_currentState.transform != matrix)
	{
		FlushInternal();
		m_currentState.transform = matrix;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	if (m_currentState.brush != brush)
	{
		FlushInternal();
		m_currentState.brush = brush;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetPen(Pen* pen)
{
	if (m_currentState.pen != pen)
	{
		FlushInternal();
		m_currentState.pen = pen;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	// TODO: opacity はFlushいらないと思う。直接頂点色に乗算しておいた方が高速？
	if (m_currentState.opacity != opacity)
	{
		FlushInternal();
		m_currentState.opacity = opacity;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTone(const ToneF& tone)
{
	if (m_currentState.tone != tone)
	{
		FlushInternal();
		m_currentState.tone = tone;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	if (m_currentState.font != font)
	{
		FlushInternal();
		m_currentState.font = font;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PointList);
	DrawingCommands_DrawPoint cmd;
	cmd.type = DrawingCommandType::DrawPoint;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	SetDrawingClassInternal(detail::DrawingClass::LineList);
	//CheckFlush();

	DrawingCommands_DrawLine cmd;
	cmd.type = DrawingCommandType::DrawLine;
	cmd.from = from;
	cmd.to = to;
	cmd.fromColor = fromColor;
	cmd.toColor = toColor;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
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
void DrawingContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	DrawingCommands_DrawTriangle cmd;
	cmd.type = DrawingCommandType::DrawTriangle;
	cmd.p1 = p1; cmd.p1Color = p1Color;
	cmd.p2 = p2; cmd.p2Color = p2Color;
	cmd.p3 = p3; cmd.p3Color = p3Color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	//CheckFlush();

	DrawingCommands_DrawRectangle cmd;
	cmd.type = DrawingCommandType::DrawRectangle;
	cmd.rect = rect;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	bool flish = false;
	if (texture != m_internalTextureBrush->GetTexture())
	{
		m_internalTextureBrush->SetTexture(texture);
		flish = true;
	}
	else if (m_currentState.brush != m_internalTextureBrush)
	{
		flish = true;
	}
	if (flish)
	{
		FlushInternal();
		m_currentState.brush = m_internalTextureBrush;
	}
	m_internalTextureBrush->SetSourceRect(srcRect);
	DrawRectangle(rect, color);
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
	if (m_flushRequested)
	{
		DrawingContextImpl::DrawingState state;
		state.Create(m_currentState);
		LN_CALL_COMMAND(SetState, DrawingContextImpl::SetStateCommand, state);

		LN_CALL_COMMAND(DoCommandList, DrawingContextImpl::DoCommandListCommand, m_commandsBuffer.GetConstData(), m_commandsUsingByte, m_currentDrawingClass);
		m_commandsUsingByte = 0;

		LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);

		m_flushRequested = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetDrawingClassInternal(detail::DrawingClass dc)
{
	if (m_currentDrawingClass != dc)
	{
		FlushInternal();
		m_currentDrawingClass = dc;
		// コマンドバッファの先頭に区分を入れておく
		//AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
	}
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContext::CheckFlush()
//{
//		FlushInternal();
//	}
//
//}




//=============================================================================
// GraphicsContext
/*		GraphicsContext は JavaFX のクラス。
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext* GraphicsContext::GetContext()
{
	return GraphicsManager::Instance->GetGraphicsContext();
}

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
void GraphicsContext::Set2DRenderingMode(float minZ, float maxZ)
{
	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	Matrix proj = Matrix::Perspective2DLH(size.Width, size.Height, minZ, maxZ);
	m_drawingContext.SetViewProjection(Matrix::Identity, proj, size);
	m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetBrush(Brush* brush)
{
	m_drawingContext.SetBrush(brush);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetOpacity(float opacity)
{
	m_drawingContext.SetOpacity(opacity);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawPoint(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawRectangle(rect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawTexture(rect, texture, srcRect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_drawingContext.Flush();
	m_spriteRenderer->Flush();
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
void GraphicsContext::TryChangeRenderingClass(RendererType dc)
{
	if (dc != m_currentRenderer)
	{
		Flush();
		m_currentRenderer = dc;
	}

}

LN_NAMESPACE_END
