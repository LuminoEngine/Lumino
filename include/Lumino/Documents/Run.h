
#pragma once
#include "../Graphics/GlyphRun.h"
#include "Inline.h"

LN_NAMESPACE_BEGIN
namespace Documents
{

/**
	@brief
*/
class Run
	: public Inline
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Run(DocumentsManager* manager);
	Run(const String& text, DocumentsManager* manager);
	virtual ~Run();

private:
	void SetText(const String& text);// { m_text = text; }
	//const String& GetText() const { return m_text; }

	// override TextElement
	virtual Size Measure();
	virtual void Render(DocumentsRenderer* renderer);

private:
	GenericStringBuilderCore<UTF32>	m_text;
	RefPtr<GlyphRun>		m_glyphRun;
};


} // namespace Documents
LN_NAMESPACE_END
