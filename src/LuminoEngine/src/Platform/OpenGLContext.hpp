#pragma once
#include <LuminoEngine/Platform/Common.hpp>

namespace ln {
namespace detail {

class OpenGLContext
	: public RefObject
{
public:
	//virtual SizeI getBackendBufferSize() const = 0;
	virtual void makeCurrentMain() = 0;
	virtual void makeCurrent(PlatformWindow* window) = 0;

protected:
	OpenGLContext();

private:
};

} // namespace detail
} // namespace ln

