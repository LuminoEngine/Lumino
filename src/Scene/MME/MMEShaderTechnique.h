
#pragma once

//#include "../../Graphics/Interface.h"
//#include "../../Graphics/DeviceObjects.h"
//#include "../DrawCommand.h"
//#include "MMETypes.h"
//#include "SceneShader.h"
#include <Lumino/Base/Nullable.h>
#include "MMETypes.h"
#include "ShaderScriptCommandList.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN
class MMEShader;
class MMEShaderPass;
class MMEShaderErrorInfo;

/**
	@note	
		アノテーション
			string Subset		e.g) "0-4,6,8"
			string MMDPass		"object" or "object_ss" or "shadow" or "edge"
			bool UseTexture
			bool UseSphereMap
			bool UseToon
			bool UseSelfShadow	[MMM]
			string Script

			MMDPass の "object_ss" は MMM では使用非推奨。
*/
class MMEShaderTechnique
	: public RefObject
{
public:
	MMEShaderTechnique();
	virtual ~MMEShaderTechnique();

public:

	/// 初期化
	void Initialize(MMEShader* shader, Graphics::ShaderTechnique* tech, MMEShaderErrorInfo* errorInfo);

	/// このテクニックを持つシェーダを取得する
	MMEShader* GetOwnerShader() const { return m_ownerShader; }

	/// 描画パスを取得する
	MMDPass GetMMDPass() const { return m_mmdPass; }

	/// テクスチャを使用している材質のみを対象とするかどうか
	const Nullable<bool>& IsUseTexture() const { return m_isUseTexture; }

	/// スフィアマップを使用している材質のみを対象とするかどうか
	const Nullable<bool>& IsUseSphereMap() const { return m_isUseSphereMap; }

	/// モデルのみを対象とするかどうか
	const Nullable<bool>& IsUseToon() const { return m_isUseToon; }

	/// [MMM] セルフシャドウを使用している材質のみを対象とするかどうか
	const Nullable<bool>& IsUseSelfShadow() const { return m_isUseSelfShadow; }

	/// このテクニックは指定した番号のサブセットを描画できるか
	bool ContainsSubsetIndex(int subsetIndex) const;
	
	/// 描画コマンドリストの取得
	ShaderScriptCommandList& GetDrawingCommandList() { return m_scriptCommandList; }

private:

	/// MMEShaderPass を検索する
	MMEShaderPass* FindMMEShaderPass(const TCHAR* passName);

	/// スクリプト文字列の変数を受け取って commandList にコマンドを追加していく
	//      thisPass : パス内のスクリプトを解析する場合、そのパスを渡す (Draw="Geometry" の実行に渡すpass)
	void ParseScriptCommandString(Graphics::ShaderVariable* scriptAnno, Graphics::ShaderPass* thisPass, ShaderScriptCommandList* commandList);


	static void ParseSubsetIndexArray(const String& subsetText, Array<bool>* subsetIndexArray, bool* endToRange);

	/// 描画サブセットを示す文字列から、サブセット番号の配列を作成する
	///		'-' による範囲指定で「x番以降すべて」を描画する場合、配列の終端には -1 が格納される。
	static void ParseSubsetIndexArray(const TCHAR* subsetText, Array<int>* subsetIndexArray);

private:

	typedef Array< std::pair<String, ShaderScriptCommandList> >	PassScriptCommandList;

	MMEShaderErrorInfo*			m_errorInfo;
	MMEShader*					m_ownerShader;
	Graphics::Shader*			m_coreShader;
	Graphics::ShaderTechnique*	m_technique;

	MMDPass						m_mmdPass;
	Nullable<bool>				m_isUseTexture;
	Nullable<bool>				m_isUseSphereMap;
	Nullable<bool>				m_isUseToon;
	Nullable<bool>				m_isUseSelfShadow;
	Array<bool>					m_subsetIndexArray;
	bool						m_subsetIndexEndToRange;

	ShaderScriptCommandList		m_scriptCommandList;	///< このテクニックの Script 及び子パスの Script
	Array<MMEShaderPass*>		m_mmeShaderPasses;
};


class MMEShaderPass
	: public RefObject
{
public:
	Graphics::ShaderPass*	m_pass;
	ShaderScriptCommandList	m_scriptCommandList;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
