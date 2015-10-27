
#pragma once
#include "../Internal.h"
#include "MMETypes.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class IMMESceneObject;
class MMEShaderErrorInfo;
class MMEShaderVariable;
class MMEShaderTechnique;
	
/// MMEShader
class MMEShader
	: public RefObject
{
public:
	static const int MaxAffectLights = 4;	///< 影響を受けるライトの最大数 (MMM は 3)

	/// 作成
	static MMEShader* Create(const char* code, int codeLength, MMEShaderErrorInfo* errorInfo, SceneGraphManager* manager);

public:

	/// Graphics::Shader の取得
	Shader* GetCoreShader() const { return m_coreShader; }

	/// 設定する必要があるライトの数の取得
	int GetRequiredLightCount() const { return m_requiredLightCount; }

	/// Scene 単位で必要なパラメータを設定する
	void UpdateSceneParams(const MMESceneParams& params, SceneGraphManager* scene);

	/// Camera 単位で必要なパラメータを設定する
	void UpdateCameraParams(Camera* camera, const SizeF& viewPixelSize);

	/// Node 単位で必要なパラメータを設定する
	void UpdateNodeParams(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList);

	/// Subset 単位で必要なパラメータを設定する
	void UpdateSubsetParams(const Internal::VisualNodeSubsetParams& params);

	/// CONTROLLOBJECT 設定 (obj は NULL 可能。存在しなければ bool 型変数に false を設定するため)
	static void SetControllObjectParam(MMEShaderVariable* sv, const IMMESceneObject* obj);

	/// WORLD、VIEW、PROJECTION、WORLDVIEW、VIEWPROJECTION、WORLDVIEWPROJECTION
	// オブジェクトのワールド行列と、カメラまたはライトのビュー・プロジェクション行列を求める。
	// outMatrix に値を格納した場合は true を返す。
	// node の結合済みグローバル行列を作成した後で呼び出すこと。
	static bool GetGeometryTransform(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList, MMEVariableRequest req, int lightIndex, Matrix* outMatrix);

	/// 指定したパスに一致するテクニックを検索する
	MMEShaderTechnique* FindTechnique(MMDPass mmdPass, bool UseTexture, bool UseSphereMap, bool UseToon, bool UseSelfShadow, int subsetIndex);


private:
	MMEShader(SceneGraphManager* manager);
	virtual ~MMEShader();

	typedef Array<MMEShaderVariable*>	MMEShaderVariableList;
	typedef Array<MMEShaderTechnique*>	MMEShaderTechniqueList;

	SceneGraphManager*		m_manager;

	// 以下、MMEShaderBuilder　から直接セットされるデータ
	friend class MMEShaderBuilder;
	RefPtr<Shader>				m_coreShader;
	MMEScriptOutput				m_mmeScriptOutput;
	MMEScriptClass				m_mmeScriptClass;
	MMEScriptOrder				m_mmeScriptOrder;
	MMEShaderVariableList		m_mmeShaderVariableList;
	MMEShaderVariableList		m_controlObjectVariables;	///< "CONTROLOBJECT" を要求している変数リスト
	uint32_t					m_worldMatrixCalcMask;		///< MMEWorldMatrixCalcFlags の組み合わせ (もしかしたら使わないかも)
	int							m_requiredLightCount;		///< このシェーダが必要としているライトの数 (ライトを要求する変数のうち、一番多い要素数)
	MMEShaderTechniqueList		m_mmeShaderTechniqueList;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
