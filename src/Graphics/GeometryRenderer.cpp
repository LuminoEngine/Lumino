/*
	動的頂点バッファのNoOverwriteとDiscard
	http://blogs.msdn.com/b/ito/archive/2011/05/22/no-overwrite-or-discard.aspx
*/
#include "Internal.h"
#include <math.h>
#include <float.h>
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"
#include "PainterEngine.h"	// for CacheBuffer
#include "GraphicsManager.h"
#include "RendererImpl.h"
#include <Lumino/Graphics/Brush.h>
#include "GeometryRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

struct DrawingBasicVertex
{
public:

	Vector3	Position;			///< 位置
	Vector4	Color;				///< 頂点カラー
	Vector4	UVOffset;		///< テクスチャUV (転送元のUV情報)		// TODO: UVSrcRect とかの名前のほうがいい
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



struct VertexExpandResult
{
	Vector2 posMin = Vector2(FLT_MAX, FLT_MAX);
	Vector2 posMax = Vector2(FLT_MIN, FLT_MIN);
};

enum class GeometryType
{
	Rectangle,
};

struct GeometryData
{
	GeometryType	type;

	union
	{
		// Rectangle
		struct
		{
			float	x, y, w, h;

		} rectangle;
	};

	void ExpandFill_Rectangle(CacheBuffer<DrawingBasicVertex>* vb, CacheBuffer<uint16_t>* ib, const ColorF& color, VertexExpandResult* outResult);
	void ExpandUV_Stretch(DrawingBasicVertex* begin, DrawingBasicVertex* end, const RectF& srcPixelRect, const RectF& srcUVRect, const Vector2& posMin, const Vector2& posMax);
	void ExpandUV_Tilling(DrawingBasicVertex* begin, DrawingBasicVertex* end, const RectF& srcPixelRect, const RectF& srcUVRect, const Vector2& posMin, const Vector2& posMax);
};


//------------------------------------------------------------------------------
void GeometryData::ExpandFill_Rectangle(CacheBuffer<DrawingBasicVertex>* vb, CacheBuffer<uint16_t>* ib, const ColorF& color, VertexExpandResult* outResult)
{
	RectF* rect = (RectF*)&rectangle;

	uint16_t i = vb->GetCount();
	ib->Add(i + 0);
	ib->Add(i + 1);
	ib->Add(i + 2);
	ib->Add(i + 2);
	ib->Add(i + 1);
	ib->Add(i + 3);

	DrawingBasicVertex v;
	v.Color = color;
	v.Position.Set(rect->GetLeft(), rect->GetTop(), 0);		// 左上
	vb->Add(v);
	v.Position.Set(rect->GetLeft(), rect->GetBottom(), 0);	// 左下
	vb->Add(v);
	v.Position.Set(rect->GetRight(), rect->GetTop(), 0);	// 右上
	vb->Add(v);
	v.Position.Set(rect->GetRight(), rect->GetBottom(), 0);	// 右下
	vb->Add(v);

	outResult->posMin.x = rect->GetLeft();
	outResult->posMin.y = rect->GetTop();
	outResult->posMax.x = rect->GetRight();
	outResult->posMax.y = rect->GetBottom();
}

//------------------------------------------------------------------------------
void GeometryData::ExpandUV_Stretch(DrawingBasicVertex* begin, DrawingBasicVertex* end, const RectF& srcPixelRect, const RectF& srcUVRect, const Vector2& posMin, const Vector2& posMax)
{
	Vector2 range = posMax - posMin;

	for (DrawingBasicVertex* v = begin; v < end; ++v)
	{
		v->UVOffset.x = srcUVRect.x;
		v->UVOffset.y = srcUVRect.y;
		v->UVOffset.z = srcUVRect.width;
		v->UVOffset.w = srcUVRect.height;

		// Geometry の境界ボックス内のどこにある点かを 0.0 ～ 1.0 であらわす
		Vector2 geoLocal((v->Position.x - posMin.x) / range.x, (v->Position.y - posMin.y) / range.y);

		v->UVTileUnit.x = 1.0f + geoLocal.x;	// 0 だとシェーダ内で0除算するので +1
		v->UVTileUnit.y = 1.0f + geoLocal.y;
	}
}

//------------------------------------------------------------------------------
void GeometryData::ExpandUV_Tilling(DrawingBasicVertex* begin, DrawingBasicVertex* end, const RectF& srcPixelRect, const RectF& srcUVRect, const Vector2& posMin, const Vector2& posMax)
{
	Vector2 range = posMax - posMin;

	float blockCountW = (srcPixelRect.width != 0) ? (range.x / srcPixelRect.width) : 0;	// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	float blockCountH = (srcPixelRect.height != 0) ? (range.y / srcPixelRect.height) : 0;	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	// ↑TODO: スクリーン1 に対して srcRect 1px に対応付けているので、3D空間に描くときは調整が必要

	for (DrawingBasicVertex* v = begin; v < end; ++v)
	{
		v->UVOffset.x = srcUVRect.x;
		v->UVOffset.y = srcUVRect.y;
		v->UVOffset.z = srcUVRect.width;
		v->UVOffset.w = srcUVRect.height;

		// Geometry の境界ボックス内のどこにある点かを 0.0 ～ 1.0 であらわす
		Vector2 geoLocal((v->Position.x - posMin.x) / range.x, (v->Position.y - posMin.y) / range.y);

		v->UVTileUnit.x = 1.0f + (geoLocal.x * blockCountW);	// 0 だとシェーダ内で0除算するので +1
		v->UVTileUnit.y = 1.0f + (geoLocal.y * blockCountH);
	}
}














//==============================================================================
// DrawingCommands
//==============================================================================
enum class DrawingCommandType : uint32_t
{
	MoveTo,
	LineTo,
	BezierCurveTo,
	DrawPoint,
	DrawLine,
	DrawTriangle,
	DrawRectangle,
	//DrawEllipse,
	ClosePath,

};
struct DrawingCommands_MoveTo
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_LineTo
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_BezierCurveTo
{
	DrawingCommandType	type;
	Vector3				cp1;
	Vector3				cp2;
	Vector3				endPoint;
	ColorF				color;
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
};
//struct DrawingCommands_DrawEllipse
//{
//	DrawingCommandType	type;
//	Vector3				center;
//	Vector2				radius;
//};
struct DrawingCommands_ClosePath
{
	DrawingCommandType	type;
};



//==============================================================================
// PrimitiveCache
//==============================================================================
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

//------------------------------------------------------------------------------
PrimitiveCache::PrimitiveCache()
{
	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);
}

//------------------------------------------------------------------------------
PrimitiveCache::~PrimitiveCache()
{
}

//------------------------------------------------------------------------------
void PrimitiveCache::Clear()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
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
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void PrimitiveCache::ApplyBuffers(Driver::IVertexBuffer* vb, Driver::IIndexBuffer* ib)
{
	vb->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	ib->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
}



//==============================================================================
// DrawingContextImpl
//==============================================================================
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
					SolidColorBrush.Color[0] = c.r;		// TODO: POD 型をまとめて定義したほうがいい気がする
					SolidColorBrush.Color[1] = c.g;
					SolidColorBrush.Color[2] = c.b;
					SolidColorBrush.Color[3] = c.a;
				}
				else if (Type == BrushType_Texture)
				{
					auto t = static_cast<ln::TextureBrush*>(brush);
					TextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					TextureBrush.SourceRect[0] = r.x;		// TODO: POD 型をまとめて定義したほうがいい気がする
					TextureBrush.SourceRect[1] = r.y;
					TextureBrush.SourceRect[2] = r.width;
					TextureBrush.SourceRect[3] = r.height;
					TextureBrush.WrapMode = t->GetWrapMode();
				}
				else if (Type == BrushType_FrameTexture)
				{
					auto t = static_cast<ln::FrameTextureBrush*>(brush);
					FrameTextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					const Rect& r2 = t->GetInnerAreaSourceRect();
					FrameTextureBrush.SourceRect[0] = r.x;		// TODO: POD 型をまとめて定義したほうがいい気がする
					FrameTextureBrush.SourceRect[1] = r.y;
					FrameTextureBrush.SourceRect[2] = r.width;
					FrameTextureBrush.SourceRect[3] = r.height;
					FrameTextureBrush.InnerSourceRect[0] = r2.x;		// TODO: POD 型をまとめて定義したほうがいい気がする
					FrameTextureBrush.InnerSourceRect[1] = r2.y;
					FrameTextureBrush.InnerSourceRect[2] = r2.width;
					FrameTextureBrush.InnerSourceRect[3] = r2.height;
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
				//ForeColor.r = Brush.SolidColorBrush.Color[0];
				//ForeColor.g = Brush.SolidColorBrush.Color[1];
				//ForeColor.b = Brush.SolidColorBrush.Color[2];
				//ForeColor.a = Brush.SolidColorBrush.Color[3];
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
				ForeColor.r = Brush.SolidColorBrush.Color[0];
				ForeColor.g = Brush.SolidColorBrush.Color[1];
				ForeColor.b = Brush.SolidColorBrush.Color[2];
				ForeColor.a = Opacity * Brush.SolidColorBrush.Color[3];
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


	enum class PathType
	{
		Point,
		Line,		// Stroke のみ
		Rectangle,	// TODO: いらないかも
		Path,	// ExpandFill などを実行して面張りを行う
	};

	struct BasePoint
	{
		Vector3			point;
		ColorF			color;
		Vector3			dir;			// 次の点への向き
		float			len;			// 次の点との距離
		Vector3			extrusionDir;	// 押し出し方向 (dirに対する左方向。符号を反転すると右方向への押し出し量になる)

		Vector2			userUV;
		bool			userUVUsed = false;


		//Vector3			distance;	// 次の点との距離
		//Vector3			left;		// 左方向。つまり、ストロークの押し出し方向
		//float			len;
		BasePoint(const Vector3& p, const ColorF& c) : point(p), color(c) {}
	};

	struct Path
	{
		PathType		type;
		int				firstIndex;		// このパスの開始点を示す m_pathPoints 上のインデックス
		int				pointCount;		// このパスを構成する m_pathPoints 上の点の数
		bool			closed;			// パスが閉じているか (false は線分)
	};



	void ExpandGeometriesFill();

	void AddPath(PathType type);
	Path* GetCurrentPath();
	void AddBasePoint(const Vector3& point, const ColorF& color);
	void AddBasePoint(const Vector3& point, const ColorF& color, const Vector2& userUV, bool userUVUsed);
	//void AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color);
	void EndPath(const Vector3* lastPoint, const ColorF* lastColor, bool pathClose);


	void ExpandPoints();

	void ExpandFill();

	//void PutMoveTo(const Vector3& pt, const ColorF& color);
	//void PutLineTo(const Vector3& pt, const ColorF& color);

	void ExpandStroke(bool vertexOnly);
	void MakeJoint_Bevel(const BasePoint* p0, const BasePoint* p1);

	void AddVertex(const Vector3& point, const ColorF& color);

	static float Normalize(float *x, float* y)
	{
		float d = sqrtf((*x)*(*x) + (*y)*(*y));
		if (d > 1e-6f) {
			float id = 1.0f / d;
			*x *= id;
			*y *= id;
		}
		return d;
	}

	GraphicsManager*		m_manager;
	//PrimitiveCache			m_primitiveCache;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	Matrix					m_view;
	Matrix					m_proj;
	DrawingState			m_currentState;
	CacheBuffer<GeometryData>	m_geometryDataList;

	//CacheBuffer<PathPoint>	m_pathPoints;
	CacheBuffer<Path>		m_pathes;
	bool					m_pathCreating;

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
		Driver::IShaderVariable*    varGlyphMaskSampler;
		Driver::IShaderTechnique*   techMainDraw;
		Driver::IShaderPass*        passP0;

	} m_shader3D;
};


//------------------------------------------------------------------------------
DrawingContextImpl::DrawingContextImpl(GraphicsManager* manager)
	: m_manager(manager)
	, m_pathCreating(false)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
	//m_pathPoints.Reserve(1024);
	m_geometryDataList.Reserve(1024);
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
#include "Resource/GeometryRenderer.fx.h"
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
	m_shader3D.varGlyphMaskSampler = m_shader3D.shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader3D.techMainDraw = m_shader3D.shader->GetTechnique(0);
	m_shader3D.passP0 = m_shader3D.techMainDraw->GetPass(0);
}

//------------------------------------------------------------------------------
DrawingContextImpl::~DrawingContextImpl()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_shader3D.shader);
}

//------------------------------------------------------------------------------
void DrawingContextImpl::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	m_view = view;
	m_proj = proj;
}

//------------------------------------------------------------------------------
void DrawingContextImpl::SetState(const DrawingState& state)
{
	m_currentState.Copy(state);
	m_currentState.UpdateCurrentForeColor();
}

static float BezierCurve(float x1, float x2, float x3, float x4, float t)
{
	float tp = 1.f - t;
	return t*t*t*x4 + 3.f*t*t*tp*x3 + 3.f*t*tp*tp*x2 + tp*tp*tp*x1;
}

//------------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size, detail::DrawingClass drawingClass)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	m_currentState.drawingClass = drawingClass;

	const Vector3* lastPoint = nullptr;
	const ColorF* lastColor = nullptr;
	m_pathCreating = false;

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);

		if (drawingClass == detail::DrawingClass::PathStroke)
		{
			switch (type)
			{
				////////////////////////////////////////////////////////////
				case DrawingCommandType::MoveTo:
				{
					AddPath(PathType::Path);
					auto* cmd = (const DrawingCommands_MoveTo*)pos;
					lastPoint = &cmd->point;
					lastColor = &cmd->color;
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::LineTo:
				{
					auto* cmd = (const DrawingCommands_LineTo*)pos;
					if (!GetCurrentPath()->closed)
					{
						AddBasePoint(*lastPoint, *lastColor);
						lastPoint = &cmd->point;
						lastColor = &cmd->color;
					}
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::BezierCurveTo:
				{
					auto* cmd = (const DrawingCommands_BezierCurveTo*)pos;
					if (!GetCurrentPath()->closed)
					{
						// 
						float lenMax = ((*lastPoint) - cmd->endPoint).GetLength() * 2;
						int divCount = (int)(lenMax / 10);// TODO:単純に1/10で分割しているだけ。ピクセル単位にしたい
						for (float i = 0; i < divCount; i += 1.f)
						{
							float t = i / divCount;
							AddBasePoint(
								Vector3(
									BezierCurve(lastPoint->x, cmd->cp1.x, cmd->cp2.x, cmd->endPoint.x, t),
									BezierCurve(lastPoint->y, cmd->cp1.y, cmd->cp2.y, cmd->endPoint.y, t),
									cmd->endPoint.z),	// TODO
								ColorF::Lerp(*lastColor, cmd->color, t));
						}
						lastPoint = &cmd->endPoint;
						lastColor = &cmd->color;
					}
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::ClosePath:
				{
					auto* cmd = (const DrawingCommands_ClosePath*)pos;
					EndPath(lastPoint, lastColor, true);
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				default:
					LN_THROW(0, InvalidOperationException);
					return;
			}
		}
		else
		{
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
						pos += sizeof(DrawingCommands_DrawPoint);

					} while (pos < end && *((const DrawingCommandType*)pos) == DrawingCommandType::DrawPoint);

					EndPath(nullptr, nullptr, false);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawLine:
				{
					auto* cmd = (const DrawingCommands_DrawLine*)pos;

					AddPath(PathType::Line);
					AddBasePoint(cmd->from, cmd->fromColor);
					AddBasePoint(cmd->to, cmd->toColor);
					EndPath(nullptr, nullptr, false);

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
					EndPath(nullptr, nullptr, true);
					pos += sizeof(DrawingCommands_DrawTriangle);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawRectangle:
				{
					auto* cmd = (const DrawingCommands_DrawRectangle*)pos;
					const RectF& rect = cmd->rect;

					GeometryData g;
					g.type = GeometryType::Rectangle;
					g.rectangle.x = rect.x;
					g.rectangle.y = rect.y;
					g.rectangle.w = rect.width;
					g.rectangle.h = rect.height;
					m_geometryDataList.Add(g);

					pos += sizeof(DrawingCommands_DrawRectangle);
					break;
				}
				default:
					LN_THROW(0, InvalidOperationException);
					return;
				}
		}
	}

	EndPath(lastPoint, lastColor, false);
}

//------------------------------------------------------------------------------
void DrawingContextImpl::Flush()
{
	//if (m_pathes.GetCount() == 0) { return; }
	if (m_geometryDataList.GetCount() == 0) return;


	Driver::IRenderer* renderer = m_manager->GetGraphicsDevice()->GetRenderer();
	m_shader3D.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader3D.varViewProjMatrix->SetMatrix(m_view * m_proj);

	//renderer->Clear(ClearFlags::Depth, ColorF::White, 1.0f);

	if (m_shader3D.varPixelStep != nullptr)
	{
		const Size& size = m_manager->GetRenderer()->GetRenderTarget(0)->GetSize();
		m_shader3D.varPixelStep->SetVector(Vector4(0.5f / size.width, 0.5f / size.height, 0, 0));
	}

	//if (m_currentState.drawingClass == detail::DrawingClass::PathStroke)
	//{
	//	ExpandStroke(false);	// TODO test
	//	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//	m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
	//	m_shader3D.passP0->Apply();
	//	renderer->DrawPrimitive(m_vertexBuffer, PrimitiveType_TriangleStrip, 0, m_vertexCache.GetCount() - 2);
	//}
	//// PointList
	//else if (m_currentState.drawingClass == detail::DrawingClass::PointList)
	//{
	//	ExpandPoints();
	//	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//	m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
	//	m_shader3D.passP0->Apply();
	//	renderer->DrawPrimitive(m_vertexBuffer, PrimitiveType_PointList, 0, m_vertexCache.GetCount());
	//}
	//else
	{
		ExpandGeometriesFill();

		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.varGlyphMaskSampler->SetTexture(m_manager->GetDummyTexture());
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitiveIndexed(m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
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
	m_geometryDataList.Clear();
}

void DrawingContextImpl::ExpandGeometriesFill()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	ColorF fillColor = ColorF::White;
	if (m_currentState.Brush.Type == BrushType_SolidColor)
		fillColor = *((ColorF*)m_currentState.Brush.SolidColorBrush.Color);

	RectF srcPixelRect = RectF::Zero;
	RectF srcUVRect = RectF::Zero;
	if (m_currentState.Brush.Type == BrushType_Texture)
	{
		const Size& size = m_currentState.Brush.TextureBrush.Texture->GetRealSize();
		int* rc = m_currentState.Brush.TextureBrush.SourceRect;
		srcPixelRect.x = rc[0];
		srcPixelRect.y = rc[1];
		srcPixelRect.width = std::min(rc[2], size.width);
		srcPixelRect.height = std::min(rc[3], size.width);
		srcUVRect.x = srcPixelRect.x / size.width;
		srcUVRect.y = srcPixelRect.y / size.width;
		srcUVRect.width= srcPixelRect.width / size.width;
		srcUVRect.height = srcPixelRect.height / size.width;
	}
	

	for (int i = 0; i < m_geometryDataList.GetCount(); ++i)
	{
		GeometryData& g = m_geometryDataList.GetAt(i);
		VertexExpandResult result;
		DrawingBasicVertex* begin = &m_vertexCache.GetAt(m_vertexCache.GetCount());

		switch (g.type)
		{
		case GeometryType::Rectangle:
			g.ExpandFill_Rectangle(&m_vertexCache, &m_indexCache, fillColor, &result);
			g.ExpandUV_Stretch(begin, begin + m_vertexCache.GetCount(), srcPixelRect, srcUVRect, result.posMin, result.posMax);
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
void DrawingContextImpl::AddPath(PathType type)
{
	if (!m_pathCreating)
	{
		Path path;
		path.type = type;
		path.firstIndex = m_basePoints.GetCount();//m_pathPoints.GetCount();
		path.pointCount = 0;
		path.closed = false;
		m_pathes.Add(path);
		m_pathCreating = true;
	}
}

//------------------------------------------------------------------------------
//void DrawingContextImpl::AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color)
//{
//	PathPoint pt;
//	pt.attr = attr;
//	pt.point = point;
//	pt.color = color;
//	m_pathPoints.Add(pt);
//	GetCurrentPath()->pointCount++;
//}

//------------------------------------------------------------------------------
void DrawingContextImpl::AddBasePoint(const Vector3& point, const ColorF& color)
{
	BasePoint pt(point, color);
	pt.color.r *= m_currentState.ForeColor.r;
	pt.color.g *= m_currentState.ForeColor.g;
	pt.color.b *= m_currentState.ForeColor.b;
	pt.color.a *= m_currentState.ForeColor.a;
	m_basePoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//------------------------------------------------------------------------------
void DrawingContextImpl::AddBasePoint(const Vector3& point, const ColorF& color, const Vector2& userUV, bool userUVUsed)
{
	BasePoint pt(point, color);
	pt.color.r *= m_currentState.ForeColor.r;
	pt.color.g *= m_currentState.ForeColor.g;
	pt.color.b *= m_currentState.ForeColor.b;
	pt.color.a *= m_currentState.ForeColor.a;
	pt.userUV = userUV;
	pt.userUVUsed = userUVUsed;
	m_basePoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//------------------------------------------------------------------------------
void DrawingContextImpl::EndPath(const Vector3* lastPoint, const ColorF* lastColor, bool pathClose)
{
	if (m_pathCreating)
	{
		if (lastPoint != nullptr && lastColor != nullptr) {
			AddBasePoint(*lastPoint, *lastColor);
		}
		Path* curPath = GetCurrentPath();
		curPath->closed = pathClose;
		m_pathCreating = false;

		// 各頂点について、距離などの情報を作る。
		// 終端の点は先頭の点との距離になる。
		if (curPath->pointCount >= 2)
		{
			BasePoint* p0 = &m_basePoints.GetAt(curPath->pointCount - 1);
			BasePoint* p1 = &m_basePoints.GetAt(curPath->firstIndex);
			for (int i = 0; i < curPath->pointCount; ++i)
			{
				p0->dir.x = p1->point.x - p0->point.x;
				p0->dir.y = p1->point.y - p0->point.y;
				p0->len = Normalize(&p0->dir.x, &p0->dir.y);	// dir を正規化した上で長さを返す

																//p0->distance.X = p1->point.X - p0->point.X;
																//p0->distance.Y = p1->point.Y - p0->point.Y;
																//p0->left = Vector3::Cross(p0->distance, Vector3::UnitZ);
																//p0->left.Normalize();
																//p0->len = p0->distance.GetLength();
				p0 = p1++;
			}

			// 押し出し方向を求める
			p0 = &m_basePoints.GetAt(curPath->pointCount - 1);
			p1 = &m_basePoints.GetAt(curPath->firstIndex);
			for (int i = 0; i < curPath->pointCount; ++i)
			{
				//p1->extrusion.X = (p0->dir.Y + p1->dir.Y);
				//p1->extrusion.Y = (p0->dir.X + p1->dir.X);
				//Normalize(&p1->extrusion.X, &p1->extrusion.Y);
				p1->extrusionDir = Vector3::Cross(p1->dir, Vector3::UnitZ);
				p0 = p1++;
			}
		}
	}
}

//------------------------------------------------------------------------------
DrawingContextImpl::Path* DrawingContextImpl::GetCurrentPath()
{
	return &m_pathes.GetLast();
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void DrawingContextImpl::PutMoveTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::MoveTo, pt, color);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void DrawingContextImpl::PutLineTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::LineTo, pt, color);
//}

//------------------------------------------------------------------------------
void DrawingContextImpl::ExpandPoints()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		// 頂点バッファを作る
		for (int i = 0; i < path.pointCount; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			m_vertexCache.Add(v);
		}
	}
}

//------------------------------------------------------------------------------
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
			posMin.x = std::min(posMin.x, pt.point.x);
			posMin.y = std::min(posMin.y, pt.point.y);
			posMax.x = std::max(posMax.x, pt.point.x);
			posMax.y = std::max(posMax.y, pt.point.y);
		}
		Vector2 uvSpan = posMax - posMin;
		if (uvSpan.x == 0.0f || uvSpan.y == 0.0) continue;	// 潰れたパス。描く必要はない
		uvSpan = 1.0f / uvSpan;

		// 頂点バッファを作る
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			v.UVOffset.x = (pt.point.x - posMin.x) * uvSpan.x;
			v.UVOffset.y = (pt.point.y - posMin.y) * uvSpan.y;
			m_vertexCache.Add(v);
		}

		// インデックスバッファを作る (反時計回り)
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
			/*
				↑の概要：
					頂点は反時計回りに並んでいることを前提とし、
					前後それぞれの方向からカーソルを進めるようにして三角形を作っていく。

				- 0回目、0,1,5 を結ぶ
					0-5 4
					|/
					1 2 3

				- 1回目、1,2,5 を結ぶ
					0-5 4
					|/|
					1-2 3

				- 3回目、5,2,4 を結ぶ
					0-5-4
					|/|/
					1-2 3

				- 4回目、2,3,4 を結ぶ
					0-5-4
					|/|/|
					1-2-3
			*/
		}
	}
}

//------------------------------------------------------------------------------
void DrawingContextImpl::ExpandStroke(bool vertexOnly)
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);
		int count = path.pointCount;

#if 0
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
#endif

		// パスが閉じていれば p0 は[last]、p1は[0] として以下の処理を開始
		// パスが閉じていなければ p0 は[0]、p1は[1] として以下の処理を開始
		BasePoint* p0 = &m_basePoints.GetAt(path.firstIndex + path.pointCount - 1);
		BasePoint* p1 = &m_basePoints.GetAt(path.firstIndex);
		if (!path.closed)
		{
			p0 = p1++;
			--count;	// ループ数も-1
		}
		else {
			// パスが閉じている時は、最初に last->begin へ線が引かれることになる。
		}

		// 頂点バッファを作る
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = *p0;//m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;

			//MakeJoint_Bevel(p0, p1);

			//v.Position = pt.point - (pt.left * 5);	// 内側
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);
			//v.Position = p0->point + (p0->left * 5);	// 外側
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);


			//v.Position = pt.point + (pt.extrusionDir * 5);	// 外側
			//v.Color = pt.color;
			////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);

			// 直線の視点部分

			v.Position = pt.point - (pt.extrusionDir * 5);	// 内側
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = pt.point + (pt.extrusionDir * 5);	// 外側
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = p1->point - (p0->extrusionDir * 5);	// 内側
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = p1->point + (p0->extrusionDir * 5);	// 外側
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);



			/*if (i == count - 1)
			{

			}
			else *//*if (!path.closed && i == count - 2)
			{
			}
			else*/
			{
				// p1 → p2 への接合を行う

				v.Position = p1->point - (p0->extrusionDir * 5);	// 内側
				v.Color = pt.color;
				//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
				//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
				m_vertexCache.Add(v);

				v.Position = p1->point + (p1->extrusionDir * 5);	// 外側
				v.Color = pt.color;
				//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
				//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
				m_vertexCache.Add(v);
			}


			//v.Position = p1->point + (p0->left * 5);	// 外側
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);

			/*
			方針
			p1 の点を打つ。p0はもう打たれている (終点はまだだが)
			つまりというか、p0→p1の線を引くのが1ループ。
			p1が処理の中心で、p0は「1つ前の点」
			接合点の処理はp0の部分に対して行う。
			*/


			p0 = p1++;
		}

		// パスが閉じていれば end->begin へも引く
		//if (path.closed)
		//{
		//	const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);

		//	DrawingBasicVertex v;
		//	v.Position = pt.point;
		//	v.Color = pt.color;
		//	v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
		//	v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
		//	m_vertexCache.Add(v);

		//	v.Position = pt.point + (pt.left * 5);
		//	v.Color = pt.color;
		//	v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
		//	v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
		//	m_vertexCache.Add(v);
		//}
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

//------------------------------------------------------------------------------
void DrawingContextImpl::MakeJoint_Bevel(const BasePoint* p0, const BasePoint* p1)
{
	//DrawingBasicVertex v;
	//v.Position = p0->point - (p0->left * 5);	// 内側
	//v.Color = p0->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p0->point + (p0->left * 5);	// 外側
	//v.Color = p0->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p1->point - (p0->left * 5);	// 内側
	//v.Color = p1->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p1->point + (p0->left * 5);	// 外側
	//v.Color = p1->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);
}

//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
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


//==============================================================================
// GeometryRenderer
//==============================================================================

//------------------------------------------------------------------------------
GeometryRenderer::GeometryRenderer()
	: m_manager(nullptr)
	, m_commandsUsingByte(0)
	, m_internal(nullptr)
	, m_currentDrawingClass(detail::DrawingClass::Unknown)
	, m_flushRequested(false)
	, m_internalTextureBrush(nullptr)
{
	m_internalTextureBrush = LN_NEW TextureBrush();
}

//------------------------------------------------------------------------------
GeometryRenderer::~GeometryRenderer()
{
	LN_SAFE_RELEASE(m_internalTextureBrush);
	LN_SAFE_RELEASE(m_internal);
}

//------------------------------------------------------------------------------
void GeometryRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_internal = LN_NEW	DrawingContextImpl(m_manager);
	m_commandsBuffer.Resize(1024);
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize)
{
	m_uvParPixel.x = 1.0f / viewPixelSize.width;
	m_uvParPixel.y = 1.0f / viewPixelSize.height;
	m_invViewProj = Matrix::MakeInverse(view * proj);
	m_viewOnePixelOffset = Vector2::TransformCoord(m_uvParPixel, m_invViewProj);
	LN_CALL_COMMAND(SetViewProjection, DrawingContextImpl::SetViewProjectionCommand, view, proj);
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetTransform(const Matrix& matrix)
{
	if (m_currentState.transform != matrix)
	{
		FlushInternal();
		m_currentState.transform = matrix;
	}
}

//template<typename T>
//bool operator != (RefPtr<T>& left, void* ptr) { return (left.GetObjectPtr() != ptr); }
//
//template<typename T>
//bool operator != (const RefPtr<T>& left, T* ptr) { return (left.GetObjectPtr() != ptr); }
//
//template<typename T>
//bool operator != (const RefPtr<T>& left, RefObject* ptr) { return (left.GetObjectPtr() != ptr); }
//
//------------------------------------------------------------------------------
void GeometryRenderer::SetBrush(Brush* brush)
{
	if (m_currentState.brush != brush)
	{
		FlushInternal();
		m_currentState.brush = brush;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetPen(Pen* pen)
{
	if (m_currentState.pen != pen)
	{
		FlushInternal();
		m_currentState.pen = pen;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetOpacity(float opacity)
{
	// TODO: opacity はFlushいらないと思う。直接頂点色に乗算しておいた方が高速？
	if (m_currentState.opacity != opacity)
	{
		FlushInternal();
		m_currentState.opacity = opacity;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetTone(const ToneF& tone)
{
	if (m_currentState.tone != tone)
	{
		FlushInternal();
		m_currentState.tone = tone;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetFont(Font* font)
{
	if (m_currentState.font != font)
	{
		FlushInternal();
		m_currentState.font = font;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::MoveTo(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_MoveTo cmd;
	cmd.type = DrawingCommandType::MoveTo;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void GeometryRenderer::LineTo(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_LineTo cmd;
	cmd.type = DrawingCommandType::LineTo;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void GeometryRenderer::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_BezierCurveTo cmd;
	cmd.type = DrawingCommandType::BezierCurveTo;
	cmd.cp1 = cp1;
	cmd.cp2 = cp2;
	cmd.endPoint = endPt;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void GeometryRenderer::ClosePath()
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_ClosePath cmd;
	cmd.type = DrawingCommandType::ClosePath;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
//void GeometryRenderer::DrawPoint(const Vector3& point, const ColorF& color)
//{
//	SetDrawingClassInternal(detail::DrawingClass::PointList);
//	DrawingCommands_DrawPoint cmd;
//	cmd.type = DrawingCommandType::DrawPoint;
//	cmd.point = point;
//	cmd.color = color;
//	AddCommand(&cmd, sizeof(cmd));
//	m_flushRequested = true;
//}
//
//------------------------------------------------------------------------------
void GeometryRenderer::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
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

//------------------------------------------------------------------------------
void GeometryRenderer::DrawLine(const Vector3& from, const Vector3& to, const ColorF& color)
{
	DrawLine(from, to, color, color);
}

//------------------------------------------------------------------------------
void GeometryRenderer::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
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

//------------------------------------------------------------------------------
void GeometryRenderer::DrawRectangle(const RectF& rect)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	//CheckFlush();

	DrawingCommands_DrawRectangle cmd;
	cmd.type = DrawingCommandType::DrawRectangle;
	cmd.rect = rect;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void GeometryRenderer::DrawEllipse(const Vector3& center, const Vector2& radius)
{
}

//------------------------------------------------------------------------------
void GeometryRenderer::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
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
	DrawRectangle(rect);
}

//------------------------------------------------------------------------------
void GeometryRenderer::Flush()
{
	FlushInternal();
}

//------------------------------------------------------------------------------
void GeometryRenderer::AddCommand(const void* command, size_t size)
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

//------------------------------------------------------------------------------
void GeometryRenderer::FlushInternal()
{
	if (m_flushRequested)
	{
		DrawingContextImpl::DrawingState state;
		state.Create(m_currentState);
		LN_CALL_COMMAND(SetState, DrawingContextImpl::SetStateCommand, state);

		LN_CALL_COMMAND(DoCommandList, DrawingContextImpl::DoCommandListCommand, m_commandsBuffer.GetConstData(), m_commandsUsingByte, m_currentDrawingClass);
		m_commandsUsingByte = 0;

		//LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);
        if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded) {
            m_manager->GetPrimaryRenderingCommandList()->AddCommand<DrawingContextImpl::FlushCommand>(m_internal);
        }
        else {
            m_internal->Flush();
        }
        
		m_flushRequested = false;
	}
}

//------------------------------------------------------------------------------
void GeometryRenderer::SetDrawingClassInternal(detail::DrawingClass dc)
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
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GeometryRenderer::CheckFlush()
//{
//		FlushInternal();
//	}
//
//}

} // namespace detail

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

