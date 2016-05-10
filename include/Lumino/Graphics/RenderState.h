
#pragma once 

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/** ブレンディングの演算方法 */
enum class BlendOp
{
	Add,						/**< 転送元に転送先を加算する。*/
	Subtract,					/**< 転送元から転送先を減算する。*/
	ReverseSubtract,			/**< 転送先から転送元を減算する。*/
	Min,						/**< 転送先と転送元から小さいほうを使用する。*/
	Max,						/**< 転送先と転送元から大きいほうを使用する。*/
};

/** ブレンディングの係数 */
enum class BlendFactor
{
	Zero,						/**< ブレンディング係数は、(0, 0, 0, 0) */
	One,						/**< ブレンディング係数は、(1, 1, 1, 1) */
	SourceColor,				/**< ブレンディング係数は、(Rs, Gs, Bs, As) */
	InverseSourceColor,			/**< ブレンディング係数は、(1-Rs, 1-Gs, 1-Bs, 1-As) */
	SourceAlpha,				/**< ブレンディング係数は、(As, As, As, As) */
	InverseSourceAlpha,			/**< ブレンディング係数は、(1-As, 1-As, 1-As, 1-As) */
	DestinationColor,			/**< ブレンディング係数は、(Rd, Gd, Bd, Ad) */
	InverseDestinationColor,	/**< ブレンディング係数は、(1-Rd, 1-Gd, 1-Bd, 1-Ad) */
	DestinationAlpha,			/**< ブレンディング係数は、(Ad, Ad, Ad, Ad) */
	InverseDestinationAlpha,	/**< ブレンディング係数は、(1-Ad, 1-Ad, 1-Ad, 1-Ad) */
};

/** カリング方法 */
enum class CullingMode
{
	None,						/**< 両面表示 */
	Front,						/**< 前面を描画しない */
	Back,						/**< 裏面を描画しない */
};

/// 塗りつぶし方法を表します
enum FillMode
{
	FillMode_Solid = 0,			///< 面を塗りつぶす
	FillMode_Wire,				///< ワイヤーフレーム
	FillMode_Point,				///< 頂点

	FillMode_Max,				///< (terminator)
};

/// 比較関数の定数
LN_ENUM(CompareFunc)
{
	Never = 0,					///< 常に失敗します。
	Less,						///< (新しいピクセル値 < 現在のピクセル値) 新しいピクセル値が、現在のピクセル値未満の場合に、新しいピクセル値を採用します。
	LessEqual,					///< (新しいピクセル値 <= 現在のピクセル値) 新しいピクセル値が、現在のピクセル値以下の場合に、新しいピクセル値を採用します。
	Greater,					///< (新しいピクセル値 > 現在のピクセル値) 新しいピクセル値が、現在のピクセル値を超える場合に、新しいピクセル値を採用します。
	GreaterEqual,				///< (新しいピクセル値 >= 現在のピクセル値) 新しいピクセル値が、現在のピクセル値以上の場合に、新しいピクセル値を採用します。
	Equal,						///< (新しいピクセル値 == 現在のピクセル値) 新しいピクセル値が、現在のピクセル値と等しい場合に、新しいピクセル値を採用します。
	NotEqual,					///< (新しいピクセル値 != 現在のピクセル値) 新しいピクセル値が、現在のピクセル値と等しくない場合に、新しいピクセル値を採用します。
	Always,						///< 常に成功します。
};
LN_ENUM_DECLARE(CompareFunc);

/// ステンシルテストの結果に応じたステンシル処理方法です。
LN_ENUM(StencilOp)
{
	Keep = 0,					///< 既存のステンシル データを保持します。(何もしません)
	Replace,					///< ステンシルデータをステンシル参照値に設定します。
};
LN_ENUM_DECLARE(StencilOp);


/** 合成方法 */
enum class BlendMode
{
	Normal,						/**< 通常 (アルファ無視) */
	Alpha,						/**< アルファブレンド */
	Add,						/**< 加算 */
	AddAlphaDisabled,			/**< 加算（アルファ無視）*/
	Subtract,					/**< 減算 */
	SubtractAlphaDisabled,		/**< 減算（アルファ無視*/
	MultiplyAlphaDisabled,		/**< 乗算 (アルファ無視) */
};

/**
	@brief	グラフィックデバイスのレンダリングステートを表すクラスです。
*/
class RenderState
{
public:
	static const RenderState Default;

	bool			alphaBlendEnabled;	/**< アルファブレンドの有無 */
	BlendOp			blendOp;			/**< ブレンディングの演算方法 (default: Add) */
	BlendFactor		sourceBlend;		/**< ブレンディングの係数 (default: One) */
	BlendFactor		destinationBlend;	/**< ブレンディングの係数 (default: Zero) */
	
	BlendMode		Blend;	/*TODO: 廃止予定*/		///< 合成方法 (default:BlendMode::Alpha)
	CullingMode		Culling;		///< カリング方法 (default:CullingMode_Back)
	FillMode		Fill;			///< 塗りつぶし方法 (default:FillMode_Solid)
	bool			AlphaTest;      ///< アルファテストの有無 (default:true)
	//bool			PointSprite;	///< ポイントスプライトモードの有無 (default:false)

public:

	/**
		@brief	デフォルト値で初期化します。
	*/
	RenderState();

	/**
		@brief	現在の設定のハッシュ値を計算します。
	*/
	uint32_t GetHashCode() const;

	bool Equals(const RenderState& state) const;

public:
	bool operator == (const RenderState& obj) const;
	bool operator != (const RenderState& obj) const { return !operator==(obj); }
};

/**
	@brief	グラフィックデバイスの深度テスト及びステンシルテストのステートを表すクラスです。
*/
class DepthStencilState
{
public:
	static const DepthStencilState Default;

	bool		DepthTestEnabled;		///< 深度テストの有無 (default:true)
	bool		DepthWriteEnabled;		///< 深度書き込みの有無 (default:true)
	CompareFunc	DepthTestFunc;			///< 深度テストの比較関数 (default:LessEqual)

	bool		StencilEnabled;			///< ステンシルテストの有無 (default:false)
	CompareFunc	StencilFunc;			///< ステンシルテストの比較関数 (default:Always)
	uint8_t		StencilReferenceValue;	///< ステンシルテストの参照値 (default:0)
	StencilOp	StencilFailOp;			///< ステンシルテストに失敗した場合のステンシル処理です。(default:Keep)
	StencilOp	StencilDepthFailOp;		///< ステンシルテストに合格で、深度テストが不合格の場合のステンシル処理です。(default:Keep)
	StencilOp	StencilPassOp;			///< ステンシルテストと深度テストに合格した場合のステンシル処理です。(default:Keep)

public:
	/**
		@brief	デフォルト値で初期化します。
	*/
	DepthStencilState();

	// TODO:
	// OpenGL : デプスバッファでの比較→ステンシルバッファでの比較
	// DirectX : 「ステンシルテスト」→「深度テスト」
	// ・・・らしい。要確認。

	bool Equals(const DepthStencilState& state) const;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
