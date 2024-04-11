// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class OpenGLGraphicsContext : public GraphicsContext {
public:

private:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext() override;
};

} // namespace ln
