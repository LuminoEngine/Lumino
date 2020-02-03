#pragma once
#include "UIElement.hpp"

namespace ln {
class Shader;
class Texture;
class UIStyleContext;
class UIStyleClass;
class UITheme;
namespace detail {
class UIStyleInstance;
class UIStyleClassInstance;

enum class UIStyleAttributeValueSource
{
    //InMerging,		// not set
    Default,		// not set
    //InheritParent,	// 親 UIElement の Style から継承していることを示す
    //InheritBaseOn,
    //InheritBase,	// VisualState に対応するスタイルは、共通の基準スタイルを持つ (HTML の a:hover は a が基準スタイル)。その基準スタイルから継承しているか
    ByStyleInherit,		// VisualState ごとのスタイルから継承しているか
    ByUserLocal,		// 直接設定された値であるか
};

template<typename T>
class UIStyleAttribute
{
public:
    UIStyleAttribute()
        : m_value()
        , m_source(UIStyleAttributeValueSource::Default)
    {}

    void reset()
    {
        m_value = T();
        m_source = UIStyleAttributeValueSource::Default;
    }

    operator const T&() const
    {
        return m_value;
    }

    UIStyleAttribute& operator=(const T& v)
    {
        set(v);
        return *this;
    }

    void set(const T& v)
    {
        m_value = v;
        m_source = UIStyleAttributeValueSource::ByUserLocal;
    }
    const T& get() const
    {
        assert(hasValue());
        return m_value;
    }
    const T& getOrDefault(const T& defaultValue) const
    {
        if (hasValue()) {
            return m_value;
        }
        else {
            return defaultValue;
        }
    }

    bool hasValue() const
    {
        return m_source != UIStyleAttributeValueSource::Default;
    }

    bool inherit(const UIStyleAttribute& parent/*, UIStyleAttributeInheritSourceType sourceType*/)
    {
        // そもそも parent が値を持っていなければ何もする必要はない
        if (parent.m_source == UIStyleAttributeValueSource::Default) return false;
        // Local 値を持っているので継承する必要はない
        if (m_source == UIStyleAttributeValueSource::ByUserLocal) return false;

        //bool inherit = ((int)m_source < ((int)parent.m_source - 1));
        //bool inherit = false;
        //if (m_source == UIStyleAttributeValueSource::Default)
        //{
        //	inherit = true;
        //}
        //else if (m_source == UIStyleAttributeValueSource::InheritParent)
        //{
        //	if (sourceType == UIStyleAttributeInheritSourceType::ParentElement ||
        //		sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle ||
        //		sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
        //		sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
        //	{
        //		inherit = true;
        //	}
        //}
        //else if (m_source == UIStyleAttributeValueSource::InheritBaseOn)
        //{
        //	if (sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle ||
        //		sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
        //		sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
        //	{
        //		inherit = true;
        //	}
        //}
        //else if (m_source == UIStyleAttributeValueSource::InheritBase)
        //{
        //	if (sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
        //		sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
        //	{
        //		inherit = true;
        //	}
        //}
        //else if (m_source == UIStyleAttributeValueSource::StyleLocal)
        //{
        //	if (sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
        //	{
        //		inherit = true;
        //	}
        //}

        bool changed = false;
        //if (inherit)
        {
            //if (sourceType == UIStyleAttributeInheritSourceType::ParentElement)
            //	m_source = UIStyleAttributeValueSource::InheritParent;
            //else if (sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle)
            //	m_source = UIStyleAttributeValueSource::InheritBaseOn;
            //else if (sourceType == UIStyleAttributeInheritSourceType::BaseStyle)
            //	m_source = UIStyleAttributeValueSource::InheritBase;
            //else
            //	m_source = UIStyleAttributeValueSource::StyleLocal;
            m_source = UIStyleAttributeValueSource::ByStyleInherit;
            changed = (m_value != parent.m_value);
            m_value = parent.m_value;
        }
        return changed;
    }

    T							m_value;
    UIStyleAttributeValueSource	m_source;
};

} // namespace detail


enum class UIOverflowBehavior
{
	/** 内容がボックスに収まらない場合、ボックスからはみ出して表示する。 */
	Visible,
	
	/** 内容がボックスに収まらない場合、収まらない部分は非表示とする。(スクロールバーは表示されない) */
	Hidden,
	
	/** 内容がボックスに収まらない場合、スクロールバーを表示する。 */
	Scroll,
};

// 装飾対象の padding の影響を受けない
class UIStyleDecorator
	: public Object
{
public:	// TODO: internal
	float m_width = std::numeric_limits<float>::quiet_NaN();
	float m_height = std::numeric_limits<float>::quiet_NaN();
	Thickness m_margin;
	HAlignment m_hAlignment = HAlignment::Center;
	VAlignment m_vAlignment = VAlignment::Center;

	// TODO: image

	Color m_color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	Ref<Font> m_font;	// decorator の font は継承関係を作らない。そのためプロパティを分ける必要はない
	uint32_t m_codePoint = 0;


	void setIconName(const StringRef& value, int size);

	void render(UIRenderingContext* context, const Size& slotRect);

LN_CONSTRUCT_ACCESS:
	UIStyleDecorator();
	void init();


private:
};

class UIStyle
    : public Object
{
public:

	//// layout
	//Thickness margin;
	//Thickness padding;
	//HAlignment horizontalAlignment;
	//VAlignment verticalAlignment;
	//float minWidth;
	//float minHeight;
	//float maxWidth;
	//float maxHeight;

	//// layout transform
	//Vector3 position;
	//Quaternion rotation;
	//Vector3 scale;
	//Vector3 centerPoint;

	//// text
	//Color textColor;
	//String fontFamily;
	//float fontSize;
	//UIFontWeight fontWeight;
	//UIFontStyle fontStyle;

	//// render effects
	//bool visible;
	//BlendMode blendMode;

	//float opacity;
	//Color colorScale;
	//Color blendColor;
	//ColorTone tone;




	// layout
    detail::UIStyleAttribute<float> width;
    detail::UIStyleAttribute<float> height;
    detail::UIStyleAttribute<Thickness> margin;
    detail::UIStyleAttribute<Thickness> padding;
	detail::UIStyleAttribute<HAlignment> horizontalAlignment;
	detail::UIStyleAttribute<VAlignment> verticalAlignment;
    detail::UIStyleAttribute<HAlignment> horizontalContentAlignment;    // UILayoutPanel をどのように配置するか
    detail::UIStyleAttribute<VAlignment> verticalContentAlignment;
	detail::UIStyleAttribute<float> minWidth;
	detail::UIStyleAttribute<float> minHeight;
	detail::UIStyleAttribute<float> maxWidth;
	detail::UIStyleAttribute<float> maxHeight;
	detail::UIStyleAttribute<UIOverflowBehavior> overflowX;
	detail::UIStyleAttribute<UIOverflowBehavior> overflowY;

    static const float DefaultWidth;
    static const float DefaultHeight;
	static const Thickness DefaultMargin;
	static const Thickness DefaultPadding;
	static const HAlignment DefaultHorizontalAlignment;
	static const VAlignment DefaultVerticalAlignment;
	static const HAlignment DefaultHorizontalContentAlignment;
	static const VAlignment DefaultVerticalContentAlignment;
	static const float DefaultMinWidth;
	static const float DefaultMinHeight;
	static const float DefaultMaxWidth;
	static const float DefaultMaxHeight;
	static const UIOverflowBehavior DefaultOverflowX;
	static const UIOverflowBehavior DefaultOverflowY;

	// transform
    detail::UIStyleAttribute<Vector3> position;
    detail::UIStyleAttribute<Quaternion> rotation;
    detail::UIStyleAttribute<Vector3> scale;
    detail::UIStyleAttribute<Vector3> centerPoint;

	static const Vector3 DefaultPosition;
	static const Quaternion DefaultRotation;
	static const Vector3 DefaultScale;
	static const Vector3 DefaultCenterPoint;

	// background
    detail::UIStyleAttribute<BrushImageDrawMode> backgroundDrawMode;
	detail::UIStyleAttribute<Color> backgroundColor;
	detail::UIStyleAttribute<Ref<Texture>> backgroundImage;
	detail::UIStyleAttribute<Ref<Shader>> backgroundShader;
	// ※↑ここは Material にはしない。そういった大きなクラスを持たせるとまた Brush の時みたいな問題 (アニメーションや、プロパティのパス指定) が出てくる。代わりに Material の構築に必要なものを持たせる
    detail::UIStyleAttribute<Rect> backgroundImageRect;
    detail::UIStyleAttribute<Thickness> backgroundImageBorder;

	static const BrushImageDrawMode DefaultBackgroundDrawMode;
	static const Color DefaultBackgroundColor;
	static const Ref<Texture> DefaultBackgroundImage;
	static const Ref<Shader> DefaultBackgroundShader;
	static const Rect DefaultBackgroundImageRect;
	static const Thickness DefaultBackgroundImageBorder;

    // border
    detail::UIStyleAttribute<Thickness>		borderThickness;
    detail::UIStyleAttribute<CornerRadius>		cornerRadius;
    detail::UIStyleAttribute<Color>				leftBorderColor;	// TODO: borderLeftColor
    detail::UIStyleAttribute<Color>				topBorderColor;
    detail::UIStyleAttribute<Color>				rightBorderColor;
    detail::UIStyleAttribute<Color>				bottomBorderColor;
    detail::UIStyleAttribute<BorderDirection>	borderDirection;
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
	detail::UIStyleAttribute<Color> textColor;	// (default: Black)
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

	static const UIVisibility DefaultVisible;
	static const BlendMode DefaultBlendMode;
	static const float DefaultOpacity;
	static const Color DefaultColorScale;
	static const Color DefaultBlendColor;
	static const ColorTone DefaultTone;

	// decorators
	List<Ref<UIStyleDecorator>> decorators;


    void setBorderColor(const Color& color)
    {
        leftBorderColor = color;
        topBorderColor = color;
        rightBorderColor = color;
        bottomBorderColor = color;
    }

public:	// TODO: internal
	void setupDefault();
	void reset();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyle* other);

LN_CONSTRUCT_ACCESS:
    UIStyle();
    virtual ~UIStyle();
	void init();

private:
};

// 要素ひとつに対応。複数の class を管理する
class UIStyleSet
    : public Object
{
public:
    const String& elementName() const { return m_elementName; }
    //void setClassName(const StringRef& value) { m_className = value; }

	UIStyleClass* mainStyleClass() const { return m_mainStyleClass; }
    void setMainStyleClass(UIStyleClass* value) { m_mainStyleClass = value; }

    void addStyleClass(UIStyleClass* styleClass);
	UIStyleClass* addStyleClass(const StringRef& className);
	UIStyleClass* findStyleClass(const StringRef& className) const;

    // TODO: internal
	void copyFrom(const UIStyleSet* other);
    void mergeFrom(const UIStyleSet* other);

LN_CONSTRUCT_ACCESS:
	UIStyleSet();
    virtual ~UIStyleSet();
    void init(const StringRef& elementName);

private:
	String m_elementName;
	Ref<UIStyleClass> m_mainStyleClass;
	List<Ref<UIStyleClass>> m_styleClasses;

    friend class detail::UIStyleClassInstance;
    friend class UIStyleContext;
};

// 要素ひとつ、class ひとつに対応。VisualState を管理する
class UIStyleClass
	: public Object
{
public:
	const String& name() const { return m_name; }

	UIStyle* mainStyle() const { return m_mainStyle; }
	void setMainStyle(UIStyle* value) { m_mainStyle = value; }

	void addStateStyle(const StringRef& stateName, UIStyle* style);
	UIStyle* addStateStyle(const StringRef& stateName);
	UIStyle* findStateStyle(const StringRef& stateName) const;

	// TODO: internal
	void copyFrom(const UIStyleClass* other);
	void mergeFrom(const UIStyleClass* other);

LN_CONSTRUCT_ACCESS:
	UIStyleClass();
	virtual ~UIStyleClass();
	void init(const StringRef& name);

private:
	struct VisualStateSlot
	{
		String name;
		Ref<UIStyle> style;
	};

	String m_name;
	Ref<UIStyle> m_mainStyle;
	List<VisualStateSlot> m_visualStateStyles;

	friend class detail::UIStyleClassInstance;
	friend class UIStyleContext;
};

class UIStyleSheet
    : public Object
{
public:
    void addStyleSet(const StringRef& elementName, UIStyleSet* styleClass);
    Ref<UIStyleSet> addStyleSet(const StringRef& elementName);
    UIStyleSet* findStyleSet(const StringRef& elementName) const;

	UIStyle* obtainStyle(const StringRef& selector);
    
LN_CONSTRUCT_ACCESS:
    UIStyleSheet();
    virtual ~UIStyleSheet();
    void init();

private:
    std::unordered_map<String, Ref<UIStyleSet>> m_classes;

    friend class UIStyleContext;
};

class UIStyleContext
    : public Object
{
public:
    void addStyleSheet(UIStyleSheet* sheet);
    UIStyleSet* globalStyle() const { return m_globalStyle; }

    // TODO: internal
	// find を高速にするため、事前に確定できる継承関係を解決する。
    void build();
    UIStyleSet* findStyleSet(const StringRef& elementName) const; // 無い場合は null
    //detail::UIStyleClassInstance* findResolvedStyleClass(const StringRef& elementName) const; // 無い場合は global
	// VisualState 以外を結合 (mainStyle のみ結合)
	void combineStyle(UIStyle* style, const StringRef& elementName, const List<String>* classList) const;

	Ref<UITheme> mainTheme;


	//const Color& defaultTextColor() const { return Color::White; }
	//const String& defaultFontFamily() const { return; }
	//float defaultFontSize() const { return 12.0f; }
	//UIFontWeight DefaultFontWeight() const { return UIFontWeight::Normal; }
	//UIFontStyle DefaultFontStyle() const { return UIFontStyle::Normal; }

LN_CONSTRUCT_ACCESS:
    UIStyleContext();
    virtual ~UIStyleContext();
    void init();

private:
    List<Ref<UIStyleSheet>> m_styleSheets;
    Ref<UIStyleSet> m_globalStyle;
    std::unordered_map<String, Ref<UIStyleSet>> m_elementStyles;

	// TODO: つかわないかも
    //Ref<detail::UIStyleClassInstance> m_resolvedGlobalStyle;                            // for unused VisualStateManager
    //std::unordered_map<String, Ref<detail::UIStyleClassInstance>> m_resolvedClasses;    // for unused VisualStateManager
};

namespace detail {

// どんな Element でも持つ一般的なスタイル値。
// UIStyle の optional 等を解決したもの。
// メモリ消費を抑えるため、UIStyleAttribute は使わないようにしている。
class UIStyleInstance
    : public RefObject
{
public:
    //UIStyle* sourceLocalStyle = nullptr;	// 以下のデータの生成元となったローカスのスタイル

    // layout
    float width;
    float height;
    Thickness margin;
    Thickness padding;
    HAlignment horizontalAlignment;
    VAlignment verticalAlignment;
    HAlignment horizontalContentAlignment;
    VAlignment verticalContentAlignment;
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
    BrushImageDrawMode backgroundDrawMode;
    Color backgroundColor;
    Ref<Texture> backgroundImage;
    Ref<Shader> backgroundShader;
    Rect backgroundImageRect;
    Thickness backgroundImageBorder;

    // border
    Thickness borderThickness = Thickness::Zero;
    CornerRadius cornerRadius = CornerRadius(0, 0, 0, 0);	// TODO: borderRadius
    Color 		leftBorderColor = Color::Gray;
    Color 		topBorderColor = Color::Gray;
    Color 		rightBorderColor = Color::Gray;
    Color 		bottomBorderColor = Color::Gray;
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

    float opacity;
    Color colorScale;
    Color blendColor;
	ColorTone tone;


	// decorators
	List<Ref<UIStyleDecorator>> decorators;

    // commited cache
    Ref<Font> font;
    Ref<AbstractMaterial> backgroundMaterial;

	UITheme* theme = nullptr;

    // TODO: 今後サブクラスごとにスタイルを追加する場合は、ここに map を設ける

    UIStyleInstance();
    void setupDefault();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyleInstance* other);

    void makeRenderObjects();

    static void updateStyleDataHelper(const UIStyleContext* context, const detail::UIStyleInstance* parentStyleData, const UIStyle* combinedStyle, detail::UIStyleInstance* outStyleData);

	Size actualOuterSpace() const
	{
		return Size(margin.width(), margin.height());
		//if (borderInset)
		//	return Size(margin.width(), margin.height());
		//else
		//	return Size(margin.width() + borderThickness.width(), margin.height() + borderThickness.height());
	}
	Size actualOuterOffset() const
	{
		return Size(margin.left, margin.top);
	}

LN_CONSTRUCT_ACCESS:

private:

};

class UIStyleClassInstance
    : public RefObject
{
public:
    UIStyleClassInstance();
    const Ref<UIStyleInstance>& style() const { return m_style; }
    UIStyleInstance* findStateStyle(const StringRef& stateName) const;  // 無い場合は nullptr
    UIStyleInstance* findSubElementStyle(const StringRef& elementName) const;   // 無い場合は nullptr
    void mergeFrom(const UIStyleSet* other);

private:
    struct VisualStateSlot
    {
        String name;
        Ref<UIStyleInstance> style;
    };

    struct SubElementSlot
    {
        String name;
        Ref<UIStyleInstance> style;
    };

    Ref<UIStyleInstance> m_style;
    List<VisualStateSlot> m_visualStateStyles;
    List<SubElementSlot> m_subElements;

    friend class UIStyleContext;
};

} // namespace detail



class UIVisualStateManager
    : public Object
{
public:
    struct Group
    {
        String name;
        List<String> stateNames;
        int activeStateIndex;   // -1: no-state
    };

    void registerState(const StringRef& groupName, const StringRef& stateName)
    {
        int groupIndex = m_groups.indexOfIf([&](const Group& g) { return g.name == groupName; });
        if (groupIndex >= 0) {
           m_groups[groupIndex].stateNames.add(stateName);
        }
        else {
            m_groups.add(Group{ groupName, { stateName }, -1 });
        }
    }

    void gotoState(const StringRef& stateName)
    {
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
            //if (!stateFound) {
            //    // use default
            //    g.activeStateIndex = -1;
            //    m_dirty = true;
            //}
        }

        if (m_dirty) {
            m_ownerElement->invalidate(detail::UIElementDirtyFlags::Style, true);
        }
    }

    //UIStyle* combineStyle(const UIStyleContext* styleContext, const ln::String& elementName);
	void combineStyle(UIStyle* style, const UIStyleContext* styleContext, const ln::String& elementName, const List<String>* classList);
    void combineStyle(UIStyle* style, const UIStyleClass* styleClass);
    //detail::UIStyleInstance* resolveStyle(const UIStyleContext* styleContext, const ln::String& className);

LN_CONSTRUCT_ACCESS:
    UIVisualStateManager();
    void init(UIElement* ownerElement);

private:
    bool isDirty() const { return m_dirty; }
    void clearDirty() { m_dirty = false; }

    UIElement* m_ownerElement;
    List<Group> m_groups;
    Ref<UIStyle> m_combinedStyle;
    Ref<detail::UIStyleInstance> m_resolvedStyle;
    bool m_dirty;

    //friend class UIElement;
};


enum class UIColorHues
{
    Red,
    Pink,
    Purple,
    DeepPurple,
    Indigo,
    Blue,
    LightBlue,
    Cyan,
    Teal,
    Green,
    LightGreen,
    Lime,
    Yellow,
    Amber,
    Orange,
    DeepOrange,
    Brown,
    Grey,
    Black,
    White,
};

class UIColors
{
public:
    static const int MaxShades = 10;

    /** 色を取得します。*/
    static const Color& get(UIColorHues hue, int shades = 5);
};

enum class UIThemeConstantPalette
{
	// Background
	DefaultBackgroundColor,
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

	// Divider
	DefaultDivider,
};

class UITheme
    : public Object
{
public:
    void add(const StringRef& name, const Color& color);

    // ない場合は透明 (0, 0, 0, 0)
    const Color& get(const StringRef& name) const;

    void setSpacing(float value) { m_spacing = value; }

    float spacing(float factor) const { return m_spacing * factor; }

	float lineContentHeight() const { return spacing(4); }

	void setColor(UIThemeConstantPalette palette, const Color& color) { m_constantPalette[static_cast<int>(palette)] = color; }
	const Color& color(UIThemeConstantPalette palette) const { return m_constantPalette[static_cast<int>(palette)]; }


	void buildLumitelier();
	void setDefaultStyle(UIStyle* value) { m_defaultStyle = value; }
	const Ref<UIStyle>& defaultStyle() const { return m_defaultStyle; }
	const Ref<UIStyleSheet>& styleSheet() const { return m_styleSheet; }
    
LN_CONSTRUCT_ACCESS:
    UITheme();
	void init();

private:
    std::unordered_map<String, Color> m_colors;
    float m_spacing;
	std::array<Color, 20> m_constantPalette;
	Ref<UIStyle> m_defaultStyle;
	Ref<UIStyleSheet> m_styleSheet;
};

class UIVisualStates
{
public:
	// Group
	static const String CommonStates;
	static const String FocusStates;
	static const String CheckStates;
	static const String ValidationStates;
	static const String SelectionStates;

	// CommonStates
	static const String Normal;
	static const String MouseOver;
	static const String Pressed;
	static const String Disabled;

	// CheckStates
	static const String CheckedState;
	static const String UncheckedState;

	// SelectionStates
	static const String Unselected;
	static const String Selected;
};

} // namespace ln

