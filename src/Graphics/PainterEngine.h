
#pragma once

#include "Device/DeviceInterface.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
namespace Graphics
{

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

	byte_t* GetBuffer() { return m_buffer.GetData(); }
	size_t GetBufferUsedByteCount() { return m_count * sizeof(T); }

private:
	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};


class PainterEngine
	: public RefObject
{
public:
	PainterEngine();
	virtual ~PainterEngine();

public:
	void Create(GraphicsManager* manager);
	//void PushTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);

	// srcTexture は NULL ならダミーテクスチャが使われる
	/// srcRect はサイズが INT_MAX であれば全体を転送することを示す
	void DrawFrameRectangle(const RectF& rect, float frameWidth, Device::ITexture* srcTexture, const Rect& srcRect);

private:
	void InternalDrawRectangle(const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, const RectF& srcUVRect);

private:
	struct PainterVertex
	{
	public:

		Vector3	Position;			///< 位置
		Vector4	Color;				///< 頂点カラー
		Vector4	UVOffset;		///< テクスチャUV (転送元のUV情報)
		Vector2	UVTileUnit;		///< テクスチャUV (タイリング空間のどこにいるか)

		/// 頂点レイアウト
		static Graphics::VertexElement* Elements()
		{
			static Graphics::VertexElement elements[] =
			{
				{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_Color, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 0 },
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 4;
	};

	
	Device::IRenderer*				m_renderer;
	CacheBuffer<PainterVertex>		m_vertexCache;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
	RefPtr<Device::ITexture>		m_dummyTexture;

	struct
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderTechnique*	Technique;
		Device::IShaderPass*		Pass;
		Device::IShaderVariable*	varWorldMatrix;
		Device::IShaderVariable*	varViewProjMatrix;
		Device::IShaderVariable*	varTexture;

	} m_shader;
};

} // namespace Graphics
} // namespace Lumino
