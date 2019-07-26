// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"

namespace ln {

/** ブレンディングの演算方法 */
enum class BlendOp : uint8_t
{
    /** 転送元に転送先を加算する。*/
    Add,

    /** 転送元から転送先を減算する。*/
    Subtract,

    /** 転送先から転送元を減算する。*/
    ReverseSubtract,

    /** 転送先と転送元から小さいほうを使用する。*/
    Min,

    /** 転送先と転送元から大きいほうを使用する。*/
    Max,
};

/** ブレンディングの係数 */
enum class BlendFactor : uint8_t
{
    /** ブレンディング係数は、(0, 0, 0, 0) */
    Zero,

    /** ブレンディング係数は、(1, 1, 1, 1) */
    One,

    /** ブレンディング係数は、(Rs, Gs, Bs, As) */
    SourceColor,

    /** ブレンディング係数は、(1-Rs, 1-Gs, 1-Bs, 1-As) */
    InverseSourceColor,

    /** ブレンディング係数は、(As, As, As, As) */
    SourceAlpha,

    /** ブレンディング係数は、(1-As, 1-As, 1-As, 1-As) */
    InverseSourceAlpha,

    /** ブレンディング係数は、(Rd, Gd, Bd, Ad) */
    DestinationColor,

    /** ブレンディング係数は、(1-Rd, 1-Gd, 1-Bd, 1-Ad) */
    InverseDestinationColor,

    /** ブレンディング係数は、(Ad, Ad, Ad, Ad) */
    DestinationAlpha,

    /** ブレンディング係数は、(1-Ad, 1-Ad, 1-Ad, 1-Ad) */
    InverseDestinationAlpha,
};

/** 塗りつぶし方法 */
enum class FillMode : uint8_t
{
    /** 面を塗りつぶす */
    Solid,

    /** ワイヤーフレーム */
    Wireframe,
};

/** カリング方法 */
enum class CullMode : uint8_t
{
    /** 両面を描画します。 */
    None,

    /** 前向きの面を描画しません。 */
    Front,

    /** 後ろ向きの面を描画しません。 */
    Back,
};

/** 比較関数 */
enum class ComparisonFunc : uint8_t
{
    /** 常に失敗します。 */
    Never,

    /** (新しいピクセル値 < 現在のピクセル値) 新しいピクセル値が、現在のピクセル値未満の場合に、新しいピクセル値を採用します。 */
    Less,

    /** (新しいピクセル値 <= 現在のピクセル値) 新しいピクセル値が、現在のピクセル値以下の場合に、新しいピクセル値を採用します。 */
    LessEqual,

    /** (新しいピクセル値 > 現在のピクセル値) 新しいピクセル値が、現在のピクセル値を超える場合に、新しいピクセル値を採用します。 */
    Greater,

    /** (新しいピクセル値 >= 現在のピクセル値) 新しいピクセル値が、現在のピクセル値以上の場合に、新しいピクセル値を採用します。 */
    GreaterEqual,

    /** (新しいピクセル値 == 現在のピクセル値) 新しいピクセル値が、現在のピクセル値と等しい場合に、新しいピクセル値を採用します。 */
    Equal,

    /** (新しいピクセル値 != 現在のピクセル値) 新しいピクセル値が、現在のピクセル値と等しくない場合に、新しいピクセル値を採用します。 */
    NotEqual,

    /** 常に成功します。 */
    Always,
};

/** ステンシル処理方法 */
enum class StencilOp : uint8_t
{
    /** 既存のステンシル データを保持します。(何もしません) */
    Keep,

    /** ステンシルデータをステンシル参照値に設定します。 */
    Replace,
};

/** レンダー ターゲットのブレンディングステート */
struct RenderTargetBlendDesc
{
    /** ブレンディングの有無 (default:false) */
    bool blendEnable : 1;

    /** 入力元の RGB に対する係数 (default: One) */
    BlendFactor sourceBlend : 4;

    /** 出力先の RGB に対する係数 (default: Zero) */
    BlendFactor destinationBlend : 4;

    /** RGB のブレンディング操作 (default: Add) */
    BlendOp blendOp;

    /** 入力元のアルファ値に対する係数 (default: One) */
    BlendFactor sourceBlendAlpha : 4;

    /** 出力先のアルファ値に対する係数 (default: Zero) */
    BlendFactor destinationBlendAlpha : 4;

    /** アルファ値のブレンディング操作 (default: Add) */
    BlendOp blendOpAlpha : 4;

    RenderTargetBlendDesc();
	static bool equals(const RenderTargetBlendDesc& lhs, const RenderTargetBlendDesc& rhs);
};

/** ブレンディングステート */
struct BlendStateDesc
{
    static const int MaxRenderTargets = 4;

    /** レンダーターゲットで独立したブレンディングを有効にするには、true に設定します。(default:false) */
    bool independentBlendEnable;

    /** レンダーターゲットごとのブレンドステートの配列です。 */
    RenderTargetBlendDesc renderTargets[MaxRenderTargets];

    BlendStateDesc();
	static bool equals(const BlendStateDesc& lhs, const BlendStateDesc& rhs);
};

/** ラスタライザステート */
struct RasterizerStateDesc
{
    /** 塗りつぶし方法 (default:Solid) */
    FillMode fillMode : 4;

    /** カリング方法 (default:Back) */
    CullMode cullMode : 4;

    RasterizerStateDesc();
	static bool equals(const RasterizerStateDesc& lhs, const RasterizerStateDesc& rhs);
};

/** ステンシル処理 */
struct StencilOpDesc
{
    /** ステンシルテストに失敗した場合のステンシル処理です。(default:Keep) */
    StencilOp stencilFailOp : 4;

    /** ステンシルテストに合格で、深度テストが不合格の場合のステンシル処理です。(default:Keep) */
    StencilOp stencilDepthFailOp : 4;

    /** ステンシルテストと深度テストに合格した場合のステンシル処理です。(default:Keep) */
    StencilOp stencilPassOp : 4;

    /** ステンシルテストの比較関数　(default:Always) */
    ComparisonFunc stencilFunc : 4;

    StencilOpDesc();
	static bool equals(const StencilOpDesc& lhs, const StencilOpDesc& rhs);
};

/** 深度ステンシルステート */
struct DepthStencilStateDesc
{
    /** 深度テストの比較関数 (default:ComparisonFunc::LessEqual) */
    ComparisonFunc depthTestFunc : 4;

    /** 深度書き込みの有効状態 (default:true) */
    bool depthWriteEnabled : 1;

    /** ステンシルテストの有効状態 (default:false) */
    bool stencilEnabled : 1;

    /** ステンシルテストの参照値 (default:0xFF) */
    uint8_t stencilReferenceValue;

    /** 法線がカメラの方向を向いている面のステンシル処理 */
    StencilOpDesc frontFace;

    /** 法線がカメラと逆方向を向いている面のステンシル処理 */
    StencilOpDesc backFace;

    DepthStencilStateDesc();
	static bool equals(const DepthStencilStateDesc& lhs, const DepthStencilStateDesc& rhs);
};

} // namespace ln
