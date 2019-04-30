#pragma once

namespace ln {
class GraphicsContext;
class ShaderTechnique;
namespace detail {
struct CameraInfo;
struct ElementInfo;
struct SubsetInfo;
class RenderDrawElement;
}

class RenderFeature
	: public Object
{
public:
    virtual void onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo);
	virtual void flush(GraphicsContext* context) = 0;

    // 主に PrimitiveRenderFeature など、バッチ描画を行うクラスのために用意したプロパティ。
    // true を返すようにオーバーライドすると、SceneRenderer は DrawElement が持っている Transform を ElementInfo に渡さなくなる。(単位行列になる)
    virtual bool drawElementTransformNegate() const;
    // TODO: ↑は↓に統合していい気がする

    // SceneRenderer で確定し、各 RenderFeature に渡されてくる入力パラメータを処理する。
    // 基本的にこのベースメソッドを呼び出すだけでよい。
    // SpritetextRenderFerture など、RenderFeature 固有のパラメータ (グリフテクスチャなど) で
    // パラメータをオーバーライドしたい場合はここで処理する。
    virtual void updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo);

    static void updateRenderParametersDefault(ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo);

private:
};

} // namespace ln

