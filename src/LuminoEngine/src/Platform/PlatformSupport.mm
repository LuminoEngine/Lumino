
#include "Internal.hpp"
#import <Cocoa/Cocoa.h>

namespace ln {
namespace detail {

void Cocoa_getScreenMousePosition(int* x, int* y)
{
	// origin is top left
	CGEventRef ourEvent = CGEventCreate(NULL);
	CGPoint point = CGEventGetLocation(ourEvent);
	CFRelease(ourEvent);
	*x = point.x;
	*y = point.y;
}

void Cocoa_pointFromScreen(void* windowId, int screenPointX, int screenPointY, int* clientPointX, int* clientPointY)
{
	NSRect e = [[NSScreen mainScreen] frame];
	int screenHeight = (int)e.size.height;
	
	NSWindow* wnd = (NSWindow*)windowId;
	NSView* view = [wnd contentView];
	NSRect rect = [view frame];
	
	// native point = origin is bottom left
	NSRect nativeScreenPoint = NSMakeRect(screenPointX, screenHeight - screenPointY, 1, 1);
	NSRect nativeClientPoint = [wnd convertRectFromScreen:nativeScreenPoint];
	*clientPointX = nativeClientPoint.origin.x;
	*clientPointY = ((int)rect.size.height) - nativeClientPoint.origin.y;
}

void Cocoa_pointToScreen(void* windowId, int clientPointX, int clientPointY, int* screenPointX, int* screenPointY)
{
	NSRect e = [[NSScreen mainScreen] frame];
	int screenHeight = (int)e.size.height;
		
	NSWindow* wnd = (NSWindow*)windowId;
	NSView* view = [wnd contentView];
	NSRect rect = [view frame];
	
	// native point = origin is bottom left
	NSPoint nativeClientPoint = NSMakePoint(clientPointX, -clientPointY + rect.size.height);
	NSPoint nativeScreenPoint = [view convertPointFromBacking:nativeClientPoint];
	*screenPointX = nativeScreenPoint.x;
	*screenPointY = -nativeScreenPoint.y + screenHeight;
}

} // namespace detail
} // namespace ln
