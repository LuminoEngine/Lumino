
#pragma once
#include "TextElement.h"

namespace Lumino
{
namespace Documents
{

/**
	@brief
*/
class Inline
	: public TextElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Inline(DocumentsManager* manager);
	virtual ~Inline();
};



} // namespace Documents
} // namespace Lumino
