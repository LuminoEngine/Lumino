#pragma once

namespace ln {
class GraphicsContext;
namespace detail {
struct CameraInfo;
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

private:
};

} // namespace ln

