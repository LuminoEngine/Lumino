
#include "PathShapeContext.hpp"

namespace ln {
namespace detail {
	
std::unique_ptr<PathShape_NVGcontext> PathShape_NVGcontext::nvgCreate(int flags)
{
    NVGparams params;
    NVGcontext* ctx = NULL;
    auto gl = std::make_unique<PathShape_NVGcontext>();
    if (gl == NULL) goto error;
    gl->clear();

    gl->makeNVGparams(&params);
    params.renderCreate = glnvg__renderCreate;
    params.renderCreateTexture = glnvg__renderCreateTexture;
    params.renderDeleteTexture = glnvg__renderDeleteTexture;
    params.renderUpdateTexture = glnvg__renderUpdateTexture;
    params.renderGetTextureSize = glnvg__renderGetTextureSize;
    params.renderFlush = glnvg__renderFlush;
    params.renderDelete = glnvg__renderDelete;
    params.userPtr = gl.get();
    params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

    gl->flags = flags;

    ctx = nvgCreateInternal(&params);
    if (ctx == NULL) goto error;

    gl->m_nvgContext = ctx;

    return gl;

error:
    // 'gl' is freed by nvgDeleteInternal.
    if (ctx != NULL) nvgDeleteInternal(ctx);
    return NULL;
}

} // namespace detail
} // namespace ln


