
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Brush.h>
#include "Device/GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Brush
//==============================================================================

// 以前は ColorF の static 変数を参照していたが、それだと初期化の順によってはこちらの値がすべて 0,0,0,0 になってしまうことがあった
static Brush g_ColorBrush_White(Color(1.0, 1.0, 1.0, 1.0));
static Brush g_ColorBrush_Black(Color(0.0, 0.0, 0.0, 1.0));
static Brush g_ColorBrush_Gray(Color(0.5, 0.5, 0.5, 1.0));
static Brush g_ColorBrush_Red(Color(1.0, 0.0, 0.0, 1.0));
static Brush g_ColorBrush_Green(Color(0.0, 1.0, 0.0, 1.0));
static Brush g_ColorBrush_Blue(Color(0.0, 0.0, 1.0, 1.0));
static Brush g_ColorBrush_DimGray(Color(0.25, 0.25, 0.25, 1.0));
Brush*	Brush::White = &g_ColorBrush_White;
Brush*	Brush::Black = &g_ColorBrush_Black;
Brush*	Brush::Gray = &g_ColorBrush_Gray;
Brush*	Brush::Red = &g_ColorBrush_Red;
Brush*	Brush::Green = &g_ColorBrush_Green;
Brush*	Brush::Blue = &g_ColorBrush_Blue;
Brush*	Brush::DimGray = &g_ColorBrush_DimGray;

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
void Brush::Initialize()
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
RefPtr<TextureBrush> TextureBrush::Create(Texture* texture, BrushImageDrawMode drawMode, const RectF& sourceRect, const ThicknessF& borderThickness, BrushWrapMode wrapMode)
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
void TextureBrush::Initialize()
{
	Brush::Initialize();
}

//------------------------------------------------------------------------------
void TextureBrush::Initialize(const StringRef& filePath)
{
	Brush::Initialize();
	auto texture = Texture2D::Create(filePath, TextureFormat::R8G8B8A8, false);		//TODO: GraphicsManager?
	SetTexture(texture);
}

//------------------------------------------------------------------------------
void TextureBrush::Initialize(Texture* texture)
{
	Brush::Initialize();
	SetTexture(texture);
}

//------------------------------------------------------------------------------
RectF TextureBrush::GetActualSourceRect() const
{
	Size textureSize(0, 0);
	Texture* texture = GetTexture();
	if (texture != nullptr) textureSize = texture->GetSize().ToFloatSize();

	const RectF& rc = GetSourceRect();
	return RectF(
		(Math::IsNaNOrInf(rc.x)) ? 0.0f : rc.x,
		(Math::IsNaNOrInf(rc.y)) ? 0.0f : rc.y,
		(Math::IsNaNOrInf(rc.width)) ? textureSize.width : rc.width,
		(Math::IsNaNOrInf(rc.height)) ? textureSize.height : rc.height);
}

//------------------------------------------------------------------------------
Size TextureBrush::GetSize() const
{
	return GetActualSourceRect().GetSize();
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

