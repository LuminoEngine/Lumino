
#pragma once
#include <LuminoRuntime/Common.hpp>

extern "C"
{



/**
    @brief ピクセルフォーマット
*/
typedef enum tagLnPixelFormat
{
    /**
        @brief Unknown
    */
    Unknown = 0,

    /**
        @brief 8bit アルファ値のみのフォーマット
    */
    A8 = 1,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    RGBA8 = 2,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    RGB8 = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    RGBA32F = 4,

} LnPixelFormat;

/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLnTextureFormat
{
    /**
        @brief Unknown
    */
    Unknown = 0,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    RGBA8 = 1,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    RGB8 = 2,

    /**
        @brief RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    */
    RGBA16F = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    RGBA32F = 4,

    /**
        @brief 16bit 浮動小数点フォーマット
    */
    R16F = 5,

    /**
        @brief 32bit 浮動小数点フォーマット
    */
    R32F = 6,

    /**
        @brief 32bit の符号なし整数フォーマット
    */
    R32U = 7,

} LnTextureFormat;

/**
    @brief 深度バッファのフォーマット
*/
typedef enum tagLnDepthBufferFormat
{
    /**
        @brief S8 32 ビット
    */
    D24S8 = 0,

} LnDepthBufferFormat;





//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_API LnResult LnEngine_Initialize()
;

/**
    @brief エンジンの終了処理を行います。
*/
LN_API LnResult LnEngine_Finalize()
;

/**
    @brief 
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_API LnResult LnEngine_Update(LnBool* outReturn)
;

//==============================================================================
// ln::GraphicsResource

//==============================================================================
// ln::Texture

//==============================================================================
// ln::Texture2D

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @return 作成されたテクスチャ
    @details 説明文
*/
LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height)
;

/**
    @brief テクスチャを作成します。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[in] format : ピクセルフォーマット
*/
LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height, LnTextureFormat format)
;



} // extern "C"

