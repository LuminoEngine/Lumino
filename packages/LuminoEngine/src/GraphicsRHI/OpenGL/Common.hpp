#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>

#ifdef LN_EMSCRIPTEN
#define LN_GRAPHICS_OPENGLES
#endif

#if defined(LN_EMSCRIPTEN)
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GL/gl.h>

//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>

#elif defined(LN_GRAPHICS_OPENGLES)
#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>
//#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#define GL_GLES_PROTOTYPES

#else
//#include <GL/gl.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>
//#include <glad/glad.h>
#endif

#else
#include <glad/glad.h>
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif // GL_TEXTURE_MAX_ANISOTROPY_EXT

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif // GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT

#ifdef LN_GRAPHICS_OPENGLES
#define glClearDepth glClearDepthf
#endif

#define GL_CHECK(call) \
    for (;;) { \
        call; \
        GLenum gl_err = glGetError(); \
        if (gl_err != 0) { \
            LN_ERROR(#call "; GL error {:#x}: {}", gl_err, ::ln::detail::OpenGLHelper::glEnumName(gl_err)); \
        } \
        break; \
    }
