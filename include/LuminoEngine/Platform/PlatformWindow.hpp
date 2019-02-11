
#pragma once
#include "PlatformEvent.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
namespace detail {	// TODO: remove

class PlatformWindow
	: public RefObject
{
public:
	PlatformWindow();
	virtual ~PlatformWindow() = default;
	virtual void dispose() = 0;

    virtual void setWindowTitle(const String& title) = 0;
	virtual void getSize(SizeI* size) = 0;
	virtual void getFramebufferSize(int* width, int* height) = 0;

	virtual PointI pointFromScreen(const PointI& screenPoint) = 0;
	virtual PointI pointToScreen(const PointI& clientPoint) = 0;

	void attachEventListener(IPlatforEventListener* listener);
	void detachEventListener(IPlatforEventListener* listener);
	bool sendEventToAllListener(const PlatformEventArgs& e);	// return : isHandled

private:
	List<IPlatforEventListener*> m_eventListeners;
};

} // namespace detail
} // namespace ln
