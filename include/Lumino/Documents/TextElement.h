
#pragma once
#include "../Property.h"
#include "../Graphics/GraphicsManager.h"
#include "DocumentsRenderer.h"

namespace Lumino
{
namespace Documents
{
class DocumentsManager;

/**
	@brief
*/
class TextElement
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	LN_PROPERTY(String, FontFamilyProperty);
	LN_PROPERTY(int,	FontSizeProperty);
	LN_PROPERTY(int,	FontEdgeSizeProperty);	// TODO: Pen に移したい
	LN_PROPERTY(bool,	IsFontBoldProperty);
	LN_PROPERTY(bool,	IsFontItalicProperty);
	LN_PROPERTY(bool,	IsFontAntiAliasProperty);


public:
	TextElement(DocumentsManager* manager);
	virtual ~TextElement();

public:

	//const String& GetFontFamily() const { return m_fontData.Family; }

	String	GetFontFamily() const { return GetTypedPropertyValue<String>(FontFamilyProperty); }
	int		GetFontSize() const { return m_fontData.Size; }
	int		GetFontEdgeSize() const { return m_fontData.EdgeSize; }
	bool	IsFontBold() const { return m_fontData.IsBold; }
	bool	IsFontItalic() const { return m_fontData.IsItalic; }
	bool	IsFontAntiAlias() const { return m_fontData.IsAntiAlias; }

	virtual Size Measure();
	virtual void Render(DocumentsRenderer* renderer);

protected:
	void UpdateFontData();

private:
	friend class Helper;
	TextElement*			m_parent;

protected:
	DocumentsManager* m_manager;
	Graphics::GraphicsManager::FontData	m_fontData;
	bool	m_fontDataModified;
};


} // namespace Documents
} // namespace Lumino
