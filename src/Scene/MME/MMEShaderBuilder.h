
#pragma once
#include "MMETypes.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class MMEShader;
class MMEShaderErrorInfo;

/// MMEShaderBuilder
///		このクラスは MMEShader の静的なデータの構築を行う。
///		・変数の要求データ、属性の決定
///		・テクニックの解析
///		・テクニック、パスのスクリプトのコマンド化
///		テクスチャ等リソースの確保はこのクラスでは行わない。
///		特にレンダリングターゲットはサイズを倍率指定で作成している場合はバックバッファサイズが変わったら再確保するべきなので、
///		実行中に実態が (頻繁にではないが) 変わることもあり得る。こういったリソースは「動的」と位置付けている。
///		また、メタデータの解析処理とリソース確保の処理を合わせると相当なコード量になるので、担当を分けることで簡略化を図っている。
class MMEShaderBuilder
{
public:
	static MMEShader* Create(SceneGraphManager* manager, MMEShader* shader, MMEShaderErrorInfo* errorInfo);

private:
	MMEShaderBuilder(SceneGraphManager* manager, MMEShader* shader, MMEShaderErrorInfo* errorInfo);
	~MMEShaderBuilder();

	void Build();

	///	シェーダ変数のセマンティクスを調べて対応する要求項目を返す
	///		@param[out] script_output		: STANDARDSGLOBAL の ScriptOutput の値を格納する変数のアドレス (常に "color")
	///		@param[out] script_class		: エフェクトファイルの使用目的 (デフォルトで "object")
	///		@param[out] script_order		: エフェクトファイルの実行タイミング (デフォルトは "standard")            
	///		@param[out] is_controllobject	: 有効な CONTROLOBJECT セマンティクスの場合、true が格納される
	///		script_output_、script_class_、script_order_ は STANDARDSGLOBAL が
	///		ある場合のみ設定される。この関数の呼び出し側で sv_->Request == MME_VARREQ_STANDARDSGLOBAL と
	///		チェック後、変数の中身を確認すること。
	static void CheckVariableRequest(
		ShaderVariable* var,    // TODO: const化したい
		MMEShaderVariable* sv,
		MMEScriptOutput* script_output,
		MMEScriptClass* script_class,
		MMEScriptOrder* script_order,
		bool* is_controllobject);

	/// セマンティクス名に対応する MMEセマンティクス を取得する
	static MMESemantic GetMMESemanticBySemanticName(const String& name);

	/// 変数の "Object" アノテーションを調べて、"Light" の場合は fales、それ以外("Camera") は true を返す
	///		TODO: ビューとプロジェクション行列の視点を決めるために使う。必ずどちらか必要なので、省略されていた時は警告するべきかも？
	static bool CheckAnnotationCameraOrLight(ShaderVariable* var);

	/// 変数の "Object" アノテーションを調べて、"Geometry" の場合は 1、"Light" の場合は 2、それ以外は 0 を返す
	static int CheckAnnotationGeometryOrLight(ShaderVariable* var);

	/// 名前を指定してアノテーションを検索する。見つからなければ NULL を返す。 (大文字小文字の区別無し。IShaderVariable からは大小小文字区別ありでしか取れないので用意)
	static ShaderVariable* GetAnnotationByName(ShaderVariable* var, const TCHAR* name);

	/// 要求項目から、計算する必要のある行列マスクのビット列を作成して返す
	static uint32_t CheckMatrixMask(MMEVariableRequest req);

private:

	MMEShader*				m_mmeShader;
	MMEShaderErrorInfo*		m_errorInfo;

};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
