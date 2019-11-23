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

# アプリケーション全体にかかわる処理を行います。
class Lumino::Engine
    # エンジンの初期化処理を行います。
    # @overload initialize()
    def initialize
    end

    # エンジンの終了処理を行います。
    # @overload finalize()
    def finalize
    end

    # 
    # @overload update()
    def update
    end

    # 。
    # @overload main_ui_view()
    def main_ui_view
    end

end

# Graphics 機能に関係するリソースのベースクラスです。
class Lumino::GraphicsResource
end

# テクスチャのベースクラスです。
class Lumino::Texture < GraphicsResource
end

# 2D テクスチャのクラスです。
class Lumino::Texture2D < Texture
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @overload initialize(width, height)
    # @overload initialize(width, height, format)
    # @overload initialize(filePath, format)
    def initialize
    end

end

# 
class Lumino::Component
end

# 
class Lumino::VisualComponent < Component
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @overload visible=(value)
    def visible=
    end

    # 可視状態を取得します。
    # @overload visible?()
    def visible?
    end

end

# 
class Lumino::SpriteComponent < VisualComponent
    # スプライトが表示するテクスチャを設定します。
    # @overload texture=(texture)
    def texture=
    end

end

# 
class Lumino::ComponentList
    # 
    # @overload get_length()
    def get_length
    end

    # 
    # @overload get_item(index)
    def get_item
    end

end

# World 内の全エンティティのベースクラスです。
class Lumino::WorldObject
    # このオブジェクトの位置を設定します。
    # @overload position=(pos)
    # @overload position=(x, y, z)
    def position=
    end

    # このオブジェクトの位置を位置を取得します。
    # @overload position()
    def position
    end

    # このオブジェクトの回転を設定します。
    # @overload rotation=(rot)
    def rotation=
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @overload set_euler_angles(x, y, z)
    def set_euler_angles
    end

    # このオブジェクトの回転を取得します。
    # @overload rotation()
    def rotation
    end

    # このオブジェクトの拡大率を設定します。
    # @overload scale=(scale)
    # @overload scale=(xyz)
    # @overload scale=(x, y, z)
    def scale=
    end

    # このオブジェクトの拡大率を取得します。
    # @overload scale()
    def scale
    end

    # このオブジェクトのローカルの中心位置を設定します。
    # @overload center_point=(value)
    # @overload center_point=(x, y, z)
    def center_point=
    end

    # このオブジェクトのローカルの中心位置を取得します。
    # @overload center_point()
    def center_point
    end

    # 
    # @overload components()
    def components
    end

    # フレーム更新
    # @overload on_update(elapsedSeconds)
    def on_update
    end

end

# VisualComponent がアタッチされた WorldObject のプレハブクラスです。
class Lumino::VisualObject < WorldObject
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @overload visible=(value)
    def visible=
    end

    # 可視状態を取得します。
    # @overload visible?()
    def visible?
    end

end

# スプライトオブジェクトを表します。
class Lumino::Sprite < VisualObject
    # スプライトが表示するテクスチャを設定します。
    # @overload texture=(texture)
    def texture=
    end

    # 
    # @overload set_source_rect(x, y, width, height)
    def set_source_rect
    end

    # init
    # @overload initialize(texture, width, height)
    def initialize
    end

end

# 
class Lumino::UIEventArgs
    # イベントの発生元となった要素を取得します。
    # @overload sender()
    def sender
    end

end

# 
class Lumino::UILayoutElement
end

# 
class Lumino::UIElement < UILayoutElement
    # このオブジェクトの位置を設定します。
    # @overload position=(pos)
    # @overload position=(x, y, z)
    def position=
    end

    # このオブジェクトの位置を位置を取得します。
    # @overload position()
    def position
    end

    # このオブジェクトの回転を設定します。
    # @overload rotation=(rot)
    def rotation=
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @overload set_euler_angles(x, y, z)
    def set_euler_angles
    end

    # このオブジェクトの回転を取得します。
    # @overload rotation()
    def rotation
    end

    # このオブジェクトの拡大率を設定します。
    # @overload scale=(scale)
    # @overload scale=(xyz)
    # @overload scale=(x, y)
    def scale=
    end

    # このオブジェクトの拡大率を取得します。
    # @overload scale()
    def scale
    end

    # このオブジェクトのローカルの中心位置を設定します。
    # @overload center_point=(value)
    # @overload center_point=(x, y, z)
    def center_point=
    end

    # このオブジェクトのローカルの中心位置を取得します。
    # @overload center_point()
    def center_point
    end

    # Add element to container.
    # @overload add_child(child)
    def add_child
    end

end

# 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
class Lumino::UIControl < UIElement
end

# 
class Lumino::UIButtonBase < UIControl
    # set text.
    # @overload set_text(text)
    def set_text
    end

end

# 
class Lumino::UIButton < UIButtonBase
    # init.
    # @overload initialize()
    def initialize
    end

    # Clicked イベントの通知を受け取るコールバックを登録します。
    # @overload connect_on_clicked(handler)
    def connect_on_clicked
    end

end

