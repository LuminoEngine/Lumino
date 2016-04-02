
#pragma once 

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/// 合成方法を表します
enum BlendMode
{
	BlendMode_Normal = 0,		///< 通常 (アルファ無視)
	BlendMode_Alpha,			///< アルファブレンド
	BlendMode_Add,				///< 加算
	BlendMode_AddAlphaDisable,	///< 加算（アルファ無視）
	BlendMode_Sub,				///< 減算
	BlendMode_SubAlphaDisable,	///< 減算（アルファ無視）
	BlendMode_Mul,				///< 乗算
	BlendMode_Screen,			///< スクリーン
	//BlendMode_Reverse,			///< リバース

	BlendMode_Max,				///< (terminator)
};

/// カリング方法を表します
enum CullingMode
{
	CullingMode_None = 0,		///< 両面表示
	CullingMode_Front,			///< 前面を描画しない
	CullingMode_Back,			///< 裏面を描画しない

	CullingMode_Max,			///< (terminator)
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
	Less,						///< (現在のピクセル値 < 新しいピクセル値) 新しいピクセル値が、現在のピクセル値未満の場合に、新しいピクセル値を採用します。
	LessEqual,					///< (現在のピクセル値 <= 新しいピクセル値) 新しいピクセル値が、現在のピクセル値以下の場合に、新しいピクセル値を採用します。
	Greater,					///< (現在のピクセル値 > 新しいピクセル値) 新しいピクセル値が、現在のピクセル値を超える場合に、新しいピクセル値を採用します。
	GreaterEqual,				///< (現在のピクセル値 >= 新しいピクセル値) 新しいピクセル値が、現在のピクセル値以上の場合に、新しいピクセル値を採用します。
	Equal,						///< (現在のピクセル値 == 新しいピクセル値) 新しいピクセル値が、現在のピクセル値と等しい場合に、新しいピクセル値を採用します。
	NotEqual,					///< (現在のピクセル値 != 新しいピクセル値) 新しいピクセル値が、現在のピクセル値と等しくない場合に、新しいピクセル値を採用します。
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

/**
	@brief	グラフィックデバイスのレンダリングステートを表すクラスです。
*/
class RenderState
{
public:

	BlendMode		Blend;			///< 合成方法 (default:BlendMode_Normal)
	CullingMode		Culling;		///< カリング方法 (default:CullingMode_Back)
	FillMode		Fill;			///< 塗りつぶし方法 (default:FillMode_Solid。Opengl ES では無効)
	bool			AlphaTest;      ///< アルファテストの有無 (default:true。Opengl ES では無効)
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


namespace detail
{

	
LN_ENUM_FLAGS(ContextStateFlags)
{
	None = 0x00,
	CommonState = 0x01,
	ShaderPass = 0x02,		// これ以外のステートに比べて割と頻繁に変更されるので個別に分ける
	All = 0xFFFF,
};
LN_ENUM_FLAGS_DECLARE(ContextStateFlags);

struct ContextState
{


	static const int MaxMultiRenderTargets = 4;

	RenderState				renderState;
	DepthStencilState		depthStencilState;
	Texture*				depthBuffer = nullptr;
	Rect					viewport;
	VertexBuffer*			vertexBuffer = nullptr;
	IndexBuffer*			indexBuffer = nullptr;

	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	~ContextState();


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void Reset()
	{
		renderState = RenderState();
		depthStencilState = DepthStencilState();
		depthBuffer = nullptr;
		viewport = Rect();
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
	}

private:
	std::array<Texture*, MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
};

}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
