
#pragma once
#include "../Graphics/TextRenderer.h"
#include "../Graphics/GlyphRun.h"
#include "Inline.h"

namespace Lumino
{
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

private:
	GenericStringBuilderCore<UTF32>	m_text;
	RefPtr<Graphics::GlyphRun>		m_glyphRun;
};


} // namespace Documents
} // namespace Lumino
