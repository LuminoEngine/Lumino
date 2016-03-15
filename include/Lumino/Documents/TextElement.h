
#pragma once
#include "../Property.h"
#include "../Graphics/GraphicsManager.h"
#include "DocumentsRenderer.h"

LN_NAMESPACE_BEGIN
namespace Documents
{
class DocumentsManager;

/**
	@brief
*/
class TextElement
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
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

	//String	GetFontFamily() const { return GetTypedPropertyValue<String>(FontFamilyProperty); }
	//int		GetFontSize() const { return m_fontData.Size; }
	int		GetFontEdgeSize() const { return m_fontData.EdgeSize; }
	//bool	IsFontBold() const { return m_fontData.IsBold; }
	//bool	IsFontItalic() const { return m_fontData.IsItalic; }
	//bool	IsFontAntiAlias() const { return m_fontData.IsAntiAlias; }


	/** フォントファミリ名を設定します。*/
	void SetFontFamily(const String& value) { SetTypedPropertyValue<String>(FontFamilyProperty, value); }

	/** フォントファミリ名を取得します。*/
	String GetFontFamily() const { return GetTypedPropertyValue<String>(FontFamilyProperty); }

	/** フォントサイズを設定します。*/
	void SetFontSize(int value) { SetTypedPropertyValue<int>(FontSizeProperty, value); }

	/** フォントサイズを取得します。*/
	int GetFontSize() const { return GetTypedPropertyValue<int>(FontSizeProperty); }

	/** フォントの太字有無を設定します。*/
	void SetFontBold(bool value) { SetTypedPropertyValue<bool>(IsFontBoldProperty, value); }

	/** フォントの太字有無を取得します。*/
	bool IsFontBold() const { return GetTypedPropertyValue<bool>(IsFontBoldProperty); }

	/** フォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { SetTypedPropertyValue<bool>(IsFontItalicProperty, value); }

	/** フォントのイタリック体有無を取得します。*/
	bool IsFontItalic() const { return GetTypedPropertyValue<bool>(IsFontItalicProperty); }

	/** フォントのアンチエイリアス有無を設定します。*/
	void SetFontAntiAlias(bool value) { SetTypedPropertyValue<bool>(IsFontAntiAliasProperty, value); }

	/** フォントのアンチエイリアス有無を取得します。*/
	bool IsFontAntiAlias() const { return GetTypedPropertyValue<bool>(IsFontAntiAliasProperty); }

	virtual Size Measure();
	virtual void Render(DocumentsRenderer* renderer);

protected:
	void UpdateFontData();

private:
	friend class DocumentsHelper;
	TextElement*			m_parent;

LN_INTERNAL_ACCESS:
	void SetFontData(const GraphicsManager::FontData& fontData);

protected:
	DocumentsManager* m_manager;
	GraphicsManager::FontData	m_fontData;
	bool	m_fontDataModified;
};


} // namespace Documents
LN_NAMESPACE_END
