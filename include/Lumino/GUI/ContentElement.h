
#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

class ContentElement
	: public CoreObject
{
public:
	ContentElement(GUIManager* manager);
	virtual ~ContentElement();

private:
	GUIManager*	m_manager;
};


} // namespace GUI
} // namespace Lumino
