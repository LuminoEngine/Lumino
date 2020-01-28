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
    # 32bit の符号なし整数フォーマット
    R32U = 7
end

# DepthBufferFormat
module Lumino::DepthBufferFormat
    # S8 32 ビット
    D24S8 = 0
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

# 
# 
class Lumino::Object
    # onSerialize
    # @param [Lumino::Serializer] ar 
    # 
    def on_serialize(*args)
    end



end

# Promise failure.
# 
class Lumino::PromiseFailureDelegate
    # 
    def initialize(*args)
    end



end

# Test delegate 1.
# 
class Lumino::ZVTestDelegate1
    # 
    def initialize(*args)
    end



end

# Test delegate 2.
# 
class Lumino::ZVTestDelegate2
    # 
    def initialize(*args)
    end



end

# Test delegate 3.
# 
class Lumino::ZVTestDelegate3
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



    # init method.
    def initialize(*args)
    end



end

# 
# 
class Lumino::Serializer
    # write
    # @param [] name 
    # @param [Boolean] value 
    # 
    def write_bool(*args)
    end



    # write
    # @param [] name 
    # @param [Integer] value 
    # 
    def write_int(*args)
    end



    # write
    # @param [] name 
    # @param [Float] value 
    # 
    def write_float(*args)
    end



    # write
    # @param [] name 
    # @param [] value 
    # 
    def write_string(*args)
    end



    # write
    # @param [] name 
    # @param [Lumino::Object] value 
    # 
    def write_object(*args)
    end



    # read
    # @param [] name 
    # 
    # @return [Boolean] 
    def read_bool(*args)
    end



    # read
    # @param [] name 
    # 
    # @return [Integer] 
    def read_int(*args)
    end



    # read
    # @param [] name 
    # 
    # @return [Float] 
    def read_float(*args)
    end



    # read
    # @param [] name 
    # 
    # @return [String] 
    def read_string(*args)
    end



    # read
    # @param [] name 
    # 
    # @return [Lumino::Object] 
    def read_object(*args)
    end



    # serialize
    # @param [Lumino::Object] value 
    # @param [String] basePath 
    # 
    # @return [String] 
    def serialize(*args)
    end



    # serialize
    # @param [String] str 
    # @param [String] basePath 
    # 
    # @return [Lumino::Object] 
    def deserialize(*args)
    end



end

# 
# 
class Lumino::AssetModel
    # target
    # @return [Lumino::Object] 
    def target(*args)
    end



    # init
    # @param [Lumino::Object] target 
    # 
    def initialize(*args)
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



end

# アプリケーション起動時に参照する初期化設定です。
# 
class Lumino::EngineSettings
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



    # メインウィンドウのタイトル文字列を設定します。
    # @param [String] title 
    # 
    def set_main_window_title(*args)
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



    # デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
    # @param [Boolean] enabled 
    # 
    def set_engine_log_enabled(*args)
    end



    # デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
    # @param [String] filePath 
    # 
    def set_engine_log_file_path(*args)
    end



end

# アプリケーション全体にかかわる処理を行います。
# 
class Lumino::Engine
    # エンジンの初期化処理を行います。
    def initialize(*args)
    end



    # エンジンの終了処理を行います。
    def finalize(*args)
    end



    # 
    # @return [Boolean] アプリケーションの終了が要求されている場合は false を返します。
    def update(*args)
    end



    # 。
    # @return [Lumino::UIControl] 
    def main_ui_view(*args)
    end



    # デフォルトで作成されるメインの World です。
    # @return [Lumino::World] 
    def world(*args)
    end



end

# グローバルなアプリケーション状態を扱うための基本クラスです。
# 
class Lumino::Application
    # エンジンの初期化処理が完了した後に呼び出されます。
    def on_init(*args)
    end



    # 毎フレーム呼び出されます。
    def on_update(*args)
    end



    # 
    def initialize(*args)
    end



end

# Graphics 機能に関係するリソースのベースクラスです。
# 
class Lumino::GraphicsResource
end

# テクスチャのベースクラスです。
# 
class Lumino::Texture
end

# 2D テクスチャのクラスです。
# 
class Lumino::Texture2D
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
    # @overload initialize(filePath, format)
    #   ローカルのファイルを読み込み、テクスチャを作成します。
    #     このメソッドは TextureImporter のユーティリティです。
    #   @param [] filePath 
    #   @param [Lumino::TextureFormat] format 
    #   
    def initialize(*args)
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

# 
# 
class Lumino::World
    # オブジェクトを World に追加します。
    # @param [Lumino::WorldObject] obj 
    # 
    def add(*args)
    end



end

# 
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



    # 
    # @return [Lumino::ComponentList] 
    def components(*args)
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



end

# 
# 
class Lumino::TestDelegate
    # 
    def initialize(*args)
    end



end

# スプライトオブジェクトを表します。
# 
class Lumino::Sprite
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] value 
    # 
    def texture=(*args)
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



    # init
    # @overload initialize()
    #   init
    # @overload initialize(texture, width, height)
    #   init
    #   @param [Lumino::Texture] texture 
    #   @param [Float] width 
    #   @param [Float] height 
    #   
    def initialize(*args)
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
class Lumino::UILayoutElement
end

# 
# 
class Lumino::UIElement
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



    # Add element to container.
    # @param [Lumino::UIElement] child 
    # 
    def add_child(*args)
    end



end

# 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
# 
class Lumino::UIControl
end

# 
# 
class Lumino::UIButtonBase
    # set text.
    # @param [] text 
    # 
    def set_text(*args)
    end



end

# 
# 
class Lumino::UIButton
    # init.
    def initialize(*args)
    end



    # Clicked イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIEventHandler] handler 
    # 
    # @return [] 
    def connect_on_clicked(*args)
    end



end

