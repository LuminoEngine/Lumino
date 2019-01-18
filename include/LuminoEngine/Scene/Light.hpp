
#pragma once
#include "../Visual/LightComponent.hpp"
#include "WorldObject.hpp"

namespace ln {
class AmbientLightComponent;
class HemisphereLightComponent;
class DirectionalLightComponent;
class PointLightComponent;
class SpotLightComponent;

/** アンビエントライトのオブジェクトです。 */
class AmbientLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定でアンビエントライトを作成します。 */
	static Ref<AmbientLight> create();

	/** 色を指定してアンビエントライトを作成します。 */
	static Ref<AmbientLight> create(const Color& color);

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_component->getIntensity(); }

	/** コンポーネントを取得します。 */
	AmbientLightComponent* getAmbientLightComponent() const;

LN_CONSTRUCT_ACCESS:
	AmbientLight();
	virtual ~AmbientLight();

	/** 既定の設定でアンビエントライトを作成します。 */
	void initialize();

	/** 色を指定してアンビエントライトを作成します。 */
	void initialize(const Color& color);

private:
	Ref<AmbientLightComponent>	m_component;
};

/** 半球ライトのオブジェクトです。 */
class HemisphereLight
	: public WorldObject
{
	LN_OBJECT;
public:

	/** 既定の設定で半球ライトを作成します。 */
	static Ref<HemisphereLight> create();

	/** 色を指定して半球ライトを作成します。 */
	static Ref<HemisphereLight> create(const Color& skyColor, const Color& groundColor);

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** 空の環境光の色を取得します。 */
	const Color& getSkyColor() { return m_component->getSkyColor(); }

	/** 空の環境光の色を設定します。(default: White) */
	void setSkyColor(const Color& color) { m_component->setSkyColor(color); }

	/** 地面の環境光の色を取得します。 */
	const Color& getGroundColor() { return m_component->getGroundColor(); }

	/** 地面の環境光の色を設定します。(default: White) */
	void setGroundColor(const Color& color) { m_component->setGroundColor(color); }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_component->getIntensity(); }

	/** コンポーネントを取得します。 */
	HemisphereLightComponent* getHemisphereLightComponent() const;

LN_CONSTRUCT_ACCESS:
	HemisphereLight();
	virtual ~HemisphereLight();

	/** 既定の設定で半球ライトを作成します。 */
	void initialize();

	/** 色を指定して半球ライトを作成します。 */
	void initialize(const Color& skyColor, const Color& groundColor);

private:
	Ref<HemisphereLightComponent>	m_component;
};

/** ディレクショナルライトのオブジェクトです。 */
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
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 0.5) */
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_component->getIntensity(); }

	/** コンポーネントを取得します。 */
	DirectionalLightComponent* getDirectionalLightComponent() const;

LN_CONSTRUCT_ACCESS:
	DirectionalLight();
	virtual ~DirectionalLight();

	/** 既定の設定でディレクショナルライトを作成します。 */
	void initialize();

	/** 色を指定してディレクショナルライトを作成します。 */
	void initialize(const Color& color);

private:
	Ref<DirectionalLightComponent>	m_component;
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

public:

	/** ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true) */
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 1.0) */
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_component->getIntensity(); }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	void setRange(float range) { m_component->setRange(range); }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_component->getRange(); }

	/** ライトの減衰を設定します。(default: 1.0) */
	void setAttenuation(float attenuation) { m_component->setAttenuation(attenuation); }

	/** ライトの減衰を取得します。 */
	float getAttenuation() const { return m_component->getAttenuation(); }

	/** コンポーネントを取得します。 */
	PointLightComponent* getPointLightComponent() const;

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

/** スポットライトのオブジェクトです。 */
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
	void setEnabled(bool enabled) { m_component->setEnabled(enabled); }

	/** ライトの有効状態を取得します。 */
	bool isEnabled() const { return m_component->isEnabled(); }

	/** ライトカラーを設定します。(default: White) */
	void setColor(const Color& color) { m_component->setColor(color); }

	/** ライトカラーを取得します。 */
	const Color& getColor() const { return m_component->getColor(); }

	/** ライトの明るさを設定します。(default: 1.0) */
	void setIntensity(float intensity) { m_component->setIntensity(intensity); }

	/** ライトの明るさを取得します。 */
	float getIntensity() const { return m_component->getIntensity(); }

	/** ライトの影響範囲を設定します。(default: 10.0) */
	void setRange(float range) { m_component->setRange(range); }

	/** ライトの影響範囲を取得します。 */
	float getRange() const { return m_component->getRange(); }

	/** ライトの減衰を設定します。(default: 1.0) */
	void setAttenuation(float attenuation) { m_component->setAttenuation(attenuation); }

	/** ライトの減衰を取得します。 */
	float getAttenuation() const { return m_component->getAttenuation(); }

	/** スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3) */
	void setAngle(float angle) { m_component->setAngle(angle); }

	/** スポットライトのコーン角度を取得します。(ラジアン単位) */
	float getAngle() const { return m_component->getAngle(); }

	/** スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0) */
	void setPenumbra(float penumbra) { m_component->setPenumbra(penumbra); }

	/** スポットライトのコーン角度に対する減衰率を設定します。 */
	float getPenumbra() const { return m_component->getPenumbra(); }

	/** コンポーネントを取得します。 */
	SpotLightComponent* getSpotLightComponent() const;

LN_CONSTRUCT_ACCESS:
	SpotLight();
	virtual ~SpotLight();

	/** 既定の設定でスポットライトを作成します。 */
	void initialize();

	/** 色と範囲を指定してスポットライトを作成します。 */
	void initialize(const Color& color, float range, float angle);

private:
	Ref<SpotLightComponent>	m_component;
};

} // namespace ln
