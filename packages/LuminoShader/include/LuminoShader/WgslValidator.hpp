// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
namespace shader {

/** WGSL 検証で得られた 1 件の診断メッセージ。 */
struct WgslDiagnostic {
    enum class Severity {
        Error,
        Warning,
        Info,
    };

    Severity severity = Severity::Error;
    uint64_t line = 0;   // 生成された WGSL 上の行番号 (1 始まり)
    uint64_t column = 0; // 生成された WGSL 上の桁番号 (1 始まり)
    std::string message;
};

/** WGSL 検証の結果。 */
struct WgslValidationReport {
    /** エラーが 1 件以上あったかどうか。 */
    bool failed = false;

    /** 行番号付きの構造化された診断メッセージ。 */
    std::vector<WgslDiagnostic> diagnostics;

    /**
     * 検証バックエンドが返した整形済みテキスト。
     * 該当行のソース抜粋やノートを含むため、そのままエラーメッセージに載せられる。
     */
    std::string detail;
};

/**
 * 生成された WGSL を実際の WebGPU 実装 (Dawn) で検証する。
 *
 * Slang は WGSL の制約 (textureSample を uniform control flow から呼ぶ等) を検査しないため、
 * Slang のコンパイルが通っても WebGPU 実行時にシェーダモジュールの生成が失敗することがある。
 * ブラウザ上でしか気づけないその種の不正を、コンパイル時に検出するのがこのクラスの役割である。
 *
 * 実装は Dawn の Null バックエンドでデバイスを 1 つ作り、
 * wgpuDeviceCreateShaderModule に生成 WGSL を渡して診断を取り出す。
 * Null バックエンドは GPU を必要としないため CI やヘッドレス環境でも動作する。
 *
 * このクラスが使えるのは LUMINO_SHADER_VALIDATE_WGSL が定義されたビルド
 * (デスクトップ + Dawn がリンクされている構成) のみである。
 * 使えない場合は available() が false を返し、create() はエラーになる。
 */
class WgslValidator final {
public:
    /** このビルドで WGSL 検証が利用可能かどうか。 */
    static bool available();

    static Result<std::unique_ptr<WgslValidator>> create();

    ~WgslValidator();

    WgslValidator(const WgslValidator&) = delete;
    WgslValidator& operator=(const WgslValidator&) = delete;

    /**
     * WGSL のソースコードを検証する。
     *
     * @param wgsl    WGSL のソースコード (NUL 終端でなくてよい)
     * @param length  wgsl のバイト数
     * @param label   診断メッセージ中で使う識別名 (エントリポイント名など)
     * @return 検証を実行できた場合はレポート。検証自体が実行できなかった場合はエラー。
     */
    Result<WgslValidationReport> validate(const char* wgsl, size_t length, const std::string& label);

private:
    WgslValidator();
    VoidResult init();

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace shader
} // namespace ln
