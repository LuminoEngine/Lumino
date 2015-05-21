
#pragma once

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class UIElement
	: public CoreObject
{
public:
	UIElement();
	virtual ~UIElement();

private:
};

/**
	@brief		
*/
class Workbench
	: public UIElement
{
public:
	Workbench();
	virtual ~Workbench();

private:
};

} // namespace GUI
} // namespace Lumino
