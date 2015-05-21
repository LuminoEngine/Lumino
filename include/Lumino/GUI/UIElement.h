
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
class Desktop
	: public UIElement
{
public:
	Desktop();
	virtual ~Desktop();

private:
};

} // namespace GUI
} // namespace Lumino
