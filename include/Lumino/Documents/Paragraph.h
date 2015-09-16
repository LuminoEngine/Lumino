
#pragma once
#include "../Graphics/Common.h"
#include "../CoreObject.h"
#include "TextElement.h"
#include "Inline.h"

namespace Lumino
{
namespace Documents
{
class Paragraph;

class InlineList
	: public GenericCoreList<Inline*>
{
public:
	typedef GenericCoreList<Inline*>::value_type value_type;

private:
	InlineList(Paragraph* owner) : m_owner(owner) {}

	virtual void InsertItem(int index, const value_type& item);
	virtual void ClearItems();
	virtual void RemoveItem(int index);
	virtual void SetItem(int index, const value_type& item);

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

	FlowDirection GetFlowDirection() { return FlowDirection::LeftToRight; }

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
