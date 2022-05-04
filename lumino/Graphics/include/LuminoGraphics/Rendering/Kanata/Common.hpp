#pragma once
#include <LuminoGraphics/Common.hpp>
#include "../Common.hpp"

namespace ln {
class Material;

namespace kanata {
struct BatchElement;
struct Batch;
struct BatchProxyState;
class BatchProxy;
class SingleFrameBatchProxy;
class BatchProxyCollector;
class BatchCollector;
class DrawCommandList;
struct DrawCommand;
class RenderView;
class RenderingContext;
class SceneRenderPass;

class ScreenRectangleRenderFeature;
class PrimitiveMeshRenderer;
class MeshRenderFeature;
class SpriteRenderFeature;
class ShapesRenderFeature;
class SpriteTextRenderFeature;
class FrameRectRenderFeature;
} // namespace kanata

namespace detail {
class RenderingManager;
class SkeletonInstance;
class MeshGenerater;
} // namespace detail
} // namespace ln

