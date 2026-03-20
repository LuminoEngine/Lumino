#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Common.hpp>
#include "Common.hpp"
#include <LuminoEngine/Graphics/GPU/ShaderInterfaceFramework.hpp>

namespace ln {
class ShaderTechnique;
class RenderFeature_deprecated;
class Material;

namespace detail {
class RenderingManager;
struct CameraInfo;
struct ElementInfo;
struct SubsetInfo;
class RenderFeatureBatchList;
class RenderDrawElement;
class RenderStage;
class SkeletonInstance;
class RenderFeatureBatch;


} // namespace detail

class RenderFeature_deprecated
	: public Object
{
public:
	virtual void beginRendering() {}
	//virtual void endRendering() {}

    virtual void onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo);

	virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) = 0;
    virtual void renderBatch(GraphicsCommandList* context, detail::RenderFeatureBatch* batch) = 0;

    // 主に PrimitiveRenderFeature など、バッチ描画を行うクラスのために用意したプロパティ。
    // true を返すようにオーバーライドすると、SceneRenderer は DrawElement が持っている Transform を ElementInfo に渡さなくなる。(単位行列になる)
    virtual bool drawElementTransformNegate() const;
    // TODO: ↑は↓に統合していい気がする


private:
};

} // namespace ln

