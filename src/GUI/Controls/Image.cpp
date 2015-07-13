
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Image.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// Image
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Image, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Image);

//LN_DEFINE_PROPERTY_2(Image, const Variant&, SourceProperty, "Source", Variant::Null, &Image::SetSource, NULL);
LN_DEFINE_PROPERTY_2(Image, Rect, SourceRectProperty, "SourceRect", Rect(), &Image::SetSourceRect, &Image::GetSourceRect);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::Image(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::~Image()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSource(const Variant& value)
{
	if (value.GetType() == VariantType_Object) 
	{
		auto tex = dynamic_cast<Graphics::Texture*>(value.GetObject());
		if (tex != NULL)
		{
			SetSource(tex);
			return;
		}
	}

	LN_VERIFY(0, ArgumentException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSourceTexture(Graphics::Texture* texture)
{
	m_texture = texture;
	UpdateInternalSourceRect();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSourceRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateInternalSourceRect();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::UpdateInternalSourceRect()
{
	if (m_texture != NULL)
	{
		if (m_srcRect.IsEmpty())
		{
			// 転送元矩形が指定されていなければテクスチャサイズを使用する
			const Size& size = m_texture->GetSize();
			m_internalSrcRect.Set(0, 0, size.Width, size.Height);
		}
		else {
			m_internalSrcRect = m_srcRect;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Image::MeasureOverride(const SizeF& availableSize)
{
	SizeF desiredSize = UIElement::MeasureOverride(availableSize);

	if (m_stretch == Stretch::None)
	{
		// テクスチャ転送サイズをコントロールの最小サイズにする
		desiredSize.Width = std::max(desiredSize.Width, (float)m_internalSrcRect.Width);
		desiredSize.Height = std::max(desiredSize.Height, (float)m_internalSrcRect.Height);
	}
	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Image::ArrangeOverride(const SizeF& finalSize)
{
	SizeF size = UIElement::ArrangeOverride(finalSize);
	if (m_stretch == Stretch::None)
	{
		size.Width = m_internalSrcRect.Width;
		size.Height = m_internalSrcRect.Height;
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::OnRender()
{
	if (m_texture == NULL) { return; }

	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO



	painter.DrawTexture(m_finalGlobalRect, m_texture, m_internalSrcRect);
}

} // namespace GUI
} // namespace Lumino

