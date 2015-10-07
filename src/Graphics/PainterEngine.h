
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




//enum BrushDataType
//{
//	BrushDataType_NoSet = 0,
//	BrushDataType_SolidColor,
//	BrushDataType_Texture,
//};

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


	void Begin();	// 今のところネスト禁止
	void End();


	void SetTransform(const Matrix& matrix);


	//void PushTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);

	void SetBrush(const BrushData& data);
	void SetOpacity(float opacity);
	

	void DrawRectangle(const RectF& rect);

	void Flush();




	//void DrawFillRectangle(const RectF& rect, Driver::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode);

	// srcTexture は NULL ならダミーテクスチャが使われる
	/// srcRect はサイズが INT_MAX であれば全体を転送することを示す
	void DrawFrameRectangle(const RectF& rect, float frameWidth/*, Driver::ITexture* srcTexture, const Rect& srcRect*/);

	/// 塗りつぶし描画
	void DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Driver::ITexture* glyphsTexture, Driver::ITexture* strokesTexture/*, const ColorF& foreColor, const ColorF& strokeColor*/);

private:
	void InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect);
	void InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Driver::ITexture* srcTexture);

	void AttachBrushData();
	void DetachBrushData();

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

	struct State
	{
		BrushData	Brush;
		float		Opacity;
		ColorF		ForeColor;		///< 乗算する色。SolidColorBrush の時はその色になる。それと Opacity の乗算結果。
		RefPtr<Driver::ITexture>	InternalGlyphMask;
	};

	GraphicsManager*				m_manager;
	Driver::IRenderer*				m_renderer;
	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
	RefPtr<Driver::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Driver::IIndexBuffer>	m_indexBuffer;
	RefPtr<Driver::ITexture>		m_dummyTexture;
	Matrix							m_baseTransform;

	State	m_currentState;

	//BrushData						m_currentBrushData;

	struct
	{
		RefPtr<Driver::IShader>		Shader;
		Driver::IShaderTechnique*	Technique;
		Driver::IShaderPass*		Pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varTexture;
		Driver::IShaderVariable*	varGlyphMaskSampler;
		Driver::IShaderVariable*	varViewportSize;	///< DX9 HLSL 用のピクセルオフセット計算用。GLSL では最適化により消えることもある

	} m_shader;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
