// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include <LuminoEngine/Base/Promise.hpp>
#include <LuminoEngine/GraphicsRHI/Common.hpp>
#include <LuminoEngine/Bitmap/Common.hpp>

namespace ln {
class IGraphicsResource;
class GraphicsCommandList;
class SamplerState;
class SwapChain;
class RenderPass;
class Bitmap2D;
class Bitmap3D;
class Texture;
class Texture2D;
class RenderTargetTexture;
class DepthBuffer;
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class INativeGraphicsExtension;
class ShaderDescriptor;

/**
 * Texture2DDelegate
 */
LN_DELEGATE()
using Texture2DDelegate = Delegate<void(Ref<Texture2D> value)>;

/**
 * Texture2DPromise
 */
LN_PROMISE()
using Texture2DPromise = Promise<Ref<Texture2D>>;

///** 描画方式 */
//enum class RenderingType
//{
//	/** 即時描画 */
//	Immediate,
//
//	/** 遅延描画 */
//	Threaded,
//};

class GraphicsHelper {
public:
    static PixelFormat translateToPixelFormat(TextureFormat format);
    static TextureFormat translateToTextureFormat(PixelFormat format);
    static const Char* CandidateExts_Texture2D[5];
};

namespace detail {
class AssetManager;
class RenderingCommandList;
class GraphicsManager;
class GraphicsContextInternal;
class IGraphicsDevice;
class ICommandList;
class ISamplerState;
class IRenderPass;
class ShaderSecondaryDescriptor;
class GraphicsProfiler;
class RHIResource;
class RenderPassCache;


} // namespace detail
} // namespace ln
