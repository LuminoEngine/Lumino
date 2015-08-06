
#pragma once
#include "../Graphics/Common.h"
#include "TextElement.h"
#include "Inline.h"

namespace Lumino
{
namespace Documents
{
class Paragraph;

class InlineList
	: public GenericVariantList<Inline*>
{
private:
	InlineList(Paragraph* owner) : m_owner(owner) {}

	virtual void OnItemAdded(const Variant& item);
	virtual void OnItemRemoved(const Variant& item);

	friend class Paragraph;
	Paragraph*	m_owner;
};

/**
	@brief
*/
class Paragraph
	: public TextElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Paragraph(DocumentsManager* manager);
	virtual ~Paragraph();

	Graphics::FlowDirection GetFlowDirection() { return Graphics::FlowDirection::LeftToRight; }

	InlineList* GetInlines() const { return m_inlines; }

	virtual Size Measure();
	virtual void Render(DocumentsRenderer* renderer);
private:
	void OnInlineAdded(Inline* item);
	void OnInlineRemoved(Inline* item);

private:
	friend class InlineList;
	RefPtr<InlineList>	m_inlines;
};


} // namespace Documents
} // namespace Lumino
