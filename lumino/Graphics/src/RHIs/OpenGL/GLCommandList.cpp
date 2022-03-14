﻿#include "Internal.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLBuffer.hpp"
#include "GLTextures.hpp"
#include "GLShaderPass.hpp"
#include "GLCommandList.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GLGraphicsContext

GLGraphicsContext::GLGraphicsContext()
    : m_device(nullptr)
    , m_vao(0)
    , m_fbo(0)
    , m_currentIndexBuffer(nullptr)
    , m_activeShaderPass(nullptr) {
}

Result GLGraphicsContext::init(OpenGLDevice* owner) {
    LN_CHECK(owner);
    ICommandList::init(owner);
    m_device = owner;

    GL_CHECK(glGenVertexArrays(1, &m_vao));
    GL_CHECK(glGenFramebuffers(1, &m_fbo));

    return ok();
}

void GLGraphicsContext::dispose() {
    if (m_vao) {
        GL_CHECK(glBindVertexArray(0));
        GL_CHECK(glDeleteVertexArrays(1, &m_vao));
        m_vao = 0;
    }
    if (m_fbo != 0) {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
        m_fbo = 0;
    }

    ICommandList::dispose();
}

void GLGraphicsContext::setActiveShaderPass(GLShaderPass* pass) {
    if (m_activeShaderPass != pass) {
        m_activeShaderPass = pass;
        ::glUseProgram((m_activeShaderPass) ? m_activeShaderPass->program() : 0);
    }
}

void GLGraphicsContext::onSaveExternalRenderState() {
    GL_CHECK(glGetBooleanv(GL_CULL_FACE, &m_savedState.state_GL_CULL_FACE));
}

void GLGraphicsContext::onRestoreExternalRenderState() {
    if (m_savedState.state_GL_CULL_FACE) {
        GL_CHECK(glEnable(GL_CULL_FACE));
    }
    else {
        GL_CHECK(glDisable(GL_CULL_FACE));
    }

    GL_CHECK(glBindVertexArray(0));

    setActiveShaderPass(nullptr);
}

void GLGraphicsContext::onBeginRenderPass(IRenderPass* renderPass) {
    m_currentRenderPass = static_cast<GLRenderPass*>(renderPass);
    m_currentRenderPass->bind(this);
}

void GLGraphicsContext::onEndRenderPass(IRenderPass* renderPass) {
}

void GLGraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) {
    // UpdateRegionRects
    if (stateDirtyFlags & GraphicsContextStateDirtyFlags_RegionRects) {
        const RectI& viewportRect = state.regionRects.viewportRect;
        const RectI& scissorRect = state.regionRects.scissorRect;
        const RHIExtent2D targetSize = m_currentRenderPass->viewSize();

        GL_CHECK(glViewport(viewportRect.x, targetSize.height - (viewportRect.y + viewportRect.height), viewportRect.width, viewportRect.height));
        GL_CHECK(glEnable(GL_SCISSOR_TEST));
        GL_CHECK(glScissor(scissorRect.x, targetSize.height - (scissorRect.y + scissorRect.height), scissorRect.width, scissorRect.height));
    }

    // Update primitive data
    {
        if (m_vao) {
            GL_CHECK(glBindVertexArray(m_vao));
        }

        m_currentIndexBuffer = static_cast<GLIndexBuffer*>(state.primitive.indexBuffer);
    }

    if (pipeline) {
        auto* glPipeline = static_cast<GLPipeline*>(pipeline);
        glPipeline->bind(state.primitive.vertexBuffers, state.primitive.indexBuffer, state.descriptor);
    }

}

//void* GLGraphicsContext::onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size)
//{
//	switch (resource->resourceType())
//	{
//	case DeviceResourceType::VertexBuffer:
//		return static_cast<GLVertexBuffer*>(resource)->map(offset, size);
//	case DeviceResourceType::IndexBuffer:
//		return static_cast<GLIndexBuffer*>(resource)->map(offset, size);
//	default:
//		LN_NOTIMPLEMENTED();
//		return nullptr;
//	}
//}
//
//void GLGraphicsContext::onUnmapResource(IGraphicsRHIBuffer* resource)
//{
//	switch (resource->resourceType())
//	{
//	case DeviceResourceType::VertexBuffer:
//		static_cast<GLVertexBuffer*>(resource)->unmap();
//		break;
//	case DeviceResourceType::IndexBuffer:
//		static_cast<GLIndexBuffer*>(resource)->unmap();
//		break;
//	default:
//		LN_NOTIMPLEMENTED();
//		break;
//	}
//}

void GLGraphicsContext::onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) {
    //switch (resource->resourceType()) {
    //    case DeviceResourceType::VertexBuffer:
    //        static_cast<GLVertexBuffer*>(resource)->setSubData(offset, data, length);
    //        break;
    //    case DeviceResourceType::IndexBuffer:
    //        static_cast<GLIndexBuffer*>(resource)->setSubData(offset, data, length);
    //        break;
    //    default:
    //        LN_NOTIMPLEMENTED();
    //        break;
    //}
    LN_NOTIMPLEMENTED();
}

void GLGraphicsContext::onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) {
    static_cast<GLTextureBase*>(resource)->setSubData(x, y, width, height, data, dataSize);
}

void GLGraphicsContext::onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    static_cast<GLTextureBase*>(resource)->setSubData3D(x, y, z, width, height, depth, data, dataSize);
}

void GLGraphicsContext::onDispatch(const GraphicsContextState& state, IPipeline* pipeline, int groupCountX, int groupCountY, int groupCountZ) {
    LN_NOTIMPLEMENTED();
}

//void GLGraphicsContext::onSetDescriptorTableData(IShaderDescriptorTable* resource, const ShaderDescriptorTableUpdateInfo* data) {
//    GLShaderDescriptorTable* table = static_cast<GLShaderDescriptorTable*>(resource);
//    table->setData(data);
//}

void GLGraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    //std::array<GLenum, MaxMultiRenderTargets> buffers = {
    //	GL_COLOR_ATTACHMENT0, GL_NONE, GL_NONE, GL_NONE
    //};
    //GL_CHECK(glDrawBuffers(buffers.size(), buffers.data()));

    OpenGLHelper::clearBuffers(flags, color, z, stencil);
}

void GLGraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) {
    GLenum gl_prim;
    int vertexCount;
    getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

    GL_CHECK(glDrawArrays(gl_prim, startVertex, vertexCount));
}

void GLGraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    if (vertexOffset != 0) {
        LN_NOTIMPLEMENTED();
        return;
    }

    GLenum gl_prim;
    int vertexCount;
    getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

    GLenum indexFormat = 0;
    GLvoid* startIndexPtr = nullptr;
    if (m_currentIndexBuffer->format() == IndexBufferFormat::UInt16) {
        indexFormat = GL_UNSIGNED_SHORT;
        startIndexPtr = (GLvoid*)(sizeof(GLushort) * startIndex);
    }
    else if (m_currentIndexBuffer->format() == IndexBufferFormat::UInt32) {
        indexFormat = GL_UNSIGNED_INT;
        startIndexPtr = (GLvoid*)(sizeof(GLuint) * startIndex);
    }
    else {
        LN_UNREACHABLE();
        return;
    }

    // 引数 start end には、本来であれば0～vertexCountまでのインデックスの中の最大、最小の値を渡す。
    // http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
    // ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
    // TODO: ↑Radeon で稀に吹っ飛ぶ

    if (instanceCount > 0) {
        GL_CHECK(glDrawElementsInstanced(gl_prim, vertexCount, indexFormat, startIndexPtr, instanceCount));
    }
    else {
        GL_CHECK(glDrawElements(gl_prim, vertexCount, indexFormat, startIndexPtr));
    }
}

void GLGraphicsContext::wait() {
}

void GLGraphicsContext::getPrimitiveInfo(PrimitiveTopology primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount) {
    switch (primitive) {
        case PrimitiveTopology::TriangleList:
            *gl_prim = GL_TRIANGLES;
            *vertexCount = primitiveCount * 3;
            break;
        case PrimitiveTopology::TriangleStrip:
            *gl_prim = GL_TRIANGLE_STRIP;
            *vertexCount = 2 + primitiveCount;
            break;
        case PrimitiveTopology::TriangleFan:
            *gl_prim = GL_TRIANGLE_FAN;
            *vertexCount = 2 + primitiveCount;
            break;
        case PrimitiveTopology::LineList:
            *gl_prim = GL_LINES;
            *vertexCount = primitiveCount * 2;
            break;
        case PrimitiveTopology::LineStrip:
            *gl_prim = GL_LINE_STRIP;
            *vertexCount = 1 + primitiveCount;
            break;
        case PrimitiveTopology::PointList:
            *gl_prim = GL_POINTS;
            *vertexCount = primitiveCount;
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

} // namespace detail
} // namespace ln