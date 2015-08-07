
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/// コンテンツサイズの計算方法
LN_ENUM(Stretch)
{
	None = 0,
};
LN_ENUM_DECLARE(Stretch);

/**
	@brief
*/
class Image
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Image);
public:
	LN_PROPERTY(Variant,	SourceProperty);		// TODO: OnSourcePropertyChagend。あともしかしたら Converter がほしいかも
	LN_PROPERTY(Rect,		SourceRectProperty);

public:
	static ImagePtr Create();

	Image(GUIManager* manager);
	virtual ~Image();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetSource(const Variant& value);

	void SetSourceRect(const Rect& rect);
	const Rect& GetSourceRect() const { return m_srcRect; }

	// ImageSource Source
	// Stretch Stretch

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	

	/** @} */

public:
	void SetSourceTexture(Graphics::Texture* texture);

protected:
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender(Graphics::Painter* painter);

private:
	void UpdateInternalSourceRect();

private:
	Variant	m_source;

	RefPtr<Graphics::Texture>	m_texture;
	Rect						m_srcRect;
	Stretch						m_stretch;
	Rect						m_internalSrcRect;
};


} // namespace GUI
} // namespace Lumino
