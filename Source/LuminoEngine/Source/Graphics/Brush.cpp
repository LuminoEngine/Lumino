
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Brush.h>
#include "Device/GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Brush
//==============================================================================

Brush*	Brush::White = nullptr;
Brush*	Brush::Black = nullptr;
Brush*	Brush::Gray = nullptr;
Brush*	Brush::Red = nullptr;
Brush*	Brush::Green = nullptr;
Brush*	Brush::Blue = nullptr;
Brush*	Brush::DimGray = nullptr;

//------------------------------------------------------------------------------
Brush::Brush()
	: m_color(0, 0, 0, 1.0f)
	, m_texture(nullptr)
	, m_srcRect(Math::NaN, Math::NaN, Math::NaN, Math::NaN)
	, m_wrapMode(BrushWrapMode::Stretch)
	, m_imageDrawMode(BrushImageDrawMode::Image)
	, m_borderThickness()
{
}

//------------------------------------------------------------------------------
Brush::Brush(const Color& color)
	: Brush()
{
	m_color = color;
}

//------------------------------------------------------------------------------
Brush::~Brush()
{
}

//------------------------------------------------------------------------------
void Brush::initialize()
{
}

//------------------------------------------------------------------------------
void Brush::SetTexture(Texture* texture)
{
	m_texture = texture;
}

//------------------------------------------------------------------------------
Texture* Brush::GetTexture() const
{
	return m_texture;
}

//------------------------------------------------------------------------------
void Brush::GetRawData(detail::BrushRawData* outData) const
{
	if (LN_CHECK_ARG(outData != nullptr)) return;
	outData->color = m_color;
	outData->texture = (m_texture != nullptr) ? m_texture->ResolveDeviceObject() : nullptr;
	outData->srcRect = m_srcRect;
	outData->wrapMode = m_wrapMode;
	outData->imageDrawMode = m_imageDrawMode;
	outData->borderThickness = m_borderThickness;
}

//==============================================================================
// SolidColorBrush
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<SolidColorBrush> SolidColorBrush::Create(const Color& color)
{
	return NewObject<SolidColorBrush>(color);
}

//------------------------------------------------------------------------------
RefPtr<SolidColorBrush> SolidColorBrush::Create(const Color& rgb, float a)
{
	return NewObject<SolidColorBrush>(rgb, a);
}

//------------------------------------------------------------------------------
SolidColorBrush::SolidColorBrush()
{
}

//------------------------------------------------------------------------------
SolidColorBrush::~SolidColorBrush()
{
}

//------------------------------------------------------------------------------
void SolidColorBrush::initialize(const Color& color)
{
	Brush::initialize();
	SetColor(color);
}

//------------------------------------------------------------------------------
void SolidColorBrush::initialize(const Color& rgb, float a)
{
	Brush::initialize();
	SetColor(rgb.WithAlpha(a));
}

//==============================================================================
// TextureBrush
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(const StringRef& filePath)
{
	return NewObject<TextureBrush>(filePath);
}

//------------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(Texture* texture)
{
	return NewObject<TextureBrush>(texture);
}

//------------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(Texture* texture, BrushImageDrawMode drawMode, const Rect& sourceRect, const ThicknessF& borderThickness, BrushWrapMode wrapMode)
{
	auto ptr = NewObject<TextureBrush>(texture);
	ptr->SetImageDrawMode(drawMode);
	ptr->SetSourceRect(sourceRect);
	ptr->SetBorderThickness(borderThickness);
	ptr->SetWrapMode(wrapMode);
	return ptr;
}

//------------------------------------------------------------------------------
TextureBrush::TextureBrush()
	: Brush()
{
}

//------------------------------------------------------------------------------
TextureBrush::~TextureBrush()
{
}

//------------------------------------------------------------------------------
void TextureBrush::initialize()
{
	Brush::initialize();
}

//------------------------------------------------------------------------------
void TextureBrush::initialize(const StringRef& filePath)
{
	Brush::initialize();
	auto texture = Texture2D::Create(filePath, TextureFormat::R8G8B8A8, false);		//TODO: GraphicsManager?
	SetTexture(texture);
}

//------------------------------------------------------------------------------
void TextureBrush::initialize(Texture* texture)
{
	Brush::initialize();
	SetTexture(texture);
}

//------------------------------------------------------------------------------
Rect TextureBrush::GetActualSourceRect() const
{
	Size textureSize(0, 0);
	Texture* texture = GetTexture();
	if (texture != nullptr) textureSize = texture->getSize().ToFloatSize();

	const Rect& rc = GetSourceRect();
	return Rect(
		(Math::IsNaNOrInf(rc.x)) ? 0.0f : rc.x,
		(Math::IsNaNOrInf(rc.y)) ? 0.0f : rc.y,
		(Math::IsNaNOrInf(rc.width)) ? textureSize.width : rc.width,
		(Math::IsNaNOrInf(rc.height)) ? textureSize.height : rc.height);
}

//------------------------------------------------------------------------------
Size TextureBrush::getSize() const
{
	return GetActualSourceRect().getSize();
}

#if 0
//==============================================================================
// ColorBrush
//==============================================================================


//------------------------------------------------------------------------------
ColorBrush::ColorBrush(const Color32& color)
	: m_color(color)
{
}

//------------------------------------------------------------------------------
ColorBrush::ColorBrush(const Color& color)
	: m_color(color)
{
}

//------------------------------------------------------------------------------
ColorBrush::ColorBrush(float r, float g, float b, float a)
	: m_color(r, g, b, a)
{
}

//------------------------------------------------------------------------------
ColorBrush::~ColorBrush()
{
}

//==============================================================================
// TextureBrush
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(const StringRef& filePath)
{
	RefPtr<TextureBrush> obj(LN_NEW TextureBrush(), false);
	obj->Create(filePath.GetBegin(), nullptr);	// TODO: getBegin
	return obj;
}

//------------------------------------------------------------------------------
TextureBrush::TextureBrush()
	: m_srcRect(0, 0, INT_MAX, INT_MAX)
	, m_wrapMode(BrushWrapMode::Stretch)
	, m_imageDrawMode(BrushImageDrawMode::Image)
{
}

//------------------------------------------------------------------------------
TextureBrush::~TextureBrush()
{
}

//------------------------------------------------------------------------------
void TextureBrush::Create(const TCHAR* filePath, detail::GraphicsManager* manager)
{
	m_texture = Texture2D::Create(filePath, TextureFormat::R8G8B8A8, false);		//TODO: GraphicsManager?
}

//------------------------------------------------------------------------------
void TextureBrush::Create(Texture* texture)
{
	m_texture = texture;
}
#endif

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

