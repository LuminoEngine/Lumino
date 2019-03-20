#pragma once
#include "UIElement.hpp"

namespace ln {
class Shader;
class Texture;
namespace detail {

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
	//ToneF tone;




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
	detail::UIStyleAttribute<Color> backgroundColor;
	detail::UIStyleAttribute<Ref<Texture>> backgroundImage;
	detail::UIStyleAttribute<Ref<Shader>> backgroundShader;
	// ※ここは Material にはしない。そういった大きなクラスを持たせるとまた Brush の時みたいな問題が出てくる。代わりに Material の構築に必要なものを持たせる

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
    detail::UIStyleAttribute<ToneF> tone;

public:	// TODO: internal
	void setupDefault();
	static void updateStyleDataHelper(UIStyle* localStyle, const detail::StyleData* parentStyleData, const UIStyle* defaultStyle, detail::StyleData* outStyleData);

LN_CONSTRUCT_ACCESS:
    UIStyle();
    virtual ~UIStyle();
	void init();

private:
};

} // namespace ln

