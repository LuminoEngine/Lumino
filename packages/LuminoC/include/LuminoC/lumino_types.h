// clang-format off
#ifndef LUMINO_API_TYPES_H_
#define LUMINO_API_TYPES_H_

#include <stdint.h>

//------------------------------------------------------------------------------
// エクスポート / 可視性マクロ
//------------------------------------------------------------------------------
#if !defined(LUMINO_API)
    #if defined(__EMSCRIPTEN__)
        #include <emscripten.h>
        #define LUMINO_API EMSCRIPTEN_KEEPALIVE
    #elif defined(_WIN32) && defined(LUMINO_BUILD_DLL)
        #define LUMINO_API __declspec(dllexport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LUMINO_API __attribute__((visibility("default")))
    #else
        #define LUMINO_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef uint32_t LNBool;
#define LN_FALSE 0U
#define LN_TRUE 1U

//------------------------------------------------------------------------------
// 結果 / エラーコード
//------------------------------------------------------------------------------
typedef enum LNResult {
    /** 成功 */
    LN_OK = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

    /** ランタイム未初期化 */
    LN_RUNTIME_UNINITIALIZED = -2,

    /** 無効な引数 */
    LN_ERROR_INVALID_ARGUMENT = -3,

    /** 無効なハンドル */
    LN_ERROR_INVALID_HANDLE = -4,

    /** 現在のビルド/プラットフォームでは未対応の機能 (例: WASM ビルドでの一部 GraphicsContext API) */
    LN_ERROR_NOT_SUPPORTED = -5,

    /**
     * GPU デバイスロスト。復旧は Lumino 内部で自動的に行われるため、
     * クライアントはこのフレームの描画をスキップし、次のフレームで
     * LNGraphicsContext_BeginFrame を再試行してください。
     */
    LN_ERROR_DEVICE_LOST = -6,
} LNResult;

//------------------------------------------------------------------------------
// 列挙型
//------------------------------------------------------------------------------

/**
 * ログ出力レベル。値が大きいほど重大です。
 */
typedef enum LNLogLevel {
    /** 関数の入口や分岐のトレース */
    LN_LOG_LEVEL_TRACE   = 0,
    /** デバッグに有用な情報 (VERBOSE がクライアント向けの情報なのに対し、 DEBUG は Lumino 開発者向けの情報) */
    LN_LOG_LEVEL_DEBUG   = 1,
    /** Info に加えた詳細情報 */
    LN_LOG_LEVEL_VERBOSE = 2,
    /** 通常運用で有用な情報 (デフォルト) */
    LN_LOG_LEVEL_INFO    = 3,
    /** 問題になる可能性がある状況 */
    LN_LOG_LEVEL_WARNING = 4,
    /** 処理を続行できない障害 */
    LN_LOG_LEVEL_ERROR   = 5,
    /** プログラム実行が不可能な状況 */
    LN_LOG_LEVEL_FATAL   = 6,
    /** すべてのログを破棄します */
    LN_LOG_LEVEL_DISABLE = 7,
} LNLogLevel;

/** グラフィックスバックエンド。 */
typedef enum LNGraphicsBackend {
    LN_GRAPHICS_BACKEND_DEFAULT = 0,
    LN_GRAPHICS_BACKEND_VULKAN = 1,
    LN_GRAPHICS_BACKEND_WEBGPU = 2,
} LNGraphicsBackend;

/**
 * レンダーパスのアタッチメントのロード操作。
 * ゼロ初期化時のデフォルトは LN_LOAD_OP_CLEAR。
 */
typedef enum LNLoadOp {
    /**
     * 内容をクリアします。
     */
    LN_LOAD_OP_CLEAR     = 0,
    /**
     * 既存の内容を保持します。
     */
    LN_LOAD_OP_LOAD      = 1,
    /**
     * 内容不定 (パフォーマンス最適化)
     * ポストプロセスや G-Buffer のように出力先の全ピクセルを書き換える時に指定できます。
     */
    LN_LOAD_OP_DONT_CARE = 2,

    LN_LOAD_OP__FORCE32 = 0x7FFFFFFF,
} LNLoadOp;

//------------------------------------------------------------------------------
// ハンドル型
//------------------------------------------------------------------------------

/**
 * オブジェクトハンドル。上位16bitが世代番号、下位16bitがインデックス。
 */
typedef uint32_t LNHandle;

/** 無効なハンドル */
#define LN_NULL_HANDLE ((LNHandle)0)

//------------------------------------------------------------------------------
// グラフィックス型
//------------------------------------------------------------------------------

/** ビルトインシェーダの種類 */
typedef enum LNBuiltinShader {
    /** Unlit: テクスチャ * カラー、ライティングなし */
    LN_BUILTIN_SHADER_UNLIT       = 0,
    /** BasicLit: Blinn-Phong、1方向ライト */
    LN_BUILTIN_SHADER_BASIC_LIT   = 1,
    /** StencilMask: ステンシルバッファへの書き込み専用 */
    LN_BUILTIN_SHADER_STENCIL_MASK = 2,
} LNBuiltinShader;

/**
 * テクスチャフォーマット。
 * 値は内部 rhi::TextureFormat と一致します。
 */
typedef enum LNTextureFormat {
    LN_TEXTURE_FORMAT_UNDEFINED = 0,
    LN_TEXTURE_FORMAT_BGRA8_UNORM      = 1,
    LN_TEXTURE_FORMAT_BGRA8_UNORM_SRGB = 2,
    LN_TEXTURE_FORMAT_RGBA8_UNORM      = 3,
    LN_TEXTURE_FORMAT_RGBA8_UNORM_SRGB = 4,
    LN_TEXTURE_FORMAT_R8_UNORM         = 7,
    LN_TEXTURE_FORMAT_RG8_UNORM        = 8,
    LN_TEXTURE_FORMAT_RGBA16_FLOAT     = 9,
    LN_TEXTURE_FORMAT_RGBA32_FLOAT     = 10,
} LNTextureFormat;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_TYPES_H_
