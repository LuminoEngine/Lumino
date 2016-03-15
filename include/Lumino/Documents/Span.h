
#pragma once
#include "Inline.h"

LN_NAMESPACE_BEGIN
namespace Documents
{

/**
	@brief
*/
class Span
	: public Inline
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Span(DocumentsManager* manager);
	virtual ~Span();

private:
	tr::ReflectionObjectList<Inline*>	m_inlineList;
};


} // namespace Documents
LN_NAMESPACE_END
