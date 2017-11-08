
#pragma once
#include "../Graphics/Color.h"
#include "../Rendering/Rendering.h"	// for DynamicLightInfo
#include "SceneNode.h"
#include "WorldObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/** ライトのコンポーネントです。 */
class LightComponent
	: public SceneNode
{

public:

	/** ライト種類を設定します。 */
	void setType(LightType type) { m_lightInfo->m_type = type; }

	/** ライト種類を取得します。 */
	LightType getType() const { return m_lightInfo->m_type; }

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: false) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_lightInfo->m_diffuse = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_lightInfo->m_diffuse; }

	/** ライトの影響範囲を設定します。(default: 10) */
	void setRange(float range) { m_lightInfo->m_range = range; }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_lightInfo->m_range; }


	/// スポットライトのコーン角度の設定 (ラジアン単位)
	void setSpotAngle(float angle) { m_spotAngle = angle; }

	/// スポットライトのコーン角度の取得 (ラジアン単位)
	float getSpotAngle() const { return m_spotAngle; }

	/// [Advanced]
	void setShadowZFar(float z) { m_lightInfo->m_shadowZFar = z; }

	/// [Advanced]
	float getShadowZFar() const { return m_lightInfo->m_shadowZFar; }

public:	// internal

	/// 各行列を更新する (SceneNode::updateFrameHierarchy() の後で呼び出すこと)
	//void updateMatrices(/*const Size& viewSize*/);

	// 向きの取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	//const Vector4& getDirectionInternal() const { return m_lightInfo->m_direction; }

	// 行列の取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	//const Matrix& GetWorldViewProj() const { return m_worldViewProjMatrix; }
	//const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	//const Matrix& GetProjectionMatrix() const { return m_projMatrix; }
	//const Matrix& GetViewProjectionMatrix() const { return m_viewProjMatrix; }
	//const Matrix& GetViewMatrixI() const { return m_viewMatrixI; }
	//const Matrix& GetProjectionMatrixI() const { return m_projMatrixI; }
	//const Matrix& GetViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	//const Matrix& GetViewMatrixT() const { return m_viewMatrixT; }
	//const Matrix& GetProjectionMatrixT() const { return m_projMatrixT; }
	//const Matrix& GetViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	//const Matrix& GetViewMatrixIT() const { return m_viewMatrixIT; }
	//const Matrix& GetProjectionMatrixIT() const { return m_projMatrixIT; }
	//const Matrix& GetViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

protected:
	virtual void onPreRender(DrawList* context) override;

LN_CONSTRUCT_ACCESS:
	LightComponent();
	virtual ~LightComponent();
	void initialize(LightType type);

private:
	friend class VisualComponent;

	//LightType			m_type;				///< ライトの種類
	//Color	m_diffuse;			///< ディフューズカラー
	//Color	m_ambient;			///< アンビエントカラー
	//Color	m_specular;			///< スペキュラカラー
	Ref<detail::DynamicLightInfo>	m_lightInfo;
	bool				m_enabled;			///< 有効状態
	float				m_spotAngle;		///< コーン角度 (ラジアン単位)

	//Matrix				m_viewMatrix;		///< ビュー行列
	//Matrix				m_projMatrix;		///< プロジェクション行列
	//Matrix				m_viewProjMatrix;	///< ビュー行列とプロジェクション行列の積
	//Vector4				m_direction;		///< 向き

	// 以下はシェーダ変数への設定用。ライトは個々のノードに比べて参照される回数が多いので
	// 必要になるたびに計算するのではなく、あらかじめ計算しておく。
	//Matrix				m_worldViewProjMatrix;
	//Matrix				m_viewMatrixI;		///< ビュー行列 (inverse)
	//Matrix				m_projMatrixI;		///< プロジェクション行列 (inverse)
	//Matrix				m_viewProjMatrixI;	///< ビュー行列とプロジェクション行列の積 (inverse)
	//Matrix				m_viewMatrixT;		///< ビュー行列 (transpose)
	//Matrix				m_projMatrixT;		///< プロジェクション行列 (transpose)
	//Matrix				m_viewProjMatrixT;	///< ビュー行列とプロジェクション行列の積 (transpose)
	//Matrix				m_viewMatrixIT;		///< ビュー行列 (inverse * transpose)
	//Matrix				m_projMatrixIT;		///< プロジェクション行列 (inverse * transpose)
	//Matrix				m_viewProjMatrixIT;	///< ビュー行列とプロジェクション行列の積 (inverse * transpose)

	float				m_tmpDistance;		///< ノードに近いライトを検索するときの作業用変数
};



/** ポイントライトのコンポーネントです。 */
class PointLightComponent
	: public SceneNode
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_lightInfo->m_diffuse = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_lightInfo->m_diffuse; }

	/** ライトの影響範囲を設定します。(default: 10) */
	void setRange(float range) { m_lightInfo->m_range = range; }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_lightInfo->m_range; }

protected:
	virtual void onPreRender(DrawList* context) override;

LN_CONSTRUCT_ACCESS:
	PointLightComponent();
	virtual ~PointLightComponent();
	void initialize();

private:
	Ref<detail::DynamicLightInfo>	m_lightInfo;
	bool							m_enabled;
};






/**
	@brief
*/
class Light
	: public WorldObject
{
	LN_OBJECT;
public:
	LightComponent* GetLightComponent() const;

LN_CONSTRUCT_ACCESS:
	Light();
	virtual ~Light();
	void initialize();

LN_INTERNAL_ACCESS:

private:
	Ref<LightComponent>	m_component;
};

/** ポイントライトのオブジェクトです。 */
class PointLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定でポイントライトを作成します。 */
	static Ref<PointLight> create();

	/** 色と範囲を指定してポイントライトを作成します。 */
	static Ref<PointLight> create(const Color& color, float range);

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの影響範囲を設定します。(default: 10) */
	void setRange(float range) { m_component->setRange(range); }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_component->getRange(); }

	/** コンポーネントを取得します。 */
	PointLightComponent* GetPointLightComponent() const;

LN_CONSTRUCT_ACCESS:
	PointLight();
	virtual ~PointLight();

	/** 既定の設定でポイントライトを作成します。 */
	void initialize();

	/** 色と範囲を指定してポイントライトを作成します。 */
	void initialize(const Color& color, float range);

private:
	Ref<PointLightComponent>	m_component;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
