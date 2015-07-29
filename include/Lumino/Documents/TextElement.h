
#pragma once
#include "../Property.h"
#include "../Imaging/Font.h"
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
	LN_PROPERTY_BEGIN;
	LN_PROPERTY(String, FontFamily);
	LN_PROPERTY(int,	FontSize);
	LN_PROPERTY(int,	FontEdgeSize);	// TODO: Pen ‚ÉˆÚ‚µ‚½‚¢
	LN_PROPERTY(bool,	IsFontBold);
	LN_PROPERTY(bool,	IsFontItalic);
	LN_PROPERTY(bool,	IsFontAntiAlias);
	LN_PROPERTY_END;


public:
	TextElement(DocumentsManager* manager);
	virtual ~TextElement();

public:
	//Imaging::Font* GetFont() const;

	//const String& GetFontFamily() const { return m_fontData.Family; }

	const String& GetFontFamily() const { return GetTypedPropertyValue<String>(Properties::FontFamily); }
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
	//RefPtr<Imaging::Font>	m_font;
	bool	m_fontDataModified;
};


} // namespace Documents
} // namespace Lumino
