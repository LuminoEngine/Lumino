
#pragma once
//#include "Common.hpp"

namespace ln {
class RenderingContext;
class KeyFrameAnimationCurve;

namespace detail {

class InternalSkyBox
	: public Object
{
public:
	InternalSkyBox();
	void init();
    void setLightDirection(const Vector3& value);
	void render(RenderingContext* context, const RenderViewPoint* viewPoint);

private:
	Ref<Material> m_material;
    Vector3 m_lightDirection;
};


class InternalSkyDome
	: public RefObject
{
public:
	InternalSkyDome();
	bool init();

    void setSkyColor(const Color& value) { m_skyColor = value.toVector4(); }
    void setHorizonColor(const Color& value) { m_horizonColor = value.toVector4(); }
    void setCloudColor(const Color& value) { m_cloudColor = value.toVector4(); }
    void setOverlayColor(const Color& value) { m_overlayColor = value.toVector4(); }

    void update(float elapsedTimer);
	void render(RenderingContext* context, const RenderViewPoint* viewPoint);
    const Color& sceneColor() const { return m_sceneColor; }

private:
	Ref<MeshModel> m_model;
	Ref<Material> m_material;
	Vector3 m_lightDirection;

    Ref<KeyFrameAnimationCurve> m_backGroundSkyDomeColorR;
    Ref<KeyFrameAnimationCurve> m_backGroundSkyDomeColorG;
    Ref<KeyFrameAnimationCurve> m_backGroundSkyDomeColorB;

    Ref<KeyFrameAnimationCurve> m_backGroundHorizonColorR;
    Ref<KeyFrameAnimationCurve> m_backGroundHorizonColorG;
    Ref<KeyFrameAnimationCurve> m_backGroundHorizonColorB;

    Ref<KeyFrameAnimationCurve> m_allOverlayColorR;
    Ref<KeyFrameAnimationCurve> m_allOverlayColorG;
    Ref<KeyFrameAnimationCurve> m_allOverlayColorB;
    Ref<KeyFrameAnimationCurve> m_allOverlayColorA;

    Ref<KeyFrameAnimationCurve> m_baseCloudColorAndIntensityR;
    Ref<KeyFrameAnimationCurve> m_baseCloudColorAndIntensityG;
    Ref<KeyFrameAnimationCurve> m_baseCloudColorAndIntensityB;
    Ref<KeyFrameAnimationCurve> m_baseCloudColorAndIntensityA;

    float m_timeOfDay;
    Vector4 m_skyColor;
    Vector4 m_horizonColor;
    Vector4 m_cloudColor;
    Vector4 m_overlayColor;

    Color m_fixedBackGroundSkyDomeColor;
    Color m_fixedBackGroundHorizonColor;
    Color m_fixedBaseCloudColorAndIntensity;
    Color m_fixedAllOverlayColor;
    Color m_sceneColor;
};

} // namespace detail
} // namespace ln
