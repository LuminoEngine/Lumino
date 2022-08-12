#pragma once
#include <LuminoGraphics/Rendering/Common.hpp>
#include "VisualComponent.hpp"

namespace ln {

/**
 * 環境ライトのコンポーネントです。
 *
 * @see EnvironmentLight
 */
class EnvironmentLightComponent
	: public VisualComponent
{
	LN_OBJECT;
public:
	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool value) { m_enabled = value; }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_enabled; }

	/** ディレクショナルライトの光源色を設定します。(default: White) */
	void setColor(const Color& color) { m_color = color; }

	/** ディレクショナルライトの光源色を取得します。 */
	const Color& getColor() const { return m_color; }

	/** シーン全体の環境光の色を設定します。(default: 0.5, 0.5, 0.5) */
	void setAmbientColor(const Color& value) { m_ambientColor = value; }

	/** シーン全体の環境光の色を取得します。 */
	const Color& getAmbientColor() const { return m_ambientColor; }

	/** 空の環境光の色を取得します。 */
	const Color& getSkyColor() { return m_skyColor; }

	/** 空の環境光の色を設定します。(default: Black) */
	void setSkyColor(const Color& value) { m_skyColor = value; }

	/** 地面の環境光の色を取得します。 */
	const Color& getGroundColor() { return m_groundColor; }

	/** 地面の環境光の色を設定します。(default: Black) */
	void setGroundColor(const Color& value) { m_groundColor = value; }

	/** ライトの明るさを設定します。(default: 1.0) */
	void setIntensity(float value) { m_intensity = value; }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_intensity; }

	/** 視点からの、影を生成できる距離を指定します。 (default: 0.0f) */
	void setShadowEffectiveDistance(float value) { m_shadowEffectiveDistance = value; }

	/** 視点からの、影を生成できる距離を取得します。 */
	float shadowEffectiveDistance() const { return m_shadowEffectiveDistance; }

	/** 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。 */
	void setShadowEffectiveDepth(float value) { m_shadowEffectiveDepth = value; }

	/** 光源方向からの、影を生成できる距離を指定します。 */
	float shadowEffectiveDepth() const { return m_shadowEffectiveDepth; }

protected:
	virtual void onPrepareRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	EnvironmentLightComponent();
	virtual ~EnvironmentLightComponent();
	Result init();

private:
	Color m_color;
	Color m_ambientColor;
	Color m_skyColor;
	Color m_groundColor;
	float m_intensity;
	float m_shadowEffectiveDistance;
	float m_shadowEffectiveDepth;
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

	/** 視点からの、影を生成できる距離を指定します。 (default: 0.0f) */
	void setShadowEffectiveDistance(float value) { m_shadowEffectiveDistance = value; }

	/** 視点からの、影を生成できる距離を取得します。 */
	float shadowEffectiveDistance() const { return m_shadowEffectiveDistance; }

	/** 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。 */
	void setShadowEffectiveDepth(float value) { m_shadowEffectiveDepth = value; }

	/** 光源方向からの、影を生成できる距離を指定します。 */
	float shadowEffectiveDepth() const { return m_shadowEffectiveDepth; }

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
	float m_shadowEffectiveDistance;
	float m_shadowEffectiveDepth;

	Ref<Material> m_material;
	Ref<Material> m_spriteMaterial;
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
