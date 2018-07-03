
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/Texture.hpp>

namespace ln {

//==============================================================================
// Bitmap2D

/*
	TextureFormat で処理を分岐するのは switch と関数ポインタどちらがいい？
	100000 ループした結果

		switch(20個) : FuncPtr
		323019 : 623231 [ns]
		331858 : 647465
		320168 : 606980
		490659 : 947391
		386312 : 628933
		317032 : 609831
		399997 : 705340
		318173 : 606695
		551101 : 608406
		320168 : 610972

	switch の方が数が増えるほど遅くなるが20個あれば十分。
*/

Bitmap2D::Bitmap2D()
{
}

Bitmap2D::~Bitmap2D()
{
}

void Bitmap2D::initialize(int width, int height, PixelFormat format)
{
	m_width = width;
	m_height = height;
	//m_depth = 0;
	m_format = format;
	m_buffer.resize(getBitmapByteSize(m_width, m_height, 1, m_format));
}

Color32 Bitmap2D::getColor32(int x, int y) const
{
	if (m_format == PixelFormat::RGBA32)
	{
		const uint8_t* pixel = m_buffer.data() + ((y * m_width) + x) * 4;
		return Color32(pixel[0], pixel[1], pixel[2], pixel[3]);
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return Color32();
	}
}

void Bitmap2D::flipVerticalFlow()
{
	if (LN_REQUIRE(m_format != PixelFormat::Unknown)) return;
	if (LN_REQUIRE(m_format != PixelFormat::A1)) return;

	int pixelSize = getPixelFormatByteSize(m_format);
	if (pixelSize == 1)
	{
		// XOR で工夫すると演算回数が少なくなるとか最適化の余地はあるけど、
		// とりあえず今は評価目的でしか使わないので愚直に swap。
		byte_t* pixels = m_buffer.data();
		for (int y = 0; y < (m_height / 2); ++y) {
			for (int x = 0; x < m_width; ++x) {
				std::swap(pixels[(y * m_width) + x], pixels[((m_height - 1 - y) * m_width) + x]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_buffer.data();
		for (int y = 0; y < (m_height / 2); ++y) {
			for (int x = 0; x < m_width; ++x) {
				std::swap(pixels[(y * m_width) + x], pixels[((m_height - 1 - y) * m_width) + x]);
			}
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

int Bitmap2D::getPixelFormatByteSize(PixelFormat format)
{
	const int table[] =
	{
		0,	// Unknown,
		1,	// A1,
		1,	// A8,
		4,	// RGBA32,
		16,	// R32G32B32A32Float,
	};
	return table[(int)format];
}

int Bitmap2D::getBitmapByteSize(int width, int height, int depth, PixelFormat format)
{
	return getPixelFormatByteSize(format) * width * height * depth;
}

//==============================================================================
// RenderTargetTexture

RenderTargetTexture::RenderTargetTexture()
{
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::initialize(int width, int height, TextureFormat requestFormat, bool mipmap)
{
	GraphicsResource::initialize();
	//m_size.width = width;
	//m_size.height = height;
	//m_requestFormat = requestFormat;
	//m_mipmap = mipmap;
	m_rhiObject = manager()->deviceContext()->createRenderTarget(width, height, requestFormat, mipmap);
}

void RenderTargetTexture::initialize(detail::ITexture* ref)
{
	GraphicsResource::initialize();
	m_rhiObject = ref;
}

Ref<Bitmap2D> RenderTargetTexture::readData()
{
	detail::ITexture* rhiObject = resolveRHIObject();

	SizeI size;
	rhiObject->getSize(&size);
	auto bitmap = newObject<Bitmap2D>(size.width, size.height, GraphicsHelper::translateToPixelFormat(rhiObject->getTextureFormat()));

	if (manager()->renderingType() == GraphicsRenderingType::Threaded)
	{
		LN_NOTIMPLEMENTED();
	}
	else
	{
		rhiObject->readData(bitmap->data());
	}

	bitmap->flipVerticalFlow();
	return bitmap;
}

detail::ITexture* RenderTargetTexture::resolveRHIObject()
{
	return m_rhiObject;
}

void RenderTargetTexture::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	LN_NOTIMPLEMENTED();
}

} // namespace ln
