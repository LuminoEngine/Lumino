#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/Bitmap/Bitmap.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/Testing/GraphicsTestHelper.hpp>

namespace ln {

bool GraphicsTestHelper::checkScreenShot(const Path& filePath, RenderTargetTexture* renderTarget, int passRate, bool save) {
    if (save) {
        saveScreenShot(filePath, renderTarget);
        return true;
    }
    else {
        bool result = equalsScreenShot(filePath, renderTarget, passRate);
        if (!result) {
            saveScreenShot(LN_ASSETFILE("Result/0-latest-failer.png"), renderTarget);
        }
        return result;
    }
}

void GraphicsTestHelper::saveScreenShot(const Path& filePath, RenderTargetTexture* renderTarget) {
    capture(renderTarget)->save(filePath);
}

bool GraphicsTestHelper::equalsScreenShot(const Path& filePath, RenderTargetTexture* renderTarget, int passRate) {
    bool r = equalsBitmapFile(capture(renderTarget), filePath, passRate);
    return r;
}

Ref<Bitmap2D> GraphicsTestHelper::capture(RenderTargetTexture* renderTarget) {
    assert(renderTarget);
    return detail::TextureInternal::readData(renderTarget);
}

static ColorI mixPixels(Bitmap2D* bmp, int x, int y) {
    const ColorI& c = bmp->getPixel32(x, y);
    int r = c.r;
    int g = c.g;
    int b = c.b;
    int a = c.a;
    int count = 1;

    if (y > 0) {
        if (x > 0) {
            const ColorI& c = bmp->getPixel32(x - 1, y - 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
        {
            const ColorI& c = bmp->getPixel32(x, y - 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
        if (x < bmp->width() - 1) {
            const ColorI& c = bmp->getPixel32(x + 1, y - 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
    }
    {
        if (x > 0) {
            const ColorI& c = bmp->getPixel32(x - 1, y);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
        if (x < bmp->width() - 1) {
            const ColorI& c = bmp->getPixel32(x + 1, y);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
    }
    if (y < bmp->height() - 1) {
        if (x > 0) {
            const ColorI& c = bmp->getPixel32(x - 1, y + 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
        {
            const ColorI& c = bmp->getPixel32(x, y + 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
        if (x < bmp->width() - 1) {
            const ColorI& c = bmp->getPixel32(x + 1, y + 1);
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
            ++count;
        }
    }

    return ColorI(r / count, g / count, b / count, a / count);
}

bool GraphicsTestHelper::equalsBitmapFile(Bitmap2D* bmp1, const Path& filePath, int passRate) {
    auto bmp2 = makeObject_deprecated<Bitmap2D>();
    bmp2->load(filePath);

    bool ignoreAlpha = true;

    int colorRange = 255 - (255 * passRate / 100);
    int pass = 0;
    int fail = 0;

    for (int y = 0; y < bmp1->height(); ++y) {
        for (int x = 0; x < bmp1->width(); ++x) {
            auto c1 = mixPixels(bmp1, x, y);
            auto c2 = mixPixels(bmp2, x, y);
            if (abs(c1.r - c2.r) <= colorRange &&
                abs(c1.g - c2.g) <= colorRange &&
                abs(c1.b - c2.b) <= colorRange &&
                (ignoreAlpha || abs(c1.a - c2.a) <= colorRange)) {
                ++pass;
            }
            else {
                ++fail;
            }
        }
    }

    int thr = ((bmp1->height() * bmp1->width()) * passRate / 100);
    return pass >= thr;
}

} // namespace ln
