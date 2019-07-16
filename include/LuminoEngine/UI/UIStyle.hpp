#pragma once
#include "UIElement.hpp"

namespace ln {
class Shader;
class Texture;
class UIStyleContext;
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
    detail::UIStyleAttribute<Thickness> margin;
    detail::UIStyleAttribute<Thickness> padding;
	detail::UIStyleAttribute<HAlignment> horizontalAlignment;
	detail::UIStyleAttribute<VAlignment> verticalAlignment;
	detail::UIStyleAttribute<float> minWidth;
	detail::UIStyleAttribute<float> minHeight;
	detail::UIStyleAttribute<float> maxWidth;
	detail::UIStyleAttribute<float> maxHeight;

	// layout transform
    detail::UIStyleAttribute<Vector3> position;
    detail::UIStyleAttribute<Quaternion> rotation;
    detail::UIStyleAttribute<Vector3> scale;
    detail::UIStyleAttribute<Vector3> centerPoint;

	// background
    detail::UIStyleAttribute<BrushImageDrawMode> backgroundDrawMode;
	detail::UIStyleAttribute<Color> backgroundColor;
	detail::UIStyleAttribute<Ref<Texture>> backgroundImage;
	detail::UIStyleAttribute<Ref<Shader>> backgroundShader;
	// ※↑ここは Material にはしない。そういった大きなクラスを持たせるとまた Brush の時みたいな問題 (アニメーションや、プロパティのパス指定) が出てくる。代わりに Material の構築に必要なものを持たせる
    detail::UIStyleAttribute<Rect> backgroundImageRect;
    detail::UIStyleAttribute<Thickness> backgroundImageBorder;

    // border
    detail::UIStyleAttribute<Thickness>		borderThickness;
    detail::UIStyleAttribute<CornerRadius>		cornerRadius;
    detail::UIStyleAttribute<Color>				leftBorderColor;
    detail::UIStyleAttribute<Color>				topBorderColor;
    detail::UIStyleAttribute<Color>				rightBorderColor;
    detail::UIStyleAttribute<Color>				bottomBorderColor;
    detail::UIStyleAttribute<BorderDirection>	borderDirection;

	// text
	detail::UIStyleAttribute<Color> textColor;	// (default: Black)
	detail::UIStyleAttribute<String> fontFamily;
	detail::UIStyleAttribute<float> fontSize;
	detail::UIStyleAttribute<UIFontWeight> fontWeight;
	detail::UIStyleAttribute<UIFontStyle> fontStyle;

	// render effects
    detail::UIStyleAttribute<bool> visible;
    detail::UIStyleAttribute<BlendMode> blendMode;
    detail::UIStyleAttribute<float> opacity;
    detail::UIStyleAttribute<Color> colorScale;
    detail::UIStyleAttribute<Color> blendColor;
    detail::UIStyleAttribute<ColorTone> tone;

    void setBorderColor(const Color& color)
    {
        leftBorderColor = color;
        topBorderColor = color;
        rightBorderColor = color;
        bottomBorderColor = color;
    }

public:	// TODO: internal
	void setupDefault();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyle* other);

LN_CONSTRUCT_ACCESS:
    UIStyle();
    virtual ~UIStyle();
	void init();

private:
};

class UIStyleClass
    : public Object
{
public:
    //const String& className() const { return m_className; }
    //void setClassName(const StringRef& value) { m_className = value; }

    UIStyle* style() const { return m_style; }
    void setStyle(UIStyle* value) { m_style = value; }

    void addStateStyle(const StringRef& stateName, UIStyle* style);
    UIStyle* findStateStyle(const StringRef& stateName) const;

    void addSubElementStyle(const StringRef& elementName, UIStyle* style);
    UIStyle* findSubElementStyle(const StringRef& elementName) const;

    // TODO: internal
    void mergeFrom(const UIStyleClass* other);

LN_CONSTRUCT_ACCESS:
    UIStyleClass();
    virtual ~UIStyleClass();
    void init();

private:
    struct VisualStateSlot
    {
        String name;
        Ref<UIStyle> style;
    };

    struct SubElementSlot
    {
        String name;
        Ref<UIStyle> style;
    };

    //String m_className;
    Ref<UIStyle> m_style;
    List<VisualStateSlot> m_visualStateStyles;
    List<SubElementSlot> m_subElements;

    friend class detail::UIStyleClassInstance;
    friend class UIStyleContext;
};

class UIStyleSheet
    : public Object
{
public:
    void addStyleClass(const StringRef& className, UIStyleClass* styleClass);
    Ref<UIStyleClass> addStyleClass(const StringRef& className);
    UIStyleClass* findStyleClass(const StringRef& className) const;
    
LN_CONSTRUCT_ACCESS:
    UIStyleSheet();
    virtual ~UIStyleSheet();
    void init();

private:
    std::unordered_map<String, Ref<UIStyleClass>> m_classes;

    friend class UIStyleContext;
};

class UIStyleContext
    : public Object
{
public:
    void addStyleSheet(UIStyleSheet* sheet);


    // TODO: internal
    void build();
    UIStyleClass* findStyleClass(const StringRef& className) const; // 無い場合は global
    detail::UIStyleClassInstance* findResolvedStyleClass(const StringRef& className) const; // 無い場合は global

LN_CONSTRUCT_ACCESS:
    UIStyleContext();
    virtual ~UIStyleContext();
    void init();

private:
    List<Ref<UIStyleSheet>> m_styleSheets;
    Ref<UIStyleClass> m_globalStyle;
    std::unordered_map<String, Ref<UIStyleClass>> m_classes;
    Ref<detail::UIStyleClassInstance> m_resolvedGlobalStyle;                            // for unused VisualStateManager
    std::unordered_map<String, Ref<detail::UIStyleClassInstance>> m_resolvedClasses;    // for unused VisualStateManager
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
    Thickness margin;
    Thickness padding;
    HAlignment horizontalAlignment;
    VAlignment verticalAlignment;
    float minWidth;
    float minHeight;
    float maxWidth;
    float maxHeight;

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
    CornerRadius cornerRadius = CornerRadius(0, 0, 0, 0);
    Color 		leftBorderColor = Color::Gray;
    Color 		topBorderColor = Color::Gray;
    Color 		rightBorderColor = Color::Gray;
    Color 		bottomBorderColor = Color::Gray;
    BorderDirection borderDirection = BorderDirection::Outside;

    // text
    Color textColor;
    String fontFamily;
    float fontSize;
    UIFontWeight fontWeight;
    UIFontStyle fontStyle;

    // render effects
    bool visible;
    BlendMode blendMode;

    float opacity;
    Color colorScale;
    Color blendColor;
	ColorTone tone;



    // commited cache
    Ref<Font> font;
    Ref<AbstractMaterial> backgroundMaterial;

    // TODO: 今後サブクラスごとにスタイルを追加する場合は、ここに map を設ける

    UIStyleInstance();
    void setupDefault();
    void mergeFrom(const UIStyle* other);
    void copyFrom(const UIStyleInstance* other);

    void makeRenderObjects();

    static void updateStyleDataHelper(UIStyle* localStyle, const detail::UIStyleInstance* parentStyleData, const UIStyle* combinedStyle, detail::UIStyleInstance* outStyleData);

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
    void mergeFrom(const UIStyleClass* other);

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
                        std::cout << g.stateNames[i] << std::endl;
                    }
                    stateFound = true;
                }
            }
            if (!stateFound) {
                // use default
                g.activeStateIndex = -1;
                m_dirty = true;
            }
        }
    }

    UIStyle* combineStyle(const UIStyleContext* styleContext, const ln::String& className);
    //detail::UIStyleInstance* resolveStyle(const UIStyleContext* styleContext, const ln::String& className);

LN_CONSTRUCT_ACCESS:
    UIVisualStateManager();
    void init();

private:
    bool isDirty() const { return m_dirty; }
    void clearDirty() { m_dirty = false; }

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

} // namespace ln

