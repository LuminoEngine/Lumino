
#pragma once

#include "Device/DeviceInterface.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>

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
			Device::ITexture*	Texture;
			int					SourceRect[4];	///< XYWH
			BrushWrapMode		WrapMode;

		} TextureBrush;
	};
};




/// PainterEngine
///		このクラスは Brush を参照しないようにすること。
class PainterEngine
	: public RefObject
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

public:
	void Create(GraphicsManager* manager);


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




	//void DrawFillRectangle(const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode);

	// srcTexture は NULL ならダミーテクスチャが使われる
	/// srcRect はサイズが INT_MAX であれば全体を転送することを示す
	void DrawFrameRectangle(const RectF& rect, float frameWidth/*, Device::ITexture* srcTexture, const Rect& srcRect*/);

	/// 塗りつぶし描画
	void DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Device::ITexture* glyphsTexture, Device::ITexture* strokesTexture/*, const ColorF& foreColor, const ColorF& strokeColor*/);

private:
	void InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect);
	void InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Device::ITexture* srcTexture);

	void AttachBrushData();
	void DetachBrushData();

	void SetInternalGlyphMaskTexture(Device::ITexture* mask);
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
		RefPtr<Device::ITexture>	InternalGlyphMask;
	};

	
	Device::IRenderer*				m_renderer;
	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
	RefPtr<Device::ITexture>		m_dummyTexture;
	Matrix							m_baseTransform;

	State	m_currentState;

	//BrushData						m_currentBrushData;

	struct
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderTechnique*	Technique;
		Device::IShaderPass*		Pass;
		Device::IShaderVariable*	varWorldMatrix;
		Device::IShaderVariable*	varViewProjMatrix;
		Device::IShaderVariable*	varTexture;
		Device::IShaderVariable*	varGlyphMaskSampler;
		Device::IShaderVariable*	varViewportSize;	///< DX9 HLSL 用のピクセルオフセット計算用。GLSL では最適化により消えることもある

	} m_shader;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
