
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/UI/UIImage.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIImage
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIImage, UIElement)

//------------------------------------------------------------------------------
RefPtr<UIImage> UIImage::Create()
{
	return NewObject<UIImage>();
}

//------------------------------------------------------------------------------
RefPtr<UIImage> UIImage::Create(const StringRef& filePath)
{
	return NewObject<UIImage>(filePath);
}

//------------------------------------------------------------------------------
UIImage::UIImage()
	: UIElement()
	, m_brush(nullptr)
{
}

//------------------------------------------------------------------------------
UIImage::~UIImage()
{
}

//------------------------------------------------------------------------------
void UIImage::Initialize()
{
	UIElement::Initialize(detail::EngineDomain::GetUIManager());
	m_brush = NewObject<TextureBrush>();
	SetBackground(m_brush);
}

//------------------------------------------------------------------------------
void UIImage::Initialize(const StringRef& filePath)
{
	Initialize();
	m_brush->SetTexture(NewObject<Texture2D>(filePath));
}

LN_NAMESPACE_END
