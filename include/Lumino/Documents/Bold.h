
#pragma once
#include "Span.h"

namespace Lumino
{
namespace Documents
{

/**
	@brief
*/
class Bold
	: public Span
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Bold(DocumentsManager* manager);
	virtual ~Bold();
};


} // namespace Documents
} // namespace Lumino
