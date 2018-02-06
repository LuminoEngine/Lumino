
#pragma once

//#ifdef LN_OS_MAC
//#include <OpenGL/gl.h>
//#endif


#define GLEW_STATIC
//#include <GL/gl.h>
#include "../External/GL/glew.h"
#include "../External/GL/glext.h"

#if defined(LN_OS_WIN32)
#include <Objbase.h>
//#include <GL/wglew.h>
#include "../External/GL/wglext.h"
#elif defined(LN_OS_X11)
#include <GL/glx.h>
#include "../External/GL/glxext.h"
#endif

#include "LNGL.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class GLHelper
{
public:
	static String getErrorMessage(GLenum glerror);
	static bool checkEnsureGLError(int line, const char* file);
};

} // namespace detail
LN_NAMESPACE_END

#define LN_ENSURE_GLERROR()	!ln::detail::GLHelper::checkEnsureGLError(__LINE__, __FILE__)//LN_ENSURE(GLenum lnglerr = glGetError() == GL_NO_ERROR)

