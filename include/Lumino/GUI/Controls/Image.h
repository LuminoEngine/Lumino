
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
	static const Property*	SourceProperty;
	static const Property*	SourceRectProperty;

public:
	static Image* Create(GUIManager* manager);

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
	virtual void OnRender();

private:
	void UpdateInternalSourceRect();

private:
	RefPtr<Graphics::Texture>	m_texture;
	Rect						m_srcRect;
	Stretch						m_stretch;
	Rect						m_internalSrcRect;
};


} // namespace GUI
} // namespace Lumino
