
#pragma once
#include "ContentControl.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		ルート要素。
*/
class RootPane
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RootPane);
public:
	static RootPane* Create(GUIManager* manager);

	RootPane(GUIManager* manager);
	virtual ~RootPane();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

	friend class GUIManager;

private:
};


} // namespace GUI
} // namespace Lumino
