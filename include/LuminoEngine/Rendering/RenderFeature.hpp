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

private:
};

} // namespace ln

