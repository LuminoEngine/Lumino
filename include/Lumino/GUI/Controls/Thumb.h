
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
	@note		Thumb はマウスドラッグによる移動量を通知するためのコントロール。
				Thumb 自体の位置を移動するものではない点に注意。
				ScrollBar はもちろん、GridSpritter も Thumb のサブクラス。
*/
class Thumb
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Thumb);
public:
	Thumb(GUIManager* manager);
	virtual ~Thumb();
};

} // namespace GUI
} // namespace Lumino
