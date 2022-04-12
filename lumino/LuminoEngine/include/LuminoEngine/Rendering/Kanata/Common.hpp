#pragma once
#include <LuminoGraphics/Common.hpp>
#include "../Common.hpp"

namespace ln {
class Material;

namespace kanata {
struct BatchElement;
struct Batch;
class BatchProxy;
class SingleFrameBatchProxy;
class BatchCollector;
class DrawCommandList;
class RenderView;
class RenderingContext;
class SceneRenderPass;

class ScreenRectangleRenderFeature;
class PrimitiveMeshRenderer;
class MeshRenderFeature;
} // namespace kanata

namespace detail {
class RenderingManager;
class SkeletonInstance;
class MeshGenerater;
} // namespace detail
} // namespace ln

