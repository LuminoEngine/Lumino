#include "VisualTestHelper.hpp"

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define LN_UPDATE_REFERENCES 0

namespace VisualTest {

bool savePng(const char* path, const uint8_t* data, uint32_t width, uint32_t height) {
    return stbi_write_png(path, static_cast<int>(width), static_cast<int>(height),
                          4, data, static_cast<int>(width * 4)) != 0;
}

bool loadPng(const char* path, std::vector<uint8_t>& outData, uint32_t& outWidth, uint32_t& outHeight) {
    int w, h, channels;
    uint8_t* pixels = stbi_load(path, &w, &h, &channels, 4); // RGBA に強制
    if (!pixels) return false;

    outWidth = static_cast<uint32_t>(w);
    outHeight = static_cast<uint32_t>(h);
    size_t size = static_cast<size_t>(w) * h * 4;
    outData.assign(pixels, pixels + size);
    stbi_image_free(pixels);
    return true;
}

// 座標をクランプしてピクセルをサンプリングする。
static void samplePixel(const uint8_t* data, uint32_t w, uint32_t h,
                         int x, int y, float out[4]) {
    x = std::max(0, std::min(x, static_cast<int>(w) - 1));
    y = std::max(0, std::min(y, static_cast<int>(h) - 1));
    size_t idx = (static_cast<size_t>(y) * w + x) * 4;
    for (int c = 0; c < 4; c++) {
        out[c] = static_cast<float>(data[idx + c]);
    }
}

// 3x3 近傍の平均 (アンチエイリアスによるノイズを減らす)。
static void mixPixel(const uint8_t* data, uint32_t w, uint32_t h,
                     int cx, int cy, float out[4]) {
    out[0] = out[1] = out[2] = out[3] = 0.0f;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            float p[4];
            samplePixel(data, w, h, cx + dx, cy + dy, p);
            for (int c = 0; c < 4; c++) out[c] += p[c];
        }
    }
    for (int c = 0; c < 4; c++) out[c] /= 9.0f;
}

bool compareImages(
    const uint8_t* actual, uint32_t actualW, uint32_t actualH,
    const uint8_t* Expected, uint32_t expectedW, uint32_t expectedH,
    int passRate) {
    if (actualW != expectedW || actualH != expectedH) return false;

    const float colorRange = 255.0f - (255.0f * passRate / 100.0f);
    const uint32_t totalPixels = actualW * actualH;
    uint32_t passCount = 0;

    for (uint32_t y = 0; y < actualH; y++) {
        for (uint32_t x = 0; x < actualW; x++) {
            float a[4], e[4];
            mixPixel(actual, actualW, actualH, static_cast<int>(x), static_cast<int>(y), a);
            mixPixel(Expected, expectedW, expectedH, static_cast<int>(x), static_cast<int>(y), e);

            bool pixelPass = true;
            for (int c = 0; c < 3; c++) { // アルファは無視
                if (std::abs(a[c] - e[c]) > colorRange) {
                    pixelPass = false;
                    break;
                }
            }
            if (pixelPass) passCount++;
        }
    }

    float ratio = static_cast<float>(passCount) / static_cast<float>(totalPixels) * 100.0f;
    return ratio >= static_cast<float>(passRate);
}

bool captureAndCompare(
    const char* name,
    const uint8_t* data, uint32_t width, uint32_t height,
    const std::string& dataDir,
    bool forceUpdate) {
    std::string expectedDir = dataDir + "/Expected";
    std::string expectedPath = expectedDir + "/" + name + ".png";

    // LN_UPDATE_REFERENCES が設定されていれば、新しい参照画像として保存する。
    if (forceUpdate || LN_UPDATE_REFERENCES) {
        if (savePng(expectedPath.c_str(), data, width, height)) {
            std::printf("[VisualTest] Updated reference: %s\n", expectedPath.c_str());
            return true;
        }
        std::printf("[VisualTest] ERROR: Failed to save reference: %s\n", expectedPath.c_str());
        return false;
    }

    // 参照画像を読み込んで比較する。
    std::vector<uint8_t> expectedData;
    uint32_t expectedW, expectedH;
    if (!loadPng(expectedPath.c_str(), expectedData, expectedW, expectedH)) {
        // 参照画像がない - 現在のキャプチャを保存して報告する。
        std::string latestPath = dataDir + "/Expected/" + name + "-latest.png";
        savePng(latestPath.c_str(), data, width, height);
        std::printf("[VisualTest] Reference not found: %s\n", expectedPath.c_str());
        std::printf("[VisualTest] Saved actual result to: %s\n", latestPath.c_str());
        std::printf("[VisualTest] Run with LN_UPDATE_REFERENCES=1 to generate references.\n");
        return false;
    }

    bool pass = compareImages(data, width, height,
                              expectedData.data(), expectedW, expectedH);
    if (!pass) {
        std::string failPath = dataDir + "/Expected/" + name + "-fail.png";
        savePng(failPath.c_str(), data, width, height);
        std::printf("[VisualTest] MISMATCH: %s\n", name);
        std::printf("[VisualTest] Saved failed result to: %s\n", failPath.c_str());
    }
    return pass;
}

} // namespace VisualTest
