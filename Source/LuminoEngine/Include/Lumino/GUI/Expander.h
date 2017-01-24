
#pragma once
#include <Lumino/GUI/UIElement.h>

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class Expander
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	static const String ControlTemplateTypeName;	///< "Button"

public:
	Expander(GUIManager* manager);
	virtual ~Expander();

public:

};

} // namespace GUI
} // namespace Lumino
