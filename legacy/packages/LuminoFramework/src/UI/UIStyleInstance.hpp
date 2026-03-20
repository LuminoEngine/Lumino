#pragma once
#include <LuminoFramework/UI/UIStyle.hpp>

namespace ln {
class Shader;
class Texture;
class UIStyleContext;
class UIStyleClass;
class UITheme;

namespace detail {
class UIStyleClassInstance;

// どんな Element でも持つ一般的なスタイル値。
// UIStyle の optional 等を解決したもの。
// メモリ消費を抑えるため、UIStyleAttribute は使わないようにしている。
class UIStyleInstance
    : public RefObject {
public:
    //UIStyle* sourceLocalStyle = nullptr;	// 以下のデータの生成元となったローカスのスタイル

    // layout
    float width;
    float height;
    Thickness margin2;
    Thickness border;
    Thickness padding;
    //UIHAlignment hAlignment;
    //UIVAlignment vAlignment;
    UIHAlignment horizontalContentAlignment;
    UIVAlignment verticalContentAlignment;
    float minWidth;
    float minHeight;
    float maxWidth;
    float maxHeight;
    UIOverflowBehavior overflowX;
    UIOverflowBehavior overflowY;

    // layout transform
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    Vector3 centerPoint;

    // background
    Sprite9DrawMode backgroundDrawMode;
    Color backgroundColor;
    Ref<Texture> backgroundImage;
    Ref<Shader> backgroundShader;
    Rect backgroundImageRect;
    Thickness backgroundImageBorder;

    // border
    Thickness borderThickness = Thickness::Zero;
    CornerRadius cornerRadius = CornerRadius(0, 0, 0, 0); // TODO: borderRadius
    Color leftBorderColor = Color::Gray;
    Color topBorderColor = Color::Gray;
    Color rightBorderColor = Color::Gray;
    Color bottomBorderColor = Color::Gray;
    BorderDirection borderDirection = BorderDirection::Outside;
    bool borderInset = false;

    // shadow
    float shadowOffsetX;
    float shadowOffsetY;
    float shadowBlurRadius;
    float shadowSpreadRadius;
    Color shadowColor;
    bool shadowInset;

    // text
    Color textColor;
    String fontFamily;
    float fontSize;
    UIFontWeight fontWeight;
    UIFontStyle fontStyle;

    // render effects
    UIVisibility visible;
    BlendMode blendMode;

    //detail::BuiltinEffectData builtinEffect;
    float opacity;
    Color colorScale;
    Color blendColor;
    ColorTone tone;

    // decorators
    List<Ref<UIStyleDecorator>> decorators;

    // commited cache
    Ref<Font> font;
    Ref<Material> backgroundMaterial;

    UITheme* theme = nullptr;

    // TODO: 今後サブクラスごとにスタイルを追加する場合は、ここに map を設ける

    UIStyleInstance();
    void setupDefault();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyleInstance* other);

    void makeRenderObjects();

    static void updateStyleDataHelper(const UIStyleContext* context, const detail::UIStyleInstance* parentStyleData, const UIStyle* combinedStyle, detail::UIStyleInstance* outStyleData);

    Size actualOuterSpace() const {
        float w = margin2.width();
        float h = margin2.height();
        return Size(Math::isNaN(w) ? 0.0f : w, Math::isNaN(h) ? 0.0f : h);
    }
    Vector2 actualOuterOffset() const {
        return Vector2(Math::isNaN(margin2.left) ? 0.0f : margin2.left, Math::isNaN(margin2.top) ? 0.0f : margin2.top);
    }

    void updateAnimationData();
    bool hasAnimationData() const { return m_animationData != nullptr; }
    void advanceAnimation(float elapsedTime);
    UIElementDirtyFlags applyAnimationValues();

LN_CONSTRUCT_ACCESS:

private:
    struct AnimationData {
        Ref<UIScalarAnimationInstance> width;
        Ref<UIScalarAnimationInstance> height;
        Ref<UIScalarAnimationInstance> positionX;
        Ref<UIScalarAnimationInstance> positionY;
        Ref<UIScalarAnimationInstance> positionZ;
        Ref<UIVector4AnimationInstance> backgroundColor;
        Ref<UIScalarAnimationInstance> opacity;
    };

    AnimationData* acquireAnimationData() {
        if (!m_animationData) {
            m_animationData = std::make_unique<AnimationData>();
        }
        return m_animationData.get();
    }

    // これも結構サイズ大きいので、必要なものだけ遅延で作る
    std::unique_ptr<AnimationData> m_animationData;
};

class UIStyleClassInstance
    : public RefObject {
public:
    UIStyleClassInstance();
    const Ref<UIStyleInstance>& style() const { return m_style; }
    UIStyleInstance* findStateStyle(const StringView& stateName) const;        // 無い場合は nullptr
    UIStyleInstance* findSubElementStyle(const StringView& elementName) const; // 無い場合は nullptr
    void mergeFrom(const UIStyleSet* other);

private:
    struct VisualStateSlot {
        String name;
        Ref<UIStyleInstance> style;
    };

    struct SubElementSlot {
        String name;
        Ref<UIStyleInstance> style;
    };

    Ref<UIStyleInstance> m_style;
    List<VisualStateSlot> m_visualStateStyles;
    List<SubElementSlot> m_subElements;

    friend class UIStyleContext;
};

} // namespace detail
} // namespace ln
