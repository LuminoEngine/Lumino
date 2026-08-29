#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace VisualTest {

/** RGBA8 ピクセルデータを PNG ファイルに保存する。 */
bool savePng(const char* path, const uint8_t* data, uint32_t width, uint32_t height);

/** PNG ファイルを RGBA8 ピクセルデータとして読み込む。 */
bool loadPng(const char* path, std::vector<uint8_t>& outData, uint32_t& outWidth, uint32_t& outHeight);

/**
 * 2 枚の RGBA8 画像を許容誤差付きで比較する。
 * @param passRate  0-100。許容誤差内で一致しなければならないピクセルの割合 (%)。
 * @return 許容誤差内で画像が一致すれば true。
 */
bool compareImages(
    const uint8_t* actual, uint32_t actualW, uint32_t actualH,
    const uint8_t* expected, uint32_t expectedW, uint32_t expectedH,
    int passRate = 95);

/**
 * キャプチャしたピクセルを参照画像と比較する。
 * 環境変数 LN_UPDATE_REFERENCES が設定されている場合は、比較せずにキャプチャした
 * 画像を新しい参照画像として保存する。
 * @param name        テスト名 (ファイル名として使用。例: "ClearScreen")
 * @param data        RGBA8 ピクセルデータ
 * @param width       画像の幅
 * @param height      画像の高さ
 * @param dataDir     expected/ サブフォルダの基準ディレクトリ
 * @return 比較に合格したか、参照画像を更新した場合は true。
 */
bool captureAndCompare(
    const char* name,
    const uint8_t* data, uint32_t width, uint32_t height,
    const std::string& dataDir,
    bool forceUpdate = false);

} // namespace VisualTest
