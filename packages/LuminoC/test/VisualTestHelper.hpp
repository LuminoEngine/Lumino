#pragma once

#include <cstdint>
#include <vector>
#include <string>

#define LN_UPDATE_REFERENCES 0

namespace VisualTest {

/** Save RGBA8 pixel data to a PNG file. */
bool savePng(const char* path, const uint8_t* data, uint32_t width, uint32_t height);

/** Load a PNG file as RGBA8 pixel data. */
bool loadPng(const char* path, std::vector<uint8_t>& outData, uint32_t& outWidth, uint32_t& outHeight);

/**
 * Compare two RGBA8 images with tolerance.
 * @param passRate  0-100, percentage of pixels that must match within tolerance.
 * @return true if the images match within the given tolerance.
 */
bool compareImages(
    const uint8_t* actual, uint32_t actualW, uint32_t actualH,
    const uint8_t* expected, uint32_t expectedW, uint32_t expectedH,
    int passRate = 95);

/**
 * Compare captured pixels against a reference image.
 * If the environment variable LN_UPDATE_REFERENCES is set, saves the captured
 * image as the new reference instead of comparing.
 * @param name        Test name (used as filename, e.g. "ClearScreen")
 * @param data        RGBA8 pixel data
 * @param width       Image width
 * @param height      Image height
 * @param dataDir     Base directory for expected/ subfolder
 * @return true if comparison passes or reference was updated.
 */
bool captureAndCompare(
    const char* name,
    const uint8_t* data, uint32_t width, uint32_t height,
    const std::string& dataDir);

} // namespace VisualTest
