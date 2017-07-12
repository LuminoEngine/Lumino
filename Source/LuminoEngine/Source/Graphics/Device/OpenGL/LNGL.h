
#pragma once

//#include <GL/gl.h>

#if defined(__MINGW32__) || defined(__CYGWIN__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
	#define LNGL_API __stdcall
#else
	#define LNGL_API
#endif

class LNGL
{
public:
	typedef void (LNGL_API * pf_glGenFramebuffers) (GLsizei n, GLuint* framebuffers);

public:

	bool init();

	static pf_glGenFramebuffers glGenFramebuffers;
};
