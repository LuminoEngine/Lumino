// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include <filesystem>

namespace ln {
namespace shader {

class UnifiedShader2;

class UnifiedShaderSerializer2 final {
public:
    enum FileVersion {
        FileVersion_1 = 1,
        FileVersion_2 = 2,   // ParameterBlockLayout2 に GlobalMemberInfo を追加
        FileVersion_3 = 3,   // UnifiedShader2::sourceName を追加
        FileVersion_Last,
        FileVersion_Current = FileVersion_Last - 1,
    };
    static constexpr const char* FileExt = "lcsh";

    static VoidResult saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath);

    /**
     * .lcsh のバイナリから UnifiedShader2 を復元する。
     *
     * @param target 実体化するコード blob のターゲット。ShaderTarget_UNKNOWN なら全て読み込む。
     *
     * 指定外のターゲットのコード blob は空のまま残す。
     * メタ情報 (エントリポイントやバインディングレイアウト) は ID の対応を保つため全て読み込む。
     */
    static Result<Ref<UnifiedShader2>> loadFromData(
        const void* data, size_t length, ShaderTarget target = ShaderTarget_UNKNOWN);
};

} // namespace shader
} // namespace ln
