
#pragma once
#include <Lumino/Base/SortedArray.h>
#include "../Animation/Common.h"
#include "Common.h"
#include "Detail.h"
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN
class UIStyle;
using UIStylePtr = Ref<UIStyle>;
class UIRenderElement;
class DrawingContext;

enum class UIStyleAttributeValueSource
{
	InMerging,		// not set
	Default,		// not set
	//InheritParent,	// 親 UIElement の Style から継承していることを示す
	//InheritBaseOn,
	//InheritBase,	// VisualState に対応するスタイルは、共通の基準スタイルを持つ (HTML の a:hover は a が基準スタイル)。その基準スタイルから継承しているか
	ByStyleInherit,		// VisualState ごとのスタイルから継承しているか
	ByUserLocal,		// 直接設定された値であるか
};

//enum class UIStyleAttributeInheritSourceType
//{
//	ParentElement,
//	BaseOnStyle,
//	BaseStyle,
//	StyleLocal,
//};

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

class UIRenderElement
	: public Object
{
	LN_OBJECT;
public:
	float			m_width;
	float			m_height;
	Thickness		m_margin;
	HAlignment		m_hAlignment;
	VAlignment		m_vAlignment;
	Ref<Brush>	m_brush;

LN_CONSTRUCT_ACCESS:
	UIRenderElement();
	virtual ~UIRenderElement();
	void initialize();

private:
	void layoutAndRender(DrawingContext* context, const Size& parentRenderSize);

	friend class UIElement;
};



/**
	@brief		
*/
class UIStylePropertyTable
	: public Object
{
	LN_OBJECT;
public:
	//void AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);

LN_INTERNAL_ACCESS:
	UIStylePropertyTable();
	virtual ~UIStylePropertyTable();
	void initialize(const StringRef& visualStateName);

private:
	//void ApplyInternal(UIElement* targetElement, bool useTransitionAnimation);
	//void ApplyInternal(UIElement* targetElement, const UIStyleAttribute& setter, bool useTransitionAnimation);

	String						m_visualStateName;
	//List<UIStyleAttribute>		m_attributes;

	//UIStylePropertyTable*		m_lastInheritedParent;
	//List<UIStyleAttribute*>	m_parentRefAttributes;
public:

	UIStyleAttribute<float>				width;
	UIStyleAttribute<float>				height;

	UIStyleAttribute<Thickness>		margin;
	UIStyleAttribute<Thickness>		padding;

	//UIStyleAttribute<VAlignment>		m_verticalAlignment;
	//UIStyleAttribute<HAlignment>	m_horizontalAlignment;
	UIStyleAttribute<BrushPtr>				background;

	//UIStyleAttribute < Ref<UIRenderElement>>	testDeco;
	
	//UIStyleAttribute<BrushPtr>				backgroundMargin;
	//UIStyleAttribute<BrushPtr>				foreground;
	//UIStyleAttribute<TexturePtr>			m_image;

	//UIStyleAttribute<Ref<Pen>>			m_borderLeft;
	//UIStyleAttribute<Ref<Pen>>			m_borderRight;
	//UIStyleAttribute<Ref<Pen>>			m_borderTop;
	//UIStyleAttribute<Ref<Pen>>			m_borderBottom;
	//UIStyleAttribute<FontPtr>				m_font;

	//UIStyleAttribute<String>				m_fontFamily;
	//UIStyleAttribute<int>					m_fontSize;
	//UIStyleAttribute<bool>					m_fontBold;
	//UIStyleAttribute<bool>					m_fontItalic;

	UIStyleAttribute<Thickness>		borderThickness;
	UIStyleAttribute<CornerRadius>		cornerRadius;
	UIStyleAttribute<Color>				leftBorderColor;
	UIStyleAttribute<Color>				topBorderColor;
	UIStyleAttribute<Color>				rightBorderColor;
	UIStyleAttribute<Color>				bottomBorderColor;
	UIStyleAttribute<BorderDirection>	borderDirection;

	//std::unordered_map<String, UIStyleAttribute<Ref<UIRenderElement>>>	m_renderElementMap;
	List<Ref<UIRenderElement>>	m_renderElements;
};

namespace detail {

class UIStylePropertyTableInstance
	: public RefObject
{
public:
	void clearAvailableRenderElements();
	detail::InvalidateFlags inheritParentElementStyle(UIStylePropertyTableInstance* parent);
	detail::InvalidateFlags merge(const UIStylePropertyTable* source/*, UIStyleAttributeInheritSourceType sourceType*/);
	void apply(UIElement* targetElement, bool useTransitionAnimation);

public:
	UIStyleAttribute<float>				width;
	UIStyleAttribute<float>				height;
	UIStyleAttribute<Thickness>		margin;
	UIStyleAttribute<Thickness>		padding;

	//UIStyleAttribute<VAlignment>		m_verticalAlignment;
	//UIStyleAttribute<HAlignment>	m_horizontalAlignment;
	UIStyleAttribute<BrushPtr>				background;

	//UIStyleAttribute < Ref<UIRenderElement>>	testDeco;

	//UIStyleAttribute<BrushPtr>				backgroundMargin;
	//UIStyleAttribute<BrushPtr>				foreground;
	//UIStyleAttribute<TexturePtr>			m_image;

	//UIStyleAttribute<Ref<Pen>>			m_borderLeft;
	//UIStyleAttribute<Ref<Pen>>			m_borderRight;
	//UIStyleAttribute<Ref<Pen>>			m_borderTop;
	//UIStyleAttribute<Ref<Pen>>			m_borderBottom;
	//UIStyleAttribute<FontPtr>				m_font;

	//UIStyleAttribute<String>				m_fontFamily;
	//UIStyleAttribute<int>					m_fontSize;
	//UIStyleAttribute<bool>					m_fontBold;
	//UIStyleAttribute<bool>					m_fontItalic;

	UIStyleAttribute<Thickness>		borderThickness;
	UIStyleAttribute<CornerRadius>		cornerRadius;
	UIStyleAttribute<Color>				leftBorderColor;
	UIStyleAttribute<Color>				topBorderColor;
	UIStyleAttribute<Color>				rightBorderColor;
	UIStyleAttribute<Color>				bottomBorderColor;
	UIStyleAttribute<BorderDirection>	borderDirection;

	List<Ref<UIRenderElement>>	m_availableRenderElements;

public:
	void readyMergeProcess()
	{
		if (width.m_source != UIStyleAttributeValueSource::ByUserLocal) width.reset();
		if (height.m_source != UIStyleAttributeValueSource::ByUserLocal) height.reset();
		if (margin.m_source != UIStyleAttributeValueSource::ByUserLocal) margin.reset();
		if (padding.m_source != UIStyleAttributeValueSource::ByUserLocal) padding.reset();
		if (background.m_source != UIStyleAttributeValueSource::ByUserLocal) background.reset();
		if (borderThickness.m_source != UIStyleAttributeValueSource::ByUserLocal) borderThickness.reset();
		if (cornerRadius.m_source != UIStyleAttributeValueSource::ByUserLocal) cornerRadius.reset();
		if (leftBorderColor.m_source != UIStyleAttributeValueSource::ByUserLocal) leftBorderColor.reset();
		if (topBorderColor.m_source != UIStyleAttributeValueSource::ByUserLocal) topBorderColor.reset();
		if (rightBorderColor.m_source != UIStyleAttributeValueSource::ByUserLocal) rightBorderColor.reset();
		if (bottomBorderColor.m_source != UIStyleAttributeValueSource::ByUserLocal) bottomBorderColor.reset();
		if (borderDirection.m_source != UIStyleAttributeValueSource::ByUserLocal) borderDirection.reset();
	}

	friend class UIStyle;
};

} // namespace detail

/**
	@brief		
*/
class UIStyle
	: public Object
{
	LN_OBJECT;
public:
	static UIStylePtr create();

public:
	UIStyle();
	virtual ~UIStyle();
	void initialize();

	//void AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);


	//void AddValue(const StringRef& visualStateName, const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);
	
	// e.g) ScrollBar::Horizontal { ... }
	//void AddSubStateStyle(const StringRef& subStateName, UIStyle* style);
	//UIStyle* FindSubStateStyle(const StringRef& subStateName);

	UIStylePropertyTable* getPropertyTable();
	UIStylePropertyTable* getPropertyTable(const StringRef& visualStateName);

	void setBaseOnStyle(UIStyle* style);

LN_INTERNAL_ACCESS:
	UIStylePropertyTable* findStylePropertyTable(const String& visualStateName);
	//detail::InvalidateFlags UpdateInherit(UIStyle* parent);
	//void apply(UIElement* targetElement);

	detail::InvalidateFlags mergeActiveStylePropertyTables(detail::UIStylePropertyTableInstance* store, const List<String>& visualStateNames);

public:	// TODO:
	using VisualStateStylePair = std::pair<String, Ref<UIStylePropertyTable>>;
	using SubStateStylePair = std::pair<String, Ref<UIStyle>>;


	Ref<UIStyle>	m_baseOn;

	Ref<UIStylePropertyTable>	m_basePropertyTable;

	//SortedArray<String, Ref<UIStylePropertyTable>>	m_propertyTableMap;
	List<VisualStateStylePair>	m_visualStatePropertyTableList;

	//UIStyle*					m_subStyleParent;
	//List<SubStateStylePair>		m_subStateStyles;



	//UIStyle*	m_lastUpdateParent;
	//int			m_revisionCount;



};

/**
	@brief		
*/
class UIStyleTable
	: public Object
{
	LN_OBJECT;
public:
	UIStyleTable();
	virtual ~UIStyleTable();

	//void AddStyle(const tr::TypeInfo* targetType, UIStyle* style);
	//void AddStyle(const tr::TypeInfo* targetType, const StringRef& subStateName, UIStyle* style);


	UIStyle* getStyle(const StringRef& typeName);
	UIStyle* getSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName);
	//UIStyle* getStyle(const tr::TypeInfo* targetType, const StringRef& subStateName);

LN_INTERNAL_ACCESS:

	// 見つからなければ nullptr (ベースクラス情報も使用して検索する)
	UIStyle* findStyle(const tr::TypeInfo* targetType/*, const StringRef& subControlOwnerName, const StringRef& subControlName*/);
	UIStyle* findSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName);
	//UIStyle* findStyle(const tr::TypeInfo* targetType, const StringRef& subStateName);

private:	
	//typedef const tr::TypeInfo* StyleKey;
	using StyleKey = size_t;

	SortedArray<StyleKey, Ref<UIStyle>>	m_table;
	SortedArray<StyleKey, Ref<UIStyle>>	m_subControlStyleTable;
};

enum class UIColorIndex
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

/**
	@brief
*/
class UIColors
{
public:
	static const int MaxDepth = 10;
	static const int MaxIndex = 20;

	/** 色を取得します。*/
	static const Color& getColor(UIColorIndex index, int depth = 5);

	/** SolidColorBrush を取得します。*/
	static SolidColorBrush* getBrush(UIColorIndex index, int depth = 5);
};

LN_NAMESPACE_END
