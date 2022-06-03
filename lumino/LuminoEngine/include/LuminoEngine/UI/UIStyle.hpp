#pragma once
#include <LuminoGraphics/Animation/Common.hpp>
#include "UIElement.hpp"
#include "UIStyleAnimation.hpp"

namespace ln {
class Shader;
class Texture;
class UIStyleContext;
class UIStyleClass;
class UITheme;
namespace detail {
class UIStyleInstance;
class UIStyleClassInstance;

enum class UIStyleAttributeValueSource {
    // InMerging,		// not set
    Default, // not set
             // InheritParent,	// 親 UIElement の Style から継承していることを示す
             // InheritBaseOn,
             // InheritBase,	// VisualState に対応するスタイルは、共通の基準スタイルを持つ (HTML の a:hover は a が基準スタイル)。その基準スタイルから継承しているか
    ByStyleInherit, // VisualState ごとのスタイルから継承しているか
    ByUserLocal,    // 直接設定された値であるか
};

template<typename T>
class UIStyleAttribute {
public:
    UIStyleAttribute()
        : m_value()
        , m_source(UIStyleAttributeValueSource::Default) {}

    void reset() {
        m_value = T();
        m_source = UIStyleAttributeValueSource::Default;
    }

    operator const T&() const {
        return m_value;
    }

    UIStyleAttribute& operator=(const T& v) {
        set(v);
        return *this;
    }

    void set(const T& v) {
        m_value = v;
        m_source = UIStyleAttributeValueSource::ByUserLocal;
    }
    const T& get() const {
        assert(hasValue());
        return m_value;
    }
    const T& getOrDefault(const T& defaultValue) const {
        if (hasValue()) {
            return m_value;
        }
        else {
            return defaultValue;
        }
    }

    bool hasValue() const {
        return m_source != UIStyleAttributeValueSource::Default;
    }

    T m_value;
    UIStyleAttributeValueSource m_source;
};

/*
Style更新の流れ
1. 継承を解決する
    - テーマ > 親 > ローカル の順に UIStyle を UIStyleInstance へマージしていく。
*/

// ちょっとメモリ効率よくした Optional<float>. NaN を　null とする。
class UIFloatOptional {
public:
    explicit constexpr UIFloatOptional(float value)
        : m_value(value) {}
    constexpr UIFloatOptional()
        : m_value(std::numeric_limits<float>::quiet_NaN()) {}

    constexpr float value() const { return m_value; }

    bool isNull() const { return std::isnan(m_value); }

private:
    float m_value;
};


} // namespace detail

enum class UIOverflowBehavior {
    /** 内容がボックスに収まらない場合、ボックスからはみ出して表示する。 */
    Visible,

    /** 内容がボックスに収まらない場合、収まらない部分は非表示とする。(スクロールバーは表示されない) */
    Hidden,

    /** 内容がボックスに収まらない場合、スクロールバーを表示する。 */
    Scroll,
};

// 装飾対象の padding の影響を受けない
class UIStyleDecorator
    : public Object {
public: // TODO: internal
    float m_width = std::numeric_limits<float>::quiet_NaN();
    float m_height = std::numeric_limits<float>::quiet_NaN();
    Thickness m_margin;
    UIHAlignment m_hAlignment = UIHAlignment::Center;
    UIVAlignment m_vAlignment = UIVAlignment::Center;

    // TODO: image

    Color m_color = Color(0.0f, 0.0f, 0.0f, 1.0f);
    Ref<Font> m_font; // decorator の font は継承関係を作らない。そのためプロパティを分ける必要はない
    uint32_t m_codePoint = 0;

    void setIconName(const StringView& value, int size);

    void render(UIRenderingContext* context, const Size& slotRect);

    LN_CONSTRUCT_ACCESS : UIStyleDecorator();
    void init();

private:
};

class UIStyle
    : public Object {
public:
    //// layout
    // Thickness margin;
    // Thickness padding;
    // UIHAlignment hAlignment;
    // UIVAlignment vAlignment;
    // float minWidth;
    // float minHeight;
    // float maxWidth;
    // float maxHeight;

    //// layout transform
    // Vector3 position;
    // Quaternion rotation;
    // Vector3 scale;
    // Vector3 centerPoint;

    //// text
    // Color textColor;
    // String fontFamily;
    // float fontSize;
    // UIFontWeight fontWeight;
    // UIFontStyle fontStyle;

    //// render effects
    // bool visible;
    // BlendMode blendMode;

    // float opacity;
    // Color colorScale;
    // Color blendColor;
    // ColorTone tone;

    // Flex
    Optional<UILayoutFlexDirection> flexDirection;
    Optional<UIStyleValue> flexBasis;
    Optional<float> flexGrow;
    Optional<float> flexShrink;
    Optional<UILayoutFlexWrap> flexWrap;
    // Alignment
    Optional<UILayoutJustify> justifyContent;
    Optional<UILayoutAlign> alignItems;
    Optional<UILayoutAlign> alignSelf;
    Optional<UILayoutAlign> alignContent;
    // layout
    Optional<UIStyleValue> width;
    Optional<UIStyleValue> height;
    Optional<UIStyleValue> minWidth;
    Optional<UIStyleValue> minHeight;
    Optional<UIStyleValue> maxWidth;
    Optional<UIStyleValue> maxHeight;
    Optional<UIStyleValue> marginTop;
    Optional<UIStyleValue> marginRight;
    Optional<UIStyleValue> marginBottom;
    Optional<UIStyleValue> marginLeft;
    Optional<UIStyleValue> borderTop;
    Optional<UIStyleValue> borderRight;
    Optional<UIStyleValue> borderBottom;
    Optional<UIStyleValue> borderLeft;
    Optional<UIStyleValue> paddingTop;
    Optional<UIStyleValue> paddingRight;
    Optional<UIStyleValue> paddingBottom;
    Optional<UIStyleValue> paddingLeft;
    Optional<UIStyleValue> positionTop;
    Optional<UIStyleValue> positionRight;
    Optional<UIStyleValue> positionBottom;
    Optional<UIStyleValue> positionLeft;
    Optional<UILayoutPositionType> positionType;
    Optional<UIStyleValue> aspectRatio;
    Optional<UIStyleValue> zIndex;
    //detail::UIStyleAttribute<float> width;
    //detail::UIStyleAttribute<float> height;
    //detail::UIStyleAttribute<Thickness> margin;
    //detail::UIStyleAttribute<Thickness> padding;
    //[[deprecated]] detail::UIStyleAttribute<UIHAlignment> hAlignment;
    //detail::UIStyleAttribute<UIVAlignment> vAlignment;
    //detail::UIStyleAttribute<UIHAlignment> horizontalContentAlignment; // UILayoutPanel2_Deprecated をどのように配置するか
    //detail::UIStyleAttribute<UIVAlignment> verticalContentAlignment;
    //detail::UIStyleAttribute<float> minWidth;
    //detail::UIStyleAttribute<float> minHeight;
    //detail::UIStyleAttribute<float> maxWidth;
    //detail::UIStyleAttribute<float> maxHeight;
    detail::UIStyleAttribute<UIOverflowBehavior> overflowX;
    detail::UIStyleAttribute<UIOverflowBehavior> overflowY;

    static constexpr UILayoutFlexDirection DefaultFlexDirection = UILayoutFlexDirection::Row;
    static constexpr UIStyleValue DefaultFlexBasis = UIStyleValue();
    static const float DefaultWidth;
    static const float DefaultHeight;
    static const float DefaultMinWidth;
    static const float DefaultMinHeight;
    static const float DefaultMaxWidth;
    static const float DefaultMaxHeight;
    //static const Thickness DefaultMargin;
    //static const Thickness DefaultPadding;
    //static const UIHAlignment DefaultHorizontalAlignment;
    //static const UIVAlignment DefaultVerticalAlignment;
    //static const UIHAlignment DefaultHorizontalContentAlignment;
    //static const UIVAlignment DefaultVerticalContentAlignment;
    static const UIOverflowBehavior DefaultOverflowX;
    static const UIOverflowBehavior DefaultOverflowY;

    // transform
    detail::UIStyleAttribute<Vector3> origin;
    detail::UIStyleAttribute<Quaternion> rotation; // TODO: eular にする
    detail::UIStyleAttribute<Vector3> scale;
    detail::UIStyleAttribute<Vector3> centerPoint;

    static const Vector3 DefaultOrigin;
    static const Quaternion DefaultRotation;
    static const Vector3 DefaultScale;
    static const Vector3 DefaultCenterPoint;

    // background
    detail::UIStyleAttribute<Sprite9DrawMode> backgroundDrawMode;
    detail::UIStyleAttribute<Color> backgroundColor;
    detail::UIStyleAttribute<Ref<Texture>> backgroundImage;
    detail::UIStyleAttribute<Ref<Shader>> backgroundShader;
    // ※↑ここは Material にはしない。そういった大きなクラスを持たせるとまた Brush の時みたいな問題 (アニメーションや、プロパティのパス指定) が出てくる。代わりに Material の構築に必要なものを持たせる
    detail::UIStyleAttribute<Rect> backgroundImageRect;
    detail::UIStyleAttribute<Thickness> backgroundImageBorder;

    static const Sprite9DrawMode DefaultBackgroundDrawMode;
    static const Color DefaultBackgroundColor;
    static const Ref<Texture> DefaultBackgroundImage;
    static const Ref<Shader> DefaultBackgroundShader;
    static const Rect DefaultBackgroundImageRect;
    static const Thickness DefaultBackgroundImageBorder;

    // border
    detail::UIStyleAttribute<Thickness> borderThickness;
    detail::UIStyleAttribute<CornerRadius> cornerRadius;
    detail::UIStyleAttribute<Color> leftBorderColor; // TODO: borderLeftColor
    detail::UIStyleAttribute<Color> topBorderColor;
    detail::UIStyleAttribute<Color> rightBorderColor;
    detail::UIStyleAttribute<Color> bottomBorderColor;
    detail::UIStyleAttribute<BorderDirection> borderDirection;
    detail::UIStyleAttribute<bool> borderInset;

    static const Thickness DefaultBorderThickness;
    static const CornerRadius DefaultCornerRadius;
    static const Color DefaultLeftBorderColor;
    static const Color DefaultTopBorderColor;
    static const Color DefaultRightBorderColor;
    static const Color DefaultBottomBorderColor;
    static const BorderDirection DefaultBorderDirection;
    static const bool DefaultBorderInset;

    // shadow
    detail::UIStyleAttribute<float> shadowOffsetX;
    detail::UIStyleAttribute<float> shadowOffsetY;
    detail::UIStyleAttribute<float> shadowBlurRadius;
    detail::UIStyleAttribute<float> shadowSpreadRadius;
    detail::UIStyleAttribute<Color> shadowColor;
    detail::UIStyleAttribute<bool> shadowInset;

    static const float DefaultShadowOffsetX;
    static const float DefaultShadowOffsetY;
    static const float DefaultShadowBlurRadius;
    static const float DefaultShadowSpreadRadius;
    static const Color DefaultShadowColor;
    static const bool DefaultShadowInset;

    // text
    detail::UIStyleAttribute<Color> textColor; // (default: Black)
    detail::UIStyleAttribute<String> fontFamily;
    detail::UIStyleAttribute<float> fontSize;
    detail::UIStyleAttribute<UIFontWeight> fontWeight;
    detail::UIStyleAttribute<UIFontStyle> fontStyle;

    static const Color DefaultTextColor;
    static const String DefaultFontFamily;
    static const float DefaultFontSize;
    static const UIFontWeight DefaultFontWeight;
    static const UIFontStyle DefaultFontStyle;

    // effects
    detail::UIStyleAttribute<UIVisibility> visible;
    detail::UIStyleAttribute<BlendMode> blendMode;
    detail::UIStyleAttribute<float> opacity;
    detail::UIStyleAttribute<Color> colorScale;
    detail::UIStyleAttribute<Color> blendColor;
    detail::UIStyleAttribute<ColorTone> tone;
    // NOTE: BlendMode と BuiltinEffect、特に opacity は頻繁な変更が想定される。
    // そのためこれらが変更されても、style の invalidate は行わない。
    // onRender で親 UIElement のプロパティを継承しながら描画を行う。

    static const UIVisibility DefaultVisible;
    static const BlendMode DefaultBlendMode;
    static const float DefaultOpacity;
    static const Color DefaultColorScale;
    static const Color DefaultBlendColor;
    static const ColorTone DefaultTone;

    // decorators
    List<Ref<UIStyleDecorator>> decorators;


    
    void setMargin(const Thickness& value) {
        marginTop = UIStyleValue(value.top);
        marginRight = UIStyleValue(value.right);
        marginBottom = UIStyleValue(value.bottom);
        marginLeft = UIStyleValue(value.left);
    }

    void setPadding(const Thickness& value) {
        paddingTop = UIStyleValue(value.top);
        paddingRight = UIStyleValue(value.right);
        paddingBottom = UIStyleValue(value.bottom);
        paddingLeft = UIStyleValue(value.left);
    }

    void setBorderColor(const Color& color) {
        leftBorderColor = color;
        topBorderColor = color;
        rightBorderColor = color;
        bottomBorderColor = color;
    }

    void setWidthAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setHeightAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setPositionXAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setPositionYAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setPositionZAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setBackgroundColorAnimation(const Color& startValue, const Color& targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);
    void setOpacityAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f, AnimationWrapMode wrapMode = AnimationWrapMode::Once);

    void setWidthTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setHeightTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setPositionXTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setPositionYTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setPositionZTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setBackgroundColorTransition(const Color& target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);
    void setOpacityTransition(float target, float duration, EasingMode timingFunction = EasingMode::Linear, float delay = 0.0f);

public: // TODO: internal
    void setupDefault();
    void reset();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyle* other);

    LN_CONSTRUCT_ACCESS : UIStyle();
    virtual ~UIStyle();
    void init();

private:
    struct AnimationData {
        Ref<UIValueTrack> width;
        Ref<UIValueTrack> height;
        Ref<UIValueTrack> positionX;
        Ref<UIValueTrack> positionY;
        Ref<UIValueTrack> positionZ;
        Ref<UIValueTrack> backgroundColor;
        Ref<UIValueTrack> opacity;

        uint32_t hasLocalValueFlags = 0;
        uint32_t inheritFlags = 0;

        void setLocalValueFlag(detail::UIStyleAnimationElement index, bool v) {
            if (v)
                hasLocalValueFlags |= (1 << index);
            else
                hasLocalValueFlags &= ~(1 << index);
        }
        void setInheritFlag(detail::UIStyleAnimationElement index, bool v) {
            if (v)
                inheritFlags |= (1 << index);
            else
                inheritFlags &= ~(1 << index);
        }
        bool hasValue(detail::UIStyleAnimationElement index) const { return hasLocalValue(index) || isInherited(index); } //(hasLocalValueFlags & (1 << index)) != 0; }
        bool isInherited(detail::UIStyleAnimationElement index) const { return (inheritFlags & (1 << index)) != 0; }
        bool hasLocalValue(detail::UIStyleAnimationElement index) const { return ((hasLocalValueFlags & (1 << index)) != 0); /* && !isInherited(index);*/ }
    };

    AnimationData* acquireAnimationData();

    // これも結構サイズ大きいので、必要なものだけ遅延で作る
    std::unique_ptr<AnimationData> m_animationData;

    friend class detail::UIStyleInstance;
};

// 要素ひとつに対応。複数の class を管理する
class UIStyleSet : public Object {
public:
    const String& elementName() const { return m_elementName; }
    // void setClassName(const StringView& value) { m_className = value; }

    UIStyleClass* mainStyleClass() const { return m_mainStyleClass; }
    void setMainStyleClass(UIStyleClass* value) { m_mainStyleClass = value; }

    void addStyleClass(UIStyleClass* styleClass);
    UIStyleClass* addStyleClass(const StringView& className);
    UIStyleClass* findStyleClass(const StringView& className) const;

    // TODO: internal
    void copyFrom(const UIStyleSet* other);
    void mergeFrom(const UIStyleSet* other);

LN_CONSTRUCT_ACCESS:
    UIStyleSet();
    virtual ~UIStyleSet();
    void init(const StringView& elementName);

private:
    String m_elementName;
    Ref<UIStyleClass> m_mainStyleClass;
    List<Ref<UIStyleClass>> m_styleClasses;

    friend class detail::UIStyleClassInstance;
    friend class UIStyleContext;
};

// 要素ひとつ、class ひとつに対応。VisualState を管理する
class UIStyleClass : public Object {
public:
    const String& name() const { return m_name; }

    UIStyle* mainStyle() const { return m_mainStyle; }
    void setMainStyle(UIStyle* value) { m_mainStyle = value; }

    void addStateStyle(const StringView& stateName, UIStyle* style);
    UIStyle* addStateStyle(const StringView& stateName);
    UIStyle* findStateStyle(const StringView& stateName) const;

    // TODO: internal
    void copyFrom(const UIStyleClass* other);
    void mergeFrom(const UIStyleClass* other);

    struct VisualStateSlot {
        String name;
        Ref<UIStyle> style;
    };

    const List<VisualStateSlot>& visualStateStyles() const { return m_visualStateStyles; }

LN_CONSTRUCT_ACCESS:
    UIStyleClass();
    virtual ~UIStyleClass();
    void init(const StringView& name);

private:
    String m_name;
    Ref<UIStyle> m_mainStyle;
    List<VisualStateSlot> m_visualStateStyles;

    friend class detail::UIStyleClassInstance;
    friend class UIStyleContext;
};

class UIStyleSheet : public Object {
public:
    void addStyleSet(const StringView& elementName, UIStyleSet* styleClass);
    Ref<UIStyleSet> addStyleSet(const StringView& elementName);
    UIStyleSet* findStyleSet(const StringView& elementName) const;

    UIStyle* obtainStyle(const StringView& selector);

LN_CONSTRUCT_ACCESS:
    UIStyleSheet();
    virtual ~UIStyleSheet();
    void init();

private:
    std::unordered_map<String, Ref<UIStyleSet>> m_classes;

    friend class UIStyleContext;
};

class UIStyleContext
    : public Object {
public:
    void addStyleSheet(UIStyleSheet* sheet);
    UIStyleSet* globalStyle() const { return m_globalStyle; }

    // TODO: internal
    // find を高速にするため、事前に確定できる継承関係を解決する。
    void build();
    UIStyleSet* findStyleSet(const StringView& elementName) const; // 無い場合は null
                                                                   // detail::UIStyleClassInstance* findResolvedStyleClass(const StringView& elementName) const; // 無い場合は global
    //  VisualState 以外を結合 (mainStyle のみ結合)
    void combineStyle(UIStyle* style, const StringView& elementName, const List<String>* classList) const;

    Ref<UITheme> mainTheme;

    // const Color& defaultTextColor() const { return Color::White; }
    // const String& defaultFontFamily() const { return; }
    // float defaultFontSize() const { return 12.0f; }
    // UIFontWeight DefaultFontWeight() const { return UIFontWeight::Normal; }
    // UIFontStyle DefaultFontStyle() const { return UIFontStyle::Normal; }

    LN_CONSTRUCT_ACCESS : UIStyleContext();
    virtual ~UIStyleContext();
    void init();

private:
    List<Ref<UIStyleSheet>> m_styleSheets;
    Ref<UIStyleSet> m_globalStyle;
    std::unordered_map<String, Ref<UIStyleSet>> m_elementStyles;

    // TODO: つかわないかも
    // Ref<detail::UIStyleClassInstance> m_resolvedGlobalStyle;                            // for unused VisualStateManager
    // std::unordered_map<String, Ref<detail::UIStyleClassInstance>> m_resolvedClasses;    // for unused VisualStateManager
};

class UIVisualStateManager
    : public Object {
public:
    struct Group {
        String name;
        List<String> stateNames;
        int activeStateIndex; // -1: no-state
    };

    void registerState(const StringView& groupName, const StringView& stateName) {
        int groupIndex = m_groups.indexOfIf([&](const Group& g) { return g.name == groupName; });
        if (groupIndex >= 0) {
            m_groups[groupIndex].stateNames.add(stateName);
        }
        else {
            m_groups.add(Group{ groupName, { stateName }, -1 });
        }
    }

    void gotoState(const StringView& stateName) {
        for (auto& g : m_groups) {
            bool stateFound = false;
            for (int i = 0; i < g.stateNames.size(); i++) {
                if (g.stateNames[i] == stateName) {
                    if (g.activeStateIndex != i) {
                        g.activeStateIndex = i;
                        m_dirty = true;
                    }
                    stateFound = true;
                }
            }
            // if (!stateFound) {
            //     // use default
            //     g.activeStateIndex = -1;
            //     m_dirty = true;
            // }
        }

        if (m_dirty) {
            m_ownerElement->invalidate(detail::UIElementDirtyFlags::Style, true);
        }
    }

    // UIStyle* combineStyle(const UIStyleContext* styleContext, const ln::String& elementName);
    void combineStyle(UIStyle* style, const UIStyleContext* styleContext, const ln::String& elementName, const List<String>* classList) const;
    void combineStyle(UIStyle* style, const UIStyleClass* styleClass) const;
    // detail::UIStyleInstance* resolveStyle(const UIStyleContext* styleContext, const ln::String& className);

    void printActive() {
        for (auto& g : m_groups) {
            std::cout << g.name << ": " << g.stateNames[g.activeStateIndex] << std::endl;
        }
    }

    LN_CONSTRUCT_ACCESS : UIVisualStateManager();
    void init(UIElement* ownerElement);

private:
    bool isDirty() const { return m_dirty; }
    void clearDirty() { m_dirty = false; }

    UIElement* m_ownerElement;
    List<Group> m_groups;
    Ref<UIStyle> m_combinedStyle;
    Ref<detail::UIStyleInstance> m_resolvedStyle;
    bool m_dirty;

    // friend class UIElement;
};

enum class UIThemeConstantPalette {
    // Background
    DefaultBackgroundColor, // 最奥。Dark なら、最も暗い色。FrameWindow の背景色
    ControlBackgroundColor, // Leaf コントロールの背景色。Button など
    PaperBackgroundColor,

    // Intentions
    DefaultMainColor,
    DefaultTextColor,
    PrimaryMainColor,
    PrimaryTextColor,
    SecondaryMainColor,
    SecondaryTextColor,
    ErrorMainColor,
    ErrorTextColor,
    WarningMainColor,
    WarningTextColor,
    InfoMainColor,
    InfoTextColor,
    SuccessMainColor,
    SuccessTextColor,

    // Items
    ItemHoverAction,
    ItemSelectedAction,

    // Divider
    DefaultDivider,
};

class UITheme
    : public Object {
public:
    void add(const StringView& name, const Color& color);

    // ない場合は透明 (0, 0, 0, 0)
    const Color& get(const StringView& name) const;

    void setSpacing(float value) { m_spacing = value; }

    // 1: Control 間の最小スペース, ListItem などの Container と Content の余白
    float spacing(float factor) const { return m_spacing * factor; }

    // Control の最小高さ
    float lineContentHeight() const { return m_lineContentHeight; }

    // 行と行の間。ListItem の高さ。
    float lineSpacing() const { return m_lineSpacing; }

    void setColor(UIThemeConstantPalette palette, const Color& color) { m_constantPalette[static_cast<int>(palette)] = color; }
    const Color& color(UIThemeConstantPalette palette) const { return m_constantPalette[static_cast<int>(palette)]; }

    void buildLumitelier();
    void setDefaultStyle(UIStyle* value) { m_defaultStyle = value; }
    const Ref<UIStyle>& defaultStyle() const { return m_defaultStyle; }
    const Ref<UIStyleSheet>& styleSheet() const { return m_styleSheet; }

    LN_CONSTRUCT_ACCESS : UITheme();
    void init();

private:
    std::unordered_map<String, Color> m_colors;
    float m_spacing;
    float m_lineSpacing;
    float m_lineContentHeight = 8 * 4;
    std::array<Color, 20> m_constantPalette;
    Ref<UIStyle> m_defaultStyle;
    Ref<UIStyleSheet> m_styleSheet;
};

class UIVisualStates {
public:
    // Group
    static const String CommonStates;
    static const String FocusStates;
    static const String Visibility;
    static const String CheckStates;
    static const String ValidationStates;
    static const String SelectionStates;
    static const String DisplayStates;

    // CommonStates
    static const String Normal;
    static const String MouseOver;
    static const String Pressed;
    static const String Disabled;

    // FocusStates
    static const String Focused; // Input focus を持っているか. (Logical focus は関係ない)
    static const String Unfocused;

    // VisibilityGroup
    static const String Visible;
    static const String Hidden;
    static const String Collapsed;

    // CheckStates
    static const String CheckedState;
    static const String UncheckedState;

    // SelectionStates
    static const String Unselected;
    static const String Selected;

    // DisplayStates
    static const String Opend;
    static const String Closed;
};

} // namespace ln
