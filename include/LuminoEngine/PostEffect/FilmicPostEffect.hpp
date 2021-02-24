#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "SSRPostEffect.hpp"
#include "BloomPostEffect.hpp"
#include "DepthOfFieldPostEffect.hpp"
#include "TonemapPostEffect.hpp"

namespace ln {
class SamplerState;
namespace detail { class FilmicPostEffectInstance; }

class FilmicPostEffect
    : public PostEffect
{
public:
	/** アンチエイリアスの有無を設定します。(default: true) */
	LN_METHOD(Property)
	void setAntialiasEnabled(bool value) { m_antialiasEnabled = value; }

	/** アンチエイリアスの有無を取得します。 */
	LN_METHOD(Property)
	bool isAntialiasEnabled() const { return m_antialiasEnabled; }
    
	/** SSR (Screen Space Reflection) の有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setSSREnabled(bool value) { m_ssrEnabled = value; }

	/** SSR の有無を取得します。 */
	LN_METHOD(Property)
	bool isSSREnabled() const { return m_ssrEnabled; }
    
	/** SSAO (Screen Space Ambient Occlusion) の有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setSSAOEnabled(bool value) { m_ssaoEnabled = value; }

	/** SSAO の有無を取得します。 */
	LN_METHOD(Property)
	bool isSSAOEnabled() const { return m_ssaoEnabled; }
    
	/** ブルームエフェクトの有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setBloomEnabled(bool value) { m_bloomEnabled = value; }

	/** ブルームエフェクトの有無を取得します。 */
	LN_METHOD(Property)
	bool isBloomEnabled() const { return m_bloomEnabled; }
    
	/** 被写界深度の有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setDOFEnabled(bool value) { m_dofEnabled = value; }

	/** 被写界深度の有無を取得します。 */
	LN_METHOD(Property)
	bool isDOFEnabled() const { return m_dofEnabled; }
    
	/** トーンマッピングの有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setTonemapEnabled(bool value) { m_tonemapEnabled = value; }

	/** トーンマッピングの有無を取得します。 */
	LN_METHOD(Property)
	bool isTonemapEnabled() const { return m_tonemapEnabled; }
    
	/** ビネットエフェクトの有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setVignetteEnabled(bool value) { m_vignetteEnabled = value; }

	/** ビネットエフェクトの有無を取得します。 */
	LN_METHOD(Property)
	bool isVignetteEnabled() const { return m_vignetteEnabled; }
    
	/** ガンマ補正の有無を設定します。(default: false) */
	LN_METHOD(Property)
	void setGammaEnabled(bool value) { m_gammaEnabled = value; }

	/** ガンマ補正の有無を取得します。 */
	LN_METHOD(Property)
	bool isGammaEnabled() const { return m_gammaEnabled; }


	void setFocusedLinearDepth(float value) { m_focusedLinearDepth = value; }
	float focusedLinearDepth() const { return m_focusedLinearDepth; }


	void setScreenBlendColor(const Color& value) { m_screenBlendColor = value; }

	const Color& screenBlendColor() const { return m_screenBlendColor; }

	void setScreenColorTone(const ColorTone& value) { m_screenColorTone = value; }

	const ColorTone& screenColorTone() const { return m_screenColorTone; }

	float m_linearWhite = 5.0f;
	float m_shoulderStrength = 0.15f;
	float m_linearStrength = 0.5f;
	float m_linearAngle = 0.1f;
	float m_toeStrength = 0.2f;
	float m_toeNumerator = 0.02f;
	float m_toeDenominator = 0.3f;
	float m_exposure = 0.0f;

    float m_luminosityThreshold;
    float m_bloomStrength;
    float m_bloomRadius;


	Color m_vignetteColor;
	Vector2 m_vignetteCenter;
	Vector4 m_vignetteSettings;

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    FilmicPostEffect();
    void init();

private:

	float m_focusedLinearDepth = 0.1f;
	Color m_screenBlendColor;
	ColorTone m_screenColorTone;

    bool m_antialiasEnabled;
    bool m_ssrEnabled;
    bool m_ssaoEnabled;
    bool m_bloomEnabled;
    bool m_dofEnabled;
    bool m_tonemapEnabled;
    bool m_vignetteEnabled;
	bool m_gammaEnabled;

    friend class detail::FilmicPostEffectInstance;
};

namespace detail {

class FilmicPostEffectInstance
    : public PostEffectInstance
{
public:


protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    FilmicPostEffectInstance();
    bool init(FilmicPostEffect* owner);

private:
    FilmicPostEffect* m_owner;
    Ref<Material> m_integrationMaterial;
    Ref<Material> m_ssaoMaterial;
    Ref<SamplerState> m_samplerState;
    SSRPostEffectCore m_ssrEffect;
    BloomPostEffectCore m_bloomEffect;
    DepthOfFieldPostEffectCore m_dofEffect;

#if 1
#else
    bool m_antialiasEnabled = true;
    bool m_ssrEnabled = false;
    bool m_ssaoEnabled = false;
    bool m_bloomEnabled = false;
    bool m_dofEnabled = false;
    bool m_tonemapEnabled = true;
    bool m_vignetteEnabled = true;
    bool m_gammaEnabled = true;
#endif
};

} // namespace detail
} // namespace ln

