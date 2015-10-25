
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include "Device/GraphicsDriverInterface.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

template<typename T>
class CacheBuffer
{
public:
	CacheBuffer()
		: m_buffer()
		, m_capacity(0)
		, m_count(0)
	{
	}

	void Reserve(int count)
	{
		m_buffer.Resize(sizeof(T) * count, false);
		m_capacity = count;
	}

	void Add(const T& value)
	{
		if (m_count >= m_capacity) {
			Reserve(m_capacity * 2);
		}
		memcpy(&m_buffer[sizeof(T) * m_count], &value, sizeof(T));
		m_count++;
	}

	void Clear()
	{
		m_count = 0;
	}

	int GetCount() const { return m_count; }
	byte_t* GetBuffer() { return m_buffer.GetData(); }
	size_t GetBufferUsedByteCount() { return m_count * sizeof(T); }


private:
	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};


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
};

// このクラスは描画コマンドの引数となるクラス。RefPtr とかはメンバに置かないこと。
struct PainterEngineState
{
	BrushData	Brush;
	float		Opacity;
	ColorF		ForeColor;		///< 乗算する色。SolidColorBrush の時はその色になる。それと Opacity の乗算結果。
	ToneF		Tone;

	// Painter 側で作り、PainterEngineState をコマンドリストに乗せるときに使う。
	void Create(const Details::PainterState& state)
	{
		// ブラシデータ
		ForeColor = ColorF::White;
		Lumino::Brush* brush = state.Brush;
		if (brush == NULL)
		{
			Brush.Type = BrushType_Unknown;	// no set
		}
		else
		{
			Brush.Type = brush->GetType();
			if (Brush.Type == BrushType_SolidColor)
			{
				auto t = static_cast<ColorBrush*>(brush);
				const ColorF& c = t->GetColor();
				Brush.SolidColorBrush.Color[0] = ForeColor.R = c.R;		// TODO: POD 型をまとめて定義したほうがいい気がする
				Brush.SolidColorBrush.Color[1] = ForeColor.G = c.G;
				Brush.SolidColorBrush.Color[2] = ForeColor.B = c.B;
				Brush.SolidColorBrush.Color[3] = ForeColor.A = c.A;
			}
			else if (Brush.Type == BrushType_Texture)
			{
				auto t = static_cast<TextureBrush*>(brush);
				Brush.TextureBrush.Texture = (t->GetTexture() != NULL) ? t->GetTexture()->GetDeviceObject() : NULL;
				const Rect& r = t->GetSourceRect();
				Brush.TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
				Brush.TextureBrush.SourceRect[1] = r.Y;
				Brush.TextureBrush.SourceRect[2] = r.Width;
				Brush.TextureBrush.SourceRect[3] = r.Height;
				Brush.TextureBrush.WrapMode = t->GetWrapMode();
			}
			else if (Brush.Type == BrushType_FrameTexture)
			{
				auto t = static_cast<FrameTextureBrush*>(brush);
				Brush.FrameTextureBrush.Texture = (t->GetTexture() != NULL) ? t->GetTexture()->GetDeviceObject() : NULL;
				const Rect& r = t->GetSourceRect();
				const Rect& r2 = t->GetInnerAreaSourceRect();
				Brush.FrameTextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
				Brush.FrameTextureBrush.SourceRect[1] = r.Y;
				Brush.FrameTextureBrush.SourceRect[2] = r.Width;
				Brush.FrameTextureBrush.SourceRect[3] = r.Height;
				Brush.FrameTextureBrush.InnerSourceRect[0] = r2.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
				Brush.FrameTextureBrush.InnerSourceRect[1] = r2.Y;
				Brush.FrameTextureBrush.InnerSourceRect[2] = r2.Width;
				Brush.FrameTextureBrush.InnerSourceRect[3] = r2.Height;
				Brush.FrameTextureBrush.WrapMode = t->GetWrapMode();
				Brush.FrameTextureBrush.FrameThicness = t->GetThickness();
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}

		// 不透明度
		Opacity = state.Opacity;

		// 色調
		Tone = state.Tone;
	}

	void Copy(const PainterEngineState& state)
	{
		ReleaseObjects();

		// ブラシデータ
		ForeColor = ColorF::White;
		memcpy(&Brush, &state.Brush, sizeof(Brush));
		if (Brush.Type == BrushType_SolidColor) {
			ForeColor.R = Brush.SolidColorBrush.Color[0];
			ForeColor.G = Brush.SolidColorBrush.Color[1];
			ForeColor.B = Brush.SolidColorBrush.Color[2];
			ForeColor.A = Brush.SolidColorBrush.Color[3];
		}
		else if (Brush.Type == BrushType_Texture) {
			LN_SAFE_ADDREF(Brush.TextureBrush.Texture);
		}
		else if (Brush.Type == BrushType_FrameTexture) {
			LN_SAFE_ADDREF(Brush.FrameTextureBrush.Texture);
		}

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


// Begin～Endをネストするため
struct PainterEngineSection
{
public:
	PainterEngineState	CurrentState;

};

/// PainterEngine
///		このクラスは Brush を参照しないようにすること。
class PainterEngine
	: public GraphicsResourceObject
{
public:
	struct GlyphRunData
	{
		PointF		Position;
		RectF		SrcPixelRect;
	};

public:
	PainterEngine();
	virtual ~PainterEngine();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

public:
	void Create(GraphicsManager* manager);
	void CreateInternal();


	void Begin();
	void End();
	void Flush();


	void SetTransform(const Matrix& matrix);


	//void PushTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);

	void SetState(const PainterEngineState& state);

	void DrawRectangle(const RectF& rect);


	// srcTexture は NULL ならダミーテクスチャが使われる
	/// srcRect はサイズが INT_MAX であれば全体を転送することを示す
	void DrawFrameRectangle(const RectF& rect, float frameWidth/*, Driver::ITexture* srcTexture, const Rect& srcRect*/);

	/// 塗りつぶし描画
	void DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Driver::ITexture* glyphsTexture, Driver::ITexture* strokesTexture/*, const ColorF& foreColor, const ColorF& strokeColor*/);

private:
	PainterEngineState& GetCurrentState() { return m_sectionStack.GetTop().CurrentState; }
	void InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect);
	void InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Driver::ITexture* srcTexture);

	void AttachBrushData();
	//void DetachBrushData();

	void SetInternalGlyphMaskTexture(Driver::ITexture* mask);
	void UpdateCurrentForeColor();


private:
	struct PainterVertex
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

	GraphicsManager*				m_manager;
	Driver::IRenderer*				m_renderer;
	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
	RefPtr<Driver::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Driver::IIndexBuffer>	m_indexBuffer;
	RefPtr<Driver::ITexture>		m_dummyTexture;
	Matrix							m_baseTransform;

	Stack<PainterEngineSection>		m_sectionStack;

	RefPtr<Driver::ITexture>		m_currentInternalGlyphMask;	// 文字描画時に使うグリフの形状をマスクするためのテクスチャ

	struct
	{
		RefPtr<Driver::IShader>		Shader;
		Driver::IShaderTechnique*	Technique;
		Driver::IShaderPass*		Pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varTone;
		Driver::IShaderVariable*	varTexture;
		Driver::IShaderVariable*	varGlyphMaskSampler;
		Driver::IShaderVariable*	varViewportSize;	///< DX9 HLSL 用のピクセルオフセット計算用。GLSL では最適化により消えることもある

	} m_shader;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
