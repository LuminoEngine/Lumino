
#pragma once
#include <LuminoCore.h>
#include "../include/Lumino/Graphics/Common.h"

#define GLEW_STATIC
#include "../external/GL/glew.h"
#include <GL/gl.h>
#include "../external/GL/glext.h"

#if defined(LN_WIN32)
	//#include <GL/wglew.h>
	#include "../external/GL/wglext.h"
	#include <d3d9.h>
	#include <d3dx9.h>
#elif defined(LN_X11)
	#include <GL/glx.h>
	#include "../external/GL/glxext.h"
#endif

#include "Graphics/Device/OpenGL/LNGL.h"
#include <Lumino/Variant.h>
#include <Lumino/Platform/PlatformSupport.h>
#include "Graphics/Device/DeviceInterface.h"
#include "../include/Lumino/Graphics/GraphicsException.h"

// glGetError() でエラーチェック (各 gl～ の後で必ず呼ばないと正しいエラーが取れないので注意)
#define LN_CHECK_GLERROR()		{ GLenum lnglerr = glGetError(); LN_THROW(lnglerr == GL_NO_ERROR , OpenGLException, lnglerr); } 


namespace Lumino
{
namespace Graphics
{
class GraphicsManager;

namespace Internal
{
extern GraphicsManager* Manager;

inline GraphicsManager* SelectManager(GraphicsManager* manager) { return (manager != NULL) ? manager : Manager; }

};

} // namespace Graphics
} // namespace Lumino

#include "ApplicationContext.h"
