
#include "Internal.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "PlatformManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PlatformEventArgs

PlatformEventArgs PlatformEventArgs::makeClosingEvent(PlatformWindow* sender)
{
	return PlatformEventArgs(PlatformEventType::close, sender);
}

PlatformEventArgs PlatformEventArgs::makeWindowSizeChangedEvent(PlatformWindow* sender, int width, int height)
{
	PlatformEventArgs e(PlatformEventType::WindowSizeChanged, sender);
	e.size.width = width;
	e.size.height = height;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeActivateChangedEvent(PlatformWindow* sender, bool active)
{
	PlatformEventArgs e;
	e.sender = sender;
	e.type = (active) ? PlatformEventType::WindowActivate : PlatformEventType::WindowDeactivate;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeKeyEvent(PlatformWindow* sender, PlatformEventType type, Keys keyCode, ModifierKeys modifierKeys, char keyChar)
{
	PlatformEventArgs e;
	e.type = type;
	e.sender = sender;
	e.key.keyCode = keyCode;
	e.key.modifierKeys = modifierKeys;
	e.key.keyChar = keyChar;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeMouseWheelEvent(PlatformWindow* sender, int delta)
{
	PlatformEventArgs e;
	e.type = PlatformEventType::MouseWheel;
	e.sender = sender;
	e.wheel.delta = delta;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeDragDropEvent(PlatformWindow* sender, PlatformEventType type, DataObject* data, DragDropEffects* effect)
{
	PlatformEventArgs e;
	e.type = type;
	e.sender = sender;
	e.dragDrop.data = data;
	e.dragDrop.effect = effect;
	return e;
}

} // namespace detail
} // namespace ln

