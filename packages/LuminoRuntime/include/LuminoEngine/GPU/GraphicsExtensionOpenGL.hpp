// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {
class GraphicsCommandList;
class RenderPass;

class OpenGLIntegration {
public:
    static GraphicsCommandList* getCommandListFromCurrentContext();
    static RenderPass* getRenderPass(GLuint fbo, int32_t width, int32_t height);
};

} // namespace ln
