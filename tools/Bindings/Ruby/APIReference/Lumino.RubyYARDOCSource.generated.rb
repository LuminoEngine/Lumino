# LogLevel
module Lumino::LogLevel
    # 
    UNKNOWN = 0
    # 
    VERBOSE = 1
    # 
    DEBUG = 2
    # 
    INFO = 3
    # 
    WARNING = 4
    # 
    ERROR = 5
    # 
    FATAL = 6
    # 
    DISBLE = 7
end

# EncodingType
module Lumino::EncodingType
    # 不明な文字エンコーディング (判別失敗。またはバイナリファイル)
    UNKNOWN = 0
    # ASCII (拡張アスキーは含まない http://www.asciitable.com/)
    ASCII = 1
    # UTF-8
    UTF8 = 2
    # UTF-8 (BOM 無し)
    UTF8N = 3
    # 日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装
    SJIS = 4
end

# Keys
module Lumino::Keys
    # 
    UNKNOWN = 0
    # 
    A = 1
    # 
    B = 2
    # 
    C = 3
    # 
    D = 4
    # 
    E = 5
    # 
    F = 6
    # 
    G = 7
    # 
    H = 8
    # 
    I = 9
    # 
    J = 10
    # 
    K = 11
    # 
    L = 12
    # 
    M = 13
    # 
    N = 14
    # 
    O = 15
    # 
    P = 16
    # 
    Q = 17
    # 
    R = 18
    # 
    S = 19
    # 
    T = 20
    # 
    U = 21
    # 
    V = 22
    # 
    W = 23
    # 
    X = 24
    # 
    Y = 25
    # 
    Z = 26
    # 
    D0 = 27
    # 
    D1 = 28
    # 
    D2 = 29
    # 
    D3 = 30
    # 
    D4 = 31
    # 
    D5 = 32
    # 
    D6 = 33
    # 
    D7 = 34
    # 
    D8 = 35
    # 
    D9 = 36
    # 
    F1 = 37
    # 
    F2 = 38
    # 
    F3 = 39
    # 
    F4 = 40
    # 
    F5 = 41
    # 
    F6 = 42
    # 
    F7 = 43
    # 
    F8 = 44
    # 
    F9 = 45
    # 
    F10 = 46
    # 
    F11 = 47
    # 
    F12 = 48
    # 
    SPACE = 49
    # 
    ESCAPE = 50
    # 
    UP = 51
    # 
    DOWN = 52
    # 
    LEFT = 53
    # 
    RIGHT = 54
    # 
    LSHIFT = 55
    # 
    RSHIFT = 56
    # 
    LCTRL = 57
    # 
    RCTRL = 58
    # 
    LALT = 59
    # 
    RALT = 60
    # 
    TAB = 61
    # 
    ENTER = 62
    # 
    BACK_SPACE = 63
    # 
    INSERT = 64
    # 
    DELETE = 65
    # 
    PAGE_UP = 66
    # 
    PAGE_DOWN = 67
    # 
    HOME = 68
    # 
    END = 69
    # [非推奨 GLFWでは;]
    COLON = 70
    # ;	[非推奨 GLFWでは=]
    SEMICOLON = 71
    # ,
    COMMA = 72
    # .
    PERIOD = 73
    # /
    SLASH = 74
    # -
    MINUS = 75
    # \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
    BACKSLASH = 76
    # \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
    YEN = 77
    # ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
    CARET = 78
    # [
    LBRACKET = 79
    # ]
    RBRACKET = 80
    # Android '+'
    ANDROID_PLUS = 81
    # 
    TERMINATOR = 82
end

# MouseButtons
module Lumino::MouseButtons
    # 無効または押されていないことを示す
    NONE = 0
    # 左ボタン
    LEFT = 1
    # 右ボタン
    RIGHT = 2
    # 中央ボタン
    MIDDLE = 3
    # 拡張ボタン1
    X1 = 4
    # 拡張ボタン2
    X2 = 5
end

# SoundFadeBehavior
module Lumino::SoundFadeBehavior
    # 再生を継続する
    CONTINUE = 0
    # 停止する
    STOP = 1
    # 停止して、次の再生に備えてサウンドの音量を元の値に戻す
    STOP_RESET = 2
    # 一時停止する
    PAUSE = 3
    # 一時停止して、次の再生に備えてサウンドの音量を元の値に戻す
    PAUSE_RESET = 4
end

# GraphicsAPI
module Lumino::GraphicsAPI
    # 自動選択
    DEFAULT = 0
    # OpenGL
    OPEN_GL = 1
    # Vulkan
    VULKAN = 2
    # Vulkan
    DIRECT_X12 = 3
end

# PixelFormat
module Lumino::PixelFormat
    # Unknown
    UNKNOWN = 0
    # 8bit アルファ値のみのフォーマット
    A8 = 1
    # RGBA オーダーの各要素 8bit フォーマット
    RGBA8 = 2
    # RGB オーダーの各要素 8bit フォーマット
    RGB8 = 3
    # RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    RGBA32F = 4
    # RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    R32S = 5
end

# TextureFormat
module Lumino::TextureFormat
    # Unknown
    UNKNOWN = 0
    # RGBA オーダーの各要素 8bit フォーマット
    RGBA8 = 1
    # RGB オーダーの各要素 8bit フォーマット
    RGB8 = 2
    # RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    RGBA16F = 3
    # RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    RGBA32F = 4
    # 16bit 浮動小数点フォーマット
    R16F = 5
    # 32bit 浮動小数点フォーマット
    R32F = 6
    # 32bit の符号あり整数フォーマット
    R32S = 7
end

# DepthBufferFormat
module Lumino::DepthBufferFormat
    # S8 32 ビット
    D24S8 = 0
end

# ShadingModel
module Lumino::ShadingModel
    # Default
    DEFAULT = 0
    # Unlit
    UNLIT = 1
end

# BlendMode
module Lumino::BlendMode
    # 通常
    NORMAL = 0
    # アルファブレンド (RGB をアルファブレンドし、A を加算合成)
    ALPHA = 1
    # 加算合成
    ADD = 2
    # 減算合成
    SUBTRACT = 3
    # 乗算合成
    MULTIPLY = 4
end

# SceneClearMode
module Lumino::SceneClearMode
    # クリアしません。
    NONE = 0
    # 背景色と深度バッファをクリアします。
    COLOR_AND_DEPTH = 1
    # (未実装)
    SKY = 2
    # (未実装)
    SKY0 = 3
    # スカイドームでクリアします。
    SKY_DOME = 4
end

# AnimationWrapMode
module Lumino::AnimationWrapMode
    # 繰り返しを行わず、1度だけ再生します。
    ONCE = 0
    # 最後まで再生された後、先頭に戻ってループします。
    LOOP = 1
    # 最後まで再生された後、逆方向に戻ってループします。
    ALTERNATE = 2
end

# HierarchicalAnimationMode
module Lumino::HierarchicalAnimationMode
    # ルートノードのみ、平行移動を有効化します。
    ALLOW_TRANSLATION_ONLY_ROOT = 0
    # AllowTranslationOnlyRootY
    ALLOW_TRANSLATION_ONLY_ROOT_Y = 1
    # すべてのノードの平行移動を有効化します。
    ALLOW_TRANSLATION = 2
    # 平行移動を無効化します。
    DISABLE_TRANSLATION = 3
end

# TangentMode
module Lumino::TangentMode
    # 線形補間
    LINEAR = 0
    # 接線 (速度) を使用した補間 (エルミートスプライン)
    TANGENT = 1
    # キーフレームの値を通過するなめらかな補間 (Catmull-Rom)
    AUTO = 2
    # 補間なし
    CONSTANT = 3
end

# ParticleEmitterShapeType
module Lumino::ParticleEmitterShapeType
    # 
    SPHERE = 0
    # パーティクルを球状に放出します。
    CONE = 1
    # パーティクルをコーン型に放出します。
    BOX = 2
end

# ParticleGeometryDirection
module Lumino::ParticleGeometryDirection
    # 常に視点方向を向く
    TO_VIEW = 0
    # 進行方向を軸に、表面 (Y+) が視点から見えるようにする
    TOP = 1
    # Y 軸方向に直立し、カメラの方を向きます。
    VERTICAL_BILLBOARD = 2
    # XZ 軸上の「床」平面と平行になります。
    HORIZONTAL_BILLBOARD = 3
end

# LevelTransitionEffectMode
module Lumino::LevelTransitionEffectMode
    # エフェクト無し
    NONE = 0
    # フェードイン・フェードアウト
    FADE_IN_OUT = 1
    # クロスフェード
    CROSS_FADE = 2
end

# UILayoutOrientation
module Lumino::UILayoutOrientation
    # 水平方向に配置します。
    HORIZONTAL = 0
    # 垂直方向に配置します。
    VERTICAL = 1
    # 水平方向（右から左）に配置します。
    REVERSE_HORIZONTAL = 2
    # 垂直方向（下から上）に配置します。
    REVERSE_VERTICAL = 3
end

# UIVisibility
module Lumino::UIVisibility
    # 要素を表示します。
    VISIBLE = 0
    # 要素を表示しませんが、その要素の領域をレイアウト内に予約します。
    HIDDEN = 1
    # 要素を表示しません。また、その要素の領域もレイアウト内に予約しません。
    COLLAPSED = 2
end

# UIColorHues
module Lumino::UIColorHues
    # 
    RED = 0
    # 
    PINK = 1
    # 
    PURPLE = 2
    # 
    DEEP_PURPLE = 4
    # 
    INDIGO = 5
    # 
    BLUE = 6
    # 
    LIGHT_BLUE = 7
    # 
    CYAN = 8
    # 
    TEAL = 9
    # 
    GREEN = 10
    # 
    LIGHT_GREEN = 11
    # 
    LIME = 12
    # 
    YELLOW = 13
    # 
    AMBER = 14
    # 
    ORANGE = 15
    # 
    DEEP_ORANGE = 16
    # 
    BROWN = 17
    # 
    GREY = 18
    # 
    BLACK = 19
    # 
    WHITE = 20
end

# UIVAlignment
module Lumino::UIVAlignment
    # 子要素を、親のレイアウト スロットの上端に揃えて配置します。
    TOP = 0
    # 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    CENTER = 1
    # 子要素を、親のレイアウト スロットの下端に揃えて配置します。
    BOTTOM = 2
    # 子要素を、親のレイアウト スロット全体に引き伸ばします。
    STRETCH = 3
end

# UIHAlignment
module Lumino::UIHAlignment
    # 子要素を、親のレイアウト スロットの左側に揃えて配置します。
    LEFT = 0
    # 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    CENTER = 1
    # 子要素を、親のレイアウト スロットの右側に揃えて配置します。
    RIGHT = 2
    # 子要素を、親のレイアウト スロット全体に引き伸ばします。
    STRETCH = 3
end

# UIInlinePlacement
module Lumino::UIInlinePlacement
    # TopLeft
    TOP_LEFT = 0
    # Top
    TOP = 1
    # TopRight
    TOP_RIGHT = 2
    # Left
    LEFT = 3
    # Center
    CENTER = 4
    # Right
    RIGHT = 5
    # BottomLeft
    BOTTOM_LEFT = 6
    # Bottom
    BOTTOM = 7
    # BottomRight
    BOTTOM_RIGHT = 8
end

# UIListSubmitMode
module Lumino::UIListSubmitMode
    # ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
    SINGLE = 0
    # エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.
    DOUBLE = 1
end

# 3次元のベクトルを定義します。
# 
class Lumino::Vector3
    # 
    # @return [Float] 
    def x(*args)
    end



    # 
    # @param [Float] value 
    # 
    def x=(*args)
    end



    # 
    # @return [Float] 
    def y(*args)
    end



    # 
    # @param [Float] value 
    # 
    def y=(*args)
    end



    # 
    # @return [Float] 
    def z(*args)
    end



    # 
    # @param [Float] value 
    # 
    def z=(*args)
    end



    # 指定した値を使用してインスタンスを初期化します。
    # @overload initialize()
    #   すべての要素を 0.0 に設定してインスタンスを初期化します。
    # @overload initialize(x, y, z)
    #   指定した値を使用してインスタンスを初期化します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def initialize(*args)
    end



    # get
    # @param [Float] outX 
    # @param [Float] outY 
    # @param [Float] outZ 
    # 
    def get(*args)
    end



    # ベクトルの長さを返します。
    # @return [Float] 
    def length(*args)
    end



    # ベクトルの長さの 2 乗を返します。
    # @return [Float] 
    def length_squared(*args)
    end



    # このベクトルを正規化します。
    #   ベクトルの長さが 0 の場合は正規化を行いません。
    def mutating_normalize(*args)
    end



    # 指定ベクトルを正規化したベクトルを返します。
    # @overload normalize(x, y, z)
    #   指定ベクトルを正規化したベクトルを返します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    #   @return [Lumino::Vector3] 正規化されたベクトル
    # @overload normalize(vec)
    #   指定ベクトルを正規化したベクトルを返します。
    #   @param [Lumino::Vector3] vec 
    #   
    #   @return [Lumino::Vector3] 正規化されたベクトル
    def normalize(*args)
    end



end

# 4次元のベクトルを定義します。
# 
class Lumino::Vector4
    # 
    # @return [Float] 
    def x(*args)
    end



    # 
    # @param [Float] value 
    # 
    def x=(*args)
    end



    # 
    # @return [Float] 
    def y(*args)
    end



    # 
    # @param [Float] value 
    # 
    def y=(*args)
    end



    # 
    # @return [Float] 
    def z(*args)
    end



    # 
    # @param [Float] value 
    # 
    def z=(*args)
    end



    # 
    # @return [Float] 
    def w(*args)
    end



    # 
    # @param [Float] value 
    # 
    def w=(*args)
    end



    # 指定した値を使用してインスタンスを初期化します。
    # @overload initialize()
    #   すべての要素を 0.0 に設定してインスタンスを初期化します。
    # @overload initialize(x, y, z, w)
    #   指定した値を使用してインスタンスを初期化します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   @param [Float] w 
    #   
    def initialize(*args)
    end



end

# クォータニオンを定義します。
# 
class Lumino::Quaternion
    # 
    # @return [Float] 
    def x(*args)
    end



    # 
    # @param [Float] value 
    # 
    def x=(*args)
    end



    # 
    # @return [Float] 
    def y(*args)
    end



    # 
    # @param [Float] value 
    # 
    def y=(*args)
    end



    # 
    # @return [Float] 
    def z(*args)
    end



    # 
    # @param [Float] value 
    # 
    def z=(*args)
    end



    # 
    # @return [Float] 
    def w(*args)
    end



    # 
    # @param [Float] value 
    # 
    def w=(*args)
    end



    # 指定した値を使用してインスタンスを初期化します。
    # @overload initialize()
    #   単位クォータニオンを設定してインスタンスを初期化します。
    # @overload initialize(x, y, z, w)
    #   指定した値を使用してインスタンスを初期化します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   @param [Float] w 
    #   
    # @overload initialize(axis, r)
    #   回転軸と回転角度を指定してインスタンスを初期化します。
    #     axis が単位ベクトルでなければ正規化してから計算を行います。
    #   @param [Lumino::Vector3] axis 
    #   @param [Float] r 
    #   
    def initialize(*args)
    end



end

# 4x4 の行列を定義します。
# 
class Lumino::Matrix
    # 
    # @return [Lumino::Vector4] 
    def row_0(*args)
    end



    # 
    # @param [Lumino::Vector4] value 
    # 
    def row_0=(*args)
    end



    # 
    # @return [Lumino::Vector4] 
    def row_1(*args)
    end



    # 
    # @param [Lumino::Vector4] value 
    # 
    def row_1=(*args)
    end



    # 
    # @return [Lumino::Vector4] 
    def row_2(*args)
    end



    # 
    # @param [Lumino::Vector4] value 
    # 
    def row_2=(*args)
    end



    # 
    # @return [Lumino::Vector4] 
    def row_3(*args)
    end



    # 
    # @param [Lumino::Vector4] value 
    # 
    def row_3=(*args)
    end



    # 各要素を指定してインスタンスを初期化します。
    # @overload initialize()
    #   単位行列を設定してインスタンスを初期化します。
    # @overload initialize(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
    #   各要素を指定してインスタンスを初期化します。
    #   @param [Float] m11 
    #   @param [Float] m12 
    #   @param [Float] m13 
    #   @param [Float] m14 
    #   @param [Float] m21 
    #   @param [Float] m22 
    #   @param [Float] m23 
    #   @param [Float] m24 
    #   @param [Float] m31 
    #   @param [Float] m32 
    #   @param [Float] m33 
    #   @param [Float] m34 
    #   @param [Float] m41 
    #   @param [Float] m42 
    #   @param [Float] m43 
    #   @param [Float] m44 
    #   
    def initialize(*args)
    end



end

# 各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
# 
class Lumino::Color
    # 
    # @return [Float] 
    def r(*args)
    end



    # 
    # @param [Float] value 
    # 
    def r=(*args)
    end



    # 
    # @return [Float] 
    def g(*args)
    end



    # 
    # @param [Float] value 
    # 
    def g=(*args)
    end



    # 
    # @return [Float] 
    def b(*args)
    end



    # 
    # @param [Float] value 
    # 
    def b=(*args)
    end



    # 
    # @return [Float] 
    def a(*args)
    end



    # 
    # @param [Float] value 
    # 
    def a=(*args)
    end



    # 各要素を指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0.0 で初期化します。
    # @overload initialize(r_, g_, b_, a_)
    #   各要素を指定して初期化します。
    #   @param [Float] r_ 
    #   @param [Float] g_ 
    #   @param [Float] b_ 
    #   @param [Float] a_ 
    #   
    def initialize(*args)
    end



end

# 色調を定義します。
# 
class Lumino::ColorTone
    # 
    # @return [Float] 
    def r(*args)
    end



    # 
    # @param [Float] value 
    # 
    def r=(*args)
    end



    # 
    # @return [Float] 
    def g(*args)
    end



    # 
    # @param [Float] value 
    # 
    def g=(*args)
    end



    # 
    # @return [Float] 
    def b(*args)
    end



    # 
    # @param [Float] value 
    # 
    def b=(*args)
    end



    # 
    # @return [Float] 
    def s(*args)
    end



    # 
    # @param [Float] value 
    # 
    def s=(*args)
    end



    # 各要素を指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0.0 で初期化します。
    # @overload initialize(r_, g_, b_, s_)
    #   各要素を指定して初期化します。
    #   @param [Float] r_ 
    #   @param [Float] g_ 
    #   @param [Float] b_ 
    #   @param [Float] s_ 
    #   
    def initialize(*args)
    end



end

# 2次元上の点を表します。
# 
class Lumino::Point
    # 
    # @return [Float] 
    def x(*args)
    end



    # 
    # @param [Float] value 
    # 
    def x=(*args)
    end



    # 
    # @return [Float] 
    def y(*args)
    end



    # 
    # @param [Float] value 
    # 
    def y=(*args)
    end



    # 位置を指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0 で初期化します。
    # @overload initialize(x_, y_)
    #   位置を指定して初期化します。
    #   @param [Float] x_ 
    #   @param [Float] y_ 
    #   
    def initialize(*args)
    end



    # 各要素の値を取得します。
    # @param [Float] outX 
    # @param [Float] outY 
    # 
    def get(*args)
    end



end

# 2次元上のオブジェクトサイズを表します。
# 
class Lumino::Size
    # 
    # @return [Float] 
    def width(*args)
    end



    # 
    # @param [Float] value 
    # 
    def width=(*args)
    end



    # 
    # @return [Float] 
    def height(*args)
    end



    # 
    # @param [Float] value 
    # 
    def height=(*args)
    end



    # 幅と高さを指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0 で初期化します。
    # @overload initialize(w, h)
    #   幅と高さを指定して初期化します。
    #   @param [Float] w 
    #   @param [Float] h 
    #   
    def initialize(*args)
    end



end

# 2次元の矩形を表すクラスです。
# 
class Lumino::Rect
    # 
    # @return [Float] 
    def x(*args)
    end



    # 
    # @param [Float] value 
    # 
    def x=(*args)
    end



    # 
    # @return [Float] 
    def y(*args)
    end



    # 
    # @param [Float] value 
    # 
    def y=(*args)
    end



    # 
    # @return [Float] 
    def width(*args)
    end



    # 
    # @param [Float] value 
    # 
    def width=(*args)
    end



    # 
    # @return [Float] 
    def height(*args)
    end



    # 
    # @param [Float] value 
    # 
    def height=(*args)
    end



    # 位置とサイズを指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0 で初期化します。
    # @overload initialize(x, y, width, height)
    #   位置とサイズを指定して初期化します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def initialize(*args)
    end



    # 左辺の x 軸の値を取得します。
    # @return [Float] 
    def get_left(*args)
    end



    # 幅と高さを設定します。
    # @param [Lumino::Size] size 
    # 
    def size=(*args)
    end



    # 幅と高さを取得します。
    # @return [Lumino::Size] 
    def size(*args)
    end



end

# 四角形の枠の太さを表すクラスです。
# 
class Lumino::Thickness
    # 
    # @return [Float] 
    def left(*args)
    end



    # 
    # @param [Float] value 
    # 
    def left=(*args)
    end



    # 
    # @return [Float] 
    def top(*args)
    end



    # 
    # @param [Float] value 
    # 
    def top=(*args)
    end



    # 
    # @return [Float] 
    def right(*args)
    end



    # 
    # @param [Float] value 
    # 
    def right=(*args)
    end



    # 
    # @return [Float] 
    def bottom(*args)
    end



    # 
    # @param [Float] value 
    # 
    def bottom=(*args)
    end



    # 各辺の幅を指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0 で初期化します。
    # @overload initialize(left_, top_, right_, bottom_)
    #   各辺の幅を指定して初期化します。
    #   @param [Float] left_ 
    #   @param [Float] top_ 
    #   @param [Float] right_ 
    #   @param [Float] bottom_ 
    #   
    def initialize(*args)
    end



end

# 四角形の角の半径を表します。
# 
class Lumino::CornerRadius
    # 
    # @return [Float] 
    def topleft(*args)
    end



    # 
    # @param [Float] value 
    # 
    def topleft=(*args)
    end



    # 
    # @return [Float] 
    def topright(*args)
    end



    # 
    # @param [Float] value 
    # 
    def topright=(*args)
    end



    # 
    # @return [Float] 
    def bottomright(*args)
    end



    # 
    # @param [Float] value 
    # 
    def bottomright=(*args)
    end



    # 
    # @return [Float] 
    def bottomleft(*args)
    end



    # 
    # @param [Float] value 
    # 
    def bottomleft=(*args)
    end



    # 各要素の値を指定して初期化します。
    # @overload initialize()
    #   すべての要素を 0 で初期化します。
    # @overload initialize(topLeft, topRight, bottomRight, bottomLeft)
    #   各要素の値を指定して初期化します。
    #   @param [Float] topLeft 
    #   @param [Float] topRight 
    #   @param [Float] bottomRight 
    #   @param [Float] bottomLeft 
    #   
    def initialize(*args)
    end



end

# 
# 
class Lumino::Object
    # onSerialize
    # @param [Lumino::Serializer2] ar 
    # 
    def on_serialize(*args)
    end



    # オブジェクトの参照を開放します。
    def release(*args)
    end



    # オブジェクトの参照を取得します。
    def retain(*args)
    end



    # オブジェクトの参照カウントを取得します。これは内部的に使用される関数です。
    # @return [Integer] 
    def get_reference_count(*args)
    end



end

# イベントハンドラの状態を追跡します。必要に応じて、イベントから切断するために使用します。
# 
class Lumino::EventConnection
end

# Promise failure.
# 
class Lumino::PromiseFailureDelegate
    # 
    # @param [Lumino::PromiseFailureDelegate_Function] callback 
    # 
    def initialize(*args)
    end



end

# Variant
# 
class Lumino::Variant
    # init.
    def initialize(*args)
    end



    # setInt
    # @param [Integer] value 
    # 
    def set_int(*args)
    end



    # getInt
    # @return [Integer] 
    def get_int(*args)
    end



end

# Test delegate 1.
# 
class Lumino::ZVTestDelegate1
    # 
    # @param [Lumino::ZVTestDelegate1_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test delegate 2.
# 
class Lumino::ZVTestDelegate2
    # 
    # @param [Lumino::ZVTestDelegate2_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test delegate 3.
# 
class Lumino::ZVTestDelegate3
    # 
    # @param [Lumino::ZVTestDelegate3_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test ZVTestEventHandler1.
# 
class Lumino::ZVTestEventHandler1
    # 
    # @param [Lumino::ZVTestEventHandler1_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test ZVTestEventHandler2.
# 
class Lumino::ZVTestEventHandler2
    # 
    # @param [Lumino::ZVTestEventHandler2_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test promise.
# 
class Lumino::ZVTestPromise1
    # 
    # @param [Lumino::ZVTestDelegate3] callback 
    # 
    def then_with(*args)
    end



    # 
    # @param [Lumino::PromiseFailureDelegate] callback 
    # 
    def catch_with(*args)
    end



end

# Test promise.
# 
class Lumino::ZVTestPromise2
    # 
    # @param [Lumino::ZVTestDelegate1] callback 
    # 
    def then_with(*args)
    end



    # 
    # @param [Lumino::PromiseFailureDelegate] callback 
    # 
    def catch_with(*args)
    end



end

# Test class.
# 
class Lumino::ZVTestClass1
    # init method.
    def initialize(*args)
    end



    # setTestDelegate1 method.
    # @param [Lumino::ZVTestDelegate1] value 
    # 
    def set_test_delegate_1(*args)
    end



    # setTestDelegate2 method.
    # @param [Lumino::ZVTestDelegate2] value 
    # 
    def set_test_delegate_2(*args)
    end



    # setTestDelegate3 method.
    # @param [Lumino::ZVTestDelegate3] value 
    # 
    def set_test_delegate_3(*args)
    end



    # callTestDelegate1 method.
    # @param [Integer] a 
    # 
    def call_test_delegate_1(*args)
    end



    # callTestDelegate2 method.
    # @param [Integer] a 
    # @param [Integer] b 
    # 
    # @return [Integer] 
    def call_test_delegate_2(*args)
    end



    # callTestDelegate3 method. (create object in internal)
    def call_test_delegate_3(*args)
    end



    # Promise test method. (static)
    # @param [String] filePath 
    # 
    # @return [Lumino::ZVTestPromise1] 
    def load_async(*args)
    end



    # Promise test method. (instance)
    # @return [Lumino::ZVTestPromise2] 
    def execute_async(*args)
    end



    # Promise test method.
    # @return [String] 
    def file_path(*args)
    end



    # connectOnEvent1 method.
    # @param [Lumino::ZVTestEventHandler1] handler 
    # 
    # @return [Lumino::EventConnection] 
    def connect_on_event_1(*args)
    end



    # raiseEvent1 method.
    def raise_event_1(*args)
    end



    # connectOnEvent2 method.
    # @param [Lumino::ZVTestEventHandler2] handler 
    # 
    # @return [Lumino::EventConnection] 
    def connect_on_event_2(*args)
    end



    # raiseEvent2 method.
    def raise_event_2(*args)
    end



end

# Test class.
# 
class Lumino::ZVTestEventArgs1
    # init method.
    # @overload initialize()
    #   init method.
    # @overload initialize(v)
    #   init method.
    #   @param [Integer] v 
    #   
    def initialize(*args)
    end



    # value method.
    # @return [Integer] 
    def value(*args)
    end



end

# log
# 
class Lumino::Log
    # setLevel
    # @param [Lumino::LogLevel] level 
    # 
    def set_level(*args)
    end



    # allocConsole
    def alloc_console(*args)
    end



    # write
    # @param [Lumino::LogLevel] level 
    # @param [] tag 
    # @param [] text 
    # 
    def write(*args)
    end



end

# 
# 
class Lumino::Serializer2
end

# アセットファイルやその他の外部ファイルをインポートして構築可能なオブジェクトのベースクラスです。
# 
class Lumino::AssetObject
end

# 
# 
class Lumino::AssetImportSettings
end

# 
# 
class Lumino::AssetModel
    # init
    # @param [Lumino::Object] target 
    # 
    def initialize(*args)
    end



    # target
    # @return [Lumino::Object] 
    def target(*args)
    end



end

# 
# 
class Lumino::Assets
    # Internal
    # @param [Lumino::AssetModel] asset 
    # @param [String] filePath 
    # 
    def save_asset_to_local_file(*args)
    end



    # Internal
    # @param [String] filePath 
    # 
    # @return [Lumino::AssetModel] 
    def load_asset_from_local_file(*args)
    end



    # 指定したアセットファイルを読み込み、オブジェクト生成します。
    #   ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。
    # @param [] filePath 
    # 
    # @return [Lumino::Object] 
    def load_asset(*args)
    end



    # 指定したアセットファイルを読み込み、作成済みのオブジェクトへ適用します。
    #   このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。
    # @param [] filePath 
    # @param [Lumino::Object] obj 
    # 
    def reload_asset(*args)
    end



    # readAllText
    # @param [] filePath 
    # @param [Lumino::EncodingType] encoding 
    # 
    # @return [String] 
    def read_all_text(*args)
    end



end

# Sound
# 
class Lumino::Sound
    # この Sound の音量を設定します。
    # @param [Float] value 
    # 
    def volume=(*args)
    end



    # この Sound の音量を取得します。
    # @return [Float] 
    def volume(*args)
    end



    # この Sound のピッチ (音高) を設定します。
    # @param [Float] value 
    # 
    def pitch=(*args)
    end



    # この Sound のピッチ (音高) を取得します。
    # @return [Float] 
    def pitch(*args)
    end



    # ループ再生の有無を設定します。
    # @param [Boolean] enabled 
    # 
    def loop_enabled=(*args)
    end



    # ループ再生が有効かを確認します。
    # @return [Boolean] 
    def loop_enabled?(*args)
    end



    # ループ範囲を設定します。
    # @param [Integer] begin 
    # @param [Integer] length 
    # 
    def set_loop_range(*args)
    end



    # この Sound の再生を開始します。
    def play(*args)
    end



    # この Sound の再生を停止します。
    def stop(*args)
    end



    # この Sound の再生を一時停止します。
    def pause(*args)
    end



    # 一時停止中の再生を再開します。
    def resume(*args)
    end



    # 音量のフェードを開始します。
    # @param [Float] targetVolume 
    # @param [Float] time 
    # @param [Lumino::SoundFadeBehavior] behavior 
    # 
    def fade_volume(*args)
    end



end

# Audio
# 
class Lumino::Audio
    # BGM を演奏する
    # @param [] filePath 
    # @param [Float] volume 
    # @param [Float] pitch 
    # @param [] fadeTime 
    # 
    def play_bgm(*args)
    end



    # BGM の演奏を停止する
    # @param [] fadeTime 
    # 
    def stop_bgm(*args)
    end



    # BGS (環境音) を演奏する
    # @param [] filePath 
    # @param [Float] volume 
    # @param [Float] pitch 
    # @param [] fadeTime 
    # 
    def play_bgs(*args)
    end



    # BGS の演奏を停止する
    # @param [] fadeTime 
    # 
    def stop_bgs(*args)
    end



    # ME (効果音楽) を演奏する
    # @param [] filePath 
    # @param [Float] volume 
    # @param [Float] pitch 
    # 
    def play_me(*args)
    end



    # ME の演奏を停止する
    def stop_me(*args)
    end



    # SE を演奏する
    # @param [] filePath 
    # @param [Float] volume 
    # @param [Float] pitch 
    # 
    def play_se(*args)
    end



    # SE を 3D 空間上で演奏する
    # @param [] filePath 
    # @param [Lumino::Vector3] position 
    # @param [Float] distance 
    # @param [Float] volume 
    # @param [Float] pitch 
    # 
    def play_se3d(*args)
    end



    # 全ての SE の演奏を停止する
    def stop_se(*args)
    end



end

# Texture2DDelegate
# 
class Lumino::Texture2DDelegate
    # 
    # @param [Lumino::Texture2DDelegate_Function] callback 
    # 
    def initialize(*args)
    end



end

# Texture2DPromise
# 
class Lumino::Texture2DPromise
    # 
    # @param [Lumino::Texture2DDelegate] callback 
    # 
    def then_with(*args)
    end



    # 
    # @param [Lumino::PromiseFailureDelegate] callback 
    # 
    def catch_with(*args)
    end



end

# Graphics
# 
class Lumino::Graphics
    # activeGraphicsAPI
    # @return [Lumino::GraphicsAPI] 
    def active_graphics_api(*args)
    end



end

# テクスチャのベースクラスです。
# 
class Lumino::Texture
end

# 2D テクスチャのクラスです。
# 
class Lumino::Texture2D
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @overload initialize(width, height)
    #   テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    #   @param [Integer] width 
    #   @param [Integer] height 
    #   
    # @overload initialize(width, height, format)
    #   テクスチャを作成します。
    #   @param [Integer] width 
    #   @param [Integer] height 
    #   @param [Lumino::TextureFormat] format 
    #   
    def initialize(*args)
    end



    # アセットからテクスチャを読み込みます。
    #   サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
    # @param [] filePath 
    # 
    # @return [Lumino::Texture2D] 
    def load(*args)
    end



    # loadEmoji
    # @param [] code 
    # 
    # @return [Lumino::Texture2D] 
    def load_emoji(*args)
    end



end

# シェーダーを表すクラスです。
# 
class Lumino::Shader
    # load
    # @param [] filePath 
    # @param [Lumino::AssetImportSettings] settings 
    # 
    # @return [Lumino::Shader] 
    def load(*args)
    end



    # 浮動小数点値を設定します。
    # @param [] parameterName 
    # @param [Float] value 
    # 
    def set_float(*args)
    end



    # ベクトル値を設定します。
    # @overload set_vector(parameterName, value)
    #   ベクトル値を設定します。
    #   @param [] parameterName 
    #   @param [Lumino::Vector3] value 
    #   
    # @overload set_vector(parameterName, value)
    #   ベクトル値を設定します。
    #   @param [] parameterName 
    #   @param [Lumino::Vector4] value 
    #   
    def set_vector(*args)
    end



    # setTexture
    # @param [] parameterName 
    # @param [Lumino::Texture] value 
    # 
    def set_texture(*args)
    end



end

# RenderView
# 
class Lumino::RenderView
end

# ### metallic-roughness テクスチャMetallic 値は B チャネル、Roughness 値は G チャネルからサンプリングされます。https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessmetallicroughnesstexture
# 
class Lumino::Material
    # init
    def initialize(*args)
    end



    # mainTexture
    # @param [Lumino::Texture] value 
    # 
    def main_texture=(*args)
    end



    # mainTexture
    # @return [Lumino::Texture] 
    def main_texture(*args)
    end



    # setColor
    # @param [Lumino::Color] value 
    # 
    def color=(*args)
    end



    # setRoughness
    # @param [Float] value 
    # 
    def roughness=(*args)
    end



    # setMetallic
    # @param [Float] value 
    # 
    def metallic=(*args)
    end



    # setEmissive
    # @param [Lumino::Color] value 
    # 
    def emissive=(*args)
    end



    # setShadingModel
    # @param [Lumino::ShadingModel] value 
    # 
    def shading_model=(*args)
    end



    # shadingModel
    # @return [Lumino::ShadingModel] 
    def shading_model(*args)
    end



    # shader
    # @param [Lumino::Shader] shader 
    # 
    def shader=(*args)
    end



    # shader
    # @return [Lumino::Shader] 
    def shader(*args)
    end



end

# MeshNode
# 
class Lumino::MeshNode
    # 可視状態を設定します。false の場合、このメッシュの描画は行われません。(default: true)
    # @param [Boolean] value 
    # 
    def visible=(*args)
    end



    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?(*args)
    end



end

# スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。
# 
class Lumino::AnimationController
    # アニメーションクリップを追加します。 (レイヤー0 へ追加されます)
    # @param [Lumino::AnimationClip] animationClip 
    # 
    # @return [Lumino::AnimationState] 
    def add_clip(*args)
    end



    # play
    # @param [Lumino::AnimationState] state 
    # @param [Float] duration 
    # 
    def play(*args)
    end



end

# MeshModel
# 
class Lumino::MeshModel
    # load
    # @param [] filePath 
    # @param [Lumino::MeshImportSettings] settings 
    # 
    # @return [Lumino::MeshModel] 
    def load(*args)
    end



    # findNode
    # @param [] name 
    # 
    # @return [Lumino::MeshNode] 
    def find_node(*args)
    end



    # findMaterial
    # @param [] name 
    # 
    # @return [Lumino::Material] 
    def find_material(*args)
    end



    # materialCount
    # @return [Integer] 
    def material_count(*args)
    end



    # load
    # @param [Integer] index 
    # 
    # @return [Lumino::Material] 
    def material(*args)
    end



    # animationController
    # @return [Lumino::AnimationController] 
    def animation_controller(*args)
    end



end

# MeshImportSettings
# 
class Lumino::MeshImportSettings
    # init
    def initialize(*args)
    end



end

# SkinnedMeshModel
# 
class Lumino::SkinnedMeshModel
end

# 
# 
class Lumino::CollisionShape
end

# 
# 
class Lumino::BoxCollisionShape
    # init
    # @overload initialize(size)
    #   init
    #   @param [Lumino::Vector3] size 
    #   
    # @overload initialize(width, height, depth)
    #   init
    #   @param [Float] width 
    #   @param [Float] height 
    #   @param [Float] depth 
    #   
    def initialize(*args)
    end



end

# 時間の経過をとおして値を評価します。アニメーションが生成する値の最小単位です。
# 
class Lumino::AnimationCurve
    # 指定した時間における値を評価します。
    # @param [Float] time 
    # 
    # @return [Float] 
    def evaluate(*args)
    end



end

# キーフレームを用いて補間を行う AnimationCurve です。
# 
class Lumino::KeyFrameAnimationCurve
    # init
    def initialize(*args)
    end



    # キーフレームを追加します。
    #   rightTangentMode は、新しく追加するキーフレームの右側の補間方法です。新しく追加するキーフレームの左側の保管方法は、そのひとつ前のキーフレームの右側の保管方法が設定されます。
    # @param [Float] time 
    # @param [Float] value 
    # @param [Lumino::TangentMode] rightTangentMode 
    # @param [Float] tangent 
    # 
    def add_key_frame(*args)
    end



end

# オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
# 
class Lumino::AnimationClip
    # load
    # @param [] filePath 
    # 
    # @return [Lumino::AnimationClip] 
    def load(*args)
    end



    # アニメーションの繰り返しの動作を取得します。(default: Loop)
    # @param [Lumino::AnimationWrapMode] value 
    # 
    def wrap_mode=(*args)
    end



    # アニメーションの繰り返しの動作を取得します。
    # @return [Lumino::AnimationWrapMode] 
    def wrap_mode(*args)
    end



    # 階層構造を持つアニメーションデータの動作モード。(default: AllowTranslationOnlyRoot)
    # @param [Lumino::HierarchicalAnimationMode] value 
    # 
    def hierarchical_animation_mode=(*args)
    end



    # 階層構造を持つアニメーションデータの動作モード。
    # @return [Lumino::HierarchicalAnimationMode] 
    def hierarchical_animation_mode(*args)
    end



end

# AnimationClip の再生状態を表すクラスです。
# 
class Lumino::AnimationState
end

# EffectResource
# 
class Lumino::EffectResource
end

# ParticleEmitterModel
# 
class Lumino::ParticleEmitterModel
    # init
    def initialize(*args)
    end



    # 同時に表示できるパーティクルの最大数を設定します。(default: 100)
    # @param [Integer] count 
    # 
    def max_particles=(*args)
    end



    # 1秒間に放出するパーティクルの数を設定します。(default: 1)
    # @param [Float] rate 
    # 
    def spawn_rate=(*args)
    end



    # パーティクルの生存時間を設定します。(default: 5.0)
    # @param [Float] time 
    # 
    def life_time=(*args)
    end



    # setupBoxShape
    # @param [Lumino::Vector3] size 
    # 
    def up_box_shape=(*args)
    end



    # (default: 1.0)
    # @param [Float] value 
    # 
    def size=(*args)
    end



    # 
    # @param [Float] value 
    # 
    def size_velocity=(*args)
    end



    # 
    # @param [Float] value 
    # 
    def size_acceleration=(*args)
    end



    # (default: 0)
    # @param [Float] value 
    # 
    def forward_velocity_min=(*args)
    end



    # (default: 0)
    # @param [Float] value 
    # 
    def forward_velocity_max=(*args)
    end



    # 進行方向に対するスケール値。通常、Z軸。ParticleGeometryDirection::ToView では Y scale (default: 1.0)
    # @param [Float] value 
    # 
    def forward_scale=(*args)
    end



    # (default: ToView)
    # @param [Lumino::ParticleGeometryDirection] value 
    # 
    def geometry_direction=(*args)
    end



    # setupSpriteModule
    # @param [Lumino::Material] material 
    # 
    def setup_sprite_module(*args)
    end



end

# ParticleModel
# 
class Lumino::ParticleModel
    # init
    def initialize(*args)
    end



    # setLoop
    # @param [Boolean] value 
    # 
    def loop=(*args)
    end



    # setLoop
    # @return [Boolean] 
    def loop?(*args)
    end



    # setLoop
    # @param [Lumino::ParticleEmitterModel] emitter 
    # 
    def add_emitter(*args)
    end



end

# 
# 
class Lumino::Component
end

# 
# 
class Lumino::VisualComponent
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    # 
    def visible=(*args)
    end



    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?(*args)
    end



end

# 
# 
class Lumino::SpriteComponent
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    # 
    def texture=(*args)
    end



end

# CollisionEventHandler
# 
class Lumino::CollisionEventHandler
    # 
    # @param [Lumino::CollisionEventHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# CharacterController
# 
class Lumino::CharacterController
    # CharacterController を作成します。
    def initialize(*args)
    end



    # walkVelocity
    # @param [Float] value 
    # 
    def walk_velocity=(*args)
    end



    # walkVelocity
    # @return [Float] 
    def walk_velocity(*args)
    end



    # setVelocity
    # @param [Lumino::Vector3] value 
    # 
    def velocity=(*args)
    end



    # velocity
    # @return [Lumino::Vector3] 
    def velocity(*args)
    end



    # キーボードやゲームパッドによる操作の有効状態を設定します。 (default: true)
    #   false を指定した場合、キャラクターの想定外の自走を防止するため、速度も 0 にリセットされます。
    # @param [Boolean] value 
    # 
    def input_control_enabled=(*args)
    end



    # マウスによるカメラ操作の有効状態を設定します。 (default: true)
    #   有効である場合、関連付けられているカメラを通じて、描画先となるビューの MouseGrab を取得します。つまり、マウスカーソルは非表示となり UI をポイントしてクリックする等の操作はできなくなります。
    # @param [Boolean] value 
    # 
    def camera_control_enabled=(*args)
    end



    # キャラクターの高さを設定します。この値はカプセルコライダーの高さや、カメラの注視点として使用されます。 (default: 2.0)
    # @param [Float] value 
    # 
    def height=(*args)
    end



    # キャラクターの高さを取得します。
    # @return [Float] 
    def height(*args)
    end



    # キャラクターとカメラの距離を設定します。(default: 5.0)
    # @param [Float] value 
    # 
    def camera_radius=(*args)
    end



    # キャラクターとカメラの距離を取得します。
    # @return [Float] 
    def camera_radius(*args)
    end



    # CollisionEnter イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::CollisionEventHandler] handler 
    # 
    def set_collision_enter(*args)
    end



    # CollisionLeave イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::CollisionEventHandler] handler 
    # 
    def set_collision_leave(*args)
    end



    # CollisionStay イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::CollisionEventHandler] handler 
    # 
    def set_collision_stay(*args)
    end



end

# 3D シーンを表すクラスです。
# 
class Lumino::World
    # オブジェクトを World に追加します。
    # @param [Lumino::WorldObject] obj 
    # 
    def add(*args)
    end



end

# ComponentList
# 
class Lumino::ComponentList
    # 
    # @return [Integer] 
    def get_length(*args)
    end



    # 
    # @param [Integer] index 
    # 
    # @return [Lumino::Component] 
    def get_item(*args)
    end



end

# World 内の全エンティティのベースクラスです。
# 
class Lumino::WorldObject
    # WorldObject を作成します。
    def initialize(*args)
    end



    # このオブジェクトの位置を設定します。
    # @overload position=(pos)
    #   このオブジェクトの位置を設定します。
    #   @param [Lumino::Vector3] pos 
    #   
    # @overload position=(x, y, z)
    #   このオブジェクトの位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def position=(*args)
    end



    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position(*args)
    end



    # このオブジェクトの回転を設定します。
    # @overload set_rotation(rot)
    #   このオブジェクトの回転を設定します。
    #   @param [Lumino::Quaternion] rot 
    #   
    # @overload set_rotation(x, y, z)
    #   このオブジェクトの回転をオイラー角から設定します(radian単位) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def set_rotation(*args)
    end



    # このオブジェクトの回転を取得します。
    # @return [Lumino::Quaternion] 
    def rotation(*args)
    end



    # このオブジェクトの拡大率を設定します。
    # @overload scale=(scale)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Lumino::Vector3] scale 
    #   
    # @overload scale=(xyz)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] xyz 
    #   
    # @overload scale=(x, y, z)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def scale=(*args)
    end



    # このオブジェクトの拡大率を取得します。
    # @return [Lumino::Vector3] 
    def scale(*args)
    end



    # このオブジェクトのローカルの中心位置を設定します。
    # @overload center_point=(value)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Lumino::Vector3] value 
    #   
    # @overload center_point=(x, y, z)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def center_point=(*args)
    end



    # このオブジェクトのローカルの中心位置を取得します。
    # @return [Lumino::Vector3] 
    def center_point(*args)
    end



    # 指定した座標を向くように、オブジェクトを回転させます。
    # @overload look_at(target)
    #   指定した座標を向くように、オブジェクトを回転させます。
    #   @param [Lumino::Vector3] target 
    #   
    # @overload look_at(x, y, z)
    #   指定した座標を向くように、オブジェクトを回転させます。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def look_at(*args)
    end



    # Component を追加します。
    # @param [Lumino::Component] component 
    # 
    def add_component(*args)
    end



    # Component を除外します。
    # @param [Lumino::Component] component 
    # 
    def remove_component(*args)
    end



    # タグを追加します。
    # @param [] tag 
    # 
    def add_tag(*args)
    end



    # タグを除外します。
    # @param [] tag 
    # 
    def remove_tag(*args)
    end



    # 指定したタグを持っているかを確認します。
    # @param [] tag 
    # 
    # @return [Boolean] 
    def has_tag(*args)
    end



    # WorldObject を破棄します。
    #   実際の削除は、現在のフレームのアップデート処理後に行われます。削除された WorldObject は、親の World, Level, WorldObject からも除外されます。
    def destroy(*args)
    end



    # 
    # @return [Lumino::ComponentList] 
    def components(*args)
    end



    # この WorldObject を指定した World へ追加します。省略した場合はデフォルトの World へ追加します。
    # @param [Lumino::World] world 
    # 
    def add_into(*args)
    end



    # 物理演算・衝突判定の前 (onCollisionStay() などはこの後)
    def on_pre_update(*args)
    end



    # フレーム更新
    # @param [Float] elapsedSeconds 
    # 
    def on_update(*args)
    end



end

# VisualComponent がアタッチされた WorldObject のプレハブクラスです。
# 
class Lumino::VisualObject
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    # 
    def visible=(*args)
    end



    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?(*args)
    end



    # 合成方法を設定します。(default: BlendMode::Normal)
    # @param [Lumino::BlendMode] value 
    # 
    def blend_mode_2=(*args)
    end



    # 不透明度を設定します。(default: 1.0)
    # @param [Float] value 
    # 
    def opacity=(*args)
    end



    # 不透明度を取得します。
    # @return [Float] 
    def opacity(*args)
    end



end

# カメラのクラスです。カメラは 3D シーンを描画する際の視点となります。
# 
class Lumino::Camera
end

# 環境ライトのオブジェクトです。
# 
class Lumino::EnvironmentLight
    # ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    # @param [Boolean] enabled 
    # 
    def enabled=(*args)
    end



    # ライトの有効状態を取得します。
    # @return [Boolean] 
    def enabled?(*args)
    end



    # ディレクショナルライトの光源色を設定します。(default: White)
    # @param [Lumino::Color] value 
    # 
    def color=(*args)
    end



    # ディレクショナルライトの光源色を取得します。
    # @return [Lumino::Color] 
    def color(*args)
    end



    # シーン全体の環境光の色を設定します。(default: 0.5, 0.5, 0.5)
    # @param [Lumino::Color] value 
    # 
    def ambient_color=(*args)
    end



    # シーン全体の環境光の色を取得します。
    # @return [Lumino::Color] 
    def ambient_color(*args)
    end



    # 空の環境光の色を取得します。
    # @return [Lumino::Color] 
    def sky_color(*args)
    end



    # 空の環境光の色を設定します。(default: Black)
    # @param [Lumino::Color] value 
    # 
    def sky_color=(*args)
    end



    # 地面の環境光の色を取得します。
    # @return [Lumino::Color] 
    def ground_color(*args)
    end



    # 地面の環境光の色を設定します。(default: Black)
    # @param [Lumino::Color] value 
    # 
    def ground_color=(*args)
    end



    # ライトの明るさを設定します。(default: 0.5)
    # @param [Float] intensity 
    # 
    def intensity=(*args)
    end



    # ライトの明るさを取得します。
    # @return [Float] 
    def intensity(*args)
    end



    # 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    # @param [Float] value 
    # 
    def shadow_effective_distance=(*args)
    end



    # 視点からの、影を生成できる距離を取得します。
    # @return [Float] 
    def shadow_effective_distance(*args)
    end



    # 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    # @param [Float] value 
    # 
    def shadow_effective_depth=(*args)
    end



    # 光源方向からの、影を生成できる距離を指定します。
    # @return [Float] 
    def shadow_effective_depth(*args)
    end



end

# ディレクショナルライトのオブジェクトです。
# 
class Lumino::DirectionalLight
    # 既定の設定でディレクショナルライトを作成します。
    # @overload initialize()
    #   既定の設定でディレクショナルライトを作成します。
    # @overload initialize(color)
    #   色を指定してディレクショナルライトを作成します。
    #   @param [Lumino::Color] color 
    #   
    def initialize(*args)
    end



    # ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    # @param [Boolean] enabled 
    # 
    def enabled=(*args)
    end



    # ライトの有効状態を取得します。
    # @return [Boolean] 
    def enabled?(*args)
    end



    # ライトカラーを設定します。(default: White)
    # @param [Lumino::Color] color 
    # 
    def color=(*args)
    end



    # ライトカラーを取得します。
    # @return [Lumino::Color] 
    def color(*args)
    end



    # ライトの明るさを設定します。(default: 0.5)
    # @param [Float] intensity 
    # 
    def intensity=(*args)
    end



    # ライトの明るさを取得します。
    # @return [Float] 
    def intensity(*args)
    end



    # 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    # @param [Float] value 
    # 
    def shadow_effective_distance=(*args)
    end



    # 視点からの、影を生成できる距離を取得します。
    # @return [Float] 
    def shadow_effective_distance(*args)
    end



    # 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    # @param [Float] value 
    # 
    def shadow_effective_depth=(*args)
    end



    # 光源方向からの、影を生成できる距離を指定します。
    # @return [Float] 
    def shadow_effective_depth(*args)
    end



end

# ポイントライトのオブジェクトです。
# 
class Lumino::PointLight
    # 既定の設定でポイントライトを作成します。
    # @overload initialize()
    #   既定の設定でポイントライトを作成します。
    # @overload initialize(color, range)
    #   色と範囲を指定してポイントライトを作成します。
    #   @param [Lumino::Color] color 
    #   @param [Float] range 
    #   
    def initialize(*args)
    end



    # ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    # @param [Boolean] enabled 
    # 
    def enabled=(*args)
    end



    # ライトの有効状態を取得します。
    # @return [Boolean] 
    def enabled?(*args)
    end



    # ライトカラーを設定します。(default: White)
    # @param [Lumino::Color] color 
    # 
    def color=(*args)
    end



    # ライトカラーを取得します。
    # @return [Lumino::Color] 
    def color(*args)
    end



    # ライトの明るさを設定します。(default: 1.0)
    # @param [Float] intensity 
    # 
    def intensity=(*args)
    end



    # ライトの明るさを取得します。
    # @return [Float] 
    def intensity(*args)
    end



    # ライトの影響範囲を設定します。(default: 10.0)
    # @param [Float] range 
    # 
    def range=(*args)
    end



    # ライトの影響範囲を取得します。
    # @return [Float] 
    def range(*args)
    end



    # ライトの減衰を設定します。(default: 1.0)
    # @param [Float] attenuation 
    # 
    def attenuation=(*args)
    end



    # ライトの減衰を取得します。
    # @return [Float] 
    def attenuation(*args)
    end



end

# スポットライトのオブジェクトです。
# 
class Lumino::SpotLight
    # 既定の設定でスポットライトを作成します。
    # @overload initialize()
    #   既定の設定でスポットライトを作成します。
    # @overload initialize(color, range, angle)
    #   色と範囲を指定してスポットライトを作成します。
    #   @param [Lumino::Color] color 
    #   @param [Float] range 
    #   @param [Float] angle 
    #   
    def initialize(*args)
    end



    # ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    # @param [Boolean] enabled 
    # 
    def enabled=(*args)
    end



    # ライトの有効状態を取得します。
    # @return [Boolean] 
    def enabled?(*args)
    end



    # ライトカラーを設定します。(default: White)
    # @param [Lumino::Color] color 
    # 
    def color=(*args)
    end



    # ライトカラーを取得します。
    # @return [Lumino::Color] 
    def color(*args)
    end



    # ライトの明るさを設定します。(default: 1.0)
    # @param [Float] intensity 
    # 
    def intensity=(*args)
    end



    # ライトの明るさを取得します。
    # @return [Float] 
    def intensity(*args)
    end



    # ライトの影響範囲を設定します。(default: 10.0)
    # @param [Float] range 
    # 
    def range=(*args)
    end



    # ライトの影響範囲を取得します。
    # @return [Float] 
    def range(*args)
    end



    # ライトの減衰を設定します。(default: 1.0)
    # @param [Float] attenuation 
    # 
    def attenuation=(*args)
    end



    # ライトの減衰を取得します。
    # @return [Float] 
    def attenuation(*args)
    end



    # スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3)
    # @param [Float] angle 
    # 
    def angle=(*args)
    end



    # スポットライトのコーン角度を取得します。(ラジアン単位)
    # @return [Float] 
    def angle(*args)
    end



    # スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0)
    # @param [Float] penumbra 
    # 
    def penumbra=(*args)
    end



    # スポットライトのコーン角度に対する減衰率を設定します。
    # @return [Float] 
    def penumbra(*args)
    end



end

# 
# 
class Lumino::TestDelegate
    # 
    # @param [Lumino::TestDelegate_Function] callback 
    # 
    def initialize(*args)
    end



end

# スプライトオブジェクトを表します。
# 
class Lumino::Sprite
    # init
    # @overload initialize()
    #   init
    # @overload initialize(texture)
    #   init
    #   @param [Lumino::Texture] texture 
    #   
    # @overload initialize(texture, width, height)
    #   init
    #   @param [Lumino::Texture] texture 
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def initialize(*args)
    end



    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] value 
    # 
    def texture=(*args)
    end



    # スプライトの大きさを設定します。
    # @overload size=(value)
    #   スプライトの大きさを設定します。
    #   @param [Lumino::Size] value 
    #   
    # @overload size=(width, height)
    #   スプライトの大きさを設定します。
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def size=(*args)
    end



    # 
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] width 
    # @param [Float] height 
    # 
    def set_source_rect(*args)
    end



    # test
    # @param [Lumino::TestDelegate] callback 
    # 
    def set_caller_test(*args)
    end



end

# カメラをマウスで操作するための Component です。
# 
class Lumino::CameraOrbitControlComponent
    # CameraOrbitControlComponent を作成します。
    def initialize(*args)
    end



end

# レイキャスティングを支援するためのクラスです。
# 
class Lumino::Raycaster
    # メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。
    # @param [Lumino::Point] point 
    # 
    # @return [Lumino::Raycaster] 
    def from_screen(*args)
    end



    # 指定した向きの平面との交差判定を行います。
    # @param [Float] normalX 
    # @param [Float] normalY 
    # @param [Float] normalZ 
    # 
    # @return [Lumino::RaycastResult] 
    def intersect_plane(*args)
    end



end

# レイキャスティングの結果を表すクラスです。
# 
class Lumino::RaycastResult
    # ワールド座標上の交差点
    # @return [Lumino::Vector3] 
    def point(*args)
    end



end

# 3D シーンを描画するための RenderView です。
# 
class Lumino::WorldRenderView
    # この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。
    # @param [Boolean] value 
    # 
    def guide_grid_enabled=(*args)
    end



    # この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。
    # @return [Boolean] 
    def guide_grid_enabled?(*args)
    end



end

# 
# 
class Lumino::ShapeObject
end

# 平面 (床・地面) のメッシュオブジェクトです。デフォルトのサイズは、各軸 10 です。
# 
class Lumino::PlaneMesh
    # 
    def initialize(*args)
    end



end

# 直方体のメッシュオブジェクトです。
# 
class Lumino::BoxMesh
    # 各軸のサイズが 1 である BoxMesh を作成します。
    # @overload initialize()
    #   各軸のサイズが 1 である BoxMesh を作成します。
    # @overload initialize(width, height, depth)
    #   
    #   @param [Float] width 
    #   @param [Float] height 
    #   @param [Float] depth 
    #   
    def initialize(*args)
    end



end

# UIElement
# 
class Lumino::MeshComponent
    # init
    def initialize(*args)
    end



    # setModel
    # @param [Lumino::MeshModel] model 
    # 
    def set_model(*args)
    end



    # 指定した名前の MeshContainer から、衝突判定用の Body を作成します。
    # @param [] meshContainerName 
    # 
    def make_collision_body(*args)
    end



end

# Collision
# 
class Lumino::Collision
    # 自分自身と衝突している他の WorldObject
    # @return [Lumino::WorldObject] 
    def world_object(*args)
    end



end

# TriggerBodyComponent
# 
class Lumino::TriggerBodyComponent
    # init
    def initialize(*args)
    end



    # addCollisionShape
    # @param [Lumino::CollisionShape] shape 
    # 
    def add_collision_shape(*args)
    end



end

# ParticleEmitter
# 
class Lumino::ParticleEmitter
    # init
    # @param [Lumino::ParticleModel] model 
    # 
    def initialize(*args)
    end



end

# ワールド 及び レベル 管理のユーティリティです。
# 
class Lumino::Scene
    # シーン背景のクリア方法を設定します。
    # @param [Lumino::SceneClearMode] value 
    # 
    def set_clear_mode(*args)
    end



    # ClearMode が SkyDome であるときに使用する、空の基本色を設定します。アルファ値は、設定した色の適用率です。
    # @param [Lumino::Color] value 
    # 
    def set_sky_color(*args)
    end



    # ClearMode が SkyDome であるときに使用する、地平の基本色を設定します。アルファ値は、設定した色の適用率です。
    # @param [Lumino::Color] value 
    # 
    def set_sky_horizon_color(*args)
    end



    # ClearMode が SkyDome であるときに使用する、雲の基本色を設定します。アルファ値は、設定した色の適用率です。
    # @param [Lumino::Color] value 
    # 
    def set_sky_cloud_color(*args)
    end



    # ClearMode が SkyDome であるときに使用する、空全体に影響する色を設定します。アルファ値は、設定した色の適用率です。
    # @param [Lumino::Color] value 
    # 
    def set_sky_overlay_color(*args)
    end



    # 指定したレベルへ遷移します。既存の全てのレベルは非アクティブ化または削除されます。
    # @param [Lumino::Level] level 
    # @param [Boolean] withEffect 
    # 
    def goto_level(*args)
    end



    # 現在のレベルを非アクティブ化し、指定したレベルへ遷移します。
    # @param [Lumino::Level] level 
    # @param [Boolean] withEffect 
    # 
    def call_level(*args)
    end



    # 現在のレベルを終了し、ひとつ前のレベルへ遷移します。
    # @param [Boolean] withEffect 
    # 
    def return_level(*args)
    end



    # 現在のアクティブなレベルを取得します。
    # @return [Lumino::Level] 
    def active_level(*args)
    end



    # レベルの遷移エフェクトを実行中であるかを確認します。
    # @return [Boolean] 
    def transition_effect_running?(*args)
    end



    # レベル遷移時のエフェクトの種類を設定します。
    # @param [Lumino::LevelTransitionEffectMode] value 
    # 
    def set_transition_effect_mode(*args)
    end



    # レベル遷移時のエフェクトの種類を取得します。
    # @return [Lumino::LevelTransitionEffectMode] 
    def transition_effect_mode(*args)
    end



    # レベルの遷移にかける時間を設定します。(Unit: 秒)
    # @param [Float] value 
    # 
    def set_transition_duration(*args)
    end



    # レベルの遷移にかける時間を取得します。(Unit: 秒)
    # @return [Float] 
    def transition_duration(*args)
    end



    # レベルの遷移モードが FadeInOut である場合に使用する色を設定します。
    # @param [Lumino::Color] value 
    # 
    def set_transition_effect_color(*args)
    end



    # レベルの遷移モードが FadeInOut である場合に使用する色を取得します。
    # @return [Lumino::Color] 
    def transition_effect_color(*args)
    end



    # レベルの遷移エフェクトで使用するマスクテクスチャを設定します。
    # @param [Lumino::Texture] value 
    # 
    def set_transition_effect_mask_texture(*args)
    end



    # レベルの遷移エフェクトで使用するマスクテクスチャを取得します。
    # @return [Lumino::Texture] 
    def transition_effect_mask_texture(*args)
    end



    # レベルの遷移エフェクトの境界のあいまいさを設定します。
    # @param [Float] value 
    # 
    def set_transition_effect_vague(*args)
    end



    # レベルの遷移エフェクトの境界のあいまいさを取得します。
    # @return [Float] 
    def transition_effect_vague(*args)
    end



    # フェードアウトエフェクトを開始します。
    def start_fade_out(*args)
    end



    # フェードインエフェクトを開始します。
    def start_fade_in(*args)
    end



    # フォグを開始するカメラからの距離を設定します。
    # @param [Float] value 
    # 
    def set_fog_start_distance(*args)
    end



    # フォグのメインカラーを設定します。
    # @param [Lumino::Color] value 
    # 
    def set_fog_color(*args)
    end



    # フォグの濃さを設定します。
    # @param [Float] value 
    # 
    def set_fog_density(*args)
    end



    # 高さフォグの濃さを設定します。
    # @param [Float] value 
    # 
    def set_fog_height_density(*args)
    end



    # フォグの高さの下限を設定します。
    # @param [Float] value 
    # 
    def set_fog_lower_height(*args)
    end



    # フォグの高さの上限を設定します。
    # @param [Float] value 
    # 
    def set_fog_upper_height(*args)
    end



    # HDR レンダリングの有無を設定します。 (default: false)
    # @param [Boolean] value 
    # 
    def set_hdr_enabled(*args)
    end



    # HDR レンダリングの有無を取得します。
    # @return [Boolean] 
    def hdr_enabled?(*args)
    end



    # 画面全体へのブレンドカラーを設定します。(default: Black)
    # @param [Lumino::Color] value 
    # 
    def set_screen_blend_color(*args)
    end



    # 画面全体へのブレンドカラーを取得します。
    # @return [Lumino::Color] 
    def screen_blend_color(*args)
    end



    # setColorTone
    # @param [Lumino::ColorTone] value 
    # 
    def set_color_tone(*args)
    end



    # colorTone
    # @return [Lumino::ColorTone] 
    def color_tone(*args)
    end



    # アンチエイリアスの有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def antialias_enabled=(*args)
    end



    # アンチエイリアスの有無を取得します。
    # @return [Boolean] 
    def antialias_enabled?(*args)
    end



    # SSR (Screen Space Reflection) の有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def ssr_enabled=(*args)
    end



    # SSR の有無を取得します。
    # @return [Boolean] 
    def ssr_enabled?(*args)
    end



    # SSAO (Screen Space Ambient Occlusion) の有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def ssao_enabled=(*args)
    end



    # SSAO の有無を取得します。
    # @return [Boolean] 
    def ssao_enabled?(*args)
    end



    # ブルームエフェクトの有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def bloom_enabled=(*args)
    end



    # ブルームエフェクトの有無を取得します。
    # @return [Boolean] 
    def bloom_enabled?(*args)
    end



    # 被写界深度の有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def dof_enabled=(*args)
    end



    # 被写界深度の有無を取得します。
    # @return [Boolean] 
    def dof_enabled?(*args)
    end



    # トーンマッピングの有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def tonemap_enabled=(*args)
    end



    # トーンマッピングの有無を取得します。
    # @return [Boolean] 
    def tonemap_enabled?(*args)
    end



    # ビネットエフェクトの有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def vignette_enabled=(*args)
    end



    # ビネットエフェクトの有無を取得します。
    # @return [Boolean] 
    def vignette_enabled?(*args)
    end



    # ガンマ補正の有無を設定します。(default: false)
    # @param [Boolean] value 
    # 
    def gamma_enabled=(*args)
    end



    # ガンマ補正の有無を取得します。
    # @return [Boolean] 
    def gamma_enabled?(*args)
    end



    # Tonemap exposure
    # @param [Float] value 
    # 
    def tonemap_exposure=(*args)
    end



    # setTonemapLinearWhite
    # @param [Float] value 
    # 
    def tonemap_linear_white=(*args)
    end



    # setTonemapShoulderStrength
    # @param [Float] value 
    # 
    def tonemap_shoulder_strength=(*args)
    end



    # setTonemapLinearStrength
    # @param [Float] value 
    # 
    def tonemap_linear_strength=(*args)
    end



    # setTonemapLinearAngle
    # @param [Float] value 
    # 
    def tonemap_linear_angle=(*args)
    end



    # setTonemapToeStrength
    # @param [Float] value 
    # 
    def tonemap_toe_strength=(*args)
    end



    # setTonemapToeNumerator
    # @param [Float] value 
    # 
    def tonemap_toe_numerator=(*args)
    end



    # setTonemapToeDenominator
    # @param [Float] value 
    # 
    def tonemap_toe_denominator=(*args)
    end



end

# シーンのベースクラスです。
# 
class Lumino::Level
    # Initialize
    def initialize(*args)
    end



    # 指定した WorldObject を、この Level 子オブジェクトとして追加します。
    # @param [Lumino::WorldObject] obj 
    # 
    def add_object(*args)
    end



    # 指定した WorldObject を、この Level 子オブジェクトか除外します。
    # @param [Lumino::WorldObject] obj 
    # 
    def remove_object(*args)
    end



    # 指定した Level を、この Level の Sub-Level として追加します。
    # @param [Lumino::Level] sublevel 
    # 
    def add_sub_level(*args)
    end



    # 指定した Level を、この Level の Sub-Level から除外します。
    # @param [Lumino::Level] sublevel 
    # 
    def remove_sub_level(*args)
    end



    # すべての Sub-Level を除外します。
    def remove_all_sub_levels(*args)
    end



    # 開始処理
    def on_start(*args)
    end



    # 終了処理
    def on_stop(*args)
    end



    # Called when deactivated.
    def on_pause(*args)
    end



    # Called when activated.
    def on_resume(*args)
    end



    # フレーム更新
    def on_update(*args)
    end



end

# 定義済みの色を取得するためのクラスです。
# 
class Lumino::UIColors
    # 色の濃さ (0~9) を指定して、 Red カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def red(*args)
    end



    # 色の濃さ (0~9) を指定して、 Pink カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def pink(*args)
    end



    # 色の濃さ (0~9) を指定して、 Purple カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def purple(*args)
    end



    # 色の濃さ (0~9) を指定して、 DeepPurple カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def deep_purple(*args)
    end



    # 色の濃さ (0~9) を指定して、 Indigo カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def indigo(*args)
    end



    # 色の濃さ (0~9) を指定して、 Blue カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def blue(*args)
    end



    # 色の濃さ (0~9) を指定して、 LightBlue カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def light_blue(*args)
    end



    # 色の濃さ (0~9) を指定して、 Cyan カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def cyan(*args)
    end



    # 色の濃さ (0~9) を指定して、 Teal カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def teal(*args)
    end



    # 色の濃さ (0~9) を指定して、 Green カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def green(*args)
    end



    # 色の濃さ (0~9) を指定して、 LightGreen カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def light_green(*args)
    end



    # 色の濃さ (0~9) を指定して、 Lime カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def lime(*args)
    end



    # 色の濃さ (0~9) を指定して、 Yellow カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def yellow(*args)
    end



    # 色の濃さ (0~9) を指定して、 Amber カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def amber(*args)
    end



    # 色の濃さ (0~9) を指定して、 Orange カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def orange(*args)
    end



    # 色の濃さ (0~9) を指定して、 DeepOrange カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def deep_orange(*args)
    end



    # 色の濃さ (0~9) を指定して、 Brown カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def brown(*args)
    end



    # 色の濃さ (0~9) を指定して、 Grey カラーパレットの色を取得します。
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def grey(*args)
    end



    # カラーインデックスと色の濃さ (0~9) を指定して、色を取得します。
    # @param [Lumino::UIColorHues] hue 
    # @param [Integer] shades 
    # 
    # @return [Lumino::Color] 
    def get(*args)
    end



end

# 
# 
class Lumino::UIEventArgs
    # イベントの発生元となった要素を取得します。
    # @return [Lumino::UIElement] 
    def sender(*args)
    end



end

# 
# 
class Lumino::UIGeneralEventHandler
    # 
    # @param [Lumino::UIGeneralEventHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# Test delegate 1.
# 
class Lumino::UIEventHandler
    # 
    # @param [Lumino::UIEventHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UILayoutElement
end

# UIElement
# 
class Lumino::UIElement
    # 要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
    # @overload set_size(size)
    #   要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
    #   @param [Lumino::Size] size 
    #   
    # @overload set_size(width, height)
    #   要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def set_size(*args)
    end



    # setWidth
    # @param [Float] value 
    # 
    def width=(*args)
    end



    # width
    # @return [Float] 
    def width(*args)
    end



    # setHeight
    # @param [Float] value 
    # 
    def height=(*args)
    end



    # height
    # @return [Float] 
    def height(*args)
    end



    # 要素の margin 値 (外側の余白) を設定します。
    # @param [Lumino::Thickness] margin 
    # 
    def margin=(*args)
    end



    # 要素の margin 値 (外側の余白) を取得します。
    # @return [Lumino::Thickness] 
    def margin(*args)
    end



    # 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。
    # @param [Lumino::Thickness] padding 
    # 
    def padding=(*args)
    end



    # 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。
    # @return [Lumino::Thickness] 
    def padding(*args)
    end



    # 要素の横方向の配置方法を設定します。
    # @param [Lumino::UIHAlignment] value 
    # 
    def h_alignment=(*args)
    end



    # 要素の横方向の配置方法を取得します。
    # @return [Lumino::UIHAlignment] 
    def h_alignment(*args)
    end



    # 要素の縦方向の配置方法を設定します。
    # @param [Lumino::UIVAlignment] value 
    # 
    def v_alignment=(*args)
    end



    # 要素の縦方向の配置方法を取得します。
    # @return [Lumino::UIVAlignment] 
    def v_alignment(*args)
    end



    # 要素の配置方法を設定します。
    # @param [Lumino::UIHAlignment] halign 
    # @param [Lumino::UIVAlignment] valign 
    # 
    def set_alignments(*args)
    end



    # このオブジェクトの位置を設定します。
    # @overload position=(pos)
    #   このオブジェクトの位置を設定します。
    #   @param [Lumino::Vector3] pos 
    #   
    # @overload position=(x, y, z)
    #   このオブジェクトの位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def position=(*args)
    end



    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position(*args)
    end



    # このオブジェクトの回転を設定します。
    # @param [Lumino::Quaternion] rot 
    # 
    def rotation=(*args)
    end



    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    # 
    def set_euler_angles(*args)
    end



    # このオブジェクトの回転を取得します。
    # @return [Lumino::Quaternion] 
    def rotation(*args)
    end



    # このオブジェクトの拡大率を設定します。
    # @overload scale=(scale)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Lumino::Vector3] scale 
    #   
    # @overload scale=(xyz)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] xyz 
    #   
    # @overload scale=(x, y)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   
    def scale=(*args)
    end



    # このオブジェクトの拡大率を取得します。
    # @return [Lumino::Vector3] 
    def scale(*args)
    end



    # このオブジェクトのローカルの中心位置を設定します。
    # @overload center_point=(value)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Lumino::Vector3] value 
    #   
    # @overload center_point=(x, y, z)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   
    def center_point=(*args)
    end



    # このオブジェクトのローカルの中心位置を取得します。
    # @return [Lumino::Vector3] 
    def center_point(*args)
    end



    # 要素の有効状態を設定します。
    # @param [Boolean] value 
    # 
    def enabled=(*args)
    end



    # 要素の有効状態を取得します。
    # @return [Boolean] 
    def enabled?(*args)
    end



    # 任意のユーザーデータを設定します。
    # @param [Lumino::Variant] value 
    # 
    def data=(*args)
    end



    # 任意のユーザーデータを取得します。
    # @return [Lumino::Variant] 
    def data(*args)
    end



    # 背景の色を設定します。
    # @param [Lumino::Color] value 
    # 
    def background_color=(*args)
    end



    # 背景の色を取得します。
    # @return [Lumino::Color] 
    def background_color(*args)
    end



    # 枠線の太さを設定します。
    # @param [Lumino::Thickness] value 
    # 
    def border_thickness=(*args)
    end



    # 枠線の太さを取得します。
    # @return [Lumino::Thickness] 
    def border_thickness(*args)
    end



    # 枠線の色を設定します。
    # @param [Lumino::Color] value 
    # 
    def border_color=(*args)
    end



    # 枠線の色を取得します。
    # @return [Lumino::Color] 
    def border_color(*args)
    end



    # 四角形の角の半径を設定します。
    # @param [Lumino::CornerRadius] value 
    # 
    def corner_radius=(*args)
    end



    # 四角形の角の半径を取得します。
    # @return [Lumino::CornerRadius] 
    def corner_radius(*args)
    end



    # 可視状態を設定します。(default: UIVisibility::Visible)
    # @param [Lumino::UIVisibility] value 
    # 
    def visibility=(*args)
    end



    # 可視状態を取得します。
    # @return [Lumino::UIVisibility] 
    def visibility(*args)
    end



    # 不透明度を設定します。(default: 1.0)
    # @param [Float] value 
    # 
    def opacity=(*args)
    end



    # 不透明度を取得します。
    # @return [Float] 
    def opacity(*args)
    end



    # Add element to container. 論理的な子要素として追加する。
    # @param [Lumino::UIElement] child 
    # 
    def add_child(*args)
    end



    # 入力フォーカスを得ることができるかどうかを設定します。(default: false)
    # @param [Boolean] value 
    # 
    def focusable=(*args)
    end



    # 入力フォーカスを得ることができるかどうかを取得します。
    # @return [Boolean] 
    def focusable?(*args)
    end



    # この UIElement を指定した UIElement へ子要素として追加します。省略した場合はデフォルトのルート UIElement へ追加します。
    # @param [Lumino::UIElement] parent 
    # 
    def add_into(*args)
    end



end

# 文字列を表示するための UI 要素です。少量の文字列表示に最適化されています。
# 
class Lumino::UIText
    # UIText を作成します。
    # @overload initialize()
    #   UIText を作成します。
    # @overload initialize(text)
    #   表示文字列を指定して、UITextBlock を作成します。
    #   @param [] text 
    #   
    def initialize(*args)
    end



    # 表示文字列を設定します。
    # @param [] value 
    # 
    def text=(*args)
    end



    # 表示文字列を取得します。
    # @return [String] 
    def text(*args)
    end



end

# UI 要素として 2D 空間上に表示されるスプライトオブジェクトを表します。
# 
class Lumino::UISprite
    # UISprite を作成します。
    # @overload initialize()
    #   UISprite を作成します。
    # @overload initialize(texture)
    #   テクスチャを指定して UISprite を作成します。
    #   @param [Lumino::Texture] texture 
    #   
    def initialize(*args)
    end



    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    # 
    def texture=(*args)
    end



    # テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
    # @overload source_rect=(rect)
    #   テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
    #   @param [Lumino::Rect] rect 
    #   
    # @overload source_rect=(x, y, width, height)
    #   
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def source_rect=(*args)
    end



    # テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
    # @return [Lumino::Rect] 
    def source_rect(*args)
    end



    # setShader
    # @param [Lumino::Shader] shader 
    # 
    def shader=(*args)
    end



end

# アイコンの UI 要素です。
# 
class Lumino::UIIcon
    # アイコン名を指定して UIIcon を作成します。
    # @overload load_font_icon(iconName)
    #   アイコン名を指定して UIIcon を作成します。
    #   @param [] iconName 
    #   
    #   @return [Lumino::UIIcon] 
    # @overload load_font_icon(iconName, size)
    #   アイコン名・サイズ を指定して UIIcon を作成します。
    #   @param [] iconName 
    #   @param [Integer] size 
    #   
    #   @return [Lumino::UIIcon] 
    # @overload load_font_icon(iconName, size, color)
    #   アイコン名・サイズ・色 (TextColor) を指定して UIIcon を作成します。
    #   @param [] iconName 
    #   @param [Integer] size 
    #   @param [Lumino::Color] color 
    #   
    #   @return [Lumino::UIIcon] 
    def load_font_icon(*args)
    end



end

# UIMessageTextArea
# 
class Lumino::UIMessageTextArea
    # init
    def initialize(*args)
    end



    # setText
    # @param [] value 
    # 
    def text=(*args)
    end



    # setTypingSpeed
    # @param [Float] value 
    # 
    def typing_speed=(*args)
    end



end

# UI モジュールに関係するユーティリティです。
# 
class Lumino::UI
    # add
    # @param [Lumino::UIElement] element 
    # 
    def add(*args)
    end



    # remove
    # @param [Lumino::UIElement] element 
    # 
    def remove(*args)
    end



end

# UILayoutPanel
# 
class Lumino::UILayoutPanel
end

# UIBoxLayout
# 
class Lumino::UIBoxLayout
    # init
    def initialize(*args)
    end



    # レイアウト方向を設定します。(default: Vertical)
    # @param [Lumino::UILayoutOrientation] orientation 
    # 
    def orientation=(*args)
    end



    # レイアウト方向を取得します。
    # @return [Lumino::UILayoutOrientation] 
    def orientation(*args)
    end



end

# UIStackLayout
# 
class Lumino::UIStackLayout
    # init
    def initialize(*args)
    end



    # (default: Vertical)
    # @param [Lumino::UILayoutOrientation] orientation 
    # 
    def orientation=(*args)
    end



    # 
    # @return [Lumino::UILayoutOrientation] 
    def orientation(*args)
    end



end

# 行と列から構成されるグリッド状に UI 要素をレイアウトします。
# 
class Lumino::UIGridLayout
    # init
    def initialize(*args)
    end



    # setColumnCount
    # @param [Integer] value 
    # 
    def column_count=(*args)
    end



    # setRow
    # @param [Lumino::UIElement] element 
    # @param [Integer] row 
    # 
    def set_row(*args)
    end



    # setColumn
    # @param [Lumino::UIElement] element 
    # @param [Integer] column 
    # 
    def set_column(*args)
    end



    # setPlacement
    # @param [Lumino::UIElement] element 
    # @param [Integer] row 
    # @param [Integer] column 
    # @param [Integer] rowSpan 
    # @param [Integer] columnSpan 
    # 
    def set_placement(*args)
    end



end

# 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
# 
class Lumino::UIControl
    # init
    def initialize(*args)
    end



    # addInlineVisual
    # @param [Lumino::UIElement] element 
    # @param [Lumino::UIInlinePlacement] layout 
    # 
    def add_inline_visual(*args)
    end



end

# UIButtonBase
# 
class Lumino::UIButtonBase
    # set text.
    # @param [] text 
    # 
    def set_text(*args)
    end



end

# UIButton
# 
class Lumino::UIButton
    # init.
    # @overload initialize()
    #   init.
    # @overload initialize(text)
    #   表示文字列を指定して UIButton を作成します。
    #   @param [] text 
    #   
    def initialize(*args)
    end



    # Clicked イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIEventHandler] handler 
    # 
    # @return [Lumino::EventConnection] 
    def connect_on_clicked(*args)
    end



end

# UIWindow
# 
class Lumino::UIWindow
    # init
    def initialize(*args)
    end



end

# UIListItemsControl 内の選択可能な項目を表します。
# 
class Lumino::UIListItem
    # Submit イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIGeneralEventHandler] handler 
    # 
    # @return [Lumino::EventConnection] 
    def connect_on_submit(*args)
    end



end

# 要素を線形に保持し、選択できるようにするシーケンスコンテナのベースクラスです。
# 
class Lumino::UIListItemsControl
    # setItemsLayoutPanel
    # @param [Lumino::UILayoutPanel] layout 
    # 
    def items_layout_panel=(*args)
    end



    # itemsLayoutPanel
    # @return [Lumino::UILayoutPanel] 
    def items_layout_panel(*args)
    end



    # UIListSubmitMode (default: Single)
    # @param [Lumino::UIListSubmitMode] value 
    # 
    def submit_mode=(*args)
    end



    # UIListSubmitMode
    # @return [Lumino::UIListSubmitMode] 
    def submit_mode(*args)
    end



    # Submit イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIGeneralEventHandler] handler 
    # 
    # @return [Lumino::EventConnection] 
    def connect_on_submit(*args)
    end



end

# UIListBox 内の選択可能な項目を表します。
# 
class Lumino::UIListBoxItem
    # init
    # @param [Lumino::UIElement] content 
    # 
    def initialize(*args)
    end



end

# UIListBox
# 
class Lumino::UIListBox
    # init
    def initialize(*args)
    end



    # UIListBoxItem を追加し、そのインスタンスを返します。
    # @param [Lumino::UIElement] content 
    # 
    # @return [Lumino::UIListBoxItem] 
    def add_item(*args)
    end



end

# ユーザー入力となる入力デバイス操作を定義するためのベースクラスです。
# 
class Lumino::InputGesture
end

# ユーザー入力となるキーボード操作の組み合わせを定義します。
# 
class Lumino::KeyGesture
    # init
    # @param [Lumino::Keys] key 
    # 
    def initialize(*args)
    end



end

# ゲームパッドやキーボードなどの入力デバイスを透過的に扱うためのインターフェイスです。
# 
class Lumino::Input
    # ボタンが現在押されているかを判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def pressed?(*args)
    end



    # ボタンが新しく押された瞬間を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def triggered?(*args)
    end



    # ボタンが離された瞬間を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def triggered_off?(*args)
    end



    # ボタンが新しく押された瞬間とリピート状態を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def repeated?(*args)
    end



    # 指定した軸のアナログ値を取得します。
    # @param [] buttonName 
    # 
    # @return [Float] 
    def get_axis_value(*args)
    end



    # ボタンのアクションマッピングを追加します。
    # @param [] buttonName 
    # @param [Lumino::InputGesture] gesture 
    # 
    def add_binding(*args)
    end



    # アクションマッピングを除外します。
    # @param [Lumino::InputGesture] gesture 
    # 
    def remove_binding(*args)
    end



    # 指定したボタンのアクションマッピングをすべて削除します。
    # @param [] buttonName 
    # 
    def clear_bindings(*args)
    end



    # 全てのアクションマッピングを削除します。
    def clear_all_bindings(*args)
    end



end

# マウスの入力を受け取るためのクラスです。
# 
class Lumino::Mouse
    # ボタンが現在押されているかを判定します。
    # @param [Lumino::MouseButtons] button 
    # 
    # @return [Boolean] 
    def pressed(*args)
    end



    # ボタンが新しく押された瞬間を判定します。
    # @param [Lumino::MouseButtons] button 
    # 
    # @return [Boolean] 
    def triggered(*args)
    end



    # ボタンが離された瞬間を判定します。
    # @param [Lumino::MouseButtons] button 
    # 
    # @return [Boolean] 
    def triggered_off(*args)
    end



    # ボタンが新しく押された瞬間とリピート状態を判定します。
    # @param [Lumino::MouseButtons] button 
    # 
    # @return [Boolean] 
    def repeated(*args)
    end



    # マウスポインタの位置を取得します。
    # @return [Lumino::Point] 
    def position(*args)
    end



end

# (実験的な機能)
# 
class Lumino::InterpreterCommand
    # コマンドの実行コードを取得します。
    # @return [String] 
    def code(*args)
    end



    # コマンドのパラメータ数を取得します。
    # @return [Integer] 
    def params_count(*args)
    end



    # コマンドのパラメータを取得します。
    # @param [Integer] index 
    # 
    # @return [String] 
    def param(*args)
    end



end

# (実験的な機能)
# 
class Lumino::InterpreterCommandList
    # InterpreterCommandList を作成します。
    def initialize(*args)
    end



    # コマンドを追加します。
    # @param [String] code 
    # 
    def add_command(*args)
    end



    # コマンドと 1 つのパラメータを追加します。
    # @param [String] code 
    # @param [String] param0 
    # 
    def add_command_1(*args)
    end



    # コマンドと 2 つのパラメータを追加します。
    # @param [String] code 
    # @param [String] param0 
    # @param [String] param1 
    # 
    def add_command_2(*args)
    end



    # コマンドと 3 つのパラメータを追加します。
    # @param [String] code 
    # @param [String] param0 
    # @param [String] param1 
    # @param [String] param2 
    # 
    def add_command_3(*args)
    end



    # コマンドと 4 つのパラメータを追加します。
    # @param [String] code 
    # @param [String] param0 
    # @param [String] param1 
    # @param [String] param2 
    # @param [String] param3 
    # 
    def add_command_4(*args)
    end



end

# InterpreterCommandDelegate
# 
class Lumino::InterpreterCommandDelegate
    # 
    # @param [Lumino::InterpreterCommandDelegate_Function] callback 
    # 
    def initialize(*args)
    end



end

# (実験的な機能) コルーチンベースのスクリプトインタプリタ。
# 
class Lumino::Interpreter
    # Interpreter を作成します。
    def initialize(*args)
    end



    # コマンドリストをクリアします。実行中のコマンドリストは強制的に破棄されます。
    def clear(*args)
    end



    # コマンドリストを設定し、実行を開始します。
    # @param [Lumino::InterpreterCommandList] commandList 
    # 
    def run(*args)
    end



    # コマンドリストの実行中であるかを確認します。
    # @return [Boolean] 
    def running?(*args)
    end



    # 
    def update(*args)
    end



    # 
    def terminate(*args)
    end



    # 
    # @param [] name 
    # @param [Lumino::InterpreterCommandDelegate] handler 
    # 
    def register_command_handler(*args)
    end



    # setWaitMode
    # @param [String] mode 
    # 
    def wait_mode=(*args)
    end



    # waitMode
    # @return [String] 
    def wait_mode(*args)
    end



    # setWaitCount
    # @param [Integer] count 
    # 
    def wait_count=(*args)
    end



    # waitCount
    # @return [Integer] 
    def wait_count(*args)
    end



    # waitMode に基づいて、実行停止中かを確認する (Wait 中なら true を返すこと)
    # @return [Boolean] 
    def on_update_wait(*args)
    end



end

# アプリケーション起動時に参照する初期化設定です。
# 
class Lumino::EngineSettings
    # メインウィンドウのタイトル文字列を設定します。
    # @param [String] title 
    # 
    def set_main_window_title(*args)
    end



    # メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
    # @param [Integer] width 
    # @param [Integer] height 
    # 
    def set_main_window_size(*args)
    end



    # メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等)
    # @param [Integer] width 
    # @param [Integer] height 
    # 
    def set_main_world_view_size(*args)
    end



    # メインウィンドウのサイズをユーザーが変更できるかどうかを指定します。(default: false)
    # @param [Boolean] value 
    # 
    def set_main_window_resizable(*args)
    end



    # アセットが保存されているディレクトリを登録します。
    # @param [String] path 
    # 
    def add_asset_directory(*args)
    end



    # アセットファイルを登録します。
    # @param [String] fileFullPath 
    # @param [String] password 
    # 
    def add_asset_archive(*args)
    end



    # フレームレートを設定します。(default: 60)
    # @param [Integer] value 
    # 
    def set_frame_rate(*args)
    end



    # デフォルトの UI テーマ名を設定します。
    # @param [String] value 
    # 
    def set_ui_theme(*args)
    end



    # デフォルトのフォントファイルを設定します。
    # @param [String] filePath 
    # 
    def set_font_file(*args)
    end



    # (default: Debug ビルドの場合true、それ以外は false)
    # @param [Boolean] enabled 
    # 
    def set_debug_tool_enabled(*args)
    end



    # デバッグモードの有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
    # @param [Boolean] enabled 
    # 
    def set_debug_mode(*args)
    end



    # デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
    # @param [String] filePath 
    # 
    def set_engine_log_file_path(*args)
    end



    # 優先的に使用する GPU の名前を指定します。
    #   現在は DirectX12 を使用する場合のみ有効で、デフォルトの GPU では動作が不安定な場合に "Microsoft Basic Render Driver" 等を試すことができます。
    # @param [String] filePath 
    # 
    def set_priority_gpu_name(*args)
    end



    # setDeveloperToolEnabled
    # @param [Boolean] enabled 
    # 
    def set_developer_tool_enabled(*args)
    end



    # ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。
    # @param [Integer] value 
    # 
    def set_user_main_window(*args)
    end



end

# アプリケーション全体にかかわる処理を行います。
# 
class Lumino::Engine
    # エンジンの初期化処理を行います。
    def initialize(*args)
    end



    # エンジンの終了処理を行います。
    def terminate(*args)
    end



    # 
    # @return [Boolean] アプリケーションの終了が要求されている場合は false を返します。
    def update(*args)
    end



    # アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
    # @return [] 
    def time(*args)
    end



    # デフォルトで作成されるメインの World です。
    # @return [Lumino::World] 
    def world(*args)
    end



    # デフォルトで作成されるメインの Camera です。
    # @return [Lumino::Camera] 
    def main_camera(*args)
    end



    # デフォルトで作成されるメインの Light です。
    # @return [Lumino::EnvironmentLight] 
    def main_light(*args)
    end



    # デフォルトで作成されるメインの RenderView です。
    # @return [Lumino::WorldRenderView] 
    def render_view(*args)
    end



end

# グローバルなアプリケーション状態を扱うための基本クラスです。
# 
class Lumino::Application
    # 
    def initialize(*args)
    end



    # エンジンの初期化処理が完了した後に呼び出されます。
    def on_init(*args)
    end



    # 毎フレーム呼び出されます。
    def on_update(*args)
    end



    # デフォルトで作成されるメインの World を取得します。
    # @return [Lumino::World] 
    def world(*args)
    end



    # Application の実行を開始します。
    def run(*args)
    end



end

# デバッグに利用できる各機能へアクセスする手段を提供します。
# 
class Lumino::Debug
    # 3D シーン上に、グリッドを表示するかどうかを設定します。
    # @param [Boolean] value 
    # 
    def guide_grid_enabled=(*args)
    end



    # 3D シーン上に、コリジョン及び物理演算の情報を表示するかどうかを設定します。
    # @param [Boolean] value 
    # 
    def physics_debug_draw_enabled=(*args)
    end



    # ウィンドウ上にデバッグ文字列を表示します。
    # @overload print(str)
    #   ウィンドウ上にデバッグ文字列を表示します。
    #   @param [] str 
    #   
    # @overload print(time, str)
    #   表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
    #   @param [Float] time 
    #   @param [] str 
    #   
    # @overload print(time, color, str)
    #   表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
    #   @param [Float] time 
    #   @param [Lumino::Color] color 
    #   @param [] str 
    #   
    def print(*args)
    end



end

# 
# 
class Lumino::ObjectSerializeHandler
    # 
    # @param [Lumino::ObjectSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::EventConnectionSerializeHandler
    # 
    # @param [Lumino::EventConnectionSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::VariantSerializeHandler
    # 
    # @param [Lumino::VariantSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ZVTestClass1SerializeHandler
    # 
    # @param [Lumino::ZVTestClass1SerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ZVTestEventArgs1SerializeHandler
    # 
    # @param [Lumino::ZVTestEventArgs1SerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::Serializer2SerializeHandler
    # 
    # @param [Lumino::Serializer2SerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AssetObjectSerializeHandler
    # 
    # @param [Lumino::AssetObjectSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AssetImportSettingsSerializeHandler
    # 
    # @param [Lumino::AssetImportSettingsSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AssetModelSerializeHandler
    # 
    # @param [Lumino::AssetModelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SoundSerializeHandler
    # 
    # @param [Lumino::SoundSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::TextureSerializeHandler
    # 
    # @param [Lumino::TextureSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::Texture2DSerializeHandler
    # 
    # @param [Lumino::Texture2DSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ShaderSerializeHandler
    # 
    # @param [Lumino::ShaderSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::RenderViewSerializeHandler
    # 
    # @param [Lumino::RenderViewSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::MaterialSerializeHandler
    # 
    # @param [Lumino::MaterialSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::MeshNodeSerializeHandler
    # 
    # @param [Lumino::MeshNodeSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AnimationControllerSerializeHandler
    # 
    # @param [Lumino::AnimationControllerSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::MeshModelSerializeHandler
    # 
    # @param [Lumino::MeshModelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::MeshImportSettingsSerializeHandler
    # 
    # @param [Lumino::MeshImportSettingsSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SkinnedMeshModelSerializeHandler
    # 
    # @param [Lumino::SkinnedMeshModelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CollisionShapeSerializeHandler
    # 
    # @param [Lumino::CollisionShapeSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::BoxCollisionShapeSerializeHandler
    # 
    # @param [Lumino::BoxCollisionShapeSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AnimationCurveSerializeHandler
    # 
    # @param [Lumino::AnimationCurveSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::KeyFrameAnimationCurveSerializeHandler
    # 
    # @param [Lumino::KeyFrameAnimationCurveSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AnimationClipSerializeHandler
    # 
    # @param [Lumino::AnimationClipSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::AnimationStateSerializeHandler
    # 
    # @param [Lumino::AnimationStateSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::EffectResourceSerializeHandler
    # 
    # @param [Lumino::EffectResourceSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ParticleEmitterModelSerializeHandler
    # 
    # @param [Lumino::ParticleEmitterModelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ParticleModelSerializeHandler
    # 
    # @param [Lumino::ParticleModelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ComponentSerializeHandler
    # 
    # @param [Lumino::ComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::VisualComponentSerializeHandler
    # 
    # @param [Lumino::VisualComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpriteComponentSerializeHandler
    # 
    # @param [Lumino::SpriteComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CharacterControllerSerializeHandler
    # 
    # @param [Lumino::CharacterControllerSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::WorldSerializeHandler
    # 
    # @param [Lumino::WorldSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ComponentListSerializeHandler
    # 
    # @param [Lumino::ComponentListSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::WorldObjectSerializeHandler
    # 
    # @param [Lumino::WorldObjectSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::WorldObjectPreUpdateHandler
    # 
    # @param [Lumino::WorldObjectPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::WorldObjectUpdateHandler
    # 
    # @param [Lumino::WorldObjectUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::VisualObjectSerializeHandler
    # 
    # @param [Lumino::VisualObjectSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::VisualObjectPreUpdateHandler
    # 
    # @param [Lumino::VisualObjectPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::VisualObjectUpdateHandler
    # 
    # @param [Lumino::VisualObjectUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CameraSerializeHandler
    # 
    # @param [Lumino::CameraSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CameraPreUpdateHandler
    # 
    # @param [Lumino::CameraPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CameraUpdateHandler
    # 
    # @param [Lumino::CameraUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::EnvironmentLightSerializeHandler
    # 
    # @param [Lumino::EnvironmentLightSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::EnvironmentLightPreUpdateHandler
    # 
    # @param [Lumino::EnvironmentLightPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::EnvironmentLightUpdateHandler
    # 
    # @param [Lumino::EnvironmentLightUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::DirectionalLightSerializeHandler
    # 
    # @param [Lumino::DirectionalLightSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::DirectionalLightPreUpdateHandler
    # 
    # @param [Lumino::DirectionalLightPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::DirectionalLightUpdateHandler
    # 
    # @param [Lumino::DirectionalLightUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PointLightSerializeHandler
    # 
    # @param [Lumino::PointLightSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PointLightPreUpdateHandler
    # 
    # @param [Lumino::PointLightPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PointLightUpdateHandler
    # 
    # @param [Lumino::PointLightUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpotLightSerializeHandler
    # 
    # @param [Lumino::SpotLightSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpotLightPreUpdateHandler
    # 
    # @param [Lumino::SpotLightPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpotLightUpdateHandler
    # 
    # @param [Lumino::SpotLightUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpriteSerializeHandler
    # 
    # @param [Lumino::SpriteSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpritePreUpdateHandler
    # 
    # @param [Lumino::SpritePreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::SpriteUpdateHandler
    # 
    # @param [Lumino::SpriteUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CameraOrbitControlComponentSerializeHandler
    # 
    # @param [Lumino::CameraOrbitControlComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::RaycasterSerializeHandler
    # 
    # @param [Lumino::RaycasterSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::RaycastResultSerializeHandler
    # 
    # @param [Lumino::RaycastResultSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::WorldRenderViewSerializeHandler
    # 
    # @param [Lumino::WorldRenderViewSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ShapeObjectSerializeHandler
    # 
    # @param [Lumino::ShapeObjectSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ShapeObjectPreUpdateHandler
    # 
    # @param [Lumino::ShapeObjectPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ShapeObjectUpdateHandler
    # 
    # @param [Lumino::ShapeObjectUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PlaneMeshSerializeHandler
    # 
    # @param [Lumino::PlaneMeshSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PlaneMeshPreUpdateHandler
    # 
    # @param [Lumino::PlaneMeshPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::PlaneMeshUpdateHandler
    # 
    # @param [Lumino::PlaneMeshUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::BoxMeshSerializeHandler
    # 
    # @param [Lumino::BoxMeshSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::BoxMeshPreUpdateHandler
    # 
    # @param [Lumino::BoxMeshPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::BoxMeshUpdateHandler
    # 
    # @param [Lumino::BoxMeshUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::MeshComponentSerializeHandler
    # 
    # @param [Lumino::MeshComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::CollisionSerializeHandler
    # 
    # @param [Lumino::CollisionSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::TriggerBodyComponentSerializeHandler
    # 
    # @param [Lumino::TriggerBodyComponentSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ParticleEmitterSerializeHandler
    # 
    # @param [Lumino::ParticleEmitterSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ParticleEmitterPreUpdateHandler
    # 
    # @param [Lumino::ParticleEmitterPreUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ParticleEmitterUpdateHandler
    # 
    # @param [Lumino::ParticleEmitterUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelSerializeHandler
    # 
    # @param [Lumino::LevelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelStartHandler
    # 
    # @param [Lumino::LevelStartHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelStopHandler
    # 
    # @param [Lumino::LevelStopHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelPauseHandler
    # 
    # @param [Lumino::LevelPauseHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelResumeHandler
    # 
    # @param [Lumino::LevelResumeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::LevelUpdateHandler
    # 
    # @param [Lumino::LevelUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIEventArgsSerializeHandler
    # 
    # @param [Lumino::UIEventArgsSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UILayoutElementSerializeHandler
    # 
    # @param [Lumino::UILayoutElementSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIElementSerializeHandler
    # 
    # @param [Lumino::UIElementSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UITextSerializeHandler
    # 
    # @param [Lumino::UITextSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UISpriteSerializeHandler
    # 
    # @param [Lumino::UISpriteSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIIconSerializeHandler
    # 
    # @param [Lumino::UIIconSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIMessageTextAreaSerializeHandler
    # 
    # @param [Lumino::UIMessageTextAreaSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UILayoutPanelSerializeHandler
    # 
    # @param [Lumino::UILayoutPanelSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIBoxLayoutSerializeHandler
    # 
    # @param [Lumino::UIBoxLayoutSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIStackLayoutSerializeHandler
    # 
    # @param [Lumino::UIStackLayoutSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIGridLayoutSerializeHandler
    # 
    # @param [Lumino::UIGridLayoutSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIControlSerializeHandler
    # 
    # @param [Lumino::UIControlSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIButtonBaseSerializeHandler
    # 
    # @param [Lumino::UIButtonBaseSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIButtonSerializeHandler
    # 
    # @param [Lumino::UIButtonSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIWindowSerializeHandler
    # 
    # @param [Lumino::UIWindowSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIListItemSerializeHandler
    # 
    # @param [Lumino::UIListItemSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIListItemsControlSerializeHandler
    # 
    # @param [Lumino::UIListItemsControlSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIListBoxItemSerializeHandler
    # 
    # @param [Lumino::UIListBoxItemSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::UIListBoxSerializeHandler
    # 
    # @param [Lumino::UIListBoxSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::InputGestureSerializeHandler
    # 
    # @param [Lumino::InputGestureSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::KeyGestureSerializeHandler
    # 
    # @param [Lumino::KeyGestureSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::InterpreterCommandSerializeHandler
    # 
    # @param [Lumino::InterpreterCommandSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::InterpreterCommandListSerializeHandler
    # 
    # @param [Lumino::InterpreterCommandListSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::InterpreterSerializeHandler
    # 
    # @param [Lumino::InterpreterSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::InterpreterUpdateWaitHandler
    # 
    # @param [Lumino::InterpreterUpdateWaitHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ApplicationSerializeHandler
    # 
    # @param [Lumino::ApplicationSerializeHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ApplicationInitHandler
    # 
    # @param [Lumino::ApplicationInitHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::ApplicationUpdateHandler
    # 
    # @param [Lumino::ApplicationUpdateHandler_Function] callback 
    # 
    def initialize(*args)
    end



end

