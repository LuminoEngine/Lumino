
#pragma once
#include "FlatCommon.h"

extern "C"
{
/**
    @brief 3次元のベクトルを定義します。
*/
struct LNVector3
{
    float x;
    float y;
    float z;
};

/**
    @brief 4次元のベクトルを定義します。
*/
struct LNVector4
{
    float x;
    float y;
    float z;
    float w;
};

/**
    @brief クォータニオンを定義します。
    @details このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
    この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
*/
struct LNQuaternion
{
    float x;
    float y;
    float z;
    float w;
};

/**
    @brief 4x4 の行列を定義します。
    @details 行列は数学的な定義とメモリレイアウト共に行優先 (column-major) です。
    このクラスは似た機能の static 関数とインスタンス関数があります。例えば makeRotationX(static 関数) と rotateX(インスタンス関数) です。前者は新しい行列を作成して返すのに対し、後者は現在の行列を変更します。例えば、以下の m1 と m2 は同じ結果になります。
    ~~~Matrix m1 = Matrix::makeRotationX(0.1) * Matrix::makeRotationY(0.2) * Matrix::makeRotationZ(0.3);
    Matrix m2;m2.rotateX(0.1);m2.rotateY(0.2);m2.rotateZ(0.3);~~~
    なお、後者は行列の生成と乗算をまとめて行うように最適化されており、高速に動作します。
*/
struct LNMatrix
{
    LNVector4 row0;
    LNVector4 row1;
    LNVector4 row2;
    LNVector4 row3;
};

/**
    @brief 各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
*/
struct LNColor
{
    float r;
    float g;
    float b;
    float a;
};

/**
    @brief 色調を定義します。
*/
struct LNColorTone
{
    float r;
    float g;
    float b;
    float s;
};

/**
    @brief 2次元上の点を表します。
*/
struct LNPoint
{
    float x;
    float y;
};

/**
    @brief 2次元上のオブジェクトサイズを表します。
*/
struct LNSize
{
    float width;
    float height;
};

/**
    @brief 2次元の矩形を表すクラスです。
*/
struct LNRect
{
    float x;
    float y;
    float width;
    float height;
};

/**
    @brief 四角形の枠の太さを表すクラスです。
*/
struct LNThickness
{
    float left;
    float top;
    float right;
    float bottom;
};

/**
    @brief 四角形の角の半径を表します。
*/
struct LNCornerRadius
{
    float topLeft;
    float topRight;
    float bottomRight;
    float bottomLeft;
};



/**
    @brief ログの通知レベル
*/
typedef enum tagLNLogLevel
{
    /**
        @brief 
    */
    LN_LOG_LEVEL_UNKNOWN = 0,

    /**
        @brief 
    */
    LN_LOG_LEVEL_VERBOSE = 1,

    /**
        @brief 
    */
    LN_LOG_LEVEL_DEBUG = 2,

    /**
        @brief 
    */
    LN_LOG_LEVEL_INFO = 3,

    /**
        @brief 
    */
    LN_LOG_LEVEL_WARNING = 4,

    /**
        @brief 
    */
    LN_LOG_LEVEL_ERROR = 5,

    /**
        @brief 
    */
    LN_LOG_LEVEL_FATAL = 6,

    /**
        @brief 
    */
    LN_LOG_LEVEL_DISBLE = 7,

} LNLogLevel;

/**
    @brief エンコーディングの種類
*/
typedef enum tagLNEncodingType
{
    /**
        @brief 不明な文字エンコーディング (判別失敗。またはバイナリファイル)
    */
    LN_ENCODING_TYPE_UNKNOWN = 0,

    /**
        @brief ASCII (拡張アスキーは含まない http://www.asciitable.com/)
    */
    LN_ENCODING_TYPE_ASCII = 1,

    /**
        @brief UTF-8
    */
    LN_ENCODING_TYPE_UTF8 = 2,

    /**
        @brief UTF-8 (BOM 無し)
    */
    LN_ENCODING_TYPE_UTF8N = 3,

    /**
        @brief 日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装
    */
    LN_ENCODING_TYPE_SJIS = 4,

} LNEncodingType;

/**
    @brief キーコード
*/
typedef enum tagLNKeys
{
    /**
        @brief 
    */
    LN_KEYS_UNKNOWN = 0,

    /**
        @brief 
    */
    LN_KEYS_A = 1,

    /**
        @brief 
    */
    LN_KEYS_B = 2,

    /**
        @brief 
    */
    LN_KEYS_C = 3,

    /**
        @brief 
    */
    LN_KEYS_D = 4,

    /**
        @brief 
    */
    LN_KEYS_E = 5,

    /**
        @brief 
    */
    LN_KEYS_F = 6,

    /**
        @brief 
    */
    LN_KEYS_G = 7,

    /**
        @brief 
    */
    LN_KEYS_H = 8,

    /**
        @brief 
    */
    LN_KEYS_I = 9,

    /**
        @brief 
    */
    LN_KEYS_J = 10,

    /**
        @brief 
    */
    LN_KEYS_K = 11,

    /**
        @brief 
    */
    LN_KEYS_L = 12,

    /**
        @brief 
    */
    LN_KEYS_M = 13,

    /**
        @brief 
    */
    LN_KEYS_N = 14,

    /**
        @brief 
    */
    LN_KEYS_O = 15,

    /**
        @brief 
    */
    LN_KEYS_P = 16,

    /**
        @brief 
    */
    LN_KEYS_Q = 17,

    /**
        @brief 
    */
    LN_KEYS_R = 18,

    /**
        @brief 
    */
    LN_KEYS_S = 19,

    /**
        @brief 
    */
    LN_KEYS_T = 20,

    /**
        @brief 
    */
    LN_KEYS_U = 21,

    /**
        @brief 
    */
    LN_KEYS_V = 22,

    /**
        @brief 
    */
    LN_KEYS_W = 23,

    /**
        @brief 
    */
    LN_KEYS_X = 24,

    /**
        @brief 
    */
    LN_KEYS_Y = 25,

    /**
        @brief 
    */
    LN_KEYS_Z = 26,

    /**
        @brief 
    */
    LN_KEYS_D0 = 27,

    /**
        @brief 
    */
    LN_KEYS_D1 = 28,

    /**
        @brief 
    */
    LN_KEYS_D2 = 29,

    /**
        @brief 
    */
    LN_KEYS_D3 = 30,

    /**
        @brief 
    */
    LN_KEYS_D4 = 31,

    /**
        @brief 
    */
    LN_KEYS_D5 = 32,

    /**
        @brief 
    */
    LN_KEYS_D6 = 33,

    /**
        @brief 
    */
    LN_KEYS_D7 = 34,

    /**
        @brief 
    */
    LN_KEYS_D8 = 35,

    /**
        @brief 
    */
    LN_KEYS_D9 = 36,

    /**
        @brief 
    */
    LN_KEYS_F1 = 37,

    /**
        @brief 
    */
    LN_KEYS_F2 = 38,

    /**
        @brief 
    */
    LN_KEYS_F3 = 39,

    /**
        @brief 
    */
    LN_KEYS_F4 = 40,

    /**
        @brief 
    */
    LN_KEYS_F5 = 41,

    /**
        @brief 
    */
    LN_KEYS_F6 = 42,

    /**
        @brief 
    */
    LN_KEYS_F7 = 43,

    /**
        @brief 
    */
    LN_KEYS_F8 = 44,

    /**
        @brief 
    */
    LN_KEYS_F9 = 45,

    /**
        @brief 
    */
    LN_KEYS_F10 = 46,

    /**
        @brief 
    */
    LN_KEYS_F11 = 47,

    /**
        @brief 
    */
    LN_KEYS_F12 = 48,

    /**
        @brief 
    */
    LN_KEYS_SPACE = 49,

    /**
        @brief 
    */
    LN_KEYS_ESCAPE = 50,

    /**
        @brief 
    */
    LN_KEYS_UP = 51,

    /**
        @brief 
    */
    LN_KEYS_DOWN = 52,

    /**
        @brief 
    */
    LN_KEYS_LEFT = 53,

    /**
        @brief 
    */
    LN_KEYS_RIGHT = 54,

    /**
        @brief 
    */
    LN_KEYS_LSHIFT = 55,

    /**
        @brief 
    */
    LN_KEYS_RSHIFT = 56,

    /**
        @brief 
    */
    LN_KEYS_LCTRL = 57,

    /**
        @brief 
    */
    LN_KEYS_RCTRL = 58,

    /**
        @brief 
    */
    LN_KEYS_LALT = 59,

    /**
        @brief 
    */
    LN_KEYS_RALT = 60,

    /**
        @brief 
    */
    LN_KEYS_TAB = 61,

    /**
        @brief 
    */
    LN_KEYS_ENTER = 62,

    /**
        @brief 
    */
    LN_KEYS_BACK_SPACE = 63,

    /**
        @brief 
    */
    LN_KEYS_INSERT = 64,

    /**
        @brief 
    */
    LN_KEYS_DELETE = 65,

    /**
        @brief 
    */
    LN_KEYS_PAGE_UP = 66,

    /**
        @brief 
    */
    LN_KEYS_PAGE_DOWN = 67,

    /**
        @brief 
    */
    LN_KEYS_HOME = 68,

    /**
        @brief 
    */
    LN_KEYS_END = 69,

    /**
        @brief [非推奨 GLFWでは;]
    */
    LN_KEYS_COLON = 70,

    /**
        @brief ;	[非推奨 GLFWでは=]
    */
    LN_KEYS_SEMICOLON = 71,

    /**
        @brief ,
    */
    LN_KEYS_COMMA = 72,

    /**
        @brief .
    */
    LN_KEYS_PERIOD = 73,

    /**
        @brief /
    */
    LN_KEYS_SLASH = 74,

    /**
        @brief -
    */
    LN_KEYS_MINUS = 75,

    /**
        @brief \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
    */
    LN_KEYS_BACKSLASH = 76,

    /**
        @brief \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
    */
    LN_KEYS_YEN = 77,

    /**
        @brief ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
    */
    LN_KEYS_CARET = 78,

    /**
        @brief [
    */
    LN_KEYS_LBRACKET = 79,

    /**
        @brief ]
    */
    LN_KEYS_RBRACKET = 80,

    /**
        @brief Android '+'
    */
    LN_KEYS_ANDROID_PLUS = 81,

    /**
        @brief 
    */
    LN_KEYS_TERMINATOR = 82,

} LNKeys;

/**
    @brief マウスボタン
*/
typedef enum tagLNMouseButtons
{
    /**
        @brief 無効または押されていないことを示す
    */
    LN_MOUSE_BUTTONS_NONE = 0,

    /**
        @brief 左ボタン
    */
    LN_MOUSE_BUTTONS_LEFT = 1,

    /**
        @brief 右ボタン
    */
    LN_MOUSE_BUTTONS_RIGHT = 2,

    /**
        @brief 中央ボタン
    */
    LN_MOUSE_BUTTONS_MIDDLE = 3,

    /**
        @brief 拡張ボタン1
    */
    LN_MOUSE_BUTTONS_X1 = 4,

    /**
        @brief 拡張ボタン2
    */
    LN_MOUSE_BUTTONS_X2 = 5,

} LNMouseButtons;

/**
    @brief 音量フェード完了時の動作を表します。
*/
typedef enum tagLNSoundFadeBehavior
{
    /**
        @brief 再生を継続する
    */
    LN_SOUND_FADE_BEHAVIOR_CONTINUE = 0,

    /**
        @brief 停止する
    */
    LN_SOUND_FADE_BEHAVIOR_STOP = 1,

    /**
        @brief 停止して、次の再生に備えてサウンドの音量を元の値に戻す
    */
    LN_SOUND_FADE_BEHAVIOR_STOP_RESET = 2,

    /**
        @brief 一時停止する
    */
    LN_SOUND_FADE_BEHAVIOR_PAUSE = 3,

    /**
        @brief 一時停止して、次の再生に備えてサウンドの音量を元の値に戻す
    */
    LN_SOUND_FADE_BEHAVIOR_PAUSE_RESET = 4,

} LNSoundFadeBehavior;

/**
    @brief バックエンドグラフィックス API の種類
*/
typedef enum tagLNGraphicsAPI
{
    /**
        @brief 自動選択
    */
    LN_GRAPHICS_API_DEFAULT = 0,

    /**
        @brief OpenGL
    */
    LN_GRAPHICS_API_OPEN_GL = 1,

    /**
        @brief Vulkan
    */
    LN_GRAPHICS_API_VULKAN = 2,

    /**
        @brief Vulkan
    */
    LN_GRAPHICS_API_DIRECT_X12 = 3,

} LNGraphicsAPI;

/**
    @brief ピクセルフォーマット
*/
typedef enum tagLNPixelFormat
{
    /**
        @brief Unknown
    */
    LN_PIXEL_FORMAT_UNKNOWN = 0,

    /**
        @brief 8bit アルファ値のみのフォーマット
    */
    LN_PIXEL_FORMAT_A8 = 1,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGBA8 = 2,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGB8 = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_PIXEL_FORMAT_RGBA32F = 4,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_PIXEL_FORMAT_R32S = 5,

} LNPixelFormat;

/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLNTextureFormat
{
    /**
        @brief Unknown
    */
    LN_TEXTURE_FORMAT_UNKNOWN = 0,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA8 = 1,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGB8 = 2,

    /**
        @brief RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA16F = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA32F = 4,

    /**
        @brief 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R16F = 5,

    /**
        @brief 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R32F = 6,

    /**
        @brief 32bit の符号あり整数フォーマット
    */
    LN_TEXTURE_FORMAT_R32S = 7,

} LNTextureFormat;

/**
    @brief 深度バッファのフォーマット
*/
typedef enum tagLNDepthBufferFormat
{
    /**
        @brief S8 32 ビット
    */
    LN_DEPTH_BUFFER_FORMAT_D24S8 = 0,

} LNDepthBufferFormat;

/**
    @brief ShadingModel
*/
typedef enum tagLNShadingModel
{
    /**
        @brief Default
    */
    LN_SHADING_MODEL_DEFAULT = 0,

    /**
        @brief Unlit
    */
    LN_SHADING_MODEL_UNLIT = 1,

} LNShadingModel;

/**
    @brief 合成方法
*/
typedef enum tagLNBlendMode
{
    /**
        @brief 通常
    */
    LN_BLEND_MODE_NORMAL = 0,

    /**
        @brief アルファブレンド (RGB をアルファブレンドし、A を加算合成)
    */
    LN_BLEND_MODE_ALPHA = 1,

    /**
        @brief 加算合成
    */
    LN_BLEND_MODE_ADD = 2,

    /**
        @brief 減算合成
    */
    LN_BLEND_MODE_SUBTRACT = 3,

    /**
        @brief 乗算合成
    */
    LN_BLEND_MODE_MULTIPLY = 4,

} LNBlendMode;

/**
    @brief 背景のクリア方法
*/
typedef enum tagLNSceneClearMode
{
    /**
        @brief クリアしません。
    */
    LN_SCENE_CLEAR_MODE_NONE = 0,

    /**
        @brief 背景色と深度バッファをクリアします。
    */
    LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH = 1,

    /**
        @brief (未実装)
    */
    LN_SCENE_CLEAR_MODE_SKY = 2,

    /**
        @brief (未実装)
    */
    LN_SCENE_CLEAR_MODE_SKY0 = 3,

    /**
        @brief スカイドームでクリアします。
    */
    LN_SCENE_CLEAR_MODE_SKY_DOME = 4,

} LNSceneClearMode;

/**
    @brief アニメーションの繰り返し方法
*/
typedef enum tagLNAnimationWrapMode
{
    /**
        @brief 繰り返しを行わず、1度だけ再生します。
    */
    LN_ANIMATION_WRAP_MODE_ONCE = 0,

    /**
        @brief 最後まで再生された後、先頭に戻ってループします。
    */
    LN_ANIMATION_WRAP_MODE_LOOP = 1,

    /**
        @brief 最後まで再生された後、逆方向に戻ってループします。
    */
    LN_ANIMATION_WRAP_MODE_ALTERNATE = 2,

} LNAnimationWrapMode;

/**
    @brief 階層構造を持つアニメーションデータの動作モード
*/
typedef enum tagLNHierarchicalAnimationMode
{
    /**
        @brief ルートノードのみ、平行移動を有効化します。
    */
    LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT = 0,

    /**
        @brief AllowTranslationOnlyRootY
    */
    LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y = 1,

    /**
        @brief すべてのノードの平行移動を有効化します。
    */
    LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION = 2,

    /**
        @brief 平行移動を無効化します。
    */
    LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION = 3,

} LNHierarchicalAnimationMode;

/**
    @brief キーフレームアニメーションの補間方法
*/
typedef enum tagLNTangentMode
{
    /**
        @brief 線形補間
    */
    LN_TANGENT_MODE_LINEAR = 0,

    /**
        @brief 接線 (速度) を使用した補間 (エルミートスプライン)
    */
    LN_TANGENT_MODE_TANGENT = 1,

    /**
        @brief キーフレームの値を通過するなめらかな補間 (Catmull-Rom)
    */
    LN_TANGENT_MODE_AUTO = 2,

    /**
        @brief 補間なし
    */
    LN_TANGENT_MODE_CONSTANT = 3,

} LNTangentMode;

/**
    @brief ParticleEmitterShapeType
*/
typedef enum tagLNParticleEmitterShapeType
{
    /**
        @brief 
    */
    LN_PARTICLE_EMITTER_SHAPE_TYPE_SPHERE = 0,

    /**
        @brief パーティクルを球状に放出します。
    */
    LN_PARTICLE_EMITTER_SHAPE_TYPE_CONE = 1,

    /**
        @brief パーティクルをコーン型に放出します。
    */
    LN_PARTICLE_EMITTER_SHAPE_TYPE_BOX = 2,

} LNParticleEmitterShapeType;

/**
    @brief パーティクルの向く方向
*/
typedef enum tagLNParticleGeometryDirection
{
    /**
        @brief 常に視点方向を向く
    */
    LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW = 0,

    /**
        @brief 進行方向を軸に、表面 (Y+) が視点から見えるようにする
    */
    LN_PARTICLE_GEOMETRY_DIRECTION_TOP = 1,

    /**
        @brief Y 軸方向に直立し、カメラの方を向きます。
    */
    LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD = 2,

    /**
        @brief XZ 軸上の「床」平面と平行になります。
    */
    LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD = 3,

} LNParticleGeometryDirection;

/**
    @brief レベル遷移時の画面エフェクトの種類
*/
typedef enum tagLNLevelTransitionEffectMode
{
    /**
        @brief エフェクト無し
    */
    LN_LEVEL_TRANSITION_EFFECT_MODE_NONE = 0,

    /**
        @brief フェードイン・フェードアウト
    */
    LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT = 1,

    /**
        @brief クロスフェード
    */
    LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE = 2,

} LNLevelTransitionEffectMode;

/**
    @brief コントロールのレイアウト方向を示します。
*/
typedef enum tagLNUILayoutOrientation
{
    /**
        @brief 水平方向に配置します。
    */
    LN_UILAYOUT_ORIENTATION_HORIZONTAL = 0,

    /**
        @brief 垂直方向に配置します。
    */
    LN_UILAYOUT_ORIENTATION_VERTICAL = 1,

    /**
        @brief 水平方向（右から左）に配置します。
    */
    LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL = 2,

    /**
        @brief 垂直方向（下から上）に配置します。
    */
    LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL = 3,

} LNUILayoutOrientation;

/**
    @brief UI要素の表示状態を指定します。
*/
typedef enum tagLNUIVisibility
{
    /**
        @brief 要素を表示します。
    */
    LN_UIVISIBILITY_VISIBLE = 0,

    /**
        @brief 要素を表示しませんが、その要素の領域をレイアウト内に予約します。
    */
    LN_UIVISIBILITY_HIDDEN = 1,

    /**
        @brief 要素を表示しません。また、その要素の領域もレイアウト内に予約しません。
    */
    LN_UIVISIBILITY_COLLAPSED = 2,

} LNUIVisibility;

/**
    @brief カラーインデックス
*/
typedef enum tagLNUIColorHues
{
    /**
        @brief 
    */
    LN_UICOLOR_HUES_RED = 0,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_PINK = 1,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_PURPLE = 2,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_DEEP_PURPLE = 4,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_INDIGO = 5,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_BLUE = 6,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_LIGHT_BLUE = 7,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_CYAN = 8,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_TEAL = 9,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_GREEN = 10,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_LIGHT_GREEN = 11,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_LIME = 12,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_YELLOW = 13,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_AMBER = 14,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_ORANGE = 15,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_DEEP_ORANGE = 16,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_BROWN = 17,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_GREY = 18,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_BLACK = 19,

    /**
        @brief 
    */
    LN_UICOLOR_HUES_WHITE = 20,

} LNUIColorHues;

/**
    @brief 縦方向の表示位置を示します。
*/
typedef enum tagLNUIVAlignment
{
    /**
        @brief 子要素を、親のレイアウト スロットの上端に揃えて配置します。
    */
    LN_UIVALIGNMENT_TOP = 0,

    /**
        @brief 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    */
    LN_UIVALIGNMENT_CENTER = 1,

    /**
        @brief 子要素を、親のレイアウト スロットの下端に揃えて配置します。
    */
    LN_UIVALIGNMENT_BOTTOM = 2,

    /**
        @brief 子要素を、親のレイアウト スロット全体に引き伸ばします。
    */
    LN_UIVALIGNMENT_STRETCH = 3,

} LNUIVAlignment;

/**
    @brief 横方向の表示位置を示します。
*/
typedef enum tagLNUIHAlignment
{
    /**
        @brief 子要素を、親のレイアウト スロットの左側に揃えて配置します。
    */
    LN_UIHALIGNMENT_LEFT = 0,

    /**
        @brief 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    */
    LN_UIHALIGNMENT_CENTER = 1,

    /**
        @brief 子要素を、親のレイアウト スロットの右側に揃えて配置します。
    */
    LN_UIHALIGNMENT_RIGHT = 2,

    /**
        @brief 子要素を、親のレイアウト スロット全体に引き伸ばします。
    */
    LN_UIHALIGNMENT_STRETCH = 3,

} LNUIHAlignment;

/**
    @brief UIInlinePlacement
*/
typedef enum tagLNUIInlinePlacement
{
    /**
        @brief TopLeft
    */
    LN_UIINLINE_PLACEMENT_TOP_LEFT = 0,

    /**
        @brief Top
    */
    LN_UIINLINE_PLACEMENT_TOP = 1,

    /**
        @brief TopRight
    */
    LN_UIINLINE_PLACEMENT_TOP_RIGHT = 2,

    /**
        @brief Left
    */
    LN_UIINLINE_PLACEMENT_LEFT = 3,

    /**
        @brief Center
    */
    LN_UIINLINE_PLACEMENT_CENTER = 4,

    /**
        @brief Right
    */
    LN_UIINLINE_PLACEMENT_RIGHT = 5,

    /**
        @brief BottomLeft
    */
    LN_UIINLINE_PLACEMENT_BOTTOM_LEFT = 6,

    /**
        @brief Bottom
    */
    LN_UIINLINE_PLACEMENT_BOTTOM = 7,

    /**
        @brief BottomRight
    */
    LN_UIINLINE_PLACEMENT_BOTTOM_RIGHT = 8,

} LNUIInlinePlacement;

/**
    @brief 
*/
typedef enum tagLNUIListSubmitMode
{
    /**
        @brief ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
    */
    LN_UILIST_SUBMIT_MODE_SINGLE = 0,

    /**
        @brief エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.
    */
    LN_UILIST_SUBMIT_MODE_DOUBLE = 1,

} LNUIListSubmitMode;

typedef LNResult(*LNPromiseFailureDelegateCallback)(LNHandle promisefailuredelegate);
typedef LNResult(*LNZVTestDelegate1Callback)(LNHandle zvtestdelegate1, int p1);
typedef LNResult(*LNZVTestDelegate2Callback)(LNHandle zvtestdelegate2, int p1, int p2, int* outReturn);
typedef LNResult(*LNZVTestDelegate3Callback)(LNHandle zvtestdelegate3, LNHandle p1);
typedef LNResult(*LNZVTestEventHandler1Callback)(LNHandle zvtesteventhandler1);
typedef LNResult(*LNZVTestEventHandler2Callback)(LNHandle zvtesteventhandler2, LNHandle p1);
typedef LNResult(*LNTexture2DDelegateCallback)(LNHandle texture2ddelegate, LNHandle p1);
typedef LNResult(*LNCollisionEventHandlerCallback)(LNHandle collisioneventhandler, LNHandle p1);
typedef LNResult(*LNTestDelegateCallback)(LNHandle testdelegate, int p1, int* outReturn);
typedef LNResult(*LNUIGeneralEventHandlerCallback)(LNHandle uigeneraleventhandler, LNHandle p1);
typedef LNResult(*LNUIEventHandlerCallback)(LNHandle uieventhandler);
typedef LNResult(*LNInterpreterCommandDelegateCallback)(LNHandle interpretercommanddelegate, LNHandle p1, LNBool* outReturn);
typedef LNResult(*LNObjectSerializeHandlerCallback)(LNHandle objectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNEventConnectionSerializeHandlerCallback)(LNHandle eventconnectionserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNVariantSerializeHandlerCallback)(LNHandle variantserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNZVTestClass1SerializeHandlerCallback)(LNHandle zvtestclass1serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNZVTestEventArgs1SerializeHandlerCallback)(LNHandle zvtesteventargs1serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSerializer2SerializeHandlerCallback)(LNHandle serializer2serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAssetObjectSerializeHandlerCallback)(LNHandle assetobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAssetImportSettingsSerializeHandlerCallback)(LNHandle assetimportsettingsserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAssetModelSerializeHandlerCallback)(LNHandle assetmodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSoundSerializeHandlerCallback)(LNHandle soundserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNTextureSerializeHandlerCallback)(LNHandle textureserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNTexture2DSerializeHandlerCallback)(LNHandle texture2dserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNShaderSerializeHandlerCallback)(LNHandle shaderserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRenderViewSerializeHandlerCallback)(LNHandle renderviewserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNMaterialSerializeHandlerCallback)(LNHandle materialserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNMeshNodeSerializeHandlerCallback)(LNHandle meshnodeserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAnimationControllerSerializeHandlerCallback)(LNHandle animationcontrollerserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNMeshModelSerializeHandlerCallback)(LNHandle meshmodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNMeshImportSettingsSerializeHandlerCallback)(LNHandle meshimportsettingsserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSkinnedMeshModelSerializeHandlerCallback)(LNHandle skinnedmeshmodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNCollisionShapeSerializeHandlerCallback)(LNHandle collisionshapeserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNBoxCollisionShapeSerializeHandlerCallback)(LNHandle boxcollisionshapeserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAnimationCurveSerializeHandlerCallback)(LNHandle animationcurveserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNKeyFrameAnimationCurveSerializeHandlerCallback)(LNHandle keyframeanimationcurveserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAnimationClipSerializeHandlerCallback)(LNHandle animationclipserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAnimationStateSerializeHandlerCallback)(LNHandle animationstateserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNEffectResourceSerializeHandlerCallback)(LNHandle effectresourceserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNParticleEmitterModelSerializeHandlerCallback)(LNHandle particleemittermodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNParticleModelSerializeHandlerCallback)(LNHandle particlemodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNComponentSerializeHandlerCallback)(LNHandle componentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNVisualComponentSerializeHandlerCallback)(LNHandle visualcomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpriteComponentSerializeHandlerCallback)(LNHandle spritecomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNCharacterControllerSerializeHandlerCallback)(LNHandle charactercontrollerserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldSerializeHandlerCallback)(LNHandle worldserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNComponentListSerializeHandlerCallback)(LNHandle componentlistserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldObjectSerializeHandlerCallback)(LNHandle worldobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldObjectPreUpdateHandlerCallback)(LNHandle worldobjectpreupdatehandler, LNHandle self);
typedef LNResult(*LNWorldObjectUpdateHandlerCallback)(LNHandle worldobjectupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNVisualObjectSerializeHandlerCallback)(LNHandle visualobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNVisualObjectPreUpdateHandlerCallback)(LNHandle visualobjectpreupdatehandler, LNHandle self);
typedef LNResult(*LNVisualObjectUpdateHandlerCallback)(LNHandle visualobjectupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNCameraSerializeHandlerCallback)(LNHandle cameraserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNCameraPreUpdateHandlerCallback)(LNHandle camerapreupdatehandler, LNHandle self);
typedef LNResult(*LNCameraUpdateHandlerCallback)(LNHandle cameraupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNEnvironmentLightSerializeHandlerCallback)(LNHandle environmentlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNEnvironmentLightPreUpdateHandlerCallback)(LNHandle environmentlightpreupdatehandler, LNHandle self);
typedef LNResult(*LNEnvironmentLightUpdateHandlerCallback)(LNHandle environmentlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNDirectionalLightSerializeHandlerCallback)(LNHandle directionallightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNDirectionalLightPreUpdateHandlerCallback)(LNHandle directionallightpreupdatehandler, LNHandle self);
typedef LNResult(*LNDirectionalLightUpdateHandlerCallback)(LNHandle directionallightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNPointLightSerializeHandlerCallback)(LNHandle pointlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNPointLightPreUpdateHandlerCallback)(LNHandle pointlightpreupdatehandler, LNHandle self);
typedef LNResult(*LNPointLightUpdateHandlerCallback)(LNHandle pointlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNSpotLightSerializeHandlerCallback)(LNHandle spotlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpotLightPreUpdateHandlerCallback)(LNHandle spotlightpreupdatehandler, LNHandle self);
typedef LNResult(*LNSpotLightUpdateHandlerCallback)(LNHandle spotlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNSpriteSerializeHandlerCallback)(LNHandle spriteserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpritePreUpdateHandlerCallback)(LNHandle spritepreupdatehandler, LNHandle self);
typedef LNResult(*LNSpriteUpdateHandlerCallback)(LNHandle spriteupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNCameraOrbitControlComponentSerializeHandlerCallback)(LNHandle cameraorbitcontrolcomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRaycasterSerializeHandlerCallback)(LNHandle raycasterserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRaycastResultSerializeHandlerCallback)(LNHandle raycastresultserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldRenderViewSerializeHandlerCallback)(LNHandle worldrenderviewserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNShapeObjectSerializeHandlerCallback)(LNHandle shapeobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNShapeObjectPreUpdateHandlerCallback)(LNHandle shapeobjectpreupdatehandler, LNHandle self);
typedef LNResult(*LNShapeObjectUpdateHandlerCallback)(LNHandle shapeobjectupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNPlaneMeshSerializeHandlerCallback)(LNHandle planemeshserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNPlaneMeshPreUpdateHandlerCallback)(LNHandle planemeshpreupdatehandler, LNHandle self);
typedef LNResult(*LNPlaneMeshUpdateHandlerCallback)(LNHandle planemeshupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNBoxMeshSerializeHandlerCallback)(LNHandle boxmeshserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNBoxMeshPreUpdateHandlerCallback)(LNHandle boxmeshpreupdatehandler, LNHandle self);
typedef LNResult(*LNBoxMeshUpdateHandlerCallback)(LNHandle boxmeshupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNMeshComponentSerializeHandlerCallback)(LNHandle meshcomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNCollisionSerializeHandlerCallback)(LNHandle collisionserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNTriggerBodyComponentSerializeHandlerCallback)(LNHandle triggerbodycomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNParticleEmitterSerializeHandlerCallback)(LNHandle particleemitterserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNParticleEmitterPreUpdateHandlerCallback)(LNHandle particleemitterpreupdatehandler, LNHandle self);
typedef LNResult(*LNParticleEmitterUpdateHandlerCallback)(LNHandle particleemitterupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNLevelSerializeHandlerCallback)(LNHandle levelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNLevelStartHandlerCallback)(LNHandle levelstarthandler, LNHandle self);
typedef LNResult(*LNLevelStopHandlerCallback)(LNHandle levelstophandler, LNHandle self);
typedef LNResult(*LNLevelPauseHandlerCallback)(LNHandle levelpausehandler, LNHandle self);
typedef LNResult(*LNLevelResumeHandlerCallback)(LNHandle levelresumehandler, LNHandle self);
typedef LNResult(*LNLevelUpdateHandlerCallback)(LNHandle levelupdatehandler, LNHandle self);
typedef LNResult(*LNUIEventArgsSerializeHandlerCallback)(LNHandle uieventargsserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUILayoutElementSerializeHandlerCallback)(LNHandle uilayoutelementserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIElementSerializeHandlerCallback)(LNHandle uielementserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUITextSerializeHandlerCallback)(LNHandle uitextserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUISpriteSerializeHandlerCallback)(LNHandle uispriteserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIIconSerializeHandlerCallback)(LNHandle uiiconserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIMessageTextAreaSerializeHandlerCallback)(LNHandle uimessagetextareaserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUILayoutPanelSerializeHandlerCallback)(LNHandle uilayoutpanelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIBoxLayoutSerializeHandlerCallback)(LNHandle uiboxlayoutserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIStackLayoutSerializeHandlerCallback)(LNHandle uistacklayoutserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIGridLayoutSerializeHandlerCallback)(LNHandle uigridlayoutserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIControlSerializeHandlerCallback)(LNHandle uicontrolserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIButtonBaseSerializeHandlerCallback)(LNHandle uibuttonbaseserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIButtonSerializeHandlerCallback)(LNHandle uibuttonserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIWindowSerializeHandlerCallback)(LNHandle uiwindowserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIListItemSerializeHandlerCallback)(LNHandle uilistitemserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIListItemsControlSerializeHandlerCallback)(LNHandle uilistitemscontrolserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIListBoxItemSerializeHandlerCallback)(LNHandle uilistboxitemserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIListBoxSerializeHandlerCallback)(LNHandle uilistboxserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInputGestureSerializeHandlerCallback)(LNHandle inputgestureserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNKeyGestureSerializeHandlerCallback)(LNHandle keygestureserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterCommandSerializeHandlerCallback)(LNHandle interpretercommandserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterCommandListSerializeHandlerCallback)(LNHandle interpretercommandlistserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterSerializeHandlerCallback)(LNHandle interpreterserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterUpdateWaitHandlerCallback)(LNHandle interpreterupdatewaithandler, LNHandle self, LNBool* outReturn);
typedef LNResult(*LNApplicationSerializeHandlerCallback)(LNHandle applicationserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNApplicationInitHandlerCallback)(LNHandle applicationinithandler, LNHandle self);
typedef LNResult(*LNApplicationUpdateHandlerCallback)(LNHandle applicationupdatehandler, LNHandle self);


//==============================================================================
// ln::Vector3

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector3 : Vector3
*/
LN_FLAT_API LNResult LNVector3_SetZeros(LNVector3* vector3);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector3 : Vector3
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNVector3_Set(LNVector3* vector3, float x, float y, float z);


/**
    @brief get
    @param[in] vector3 : Vector3
    @param[] outX : 
    @param[] outY : 
    @param[] outZ : 
*/
LN_FLAT_API LNResult LNVector3_Get(const LNVector3* vector3, float* outX, float* outY, float* outZ);


/**
    @brief ベクトルの長さを返します。
    @param[in] vector3 : Vector3
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVector3_Length(const LNVector3* vector3, float* outReturn);


/**
    @brief ベクトルの長さの 2 乗を返します。
    @param[in] vector3 : Vector3
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVector3_LengthSquared(const LNVector3* vector3, float* outReturn);


/**
    @brief このベクトルを正規化します。
    @param[in] vector3 : Vector3
    @details ベクトルの長さが 0 の場合は正規化を行いません。
*/
LN_FLAT_API LNResult LNVector3_MutatingNormalize(LNVector3* vector3);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[] x : 
    @param[] y : 
    @param[] z : 
    @param[out] outReturn : 結果を格納する変数。
    @return 正規化されたベクトル
*/
LN_FLAT_API LNResult LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[] vec : 
    @param[out] outReturn : 結果を格納する変数。
    @return 正規化されたベクトル
*/
LN_FLAT_API LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn);


//==============================================================================
// ln::Vector4

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector4 : Vector4
*/
LN_FLAT_API LNResult LNVector4_SetZeros(LNVector4* vector4);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector4 : Vector4
    @param[] x : 
    @param[] y : 
    @param[] z : 
    @param[] w : 
*/
LN_FLAT_API LNResult LNVector4_Set(LNVector4* vector4, float x, float y, float z, float w);


//==============================================================================
// ln::Quaternion

/**
    @brief 単位クォータニオンを設定してインスタンスを初期化します。
    @param[in] quaternion : Quaternion
*/
LN_FLAT_API LNResult LNQuaternion_SetZeros(LNQuaternion* quaternion);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] quaternion : Quaternion
    @param[] x : 
    @param[] y : 
    @param[] z : 
    @param[] w : 
*/
LN_FLAT_API LNResult LNQuaternion_Set(LNQuaternion* quaternion, float x, float y, float z, float w);


/**
    @brief 回転軸と回転角度を指定してインスタンスを初期化します。
    @param[in] quaternion : Quaternion
    @param[] axis : 
    @param[] r : 
    @details axis が単位ベクトルでなければ正規化してから計算を行います。
*/
LN_FLAT_API LNResult LNQuaternion_SetFromAxis(LNQuaternion* quaternion, const LNVector3* axis, float r);


//==============================================================================
// ln::Matrix

/**
    @brief 単位行列を設定してインスタンスを初期化します。
    @param[in] matrix : Matrix
*/
LN_FLAT_API LNResult LNMatrix_SetZeros(LNMatrix* matrix);


/**
    @brief 各要素を指定してインスタンスを初期化します。
    @param[in] matrix : Matrix
    @param[] m11 : 
    @param[] m12 : 
    @param[] m13 : 
    @param[] m14 : 
    @param[] m21 : 
    @param[] m22 : 
    @param[] m23 : 
    @param[] m24 : 
    @param[] m31 : 
    @param[] m32 : 
    @param[] m33 : 
    @param[] m34 : 
    @param[] m41 : 
    @param[] m42 : 
    @param[] m43 : 
    @param[] m44 : 
*/
LN_FLAT_API LNResult LNMatrix_Set(LNMatrix* matrix, float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);


//==============================================================================
// ln::Color

/**
    @brief すべての要素を 0.0 で初期化します。
    @param[in] color : Color
*/
LN_FLAT_API LNResult LNColor_SetZeros(LNColor* color);


/**
    @brief 各要素を指定して初期化します。
    @param[in] color : Color
    @param[] r_ : 
    @param[] g_ : 
    @param[] b_ : 
    @param[] a_ : 
*/
LN_FLAT_API LNResult LNColor_Set(LNColor* color, float r_, float g_, float b_, float a_);


//==============================================================================
// ln::ColorTone

/**
    @brief すべての要素を 0.0 で初期化します。
    @param[in] colortone : ColorTone
*/
LN_FLAT_API LNResult LNColorTone_SetZeros(LNColorTone* colortone);


/**
    @brief 各要素を指定して初期化します。
    @param[in] colortone : ColorTone
    @param[] r_ : 
    @param[] g_ : 
    @param[] b_ : 
    @param[] s_ : 
*/
LN_FLAT_API LNResult LNColorTone_Set(LNColorTone* colortone, float r_, float g_, float b_, float s_);


//==============================================================================
// ln::Point

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] point : Point
*/
LN_FLAT_API LNResult LNPoint_SetZeros(LNPoint* point);


/**
    @brief 位置を指定して初期化します。
    @param[in] point : Point
    @param[] x_ : 
    @param[] y_ : 
*/
LN_FLAT_API LNResult LNPoint_Set(LNPoint* point, float x_, float y_);


/**
    @brief 各要素の値を取得します。
    @param[in] point : Point
    @param[] outX : 
    @param[] outY : 
*/
LN_FLAT_API LNResult LNPoint_Get(const LNPoint* point, float* outX, float* outY);


//==============================================================================
// ln::Size

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] size : Size
*/
LN_FLAT_API LNResult LNSize_SetZeros(LNSize* size);


/**
    @brief 幅と高さを指定して初期化します。
    @param[in] size : Size
    @param[] w : 
    @param[] h : 
*/
LN_FLAT_API LNResult LNSize_Set(LNSize* size, float w, float h);


//==============================================================================
// ln::Rect

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] rect : Rect
*/
LN_FLAT_API LNResult LNRect_SetZeros(LNRect* rect);


/**
    @brief 位置とサイズを指定して初期化します。
    @param[in] rect : Rect
    @param[] x : 
    @param[] y : 
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNRect_Set(LNRect* rect, float x, float y, float width, float height);


/**
    @brief 左辺の x 軸の値を取得します。
    @param[in] rect : Rect
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNRect_GetLeft(const LNRect* rect, float* outReturn);


/**
    @brief 幅と高さを設定します。
    @param[in] rect : Rect
    @param[] size : 
*/
LN_FLAT_API LNResult LNRect_SetSize(LNRect* rect, const LNSize* size);


/**
    @brief 幅と高さを取得します。
    @param[in] rect : Rect
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNRect_GetSize(const LNRect* rect, LNSize* outReturn);


//==============================================================================
// ln::Thickness

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] thickness : Thickness
*/
LN_FLAT_API LNResult LNThickness_SetZeros(LNThickness* thickness);


/**
    @brief 各辺の幅を指定して初期化します。
    @param[in] thickness : Thickness
    @param[] left_ : 
    @param[] top_ : 
    @param[] right_ : 
    @param[] bottom_ : 
*/
LN_FLAT_API LNResult LNThickness_Set(LNThickness* thickness, float left_, float top_, float right_, float bottom_);


//==============================================================================
// ln::CornerRadius

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] cornerradius : CornerRadius
*/
LN_FLAT_API LNResult LNCornerRadius_SetZeros(LNCornerRadius* cornerradius);


/**
    @brief 各要素の値を指定して初期化します。
    @param[in] cornerradius : CornerRadius
    @param[] topLeft : 
    @param[] topRight : 
    @param[] bottomRight : 
    @param[] bottomLeft : 
*/
LN_FLAT_API LNResult LNCornerRadius_Set(LNCornerRadius* cornerradius, float topLeft, float topRight, float bottomRight, float bottomLeft);




//==============================================================================
// ln::Object

/**
    @brief オブジェクトの参照を開放します。
    @param[in] object : Object のハンドル
*/
LN_FLAT_API LNResult LNObject_Release(LNHandle object);

/**
    @brief オブジェクトの参照を取得します。
    @param[in] object : Object のハンドル
*/
LN_FLAT_API LNResult LNObject_Retain(LNHandle object);

/**
    @brief オブジェクトの参照カウントを取得します。これは内部的に使用される関数です。
    @param[in] object : Object のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNObject_GetReferenceCount(LNHandle object, int* outReturn);

typedef LNResult(*LNObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNObject_OnSerialize_SetOverrideCallback(LNObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] object : Object のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNObject_SetPrototype_OnSerialize(LNHandle object, LNHandle callback);

extern LN_FLAT_API int LNObject_GetTypeInfoId();
LN_FLAT_API void LNObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNObject_RegisterSubclassTypeInfo(const LNObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EventConnection

typedef LNResult(*LNEventConnection_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNEventConnection_OnSerialize_SetOverrideCallback(LNEventConnection_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNEventConnection_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] eventconnection : EventConnection のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNEventConnection_SetPrototype_OnSerialize(LNHandle eventconnection, LNHandle callback);

extern LN_FLAT_API int LNEventConnection_GetTypeInfoId();
LN_FLAT_API void LNEventConnection_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEventConnection_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNEventConnection_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNEventConnection_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEventConnection_RegisterSubclassTypeInfo(const LNEventConnection_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEventConnection_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PromiseFailureDelegate

LN_FLAT_API LNResult LNPromiseFailureDelegate_Create(LNPromiseFailureDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPromiseFailureDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPromiseFailureDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPromiseFailureDelegate_RegisterSubclassTypeInfo(const LNPromiseFailureDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPromiseFailureDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Variant

/**
    @brief init.
    @param[out] outVariant : 作成された Variant のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNVariant_Create(LNHandle* outVariant);

/**
    @brief setInt
    @param[in] variant : Variant のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNVariant_SetInt(LNHandle variant, int value);

/**
    @brief getInt
    @param[in] variant : Variant のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVariant_GetInt(LNHandle variant, int* outReturn);

typedef LNResult(*LNVariant_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNVariant_OnSerialize_SetOverrideCallback(LNVariant_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNVariant_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] variant : Variant のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNVariant_SetPrototype_OnSerialize(LNHandle variant, LNHandle callback);

extern LN_FLAT_API int LNVariant_GetTypeInfoId();
LN_FLAT_API void LNVariant_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVariant_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNVariant_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNVariant_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVariant_RegisterSubclassTypeInfo(const LNVariant_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVariant_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate1

LN_FLAT_API LNResult LNZVTestDelegate1_Create(LNZVTestDelegate1Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate1_RegisterSubclassTypeInfo(const LNZVTestDelegate1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate2

LN_FLAT_API LNResult LNZVTestDelegate2_Create(LNZVTestDelegate2Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate2_RegisterSubclassTypeInfo(const LNZVTestDelegate2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate3

LN_FLAT_API LNResult LNZVTestDelegate3_Create(LNZVTestDelegate3Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate3_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate3_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate3_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate3_RegisterSubclassTypeInfo(const LNZVTestDelegate3_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate3_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventHandler1

LN_FLAT_API LNResult LNZVTestEventHandler1_Create(LNZVTestEventHandler1Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventHandler1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventHandler1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventHandler1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventHandler1_RegisterSubclassTypeInfo(const LNZVTestEventHandler1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventHandler1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventHandler2

LN_FLAT_API LNResult LNZVTestEventHandler2_Create(LNZVTestEventHandler2Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventHandler2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventHandler2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventHandler2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventHandler2_RegisterSubclassTypeInfo(const LNZVTestEventHandler2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventHandler2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestPromise1

/**
    @brief 
    @param[in] zvtestpromise1 : ZVTestPromise1 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestPromise1_ThenWith(LNHandle zvtestpromise1, LNHandle callback);

/**
    @brief 
    @param[in] zvtestpromise1 : ZVTestPromise1 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestPromise1_CatchWith(LNHandle zvtestpromise1, LNHandle callback);

extern LN_FLAT_API int LNZVTestPromise1_GetTypeInfoId();
LN_FLAT_API void LNZVTestPromise1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestPromise1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestPromise1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestPromise1_RegisterSubclassTypeInfo(const LNZVTestPromise1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestPromise1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestPromise2

/**
    @brief 
    @param[in] zvtestpromise2 : ZVTestPromise2 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestPromise2_ThenWith(LNHandle zvtestpromise2, LNHandle callback);

/**
    @brief 
    @param[in] zvtestpromise2 : ZVTestPromise2 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestPromise2_CatchWith(LNHandle zvtestpromise2, LNHandle callback);

extern LN_FLAT_API int LNZVTestPromise2_GetTypeInfoId();
LN_FLAT_API void LNZVTestPromise2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestPromise2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestPromise2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestPromise2_RegisterSubclassTypeInfo(const LNZVTestPromise2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestPromise2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestClass1

/**
    @brief init method.
    @param[out] outZVTestClass1 : 作成された ZVTestClass1 のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNZVTestClass1_Create(LNHandle* outZVTestClass1);

/**
    @brief setTestDelegate1 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate1(LNHandle zvtestclass1, LNHandle value);

/**
    @brief setTestDelegate2 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate2(LNHandle zvtestclass1, LNHandle value);

/**
    @brief setTestDelegate3 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate3(LNHandle zvtestclass1, LNHandle value);

/**
    @brief callTestDelegate1 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] a : 
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate1(LNHandle zvtestclass1, int a);

/**
    @brief callTestDelegate2 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] a : 
    @param[] b : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate2(LNHandle zvtestclass1, int a, int b, int* outReturn);

/**
    @brief callTestDelegate3 method. (create object in internal)
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate3(LNHandle zvtestclass1);

/**
    @brief Promise test method. (static)
    @param[] filePath : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_LoadAsync(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNZVTestClass1_LoadAsyncA(const char* filePath, LNHandle* outReturn);

/**
    @brief Promise test method. (instance)
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ExecuteAsync(LNHandle zvtestclass1, LNHandle* outReturn);

/**
    @brief Promise test method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNZVTestClass1_GetFilePath(LNHandle zvtestclass1, const LNChar** outReturn);
LN_FLAT_API LNResult LNZVTestClass1_GetFilePathA(LNHandle zvtestclass1, const char** outReturn);

/**
    @brief connectOnEvent1 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent1(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn);

/**
    @brief raiseEvent1 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
*/
LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent1(LNHandle zvtestclass1);

/**
    @brief connectOnEvent2 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent2(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn);

/**
    @brief raiseEvent2 method.
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
*/
LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent2(LNHandle zvtestclass1);

typedef LNResult(*LNZVTestClass1_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_SetOverrideCallback(LNZVTestClass1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] zvtestclass1 : ZVTestClass1 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestClass1_SetPrototype_OnSerialize(LNHandle zvtestclass1, LNHandle callback);

extern LN_FLAT_API int LNZVTestClass1_GetTypeInfoId();
LN_FLAT_API void LNZVTestClass1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestClass1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNZVTestClass1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNZVTestClass1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestClass1_RegisterSubclassTypeInfo(const LNZVTestClass1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestClass1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventArgs1

/**
    @brief init method.
    @param[out] outZVTestEventArgs1 : 作成された ZVTestEventArgs1 のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_Create(LNHandle* outZVTestEventArgs1);

/**
    @brief init method.
    @param[] v : 
    @param[out] outZVTestEventArgs1 : 作成された ZVTestEventArgs1 のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_CreateWithValue(int v, LNHandle* outZVTestEventArgs1);

/**
    @brief value method.
    @param[in] zvtesteventargs1 : ZVTestEventArgs1 のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_GetValue(LNHandle zvtesteventargs1, int* outReturn);

typedef LNResult(*LNZVTestEventArgs1_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_SetOverrideCallback(LNZVTestEventArgs1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] zvtesteventargs1 : ZVTestEventArgs1 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_SetPrototype_OnSerialize(LNHandle zvtesteventargs1, LNHandle callback);

extern LN_FLAT_API int LNZVTestEventArgs1_GetTypeInfoId();
LN_FLAT_API void LNZVTestEventArgs1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventArgs1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNZVTestEventArgs1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNZVTestEventArgs1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventArgs1_RegisterSubclassTypeInfo(const LNZVTestEventArgs1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventArgs1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Log

/**
    @brief setLevel
    @param[] level : 
*/
LN_FLAT_API LNResult LNLog_SetLevel(LNLogLevel level);

/**
    @brief allocConsole
*/
LN_FLAT_API LNResult LNLog_AllocConsole();

/**
    @brief write
    @param[] level : 
    @param[] tag : 
    @param[] text : 
*/
LN_FLAT_API LNResult LNLog_Write(LNLogLevel level, const LNChar* tag, const LNChar* text);
LN_FLAT_API LNResult LNLog_WriteA(LNLogLevel level, const char* tag, const char* text);


//==============================================================================
// ln::Serializer2

typedef LNResult(*LNSerializer2_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSerializer2_OnSerialize_SetOverrideCallback(LNSerializer2_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSerializer2_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] serializer2 : Serializer2 のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSerializer2_SetPrototype_OnSerialize(LNHandle serializer2, LNHandle callback);

extern LN_FLAT_API int LNSerializer2_GetTypeInfoId();
LN_FLAT_API void LNSerializer2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSerializer2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSerializer2_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSerializer2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSerializer2_RegisterSubclassTypeInfo(const LNSerializer2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSerializer2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AssetObject

typedef LNResult(*LNAssetObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAssetObject_OnSerialize_SetOverrideCallback(LNAssetObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAssetObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] assetobject : AssetObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAssetObject_SetPrototype_OnSerialize(LNHandle assetobject, LNHandle callback);

extern LN_FLAT_API int LNAssetObject_GetTypeInfoId();
LN_FLAT_API void LNAssetObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAssetObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAssetObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetObject_RegisterSubclassTypeInfo(const LNAssetObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AssetImportSettings

typedef LNResult(*LNAssetImportSettings_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAssetImportSettings_OnSerialize_SetOverrideCallback(LNAssetImportSettings_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAssetImportSettings_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] assetimportsettings : AssetImportSettings のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAssetImportSettings_SetPrototype_OnSerialize(LNHandle assetimportsettings, LNHandle callback);

extern LN_FLAT_API int LNAssetImportSettings_GetTypeInfoId();
LN_FLAT_API void LNAssetImportSettings_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetImportSettings_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAssetImportSettings_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAssetImportSettings_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetImportSettings_RegisterSubclassTypeInfo(const LNAssetImportSettings_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetImportSettings_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AssetModel

/**
    @brief init
    @param[] target : 
    @param[out] outAssetModel : 作成された AssetModel のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNAssetModel_Create(LNHandle target, LNHandle* outAssetModel);

/**
    @brief target
    @param[in] assetmodel : AssetModel のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAssetModel_Target(LNHandle assetmodel, LNHandle* outReturn);

typedef LNResult(*LNAssetModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAssetModel_OnSerialize_SetOverrideCallback(LNAssetModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAssetModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] assetmodel : AssetModel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAssetModel_SetPrototype_OnSerialize(LNHandle assetmodel, LNHandle callback);

extern LN_FLAT_API int LNAssetModel_GetTypeInfoId();
LN_FLAT_API void LNAssetModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAssetModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAssetModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetModel_RegisterSubclassTypeInfo(const LNAssetModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Assets

/**
    @brief Internal
    @param[] asset : 
    @param[] filePath : 
*/
LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFile(LNHandle asset, const LNChar* filePath);
LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFileA(LNHandle asset, const char* filePath);

/**
    @brief Internal
    @param[] filePath : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFile(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFileA(const char* filePath, LNHandle* outReturn);

/**
    @brief 指定したアセットファイルを読み込み、オブジェクト生成します。
    @param[] filePath : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @details ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。
*/
LN_FLAT_API LNResult LNAssets_LoadAsset(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNAssets_LoadAssetA(const char* filePath, LNHandle* outReturn);

/**
    @brief 指定したアセットファイルを読み込み、作成済みのオブジェクトへ適用します。
    @param[] filePath : 
    @param[] obj : 
    @details このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。
*/
LN_FLAT_API LNResult LNAssets_ReloadAsset(const LNChar* filePath, LNHandle obj);
LN_FLAT_API LNResult LNAssets_ReloadAssetA(const char* filePath, LNHandle obj);

/**
    @brief readAllText
    @param[] filePath : 
    @param[] encoding : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAssets_ReadAllText(const LNChar* filePath, LNEncodingType encoding, const LNChar** outReturn);
LN_FLAT_API LNResult LNAssets_ReadAllTextA(const char* filePath, LNEncodingType encoding, const char** outReturn);


//==============================================================================
// ln::Sound

/**
    @brief この Sound の音量を設定します。
    @param[in] sound : Sound のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNSound_SetVolume(LNHandle sound, float value);

/**
    @brief この Sound の音量を取得します。
    @param[in] sound : Sound のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSound_GetVolume(LNHandle sound, float* outReturn);

/**
    @brief この Sound のピッチ (音高) を設定します。
    @param[in] sound : Sound のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNSound_SetPitch(LNHandle sound, float value);

/**
    @brief この Sound のピッチ (音高) を取得します。
    @param[in] sound : Sound のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSound_GetPitch(LNHandle sound, float* outReturn);

/**
    @brief ループ再生の有無を設定します。
    @param[in] sound : Sound のハンドル
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNSound_SetLoopEnabled(LNHandle sound, LNBool enabled);

/**
    @brief ループ再生が有効かを確認します。
    @param[in] sound : Sound のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSound_IsLoopEnabled(LNHandle sound, LNBool* outReturn);

/**
    @brief ループ範囲を設定します。
    @param[in] sound : Sound のハンドル
    @param[] begin : 
    @param[] length : 
*/
LN_FLAT_API LNResult LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length);

/**
    @brief この Sound の再生を開始します。
    @param[in] sound : Sound のハンドル
*/
LN_FLAT_API LNResult LNSound_Play(LNHandle sound);

/**
    @brief この Sound の再生を停止します。
    @param[in] sound : Sound のハンドル
*/
LN_FLAT_API LNResult LNSound_Stop(LNHandle sound);

/**
    @brief この Sound の再生を一時停止します。
    @param[in] sound : Sound のハンドル
*/
LN_FLAT_API LNResult LNSound_Pause(LNHandle sound);

/**
    @brief 一時停止中の再生を再開します。
    @param[in] sound : Sound のハンドル
*/
LN_FLAT_API LNResult LNSound_Resume(LNHandle sound);

/**
    @brief 音量のフェードを開始します。
    @param[in] sound : Sound のハンドル
    @param[] targetVolume : 
    @param[] time : 
    @param[] behavior : 
*/
LN_FLAT_API LNResult LNSound_FadeVolume(LNHandle sound, float targetVolume, float time, LNSoundFadeBehavior behavior);

typedef LNResult(*LNSound_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSound_OnSerialize_SetOverrideCallback(LNSound_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSound_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] sound : Sound のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSound_SetPrototype_OnSerialize(LNHandle sound, LNHandle callback);

extern LN_FLAT_API int LNSound_GetTypeInfoId();
LN_FLAT_API void LNSound_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSound_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSound_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSound_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSound_RegisterSubclassTypeInfo(const LNSound_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSound_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Audio

/**
    @brief BGM を演奏する
    @param[] filePath : 
    @param[] volume : 
    @param[] pitch : 
    @param[] fadeTime : 
*/
LN_FLAT_API LNResult LNAudio_PlayBGM(const LNChar* filePath, float volume, float pitch, double fadeTime);
LN_FLAT_API LNResult LNAudio_PlayBGMA(const char* filePath, float volume, float pitch, double fadeTime);

/**
    @brief BGM の演奏を停止する
    @param[] fadeTime : 
*/
LN_FLAT_API LNResult LNAudio_StopBGM(double fadeTime);

/**
    @brief BGS (環境音) を演奏する
    @param[] filePath : 
    @param[] volume : 
    @param[] pitch : 
    @param[] fadeTime : 
*/
LN_FLAT_API LNResult LNAudio_PlayBGS(const LNChar* filePath, float volume, float pitch, double fadeTime);
LN_FLAT_API LNResult LNAudio_PlayBGSA(const char* filePath, float volume, float pitch, double fadeTime);

/**
    @brief BGS の演奏を停止する
    @param[] fadeTime : 
*/
LN_FLAT_API LNResult LNAudio_StopBGS(double fadeTime);

/**
    @brief ME (効果音楽) を演奏する
    @param[] filePath : 
    @param[] volume : 
    @param[] pitch : 
*/
LN_FLAT_API LNResult LNAudio_PlayME(const LNChar* filePath, float volume, float pitch);
LN_FLAT_API LNResult LNAudio_PlayMEA(const char* filePath, float volume, float pitch);

/**
    @brief ME の演奏を停止する
*/
LN_FLAT_API LNResult LNAudio_StopME();

/**
    @brief SE を演奏する
    @param[] filePath : 
    @param[] volume : 
    @param[] pitch : 
*/
LN_FLAT_API LNResult LNAudio_PlaySE(const LNChar* filePath, float volume, float pitch);
LN_FLAT_API LNResult LNAudio_PlaySEA(const char* filePath, float volume, float pitch);

/**
    @brief SE を 3D 空間上で演奏する
    @param[] filePath : 
    @param[] position : 
    @param[] distance : 
    @param[] volume : 
    @param[] pitch : 
*/
LN_FLAT_API LNResult LNAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, float volume, float pitch);
LN_FLAT_API LNResult LNAudio_PlaySE3DA(const char* filePath, const LNVector3* position, float distance, float volume, float pitch);

/**
    @brief 全ての SE の演奏を停止する
*/
LN_FLAT_API LNResult LNAudio_StopSE();


//==============================================================================
// ln::Texture2DDelegate

LN_FLAT_API LNResult LNTexture2DDelegate_Create(LNTexture2DDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTexture2DDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DDelegate_RegisterSubclassTypeInfo(const LNTexture2DDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Texture2DPromise

/**
    @brief 
    @param[in] texture2dpromise : Texture2DPromise のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNTexture2DPromise_ThenWith(LNHandle texture2dpromise, LNHandle callback);

/**
    @brief 
    @param[in] texture2dpromise : Texture2DPromise のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNTexture2DPromise_CatchWith(LNHandle texture2dpromise, LNHandle callback);

extern LN_FLAT_API int LNTexture2DPromise_GetTypeInfoId();
LN_FLAT_API void LNTexture2DPromise_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DPromise_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DPromise_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DPromise_RegisterSubclassTypeInfo(const LNTexture2DPromise_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DPromise_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Graphics

/**
    @brief activeGraphicsAPI
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNGraphics_GetActiveGraphicsAPI(LNGraphicsAPI* outReturn);


//==============================================================================
// ln::Texture

typedef LNResult(*LNTexture_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNTexture_OnSerialize_SetOverrideCallback(LNTexture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNTexture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] texture : Texture のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNTexture_SetPrototype_OnSerialize(LNHandle texture, LNHandle callback);

extern LN_FLAT_API int LNTexture_GetTypeInfoId();
LN_FLAT_API void LNTexture_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNTexture_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNTexture_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture_RegisterSubclassTypeInfo(const LNTexture_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Texture2D

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[] width : 
    @param[] height : 
    @param[out] outTexture2D : 作成された Texture2D のハンドルを格納する変数。
    @return 作成されたテクスチャ
*/
LN_FLAT_API LNResult LNTexture2D_Create(int width, int height, LNHandle* outTexture2D);

/**
    @brief テクスチャを作成します。
    @param[] width : 
    @param[] height : 
    @param[] format : 
    @param[out] outTexture2D : 作成された Texture2D のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNTexture2D_CreateWithFormat(int width, int height, LNTextureFormat format, LNHandle* outTexture2D);

/**
    @brief アセットからテクスチャを読み込みます。
    @param[] filePath : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @details サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
*/
LN_FLAT_API LNResult LNTexture2D_Load(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNTexture2D_LoadA(const char* filePath, LNHandle* outReturn);

/**
    @brief loadEmoji
    @param[] code : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNTexture2D_LoadEmoji(const LNChar* code, LNHandle* outReturn);
LN_FLAT_API LNResult LNTexture2D_LoadEmojiA(const char* code, LNHandle* outReturn);

typedef LNResult(*LNTexture2D_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNTexture2D_OnSerialize_SetOverrideCallback(LNTexture2D_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNTexture2D_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] texture2d : Texture2D のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNTexture2D_SetPrototype_OnSerialize(LNHandle texture2d, LNHandle callback);

extern LN_FLAT_API int LNTexture2D_GetTypeInfoId();
LN_FLAT_API void LNTexture2D_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2D_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNTexture2D_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNTexture2D_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2D_RegisterSubclassTypeInfo(const LNTexture2D_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2D_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Shader

/**
    @brief load
    @param[] filePath : 
    @param[] settings : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNShader_Load(const LNChar* filePath, LNHandle settings, LNHandle* outReturn);
LN_FLAT_API LNResult LNShader_LoadA(const char* filePath, LNHandle settings, LNHandle* outReturn);

/**
    @brief 浮動小数点値を設定します。
    @param[in] shader : Shader のハンドル
    @param[] parameterName : 
    @param[] value : 
*/
LN_FLAT_API LNResult LNShader_SetFloat(LNHandle shader, const LNChar* parameterName, float value);
LN_FLAT_API LNResult LNShader_SetFloatA(LNHandle shader, const char* parameterName, float value);

/**
    @brief ベクトル値を設定します。
    @param[in] shader : Shader のハンドル
    @param[] parameterName : 
    @param[] value : 
*/
LN_FLAT_API LNResult LNShader_SetVector3(LNHandle shader, const LNChar* parameterName, const LNVector3* value);
LN_FLAT_API LNResult LNShader_SetVector3A(LNHandle shader, const char* parameterName, const LNVector3* value);

/**
    @brief ベクトル値を設定します。
    @param[in] shader : Shader のハンドル
    @param[] parameterName : 
    @param[] value : 
*/
LN_FLAT_API LNResult LNShader_SetVector4(LNHandle shader, const LNChar* parameterName, const LNVector4* value);
LN_FLAT_API LNResult LNShader_SetVector4A(LNHandle shader, const char* parameterName, const LNVector4* value);

/**
    @brief setTexture
    @param[in] shader : Shader のハンドル
    @param[] parameterName : 
    @param[] value : 
*/
LN_FLAT_API LNResult LNShader_SetTexture(LNHandle shader, const LNChar* parameterName, LNHandle value);
LN_FLAT_API LNResult LNShader_SetTextureA(LNHandle shader, const char* parameterName, LNHandle value);

typedef LNResult(*LNShader_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNShader_OnSerialize_SetOverrideCallback(LNShader_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNShader_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] shader : Shader のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNShader_SetPrototype_OnSerialize(LNHandle shader, LNHandle callback);

extern LN_FLAT_API int LNShader_GetTypeInfoId();
LN_FLAT_API void LNShader_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShader_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNShader_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNShader_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShader_RegisterSubclassTypeInfo(const LNShader_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShader_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::RenderView

typedef LNResult(*LNRenderView_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRenderView_OnSerialize_SetOverrideCallback(LNRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] renderview : RenderView のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNRenderView_SetPrototype_OnSerialize(LNHandle renderview, LNHandle callback);

extern LN_FLAT_API int LNRenderView_GetTypeInfoId();
LN_FLAT_API void LNRenderView_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRenderView_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRenderView_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRenderView_RegisterSubclassTypeInfo(const LNRenderView_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRenderView_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Material

/**
    @brief init
    @param[out] outMaterial : 作成された Material のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNMaterial_Create(LNHandle* outMaterial);

/**
    @brief mainTexture
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle value);

/**
    @brief mainTexture
    @param[in] material : Material のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMaterial_GetMainTexture(LNHandle material, LNHandle* outReturn);

/**
    @brief setColor
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetColor(LNHandle material, const LNColor* value);

/**
    @brief setRoughness
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetRoughness(LNHandle material, float value);

/**
    @brief setMetallic
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetMetallic(LNHandle material, float value);

/**
    @brief setEmissive
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetEmissive(LNHandle material, const LNColor* value);

/**
    @brief setShadingModel
    @param[in] material : Material のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMaterial_SetShadingModel(LNHandle material, LNShadingModel value);

/**
    @brief shadingModel
    @param[in] material : Material のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMaterial_GetShadingModel(LNHandle material, LNShadingModel* outReturn);

/**
    @brief shader
    @param[in] material : Material のハンドル
    @param[] shader : 
*/
LN_FLAT_API LNResult LNMaterial_SetShader(LNHandle material, LNHandle shader);

/**
    @brief shader
    @param[in] material : Material のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMaterial_GetShader(LNHandle material, LNHandle* outReturn);

typedef LNResult(*LNMaterial_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNMaterial_OnSerialize_SetOverrideCallback(LNMaterial_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNMaterial_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] material : Material のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNMaterial_SetPrototype_OnSerialize(LNHandle material, LNHandle callback);

extern LN_FLAT_API int LNMaterial_GetTypeInfoId();
LN_FLAT_API void LNMaterial_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMaterial_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNMaterial_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNMaterial_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMaterial_RegisterSubclassTypeInfo(const LNMaterial_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMaterial_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::MeshNode

/**
    @brief 可視状態を設定します。false の場合、このメッシュの描画は行われません。(default: true)
    @param[in] meshnode : MeshNode のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNMeshNode_SetVisible(LNHandle meshnode, LNBool value);

/**
    @brief 可視状態を取得します。
    @param[in] meshnode : MeshNode のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshNode_IsVisible(LNHandle meshnode, LNBool* outReturn);

typedef LNResult(*LNMeshNode_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNMeshNode_OnSerialize_SetOverrideCallback(LNMeshNode_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNMeshNode_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] meshnode : MeshNode のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNMeshNode_SetPrototype_OnSerialize(LNHandle meshnode, LNHandle callback);

extern LN_FLAT_API int LNMeshNode_GetTypeInfoId();
LN_FLAT_API void LNMeshNode_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshNode_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNMeshNode_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNMeshNode_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshNode_RegisterSubclassTypeInfo(const LNMeshNode_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshNode_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AnimationController

/**
    @brief アニメーションクリップを追加します。 (レイヤー0 へ追加されます)
    @param[in] animationcontroller : AnimationController のハンドル
    @param[] animationClip : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAnimationController_AddClip(LNHandle animationcontroller, LNHandle animationClip, LNHandle* outReturn);

/**
    @brief play
    @param[in] animationcontroller : AnimationController のハンドル
    @param[] state : 
    @param[] duration : 
*/
LN_FLAT_API LNResult LNAnimationController_Play(LNHandle animationcontroller, LNHandle state, float duration);

typedef LNResult(*LNAnimationController_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAnimationController_OnSerialize_SetOverrideCallback(LNAnimationController_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAnimationController_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] animationcontroller : AnimationController のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAnimationController_SetPrototype_OnSerialize(LNHandle animationcontroller, LNHandle callback);

extern LN_FLAT_API int LNAnimationController_GetTypeInfoId();
LN_FLAT_API void LNAnimationController_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationController_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAnimationController_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAnimationController_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationController_RegisterSubclassTypeInfo(const LNAnimationController_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationController_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::MeshModel

/**
    @brief load
    @param[] filePath : 
    @param[] settings : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNMeshModel_Load(const LNChar* filePath, LNHandle settings, LNHandle* outReturn);
LN_FLAT_API LNResult LNMeshModel_LoadA(const char* filePath, LNHandle settings, LNHandle* outReturn);

/**
    @brief findNode
    @param[in] meshmodel : MeshModel のハンドル
    @param[] name : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshModel_FindNode(LNHandle meshmodel, const LNChar* name, LNHandle* outReturn);
LN_FLAT_API LNResult LNMeshModel_FindNodeA(LNHandle meshmodel, const char* name, LNHandle* outReturn);

/**
    @brief findMaterial
    @param[in] meshmodel : MeshModel のハンドル
    @param[] name : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshModel_FindMaterial(LNHandle meshmodel, const LNChar* name, LNHandle* outReturn);
LN_FLAT_API LNResult LNMeshModel_FindMaterialA(LNHandle meshmodel, const char* name, LNHandle* outReturn);

/**
    @brief materialCount
    @param[in] meshmodel : MeshModel のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshModel_MaterialCount(LNHandle meshmodel, int* outReturn);

/**
    @brief load
    @param[in] meshmodel : MeshModel のハンドル
    @param[] index : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshModel_Material(LNHandle meshmodel, int index, LNHandle* outReturn);

/**
    @brief animationController
    @param[in] meshmodel : MeshModel のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMeshModel_GetAnimationController(LNHandle meshmodel, LNHandle* outReturn);

typedef LNResult(*LNMeshModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNMeshModel_OnSerialize_SetOverrideCallback(LNMeshModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNMeshModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] meshmodel : MeshModel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNMeshModel_SetPrototype_OnSerialize(LNHandle meshmodel, LNHandle callback);

extern LN_FLAT_API int LNMeshModel_GetTypeInfoId();
LN_FLAT_API void LNMeshModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNMeshModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNMeshModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshModel_RegisterSubclassTypeInfo(const LNMeshModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::MeshImportSettings

/**
    @brief init
    @param[out] outMeshImportSettings : 作成された MeshImportSettings のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNMeshImportSettings_Create(LNHandle* outMeshImportSettings);

typedef LNResult(*LNMeshImportSettings_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNMeshImportSettings_OnSerialize_SetOverrideCallback(LNMeshImportSettings_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNMeshImportSettings_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] meshimportsettings : MeshImportSettings のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNMeshImportSettings_SetPrototype_OnSerialize(LNHandle meshimportsettings, LNHandle callback);

extern LN_FLAT_API int LNMeshImportSettings_GetTypeInfoId();
LN_FLAT_API void LNMeshImportSettings_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshImportSettings_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNMeshImportSettings_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNMeshImportSettings_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshImportSettings_RegisterSubclassTypeInfo(const LNMeshImportSettings_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshImportSettings_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::SkinnedMeshModel

typedef LNResult(*LNSkinnedMeshModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSkinnedMeshModel_OnSerialize_SetOverrideCallback(LNSkinnedMeshModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSkinnedMeshModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] skinnedmeshmodel : SkinnedMeshModel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSkinnedMeshModel_SetPrototype_OnSerialize(LNHandle skinnedmeshmodel, LNHandle callback);

extern LN_FLAT_API int LNSkinnedMeshModel_GetTypeInfoId();
LN_FLAT_API void LNSkinnedMeshModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSkinnedMeshModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSkinnedMeshModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSkinnedMeshModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSkinnedMeshModel_RegisterSubclassTypeInfo(const LNSkinnedMeshModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSkinnedMeshModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::CollisionShape

typedef LNResult(*LNCollisionShape_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCollisionShape_OnSerialize_SetOverrideCallback(LNCollisionShape_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCollisionShape_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] collisionshape : CollisionShape のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCollisionShape_SetPrototype_OnSerialize(LNHandle collisionshape, LNHandle callback);

extern LN_FLAT_API int LNCollisionShape_GetTypeInfoId();
LN_FLAT_API void LNCollisionShape_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCollisionShape_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCollisionShape_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNCollisionShape_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCollisionShape_RegisterSubclassTypeInfo(const LNCollisionShape_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCollisionShape_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::BoxCollisionShape

/**
    @brief init
    @param[] size : 
    @param[out] outBoxCollisionShape : 作成された BoxCollisionShape のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNBoxCollisionShape_Create(const LNVector3* size, LNHandle* outBoxCollisionShape);

/**
    @brief init
    @param[] width : 
    @param[] height : 
    @param[] depth : 
    @param[out] outBoxCollisionShape : 作成された BoxCollisionShape のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNBoxCollisionShape_CreateWHD(float width, float height, float depth, LNHandle* outBoxCollisionShape);

typedef LNResult(*LNBoxCollisionShape_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNBoxCollisionShape_OnSerialize_SetOverrideCallback(LNBoxCollisionShape_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxCollisionShape_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] boxcollisionshape : BoxCollisionShape のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNBoxCollisionShape_SetPrototype_OnSerialize(LNHandle boxcollisionshape, LNHandle callback);

extern LN_FLAT_API int LNBoxCollisionShape_GetTypeInfoId();
LN_FLAT_API void LNBoxCollisionShape_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxCollisionShape_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNBoxCollisionShape_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNBoxCollisionShape_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxCollisionShape_RegisterSubclassTypeInfo(const LNBoxCollisionShape_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxCollisionShape_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AnimationCurve

/**
    @brief 指定した時間における値を評価します。
    @param[in] animationcurve : AnimationCurve のハンドル
    @param[] time : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAnimationCurve_Evaluate(LNHandle animationcurve, float time, float* outReturn);

typedef LNResult(*LNAnimationCurve_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAnimationCurve_OnSerialize_SetOverrideCallback(LNAnimationCurve_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAnimationCurve_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] animationcurve : AnimationCurve のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAnimationCurve_SetPrototype_OnSerialize(LNHandle animationcurve, LNHandle callback);

extern LN_FLAT_API int LNAnimationCurve_GetTypeInfoId();
LN_FLAT_API void LNAnimationCurve_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationCurve_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAnimationCurve_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAnimationCurve_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationCurve_RegisterSubclassTypeInfo(const LNAnimationCurve_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationCurve_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::KeyFrameAnimationCurve

/**
    @brief init
    @param[out] outKeyFrameAnimationCurve : 作成された KeyFrameAnimationCurve のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_Create(LNHandle* outKeyFrameAnimationCurve);

/**
    @brief キーフレームを追加します。
    @param[in] keyframeanimationcurve : KeyFrameAnimationCurve のハンドル
    @param[] time : 
    @param[] value : 
    @param[] rightTangentMode : 
    @param[] tangent : 
    @details rightTangentMode は、新しく追加するキーフレームの右側の補間方法です。新しく追加するキーフレームの左側の保管方法は、そのひとつ前のキーフレームの右側の保管方法が設定されます。
*/
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_AddKeyFrame(LNHandle keyframeanimationcurve, float time, float value, LNTangentMode rightTangentMode, float tangent);

typedef LNResult(*LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_OnSerialize_SetOverrideCallback(LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] keyframeanimationcurve : KeyFrameAnimationCurve のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_SetPrototype_OnSerialize(LNHandle keyframeanimationcurve, LNHandle callback);

extern LN_FLAT_API int LNKeyFrameAnimationCurve_GetTypeInfoId();
LN_FLAT_API void LNKeyFrameAnimationCurve_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNKeyFrameAnimationCurve_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNKeyFrameAnimationCurve_SubclassRegistrationInfo;

extern LN_FLAT_API void LNKeyFrameAnimationCurve_RegisterSubclassTypeInfo(const LNKeyFrameAnimationCurve_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNKeyFrameAnimationCurve_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AnimationClip

/**
    @brief load
    @param[] filePath : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNAnimationClip_Load(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNAnimationClip_LoadA(const char* filePath, LNHandle* outReturn);

/**
    @brief アニメーションの繰り返しの動作を取得します。(default: Loop)
    @param[in] animationclip : AnimationClip のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNAnimationClip_SetWrapMode(LNHandle animationclip, LNAnimationWrapMode value);

/**
    @brief アニメーションの繰り返しの動作を取得します。
    @param[in] animationclip : AnimationClip のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAnimationClip_GetWrapMode(LNHandle animationclip, LNAnimationWrapMode* outReturn);

/**
    @brief 階層構造を持つアニメーションデータの動作モード。(default: AllowTranslationOnlyRoot)
    @param[in] animationclip : AnimationClip のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNAnimationClip_SetHierarchicalAnimationMode(LNHandle animationclip, LNHierarchicalAnimationMode value);

/**
    @brief 階層構造を持つアニメーションデータの動作モード。
    @param[in] animationclip : AnimationClip のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNAnimationClip_GetHierarchicalAnimationMode(LNHandle animationclip, LNHierarchicalAnimationMode* outReturn);

typedef LNResult(*LNAnimationClip_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAnimationClip_OnSerialize_SetOverrideCallback(LNAnimationClip_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAnimationClip_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] animationclip : AnimationClip のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAnimationClip_SetPrototype_OnSerialize(LNHandle animationclip, LNHandle callback);

extern LN_FLAT_API int LNAnimationClip_GetTypeInfoId();
LN_FLAT_API void LNAnimationClip_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationClip_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAnimationClip_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAnimationClip_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationClip_RegisterSubclassTypeInfo(const LNAnimationClip_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationClip_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AnimationState

typedef LNResult(*LNAnimationState_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAnimationState_OnSerialize_SetOverrideCallback(LNAnimationState_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAnimationState_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] animationstate : AnimationState のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNAnimationState_SetPrototype_OnSerialize(LNHandle animationstate, LNHandle callback);

extern LN_FLAT_API int LNAnimationState_GetTypeInfoId();
LN_FLAT_API void LNAnimationState_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationState_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAnimationState_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAnimationState_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationState_RegisterSubclassTypeInfo(const LNAnimationState_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationState_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EffectResource

typedef LNResult(*LNEffectResource_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNEffectResource_OnSerialize_SetOverrideCallback(LNEffectResource_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNEffectResource_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] effectresource : EffectResource のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNEffectResource_SetPrototype_OnSerialize(LNHandle effectresource, LNHandle callback);

extern LN_FLAT_API int LNEffectResource_GetTypeInfoId();
LN_FLAT_API void LNEffectResource_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEffectResource_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNEffectResource_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNEffectResource_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEffectResource_RegisterSubclassTypeInfo(const LNEffectResource_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEffectResource_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ParticleEmitterModel

/**
    @brief init
    @param[out] outParticleEmitterModel : 作成された ParticleEmitterModel のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNParticleEmitterModel_Create(LNHandle* outParticleEmitterModel);

/**
    @brief 同時に表示できるパーティクルの最大数を設定します。(default: 100)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] count : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetMaxParticles(LNHandle particleemittermodel, int count);

/**
    @brief 1秒間に放出するパーティクルの数を設定します。(default: 1)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] rate : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetSpawnRate(LNHandle particleemittermodel, float rate);

/**
    @brief パーティクルの生存時間を設定します。(default: 5.0)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] time : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetLifeTime(LNHandle particleemittermodel, float time);

/**
    @brief setupBoxShape
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] size : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetupBoxShape(LNHandle particleemittermodel, const LNVector3* size);

/**
    @brief (default: 1.0)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetSize(LNHandle particleemittermodel, float value);

/**
    @brief 
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetSizeVelocity(LNHandle particleemittermodel, float value);

/**
    @brief 
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetSizeAcceleration(LNHandle particleemittermodel, float value);

/**
    @brief (default: 0)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardVelocityMin(LNHandle particleemittermodel, float value);

/**
    @brief (default: 0)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardVelocityMax(LNHandle particleemittermodel, float value);

/**
    @brief 進行方向に対するスケール値。通常、Z軸。ParticleGeometryDirection::ToView では Y scale (default: 1.0)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardScale(LNHandle particleemittermodel, float value);

/**
    @brief (default: ToView)
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetGeometryDirection(LNHandle particleemittermodel, LNParticleGeometryDirection value);

/**
    @brief setupSpriteModule
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] material : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetupSpriteModule(LNHandle particleemittermodel, LNHandle material);

typedef LNResult(*LNParticleEmitterModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNParticleEmitterModel_OnSerialize_SetOverrideCallback(LNParticleEmitterModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNParticleEmitterModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] particleemittermodel : ParticleEmitterModel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNParticleEmitterModel_SetPrototype_OnSerialize(LNHandle particleemittermodel, LNHandle callback);

extern LN_FLAT_API int LNParticleEmitterModel_GetTypeInfoId();
LN_FLAT_API void LNParticleEmitterModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitterModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNParticleEmitterModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNParticleEmitterModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitterModel_RegisterSubclassTypeInfo(const LNParticleEmitterModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitterModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ParticleModel

/**
    @brief init
    @param[out] outParticleModel : 作成された ParticleModel のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNParticleModel_Create(LNHandle* outParticleModel);

/**
    @brief setLoop
    @param[in] particlemodel : ParticleModel のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNParticleModel_SetLoop(LNHandle particlemodel, LNBool value);

/**
    @brief setLoop
    @param[in] particlemodel : ParticleModel のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNParticleModel_IsLoop(LNHandle particlemodel, LNBool* outReturn);

/**
    @brief setLoop
    @param[in] particlemodel : ParticleModel のハンドル
    @param[] emitter : 
*/
LN_FLAT_API LNResult LNParticleModel_AddEmitter(LNHandle particlemodel, LNHandle emitter);

typedef LNResult(*LNParticleModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNParticleModel_OnSerialize_SetOverrideCallback(LNParticleModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNParticleModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] particlemodel : ParticleModel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNParticleModel_SetPrototype_OnSerialize(LNHandle particlemodel, LNHandle callback);

extern LN_FLAT_API int LNParticleModel_GetTypeInfoId();
LN_FLAT_API void LNParticleModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNParticleModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNParticleModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleModel_RegisterSubclassTypeInfo(const LNParticleModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Component

typedef LNResult(*LNComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNComponent_OnSerialize_SetOverrideCallback(LNComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] component : Component のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNComponent_SetPrototype_OnSerialize(LNHandle component, LNHandle callback);

extern LN_FLAT_API int LNComponent_GetTypeInfoId();
LN_FLAT_API void LNComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponent_RegisterSubclassTypeInfo(const LNComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::VisualComponent

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualcomponent : VisualComponent のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNVisualComponent_SetVisible(LNHandle visualcomponent, LNBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualcomponent : VisualComponent のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVisualComponent_IsVisible(LNHandle visualcomponent, LNBool* outReturn);

typedef LNResult(*LNVisualComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNVisualComponent_OnSerialize_SetOverrideCallback(LNVisualComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] visualcomponent : VisualComponent のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNVisualComponent_SetPrototype_OnSerialize(LNHandle visualcomponent, LNHandle callback);

extern LN_FLAT_API int LNVisualComponent_GetTypeInfoId();
LN_FLAT_API void LNVisualComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNVisualComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNVisualComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualComponent_RegisterSubclassTypeInfo(const LNVisualComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::SpriteComponent

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] spritecomponent : SpriteComponent のハンドル
    @param[] texture : 
*/
LN_FLAT_API LNResult LNSpriteComponent_SetTexture(LNHandle spritecomponent, LNHandle texture);

typedef LNResult(*LNSpriteComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_SetOverrideCallback(LNSpriteComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] spritecomponent : SpriteComponent のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSpriteComponent_SetPrototype_OnSerialize(LNHandle spritecomponent, LNHandle callback);

extern LN_FLAT_API int LNSpriteComponent_GetTypeInfoId();
LN_FLAT_API void LNSpriteComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSpriteComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSpriteComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteComponent_RegisterSubclassTypeInfo(const LNSpriteComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::CollisionEventHandler

LN_FLAT_API LNResult LNCollisionEventHandler_Create(LNCollisionEventHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCollisionEventHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCollisionEventHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCollisionEventHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCollisionEventHandler_RegisterSubclassTypeInfo(const LNCollisionEventHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCollisionEventHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::CharacterController

/**
    @brief CharacterController を作成します。
    @param[out] outCharacterController : 作成された CharacterController のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNCharacterController_Create(LNHandle* outCharacterController);

/**
    @brief walkVelocity
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNCharacterController_SetWalkVelocity(LNHandle charactercontroller, float value);

/**
    @brief walkVelocity
    @param[in] charactercontroller : CharacterController のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNCharacterController_GetWalkVelocity(LNHandle charactercontroller, float* outReturn);

/**
    @brief setVelocity
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNCharacterController_SetVelocity(LNHandle charactercontroller, const LNVector3* value);

/**
    @brief velocity
    @param[in] charactercontroller : CharacterController のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNCharacterController_GetVelocity(LNHandle charactercontroller, LNVector3* outReturn);

/**
    @brief キーボードやゲームパッドによる操作の有効状態を設定します。 (default: true)
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
    @details false を指定した場合、キャラクターの想定外の自走を防止するため、速度も 0 にリセットされます。
*/
LN_FLAT_API LNResult LNCharacterController_SetInputControlEnabled(LNHandle charactercontroller, LNBool value);

/**
    @brief マウスによるカメラ操作の有効状態を設定します。 (default: true)
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
    @details 有効である場合、関連付けられているカメラを通じて、描画先となるビューの MouseGrab を取得します。つまり、マウスカーソルは非表示となり UI をポイントしてクリックする等の操作はできなくなります。
*/
LN_FLAT_API LNResult LNCharacterController_SetCameraControlEnabled(LNHandle charactercontroller, LNBool value);

/**
    @brief キャラクターの高さを設定します。この値はカプセルコライダーの高さや、カメラの注視点として使用されます。 (default: 2.0)
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNCharacterController_SetHeight(LNHandle charactercontroller, float value);

/**
    @brief キャラクターの高さを取得します。
    @param[in] charactercontroller : CharacterController のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNCharacterController_GetHeight(LNHandle charactercontroller, float* outReturn);

/**
    @brief キャラクターとカメラの距離を設定します。(default: 5.0)
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNCharacterController_SetCameraRadius(LNHandle charactercontroller, float value);

/**
    @brief キャラクターとカメラの距離を取得します。
    @param[in] charactercontroller : CharacterController のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNCharacterController_GetCameraRadius(LNHandle charactercontroller, float* outReturn);

/**
    @brief CollisionEnter イベントの通知を受け取るコールバックを登録します。
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNCharacterController_SetCollisionEnter(LNHandle charactercontroller, LNHandle handler);

/**
    @brief CollisionLeave イベントの通知を受け取るコールバックを登録します。
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNCharacterController_SetCollisionLeave(LNHandle charactercontroller, LNHandle handler);

/**
    @brief CollisionStay イベントの通知を受け取るコールバックを登録します。
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNCharacterController_SetCollisionStay(LNHandle charactercontroller, LNHandle handler);

typedef LNResult(*LNCharacterController_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCharacterController_OnSerialize_SetOverrideCallback(LNCharacterController_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCharacterController_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] charactercontroller : CharacterController のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCharacterController_SetPrototype_OnSerialize(LNHandle charactercontroller, LNHandle callback);

extern LN_FLAT_API int LNCharacterController_GetTypeInfoId();
LN_FLAT_API void LNCharacterController_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCharacterController_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCharacterController_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNCharacterController_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCharacterController_RegisterSubclassTypeInfo(const LNCharacterController_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCharacterController_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::World

/**
    @brief オブジェクトを World に追加します。
    @param[in] world : World のハンドル
    @param[] obj : 
*/
LN_FLAT_API LNResult LNWorld_Add(LNHandle world, LNHandle obj);

typedef LNResult(*LNWorld_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorld_OnSerialize_SetOverrideCallback(LNWorld_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorld_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] world : World のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNWorld_SetPrototype_OnSerialize(LNHandle world, LNHandle callback);

extern LN_FLAT_API int LNWorld_GetTypeInfoId();
LN_FLAT_API void LNWorld_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorld_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorld_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNWorld_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorld_RegisterSubclassTypeInfo(const LNWorld_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorld_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ComponentList

/**
    @brief 
    @param[in] componentlist : ComponentList のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNComponentList_GetLength(LNHandle componentlist, int* outReturn);

/**
    @brief 
    @param[in] componentlist : ComponentList のハンドル
    @param[] index : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNComponentList_GetItem(LNHandle componentlist, int index, LNHandle* outReturn);

typedef LNResult(*LNComponentList_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNComponentList_OnSerialize_SetOverrideCallback(LNComponentList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNComponentList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] componentlist : ComponentList のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNComponentList_SetPrototype_OnSerialize(LNHandle componentlist, LNHandle callback);

extern LN_FLAT_API int LNComponentList_GetTypeInfoId();
LN_FLAT_API void LNComponentList_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentList_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNComponentList_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNComponentList_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentList_RegisterSubclassTypeInfo(const LNComponentList_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentList_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::WorldObject

/**
    @brief WorldObject を作成します。
    @param[out] outWorldObject : 作成された WorldObject のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_Create(LNHandle* outWorldObject);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] pos : 
*/
LN_FLAT_API LNResult LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNWorldObject_SetPositionXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] worldobject : WorldObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_GetPosition(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] rot : 
*/
LN_FLAT_API LNResult LNWorldObject_SetRotation(LNHandle worldobject, const LNQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します(radian単位) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。
    @param[in] worldobject : WorldObject のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNWorldObject_SetRotationXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] worldobject : WorldObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_GetRotation(LNHandle worldobject, LNQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] scale : 
*/
LN_FLAT_API LNResult LNWorldObject_SetScale(LNHandle worldobject, const LNVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] xyz : 
*/
LN_FLAT_API LNResult LNWorldObject_SetScaleS(LNHandle worldobject, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNWorldObject_SetScaleXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] worldobject : WorldObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_GetScale(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNWorldObject_SetCenterPoint(LNHandle worldobject, const LNVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNWorldObject_SetCenterPointXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] worldobject : WorldObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_GetCenterPoint(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : WorldObject のハンドル
    @param[] target : 
*/
LN_FLAT_API LNResult LNWorldObject_LookAt(LNHandle worldobject, const LNVector3* target);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : WorldObject のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNWorldObject_LookAtXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief Component を追加します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] component : 
*/
LN_FLAT_API LNResult LNWorldObject_AddComponent(LNHandle worldobject, LNHandle component);

/**
    @brief Component を除外します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] component : 
*/
LN_FLAT_API LNResult LNWorldObject_RemoveComponent(LNHandle worldobject, LNHandle component);

/**
    @brief タグを追加します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] tag : 
*/
LN_FLAT_API LNResult LNWorldObject_AddTag(LNHandle worldobject, const LNChar* tag);
LN_FLAT_API LNResult LNWorldObject_AddTagA(LNHandle worldobject, const char* tag);

/**
    @brief タグを除外します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] tag : 
*/
LN_FLAT_API LNResult LNWorldObject_RemoveTag(LNHandle worldobject, const LNChar* tag);
LN_FLAT_API LNResult LNWorldObject_RemoveTagA(LNHandle worldobject, const char* tag);

/**
    @brief 指定したタグを持っているかを確認します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] tag : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_HasTag(LNHandle worldobject, const LNChar* tag, LNBool* outReturn);
LN_FLAT_API LNResult LNWorldObject_HasTagA(LNHandle worldobject, const char* tag, LNBool* outReturn);

/**
    @brief WorldObject を破棄します。
    @param[in] worldobject : WorldObject のハンドル
    @details 実際の削除は、現在のフレームのアップデート処理後に行われます。削除された WorldObject は、親の World, Level, WorldObject からも除外されます。
*/
LN_FLAT_API LNResult LNWorldObject_Destroy(LNHandle worldobject);

/**
    @brief 
    @param[in] worldobject : WorldObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldObject_GetComponents(LNHandle worldobject, LNHandle* outReturn);

/**
    @brief この WorldObject を指定した World へ追加します。省略した場合はデフォルトの World へ追加します。
    @param[in] worldobject : WorldObject のハンドル
    @param[] world : 
*/
LN_FLAT_API LNResult LNWorldObject_AddInto(LNHandle worldobject, LNHandle world);

typedef LNResult(*LNWorldObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorldObject_OnSerialize_SetOverrideCallback(LNWorldObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNWorldObject_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNWorldObject_OnPreUpdate_SetOverrideCallback(LNWorldObject_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNWorldObject_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNWorldObject_OnUpdate_SetOverrideCallback(LNWorldObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] worldobject : WorldObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnSerialize(LNHandle worldobject, LNHandle callback);

/**
    @brief 
    @param[in] worldobject : WorldObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnPreUpdate(LNHandle worldobject, LNHandle callback);

/**
    @brief 
    @param[in] worldobject : WorldObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnUpdate(LNHandle worldobject, LNHandle callback);

extern LN_FLAT_API int LNWorldObject_GetTypeInfoId();
LN_FLAT_API void LNWorldObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorldObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNWorldObject_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNWorldObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNWorldObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObject_RegisterSubclassTypeInfo(const LNWorldObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::VisualObject

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualobject : VisualObject のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNVisualObject_SetVisible(LNHandle visualobject, LNBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualobject : VisualObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVisualObject_IsVisible(LNHandle visualobject, LNBool* outReturn);

/**
    @brief 合成方法を設定します。(default: BlendMode::Normal)
    @param[in] visualobject : VisualObject のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNVisualObject_SetBlendMode2(LNHandle visualobject, LNBlendMode value);

/**
    @brief 不透明度を設定します。(default: 1.0)
    @param[in] visualobject : VisualObject のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNVisualObject_SetOpacity(LNHandle visualobject, float value);

/**
    @brief 不透明度を取得します。
    @param[in] visualobject : VisualObject のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNVisualObject_GetOpacity(LNHandle visualobject, float* outReturn);

typedef LNResult(*LNVisualObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNVisualObject_OnSerialize_SetOverrideCallback(LNVisualObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNVisualObject_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNVisualObject_OnPreUpdate_SetOverrideCallback(LNVisualObject_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNVisualObject_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNVisualObject_OnUpdate_SetOverrideCallback(LNVisualObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] visualobject : VisualObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnSerialize(LNHandle visualobject, LNHandle callback);

/**
    @brief 
    @param[in] visualobject : VisualObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnPreUpdate(LNHandle visualobject, LNHandle callback);

/**
    @brief 
    @param[in] visualobject : VisualObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnUpdate(LNHandle visualobject, LNHandle callback);

extern LN_FLAT_API int LNVisualObject_GetTypeInfoId();
LN_FLAT_API void LNVisualObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNVisualObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNVisualObject_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNVisualObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNVisualObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObject_RegisterSubclassTypeInfo(const LNVisualObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Camera

typedef LNResult(*LNCamera_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCamera_OnSerialize_SetOverrideCallback(LNCamera_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCamera_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNCamera_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNCamera_OnPreUpdate_SetOverrideCallback(LNCamera_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNCamera_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNCamera_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNCamera_OnUpdate_SetOverrideCallback(LNCamera_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNCamera_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] camera : Camera のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCamera_SetPrototype_OnSerialize(LNHandle camera, LNHandle callback);

/**
    @brief 
    @param[in] camera : Camera のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCamera_SetPrototype_OnPreUpdate(LNHandle camera, LNHandle callback);

/**
    @brief 
    @param[in] camera : Camera のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCamera_SetPrototype_OnUpdate(LNHandle camera, LNHandle callback);

extern LN_FLAT_API int LNCamera_GetTypeInfoId();
LN_FLAT_API void LNCamera_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCamera_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCamera_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNCamera_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNCamera_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNCamera_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCamera_RegisterSubclassTypeInfo(const LNCamera_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCamera_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EnvironmentLight

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetEnabled(LNHandle environmentlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_IsEnabled(LNHandle environmentlight, LNBool* outReturn);

/**
    @brief ディレクショナルライトの光源色を設定します。(default: White)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief ディレクショナルライトの光源色を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief シーン全体の環境光の色を設定します。(default: 0.5, 0.5, 0.5)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetAmbientColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief シーン全体の環境光の色を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetAmbientColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 空の環境光の色を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetSkyColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 空の環境光の色を設定します。(default: Black)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetSkyColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief 地面の環境光の色を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetGroundColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 地面の環境光の色を設定します。(default: Black)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetGroundColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief ライトの明るさを設定します。(default: 0.5)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] intensity : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetIntensity(LNHandle environmentlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetIntensity(LNHandle environmentlight, float* outReturn);

/**
    @brief 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDistance(LNHandle environmentlight, float value);

/**
    @brief 視点からの、影を生成できる距離を取得します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDistance(LNHandle environmentlight, float* outReturn);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDepth(LNHandle environmentlight, float value);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDepth(LNHandle environmentlight, float* outReturn);

typedef LNResult(*LNEnvironmentLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_SetOverrideCallback(LNEnvironmentLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNEnvironmentLight_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNEnvironmentLight_OnPreUpdate_SetOverrideCallback(LNEnvironmentLight_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNEnvironmentLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNEnvironmentLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_SetOverrideCallback(LNEnvironmentLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnSerialize(LNHandle environmentlight, LNHandle callback);

/**
    @brief 
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnPreUpdate(LNHandle environmentlight, LNHandle callback);

/**
    @brief 
    @param[in] environmentlight : EnvironmentLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnUpdate(LNHandle environmentlight, LNHandle callback);

extern LN_FLAT_API int LNEnvironmentLight_GetTypeInfoId();
LN_FLAT_API void LNEnvironmentLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNEnvironmentLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNEnvironmentLight_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNEnvironmentLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNEnvironmentLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLight_RegisterSubclassTypeInfo(const LNEnvironmentLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::DirectionalLight

/**
    @brief 既定の設定でディレクショナルライトを作成します。
    @param[out] outDirectionalLight : 作成された DirectionalLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_Create(LNHandle* outDirectionalLight);

/**
    @brief 色を指定してディレクショナルライトを作成します。
    @param[] color : 
    @param[out] outDirectionalLight : 作成された DirectionalLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_CreateWithColor(const LNColor* color, LNHandle* outDirectionalLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetEnabled(LNHandle directionallight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_IsEnabled(LNHandle directionallight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] color : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetColor(LNHandle directionallight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_GetColor(LNHandle directionallight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 0.5)
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] intensity : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetIntensity(LNHandle directionallight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_GetIntensity(LNHandle directionallight, float* outReturn);

/**
    @brief 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDistance(LNHandle directionallight, float value);

/**
    @brief 視点からの、影を生成できる距離を取得します。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDistance(LNHandle directionallight, float* outReturn);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDepth(LNHandle directionallight, float value);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。
    @param[in] directionallight : DirectionalLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDepth(LNHandle directionallight, float* outReturn);

typedef LNResult(*LNDirectionalLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_SetOverrideCallback(LNDirectionalLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNDirectionalLight_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNDirectionalLight_OnPreUpdate_SetOverrideCallback(LNDirectionalLight_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNDirectionalLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNDirectionalLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_SetOverrideCallback(LNDirectionalLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnSerialize(LNHandle directionallight, LNHandle callback);

/**
    @brief 
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnPreUpdate(LNHandle directionallight, LNHandle callback);

/**
    @brief 
    @param[in] directionallight : DirectionalLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnUpdate(LNHandle directionallight, LNHandle callback);

extern LN_FLAT_API int LNDirectionalLight_GetTypeInfoId();
LN_FLAT_API void LNDirectionalLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNDirectionalLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNDirectionalLight_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNDirectionalLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNDirectionalLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLight_RegisterSubclassTypeInfo(const LNDirectionalLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PointLight

/**
    @brief 既定の設定でポイントライトを作成します。
    @param[out] outPointLight : 作成された PointLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_Create(LNHandle* outPointLight);

/**
    @brief 色と範囲を指定してポイントライトを作成します。
    @param[] color : 
    @param[] range : 
    @param[out] outPointLight : 作成された PointLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_CreateWithColorAndRange(const LNColor* color, float range, LNHandle* outPointLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] pointlight : PointLight のハンドル
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNPointLight_SetEnabled(LNHandle pointlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] pointlight : PointLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_IsEnabled(LNHandle pointlight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] pointlight : PointLight のハンドル
    @param[] color : 
*/
LN_FLAT_API LNResult LNPointLight_SetColor(LNHandle pointlight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] pointlight : PointLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_GetColor(LNHandle pointlight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] pointlight : PointLight のハンドル
    @param[] intensity : 
*/
LN_FLAT_API LNResult LNPointLight_SetIntensity(LNHandle pointlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] pointlight : PointLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_GetIntensity(LNHandle pointlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] pointlight : PointLight のハンドル
    @param[] range : 
*/
LN_FLAT_API LNResult LNPointLight_SetRange(LNHandle pointlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] pointlight : PointLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_GetRange(LNHandle pointlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] pointlight : PointLight のハンドル
    @param[] attenuation : 
*/
LN_FLAT_API LNResult LNPointLight_SetAttenuation(LNHandle pointlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] pointlight : PointLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNPointLight_GetAttenuation(LNHandle pointlight, float* outReturn);

typedef LNResult(*LNPointLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNPointLight_OnSerialize_SetOverrideCallback(LNPointLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNPointLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNPointLight_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNPointLight_OnPreUpdate_SetOverrideCallback(LNPointLight_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPointLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNPointLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNPointLight_OnUpdate_SetOverrideCallback(LNPointLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPointLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] pointlight : PointLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnSerialize(LNHandle pointlight, LNHandle callback);

/**
    @brief 
    @param[in] pointlight : PointLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnPreUpdate(LNHandle pointlight, LNHandle callback);

/**
    @brief 
    @param[in] pointlight : PointLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnUpdate(LNHandle pointlight, LNHandle callback);

extern LN_FLAT_API int LNPointLight_GetTypeInfoId();
LN_FLAT_API void LNPointLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNPointLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNPointLight_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNPointLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNPointLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLight_RegisterSubclassTypeInfo(const LNPointLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::SpotLight

/**
    @brief 既定の設定でスポットライトを作成します。
    @param[out] outSpotLight : 作成された SpotLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_Create(LNHandle* outSpotLight);

/**
    @brief 色と範囲を指定してスポットライトを作成します。
    @param[] color : 
    @param[] range : 
    @param[] angle : 
    @param[out] outSpotLight : 作成された SpotLight のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_CreateWithColorAndRange(const LNColor* color, float range, float angle, LNHandle* outSpotLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] spotlight : SpotLight のハンドル
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNSpotLight_SetEnabled(LNHandle spotlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_IsEnabled(LNHandle spotlight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] spotlight : SpotLight のハンドル
    @param[] color : 
*/
LN_FLAT_API LNResult LNSpotLight_SetColor(LNHandle spotlight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetColor(LNHandle spotlight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] spotlight : SpotLight のハンドル
    @param[] intensity : 
*/
LN_FLAT_API LNResult LNSpotLight_SetIntensity(LNHandle spotlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetIntensity(LNHandle spotlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] spotlight : SpotLight のハンドル
    @param[] range : 
*/
LN_FLAT_API LNResult LNSpotLight_SetRange(LNHandle spotlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetRange(LNHandle spotlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] spotlight : SpotLight のハンドル
    @param[] attenuation : 
*/
LN_FLAT_API LNResult LNSpotLight_SetAttenuation(LNHandle spotlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetAttenuation(LNHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3)
    @param[in] spotlight : SpotLight のハンドル
    @param[] angle : 
*/
LN_FLAT_API LNResult LNSpotLight_SetAngle(LNHandle spotlight, float angle);

/**
    @brief スポットライトのコーン角度を取得します。(ラジアン単位)
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetAngle(LNHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0)
    @param[in] spotlight : SpotLight のハンドル
    @param[] penumbra : 
*/
LN_FLAT_API LNResult LNSpotLight_SetPenumbra(LNHandle spotlight, float penumbra);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。
    @param[in] spotlight : SpotLight のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNSpotLight_GetPenumbra(LNHandle spotlight, float* outReturn);

typedef LNResult(*LNSpotLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSpotLight_OnSerialize_SetOverrideCallback(LNSpotLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSpotLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNSpotLight_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNSpotLight_OnPreUpdate_SetOverrideCallback(LNSpotLight_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSpotLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNSpotLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNSpotLight_OnUpdate_SetOverrideCallback(LNSpotLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSpotLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] spotlight : SpotLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnSerialize(LNHandle spotlight, LNHandle callback);

/**
    @brief 
    @param[in] spotlight : SpotLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnPreUpdate(LNHandle spotlight, LNHandle callback);

/**
    @brief 
    @param[in] spotlight : SpotLight のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnUpdate(LNHandle spotlight, LNHandle callback);

extern LN_FLAT_API int LNSpotLight_GetTypeInfoId();
LN_FLAT_API void LNSpotLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSpotLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNSpotLight_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNSpotLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNSpotLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLight_RegisterSubclassTypeInfo(const LNSpotLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::TestDelegate

LN_FLAT_API LNResult LNTestDelegate_Create(LNTestDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTestDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTestDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTestDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTestDelegate_RegisterSubclassTypeInfo(const LNTestDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTestDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Sprite

/**
    @brief init
    @param[out] outSprite : 作成された Sprite のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNSprite_Create(LNHandle* outSprite);

/**
    @brief init
    @param[] texture : 
    @param[out] outSprite : 作成された Sprite のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNSprite_CreateWithTexture(LNHandle texture, LNHandle* outSprite);

/**
    @brief init
    @param[] texture : 
    @param[] width : 
    @param[] height : 
    @param[out] outSprite : 作成された Sprite のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNSprite_CreateWithTextureAndSize(LNHandle texture, float width, float height, LNHandle* outSprite);

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] sprite : Sprite のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNSprite_SetTexture(LNHandle sprite, LNHandle value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : Sprite のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNSprite_SetSize(LNHandle sprite, const LNSize* value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : Sprite のハンドル
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNSprite_SetSizeWH(LNHandle sprite, float width, float height);

/**
    @brief 
    @param[in] sprite : Sprite のハンドル
    @param[] x : 
    @param[] y : 
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNSprite_SetSourceRectXYWH(LNHandle sprite, float x, float y, float width, float height);

/**
    @brief test
    @param[in] sprite : Sprite のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSprite_SetCallerTest(LNHandle sprite, LNHandle callback);

typedef LNResult(*LNSprite_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSprite_OnSerialize_SetOverrideCallback(LNSprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNSprite_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNSprite_OnPreUpdate_SetOverrideCallback(LNSprite_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSprite_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNSprite_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNSprite_OnUpdate_SetOverrideCallback(LNSprite_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSprite_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] sprite : Sprite のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSprite_SetPrototype_OnSerialize(LNHandle sprite, LNHandle callback);

/**
    @brief 
    @param[in] sprite : Sprite のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSprite_SetPrototype_OnPreUpdate(LNHandle sprite, LNHandle callback);

/**
    @brief 
    @param[in] sprite : Sprite のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNSprite_SetPrototype_OnUpdate(LNHandle sprite, LNHandle callback);

extern LN_FLAT_API int LNSprite_GetTypeInfoId();
LN_FLAT_API void LNSprite_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSprite_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNSprite_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNSprite_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNSprite_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSprite_RegisterSubclassTypeInfo(const LNSprite_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSprite_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::CameraOrbitControlComponent

/**
    @brief CameraOrbitControlComponent を作成します。
    @param[out] outCameraOrbitControlComponent : 作成された CameraOrbitControlComponent のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNCameraOrbitControlComponent_Create(LNHandle* outCameraOrbitControlComponent);

typedef LNResult(*LNCameraOrbitControlComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_SetOverrideCallback(LNCameraOrbitControlComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] cameraorbitcontrolcomponent : CameraOrbitControlComponent のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCameraOrbitControlComponent_SetPrototype_OnSerialize(LNHandle cameraorbitcontrolcomponent, LNHandle callback);

extern LN_FLAT_API int LNCameraOrbitControlComponent_GetTypeInfoId();
LN_FLAT_API void LNCameraOrbitControlComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraOrbitControlComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCameraOrbitControlComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNCameraOrbitControlComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraOrbitControlComponent_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraOrbitControlComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Raycaster

/**
    @brief メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。
    @param[] point : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNRaycaster_FromScreen(const LNPoint* point, LNHandle* outReturn);

/**
    @brief 指定した向きの平面との交差判定を行います。
    @param[in] raycaster : Raycaster のハンドル
    @param[] normalX : 
    @param[] normalY : 
    @param[] normalZ : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNRaycaster_IntersectPlane(LNHandle raycaster, float normalX, float normalY, float normalZ, LNHandle* outReturn);

typedef LNResult(*LNRaycaster_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRaycaster_OnSerialize_SetOverrideCallback(LNRaycaster_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRaycaster_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] raycaster : Raycaster のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNRaycaster_SetPrototype_OnSerialize(LNHandle raycaster, LNHandle callback);

extern LN_FLAT_API int LNRaycaster_GetTypeInfoId();
LN_FLAT_API void LNRaycaster_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycaster_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRaycaster_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRaycaster_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycaster_RegisterSubclassTypeInfo(const LNRaycaster_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycaster_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::RaycastResult

/**
    @brief ワールド座標上の交差点
    @param[in] raycastresult : RaycastResult のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNRaycastResult_GetPoint(LNHandle raycastresult, LNVector3* outReturn);

typedef LNResult(*LNRaycastResult_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRaycastResult_OnSerialize_SetOverrideCallback(LNRaycastResult_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRaycastResult_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] raycastresult : RaycastResult のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNRaycastResult_SetPrototype_OnSerialize(LNHandle raycastresult, LNHandle callback);

extern LN_FLAT_API int LNRaycastResult_GetTypeInfoId();
LN_FLAT_API void LNRaycastResult_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycastResult_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRaycastResult_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRaycastResult_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycastResult_RegisterSubclassTypeInfo(const LNRaycastResult_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycastResult_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::WorldRenderView

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。
    @param[in] worldrenderview : WorldRenderView のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNWorldRenderView_SetGuideGridEnabled(LNHandle worldrenderview, LNBool value);

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。
    @param[in] worldrenderview : WorldRenderView のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNWorldRenderView_GetGuideGridEnabled(LNHandle worldrenderview, LNBool* outReturn);

typedef LNResult(*LNWorldRenderView_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_SetOverrideCallback(LNWorldRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] worldrenderview : WorldRenderView のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNWorldRenderView_SetPrototype_OnSerialize(LNHandle worldrenderview, LNHandle callback);

extern LN_FLAT_API int LNWorldRenderView_GetTypeInfoId();
LN_FLAT_API void LNWorldRenderView_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldRenderView_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorldRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNWorldRenderView_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldRenderView_RegisterSubclassTypeInfo(const LNWorldRenderView_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldRenderView_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ShapeObject

typedef LNResult(*LNShapeObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNShapeObject_OnSerialize_SetOverrideCallback(LNShapeObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNShapeObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNShapeObject_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNShapeObject_OnPreUpdate_SetOverrideCallback(LNShapeObject_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNShapeObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNShapeObject_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNShapeObject_OnUpdate_SetOverrideCallback(LNShapeObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNShapeObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] shapeobject : ShapeObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnSerialize(LNHandle shapeobject, LNHandle callback);

/**
    @brief 
    @param[in] shapeobject : ShapeObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnPreUpdate(LNHandle shapeobject, LNHandle callback);

/**
    @brief 
    @param[in] shapeobject : ShapeObject のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnUpdate(LNHandle shapeobject, LNHandle callback);

extern LN_FLAT_API int LNShapeObject_GetTypeInfoId();
LN_FLAT_API void LNShapeObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShapeObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNShapeObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNShapeObject_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNShapeObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNShapeObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShapeObject_RegisterSubclassTypeInfo(const LNShapeObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShapeObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PlaneMesh

/**
    @brief 
    @param[out] outPlaneMesh : 作成された PlaneMesh のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNPlaneMesh_Create(LNHandle* outPlaneMesh);

typedef LNResult(*LNPlaneMesh_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_SetOverrideCallback(LNPlaneMesh_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNPlaneMesh_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNPlaneMesh_OnPreUpdate_SetOverrideCallback(LNPlaneMesh_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPlaneMesh_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNPlaneMesh_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_SetOverrideCallback(LNPlaneMesh_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] planemesh : PlaneMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnSerialize(LNHandle planemesh, LNHandle callback);

/**
    @brief 
    @param[in] planemesh : PlaneMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnPreUpdate(LNHandle planemesh, LNHandle callback);

/**
    @brief 
    @param[in] planemesh : PlaneMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnUpdate(LNHandle planemesh, LNHandle callback);

extern LN_FLAT_API int LNPlaneMesh_GetTypeInfoId();
LN_FLAT_API void LNPlaneMesh_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMesh_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNPlaneMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNPlaneMesh_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNPlaneMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNPlaneMesh_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMesh_RegisterSubclassTypeInfo(const LNPlaneMesh_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMesh_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::BoxMesh

/**
    @brief 各軸のサイズが 1 である BoxMesh を作成します。
    @param[out] outBoxMesh : 作成された BoxMesh のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNBoxMesh_Create(LNHandle* outBoxMesh);

/**
    @brief 
    @param[] width : 
    @param[] height : 
    @param[] depth : 
    @param[out] outBoxMesh : 作成された BoxMesh のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNBoxMesh_CreateWithSize(float width, float height, float depth, LNHandle* outBoxMesh);

typedef LNResult(*LNBoxMesh_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNBoxMesh_OnSerialize_SetOverrideCallback(LNBoxMesh_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNBoxMesh_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNBoxMesh_OnPreUpdate_SetOverrideCallback(LNBoxMesh_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxMesh_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNBoxMesh_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNBoxMesh_OnUpdate_SetOverrideCallback(LNBoxMesh_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] boxmesh : BoxMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnSerialize(LNHandle boxmesh, LNHandle callback);

/**
    @brief 
    @param[in] boxmesh : BoxMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnPreUpdate(LNHandle boxmesh, LNHandle callback);

/**
    @brief 
    @param[in] boxmesh : BoxMesh のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnUpdate(LNHandle boxmesh, LNHandle callback);

extern LN_FLAT_API int LNBoxMesh_GetTypeInfoId();
LN_FLAT_API void LNBoxMesh_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMesh_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNBoxMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNBoxMesh_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNBoxMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNBoxMesh_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMesh_RegisterSubclassTypeInfo(const LNBoxMesh_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMesh_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::MeshComponent

/**
    @brief init
    @param[out] outMeshComponent : 作成された MeshComponent のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNMeshComponent_Create(LNHandle* outMeshComponent);

/**
    @brief setModel
    @param[in] meshcomponent : MeshComponent のハンドル
    @param[] model : 
*/
LN_FLAT_API LNResult LNMeshComponent_SetModel(LNHandle meshcomponent, LNHandle model);

/**
    @brief 指定した名前の MeshContainer から、衝突判定用の Body を作成します。
    @param[in] meshcomponent : MeshComponent のハンドル
    @param[] meshContainerName : 
*/
LN_FLAT_API LNResult LNMeshComponent_MakeCollisionBody(LNHandle meshcomponent, const LNChar* meshContainerName);
LN_FLAT_API LNResult LNMeshComponent_MakeCollisionBodyA(LNHandle meshcomponent, const char* meshContainerName);

typedef LNResult(*LNMeshComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNMeshComponent_OnSerialize_SetOverrideCallback(LNMeshComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNMeshComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] meshcomponent : MeshComponent のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNMeshComponent_SetPrototype_OnSerialize(LNHandle meshcomponent, LNHandle callback);

extern LN_FLAT_API int LNMeshComponent_GetTypeInfoId();
LN_FLAT_API void LNMeshComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNMeshComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNMeshComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshComponent_RegisterSubclassTypeInfo(const LNMeshComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Collision

/**
    @brief 自分自身と衝突している他の WorldObject
    @param[in] collision : Collision のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNCollision_GetWorldObject(LNHandle collision, LNHandle* outReturn);

typedef LNResult(*LNCollision_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCollision_OnSerialize_SetOverrideCallback(LNCollision_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCollision_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] collision : Collision のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNCollision_SetPrototype_OnSerialize(LNHandle collision, LNHandle callback);

extern LN_FLAT_API int LNCollision_GetTypeInfoId();
LN_FLAT_API void LNCollision_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCollision_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCollision_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNCollision_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCollision_RegisterSubclassTypeInfo(const LNCollision_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCollision_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::TriggerBodyComponent

/**
    @brief init
    @param[out] outTriggerBodyComponent : 作成された TriggerBodyComponent のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNTriggerBodyComponent_Create(LNHandle* outTriggerBodyComponent);

/**
    @brief addCollisionShape
    @param[in] triggerbodycomponent : TriggerBodyComponent のハンドル
    @param[] shape : 
*/
LN_FLAT_API LNResult LNTriggerBodyComponent_AddCollisionShape(LNHandle triggerbodycomponent, LNHandle shape);

typedef LNResult(*LNTriggerBodyComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNTriggerBodyComponent_OnSerialize_SetOverrideCallback(LNTriggerBodyComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNTriggerBodyComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] triggerbodycomponent : TriggerBodyComponent のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNTriggerBodyComponent_SetPrototype_OnSerialize(LNHandle triggerbodycomponent, LNHandle callback);

extern LN_FLAT_API int LNTriggerBodyComponent_GetTypeInfoId();
LN_FLAT_API void LNTriggerBodyComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTriggerBodyComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNTriggerBodyComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNTriggerBodyComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTriggerBodyComponent_RegisterSubclassTypeInfo(const LNTriggerBodyComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTriggerBodyComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ParticleEmitter

/**
    @brief init
    @param[] model : 
    @param[out] outParticleEmitter : 作成された ParticleEmitter のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNParticleEmitter_Create(LNHandle model, LNHandle* outParticleEmitter);

typedef LNResult(*LNParticleEmitter_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNParticleEmitter_OnSerialize_SetOverrideCallback(LNParticleEmitter_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNParticleEmitter_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNParticleEmitter_OnPreUpdate_OverrideCallback)(LNHandle worldobject);
LN_FLAT_API LNResult LNParticleEmitter_OnPreUpdate_SetOverrideCallback(LNParticleEmitter_OnPreUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNParticleEmitter_OnPreUpdate_CallOverrideBase(LNHandle worldobject);
typedef LNResult(*LNParticleEmitter_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNParticleEmitter_OnUpdate_SetOverrideCallback(LNParticleEmitter_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNParticleEmitter_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] particleemitter : ParticleEmitter のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnSerialize(LNHandle particleemitter, LNHandle callback);

/**
    @brief 
    @param[in] particleemitter : ParticleEmitter のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnPreUpdate(LNHandle particleemitter, LNHandle callback);

/**
    @brief 
    @param[in] particleemitter : ParticleEmitter のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnUpdate(LNHandle particleemitter, LNHandle callback);

extern LN_FLAT_API int LNParticleEmitter_GetTypeInfoId();
LN_FLAT_API void LNParticleEmitter_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitter_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNParticleEmitter_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNParticleEmitter_OnPreUpdate_OverrideCallback OnPreUpdate_OverrideFunc;
    LNParticleEmitter_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNParticleEmitter_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitter_RegisterSubclassTypeInfo(const LNParticleEmitter_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitter_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Scene

/**
    @brief シーン背景のクリア方法を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetClearMode(LNSceneClearMode value);

/**
    @brief ClearMode が SkyDome であるときに使用する、空の基本色を設定します。アルファ値は、設定した色の適用率です。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSkyColor(const LNColor* value);

/**
    @brief ClearMode が SkyDome であるときに使用する、地平の基本色を設定します。アルファ値は、設定した色の適用率です。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSkyHorizonColor(const LNColor* value);

/**
    @brief ClearMode が SkyDome であるときに使用する、雲の基本色を設定します。アルファ値は、設定した色の適用率です。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSkyCloudColor(const LNColor* value);

/**
    @brief ClearMode が SkyDome であるときに使用する、空全体に影響する色を設定します。アルファ値は、設定した色の適用率です。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSkyOverlayColor(const LNColor* value);

/**
    @brief 指定したレベルへ遷移します。既存の全てのレベルは非アクティブ化または削除されます。
    @param[] level : 
    @param[] withEffect : 
*/
LN_FLAT_API LNResult LNScene_GotoLevel(LNHandle level, LNBool withEffect);

/**
    @brief 現在のレベルを非アクティブ化し、指定したレベルへ遷移します。
    @param[] level : 
    @param[] withEffect : 
*/
LN_FLAT_API LNResult LNScene_CallLevel(LNHandle level, LNBool withEffect);

/**
    @brief 現在のレベルを終了し、ひとつ前のレベルへ遷移します。
    @param[] withEffect : 
*/
LN_FLAT_API LNResult LNScene_ReturnLevel(LNBool withEffect);

/**
    @brief 現在のアクティブなレベルを取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_ActiveLevel(LNHandle* outReturn);

/**
    @brief レベルの遷移エフェクトを実行中であるかを確認します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsTransitionEffectRunning(LNBool* outReturn);

/**
    @brief レベル遷移時のエフェクトの種類を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTransitionEffectMode(LNLevelTransitionEffectMode value);

/**
    @brief レベル遷移時のエフェクトの種類を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_TransitionEffectMode(LNLevelTransitionEffectMode* outReturn);

/**
    @brief レベルの遷移にかける時間を設定します。(Unit: 秒)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTransitionDuration(float value);

/**
    @brief レベルの遷移にかける時間を取得します。(Unit: 秒)
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_TransitionDuration(float* outReturn);

/**
    @brief レベルの遷移モードが FadeInOut である場合に使用する色を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTransitionEffectColor(const LNColor* value);

/**
    @brief レベルの遷移モードが FadeInOut である場合に使用する色を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_TransitionEffectColor(LNColor* outReturn);

/**
    @brief レベルの遷移エフェクトで使用するマスクテクスチャを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTransitionEffectMaskTexture(LNHandle value);

/**
    @brief レベルの遷移エフェクトで使用するマスクテクスチャを取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_TransitionEffectMaskTexture(LNHandle* outReturn);

/**
    @brief レベルの遷移エフェクトの境界のあいまいさを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTransitionEffectVague(float value);

/**
    @brief レベルの遷移エフェクトの境界のあいまいさを取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_TransitionEffectVague(float* outReturn);

/**
    @brief フェードアウトエフェクトを開始します。
*/
LN_FLAT_API LNResult LNScene_StartFadeOut();

/**
    @brief フェードインエフェクトを開始します。
*/
LN_FLAT_API LNResult LNScene_StartFadeIn();

/**
    @brief フォグを開始するカメラからの距離を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogStartDistance(float value);

/**
    @brief フォグのメインカラーを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogColor(const LNColor* value);

/**
    @brief フォグの濃さを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogDensity(float value);

/**
    @brief 高さフォグの濃さを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogHeightDensity(float value);

/**
    @brief フォグの高さの下限を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogLowerHeight(float value);

/**
    @brief フォグの高さの上限を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetFogUpperHeight(float value);

/**
    @brief HDR レンダリングの有無を設定します。 (default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetHDREnabled(LNBool value);

/**
    @brief HDR レンダリングの有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsHDREnabled(LNBool* outReturn);

/**
    @brief 画面全体へのブレンドカラーを設定します。(default: Black)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetScreenBlendColor(const LNColor* value);

/**
    @brief 画面全体へのブレンドカラーを取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_ScreenBlendColor(LNColor* outReturn);

/**
    @brief setColorTone
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetColorTone(const LNColorTone* value);

/**
    @brief colorTone
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_ColorTone(LNColorTone* outReturn);

/**
    @brief アンチエイリアスの有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetAntialiasEnabled(LNBool value);

/**
    @brief アンチエイリアスの有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsAntialiasEnabled(LNBool* outReturn);

/**
    @brief SSR (Screen Space Reflection) の有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSSREnabled(LNBool value);

/**
    @brief SSR の有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsSSREnabled(LNBool* outReturn);

/**
    @brief SSAO (Screen Space Ambient Occlusion) の有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetSSAOEnabled(LNBool value);

/**
    @brief SSAO の有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsSSAOEnabled(LNBool* outReturn);

/**
    @brief ブルームエフェクトの有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetBloomEnabled(LNBool value);

/**
    @brief ブルームエフェクトの有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsBloomEnabled(LNBool* outReturn);

/**
    @brief 被写界深度の有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetDOFEnabled(LNBool value);

/**
    @brief 被写界深度の有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsDOFEnabled(LNBool* outReturn);

/**
    @brief トーンマッピングの有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapEnabled(LNBool value);

/**
    @brief トーンマッピングの有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsTonemapEnabled(LNBool* outReturn);

/**
    @brief ビネットエフェクトの有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetVignetteEnabled(LNBool value);

/**
    @brief ビネットエフェクトの有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsVignetteEnabled(LNBool* outReturn);

/**
    @brief ガンマ補正の有無を設定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetGammaEnabled(LNBool value);

/**
    @brief ガンマ補正の有無を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNScene_IsGammaEnabled(LNBool* outReturn);

/**
    @brief Tonemap exposure
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapExposure(float value);

/**
    @brief setTonemapLinearWhite
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapLinearWhite(float value);

/**
    @brief setTonemapShoulderStrength
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapShoulderStrength(float value);

/**
    @brief setTonemapLinearStrength
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapLinearStrength(float value);

/**
    @brief setTonemapLinearAngle
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapLinearAngle(float value);

/**
    @brief setTonemapToeStrength
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapToeStrength(float value);

/**
    @brief setTonemapToeNumerator
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapToeNumerator(float value);

/**
    @brief setTonemapToeDenominator
    @param[] value : 
*/
LN_FLAT_API LNResult LNScene_SetTonemapToeDenominator(float value);


//==============================================================================
// ln::Level

/**
    @brief Initialize
    @param[out] outLevel : 作成された Level のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNLevel_Create(LNHandle* outLevel);

/**
    @brief 指定した WorldObject を、この Level 子オブジェクトとして追加します。
    @param[in] level : Level のハンドル
    @param[] obj : 
*/
LN_FLAT_API LNResult LNLevel_AddObject(LNHandle level, LNHandle obj);

/**
    @brief 指定した WorldObject を、この Level 子オブジェクトか除外します。
    @param[in] level : Level のハンドル
    @param[] obj : 
*/
LN_FLAT_API LNResult LNLevel_RemoveObject(LNHandle level, LNHandle obj);

/**
    @brief 指定した Level を、この Level の Sub-Level として追加します。
    @param[in] level : Level のハンドル
    @param[] sublevel : 
*/
LN_FLAT_API LNResult LNLevel_AddSubLevel(LNHandle level, LNHandle sublevel);

/**
    @brief 指定した Level を、この Level の Sub-Level から除外します。
    @param[in] level : Level のハンドル
    @param[] sublevel : 
*/
LN_FLAT_API LNResult LNLevel_RemoveSubLevel(LNHandle level, LNHandle sublevel);

/**
    @brief すべての Sub-Level を除外します。
    @param[in] level : Level のハンドル
*/
LN_FLAT_API LNResult LNLevel_RemoveAllSubLevels(LNHandle level);

typedef LNResult(*LNLevel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNLevel_OnSerialize_SetOverrideCallback(LNLevel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNLevel_OnStart_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnStart_SetOverrideCallback(LNLevel_OnStart_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnStart_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnStop_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnStop_SetOverrideCallback(LNLevel_OnStop_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnStop_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnPause_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnPause_SetOverrideCallback(LNLevel_OnPause_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnPause_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnResume_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnResume_SetOverrideCallback(LNLevel_OnResume_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnResume_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnUpdate_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnUpdate_SetOverrideCallback(LNLevel_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnUpdate_CallOverrideBase(LNHandle level);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnSerialize(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStart(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStop(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnPause(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnResume(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : Level のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnUpdate(LNHandle level, LNHandle callback);

extern LN_FLAT_API int LNLevel_GetTypeInfoId();
LN_FLAT_API void LNLevel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNLevel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNLevel_OnStart_OverrideCallback OnStart_OverrideFunc;
    LNLevel_OnStop_OverrideCallback OnStop_OverrideFunc;
    LNLevel_OnPause_OverrideCallback OnPause_OverrideFunc;
    LNLevel_OnResume_OverrideCallback OnResume_OverrideFunc;
    LNLevel_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNLevel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevel_RegisterSubclassTypeInfo(const LNLevel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIColors

/**
    @brief 色の濃さ (0~9) を指定して、 Red カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Red(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Pink カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Pink(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Purple カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Purple(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 DeepPurple カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_DeepPurple(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Indigo カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Indigo(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Blue カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Blue(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 LightBlue カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_LightBlue(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Cyan カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Cyan(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Teal カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Teal(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Green カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Green(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 LightGreen カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_LightGreen(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Lime カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Lime(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Yellow カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Yellow(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Amber カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Amber(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Orange カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Orange(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 DeepOrange カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_DeepOrange(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Brown カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Brown(int shades, LNColor* outReturn);

/**
    @brief 色の濃さ (0~9) を指定して、 Grey カラーパレットの色を取得します。
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Grey(int shades, LNColor* outReturn);

/**
    @brief カラーインデックスと色の濃さ (0~9) を指定して、色を取得します。
    @param[] hue : 
    @param[] shades : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIColors_Get(LNUIColorHues hue, int shades, LNColor* outReturn);


//==============================================================================
// ln::UIEventArgs

/**
    @brief イベントの発生元となった要素を取得します。
    @param[in] uieventargs : UIEventArgs のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIEventArgs_Sender(LNHandle uieventargs, LNHandle* outReturn);

typedef LNResult(*LNUIEventArgs_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_SetOverrideCallback(LNUIEventArgs_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uieventargs : UIEventArgs のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIEventArgs_SetPrototype_OnSerialize(LNHandle uieventargs, LNHandle callback);

extern LN_FLAT_API int LNUIEventArgs_GetTypeInfoId();
LN_FLAT_API void LNUIEventArgs_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventArgs_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIEventArgs_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIEventArgs_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventArgs_RegisterSubclassTypeInfo(const LNUIEventArgs_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventArgs_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIGeneralEventHandler

LN_FLAT_API LNResult LNUIGeneralEventHandler_Create(LNUIGeneralEventHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIGeneralEventHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIGeneralEventHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIGeneralEventHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIGeneralEventHandler_RegisterSubclassTypeInfo(const LNUIGeneralEventHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIGeneralEventHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIEventHandler

LN_FLAT_API LNResult LNUIEventHandler_Create(LNUIEventHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIEventHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIEventHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventHandler_RegisterSubclassTypeInfo(const LNUIEventHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UILayoutElement

typedef LNResult(*LNUILayoutElement_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_SetOverrideCallback(LNUILayoutElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilayoutelement : UILayoutElement のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUILayoutElement_SetPrototype_OnSerialize(LNHandle uilayoutelement, LNHandle callback);

extern LN_FLAT_API int LNUILayoutElement_GetTypeInfoId();
LN_FLAT_API void LNUILayoutElement_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutElement_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUILayoutElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUILayoutElement_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutElement_RegisterSubclassTypeInfo(const LNUILayoutElement_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutElement_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIElement

/**
    @brief 要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
    @param[in] uielement : UIElement のハンドル
    @param[] size : 
*/
LN_FLAT_API LNResult LNUIElement_SetSize(LNHandle uielement, const LNSize* size);

/**
    @brief 要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
    @param[in] uielement : UIElement のハンドル
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNUIElement_SetSizeWH(LNHandle uielement, float width, float height);

/**
    @brief setWidth
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetWidth(LNHandle uielement, float value);

/**
    @brief width
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetWidth(LNHandle uielement, float* outReturn);

/**
    @brief setHeight
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetHeight(LNHandle uielement, float value);

/**
    @brief height
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetHeight(LNHandle uielement, float* outReturn);

/**
    @brief 要素の margin 値 (外側の余白) を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] margin : 
*/
LN_FLAT_API LNResult LNUIElement_SetMargin(LNHandle uielement, const LNThickness* margin);

/**
    @brief 要素の margin 値 (外側の余白) を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetMargin(LNHandle uielement, LNThickness* outReturn);

/**
    @brief 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : UIElement のハンドル
    @param[] padding : 
*/
LN_FLAT_API LNResult LNUIElement_SetPadding(LNHandle uielement, const LNThickness* padding);

/**
    @brief 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetPadding(LNHandle uielement, LNThickness* outReturn);

/**
    @brief 要素の横方向の配置方法を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetHAlignment(LNHandle uielement, LNUIHAlignment value);

/**
    @brief 要素の横方向の配置方法を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetHAlignment(LNHandle uielement, LNUIHAlignment* outReturn);

/**
    @brief 要素の縦方向の配置方法を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetVAlignment(LNHandle uielement, LNUIVAlignment value);

/**
    @brief 要素の縦方向の配置方法を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetVAlignment(LNHandle uielement, LNUIVAlignment* outReturn);

/**
    @brief 要素の配置方法を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] halign : 
    @param[] valign : 
*/
LN_FLAT_API LNResult LNUIElement_SetAlignments(LNHandle uielement, LNUIHAlignment halign, LNUIVAlignment valign);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] pos : 
*/
LN_FLAT_API LNResult LNUIElement_SetPosition(LNHandle uielement, const LNVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNUIElement_SetPositionXYZ(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetPosition(LNHandle uielement, LNVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] rot : 
*/
LN_FLAT_API LNResult LNUIElement_SetRotation(LNHandle uielement, const LNQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] uielement : UIElement のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNUIElement_SetEulerAngles(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetRotation(LNHandle uielement, LNQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] scale : 
*/
LN_FLAT_API LNResult LNUIElement_SetScale(LNHandle uielement, const LNVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] xyz : 
*/
LN_FLAT_API LNResult LNUIElement_SetScaleS(LNHandle uielement, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] x : 
    @param[] y : 
*/
LN_FLAT_API LNResult LNUIElement_SetScaleXY(LNHandle uielement, float x, float y);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetScale(LNHandle uielement, LNVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetCenterPoint(LNHandle uielement, const LNVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] x : 
    @param[] y : 
    @param[] z : 
*/
LN_FLAT_API LNResult LNUIElement_SetCenterPointXYZ(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetCenterPoint(LNHandle uielement, LNVector3* outReturn);

/**
    @brief 要素の有効状態を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetEnabled(LNHandle uielement, LNBool value);

/**
    @brief 要素の有効状態を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_IsEnabled(LNHandle uielement, LNBool* outReturn);

/**
    @brief 任意のユーザーデータを設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetData(LNHandle uielement, LNHandle value);

/**
    @brief 任意のユーザーデータを取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetData(LNHandle uielement, LNHandle* outReturn);

/**
    @brief 背景の色を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetBackgroundColor(LNHandle uielement, const LNColor* value);

/**
    @brief 背景の色を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetBackgroundColor(LNHandle uielement, LNColor* outReturn);

/**
    @brief 枠線の太さを設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetBorderThickness(LNHandle uielement, const LNThickness* value);

/**
    @brief 枠線の太さを取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetBorderThickness(LNHandle uielement, LNThickness* outReturn);

/**
    @brief 枠線の色を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetBorderColor(LNHandle uielement, const LNColor* value);

/**
    @brief 枠線の色を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetBorderColor(LNHandle uielement, LNColor* outReturn);

/**
    @brief 四角形の角の半径を設定します。
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetCornerRadius(LNHandle uielement, const LNCornerRadius* value);

/**
    @brief 四角形の角の半径を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetCornerRadius(LNHandle uielement, LNCornerRadius* outReturn);

/**
    @brief 可視状態を設定します。(default: UIVisibility::Visible)
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetVisibility(LNHandle uielement, LNUIVisibility value);

/**
    @brief 可視状態を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetVisibility(LNHandle uielement, LNUIVisibility* outReturn);

/**
    @brief 不透明度を設定します。(default: 1.0)
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetOpacity(LNHandle uielement, float value);

/**
    @brief 不透明度を取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetOpacity(LNHandle uielement, float* outReturn);

/**
    @brief Add element to container. 論理的な子要素として追加する。
    @param[in] uielement : UIElement のハンドル
    @param[] child : 
*/
LN_FLAT_API LNResult LNUIElement_AddChild(LNHandle uielement, LNHandle child);

/**
    @brief 入力フォーカスを得ることができるかどうかを設定します。(default: false)
    @param[in] uielement : UIElement のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIElement_SetFocusable(LNHandle uielement, LNBool value);

/**
    @brief 入力フォーカスを得ることができるかどうかを取得します。
    @param[in] uielement : UIElement のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIElement_GetFocusable(LNHandle uielement, LNBool* outReturn);

/**
    @brief この UIElement を指定した UIElement へ子要素として追加します。省略した場合はデフォルトのルート UIElement へ追加します。
    @param[in] uielement : UIElement のハンドル
    @param[] parent : 
*/
LN_FLAT_API LNResult LNUIElement_AddInto(LNHandle uielement, LNHandle parent);

typedef LNResult(*LNUIElement_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIElement_OnSerialize_SetOverrideCallback(LNUIElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uielement : UIElement のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIElement_SetPrototype_OnSerialize(LNHandle uielement, LNHandle callback);

extern LN_FLAT_API int LNUIElement_GetTypeInfoId();
LN_FLAT_API void LNUIElement_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIElement_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIElement_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIElement_RegisterSubclassTypeInfo(const LNUIElement_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIElement_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIText

/**
    @brief UIText を作成します。
    @param[out] outUIText : 作成された UIText のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIText_Create(LNHandle* outUIText);

/**
    @brief 表示文字列を指定して、UITextBlock を作成します。
    @param[] text : 
    @param[out] outUIText : 作成された UIText のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIText_CreateWithText(const LNChar* text, LNHandle* outUIText);
LN_FLAT_API LNResult LNUIText_CreateWithTextA(const char* text, LNHandle* outUIText);

/**
    @brief 表示文字列を設定します。
    @param[in] uitext : UIText のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIText_SetText(LNHandle uitext, const LNChar* value);
LN_FLAT_API LNResult LNUIText_SetTextA(LNHandle uitext, const char* value);

/**
    @brief 表示文字列を取得します。
    @param[in] uitext : UIText のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIText_GetText(LNHandle uitext, const LNChar** outReturn);
LN_FLAT_API LNResult LNUIText_GetTextA(LNHandle uitext, const char** outReturn);

typedef LNResult(*LNUIText_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIText_OnSerialize_SetOverrideCallback(LNUIText_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIText_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uitext : UIText のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIText_SetPrototype_OnSerialize(LNHandle uitext, LNHandle callback);

extern LN_FLAT_API int LNUIText_GetTypeInfoId();
LN_FLAT_API void LNUIText_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIText_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIText_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIText_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIText_RegisterSubclassTypeInfo(const LNUIText_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIText_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UISprite

/**
    @brief UISprite を作成します。
    @param[out] outUISprite : 作成された UISprite のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUISprite_Create(LNHandle* outUISprite);

/**
    @brief テクスチャを指定して UISprite を作成します。
    @param[] texture : 
    @param[out] outUISprite : 作成された UISprite のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUISprite_CreateWithTexture(LNHandle texture, LNHandle* outUISprite);

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] uisprite : UISprite のハンドル
    @param[] texture : 
*/
LN_FLAT_API LNResult LNUISprite_SetTexture(LNHandle uisprite, LNHandle texture);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
    @param[in] uisprite : UISprite のハンドル
    @param[] rect : 
*/
LN_FLAT_API LNResult LNUISprite_SetSourceRect(LNHandle uisprite, const LNRect* rect);

/**
    @brief 
    @param[in] uisprite : UISprite のハンドル
    @param[] x : 
    @param[] y : 
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNUISprite_SetSourceRectXYWH(LNHandle uisprite, float x, float y, float width, float height);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
    @param[in] uisprite : UISprite のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUISprite_GetSourceRect(LNHandle uisprite, LNRect* outReturn);

/**
    @brief setShader
    @param[in] uisprite : UISprite のハンドル
    @param[] shader : 
*/
LN_FLAT_API LNResult LNUISprite_SetShader(LNHandle uisprite, LNHandle shader);

typedef LNResult(*LNUISprite_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUISprite_OnSerialize_SetOverrideCallback(LNUISprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUISprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uisprite : UISprite のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUISprite_SetPrototype_OnSerialize(LNHandle uisprite, LNHandle callback);

extern LN_FLAT_API int LNUISprite_GetTypeInfoId();
LN_FLAT_API void LNUISprite_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUISprite_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUISprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUISprite_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUISprite_RegisterSubclassTypeInfo(const LNUISprite_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUISprite_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIIcon

/**
    @brief アイコン名を指定して UIIcon を作成します。
    @param[] iconName : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIIcon_LoadFontIcon(const LNChar* iconName, LNHandle* outReturn);
LN_FLAT_API LNResult LNUIIcon_LoadFontIconA(const char* iconName, LNHandle* outReturn);

/**
    @brief アイコン名・サイズ を指定して UIIcon を作成します。
    @param[] iconName : 
    @param[] size : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSize(const LNChar* iconName, int size, LNHandle* outReturn);
LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeA(const char* iconName, int size, LNHandle* outReturn);

/**
    @brief アイコン名・サイズ・色 (TextColor) を指定して UIIcon を作成します。
    @param[] iconName : 
    @param[] size : 
    @param[] color : 
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeColor(const LNChar* iconName, int size, const LNColor* color, LNHandle* outReturn);
LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeColorA(const char* iconName, int size, const LNColor* color, LNHandle* outReturn);

typedef LNResult(*LNUIIcon_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIIcon_OnSerialize_SetOverrideCallback(LNUIIcon_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIIcon_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uiicon : UIIcon のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIIcon_SetPrototype_OnSerialize(LNHandle uiicon, LNHandle callback);

extern LN_FLAT_API int LNUIIcon_GetTypeInfoId();
LN_FLAT_API void LNUIIcon_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIIcon_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIIcon_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIIcon_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIIcon_RegisterSubclassTypeInfo(const LNUIIcon_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIIcon_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIMessageTextArea

/**
    @brief init
    @param[out] outUIMessageTextArea : 作成された UIMessageTextArea のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIMessageTextArea_Create(LNHandle* outUIMessageTextArea);

/**
    @brief setText
    @param[in] uimessagetextarea : UIMessageTextArea のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIMessageTextArea_SetText(LNHandle uimessagetextarea, const LNChar* value);
LN_FLAT_API LNResult LNUIMessageTextArea_SetTextA(LNHandle uimessagetextarea, const char* value);

/**
    @brief setTypingSpeed
    @param[in] uimessagetextarea : UIMessageTextArea のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIMessageTextArea_SetTypingSpeed(LNHandle uimessagetextarea, float value);

typedef LNResult(*LNUIMessageTextArea_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIMessageTextArea_OnSerialize_SetOverrideCallback(LNUIMessageTextArea_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIMessageTextArea_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uimessagetextarea : UIMessageTextArea のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIMessageTextArea_SetPrototype_OnSerialize(LNHandle uimessagetextarea, LNHandle callback);

extern LN_FLAT_API int LNUIMessageTextArea_GetTypeInfoId();
LN_FLAT_API void LNUIMessageTextArea_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIMessageTextArea_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIMessageTextArea_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIMessageTextArea_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIMessageTextArea_RegisterSubclassTypeInfo(const LNUIMessageTextArea_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIMessageTextArea_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UI

/**
    @brief add
    @param[] element : 
*/
LN_FLAT_API LNResult LNUI_Add(LNHandle element);

/**
    @brief remove
    @param[] element : 
*/
LN_FLAT_API LNResult LNUI_Remove(LNHandle element);


//==============================================================================
// ln::UILayoutPanel

typedef LNResult(*LNUILayoutPanel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUILayoutPanel_OnSerialize_SetOverrideCallback(LNUILayoutPanel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUILayoutPanel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilayoutpanel : UILayoutPanel のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUILayoutPanel_SetPrototype_OnSerialize(LNHandle uilayoutpanel, LNHandle callback);

extern LN_FLAT_API int LNUILayoutPanel_GetTypeInfoId();
LN_FLAT_API void LNUILayoutPanel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutPanel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUILayoutPanel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUILayoutPanel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutPanel_RegisterSubclassTypeInfo(const LNUILayoutPanel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutPanel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIBoxLayout

/**
    @brief init
    @param[out] outUIBoxLayout : 作成された UIBoxLayout のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIBoxLayout_Create(LNHandle* outUIBoxLayout);

/**
    @brief レイアウト方向を設定します。(default: Vertical)
    @param[in] uiboxlayout : UIBoxLayout のハンドル
    @param[] orientation : 
*/
LN_FLAT_API LNResult LNUIBoxLayout_SetOrientation(LNHandle uiboxlayout, LNUILayoutOrientation orientation);

/**
    @brief レイアウト方向を取得します。
    @param[in] uiboxlayout : UIBoxLayout のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIBoxLayout_GetOrientation(LNHandle uiboxlayout, LNUILayoutOrientation* outReturn);

typedef LNResult(*LNUIBoxLayout_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIBoxLayout_OnSerialize_SetOverrideCallback(LNUIBoxLayout_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIBoxLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uiboxlayout : UIBoxLayout のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIBoxLayout_SetPrototype_OnSerialize(LNHandle uiboxlayout, LNHandle callback);

extern LN_FLAT_API int LNUIBoxLayout_GetTypeInfoId();
LN_FLAT_API void LNUIBoxLayout_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIBoxLayout_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIBoxLayout_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIBoxLayout_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIBoxLayout_RegisterSubclassTypeInfo(const LNUIBoxLayout_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIBoxLayout_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIStackLayout

/**
    @brief init
    @param[out] outUIStackLayout : 作成された UIStackLayout のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIStackLayout_Create(LNHandle* outUIStackLayout);

/**
    @brief (default: Vertical)
    @param[in] uistacklayout : UIStackLayout のハンドル
    @param[] orientation : 
*/
LN_FLAT_API LNResult LNUIStackLayout_SetOrientation(LNHandle uistacklayout, LNUILayoutOrientation orientation);

/**
    @brief 
    @param[in] uistacklayout : UIStackLayout のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIStackLayout_GetOrientation(LNHandle uistacklayout, LNUILayoutOrientation* outReturn);

typedef LNResult(*LNUIStackLayout_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIStackLayout_OnSerialize_SetOverrideCallback(LNUIStackLayout_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIStackLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uistacklayout : UIStackLayout のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIStackLayout_SetPrototype_OnSerialize(LNHandle uistacklayout, LNHandle callback);

extern LN_FLAT_API int LNUIStackLayout_GetTypeInfoId();
LN_FLAT_API void LNUIStackLayout_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIStackLayout_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIStackLayout_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIStackLayout_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIStackLayout_RegisterSubclassTypeInfo(const LNUIStackLayout_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIStackLayout_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIGridLayout

/**
    @brief init
    @param[out] outUIGridLayout : 作成された UIGridLayout のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIGridLayout_Create(LNHandle* outUIGridLayout);

/**
    @brief setColumnCount
    @param[in] uigridlayout : UIGridLayout のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIGridLayout_SetColumnCount(LNHandle uigridlayout, int value);

/**
    @brief setRow
    @param[] element : 
    @param[] row : 
*/
LN_FLAT_API LNResult LNUIGridLayout_SetRow(LNHandle element, int row);

/**
    @brief setColumn
    @param[] element : 
    @param[] column : 
*/
LN_FLAT_API LNResult LNUIGridLayout_SetColumn(LNHandle element, int column);

/**
    @brief setPlacement
    @param[] element : 
    @param[] row : 
    @param[] column : 
    @param[] rowSpan : 
    @param[] columnSpan : 
*/
LN_FLAT_API LNResult LNUIGridLayout_SetPlacement(LNHandle element, int row, int column, int rowSpan, int columnSpan);

typedef LNResult(*LNUIGridLayout_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIGridLayout_OnSerialize_SetOverrideCallback(LNUIGridLayout_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIGridLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uigridlayout : UIGridLayout のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIGridLayout_SetPrototype_OnSerialize(LNHandle uigridlayout, LNHandle callback);

extern LN_FLAT_API int LNUIGridLayout_GetTypeInfoId();
LN_FLAT_API void LNUIGridLayout_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIGridLayout_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIGridLayout_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIGridLayout_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIGridLayout_RegisterSubclassTypeInfo(const LNUIGridLayout_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIGridLayout_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIControl

/**
    @brief init
    @param[out] outUIControl : 作成された UIControl のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIControl_Create(LNHandle* outUIControl);

/**
    @brief addInlineVisual
    @param[in] uicontrol : UIControl のハンドル
    @param[] element : 
    @param[] layout : 
*/
LN_FLAT_API LNResult LNUIControl_AddInlineVisual(LNHandle uicontrol, LNHandle element, LNUIInlinePlacement layout);

typedef LNResult(*LNUIControl_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIControl_OnSerialize_SetOverrideCallback(LNUIControl_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIControl_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uicontrol : UIControl のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIControl_SetPrototype_OnSerialize(LNHandle uicontrol, LNHandle callback);

extern LN_FLAT_API int LNUIControl_GetTypeInfoId();
LN_FLAT_API void LNUIControl_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIControl_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIControl_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIControl_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIControl_RegisterSubclassTypeInfo(const LNUIControl_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIControl_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIButtonBase

/**
    @brief set text.
    @param[in] uibuttonbase : UIButtonBase のハンドル
    @param[] text : 
*/
LN_FLAT_API LNResult LNUIButtonBase_SetText(LNHandle uibuttonbase, const LNChar* text);
LN_FLAT_API LNResult LNUIButtonBase_SetTextA(LNHandle uibuttonbase, const char* text);

typedef LNResult(*LNUIButtonBase_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIButtonBase_OnSerialize_SetOverrideCallback(LNUIButtonBase_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIButtonBase_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uibuttonbase : UIButtonBase のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIButtonBase_SetPrototype_OnSerialize(LNHandle uibuttonbase, LNHandle callback);

extern LN_FLAT_API int LNUIButtonBase_GetTypeInfoId();
LN_FLAT_API void LNUIButtonBase_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIButtonBase_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIButtonBase_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIButtonBase_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIButtonBase_RegisterSubclassTypeInfo(const LNUIButtonBase_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIButtonBase_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIButton

/**
    @brief init.
    @param[out] outUIButton : 作成された UIButton のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIButton_Create(LNHandle* outUIButton);

/**
    @brief 表示文字列を指定して UIButton を作成します。
    @param[] text : 
    @param[out] outUIButton : 作成された UIButton のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIButton_CreateWithText(const LNChar* text, LNHandle* outUIButton);
LN_FLAT_API LNResult LNUIButton_CreateWithTextA(const char* text, LNHandle* outUIButton);

/**
    @brief Clicked イベントの通知を受け取るコールバックを登録します。
    @param[in] uibutton : UIButton のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIButton_ConnectOnClicked(LNHandle uibutton, LNHandle handler, LNHandle* outReturn);

typedef LNResult(*LNUIButton_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIButton_OnSerialize_SetOverrideCallback(LNUIButton_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIButton_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uibutton : UIButton のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIButton_SetPrototype_OnSerialize(LNHandle uibutton, LNHandle callback);

extern LN_FLAT_API int LNUIButton_GetTypeInfoId();
LN_FLAT_API void LNUIButton_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIButton_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIButton_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIButton_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIButton_RegisterSubclassTypeInfo(const LNUIButton_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIButton_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIWindow

/**
    @brief init
    @param[out] outUIWindow : 作成された UIWindow のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIWindow_Create(LNHandle* outUIWindow);

typedef LNResult(*LNUIWindow_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIWindow_OnSerialize_SetOverrideCallback(LNUIWindow_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIWindow_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uiwindow : UIWindow のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIWindow_SetPrototype_OnSerialize(LNHandle uiwindow, LNHandle callback);

extern LN_FLAT_API int LNUIWindow_GetTypeInfoId();
LN_FLAT_API void LNUIWindow_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIWindow_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIWindow_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIWindow_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIWindow_RegisterSubclassTypeInfo(const LNUIWindow_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIWindow_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIListItem

/**
    @brief Submit イベントの通知を受け取るコールバックを登録します。
    @param[in] uilistitem : UIListItem のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIListItem_ConnectOnSubmit(LNHandle uilistitem, LNHandle handler, LNHandle* outReturn);

typedef LNResult(*LNUIListItem_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIListItem_OnSerialize_SetOverrideCallback(LNUIListItem_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIListItem_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilistitem : UIListItem のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIListItem_SetPrototype_OnSerialize(LNHandle uilistitem, LNHandle callback);

extern LN_FLAT_API int LNUIListItem_GetTypeInfoId();
LN_FLAT_API void LNUIListItem_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListItem_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIListItem_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIListItem_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListItem_RegisterSubclassTypeInfo(const LNUIListItem_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListItem_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIListItemsControl

/**
    @brief setItemsLayoutPanel
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[] layout : 
*/
LN_FLAT_API LNResult LNUIListItemsControl_SetItemsLayoutPanel(LNHandle uilistitemscontrol, LNHandle layout);

/**
    @brief itemsLayoutPanel
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIListItemsControl_GetItemsLayoutPanel(LNHandle uilistitemscontrol, LNHandle* outReturn);

/**
    @brief UIListSubmitMode (default: Single)
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[] value : 
*/
LN_FLAT_API LNResult LNUIListItemsControl_SetSubmitMode(LNHandle uilistitemscontrol, LNUIListSubmitMode value);

/**
    @brief UIListSubmitMode
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIListItemsControl_GetSubmitMode(LNHandle uilistitemscontrol, LNUIListSubmitMode* outReturn);

/**
    @brief Submit イベントの通知を受け取るコールバックを登録します。
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @param[out] outReturn : 結果を格納する変数。 (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNUIListItemsControl_ConnectOnSubmit(LNHandle uilistitemscontrol, LNHandle handler, LNHandle* outReturn);

typedef LNResult(*LNUIListItemsControl_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIListItemsControl_OnSerialize_SetOverrideCallback(LNUIListItemsControl_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIListItemsControl_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilistitemscontrol : UIListItemsControl のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIListItemsControl_SetPrototype_OnSerialize(LNHandle uilistitemscontrol, LNHandle callback);

extern LN_FLAT_API int LNUIListItemsControl_GetTypeInfoId();
LN_FLAT_API void LNUIListItemsControl_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListItemsControl_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIListItemsControl_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIListItemsControl_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListItemsControl_RegisterSubclassTypeInfo(const LNUIListItemsControl_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListItemsControl_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIListBoxItem

/**
    @brief init
    @param[] content : 
    @param[out] outUIListBoxItem : 作成された UIListBoxItem のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIListBoxItem_Create(LNHandle content, LNHandle* outUIListBoxItem);

typedef LNResult(*LNUIListBoxItem_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIListBoxItem_OnSerialize_SetOverrideCallback(LNUIListBoxItem_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIListBoxItem_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilistboxitem : UIListBoxItem のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIListBoxItem_SetPrototype_OnSerialize(LNHandle uilistboxitem, LNHandle callback);

extern LN_FLAT_API int LNUIListBoxItem_GetTypeInfoId();
LN_FLAT_API void LNUIListBoxItem_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListBoxItem_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIListBoxItem_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIListBoxItem_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListBoxItem_RegisterSubclassTypeInfo(const LNUIListBoxItem_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListBoxItem_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIListBox

/**
    @brief init
    @param[out] outUIListBox : 作成された UIListBox のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNUIListBox_Create(LNHandle* outUIListBox);

/**
    @brief UIListBoxItem を追加し、そのインスタンスを返します。
    @param[in] uilistbox : UIListBox のハンドル
    @param[] content : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNUIListBox_AddItem(LNHandle uilistbox, LNHandle content, LNHandle* outReturn);

typedef LNResult(*LNUIListBox_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIListBox_OnSerialize_SetOverrideCallback(LNUIListBox_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIListBox_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilistbox : UIListBox のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNUIListBox_SetPrototype_OnSerialize(LNHandle uilistbox, LNHandle callback);

extern LN_FLAT_API int LNUIListBox_GetTypeInfoId();
LN_FLAT_API void LNUIListBox_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListBox_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIListBox_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIListBox_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListBox_RegisterSubclassTypeInfo(const LNUIListBox_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListBox_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::InputGesture

typedef LNResult(*LNInputGesture_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInputGesture_OnSerialize_SetOverrideCallback(LNInputGesture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInputGesture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] inputgesture : InputGesture のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNInputGesture_SetPrototype_OnSerialize(LNHandle inputgesture, LNHandle callback);

extern LN_FLAT_API int LNInputGesture_GetTypeInfoId();
LN_FLAT_API void LNInputGesture_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInputGesture_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInputGesture_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNInputGesture_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInputGesture_RegisterSubclassTypeInfo(const LNInputGesture_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInputGesture_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::KeyGesture

/**
    @brief init
    @param[] key : 
    @param[out] outKeyGesture : 作成された KeyGesture のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNKeyGesture_Create(LNKeys key, LNHandle* outKeyGesture);

typedef LNResult(*LNKeyGesture_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNKeyGesture_OnSerialize_SetOverrideCallback(LNKeyGesture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNKeyGesture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] keygesture : KeyGesture のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNKeyGesture_SetPrototype_OnSerialize(LNHandle keygesture, LNHandle callback);

extern LN_FLAT_API int LNKeyGesture_GetTypeInfoId();
LN_FLAT_API void LNKeyGesture_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNKeyGesture_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNKeyGesture_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNKeyGesture_SubclassRegistrationInfo;

extern LN_FLAT_API void LNKeyGesture_RegisterSubclassTypeInfo(const LNKeyGesture_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNKeyGesture_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Input

/**
    @brief ボタンが現在押されているかを判定します。
    @param[] buttonName : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInput_IsPressed(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_IsPressedA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[] buttonName : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInput_IsTriggered(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_IsTriggeredA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[] buttonName : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInput_IsTriggeredOff(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_IsTriggeredOffA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[] buttonName : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInput_IsRepeated(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_IsRepeatedA(const char* buttonName, LNBool* outReturn);

/**
    @brief 指定した軸のアナログ値を取得します。
    @param[] buttonName : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInput_GetAxisValue(const LNChar* buttonName, float* outReturn);
LN_FLAT_API LNResult LNInput_GetAxisValueA(const char* buttonName, float* outReturn);

/**
    @brief ボタンのアクションマッピングを追加します。
    @param[] buttonName : 
    @param[] gesture : 
*/
LN_FLAT_API LNResult LNInput_AddBinding(const LNChar* buttonName, LNHandle gesture);
LN_FLAT_API LNResult LNInput_AddBindingA(const char* buttonName, LNHandle gesture);

/**
    @brief アクションマッピングを除外します。
    @param[] gesture : 
*/
LN_FLAT_API LNResult LNInput_RemoveBinding(LNHandle gesture);

/**
    @brief 指定したボタンのアクションマッピングをすべて削除します。
    @param[] buttonName : 
*/
LN_FLAT_API LNResult LNInput_ClearBindings(const LNChar* buttonName);
LN_FLAT_API LNResult LNInput_ClearBindingsA(const char* buttonName);

/**
    @brief 全てのアクションマッピングを削除します。
*/
LN_FLAT_API LNResult LNInput_ClearAllBindings();


//==============================================================================
// ln::Mouse

/**
    @brief ボタンが現在押されているかを判定します。
    @param[] button : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMouse_Pressed(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[] button : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMouse_Triggered(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[] button : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMouse_TriggeredOff(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[] button : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMouse_Repeated(LNMouseButtons button, LNBool* outReturn);

/**
    @brief マウスポインタの位置を取得します。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNMouse_GetPosition(LNPoint* outReturn);


//==============================================================================
// ln::InterpreterCommand

/**
    @brief コマンドの実行コードを取得します。
    @param[in] interpretercommand : InterpreterCommand のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreterCommand_Code(LNHandle interpretercommand, const LNChar** outReturn);
LN_FLAT_API LNResult LNInterpreterCommand_CodeA(LNHandle interpretercommand, const char** outReturn);

/**
    @brief コマンドのパラメータ数を取得します。
    @param[in] interpretercommand : InterpreterCommand のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreterCommand_ParamsCount(LNHandle interpretercommand, int* outReturn);

/**
    @brief コマンドのパラメータを取得します。
    @param[in] interpretercommand : InterpreterCommand のハンドル
    @param[] index : 
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreterCommand_Param(LNHandle interpretercommand, int index, const LNChar** outReturn);
LN_FLAT_API LNResult LNInterpreterCommand_ParamA(LNHandle interpretercommand, int index, const char** outReturn);

typedef LNResult(*LNInterpreterCommand_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_SetOverrideCallback(LNInterpreterCommand_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] interpretercommand : InterpreterCommand のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNInterpreterCommand_SetPrototype_OnSerialize(LNHandle interpretercommand, LNHandle callback);

extern LN_FLAT_API int LNInterpreterCommand_GetTypeInfoId();
LN_FLAT_API void LNInterpreterCommand_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommand_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreterCommand_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNInterpreterCommand_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommand_RegisterSubclassTypeInfo(const LNInterpreterCommand_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommand_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::InterpreterCommandList

/**
    @brief InterpreterCommandList を作成します。
    @param[out] outInterpreterCommandList : 作成された InterpreterCommandList のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNInterpreterCommandList_Create(LNHandle* outInterpreterCommandList);

/**
    @brief コマンドを追加します。
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] code : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand(LNHandle interpretercommandlist, const LNChar* code);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommandA(LNHandle interpretercommandlist, const char* code);

/**
    @brief コマンドと 1 つのパラメータを追加します。
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] code : 
    @param[] param0 : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1A(LNHandle interpretercommandlist, const char* code, const char* param0);

/**
    @brief コマンドと 2 つのパラメータを追加します。
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] code : 
    @param[] param0 : 
    @param[] param1 : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1);

/**
    @brief コマンドと 3 つのパラメータを追加します。
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] code : 
    @param[] param0 : 
    @param[] param1 : 
    @param[] param2 : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2);

/**
    @brief コマンドと 4 つのパラメータを追加します。
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] code : 
    @param[] param0 : 
    @param[] param1 : 
    @param[] param2 : 
    @param[] param3 : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2, const LNChar* param3);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2, const char* param3);

typedef LNResult(*LNInterpreterCommandList_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_SetOverrideCallback(LNInterpreterCommandList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] interpretercommandlist : InterpreterCommandList のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNInterpreterCommandList_SetPrototype_OnSerialize(LNHandle interpretercommandlist, LNHandle callback);

extern LN_FLAT_API int LNInterpreterCommandList_GetTypeInfoId();
LN_FLAT_API void LNInterpreterCommandList_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandList_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreterCommandList_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNInterpreterCommandList_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandList_RegisterSubclassTypeInfo(const LNInterpreterCommandList_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandList_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::InterpreterCommandDelegate

LN_FLAT_API LNResult LNInterpreterCommandDelegate_Create(LNInterpreterCommandDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandDelegate_RegisterSubclassTypeInfo(const LNInterpreterCommandDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Interpreter

/**
    @brief Interpreter を作成します。
    @param[out] outInterpreter : 作成された Interpreter のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNInterpreter_Create(LNHandle* outInterpreter);

/**
    @brief コマンドリストをクリアします。実行中のコマンドリストは強制的に破棄されます。
    @param[in] interpreter : Interpreter のハンドル
*/
LN_FLAT_API LNResult LNInterpreter_Clear(LNHandle interpreter);

/**
    @brief コマンドリストを設定し、実行を開始します。
    @param[in] interpreter : Interpreter のハンドル
    @param[] commandList : 
*/
LN_FLAT_API LNResult LNInterpreter_Run(LNHandle interpreter, LNHandle commandList);

/**
    @brief コマンドリストの実行中であるかを確認します。
    @param[in] interpreter : Interpreter のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreter_IsRunning(LNHandle interpreter, LNBool* outReturn);

/**
    @brief 
    @param[in] interpreter : Interpreter のハンドル
*/
LN_FLAT_API LNResult LNInterpreter_Update(LNHandle interpreter);

/**
    @brief 
    @param[in] interpreter : Interpreter のハンドル
*/
LN_FLAT_API LNResult LNInterpreter_Terminate(LNHandle interpreter);

/**
    @brief 
    @param[in] interpreter : Interpreter のハンドル
    @param[] name : 
    @param[] handler :  (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandler(LNHandle interpreter, const LNChar* name, LNHandle handler);
LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandlerA(LNHandle interpreter, const char* name, LNHandle handler);

/**
    @brief setWaitMode
    @param[in] interpreter : Interpreter のハンドル
    @param[] mode : 
*/
LN_FLAT_API LNResult LNInterpreter_SetWaitMode(LNHandle interpreter, const LNChar* mode);
LN_FLAT_API LNResult LNInterpreter_SetWaitModeA(LNHandle interpreter, const char* mode);

/**
    @brief waitMode
    @param[in] interpreter : Interpreter のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreter_GetWaitMode(LNHandle interpreter, const LNChar** outReturn);
LN_FLAT_API LNResult LNInterpreter_GetWaitModeA(LNHandle interpreter, const char** outReturn);

/**
    @brief setWaitCount
    @param[in] interpreter : Interpreter のハンドル
    @param[] count : 
*/
LN_FLAT_API LNResult LNInterpreter_SetWaitCount(LNHandle interpreter, int count);

/**
    @brief waitCount
    @param[in] interpreter : Interpreter のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNInterpreter_GetWaitCount(LNHandle interpreter, int* outReturn);

typedef LNResult(*LNInterpreter_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreter_OnSerialize_SetOverrideCallback(LNInterpreter_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreter_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNInterpreter_OnUpdateWait_OverrideCallback)(LNHandle interpreter, LNBool* outReturn);
LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_SetOverrideCallback(LNInterpreter_OnUpdateWait_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_CallOverrideBase(LNHandle interpreter, LNBool* outReturn);

/**
    @brief 
    @param[in] interpreter : Interpreter のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnSerialize(LNHandle interpreter, LNHandle callback);

/**
    @brief 
    @param[in] interpreter : Interpreter のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnUpdateWait(LNHandle interpreter, LNHandle callback);

extern LN_FLAT_API int LNInterpreter_GetTypeInfoId();
LN_FLAT_API void LNInterpreter_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreter_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreter_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNInterpreter_OnUpdateWait_OverrideCallback OnUpdateWait_OverrideFunc;

} LNInterpreter_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreter_RegisterSubclassTypeInfo(const LNInterpreter_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreter_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EngineSettings

/**
    @brief メインウィンドウのタイトル文字列を設定します。
    @param[] title : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitle(const LNChar* title);
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitleA(const char* title);

/**
    @brief メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowSize(int width, int height);

/**
    @brief メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等)
    @param[] width : 
    @param[] height : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWorldViewSize(int width, int height);

/**
    @brief メインウィンドウのサイズをユーザーが変更できるかどうかを指定します。(default: false)
    @param[] value : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowResizable(LNBool value);

/**
    @brief アセットが保存されているディレクトリを登録します。
    @param[] path : 
*/
LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectory(const LNChar* path);
LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectoryA(const char* path);

/**
    @brief アセットファイルを登録します。
    @param[] fileFullPath : 
    @param[] password : 
*/
LN_FLAT_API LNResult LNEngineSettings_AddAssetArchive(const LNChar* fileFullPath, const LNChar* password);
LN_FLAT_API LNResult LNEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password);

/**
    @brief フレームレートを設定します。(default: 60)
    @param[] value : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetFrameRate(int value);

/**
    @brief デフォルトの UI テーマ名を設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetUITheme(const LNChar* value);
LN_FLAT_API LNResult LNEngineSettings_SetUIThemeA(const char* value);

/**
    @brief デフォルトのフォントファイルを設定します。
    @param[] filePath : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetFontFile(const LNChar* filePath);
LN_FLAT_API LNResult LNEngineSettings_SetFontFileA(const char* filePath);

/**
    @brief (default: Debug ビルドの場合true、それ以外は false)
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetDebugToolEnabled(LNBool enabled);

/**
    @brief デバッグモードの有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetDebugMode(LNBool enabled);

/**
    @brief デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
    @param[] filePath : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePath(const LNChar* filePath);
LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePathA(const char* filePath);

/**
    @brief 優先的に使用する GPU の名前を指定します。
    @param[] filePath : 
    @details 現在は DirectX12 を使用する場合のみ有効で、デフォルトの GPU では動作が不安定な場合に "Microsoft Basic Render Driver" 等を試すことができます。
*/
LN_FLAT_API LNResult LNEngineSettings_SetPriorityGPUName(const LNChar* filePath);
LN_FLAT_API LNResult LNEngineSettings_SetPriorityGPUNameA(const char* filePath);

/**
    @brief setDeveloperToolEnabled
    @param[] enabled : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetDeveloperToolEnabled(LNBool enabled);

/**
    @brief ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNEngineSettings_SetUserMainWindow(intptr_t value);


//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_FLAT_API LNResult LNEngine_Initialize();

/**
    @brief エンジンの終了処理を行います。
*/
LN_FLAT_API LNResult LNEngine_Terminate();

/**
    @brief 
    @param[out] outReturn : 結果を格納する変数。
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_FLAT_API LNResult LNEngine_Update(LNBool* outReturn);

/**
    @brief アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEngine_GetTime(double* outReturn);

/**
    @brief デフォルトで作成されるメインの World です。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEngine_GetWorld(LNHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの Camera です。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEngine_GetMainCamera(LNHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの Light です。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEngine_GetMainLight(LNHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの RenderView です。
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNEngine_GetRenderView(LNHandle* outReturn);


//==============================================================================
// ln::Application

/**
    @brief 
    @param[out] outApplication : 作成された Application のハンドルを格納する変数。
*/
LN_FLAT_API LNResult LNApplication_Create(LNHandle* outApplication);

/**
    @brief エンジンの初期化処理が完了した後に呼び出されます。
    @param[in] application : Application のハンドル
*/
LN_FLAT_API LNResult LNApplication_OnInit(LNHandle application);

/**
    @brief 毎フレーム呼び出されます。
    @param[in] application : Application のハンドル
*/
LN_FLAT_API LNResult LNApplication_OnUpdate(LNHandle application);

/**
    @brief デフォルトで作成されるメインの World を取得します。
    @param[in] application : Application のハンドル
    @param[out] outReturn : 結果を格納する変数。
*/
LN_FLAT_API LNResult LNApplication_World(LNHandle application, LNHandle* outReturn);

/**
    @brief Application の実行を開始します。
    @param[in] application : Application のハンドル
*/
LN_FLAT_API LNResult LNApplication_Run(LNHandle application);

typedef LNResult(*LNApplication_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNApplication_OnSerialize_SetOverrideCallback(LNApplication_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNApplication_OnInit_OverrideCallback)(LNHandle application);
LN_FLAT_API LNResult LNApplication_OnInit_SetOverrideCallback(LNApplication_OnInit_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnInit_CallOverrideBase(LNHandle application);
typedef LNResult(*LNApplication_OnUpdate_OverrideCallback)(LNHandle application);
LN_FLAT_API LNResult LNApplication_OnUpdate_SetOverrideCallback(LNApplication_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnUpdate_CallOverrideBase(LNHandle application);

/**
    @brief 
    @param[in] application : Application のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnSerialize(LNHandle application, LNHandle callback);

/**
    @brief 
    @param[in] application : Application のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnInit(LNHandle application, LNHandle callback);

/**
    @brief 
    @param[in] application : Application のハンドル
    @param[] callback : 
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnUpdate(LNHandle application, LNHandle callback);

extern LN_FLAT_API int LNApplication_GetTypeInfoId();
LN_FLAT_API void LNApplication_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplication_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNApplication_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNApplication_OnInit_OverrideCallback OnInit_OverrideFunc;
    LNApplication_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNApplication_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplication_RegisterSubclassTypeInfo(const LNApplication_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplication_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Debug

/**
    @brief 3D シーン上に、グリッドを表示するかどうかを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNDebug_SetGuideGridEnabled(LNBool value);

/**
    @brief 3D シーン上に、コリジョン及び物理演算の情報を表示するかどうかを設定します。
    @param[] value : 
*/
LN_FLAT_API LNResult LNDebug_SetPhysicsDebugDrawEnabled(LNBool value);

/**
    @brief ウィンドウ上にデバッグ文字列を表示します。
    @param[] str : 
*/
LN_FLAT_API LNResult LNDebug_Print(const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintA(const char* str);

/**
    @brief 表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[] time : 
    @param[] str : 
*/
LN_FLAT_API LNResult LNDebug_PrintWithTime(float time, const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintWithTimeA(float time, const char* str);

/**
    @brief 表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[] time : 
    @param[] color : 
    @param[] str : 
*/
LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColor(float time, const LNColor* color, const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColorA(float time, const LNColor* color, const char* str);


//==============================================================================
// ObjectSerializeHandler

LN_FLAT_API LNResult LNObjectSerializeHandler_Create(LNObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNObjectSerializeHandler_RegisterSubclassTypeInfo(const LNObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EventConnectionSerializeHandler

LN_FLAT_API LNResult LNEventConnectionSerializeHandler_Create(LNEventConnectionSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEventConnectionSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEventConnectionSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEventConnectionSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEventConnectionSerializeHandler_RegisterSubclassTypeInfo(const LNEventConnectionSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEventConnectionSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VariantSerializeHandler

LN_FLAT_API LNResult LNVariantSerializeHandler_Create(LNVariantSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVariantSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVariantSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVariantSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVariantSerializeHandler_RegisterSubclassTypeInfo(const LNVariantSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVariantSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ZVTestClass1SerializeHandler

LN_FLAT_API LNResult LNZVTestClass1SerializeHandler_Create(LNZVTestClass1SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestClass1SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestClass1SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestClass1SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestClass1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestClass1SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestClass1SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ZVTestEventArgs1SerializeHandler

LN_FLAT_API LNResult LNZVTestEventArgs1SerializeHandler_Create(LNZVTestEventArgs1SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventArgs1SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventArgs1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// Serializer2SerializeHandler

LN_FLAT_API LNResult LNSerializer2SerializeHandler_Create(LNSerializer2SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSerializer2SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSerializer2SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSerializer2SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSerializer2SerializeHandler_RegisterSubclassTypeInfo(const LNSerializer2SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSerializer2SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AssetObjectSerializeHandler

LN_FLAT_API LNResult LNAssetObjectSerializeHandler_Create(LNAssetObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAssetObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAssetObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetObjectSerializeHandler_RegisterSubclassTypeInfo(const LNAssetObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AssetImportSettingsSerializeHandler

LN_FLAT_API LNResult LNAssetImportSettingsSerializeHandler_Create(LNAssetImportSettingsSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAssetImportSettingsSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetImportSettingsSerializeHandler_RegisterSubclassTypeInfo(const LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetImportSettingsSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AssetModelSerializeHandler

LN_FLAT_API LNResult LNAssetModelSerializeHandler_Create(LNAssetModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAssetModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAssetModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetModelSerializeHandler_RegisterSubclassTypeInfo(const LNAssetModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SoundSerializeHandler

LN_FLAT_API LNResult LNSoundSerializeHandler_Create(LNSoundSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSoundSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSoundSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSoundSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSoundSerializeHandler_RegisterSubclassTypeInfo(const LNSoundSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSoundSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// TextureSerializeHandler

LN_FLAT_API LNResult LNTextureSerializeHandler_Create(LNTextureSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTextureSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTextureSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTextureSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTextureSerializeHandler_RegisterSubclassTypeInfo(const LNTextureSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTextureSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// Texture2DSerializeHandler

LN_FLAT_API LNResult LNTexture2DSerializeHandler_Create(LNTexture2DSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTexture2DSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DSerializeHandler_RegisterSubclassTypeInfo(const LNTexture2DSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ShaderSerializeHandler

LN_FLAT_API LNResult LNShaderSerializeHandler_Create(LNShaderSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNShaderSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShaderSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNShaderSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShaderSerializeHandler_RegisterSubclassTypeInfo(const LNShaderSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShaderSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RenderViewSerializeHandler

LN_FLAT_API LNResult LNRenderViewSerializeHandler_Create(LNRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRenderViewSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRenderViewSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNRenderViewSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// MaterialSerializeHandler

LN_FLAT_API LNResult LNMaterialSerializeHandler_Create(LNMaterialSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNMaterialSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMaterialSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNMaterialSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMaterialSerializeHandler_RegisterSubclassTypeInfo(const LNMaterialSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMaterialSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// MeshNodeSerializeHandler

LN_FLAT_API LNResult LNMeshNodeSerializeHandler_Create(LNMeshNodeSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNMeshNodeSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshNodeSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNMeshNodeSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshNodeSerializeHandler_RegisterSubclassTypeInfo(const LNMeshNodeSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshNodeSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AnimationControllerSerializeHandler

LN_FLAT_API LNResult LNAnimationControllerSerializeHandler_Create(LNAnimationControllerSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAnimationControllerSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationControllerSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAnimationControllerSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationControllerSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationControllerSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationControllerSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// MeshModelSerializeHandler

LN_FLAT_API LNResult LNMeshModelSerializeHandler_Create(LNMeshModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNMeshModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNMeshModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshModelSerializeHandler_RegisterSubclassTypeInfo(const LNMeshModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// MeshImportSettingsSerializeHandler

LN_FLAT_API LNResult LNMeshImportSettingsSerializeHandler_Create(LNMeshImportSettingsSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNMeshImportSettingsSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshImportSettingsSerializeHandler_RegisterSubclassTypeInfo(const LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshImportSettingsSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SkinnedMeshModelSerializeHandler

LN_FLAT_API LNResult LNSkinnedMeshModelSerializeHandler_Create(LNSkinnedMeshModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSkinnedMeshModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSkinnedMeshModelSerializeHandler_RegisterSubclassTypeInfo(const LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CollisionShapeSerializeHandler

LN_FLAT_API LNResult LNCollisionShapeSerializeHandler_Create(LNCollisionShapeSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCollisionShapeSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCollisionShapeSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCollisionShapeSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(const LNCollisionShapeSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCollisionShapeSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxCollisionShapeSerializeHandler

LN_FLAT_API LNResult LNBoxCollisionShapeSerializeHandler_Create(LNBoxCollisionShapeSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxCollisionShapeSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(const LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AnimationCurveSerializeHandler

LN_FLAT_API LNResult LNAnimationCurveSerializeHandler_Create(LNAnimationCurveSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAnimationCurveSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationCurveSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAnimationCurveSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationCurveSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationCurveSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// KeyFrameAnimationCurveSerializeHandler

LN_FLAT_API LNResult LNKeyFrameAnimationCurveSerializeHandler_Create(LNKeyFrameAnimationCurveSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNKeyFrameAnimationCurveSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNKeyFrameAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(const LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNKeyFrameAnimationCurveSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AnimationClipSerializeHandler

LN_FLAT_API LNResult LNAnimationClipSerializeHandler_Create(LNAnimationClipSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAnimationClipSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationClipSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAnimationClipSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationClipSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationClipSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationClipSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AnimationStateSerializeHandler

LN_FLAT_API LNResult LNAnimationStateSerializeHandler_Create(LNAnimationStateSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAnimationStateSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAnimationStateSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAnimationStateSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAnimationStateSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationStateSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAnimationStateSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EffectResourceSerializeHandler

LN_FLAT_API LNResult LNEffectResourceSerializeHandler_Create(LNEffectResourceSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEffectResourceSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEffectResourceSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEffectResourceSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEffectResourceSerializeHandler_RegisterSubclassTypeInfo(const LNEffectResourceSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEffectResourceSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ParticleEmitterModelSerializeHandler

LN_FLAT_API LNResult LNParticleEmitterModelSerializeHandler_Create(LNParticleEmitterModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNParticleEmitterModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitterModelSerializeHandler_RegisterSubclassTypeInfo(const LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitterModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ParticleModelSerializeHandler

LN_FLAT_API LNResult LNParticleModelSerializeHandler_Create(LNParticleModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNParticleModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNParticleModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleModelSerializeHandler_RegisterSubclassTypeInfo(const LNParticleModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ComponentSerializeHandler

LN_FLAT_API LNResult LNComponentSerializeHandler_Create(LNComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentSerializeHandler_RegisterSubclassTypeInfo(const LNComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualComponentSerializeHandler

LN_FLAT_API LNResult LNVisualComponentSerializeHandler_Create(LNVisualComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualComponentSerializeHandler_RegisterSubclassTypeInfo(const LNVisualComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteComponentSerializeHandler

LN_FLAT_API LNResult LNSpriteComponentSerializeHandler_Create(LNSpriteComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteComponentSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CharacterControllerSerializeHandler

LN_FLAT_API LNResult LNCharacterControllerSerializeHandler_Create(LNCharacterControllerSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCharacterControllerSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCharacterControllerSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCharacterControllerSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCharacterControllerSerializeHandler_RegisterSubclassTypeInfo(const LNCharacterControllerSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCharacterControllerSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldSerializeHandler

LN_FLAT_API LNResult LNWorldSerializeHandler_Create(LNWorldSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldSerializeHandler_RegisterSubclassTypeInfo(const LNWorldSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ComponentListSerializeHandler

LN_FLAT_API LNResult LNComponentListSerializeHandler_Create(LNComponentListSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNComponentListSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentListSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNComponentListSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentListSerializeHandler_RegisterSubclassTypeInfo(const LNComponentListSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentListSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldObjectSerializeHandler

LN_FLAT_API LNResult LNWorldObjectSerializeHandler_Create(LNWorldObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObjectSerializeHandler_RegisterSubclassTypeInfo(const LNWorldObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldObjectPreUpdateHandler

LN_FLAT_API LNResult LNWorldObjectPreUpdateHandler_Create(LNWorldObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObjectPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldObjectUpdateHandler

LN_FLAT_API LNResult LNWorldObjectUpdateHandler_Create(LNWorldObjectUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldObjectUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObjectUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldObjectUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObjectUpdateHandler_RegisterSubclassTypeInfo(const LNWorldObjectUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObjectUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualObjectSerializeHandler

LN_FLAT_API LNResult LNVisualObjectSerializeHandler_Create(LNVisualObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObjectSerializeHandler_RegisterSubclassTypeInfo(const LNVisualObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualObjectPreUpdateHandler

LN_FLAT_API LNResult LNVisualObjectPreUpdateHandler_Create(LNVisualObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObjectPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualObjectUpdateHandler

LN_FLAT_API LNResult LNVisualObjectUpdateHandler_Create(LNVisualObjectUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualObjectUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObjectUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualObjectUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObjectUpdateHandler_RegisterSubclassTypeInfo(const LNVisualObjectUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObjectUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraSerializeHandler

LN_FLAT_API LNResult LNCameraSerializeHandler_Create(LNCameraSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraSerializeHandler_RegisterSubclassTypeInfo(const LNCameraSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraPreUpdateHandler

LN_FLAT_API LNResult LNCameraPreUpdateHandler_Create(LNCameraPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraPreUpdateHandler_RegisterSubclassTypeInfo(const LNCameraPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraUpdateHandler

LN_FLAT_API LNResult LNCameraUpdateHandler_Create(LNCameraUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraUpdateHandler_RegisterSubclassTypeInfo(const LNCameraUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EnvironmentLightSerializeHandler

LN_FLAT_API LNResult LNEnvironmentLightSerializeHandler_Create(LNEnvironmentLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEnvironmentLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLightSerializeHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EnvironmentLightPreUpdateHandler

LN_FLAT_API LNResult LNEnvironmentLightPreUpdateHandler_Create(LNEnvironmentLightPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEnvironmentLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLightPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EnvironmentLightUpdateHandler

LN_FLAT_API LNResult LNEnvironmentLightUpdateHandler_Create(LNEnvironmentLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEnvironmentLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLightUpdateHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// DirectionalLightSerializeHandler

LN_FLAT_API LNResult LNDirectionalLightSerializeHandler_Create(LNDirectionalLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNDirectionalLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNDirectionalLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLightSerializeHandler_RegisterSubclassTypeInfo(const LNDirectionalLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// DirectionalLightPreUpdateHandler

LN_FLAT_API LNResult LNDirectionalLightPreUpdateHandler_Create(LNDirectionalLightPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNDirectionalLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLightPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// DirectionalLightUpdateHandler

LN_FLAT_API LNResult LNDirectionalLightUpdateHandler_Create(LNDirectionalLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNDirectionalLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNDirectionalLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLightUpdateHandler_RegisterSubclassTypeInfo(const LNDirectionalLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PointLightSerializeHandler

LN_FLAT_API LNResult LNPointLightSerializeHandler_Create(LNPointLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPointLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPointLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLightSerializeHandler_RegisterSubclassTypeInfo(const LNPointLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PointLightPreUpdateHandler

LN_FLAT_API LNResult LNPointLightPreUpdateHandler_Create(LNPointLightPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPointLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLightPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPointLightPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNPointLightPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLightPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PointLightUpdateHandler

LN_FLAT_API LNResult LNPointLightUpdateHandler_Create(LNPointLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPointLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPointLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLightUpdateHandler_RegisterSubclassTypeInfo(const LNPointLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpotLightSerializeHandler

LN_FLAT_API LNResult LNSpotLightSerializeHandler_Create(LNSpotLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpotLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpotLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLightSerializeHandler_RegisterSubclassTypeInfo(const LNSpotLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpotLightPreUpdateHandler

LN_FLAT_API LNResult LNSpotLightPreUpdateHandler_Create(LNSpotLightPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpotLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLightPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpotLightPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNSpotLightPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLightPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpotLightUpdateHandler

LN_FLAT_API LNResult LNSpotLightUpdateHandler_Create(LNSpotLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpotLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpotLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLightUpdateHandler_RegisterSubclassTypeInfo(const LNSpotLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteSerializeHandler

LN_FLAT_API LNResult LNSpriteSerializeHandler_Create(LNSpriteSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpritePreUpdateHandler

LN_FLAT_API LNResult LNSpritePreUpdateHandler_Create(LNSpritePreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpritePreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpritePreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpritePreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpritePreUpdateHandler_RegisterSubclassTypeInfo(const LNSpritePreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpritePreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteUpdateHandler

LN_FLAT_API LNResult LNSpriteUpdateHandler_Create(LNSpriteUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteUpdateHandler_RegisterSubclassTypeInfo(const LNSpriteUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraOrbitControlComponentSerializeHandler

LN_FLAT_API LNResult LNCameraOrbitControlComponentSerializeHandler_Create(LNCameraOrbitControlComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraOrbitControlComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraOrbitControlComponentSerializeHandler_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RaycasterSerializeHandler

LN_FLAT_API LNResult LNRaycasterSerializeHandler_Create(LNRaycasterSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRaycasterSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycasterSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRaycasterSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycasterSerializeHandler_RegisterSubclassTypeInfo(const LNRaycasterSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycasterSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RaycastResultSerializeHandler

LN_FLAT_API LNResult LNRaycastResultSerializeHandler_Create(LNRaycastResultSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRaycastResultSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycastResultSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRaycastResultSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycastResultSerializeHandler_RegisterSubclassTypeInfo(const LNRaycastResultSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycastResultSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldRenderViewSerializeHandler

LN_FLAT_API LNResult LNWorldRenderViewSerializeHandler_Create(LNWorldRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldRenderViewSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ShapeObjectSerializeHandler

LN_FLAT_API LNResult LNShapeObjectSerializeHandler_Create(LNShapeObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNShapeObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShapeObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNShapeObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShapeObjectSerializeHandler_RegisterSubclassTypeInfo(const LNShapeObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShapeObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ShapeObjectPreUpdateHandler

LN_FLAT_API LNResult LNShapeObjectPreUpdateHandler_Create(LNShapeObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNShapeObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShapeObjectPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShapeObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShapeObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ShapeObjectUpdateHandler

LN_FLAT_API LNResult LNShapeObjectUpdateHandler_Create(LNShapeObjectUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNShapeObjectUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNShapeObjectUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNShapeObjectUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNShapeObjectUpdateHandler_RegisterSubclassTypeInfo(const LNShapeObjectUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNShapeObjectUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PlaneMeshSerializeHandler

LN_FLAT_API LNResult LNPlaneMeshSerializeHandler_Create(LNPlaneMeshSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPlaneMeshSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMeshSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPlaneMeshSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(const LNPlaneMeshSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMeshSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PlaneMeshPreUpdateHandler

LN_FLAT_API LNResult LNPlaneMeshPreUpdateHandler_Create(LNPlaneMeshPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPlaneMeshPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMeshPreUpdateHandler_RegisterSubclassTypeInfo(const LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMeshPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PlaneMeshUpdateHandler

LN_FLAT_API LNResult LNPlaneMeshUpdateHandler_Create(LNPlaneMeshUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPlaneMeshUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMeshUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPlaneMeshUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(const LNPlaneMeshUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMeshUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxMeshSerializeHandler

LN_FLAT_API LNResult LNBoxMeshSerializeHandler_Create(LNBoxMeshSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxMeshSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMeshSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxMeshSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMeshSerializeHandler_RegisterSubclassTypeInfo(const LNBoxMeshSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMeshSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxMeshPreUpdateHandler

LN_FLAT_API LNResult LNBoxMeshPreUpdateHandler_Create(LNBoxMeshPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxMeshPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMeshPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMeshPreUpdateHandler_RegisterSubclassTypeInfo(const LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMeshPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxMeshUpdateHandler

LN_FLAT_API LNResult LNBoxMeshUpdateHandler_Create(LNBoxMeshUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxMeshUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMeshUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxMeshUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMeshUpdateHandler_RegisterSubclassTypeInfo(const LNBoxMeshUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMeshUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// MeshComponentSerializeHandler

LN_FLAT_API LNResult LNMeshComponentSerializeHandler_Create(LNMeshComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNMeshComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNMeshComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNMeshComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNMeshComponentSerializeHandler_RegisterSubclassTypeInfo(const LNMeshComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNMeshComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CollisionSerializeHandler

LN_FLAT_API LNResult LNCollisionSerializeHandler_Create(LNCollisionSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCollisionSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCollisionSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCollisionSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCollisionSerializeHandler_RegisterSubclassTypeInfo(const LNCollisionSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCollisionSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// TriggerBodyComponentSerializeHandler

LN_FLAT_API LNResult LNTriggerBodyComponentSerializeHandler_Create(LNTriggerBodyComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTriggerBodyComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTriggerBodyComponentSerializeHandler_RegisterSubclassTypeInfo(const LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ParticleEmitterSerializeHandler

LN_FLAT_API LNResult LNParticleEmitterSerializeHandler_Create(LNParticleEmitterSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNParticleEmitterSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitterSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNParticleEmitterSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitterSerializeHandler_RegisterSubclassTypeInfo(const LNParticleEmitterSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitterSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ParticleEmitterPreUpdateHandler

LN_FLAT_API LNResult LNParticleEmitterPreUpdateHandler_Create(LNParticleEmitterPreUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNParticleEmitterPreUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitterPreUpdateHandler_RegisterSubclassTypeInfo(const LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitterPreUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ParticleEmitterUpdateHandler

LN_FLAT_API LNResult LNParticleEmitterUpdateHandler_Create(LNParticleEmitterUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNParticleEmitterUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNParticleEmitterUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNParticleEmitterUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNParticleEmitterUpdateHandler_RegisterSubclassTypeInfo(const LNParticleEmitterUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNParticleEmitterUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelSerializeHandler

LN_FLAT_API LNResult LNLevelSerializeHandler_Create(LNLevelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelSerializeHandler_RegisterSubclassTypeInfo(const LNLevelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelStartHandler

LN_FLAT_API LNResult LNLevelStartHandler_Create(LNLevelStartHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelStartHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelStartHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelStartHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelStartHandler_RegisterSubclassTypeInfo(const LNLevelStartHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelStartHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelStopHandler

LN_FLAT_API LNResult LNLevelStopHandler_Create(LNLevelStopHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelStopHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelStopHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelStopHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelStopHandler_RegisterSubclassTypeInfo(const LNLevelStopHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelStopHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelPauseHandler

LN_FLAT_API LNResult LNLevelPauseHandler_Create(LNLevelPauseHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelPauseHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelPauseHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelPauseHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelPauseHandler_RegisterSubclassTypeInfo(const LNLevelPauseHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelPauseHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelResumeHandler

LN_FLAT_API LNResult LNLevelResumeHandler_Create(LNLevelResumeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelResumeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelResumeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelResumeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelResumeHandler_RegisterSubclassTypeInfo(const LNLevelResumeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelResumeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelUpdateHandler

LN_FLAT_API LNResult LNLevelUpdateHandler_Create(LNLevelUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelUpdateHandler_RegisterSubclassTypeInfo(const LNLevelUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIEventArgsSerializeHandler

LN_FLAT_API LNResult LNUIEventArgsSerializeHandler_Create(LNUIEventArgsSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIEventArgsSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventArgsSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIEventArgsSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventArgsSerializeHandler_RegisterSubclassTypeInfo(const LNUIEventArgsSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventArgsSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UILayoutElementSerializeHandler

LN_FLAT_API LNResult LNUILayoutElementSerializeHandler_Create(LNUILayoutElementSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUILayoutElementSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutElementSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUILayoutElementSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutElementSerializeHandler_RegisterSubclassTypeInfo(const LNUILayoutElementSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutElementSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIElementSerializeHandler

LN_FLAT_API LNResult LNUIElementSerializeHandler_Create(LNUIElementSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIElementSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIElementSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIElementSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIElementSerializeHandler_RegisterSubclassTypeInfo(const LNUIElementSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIElementSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UITextSerializeHandler

LN_FLAT_API LNResult LNUITextSerializeHandler_Create(LNUITextSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUITextSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUITextSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUITextSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUITextSerializeHandler_RegisterSubclassTypeInfo(const LNUITextSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUITextSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UISpriteSerializeHandler

LN_FLAT_API LNResult LNUISpriteSerializeHandler_Create(LNUISpriteSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUISpriteSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUISpriteSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUISpriteSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUISpriteSerializeHandler_RegisterSubclassTypeInfo(const LNUISpriteSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUISpriteSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIIconSerializeHandler

LN_FLAT_API LNResult LNUIIconSerializeHandler_Create(LNUIIconSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIIconSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIIconSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIIconSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIIconSerializeHandler_RegisterSubclassTypeInfo(const LNUIIconSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIIconSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIMessageTextAreaSerializeHandler

LN_FLAT_API LNResult LNUIMessageTextAreaSerializeHandler_Create(LNUIMessageTextAreaSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIMessageTextAreaSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIMessageTextAreaSerializeHandler_RegisterSubclassTypeInfo(const LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIMessageTextAreaSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UILayoutPanelSerializeHandler

LN_FLAT_API LNResult LNUILayoutPanelSerializeHandler_Create(LNUILayoutPanelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUILayoutPanelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutPanelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutPanelSerializeHandler_RegisterSubclassTypeInfo(const LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutPanelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIBoxLayoutSerializeHandler

LN_FLAT_API LNResult LNUIBoxLayoutSerializeHandler_Create(LNUIBoxLayoutSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIBoxLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIBoxLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIBoxLayoutSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIStackLayoutSerializeHandler

LN_FLAT_API LNResult LNUIStackLayoutSerializeHandler_Create(LNUIStackLayoutSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIStackLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIStackLayoutSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIStackLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIStackLayoutSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIGridLayoutSerializeHandler

LN_FLAT_API LNResult LNUIGridLayoutSerializeHandler_Create(LNUIGridLayoutSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIGridLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIGridLayoutSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIGridLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIGridLayoutSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIControlSerializeHandler

LN_FLAT_API LNResult LNUIControlSerializeHandler_Create(LNUIControlSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIControlSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIControlSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIControlSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIControlSerializeHandler_RegisterSubclassTypeInfo(const LNUIControlSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIControlSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIButtonBaseSerializeHandler

LN_FLAT_API LNResult LNUIButtonBaseSerializeHandler_Create(LNUIButtonBaseSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIButtonBaseSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIButtonBaseSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIButtonBaseSerializeHandler_RegisterSubclassTypeInfo(const LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIButtonBaseSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIButtonSerializeHandler

LN_FLAT_API LNResult LNUIButtonSerializeHandler_Create(LNUIButtonSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIButtonSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIButtonSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIButtonSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIButtonSerializeHandler_RegisterSubclassTypeInfo(const LNUIButtonSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIButtonSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIWindowSerializeHandler

LN_FLAT_API LNResult LNUIWindowSerializeHandler_Create(LNUIWindowSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIWindowSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIWindowSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIWindowSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIWindowSerializeHandler_RegisterSubclassTypeInfo(const LNUIWindowSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIWindowSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIListItemSerializeHandler

LN_FLAT_API LNResult LNUIListItemSerializeHandler_Create(LNUIListItemSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIListItemSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListItemSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIListItemSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListItemSerializeHandler_RegisterSubclassTypeInfo(const LNUIListItemSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListItemSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIListItemsControlSerializeHandler

LN_FLAT_API LNResult LNUIListItemsControlSerializeHandler_Create(LNUIListItemsControlSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIListItemsControlSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListItemsControlSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListItemsControlSerializeHandler_RegisterSubclassTypeInfo(const LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListItemsControlSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIListBoxItemSerializeHandler

LN_FLAT_API LNResult LNUIListBoxItemSerializeHandler_Create(LNUIListBoxItemSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIListBoxItemSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListBoxItemSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListBoxItemSerializeHandler_RegisterSubclassTypeInfo(const LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListBoxItemSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIListBoxSerializeHandler

LN_FLAT_API LNResult LNUIListBoxSerializeHandler_Create(LNUIListBoxSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIListBoxSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIListBoxSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIListBoxSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIListBoxSerializeHandler_RegisterSubclassTypeInfo(const LNUIListBoxSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIListBoxSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InputGestureSerializeHandler

LN_FLAT_API LNResult LNInputGestureSerializeHandler_Create(LNInputGestureSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInputGestureSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInputGestureSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInputGestureSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInputGestureSerializeHandler_RegisterSubclassTypeInfo(const LNInputGestureSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInputGestureSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// KeyGestureSerializeHandler

LN_FLAT_API LNResult LNKeyGestureSerializeHandler_Create(LNKeyGestureSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNKeyGestureSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNKeyGestureSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNKeyGestureSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNKeyGestureSerializeHandler_RegisterSubclassTypeInfo(const LNKeyGestureSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNKeyGestureSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterCommandSerializeHandler

LN_FLAT_API LNResult LNInterpreterCommandSerializeHandler_Create(LNInterpreterCommandSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterCommandListSerializeHandler

LN_FLAT_API LNResult LNInterpreterCommandListSerializeHandler_Create(LNInterpreterCommandListSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandListSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandListSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandListSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterSerializeHandler

LN_FLAT_API LNResult LNInterpreterSerializeHandler_Create(LNInterpreterSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterUpdateWaitHandler

LN_FLAT_API LNResult LNInterpreterUpdateWaitHandler_Create(LNInterpreterUpdateWaitHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterUpdateWaitHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterUpdateWaitHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterUpdateWaitHandler_RegisterSubclassTypeInfo(const LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterUpdateWaitHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationSerializeHandler

LN_FLAT_API LNResult LNApplicationSerializeHandler_Create(LNApplicationSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationSerializeHandler_RegisterSubclassTypeInfo(const LNApplicationSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationInitHandler

LN_FLAT_API LNResult LNApplicationInitHandler_Create(LNApplicationInitHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationInitHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationInitHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationInitHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationInitHandler_RegisterSubclassTypeInfo(const LNApplicationInitHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationInitHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationUpdateHandler

LN_FLAT_API LNResult LNApplicationUpdateHandler_Create(LNApplicationUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationUpdateHandler_RegisterSubclassTypeInfo(const LNApplicationUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationUpdateHandler_GetSubinstanceId(LNHandle handle);



} // extern "C"

