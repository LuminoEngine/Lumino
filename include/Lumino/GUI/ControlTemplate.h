
#pragma once
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
	@note	WPF の FrameworkElementFactory に相当する。
*/
class UIElementFactory
	: public CoreObject
{
public:
	UIElementFactory();
	virtual ~UIElementFactory();
};

/**
	@brief		
	@note	ContentPresenter が必要。
*/
class ControlTemplate
	: public CoreObject
{
public:
	ControlTemplate();
	virtual ~ControlTemplate();

private:
	String				m_targetType;		///< 対象コントロール名 ("Button" 等)
	UIElementFactory*	m_visualTreeRoot;	///< テンプレートの VisualTree のルートノード
};


	

} // namespace GUI
} // namespace Lumino
