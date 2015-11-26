
# RESULT
module Lumino::Result
    # 成功
    OK = 0
    # 不明なエラー
    UNKNOWN = -1
    # 前提条件の検証エラー
    VERIFY = -2
    # 引数が無効
    ARGUMENT = -3
    # オブジェクトの現在の状態に対して無効な呼び出しが行われた
    INVALID_OPERATION = -4
    # 未実装の機能を呼び出した
    NOT_IMPLEMENTED = -5
    # メモリ確保に失敗
    OUT_OF_MEMORY = -6
    # 値が有効な範囲に存在しない
    OUT_OF_RANGE = -7
    # 検索に使用したキーが存在しない
    KEY_NOT_FOUND = -8
    # 数値演算によるオーバーフローが発生した
    OVERFLOW = -9
    # その他のIOエラー
    IO = -10
    # ファイルにアクセスしようとして失敗した
    FILE_NOT_FOUND = -11
    # 無効なディレクトリにアクセスしようとした
    DIRECTORY_NOT_FOUND = -12
    # ファイルや文字列等の形式が不正
    INVALID_FORMAT = -13
    # ストリームの末尾を越えてアクセスしようとした
    END_OF_STREAM = -14
    # 文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった
    ENCODING = -15
    # WindowsAPI のエラー
    WIN32 = -16
    # COM のエラー
    COM = -17
end
# VARIANTTYPE
module Lumino::VariantType
    # 不明な型
    UNKNOWN = 0
    # Boolean
    BOOL = 1
    # 32bit 整数
    INT = 2
    # 単精度浮動小数
    FLOAT = 3
    # 文字列
    STRING = 4
    # 配列
    LIST = 5
    # オブジェクト
    OBJECT = 6
    # LNSizeF 構造体
    SIZE_F = 7
end
# FILEACCESSPRIORITY
module Lumino::FileAccessPriority
    # アーカイブ優先
    ARCHIVE_FIRST = 0
    # ディレクトリ優先
    DIRECTORY_FIRST = 1
    # アーカイブのみ
    ARCHIVE_ONLY = 2
end
# FILETYPE
module Lumino::FileType
    # バイナリモードで開く
    BINARY = 0
    # テキストモードで開く
    TEXT = 1
end
# FILEMODE
module Lumino::FileMode
    # 既存のファイルを開く
    OPEN = 0
    # 新しいファイルを作成する。既に存在する場合は上書きされる
    CREATE = 1
    # ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する
    APPEND = 2
end
# FILEACCESS
module Lumino::FileAccess
    # 読み取りアクセス
    READ = 0
    # 読み取り及び書き込みアクセス
    READWRITE = 1
    # 書き込みアクセス
    WRITE = 2
end
# SEEKORIGIN
module Lumino::SeekOrigin
    # ファイルの先頭
    SEEK_BEGIN = 0
    # 現在のシーク位置
    SEEK_CURRENT = 1
    # ファイルの末尾
    SEEK_END = 2
end
# ASYNCIOSTATE
module Lumino::AsyncIOState
    # 待機状態 (初期状態)
    IDLE = 0
    # 実行可能状態 (実際の処理開始を待っている)
    READY = 1
    # 実行状態 (処理中)
    PROCESSING = 2
    # 終了状態 (成功)
    COMPLETED = 3
    # 終了状態 (処理終了後・処理に失敗した)
    FAILED = 4
end
# ROTATIONORDER
module Lumino::RotationOrder
    # X → Y → Z
    XYZ = 0
    # X → Z → Y
    XZY = 1
    # Y → X → Z
    YXZ = 2
    # Y → Z → X
    YZX = 3
    # Z → X → Y
    ZXY = 4
    # Z → Y → X
    ZYX = 5
end
# KEYCODE
module Lumino::KeyCode
    # 
    KEY_UNKNOWN = 0
    # 
    KEY_A = 1
    # 
    KEY_B = 2
    # 
    KEY_C = 3
    # 
    KEY_D = 4
    # 
    KEY_E = 5
    # 
    KEY_F = 6
    # 
    KEY_G = 7
    # 
    KEY_H = 8
    # 
    KEY_I = 9
    # 
    KEY_J = 10
    # 
    KEY_K = 11
    # 
    KEY_L = 12
    # 
    KEY_M = 13
    # 
    KEY_N = 14
    # 
    KEY_O = 15
    # 
    KEY_P = 16
    # 
    KEY_Q = 17
    # 
    KEY_R = 18
    # 
    KEY_S = 19
    # 
    KEY_T = 20
    # 
    KEY_U = 21
    # 
    KEY_V = 22
    # 
    KEY_W = 23
    # 
    KEY_X = 24
    # 
    KEY_Y = 25
    # 
    KEY_Z = 26
    # 
    KEY_F10 = 46
    # 
    KEY_F11 = 47
    # 
    KEY_F12 = 48
    # 
    KEY_SPACE = 49
    # 
    KEY_ESCAPE = 50
    # 
    KEY_DOWN = 52
    # 
    KEY_LEFT = 53
    # 
    KEY_RIGHT = 54
    # 
    KEY_LSHIFT = 55
    # 
    KEY_RSHIFT = 56
    # 
    KEY_LCTRL = 57
    # 
    KEY_RCTRL = 58
    # 
    KEY_LALT = 59
    # 
    KEY_RALT = 60
    # 
    KEY_TAB = 61
    # 
    KEY_ENTER = 62
    # 
    KEY_BACKSPACE = 63
    # 
    KEY_INSERT = 64
    # 
    KEY_DELETE = 65
    # 
    KEY_PAGEUP = 66
    # 
    KEY_PAGEDOWN = 67
    # 
    KEY_HOME = 68
    # 
    KEY_END = 69
    # :	[非推奨 GLFWでは;]
    KEY_COLON = 70
    # ;	[非推奨 GLFWでは=]
    KEY_SEMICOLON = 71
    # ,
    KEY_COMMA = 72
    # .
    KEY_PERIOD = 73
    # /
    KEY_SLASH = 74
    # -
    KEY_MINUS = 75
    # \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
    KEY_BACKSLASH = 76
    # \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
    KEY_YEN = 77
    # ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
    KEY_CARET = 78
    # [
    KEY_LBRACKET = 79
    # ]
    KEY_RBRACKET = 80
    # Android '+'
    KEY_ANDROID_PLUS = 81
end
# ACCELERATORKEY
module Lumino::AcceleratorKey
    # フルスクリーン切り替えを行うアクセラレータキー
    TOGGLE_FULLSCREEN = 0
    # デバッグ用の情報を表示切替を行うアクセラレータキー
    SHOW_DEBUG_STATISTICS = 1
end
# DIRECTMUSICMODE
module Lumino::DirectMusicMode
    # DirectMusic を使用しない
    NOT_USE = 0
    # 通常
    NORMAL = 1
    # 別スレッドで初期化して、再生時に未完了の場合は待つ
    THREAD_WAIT = 2
    # 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
    THREAD_REQUEST = 3
end
# SOUNDPLAYINGMODE
module Lumino::SoundPlayingMode
    # 不明な再生方法
    UNKNOWN = 0
    # 自動選択 (デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります)
    AUTO = 1
    # オンメモリ
    ONMEMORY = 2
    # ストリーミング
    STREAMING = 3
    # SMF
    MIDI = 4
end
# SOUNDPLAYINGSTATE
module Lumino::SoundPlayingState
    # 停止中
    STOPPED = 0
    # 再生中
    PLAYING = 1
    # 一時停止中
    PAUSING = 2
end
# SOUNDFADEBEHAVIOR
module Lumino::SoundFadeBehavior
    # 継続(なにもしない)
    CONTINUE = 0
    # 停止する
    STOP = 1
    # 停止して、音量を元の値に戻す
    STOP_RESET = 2
    # 一時停止する
    PAUSE = 3
    # 一時停止して、音量を元の値に戻す
    PAUSE_RESET = 4
end
# BACKBUFFERRESIZEMODE
module Lumino::BackbufferResizeMode
    # バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
    SCALING = 0
    # バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
    SCALING_WITH_LETTER_BOX = 1
    # バックバッファをウィンドウサイズに合わせてリサイズする
    RESIZE = 2
end
# TEXTUREFORMAT
module Lumino::TextureFormat
    # 不明なフォーマット
    UNKNOWN = 0
    # 32 ビットのアルファ付きフォーマット
    A8R8G8B8 = 1
    # 32 ビットのアルファ無しフォーマット
    X8R8G8B8 = 2
    # 64 ビットの浮動小数点フォーマット
    A16B16G16R16F = 3
    # 128 ビットの浮動小数点フォーマット
    A32B32G32R32F = 4
    # 32 ビットの Z バッファフォーマット
    D24S8 = 5
end
# BLENDMODE
module Lumino::BlendMode
    # 通常 (アルファ無視)
    NORMAL = 0
    # アルファブレンド (デフォルト)
    ALPHA = 1
    # 加算
    ADD = 2
    # 加算（アルファチャンネル無視）
    ADD_ALPHA_DISABLE = 3
    # 減算
    SUB = 4
    # 減算（アルファチャンネル無視）
    SUB_ALPHA_DISABLE = 5
    # 乗算（アルファチャンネル無視）
    MUL = 6
    # スクリーン（アルファチャンネル無視）
    SCREEN = 7
    # リバース
    REVERSE = 8
end
# CULLINGMODE
module Lumino::CullingMode
    # 前面表示
    FRONT = 0
    # 裏面表示
    BACK = 1
    # 両面表示
    DOUBLE = 2
end
# TEXTALIGN
module Lumino::TextAlign
    # 左揃え
    LEFT = 0
    # 中央揃え
    CENTER = 1
    # 右揃え
    RIGHT = 2
end
# PLACEMENTTYPE
module Lumino::PlacementType
    # 通常の配置方法
    NORMAL = 0
    # 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
    BILLBOARD = 1
    # Y 軸固定
    Y_FIXED = 2
end
# FRONTAXIS
module Lumino::FrontAxis
    # X+ 方向 (右向き)
    X = 0
    # Y+ 方向 (上向き)
    Y = 1
    # Z+ 方向 (奥向き)
    Z = 2
    # X- 方向 (左向き)
    RX = 3
    # Y- 方向 (下向き)
    RY = 4
    # Z- 方向 (手前向き)
    RZ = 5
end
# MESHCREATEFLAGS
module Lumino::MeshCreateFlags
    # ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
    SOFTWARE_SKINNING = 0x01
    # 物理演算のメッシュボディとして扱うメッシュを生成する
    COLLISION_MESH = 0x02
    # マテリアルに含まれるテクスチャファイルが見つからなくても無視する
    IGNORE_TEXTURE_NOT_FOUND = 0x04
end
# CAMERATYPE
module Lumino::CameraType
    # 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
    POSITION_AND_LOOKAT = 0
    # 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
    POSITION_AND_ANGLE = 1
    # 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する
    TRANSFORM_MATRIX = 2
end
# COORD
module Lumino::Coord
    # 2D 空間
    SPACE_2D = 0
    # 3D 空間
    SPACE_3D = 1
end
# OBJECTTYPE
module Lumino::ObjectType
    # 
    SOUND = 0
end
# 初期設定を行います。
class Lumino::Config
    # デバッグ用のログファイルの出力有無を設定します。(既定値:false)
    # @overload set_application_log_enabled(enabled)
    #   @param [Bool] enabled true:出力する / false:出力しない
    def set_application_log_enabled
    end
    # 標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(既定値:false)
    # @overload set_console_enabled(enabled)
    #   @param [Bool] enabled true:割り当てる / false:割り当てない
    def set_console_enabled
    end
    # ファイルを開くときにアクセスする暗号化アーカイブを登録します。
    # @overload register_archive(filePath, password)
    #   @param [String] filePath アーカイブファイルパス
    #   @param [String] password アーカイブファイルを開くためのパスワード
    def register_archive
    end
    # ファイルへのアクセス優先順位を設定します。
    # @overload set_file_access_priority(priority)
    #   @param [Lumino::FileAccessPriority] priority 制限方法 (default:FileAccessPriority::DIRECTORY_FIRST)
    def set_file_access_priority
    end
    # ユーザー定義のウィンドウハンドルを設定します。(既定値:nil)
    # @overload set_user_window_handle(windowHandle)
    #   @param [Integer] windowHandle ユーザー定義のウィンドウハンドル
    def set_user_window_handle
    end
    # サウンドオブジェクトのキャッシュサイズの設定
    # @overload set_sound_cache_size(objectCount, memorySize)
    #   　
    #     objectCount が 0 の場合、キャッシュを使用しません。
    #     memorySize が 0 の場合、メモリ使用量に制限を設けません。
    #   @param [Integer] objectCount キャッシュできるサウンドオブジェクトの最大数 (既定値:32)
    #   @param [Integer] memorySize サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (既定値:0)
    def set_sound_cache_size
    end
    # DirectMusic の初期化方法を設定します。(既定値:DirectMusicMode::NOT_USE)
    # @overload set_direct_music_mode(mode)
    #   　
    #     DirectMusic の初期化には比較的時間がかかります。
    #     これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
    #     なお、DirectMusic を使用するためには Config.set_user_window_handle でウィンドウハンドルを設定する必要があります。
    #   @param [Lumino::DirectMusicMode] mode DirectMusic の初期化方法
    def set_direct_music_mode
    end
    # DirectMusic のリバーブエフェクトの強さを設定します。(既定値:0.75)
    # @overload set_direct_music_reverb_level(level)
    #   @param [Float] level リバーブの強さ (0.0 ～ 1.0)
    def set_direct_music_reverb_level
    end
end
# ライブラリ全体の初期化や更新等、包括的な処理を行うクラスです。
class Lumino::Application
    # ライブラリを初期化します。
    # @overload initialize()
    def initialize
    end
    # ライブラリを初期化します。音声機能のみを使用する場合に呼び出します。
    # @overload initialize_audio()
    def initialize_audio
    end
    # ライブラリの終了処理を行います。
    # @overload terminate()
    def terminate
    end
end
# バージョン情報です。
class Lumino::Version
    # メジャーバージョンを取得します。
    # @overload get_major()
    # @return [Integer] バージョン番号
    def get_major
    end
    # マイナーバージョンを取得します。
    # @overload get_minor()
    # @return [Integer] バージョン番号
    def get_minor
    end
    # リビジョンバージョンを取得します。
    # @overload get_revision()
    # @return [Integer] バージョン番号
    def get_revision
    end
    # バージョン文字列を取得します。
    # @overload get_string()
    # @return [String] 文字列
    def get_string
    end
    # 指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
    # @overload is_at_least(major, minor, revision)
    #   　
    #     指定バージョン >= コンパイルバージョン である場合、true となります。
    #   @param [Integer] major メジャーバージョン
    #   @param [Integer] minor マイナーバージョン
    #   @param [Integer] revision リビジョンバージョン
    # @return [Bool] 結果
    def is_at_least
    end
end
# エラーに関係する処理を行います。
class Lumino::Error < RefObject
    # 最後に発生したエラーのエラーコードを取得します。
    # @overload get_last_error_code()
    def get_last_error_code
    end
    # 最後に発生したエラーのエラーメッセージを取得します。
    # @overload get_last_error_message()
    # @return [String] メッセージ文字列のポインタ。
    def get_last_error_message
    end
end
# 全てのオブジェクトのベースオブジェクトです。
class Lumino::RefObject < RefObject
    # オブジェクトを解放します。
    # @overload release()
    #   　
    #     指定されたオブジェクトの参照を解放します。
    def release
    end
    # オブジェクトの参照カウントをインクリメントします。
    # @overload add_ref()
    #   　
    #     この関数は「参照カウント」という概念について十分に理解した上で使用してください。
    #     参照カウントは RefObject.add_ref でインクリメントされ、RefObject.release でデクリメントされます。
    #     Release 漏れがあるとメモリリークの原因になります。
    def add_ref
    end
    # オブジェクトの参照カウント取得します。
    # @overload get_ref_count()
    # @return [Integer] 参照カウントを格納する変数
    def get_ref_count
    end
end
# オブジェクトの幅と高さを表します。
class Lumino::Size
    # 初期化
    # @overload initialize(width, height)
    #   @param [Int] width 幅
    #   @param [Int] height 高さ
    def initialize
    end

    # 幅
    # @overload width=(v)
    #   @param [Int] v 
    def width=(v)
    end

    # 幅
    # @overload width
    #   @return [Int]
    def width
    end

    # 高さ
    # @overload height=(v)
    #   @param [Int] v 
    def height=(v)
    end

    # 高さ
    # @overload height
    #   @return [Int]
    def height
    end

end
# 2次元のベクトルのクラスです。
class Lumino::Vector2
    # 初期化
    # @overload initialize(x, y)
    #   @param [Float] x 
    #   @param [Float] y 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [Float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [Float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [Float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [Float]
    def y
    end

    # 2D ベクトルの長さを取得します。
    # @overload length()
    # @return [Float] ベクトルの長さを格納する変数
    def length
    end
    # 2D ベクトルの長さの2乗を取得します。
    # @overload square_length()
    # @return [Float] ベクトルの長さの2乗を格納する変数
    def square_length
    end
    # 2D ベクトルに値を設定します。
    # @overload set(x, y)
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    def set
    end
    # 2D ベクトルを正規化します。
    # @overload normalize(vec)
    #   2D ベクトルを正規化します。
    #   @param [Lumino::Vector2] vec 処理の基になる 2D ベクトル
    # @return [Lumino::Vector2] 演算結果を格納する 2D ベクトル
    # @overload normalize()
    #   2D ベクトルを正規化します。
    def normalize
    end
end
# 3 次元ベクトルのクラスです。
class Lumino::Vector3
    # 初期化
    # @overload initialize(x, y, z)
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [Float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [Float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [Float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [Float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [Float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [Float]
    def z
    end

    # 3Dベクトルの長さを取得します。
    # @overload length()
    # @return [Float] ベクトルの長さを格納する変数
    def length
    end
    # 3D ベクトルの長さの2乗を計算する
    # @overload square_length()
    # @return [Float] ベクトルの長さの2乗を格納する変数
    def square_length
    end
    # 3Dベクトルに値を設定します。
    # @overload set(x, y, z)
    #   3Dベクトルに値を設定します。
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    #   @param [Float] z Z 値
    # @overload set(vec2, z)
    #   2D ベクトル と Z値 を指定して、3Dベクトルに値を設定します。
    #   @param [Lumino::Vector2] vec2 2D ベクトル
    #   @param [Float] z Z 値
    def set
    end
    # 3D ベクトルを正規化します。
    # @overload normalize(vec)
    #   3D ベクトルを正規化します。
    #   @param [Lumino::Vector3] vec 処理の基になる 3D ベクトル
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    # @overload normalize()
    #   3D ベクトルを正規化します。
    def normalize
    end
    # 2 つの 3D ベクトルの内積を計算します。
    # @overload dot(vec1, vec2)
    #   @param [Lumino::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [Lumino::Vector3] vec2 処理の基になる 3D ベクトル
    # @return [Float] ベクトルの内積を格納する変数
    def dot
    end
    # 2 つの 3D ベクトルの外積を計算します。
    # @overload cross(vec1, vec2)
    #   @param [Lumino::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [Lumino::Vector3] vec2 処理の基になる 3D ベクトル
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def cross
    end
    # 指定された法線で反射させたベクトルを計算します。
    # @overload reflect(vec, normal)
    #   @param [Lumino::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [Lumino::Vector3] normal 面方向を表す法線 3D ベクトル
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def reflect
    end
    # 指定された法線と方向から滑りベクトルを計算します。
    # @overload slide(vec, normal)
    #   @param [Lumino::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [Lumino::Vector3] normal 面方向を表す法線 3D ベクトル
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def slide
    end
    # 2 つの 3D ベクトル間を線形補間します。
    # @overload lerp(vec1, vec2, t)
    #   @param [Lumino::Vector3] vec1 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
    #   @param [Lumino::Vector3] vec2 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
    #   @param [Float] t 補間値 ( 0.0 ～ 1.0 )
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def lerp
    end
    # 指定された 3D ベクトルを Catmull-Rom 補間します。
    # @overload catmull_rom(vec1, vec2, vec3, vec4, t)
    #   @param [Lumino::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [Lumino::Vector3] vec2 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
    #   @param [Lumino::Vector3] vec3 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
    #   @param [Lumino::Vector3] vec4 処理の基になる 3D ベクトル
    #   @param [Float] t 補間値 ( 0.0 ～ 1.0 )
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def catmull_rom
    end
    # 指定された行列を使用して座標変換します。
    # @overload transform(vec, mat)
    #   　
    #     ベクトルを ( x, y, z, 1 ) として座標変換します。
    #     結果の w は出力されません。<br>
    #     結果を w = 1 に射影する (x y z を w で除算) 場合は
    #     Vector3TransformCoord() を使用してください。
    #   @param [Lumino::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [Lumino::Matrix] mat 処理の基になる 行列
    # @return [Lumino::Vector4] 演算結果を格納する 3D ベクトル
    def transform
    end
    # 指定された行列を使用して座標変換し、結果を w = 1 に射影します。
    # @overload transform_coord(vec, mat)
    #   　
    #     ベクトルを ( x, y, z, 1 ) として座標変換し、
    #     結果を w = 1 に射影 (x y z を w で除算) します。
    #   @param [Lumino::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [Lumino::Matrix] mat 処理の基になる 行列
    # @return [Lumino::Vector3] 演算結果を格納する 3D ベクトル
    def transform_coord
    end
end
# 4 次元ベクトルのクラスです。
class Lumino::Vector4
    # 初期化
    # @overload initialize(x, y, z, w)
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   @param [Float] w 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [Float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [Float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [Float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [Float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [Float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [Float]
    def z
    end

    # 
    # @overload w=(v)
    #   @param [Float] v 
    def w=(v)
    end

    # 
    # @overload w
    #   @return [Float]
    def w
    end

    # 4Dベクトルに値を設定します。
    # @overload set(x, y, z, w)
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    #   @param [Float] z Z 値
    #   @param [Float] w W 値
    def set
    end
end
# 4x4 行列のクラスです。
class Lumino::Matrix
    # 初期化
    # @overload initialize(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
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
    def initialize
    end

    # 
    # @overload m11=(v)
    #   @param [Float] v 
    def m11=(v)
    end

    # 
    # @overload m11
    #   @return [Float]
    def m11
    end

    # 
    # @overload m12=(v)
    #   @param [Float] v 
    def m12=(v)
    end

    # 
    # @overload m12
    #   @return [Float]
    def m12
    end

    # 
    # @overload m13=(v)
    #   @param [Float] v 
    def m13=(v)
    end

    # 
    # @overload m13
    #   @return [Float]
    def m13
    end

    # 
    # @overload m14=(v)
    #   @param [Float] v 
    def m14=(v)
    end

    # 
    # @overload m14
    #   @return [Float]
    def m14
    end

    # 
    # @overload m21=(v)
    #   @param [Float] v 
    def m21=(v)
    end

    # 
    # @overload m21
    #   @return [Float]
    def m21
    end

    # 
    # @overload m22=(v)
    #   @param [Float] v 
    def m22=(v)
    end

    # 
    # @overload m22
    #   @return [Float]
    def m22
    end

    # 
    # @overload m23=(v)
    #   @param [Float] v 
    def m23=(v)
    end

    # 
    # @overload m23
    #   @return [Float]
    def m23
    end

    # 
    # @overload m24=(v)
    #   @param [Float] v 
    def m24=(v)
    end

    # 
    # @overload m24
    #   @return [Float]
    def m24
    end

    # 
    # @overload m31=(v)
    #   @param [Float] v 
    def m31=(v)
    end

    # 
    # @overload m31
    #   @return [Float]
    def m31
    end

    # 
    # @overload m32=(v)
    #   @param [Float] v 
    def m32=(v)
    end

    # 
    # @overload m32
    #   @return [Float]
    def m32
    end

    # 
    # @overload m33=(v)
    #   @param [Float] v 
    def m33=(v)
    end

    # 
    # @overload m33
    #   @return [Float]
    def m33
    end

    # 
    # @overload m34=(v)
    #   @param [Float] v 
    def m34=(v)
    end

    # 
    # @overload m34
    #   @return [Float]
    def m34
    end

    # 
    # @overload m41=(v)
    #   @param [Float] v 
    def m41=(v)
    end

    # 
    # @overload m41
    #   @return [Float]
    def m41
    end

    # 
    # @overload m42=(v)
    #   @param [Float] v 
    def m42=(v)
    end

    # 
    # @overload m42
    #   @return [Float]
    def m42
    end

    # 
    # @overload m43=(v)
    #   @param [Float] v 
    def m43=(v)
    end

    # 
    # @overload m43
    #   @return [Float]
    def m43
    end

    # 
    # @overload m44=(v)
    #   @param [Float] v 
    def m44=(v)
    end

    # 
    # @overload m44
    #   @return [Float]
    def m44
    end

    # 右方向を示す 3D ベクトルを取得します。
    # @overload right()
    # @return [Lumino::Vector3] 結果を格納する Vector3 変数
    def right
    end
    # 上方向を示す 3D ベクトルを取得します。
    # @overload up()
    # @return [Lumino::Vector3] 結果を格納する Vector3 変数
    def up
    end
    # 正面方向を示す 3D ベクトルを取得します。
    # @overload front()
    # @return [Lumino::Vector3] 結果を格納する Vector3 変数
    def front
    end
    # 位置を示す 3D ベクトルを取得します。
    # @overload position()
    # @return [Lumino::Vector3] 結果を格納する Vector3 変数
    def position
    end
    # 単位行列を作成します。
    # @overload identity()
    def identity
    end
    # 行列を平行移動します。
    # @overload translate(x, y, z)
    #   　
    #     与えられた引数から平行移動行列を作り、現在の行列に乗算します。
    #   @param [Float] x X 方向の移動量
    #   @param [Float] y Y 方向の移動量
    #   @param [Float] z Z 方向の移動量
    def translate
    end
    # 行列を平行移動します。
    # @overload translate_vec_3(vec)
    #   　
    #     与えられた引数から平行移動行列を作り、現在の行列に乗算します。
    #   @param [Lumino::Vector3] vec 移動量
    def translate_vec_3
    end
    # X 軸を回転軸にして行列を回転します。 (ラジアン単位)
    # @overload rotate_x(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度 ( 軸方向に対して反時計回り )
    def rotate_x
    end
    # Y 軸を回転軸にして行列を回転します。 (ラジアン単位)
    # @overload rotate_y(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度
    def rotate_y
    end
    # Z 軸を回転軸にして行列を回転します。 (ラジアン単位)
    # @overload rotate_z(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度
    def rotate_z
    end
    # 行列を回転します。 (ラジアン単位)
    # @overload rotate(xRad, yRad, zRad, rotOrder=XYZ)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] xRad X 軸を回転軸とした角度
    #   @param [Float] yRad Y 軸を回転軸とした角度
    #   @param [Float] zRad Z 軸を回転軸とした角度
    #   @param [Lumino::RotationOrder] rotOrder 回転順序の指定
    def rotate
    end
    # 行列を回転します。 (ラジアン単位)
    # @overload rotate_vec_3(vec, rotOrder=XYZ)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Lumino::Vector3] vec 各軸の回転角度
    #   @param [Lumino::RotationOrder] rotOrder 回転順序の指定
    def rotate_vec_3
    end
    # 任意の軸を回転軸にして行列を回転します。 (ラジアン単位)
    # @overload rotate_axis(axis, radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    #     与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    #   @param [Lumino::Vector3] axis 回転軸を示す Vector3
    #   @param [Float] radian 回転角度
    def rotate_axis
    end
    # クォータニオンを使って行列を回転します。
    # @overload rotate_quaternion(qua)
    #   　
    #     クォータニオンから回転行列を作り、現在の行列に乗算します。
    #   @param [Lumino::Quaternion] qua 処理の基になる Quaternion
    def rotate_quaternion
    end
    # 行列をスケーリングします。
    # @overload scale(xyz)
    #   　
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
    #   @param [Float] xyz X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)
    def scale
    end
    # 行列をスケーリングします。
    # @overload scale_xyz(x, y, z)
    #   　
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
    #   @param [Float] x X 方向の拡大率
    #   @param [Float] y Y 方向の拡大率
    #   @param [Float] z Z 方向の拡大率
    def scale_xyz
    end
    # 行列をスケーリングします。
    # @overload scale_vec_3(scale)
    #   　
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
    #   @param [Lumino::Vector3] scale 各方向の拡大率
    def scale_vec_3
    end
    # 行列を乗算します。
    # @overload multiply(mat1, mat2)
    #   @param [Lumino::Matrix] mat1 処理の基になる行列
    #   @param [Lumino::Matrix] mat2 処理の基になる行列
    # @return [Lumino::Matrix] 結果を格納する行列
    def multiply
    end
    # 逆行列を求めます。
    # @overload inverse(mat)
    #   @param [Lumino::Matrix] mat 処理の基になる行列
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def inverse
    end
    # 転置行列を求めます。
    # @overload transpose(mat)
    #   @param [Lumino::Matrix] mat 処理の基になる行列
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def transpose
    end
    # 左手座標系ビュー行列を作成します。
    # @overload view_transform_lh(pos, lookAt, upDir)
    #   @param [Lumino::Vector3] pos 視点の位置を示す Vector3
    #   @param [Lumino::Vector3] lookAt 注視点を示す Vector3
    #   @param [Lumino::Vector3] upDir 上方向を示す Vector3
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def view_transform_lh
    end
    # 右手座標系ビュー行列を作成します。
    # @overload view_transform_rh(pos, lookAt, upDir)
    #   @param [Lumino::Vector3] pos 視点の位置を示す Vector3
    #   @param [Lumino::Vector3] lookAt 注視点を示す Vector3
    #   @param [Lumino::Vector3] upDir 上方向を示す Vector3
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def view_transform_rh
    end
    # 左手座標系射影行列を作成します。
    # @overload perspective_fov_lh(fovY, aspect, nearZ, farZ)
    #   @param [Float] fovY Y 方向への視野角 (ラジアン単位)
    #   @param [Float] aspect アスペクト比
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def perspective_fov_lh
    end
    # 右手座標系射影行列作成します。
    # @overload perspective_fov_rh(fovY, aspect, nearZ, farZ)
    #   @param [Float] fovY Y 方向への視野角 (ラジアン単位)
    #   @param [Float] aspect アスペクト比
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def perspective_fov_rh
    end
    # 左手座標系正射影行列作成します。
    # @overload ortho_lh(width, height, nearZ, farZ)
    #   @param [Float] width ビューの幅
    #   @param [Float] height ビューの高さ
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def ortho_lh
    end
    # 右手座標系正射影行列作成します。
    # @overload ortho_rh(width, height, nearZ, farZ)
    #   @param [Float] width ビューの幅
    #   @param [Float] height ビューの高さ
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    # @return [Lumino::Matrix] 演算結果を格納する行列
    def ortho_rh
    end
    # 回転行列からオイラー角を計算します。
    # @overload get_euler_angles()
    # @return [Lumino::Vector3] 各軸の回転角度(ラジアン)を格納する Vector3 変数
    def get_euler_angles
    end
end
# クォータニオンのクラスです。
class Lumino::Quaternion
    # 初期化
    # @overload initialize(x, y, z, w)
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    #   @param [Float] w 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [Float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [Float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [Float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [Float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [Float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [Float]
    def z
    end

    # 
    # @overload w=(v)
    #   @param [Float] v 
    def w=(v)
    end

    # 
    # @overload w
    #   @return [Float]
    def w
    end

    # クォータニオンに値を設定します。
    # @overload set(x, y, z, w)
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    #   @param [Float] z Z 値
    #   @param [Float] w W 値
    def set
    end
    # 単位クォータニオンを作成します。
    # @overload identity()
    # @return [Lumino::Quaternion] 結果を格納する Quaternion 変数
    def identity
    end
    # 任意の軸を回転軸にして回転するクォータニオンを作成します。
    # @overload rotation_axis(axis, r)
    #   　
    #     与えられた軸が正規化されていない場合は正規化された軸を使用します。
    #   @param [Lumino::Vector3] axis 回転軸を示す Vector3 変数
    #   @param [Float] r 回転角度 (ラジアン)
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_axis
    end
    # 回転行列からクォータニオンを作成します。
    # @overload rotation_matrix(mat)
    #   @param [Lumino::Matrix] mat 処理の基になる行列
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_matrix
    end
    # ヨー、ピッチ、およびロールを指定してクォータニオンを作成します。
    # @overload rotation_yaw_pitch_roll(yaw, pitch, roll)
    #   @param [Float] yaw y 軸を中心とするヨー (ラジアン単位)
    #   @param [Float] pitch x 軸を中心とするピッチ (ラジアン単位)
    #   @param [Float] roll z 軸を中心とするロール (ラジアン単位)
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_yaw_pitch_roll
    end
    # クォータニオンを正規化します。
    # @overload normalize(qua)
    #   @param [Lumino::Quaternion] qua 処理の基になるクォータニオン
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def normalize
    end
    # クォータニオンの共役を計算します。
    # @overload conjugate(qua)
    #   @param [Lumino::Quaternion] qua 処理の基になるクォータニオン
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def conjugate
    end
    # 2 つのクォータニオンの積を計算します。
    # @overload multiply(qua1, qua2)
    #   @param [Lumino::Quaternion] qua1 処理の基になるクォータニオン
    #   @param [Lumino::Quaternion] qua2 処理の基になるクォータニオン
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def multiply
    end
    # 2 つのクォータニオンを球面線形補間します。
    # @overload slerp(qua1, qua2, t)
    #   @param [Lumino::Quaternion] qua1 処理の基になるクォータニオン
    #   @param [Lumino::Quaternion] qua2 処理の基になるクォータニオン
    #   @param [Float] t 補間値
    # @return [Lumino::Quaternion] 演算結果を格納する Quaternion 変数
    def slerp
    end
end
# ゲーム向け音声再生のユーティリティクラスです。
class Lumino::GameAudio
    # BGM を演奏します。
    # @overload play_bgm(filePath, volume=1.0, pitch=1.0, fadeTime=0.0)
    #   　
    #     BGM はひとつだけ演奏することができ、ループ再生します。
    #     新しい BGM を演奏ようとした場合、再生中の BGM は停止します。
    #     フェードイン時間を指定した場合はクロスフェードしながら音声を切り替えます。
    #   @param [String] filePath ファイルパス
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    #   @param [Float] fadeTime フェードインにかける時間 (秒)
    def play_bgm
    end
    # メモリ上の音声ファイルデータを使用して BGM を演奏します。
    # @overload play_bgm_mem(data, dataSize, volume=1.0, pitch=1.0, fadeTime=0.0)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    #   @param [Float] fadeTime フェードインにかける時間 (秒)
    def play_bgm_mem
    end
    # BGM の演奏を停止します。
    # @overload stop_bgm(fadeTime=0.0)
    #   @param [Float] fadeTime フェードアウトにかける時間 (秒)
    def stop_bgm
    end
    # BGS を演奏します。
    # @overload play_bgs(filePath, volume=1.0, pitch=1.0, fadeTime=0.0)
    #   　
    #     BGS は風や滝の音などの環境音です。
    #     BGM と同様、ひとつだけ演奏することができ、ループ再生します。
    #   @param [String] filePath ファイルパス
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    #   @param [Float] fadeTime フェードインにかける時間 (秒)
    def play_bgs
    end
    # メモリ上の音声ファイルデータから BGS を演奏します。
    # @overload play_bgs_mem(data, dataSize, volume=1.0, pitch=1.0, fadeTime=0.0)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    #   @param [Float] fadeTime フェードインにかける時間 (秒)
    def play_bgs_mem
    end
    # BGS の演奏を停止します。、
    # @overload stop_bgs(fadeTime=0.0)
    #   @param [Float] fadeTime フェードアウトにかける時間 (秒)
    def stop_bgs
    end
    # ME を演奏します。
    # @overload play_me(filePath, volume=1.0, pitch=1.0)
    #   　
    #     ME は レベルアップやアイテム取得時のファンファーレ等の効果音楽です。
    #     ME はひとつだけ演奏することができ、ループ再生せずに一度だけ演奏されます。
    #     BGM が再生中であれば一時停止し、ME 終了後に再開させます。
    #   @param [String] filePath ファイルパス
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_me
    end
    # メモリ上の音声ファイルデータから ME を演奏します。
    # @overload play_me_mem(data, dataSize, volume=1.0, pitch=1.0)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_me_mem
    end
    # ME の演奏を停止します。
    # @overload stop_me()
    def stop_me
    end
    # SE を演奏します。
    # @overload play_se(filePath, volume=1.0, pitch=1.0)
    #   　
    #     SE はゲーム中の効果音です。 SE は同時に複数再生することができ、同じ音声を演奏した場合は 重ねがけされて演奏されます。
    #   @param [String] filePath ファイルパス
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_se
    end
    # SE を演奏します。(3D サウンド)
    # @overload play_se3d(filePath, position, distance, volume=1.0, pitch=1.0)
    #   SE を演奏します。(3D サウンド)
    #   @param [String] filePath ファイルパス
    #   @param [Lumino::Vector3] position 3D 空間上の座標
    #   @param [Float] distance 減衰距離
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    # @overload play_se3d(filePath, x, y, z, distance, volume=1.0, pitch=1.0)
    #   SE を演奏します。(3D サウンド)
    #   @param [String] filePath ファイルパス
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    #   @param [Float] distance 減衰距離
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_se3d
    end
    # メモリ上の音声データから SE を演奏します。
    # @overload play_se_mem(data, dataSize, volume=1.0, pitch=1.0)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_se_mem
    end
    # メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
    # @overload play_se3d_mem(data, dataSize, position, distance, volume=1.0, pitch=1.0)
    #   メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Lumino::Vector3] position 3D 空間上の座標
    #   @param [Float] distance 減衰距離
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    # @overload play_se3d_mem(data, dataSize, x, y, z, distance, volume=1.0, pitch=1.0)
    #   メモリ上の音声ファイルデータから SE を演奏します。(3D サウンド)
    #   @param [String] data メモリ上の音声ファイルデータ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    #   @param [Float] distance 減衰距離
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def play_se3d_mem
    end
    # すべての SE の演奏を停止します。
    # @overload stop_se()
    def stop_se
    end
    # 再生中のBGMの音量を設定します。(フェードアウト中は無効)
    # @overload set_bgm_volume(volume, fadeTime=0.0)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] fadeTime フェードアウトにかける時間 (秒)
    def set_bgm_volume
    end
    # 再生中のBGSの音量を設定します。(フェードアウト中は無効)
    # @overload set_bgs_volume(volume, fadeTime=0.0)
    #   @param [Float] volume ボリューム (0.0～1.0)
    #   @param [Float] fadeTime フェードアウトにかける時間 (秒)
    def set_bgs_volume
    end
end
# 3D音声のリスナーに関する情報を操作します。
class Lumino::SoundListener
    # 3D音声のリスナーの位置を設定します。
    # @overload position=(position)
    #   @param [Lumino::Vector3] position 3D 空間上の座標
    def position=
    end
    # 3D音声のリスナーの正面方向を設定します。
    # @overload direction=(direction)
    #   3D音声のリスナーの正面方向を設定します。
    #   @param [Lumino::Vector3] direction 向き
    # @overload direction=(x, y, z)
    #   3D音声のリスナーの正面方向を設定します。
    #   @param [Float] x 向きの X 成分
    #   @param [Float] y 向きの Y 成分
    #   @param [Float] z 向きの Z 成分
    def direction=
    end
    # 3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
    # @overload up_direction=(direction)
    #   3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
    #   @param [Lumino::Vector3] direction 上方向
    # @overload up_direction=(x, y, z)
    #   3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
    #   @param [Float] x 向きの X 成分
    #   @param [Float] y 向きの Y 成分
    #   @param [Float] z 向きの Z 成分
    def up_direction=
    end
    # 3D音声のリスナーの速度を設定します。
    # @overload velocity=(velocity)
    #   3D音声のリスナーの速度を設定します。
    #   @param [Lumino::Vector3] velocity 速度
    # @overload velocity=(x, y, z)
    #   3D音声のリスナーの速度を設定します。
    #   @param [Float] x 速度の X 成分
    #   @param [Float] y 速度の Y 成分
    #   @param [Float] z 速度の Z 成分
    def velocity=
    end
    # 3D音声のリスナーの位置を設定します。
    # @overload set_position_xyz(x, y, z)
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    def set_position_xyz
    end
end
# 音声データひとつ分を表し、再生などの操作を行うクラスです。
class Lumino::Sound < RefObject
    # ファイルからサウンドオブジェクトを作成します。
    # @overload initialize(filePath)
    #   ファイルからサウンドオブジェクトを作成します。
    #   @param [String] filePath 音声ファイルのパス
    # @return [Lumino::Sound] 作成されたサウンドオブジェクトのハンドル
    # @overload initialize(data, dataSize)
    #   メモリ上の音声ファイルデータからサウンドオブジェクトを作成します。
    #   @param [String] data メモリ上の音声データ
    #   @param [Integer] dataSize データサイズ (バイト単位)
    # @return [Lumino::Sound] 作成されたサウンドオブジェクトのハンドル
    def initialize
    end
    # サウンドのボリュームを設定します。
    # @overload volume=(volume)
    #   @param [Float] volume ボリューム (0.0～1.0)
    def volume=
    end
    # サウンドのボリュームを取得します。
    # @overload volume()
    # @return [Float] ボリュームを格納する変数
    def volume
    end
    # サウンドのピッチを設定します。
    # @overload pitch=(pitch)
    #   @param [Float] pitch ピッチ (0.5～2.0)
    def pitch=
    end
    # サウンドのピッチを取得します。
    # @overload pitch()
    # @return [Float] ピッチを格納する変数
    def pitch
    end
    # サウンドのループ再生の有無を設定します。
    # @overload is_loop_enabled=(loopEnable)
    #   @param [Bool] loopEnable true = ループ再生する / false = しない
    def is_loop_enabled=
    end
    # サウンドのループ再生が有効であるかを確認します。
    # @overload is_loop_enabled?()
    # @return [Bool] ループ再生の有無状態を格納する変数
    def is_loop_enabled?
    end
    # サウンドを 3D 音源であるかを設定します。
    # @overload is_3d_enabled=(enabled)
    #   @param [Bool] enabled true = 3D音声 / false = 非3D
    def is_3d_enabled=
    end
    # サウンドが 3D 音源であるかを確認します。
    # @overload is_3d_enabled?()
    # @return [Bool] 状態 (true = 3D音声 / false = 非 3D)
    def is_3d_enabled?
    end
    # サウンド再生時の音声データの再生方法を設定します。
    # @overload playing_mode=(mode)
    #   @param [Lumino::SoundPlayingMode] mode 読み込み方法
    def playing_mode=
    end
    # サウンド再生時の音声データの再生方法を取得します。
    # @overload playing_mode()
    # @return [Lumino::SoundPlayingMode] 再生方法
    def playing_mode
    end
    # サウンドの再生状態を取得します。
    # @overload playing_state()
    # @return [Lumino::SoundPlayingState] 状態を格納する変数
    def playing_state
    end
    # サウンドの再生したサンプル数を取得します。
    # @overload played_samples()
    # @return [Integer] 再生したサンプル数を格納する変数
    def played_samples
    end
    # サウンドの音声データ全体のサンプル数を取得します。
    # @overload total_samples()
    # @return [Integer] 音声データ全体のサンプル数を格納する変数
    def total_samples
    end
    # サウンドのサンプリングレートを取得します。
    # @overload sampling_rate()
    # @return [Integer] サンプリングレートを格納する変数
    def sampling_rate
    end
    # サウンドの 3D 音源としての位置を設定します。
    # @overload emitter_position=(position)
    #   サウンドの 3D 音源としての位置を設定します。
    #   @param [Lumino::Vector3] position 3D 空間上の座標
    # @overload emitter_position=(x, y, z)
    #   サウンドの 3D 音源としての位置を設定します。
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    def emitter_position=
    end
    # サウンドの 3D 音源としての速度を設定します。
    # @overload emitter_velocity=(velocity)
    #   サウンドの 3D 音源としての速度を設定します。
    #   @param [Lumino::Vector3] velocity 速度
    # @overload emitter_velocity=(x, y, z)
    #   サウンドの 3D 音源としての速度を設定します。
    #   @param [Float] x 速度の X 成分
    #   @param [Float] y 速度の Y 成分
    #   @param [Float] z 速度の Z 成分
    def emitter_velocity=
    end
    # サウンドの 3D 音源の減衰距離 (聴こえなくなる距離) を設定します。(default:100)
    # @overload emitter_max_distance=(distance)
    #   @param [Float] distance 距離
    def emitter_max_distance=
    end
    # サウンドのループ再生の範囲を設定します。
    # @overload set_loop_range(begin, length)
    #   　
    #     begin と length に 0 を指定すると、全体をループ領域として設定します。
    #   @param [Integer] begin ループ領域の先頭位置 (サンプル数単位)
    #   @param [Integer] length ループ領域長さ (サンプル数単位)
    def set_loop_range
    end
    # サウンドを再生します。
    # @overload play()
    def play
    end
    # サウンドの再生を停止します。
    # @overload stop()
    def stop
    end
    # サウンドの再生を一時停止します。
    # @overload pause()
    def pause
    end
    # サウンドの再生を一時停止します。
    # @overload resume()
    def resume
    end
    # サウンド音量のフェード操作を行います。
    # @overload fade_volume(targetVolume, time, behavior)
    #   @param [Float] targetVolume 変更先の音量
    #   @param [Float] time フェードにかける時間 (秒)
    #   @param [Lumino::SoundFadeBehavior] behavior フェード完了後の動作の指定
    def fade_volume
    end
end
# テクスチャを操作するためのベースクラスです。
class Lumino::Texture < RefObject
    # テクスチャのサイズを取得します。
    # @overload size()
    # @return [Lumino::Size] サイズを格納する変数のアドレス
    def size
    end
end
# 2Dテクスチャを操作するためのクラスです。
class Lumino::Texture2D < Texture
    # ファイルから2Dテクスチャオブジェクトを作成します。
    # @overload initialize(filePath)
    #   　
    #     全てのプラットフォームでサポートされているファイルフォーマットは .png です。
    #     グラフィックスAPI に DirectX9 を使用している場合は MSDN の D3DXCreateTextureFromFileEx を参照してください。
    #   @param [String] filePath 画像ファイルのパス
    # @return [Lumino::Texture2D] 作成された2Dテクスチャオブジェクトのハンドル
    def initialize
    end
end


