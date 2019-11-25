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

end

# Graphics 機能に関係するリソースのベースクラスです。
# 
class Lumino::GraphicsResource
end

# テクスチャのベースクラスです。
# 
class Lumino::Texture < GraphicsResource
end

# 2D テクスチャのクラスです。
# 
class Lumino::Texture2D < Texture
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @overload initialize(width, height)
    #   テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    #   @param [Integer] width 
    #   @param [Integer] height 
    # @overload initialize(width, height, format)
    #   テクスチャを作成します。
    #   @param [Integer] width 
    #   @param [Integer] height 
    #   @param [Lumino::TextureFormat] format 
    # @overload initialize(filePath, format)
    #   ローカルのファイルを読み込み、テクスチャを作成します。
    #     このメソッドは TextureImporter のユーティリティです。
    #   @param [String] filePath 
    #   @param [Lumino::TextureFormat] format 
    def initialize(*args)
    end

end

# 
# 
class Lumino::Component
end

# 
# 
class Lumino::VisualComponent < Component
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    def visible=(*args)
    end

    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?(*args)
    end

end

# 
# 
class Lumino::SpriteComponent < VisualComponent
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    def texture=(*args)
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
    # @overload position=(x, y, z)
    #   このオブジェクトの位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    def position=(*args)
    end

    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position(*args)
    end

    # このオブジェクトの回転を設定します。
    # @param [Lumino::Quaternion] rot 
    def rotation=(*args)
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
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
    # @overload scale=(xyz)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] xyz 
    # @overload scale=(x, y, z)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
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
    # @overload center_point=(x, y, z)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
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
    def on_update(*args)
    end

end

# VisualComponent がアタッチされた WorldObject のプレハブクラスです。
# 
class Lumino::VisualObject < WorldObject
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    def visible=(*args)
    end

    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?(*args)
    end

end

# スプライトオブジェクトを表します。
# 
class Lumino::Sprite < VisualObject
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    def texture=(*args)
    end

    # 
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] width 
    # @param [Float] height 
    def set_source_rect(*args)
    end

    # init
    # @param [Lumino::Texture] texture 
    # @param [Float] width 
    # @param [Float] height 
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
class Lumino::UIElement < UILayoutElement
    # このオブジェクトの位置を設定します。
    # @overload position=(pos)
    #   このオブジェクトの位置を設定します。
    #   @param [Lumino::Vector3] pos 
    # @overload position=(x, y, z)
    #   このオブジェクトの位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    def position=(*args)
    end

    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position(*args)
    end

    # このオブジェクトの回転を設定します。
    # @param [Lumino::Quaternion] rot 
    def rotation=(*args)
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
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
    # @overload scale=(xyz)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] xyz 
    # @overload scale=(x, y)
    #   このオブジェクトの拡大率を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
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
    # @overload center_point=(x, y, z)
    #   このオブジェクトのローカルの中心位置を設定します。
    #   @param [Float] x 
    #   @param [Float] y 
    #   @param [Float] z 
    def center_point=(*args)
    end

    # このオブジェクトのローカルの中心位置を取得します。
    # @return [Lumino::Vector3] 
    def center_point(*args)
    end

    # Add element to container.
    # @param [Lumino::UIElement] child 
    def add_child(*args)
    end

end

# 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
# 
class Lumino::UIControl < UIElement
end

# 
# 
class Lumino::UIButtonBase < UIControl
    # set text.
    # @param [String] text 
    def set_text(*args)
    end

end

# 
# 
class Lumino::UIButton < UIButtonBase
    # init.
    def initialize(*args)
    end

    # Clicked イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIEventHandler] handler 
    # @return [] 
    def connect_on_clicked(*args)
    end

end

