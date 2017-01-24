
#pragma once
#include <Lumino/GUI/VisualState.h>

namespace Lumino
{
namespace GUI
{

/// データクラスである VisualStateManager、VisualStateGroup、VisualState に対する個別の UIElement が管理する情報。
class VisualStateInstance
{
public:
	VisualStateInstance() {}
	~VisualStateInstance() {}

	/// VisualStateManager を変更する
	void SetVisualStateManager(VisualStateManager* manager);

private:
	RefPtr<VisualStateManager>	m_manager;
};


} // namespace GUI
} // namespace Lumino
