
#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include "../Rendering/Common.hpp"
#include "WorldObject.hpp"

namespace ln {
class VisualComponent;

/** VisualComponent がアタッチされた WorldObject のプレハブクラスです。 */
LN_CLASS()
class VisualObject
	: public WorldObject
{
    LN_OBJECT;
    LN_BUILDER;
public:
	/** 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true) */
    LN_METHOD(Property)
    void setVisible(bool value);

	/** 可視状態を取得します。*/
    LN_METHOD(Property)
    bool isVisible() const;
    
    //-------------------------------------------------------------------------
    /** @name RenderState */
    /** @{ */

    /** ShadingModel を設定します。(default: ShadingModel::Default) */
    void setShadingModel(const Optional<ShadingModel>& value);

    /** ShadingModel を取得します。*/
    const Optional<ShadingModel>& shadingModel() const;

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    LN_METHOD(Property)
    void setBlendMode2(BlendMode value) { setBlendMode(value); }
    // TODO: Optional の Binding は、clearBlendMode() みたいなメソッドの自動生成程度でいいかも。
    // 正直凝ったことしても忘れるし、ほとんどのケースでは set だけ使って、get, clear はあまり使わないし。

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    void setBlendMode(const Optional<BlendMode>& mode);

    /** 合成方法を取得します。*/
    const Optional<BlendMode>& blendMode() const;

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
    LN_METHOD(Property)
    void setOpacity(float value);

    /** 不透明度を取得します。 */
    LN_METHOD(Property)
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

    const Ref<VisualComponent>& mainVisualComponent() const { return m_component; }


    // TODO: internal
    void setRenderPriority(int value);

protected:
    void serialize(Serializer2& ar) override;
    void setMainVisualComponent(VisualComponent* component);

LN_CONSTRUCT_ACCESS:
    VisualObject();
	virtual ~VisualObject();
	bool init();


private:
    Ref<VisualComponent> m_component;
};

//==============================================================================
// VisualObject::Builder

struct VisualObject::BuilderDetails : public WorldObject::BuilderDetails
{
    LN_BUILDER_DETAILS(VisualObject);

    Optional<bool> visible;
    Optional<ShadingModel> shadingModel;
    Optional<BlendMode> blendMode;
    Optional<CullMode> cullMode;

    Optional<bool> depthTest;
    Optional<bool> depthWrite;

    Optional<float> opacity;
    Optional<Color> colorScale;
    Optional<Color> blendColor;
    Optional<ColorTone> colorTone;

    void apply(VisualObject* p) const;
};

template<class T, class B, class D>
struct VisualObject::BuilderCore : public WorldObject::BuilderCore<T, B, D>
{
    LN_BUILDER_CORE(WorldObject::BuilderCore);

    B& visible(bool value) { d()->visible = value; return self(); }
    B& shadingModel(ShadingModel value) { d()->shadingModel = value; return self(); }
    B& blendMode(BlendMode value) { d()->blendMode = value; return self(); }
    B& cullMode(CullMode value) { d()->cullMode = value; return self(); }

    B& depthTest(bool value) { d()->depthTest = value; return self(); }
    B& depthWrite(bool value) { d()->depthWrite = value; return self(); }

    B& opacity(float value) { d()->opacity = value; return self(); }
    B& colorScale(const Color& value) { d()->colorScale = value; return self(); }
    B& blendColor(const Color& value) { d()->blendColor = value; return self(); }
    B& colorTone(const ColorTone& value) { d()->colorTone = value; return self(); }
};

LN_BUILDER_IMPLEMENT(VisualObject);

} // namespace ln
