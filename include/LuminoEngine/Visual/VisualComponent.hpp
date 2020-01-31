
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderingContext.hpp"
#include "../Scene/Component.hpp"

namespace ln {
namespace detail {
class GeometryStageParameters;
class BuiltinEffectData;
}

LN_CLASS()
class VisualComponent
	: public Component
{
    LN_OBJECT;
public:
	/** 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true) */
	LN_METHOD(Property)
	void setVisible(bool value) { m_isVisible = value; }

	/** 可視状態を取得します。*/
	LN_METHOD(Property)
	bool isVisible() const { return m_isVisible; }

    //-------------------------------------------------------------------------
    /** @name RenderState */
    /** @{ */

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    void setBlendMode(const Optional<BlendMode>& mode);

    /** 合成方法を取得します。*/
    const Optional<BlendMode>& blendMode() const;

    /** ShadingModel を設定します。(default: ShadingModel::Default) */
    void setShadingModel(const Optional<ShadingModel>& value);

    /** ShadingModel を取得します。*/
    const Optional<ShadingModel>& shadingModel() const;

    /** このノードを描画する際の深度テストの有無を設定します。(default: true) */
    void setDepthTestEnabled(const Optional<bool>& enabled);

    /** このノードを描画する際の深度テストの有無を取得します。*/
    const Optional<bool>& isDepthTestEnabled() const;

    /** このノードを描画する際の深度書き込みの有無を設定します。(default: true) */
    void setDepthWriteEnabled(const Optional<bool>& enabled);

    /** このノードを描画する際の深度書き込みの有無を取得します。*/
    const Optional<bool>& isDepthWriteEnabled() const;

    /** このノードを描画する際のカリング方法を設定します。(default: CullMode::Back) */
    void setCullMode(const Optional<CullMode>& mode);

    /** このノードを描画する際のカリング方法を取得します。*/
    const Optional<CullMode>& cullMode() const;

    /** @} */

    //-------------------------------------------------------------------------
    /** @name Effects */
    /** @{ */

    /** 不透明度を設定します。(default: 1.0) */
    void setOpacity(float value);

    /** 不透明度を取得します。 */
    float opacity() const;

    /** カラースケールを設定します。(default: Color(1, 1, 1, 1)) */
    void setColorScale(const Color& value);

    /** カラースケールを取得します。 */
    const Color& colorScale() const;

    /** ブレンドカラーを設定します。(default: Color(0, 0, 0, 0)) */
    void setBlendColor(const Color& value);

    /** ブレンドカラーを取得します。 */
    const Color& blendColor() const;

    /** 色調を設定します。(default: Tone(0, 0, 0, 0)) */
    void setTone(const ColorTone& value);

    /** 色調を取得します。 */
    const ColorTone& tone() const;

    /** @} */

    // TODO: internal
    void setRenderPriority(int value);

protected:

LN_CONSTRUCT_ACCESS:
    VisualComponent();
	virtual ~VisualComponent();
	bool init();

private:
    virtual void render(RenderingContext* context) override;

    std::unique_ptr<detail::GeometryStageParameters> m_geometryStageParameters;
    std::unique_ptr<detail::BuiltinEffectData> m_builtinEffectData;
    int m_renderPriority;
    bool m_isVisible;
};

} // namespace ln
