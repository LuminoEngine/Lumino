#pragma once
#include "../Common.hpp"

namespace ln {

class GraphicsTestHelper {
public:
    // 基本的に 95% くらい一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある
    static bool checkScreenShot(const Path& filePath, GraphicsContext* context, RenderTargetTexture* renderTarget, int passRate = 95, bool save = false);
    static void saveScreenShot(const Path& filePath, GraphicsContext* context, RenderTargetTexture* renderTarget = nullptr);
    static bool equalsScreenShot(const Path& filePath, GraphicsContext* context, RenderTargetTexture* renderTarget, int passRate);
    static bool equalsBitmapFile(Bitmap2D* bmp1, const Path& filePath, int passRate);
    static Ref<Bitmap2D> capture(GraphicsContext* context, RenderTargetTexture* renderTarget);
};

} // namespace ln

#define ASSERT_RENDERTARGET(filePath, context, renderTarget) ASSERT_TRUE(ln::GraphicsTestHelper::checkScreenShot(filePath, context, renderTarget))
#define ASSERT_RENDERTARGET_S(filePath, context, renderTarget) ASSERT_TRUE(ln::GraphicsTestHelper::checkScreenShot(filePath, context, renderTarget, 95, true))
