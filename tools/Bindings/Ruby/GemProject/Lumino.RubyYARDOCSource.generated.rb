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

# 
# 
class Lumino::Object
    # onSerialize
    # @param [Lumino::Serializer] ar 
    # 
    def on_serialize(*args)
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

# Test ZVTestEventHandler1.
# 
class Lumino::ZVTestEventHandler1
    # 
    def initialize(*args)
    end



end

# Test ZVTestEventHandler2.
# 
class Lumino::ZVTestEventHandler2
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



    # init method.
    def initialize(*args)
    end



end

# Test class.
# 
class Lumino::ZVTestEventArgs1
    # value method.
    # @return [Integer] 
    def value(*args)
    end



    # init method.
    # @overload initialize()
    #   init method.
    # @overload initialize(v)
    #   init method.
    #   @param [Integer] v 
    #   
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

# RenderView
# 
class Lumino::RenderView
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



    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @overload set_rotation(rot)
    #   このオブジェクトの回転を設定します。
    #   @param [Lumino::Quaternion] rot 
    #   
    # @overload set_rotation(x, y, z)
    #   このオブジェクトの回転をオイラー角から設定します。(radian)
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



    # スプライトの大きさを設定します。
    # @param [Float] width 
    # @param [Float] height 
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



end

# カメラのクラスです。カメラは 3D シーンを描画する際の視点となります。
# 
class Lumino::Camera
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

# 直方体のメッシュオブジェクトです。
# 
class Lumino::BoxMesh
    # 各軸のサイズが 1 である BoxMesh を作成します。
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
class Lumino::UIGeneralEventHandler
    # 
    def initialize(*args)
    end



end

# Test delegate 1.
# 
class Lumino::UIEventHandler
    # 
    def initialize(*args)
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
    # @return [Lumino::EventConnection] 
    def connect_on_clicked(*args)
    end



end

# 文字列を表示するための UI 要素です。少量の文字列表示に最適化されています。
# 
class Lumino::UITextBlock
    # UITextBlock を作成します。
    # @overload initialize()
    #   UITextBlock を作成します。
    # @overload initialize(text)
    #   表示文字列を指定して、UITextBlock を作成します。
    #   @param [] text 
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
    def pressed(*args)
    end



    # ボタンが新しく押された瞬間を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def triggered(*args)
    end



    # ボタンが離された瞬間を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def triggered_off(*args)
    end



    # ボタンが新しく押された瞬間とリピート状態を判定します。
    # @param [] buttonName 
    # 
    # @return [Boolean] 
    def repeated(*args)
    end



    # 指定した軸のアナログ値を取得します。
    # @param [] buttonName 
    # 
    # @return [Float] 
    def get_axis_value(*args)
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



    # (default: Debug ビルドの場合true、それ以外は false)
    # @param [Boolean] enabled 
    # 
    def set_debug_tool_enabled(*args)
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



    # アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
    # @return [] 
    def time(*args)
    end



    # デフォルトで作成されるメインの Camera です。
    # @return [Lumino::Camera] 
    def camera(*args)
    end



    # デフォルトで作成されるメインの RenderView です。
    # @return [Lumino::WorldRenderView] 
    def render_view(*args)
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



    # デフォルトで作成されるメインの World を取得します。
    # @return [Lumino::World] 
    def world(*args)
    end



    # 
    def initialize(*args)
    end



end

# 
# 
class Lumino::Debug
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

