#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {
class OpenGLDevice;

class GLGraphicsContext
    : public ICommandList {
public:
    GLGraphicsContext();
    Result init(OpenGLDevice* owner);
    void dispose();
    void setActiveShaderPass(GLShaderPass* pass);
    GLuint fbo() const { return m_fbo; }

protected:
    void onSaveExternalRenderState() override;
    void onRestoreExternalRenderState() override;
    void onBeginCommandRecoding() override {}
    void onEndCommandRecoding() override {}
    void onBeginRenderPass(IRenderPass* renderPass) override;
    void onEndRenderPass(IRenderPass* renderPass) override;
    void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
    void onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) override;
    void onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
    void onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
    void onDispatch(const GraphicsContextState& state, IPipeline* pipeline, int groupCountX, int groupCountY, int groupCountZ) override;
    void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) override;
    void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
    void onDrawExtension(INativeGraphicsExtension* extension) override { LN_NOTIMPLEMENTED(); }

    void wait() override;

private:
    static void getPrimitiveInfo(PrimitiveTopology primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

    OpenGLDevice* m_device;
    GLuint m_vao; // https://www.khronos.org/opengl/wiki/Vertex_Specification#Index_buffers
    GLuint m_fbo;
    GLIndexBuffer* m_currentIndexBuffer;
    GLRenderPass* m_currentRenderPass = nullptr;
    GLShaderPass* m_activeShaderPass;
    GLPipeline* m_pipeline = nullptr;

    struct {
        GLboolean m_GL_BLEND;
        GLboolean m_GL_CULL_FACE;
        GLboolean m_GL_DEPTH_TEST;
        GLboolean m_GL_DEPTH_WRITEMASK;
        GLint m_GL_DEPTH_FUNC;
        GLint m_GL_CULL_FACE_MODE;
        GLint m_GL_BLEND_SRC_RGB;
        GLint m_GL_BLEND_DST_RGB;
        GLint m_GL_BLEND_SRC_ALPHA;
        GLint m_GL_BLEND_DST_ALPHA;
        GLint m_GL_BLEND_EQUATION;
        GLint m_GL_ARRAY_BUFFER_BINDING;
        GLint m_GL_ELEMENT_ARRAY_BUFFER_BINDING;
        GLint m_GL_CURRENT_PROGRAM;
        GLint m_vao;
        GLint m_drawFBO;
    } m_savedState;
};

} // namespace detail
} // namespace ln
