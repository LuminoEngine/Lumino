
#pragma once
//#include "../Graphics/Color.h"
//#include "../Rendering/Rendering.h"	// for DynamicLightInfo
//#include "VisualComponent.h"
//#include "WorldObject.h"
#include "../Rendering/Common.hpp"
#include "VisualComponent.hpp"

namespace ln {
//namespace detail { class ShadowCasterPass; }

class Material;
#if 0
/** ライトのコンポーネントです。 */
class LightComponent
	: public VisualComponent
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
	void setColor(const Color& color) { m_lightInfo->m_color = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_lightInfo->m_color; }

	/** ライトの影響範囲を設定します。(default: 10) */
	void setRange(float range) { m_lightInfo->m_range = range; }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_lightInfo->m_range; }


	/// スポットライトのコーン角度の設定 (ラジアン単位)
	void setSpotAngle(float angle) { m_spotAngle = angle; }

	/// スポットライトのコーン角度の取得 (ラジアン単位)
	float getSpotAngle() const { return m_spotAngle; }

	///// [Advanced]
	//void setShadowZFar(float z) { m_lightInfo->m_shadowZFar = z; }

	///// [Advanced]
	//float getShadowZFar() const { return m_lightInfo->m_shadowZFar; }

public:	// internal

	/// 各行列を更新する (VisualComponent::updateFrameHierarchy() の後で呼び出すこと)
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
	void init(LightType type);

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
#endif

/**
 * アンビエントライトのコンポーネントです。
 *
 * たとえばこの色を一番明るくすると(つまり真っ白)、光源が無くても影が生成されず、
 * マテリアルに指定した色指定がそのまま表示されます。
 */
class AmbientLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_color = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_color; }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_intensity = intensity; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

protected:
	virtual void onPrepareRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	AmbientLightComponent();
	virtual ~AmbientLightComponent();
	void init();

private:
	Color m_color;
	float m_intensity;
	bool m_enabled;
};

/** 半球ライトのコンポーネントです。 */
class HemisphereLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** 空の環境光の色を取得します。 */
	const Color& getSkyColor() { return m_skyColor; }

	/** 空の環境光の色を設定します。(default: White) */
	void setSkyColor(const Color& color) { m_skyColor = color; }

	/** 地面の環境光の色を取得します。 */
	const Color& getGroundColor() { return m_groundColor; }

	/** 地面の環境光の色を設定します。(default: White) */
	void setGroundColor(const Color& color) { m_groundColor = color; }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_intensity = intensity; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

protected:
	virtual void onPrepareRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	HemisphereLightComponent();
	virtual ~HemisphereLightComponent();
	void init();

private:
	Color m_skyColor;
	Color m_groundColor;
	float m_intensity;
	bool m_enabled;
};

/** ディレクショナルライトのコンポーネントです。 */
class DirectionalLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_color = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_color; }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_intensity = intensity; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

	/** 影の有無を設定します。(default: false) */
	void setShadowCast(bool enabled);

	/** 影の有無を取得します。 */
	bool isShadowCast() const;

protected:
	virtual void onPrepareRender(RenderingContext* context) override;
	virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	DirectionalLightComponent();
	virtual ~DirectionalLightComponent();
	void init();

private:
	Color m_color;
	float m_intensity;
	bool m_enabled;

	Ref<Material> m_material;
};

/** ポイントライトのコンポーネントです。 */
class PointLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_color = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_color; }

	/** ライトの明るさを設定します。(default: 1.0) */
	void setIntensity(float intensity) { m_intensity = intensity; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	void setRange(float range) { m_range = range; }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_range; }

	/** ライトの減衰を設定します。(default: 1.0) */
	void setAttenuation(float attenuation) { m_attenuation = attenuation; }

	/** ライトの減衰を取得します。 */
	float getAttenuation() const { return m_attenuation; }

protected:
	virtual void onPrepareRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	PointLightComponent();
	virtual ~PointLightComponent();
	void init();

private:
	Color m_color;
	float m_intensity;
	float m_range;
	float m_attenuation;
	bool m_enabled;
};

/** スポットライトのコンポーネントです。 */
class SpotLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_color = color; }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_color; }

	/** ライトの明るさを設定します。(default: 1.0) */
	void setIntensity(float intensity) { m_intensity = intensity; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	void setRange(float range) { m_range = range; }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_range; }

	/** ライトの減衰を設定します。(default: 1.0) */
	void setAttenuation(float attenuation) { m_attenuation = attenuation; }

	/** ライトの減衰を取得します。 */
	float getAttenuation() const { return m_attenuation; }

	/** スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3) */
	void setAngle(float angle) { m_spotAngle = angle; }

	/** スポットライトのコーン角度を取得します。(ラジアン単位) */
	float getAngle() const { return m_spotAngle; }

	/** スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0.1) */
	void setPenumbra(float penumbra) { m_spotPenumbra = penumbra; }

	/** スポットライトのコーン角度に対する減衰率を設定します。 */
	float getPenumbra() const { return m_spotPenumbra; }

protected:
	virtual void onPrepareRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	SpotLightComponent();
	virtual ~SpotLightComponent();
	void init();

private:
	Color m_color;
	float m_intensity;
	float m_range;
	float m_attenuation;
	float m_spotAngle;
	float m_spotPenumbra;
	bool m_enabled;
};




} // namespace ln
