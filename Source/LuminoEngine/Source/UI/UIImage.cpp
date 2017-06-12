
#include "Internal.h"
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/UI/UIImage.h>

LN_NAMESPACE_BEGIN


/** コンテンツのサイズを変更する方法を示します。*/
enum class StretchDirection
{
	/** コンテンツが親よりも小さい場合のみ、拡大方向にスケーリングします。 コンテンツの方が親より大きくても、縮小方向にはスケーリングしません。*/
	UpOnly,

	/** コンテンツが親よりも大きい場合のみ、縮小方向にスケーリングします。 コンテンツの方が親より小さくても、拡大方向にはスケーリングしません。*/
	DownOnly,

	/** コンテンツは、StretchType モードに基づいて、親に合わせて伸縮します。*/
	Both,
};

static Vector2 CalcViewBoxScale(
	const Size& viewSize,
	const Size& contentSize,
	StretchMode stretchMode,
	StretchDirection stretchDirection
){
	Vector2 scale(1.0f, 1.0f);

	if (stretchMode != StretchMode::None)
	{
		scale.x = (contentSize.width != 0.0f) ? viewSize.width / contentSize.width : 0.0f;
		scale.y = (contentSize.height != 0.0f) ? viewSize.height / contentSize.height : 0.0f;

		if (stretchMode == StretchMode::Uniform)
		{
			scale.x = scale.y = std::min(scale.x, scale.y);
		}
		else if (stretchMode == StretchMode::UniformToFill)
		{
			scale.x = scale.y = std::max(scale.x, scale.y);
		}

		if (stretchDirection == StretchDirection::UpOnly)
		{
			if (scale.x < 1.0) scale.x = 1.0;
			if (scale.y < 1.0) scale.y = 1.0;
		}
		else if (stretchDirection == StretchDirection::DownOnly)
		{
			if (scale.x > 1.0) scale.x = 1.0;
			if (scale.y > 1.0) scale.y = 1.0;
		}
	}

	return scale;
}

//==============================================================================
// UIImage
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIImage, UIElement)

//------------------------------------------------------------------------------
RefPtr<UIImage> UIImage::create()
{
	return NewObject<UIImage>();
}

//------------------------------------------------------------------------------
RefPtr<UIImage> UIImage::create(const StringRef& filePath)
{
	return NewObject<UIImage>(filePath);
}

//------------------------------------------------------------------------------
UIImage::UIImage()
	: UIElement()
	, m_texture(nullptr)
	, m_stretchMode(StretchMode::None)
{
}

//------------------------------------------------------------------------------
UIImage::~UIImage()
{
}

//------------------------------------------------------------------------------
void UIImage::initialize()
{
	UIElement::initialize();
}

//------------------------------------------------------------------------------
void UIImage::initialize(const StringRef& filePath)
{
	initialize();
	//m_brush->SetTexture(NewObject<Texture2D>(filePath));

	m_texture = NewObject<Texture2D>(filePath);
}

//------------------------------------------------------------------------------
Size UIImage::MeasureOverride(const Size& availableSize)
{
	return MeasureInternal(availableSize);
}

//------------------------------------------------------------------------------
Size UIImage::ArrangeOverride(const Size& finalSize)
{
	return MeasureInternal(finalSize);
}

//------------------------------------------------------------------------------
void UIImage::OnRender(DrawingContext* g)
{
	UIElement::OnRender(g);

	g->DrawTexture(GetFinalGlobalRect(), m_texture, Rect(0, 0, 32, 32));	// TODO:
}

//------------------------------------------------------------------------------
Size UIImage::MeasureInternal(Size contentSize)
{
	if (m_texture == nullptr)
	{
		return Size::Zero;
	}

	Size size = m_texture->getSize().ToFloatSize();

	Vector2 scale = CalcViewBoxScale(
		contentSize,
		contentSize,
		m_stretchMode,
		StretchDirection::Both);

	return Size(scale.x * size.width, scale.y * size.height);
}

LN_NAMESPACE_END
