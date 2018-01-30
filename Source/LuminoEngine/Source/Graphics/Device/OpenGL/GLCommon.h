
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

#define LN_CHECK_GLERROR()		{ GLenum lnglerr = glGetError(); LN_THROW(lnglerr == GL_NO_ERROR , OpenGLException, lnglerr); } 

LN_NAMESPACE_BEGIN
namespace detail
{
class GLHelper
{
public:
	static String getErrorMessage(GLenum glerror);
};

} // namespace detail
LN_NAMESPACE_END

#define LN_ENSURE_GLERROR(expr, err)	for (GLenum lnglerr = glGetError(); LN_ENSURE(lnglerr == GL_NO_ERROR); lnglerr = GL_NO_ERROR)

