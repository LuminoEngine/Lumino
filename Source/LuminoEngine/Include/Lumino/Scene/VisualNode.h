
#pragma once
#include "Common.h"
#include "Detail.h"
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include "SceneNode.h"
#include "WorldObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class MaterialList2;

/// VisualComponent
LN_CLASS()
class VisualComponent
	: public SceneNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:



	//-------------------------------------------------------------------------
	/** @name Main material utilities. */
	/** @{ */

	Material* GetMainMaterial() const;

	tr::ReflectionObjectList<Material*>* GetMaterials() const;




	/** 不透明度を設定します。(default: 1.0) */
	void SetOpacity(float value);

	/** 不透明度を取得します。 */
	float GetOpacity() const;

	/** カラースケールを設定します。(default: Color(1, 1, 1, 1)) */
	void SetColorScale(const Color& value);

	/** カラースケールを取得します。 */
	const Color& GetColorScale() const;

	/** ブレンドカラーを設定します。(default: Color(0, 0, 0, 0)) */
	void SetBlendColor(const Color& value);

	/** ブレンドカラーを取得します。 */
	const Color& GetBlendColor() const;

	/** 色調を設定します。(default: Tone(0, 0, 0, 0)) */
	void SetTone(const ToneF& value);

	/** 色調を取得します。 */
	const ToneF& GetTone() const;

	/** シェーダを設定します。(default: nullptr) */
	void SetShader(Shader* value);

	/** シェーダを取得します。 */
	Shader* GetShader() const;





	///** メインマテリアルの不透明度を設定します。(default: 1.0)*/
	//void SetOpacity(float value, int subsetIndex = -1);
	//float GetOpacity() const;


	///** メインマテリアルのカラースケールを設定します。(default: )*/
	///// 乗算色の設定TODO: Color32
	//void SetColorScale(const Color& value, int subsetIndex = -1);
	//void SetColorScale(float r, float g, float b, float a = 1.0f, int subsetIndex = -1);
	//const Color& GetColorScale() const;

	//void SetColor(const Color32& value);
	//void SetColor(int r, int g, int b, int a = 255);

	///// ブレンドカラーの設定 TODO: Color32
	//void SetBlendColor(const Color& value, int subsetIndex = -1);

	///// 色調の設定 TODO: Tone32
	//void SetTone(const ToneF& value, int subsetIndex = -1);


	/////// UV 変換行列の設定
	////void SetUVTransform(const Matrix& matrix, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform = matrix; }

	/////// UV 変換行列の設定
	////const Matrix& GetUVTransform(int subsetIndex = -1)  const { return m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform; }

	///// シェーダの設定
	//void SetShader(Shader* value, int subsetIndex = -1);

	/** @} */

	//-------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

	/** このノードを描画する際の合成方法を設定します。デフォルトは BlendMode::Alpha です。*/
	void SetBlendMode(BlendMode mode);

	/** このノードを描画する際の合成方法を取得します。*/
	//BlendMode GetBlendMode() const;

	/** このノードを描画する際のカリング方法を設定します。デフォルトは CullingMode::Back です。*/
	void SetCullingMode(CullingMode mode);

	/** このノードを描画する際のカリング方法を取得します。*/
	//CullingMode GetCullingMode() const;

	/** このノードを描画する際の深度テストの有無を設定します。デフォルトは true です。*/
	void SetDepthTestEnabled(bool enabled);

	/** このノードを描画する際の深度テストの有無を取得します。*/
	//bool IsDepthTestEnabled() const;

	/** このノードを描画する際の深度書き込みの有無を設定します。デフォルトは true です。*/
	void SetDepthWriteEnabled(bool enabled);

	/** このノードを描画する際の深度書き込みの有無を取得します。*/
	//bool IsDepthWriteEnabled() const;

	/** @} */

public:


	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_VisualNode; }
	virtual void UpdateFrameHierarchy(SceneNode* parent, float deltaTime) override;

	/// ノード単位の描画情報の更新。この後すぐ一連のサブセット描画が始まる
	//virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) {}


	virtual detail::Sphere GetBoundingSphere();

	//void DrawSubsetInternal(SceneGraphRenderingContext* dc, int subsetIndex, MMEShader* shader, ShaderPass* pass);

	//virtual void OnRender(SceneGraphRenderingContext* dc);
	//virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) {}


protected:
	VisualComponent();
	virtual ~VisualComponent();
	void Initialize(SceneGraph* owner, int subsetCount);	// TODO: ここでサブセット数渡す必要はないかな

LN_INTERNAL_ACCESS:
	MaterialList2* GetMaterialList() { return m_materialList; }
	Shader* GetPrimaryShader() const;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual detail::SceneNodeDefaultShaderClass GetShaderClass() { return detail::SceneNodeDefaultShaderClass_StaticMesh; }

protected:
	RefPtr<MaterialList2>	m_materialList;

private:
	void Render(DrawList* context) override;
};


/**
	@brief
*/
class MaterialList2
	: public tr::ReflectionObjectList<Material*>	// SubMaterials (0 の場合もある)
{
public:
	Material* GetMainMaterial() const;
	
LN_INTERNAL_ACCESS:
	MaterialList2();
	virtual ~MaterialList2();
	void Initialize(int subMaterialCount, bool createMainMaterial);
	void CopyShared(MaterialList* srcList, bool createMainMaterial);
	
LN_INTERNAL_ACCESS:
	RefPtr<Material>				m_mainMaterial;
};




/**
	@brief		
*/
class VisualObject
	: public WorldObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/** 不透明度を設定します。(default: 1.0) */
	void SetOpacity(float value);

	/** 不透明度を取得します。 */
	float GetOpacity() const;

	/** カラースケールを設定します。(default: Color(1, 1, 1, 1)) */
	void SetColorScale(const Color& value);

	/** カラースケールを設定します。(default: Color(1, 1, 1, 1)) */
	void SetColorScale(float r, float g, float b, float a = 1.0f);

	/** カラースケールを取得します。 */
	const Color& GetColorScale() const;

	/** ブレンドカラーを設定します。(default: Color(0, 0, 0, 0)) */
	void SetBlendColor(const Color& value);

	/** ブレンドカラーを取得します。 */
	const Color& GetBlendColor() const;

	/** 色調を設定します。(default: Tone(0, 0, 0, 0)) */
	void SetTone(const ToneF& value);

	/** 色調を取得します。 */
	const ToneF& GetTone() const;

	/** シェーダを設定します。(default: nullptr) */
	void SetShader(Shader* value);

	/** シェーダを取得します。 */
	Shader* GetShader() const;

protected:
	virtual VisualComponent* GetMainVisualComponent() const = 0;

LN_CONSTRUCT_ACCESS:
	VisualObject();
	virtual ~VisualObject();
	void Initialize();
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
