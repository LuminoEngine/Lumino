
#pragma once
#include "ContentControl.h"
#include "ButtonChrome.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class TextBox
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(TextBox);
public:
	LN_PROPERTY(String, TextProperty);

public:

	/**
		@brief	TextBox ÇçÏê¨ÇµÇ‹Ç∑ÅB
	*/
	static TextBox* Create(GUIManager* manager = NULL);

public:

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** */
	void SetText(const String& value) { SetTypedPropertyValue<String>(TextProperty, value); }

	/** */
	String GetText() const { return GetTypedPropertyValue<String>(TextProperty); }

	/** @} */

protected:
	TextBox(GUIManager* manager);
	virtual ~TextBox();
	virtual void OnRender(Graphics::Painter* painter);

private:
	void set_Text(const String& string);
	const String& get_Text() const;

private:
	//RefPtr<Documents::Document>	m_document;
};

} // namespace GUI
} // namespace Lumino
