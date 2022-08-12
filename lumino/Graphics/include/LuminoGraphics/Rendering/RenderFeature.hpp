#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/Common.hpp>
#include "Common.hpp"
#include <LuminoGraphics/GPU/ShaderInterfaceFramework.hpp>

namespace ln {
class ShaderTechnique;
class RenderFeature;
class Material;

namespace detail {
class RenderingManager;
struct CameraInfo;
struct ElementInfo;
struct SubsetInfo;
class RenderFeatureBatchList;
class RenderDrawElement;
class RenderStage;
class SceneRendererPass;
class SkeletonInstance;
class RenderFeatureBatch;


} // namespace detail

class RenderFeature
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

    // SceneRenderer で確定し、各 RenderFeature に渡されてくる入力パラメータを処理する。
    // 基本的にこのベースメソッドを呼び出すだけでよい。
    // SpritetextRenderFerture など、RenderFeature 固有のパラメータ (グリフテクスチャなど) で
    // パラメータをオーバーライドしたい場合はここで処理する。
    virtual void updateRenderParameters(
		detail::ShaderSecondaryDescriptor* descriptor,
		detail::RenderDrawElement* element,
		ShaderTechnique* tech,
		const detail::RenderViewInfo& renderViewInfo,
		const detail::SceneInfo& sceneInfo,
		const detail::ElementInfo& elementInfo,
		const detail::SubsetInfo& subsetInfo);

	static void updateRenderParametersDefault(
		detail::ShaderSecondaryDescriptor* descriptor,
		ShaderTechnique* tech,
		const detail::RenderViewInfo& renderViewInfo,
		const detail::SceneInfo& sceneInfo,
		const detail::ElementInfo& elementInfo,
		const detail::SubsetInfo& subsetInfo);

private:
};

} // namespace ln

