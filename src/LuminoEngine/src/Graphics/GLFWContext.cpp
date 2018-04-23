
#include "Internal.hpp"
#include "OpenGLDeviceContext.hpp"

#define LN_MACRO_BLOCK_BEGIN for(;;) {
#define LN_MACRO_BLOCK_END break; }

#define GL_CHECK(call) \
	LN_MACRO_BLOCK_BEGIN \
		call; \
		GLenum gl_err = glGetError(); \
		LN_ENSURE(0 == gl_err, #call "; GL error 0x%x: %s", gl_err, OpenGLHelper::glEnumName(gl_err) ); \
	LN_MACRO_BLOCK_END

namespace ln {
namespace detail {

class OpenGLHelper
{
public:
	static const char* glEnumName(GLenum _enum)
	{
#define GLENUM(_ty) case _ty: return #_ty

		switch (_enum)
		{
			GLENUM(GL_TEXTURE);
			//GLENUM(GL_RENDERBUFFER);

			GLENUM(GL_INVALID_ENUM);
			//GLENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
			GLENUM(GL_INVALID_VALUE);
			GLENUM(GL_INVALID_OPERATION);
			GLENUM(GL_OUT_OF_MEMORY);

			//GLENUM(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
			//GLENUM(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
			//			GLENUM(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
			//			GLENUM(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
			//GLENUM(GL_FRAMEBUFFER_UNSUPPORTED);
		}

#undef GLENUM
		return "<GLenum?>";
	}
};


OpenGLDeviceContext::OpenGLDeviceContext()
{
}

void OpenGLDeviceContext::initialize()
{
}

void OpenGLDeviceContext::dispose()
{
}

void OpenGLDeviceContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	GLuint glflags = 0;

	if (testFlag(flags, ClearFlags::Color))
	{
		GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
		glflags |= GL_COLOR_BUFFER_BIT;
	}

	if (testFlag(flags, ClearFlags::Depth))
	{
		GL_CHECK(glDepthMask(GL_TRUE));
		GL_CHECK(glClearDepth(z););
		glflags |= GL_DEPTH_BUFFER_BIT;
	}

	if (testFlag(flags, ClearFlags::Stencil))
	{
		GL_CHECK(glClearStencil(stencil));
		glflags |= GL_STENCIL_BUFFER_BIT;
	}

	GL_CHECK(glClear(glflags));
}

} // namespace detail
} // namespace ln

