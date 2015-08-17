
#pragma once
#include "ContentControl.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief	GUI 要素のルート要素です。
*/
class RootFrame
	: public ContentControl
{
public:

	/**
		@brief	RootFrame を作成します。
	*/
	static RootFrame* Create();

private:
	RootFrame(GUIManager* manager);
	virtual ~RootFrame();

	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RootFrame);
};


} // namespace GUI
} // namespace Lumino
