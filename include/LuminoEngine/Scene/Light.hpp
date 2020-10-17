
#pragma once
#include "../Visual/LightComponent.hpp"
#include "WorldObject.hpp"

namespace ln {
class AmbientLightComponent;
class HemisphereLightComponent;
class DirectionalLightComponent;
class PointLightComponent;
class SpotLightComponent;

/**
 * 環境ライトのオブジェクトです。
 *
 * 環境ライトは、アンビエントライティング、半球ライティング、ディレクショナルライトが統合された、
 * シーン全体のライティングをコントロールするためのオブジェクトです。
 *
 * World はメインライトとして、1 つの EnvironmentLight がデフォルトで存在しています。
 * 
 */
LN_CLASS()
class EnvironmentLight
	: public WorldObject
{
	LN_OBJECT;
public:
	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	LN_METHOD(Property)
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	LN_METHOD(Property)
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ディレクショナルライトの光源色を設定します。(default: White) */
	LN_METHOD(Property)
	void setColor(const Color& value) { m_component->setColor(value); }

	/** ディレクショナルライトの光源色を取得します。 */
	LN_METHOD(Property)
	const Color& getColor() const { return m_component->getColor(); }

	/** シーン全体の環境光の色を設定します。(default: 0.5, 0.5, 0.5) */
	LN_METHOD(Property)
	void setAmbientColor(const Color& value) { m_component->setAmbientColor(value); }

	/** シーン全体の環境光の色を取得します。 */
	LN_METHOD(Property)
	const Color& getAmbientColor() const { return m_component->getAmbientColor(); }

	/** 空の環境光の色を取得します。 */
	LN_METHOD(Property)
	const Color& getSkyColor() { return m_component->getSkyColor(); }

	/** 空の環境光の色を設定します。(default: Black) */
	LN_METHOD(Property)
	void setSkyColor(const Color& value) { m_component->setSkyColor(value); }

	/** 地面の環境光の色を取得します。 */
	LN_METHOD(Property)
	const Color& getGroundColor() { return m_component->getGroundColor(); }

	/** 地面の環境光の色を設定します。(default: Black) */
	LN_METHOD(Property)
	void setGroundColor(const Color& value) { m_component->setGroundColor(value); }

	/** ライトの明るさを設定します。(default: 0.5) */
	LN_METHOD(Property)
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	LN_METHOD(Property)
	float getIntensity() const { return m_component->getIntensity(); }

	/** 視点からの、影を生成できる距離を指定します。 (default: 0.0f) */
	LN_METHOD(Property)
	void setShadowEffectiveDistance(float value) { m_component->setShadowEffectiveDistance(value); }

	/** 視点からの、影を生成できる距離を取得します。 */
	LN_METHOD(Property)
	float shadowEffectiveDistance() const { return m_component->shadowEffectiveDistance(); }

	/** 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。 */
	LN_METHOD(Property)
	void setShadowEffectiveDepth(float value) { m_component->setShadowEffectiveDepth(value); }

	/** 光源方向からの、影を生成できる距離を指定します。 */
	LN_METHOD(Property)
	float shadowEffectiveDepth() const { return m_component->shadowEffectiveDepth(); }

	/** コンポーネントを取得します。 */
	EnvironmentLightComponent* environmentLightComponent() const;

LN_CONSTRUCT_ACCESS:
	EnvironmentLight();
	virtual ~EnvironmentLight();
	void init();

private:
	Ref<EnvironmentLightComponent> m_component;
};

/** ディレクショナルライトのオブジェクトです。 */
LN_CLASS()
class DirectionalLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定でディレクショナルライトを作成します。 */
	static Ref<DirectionalLight> create();

	/** 色を指定してディレクショナルライトを作成します。 */
	static Ref<DirectionalLight> create(const Color& color);

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	LN_METHOD(Property)
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	LN_METHOD(Property)
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	LN_METHOD(Property)
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	LN_METHOD(Property)
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 0.5) */
	LN_METHOD(Property)
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	LN_METHOD(Property)
	float getIntensity() const { return m_component->getIntensity(); }

	/** 視点からの、影を生成できる距離を指定します。 (default: 0.0f) */
	LN_METHOD(Property)
	void setShadowEffectiveDistance(float value) { m_component->setShadowEffectiveDistance(value); }

	/** 視点からの、影を生成できる距離を取得します。 */
	LN_METHOD(Property)
	float shadowEffectiveDistance() const { return m_component->shadowEffectiveDistance(); }

	/** 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。 */
	LN_METHOD(Property)
	void setShadowEffectiveDepth(float value) { m_component->setShadowEffectiveDepth(value); }

	/** 光源方向からの、影を生成できる距離を指定します。 */
	LN_METHOD(Property)
	float shadowEffectiveDepth() const { return m_component->shadowEffectiveDepth(); }

	/** コンポーネントを取得します。 */
	DirectionalLightComponent* getDirectionalLightComponent() const;

LN_CONSTRUCT_ACCESS:
	DirectionalLight();
	virtual ~DirectionalLight();

	/** 既定の設定でディレクショナルライトを作成します。 */
	LN_METHOD()
	void init();

	/** 色を指定してディレクショナルライトを作成します。 */
	LN_METHOD(OverloadPostfix = "WithColor")
	void init(const Color& color);

private:
	Ref<DirectionalLightComponent>	m_component;
};

/** ポイントライトのオブジェクトです。 */
LN_CLASS()
class PointLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定でポイントライトを作成します。 */
	static Ref<PointLight> create();

	/** 色と範囲を指定してポイントライトを作成します。 */
	static Ref<PointLight> create(const Color& color, float range);

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	LN_METHOD(Property)
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	LN_METHOD(Property)
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	LN_METHOD(Property)
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	LN_METHOD(Property)
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 1.0) */
	LN_METHOD(Property)
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	LN_METHOD(Property)
	float getIntensity() const { return m_component->getIntensity(); }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	LN_METHOD(Property)
	void setRange(float range) { m_component->setRange(range); }

	/** ライトの影響範囲を取得します。 */
	LN_METHOD(Property)
	float getRange() const { return m_component->getRange(); }

	/** ライトの減衰を設定します。(default: 1.0) */
	LN_METHOD(Property)
	void setAttenuation(float attenuation) { m_component->setAttenuation(attenuation); }

	/** ライトの減衰を取得します。 */
	LN_METHOD(Property)
	float getAttenuation() const { return m_component->getAttenuation(); }

	/** コンポーネントを取得します。 */
	PointLightComponent* getPointLightComponent() const;

LN_CONSTRUCT_ACCESS:
	PointLight();
	virtual ~PointLight();

	/** 既定の設定でポイントライトを作成します。 */
	LN_METHOD()
	void init();

	/** 色と範囲を指定してポイントライトを作成します。 */
	LN_METHOD(OverloadPostfix = "WithColorAndRange")
	void init(const Color& color, float range);

private:
	Ref<PointLightComponent>	m_component;
};

/** スポットライトのオブジェクトです。 */
LN_CLASS()
class SpotLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定でスポットライトを作成します。 */
	static Ref<SpotLight> create();

	/** 色と範囲を指定してスポットライトを作成します。 */
	static Ref<SpotLight> create(const Color& color, float range, float angle);

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	LN_METHOD(Property)
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	LN_METHOD(Property)
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	LN_METHOD(Property)
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	LN_METHOD(Property)
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 1.0) */
	LN_METHOD(Property)
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	LN_METHOD(Property)
	float getIntensity() const { return m_component->getIntensity(); }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	LN_METHOD(Property)
	void setRange(float range) { m_component->setRange(range); }

	/** ライトの影響範囲を取得します。 */
	LN_METHOD(Property)
	float getRange() const { return m_component->getRange(); }

	/** ライトの減衰を設定します。(default: 1.0) */
	LN_METHOD(Property)
	void setAttenuation(float attenuation) { m_component->setAttenuation(attenuation); }

	/** ライトの減衰を取得します。 */
	LN_METHOD(Property)
	float getAttenuation() const { return m_component->getAttenuation(); }

	/** スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3) */
	LN_METHOD(Property)
	void setAngle(float angle) { m_component->setAngle(angle); }

	/** スポットライトのコーン角度を取得します。(ラジアン単位) */
	LN_METHOD(Property)
	float getAngle() const { return m_component->getAngle(); }

	/** スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0) */
	LN_METHOD(Property)
	void setPenumbra(float penumbra) { m_component->setPenumbra(penumbra); }

	/** スポットライトのコーン角度に対する減衰率を設定します。 */
	LN_METHOD(Property)
	float getPenumbra() const { return m_component->getPenumbra(); }

	/** コンポーネントを取得します。 */
	SpotLightComponent* getSpotLightComponent() const;

LN_CONSTRUCT_ACCESS:
	SpotLight();
	virtual ~SpotLight();

	/** 既定の設定でスポットライトを作成します。 */
	LN_METHOD()
	void init();

	/** 色と範囲を指定してスポットライトを作成します。 */
	LN_METHOD(OverloadPostfix = "WithColorAndRange")
	void init(const Color& color, float range, float angle);

private:
	Ref<SpotLightComponent>	m_component;
};

} // namespace ln
