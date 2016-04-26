
#pragma once 

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

class GLContext
	: public RefObject
{
public:
	GLContext();
	virtual ~GLContext();
	virtual void SwapBuffers() = 0;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
