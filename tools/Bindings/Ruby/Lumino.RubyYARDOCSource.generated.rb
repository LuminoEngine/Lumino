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

