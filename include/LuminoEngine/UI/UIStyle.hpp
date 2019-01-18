#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "UIElement.hpp"

namespace ln {
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

        bool inherit = ((int)m_source < ((int)parent.m_source - 1));
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
        if (inherit)
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
    detail::UIStyleAttribute<Thickness> margin;
    detail::UIStyleAttribute<Thickness> padding;

    detail::UIStyleAttribute<Vector3> position;
    detail::UIStyleAttribute<Quaternion> rotation;
    detail::UIStyleAttribute<Vector3> scale;
    detail::UIStyleAttribute<Vector3> centerPoint;

    detail::UIStyleAttribute<float> visible;
    detail::UIStyleAttribute<BlendMode> blendMode;

    detail::UIStyleAttribute<float> opacity;
    detail::UIStyleAttribute<Color> colorScale;
    detail::UIStyleAttribute<Color> blendColor;
    detail::UIStyleAttribute<ToneF> tone;

LN_CONSTRUCT_ACCESS:
    UIStyle();
    virtual ~UIStyle();
	void initialize();

private:
};

} // namespace ln

