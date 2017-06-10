
#pragma once
#include "../Animation/Common.h"
#include "Common.h"
#include "Detail.h"
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN
class UIStyle;
using UIStylePtr = RefPtr<UIStyle>;
class UIRenderElement;
class DrawingContext;

enum class UIStyleAttributeValueSource
{
	Default,		// not set
	InheritParent,	// 親 UIElement の Style から継承していることを示す
	InheritBaseOn,
	InheritBase,	// VisualState に対応するスタイルは、共通の基準スタイルを持つ (HTML の a:hover は a が基準スタイル)。その基準スタイルから継承しているか
	StyleLocal,		// VisualState ごとのスタイルから継承しているか
	UserLocal,		// 直接設定された値であるか
};

enum class UIStyleAttributeInheritSourceType
{
	ParentElement,
	BaseOnStyle,
	BaseStyle,
	StyleLocal,
};

template<typename T>
class UIStyleAttribute
{
public:
	UIStyleAttribute()
		: m_value()
		, m_source(UIStyleAttributeValueSource::Default)
	{}

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
		m_source = UIStyleAttributeValueSource::UserLocal;
	}
	const T& get() const
	{
		return m_value;
	}

	bool hasValue() const
	{
		return m_source != UIStyleAttributeValueSource::Default;
	}

	bool inherit(const UIStyleAttribute& parent, UIStyleAttributeInheritSourceType sourceType)
	{
		// そもそも parent が値を持っていなければ何もする必要はない
		if (parent.m_source == UIStyleAttributeValueSource::Default) return false;
		// Local 値を持っているので継承する必要はない
		if (m_source == UIStyleAttributeValueSource::UserLocal) return false;

		bool inherit = false;
		if (m_source == UIStyleAttributeValueSource::Default)
		{
			inherit = true;
		}
		else if (m_source == UIStyleAttributeValueSource::InheritParent)
		{
			if (sourceType == UIStyleAttributeInheritSourceType::ParentElement ||
				sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle ||
				sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
				sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
			{
				inherit = true;
			}
		}
		else if (m_source == UIStyleAttributeValueSource::InheritBaseOn)
		{
			if (sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle ||
				sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
				sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
			{
				inherit = true;
			}
		}
		else if (m_source == UIStyleAttributeValueSource::InheritBase)
		{
			if (sourceType == UIStyleAttributeInheritSourceType::BaseStyle ||
				sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
			{
				inherit = true;
			}
		}
		else if (m_source == UIStyleAttributeValueSource::StyleLocal)
		{
			if (sourceType == UIStyleAttributeInheritSourceType::StyleLocal)
			{
				inherit = true;
			}
		}

		bool changed = false;
		if (inherit)
		{
			if (sourceType == UIStyleAttributeInheritSourceType::ParentElement)
				m_source = UIStyleAttributeValueSource::InheritParent;
			else if (sourceType == UIStyleAttributeInheritSourceType::BaseOnStyle)
				m_source = UIStyleAttributeValueSource::InheritBaseOn;
			else if (sourceType == UIStyleAttributeInheritSourceType::BaseStyle)
				m_source = UIStyleAttributeValueSource::InheritBase;
			else
				m_source = UIStyleAttributeValueSource::StyleLocal;

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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	float		m_width;
	float		m_height;
	ThicknessF	m_margin;
	HAlignment	m_hAlignment;
	VAlignment	m_vAlignment;

LN_CONSTRUCT_ACCESS:
	UIRenderElement();
	virtual ~UIRenderElement();
	void Initialize();

private:
	void LayoutAndRender(DrawingContext* context, const Size& parentRenderSize);

	friend class UIElement;
};



/**
	@brief		
*/
class UIStylePropertyTable
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	//void AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);

LN_INTERNAL_ACCESS:
	UIStylePropertyTable();
	virtual ~UIStylePropertyTable();
	void Initialize(const StringRef& visualStateName);
	detail::InvalidateFlags InheritParentElementStyle(UIStylePropertyTable* parent);
	detail::InvalidateFlags Merge(const UIStylePropertyTable* source, UIStyleAttributeInheritSourceType sourceType);
	void Apply(UIElement* targetElement, bool useTransitionAnimation);

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

	//UIStyleAttribute<ThicknessF>			m_margin;
	//UIStyleAttribute<ThicknessF>			m_padding;
	//UIStyleAttribute<VAlignment>		m_verticalAlignment;
	//UIStyleAttribute<HAlignment>	m_horizontalAlignment;
	UIStyleAttribute<BrushPtr>				background;

	UIStyleAttribute < RefPtr<UIRenderElement>>	testDeco;
	
	//UIStyleAttribute<BrushPtr>				backgroundMargin;
	//UIStyleAttribute<BrushPtr>				foreground;
	//UIStyleAttribute<TexturePtr>			m_image;

	//UIStyleAttribute<RefPtr<Pen>>			m_borderLeft;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderRight;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderTop;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderBottom;
	//UIStyleAttribute<FontPtr>				m_font;

	//UIStyleAttribute<String>				m_fontFamily;
	//UIStyleAttribute<int>					m_fontSize;
	//UIStyleAttribute<bool>					m_fontBold;
	//UIStyleAttribute<bool>					m_fontItalic;

	// TODO: Property はちょっとサイズ大きいので、SimpleProperty とか変更通知持たないのを作りたい
	tr::Property<ThicknessF>		borderThickness;
	tr::Property<CornerRadius>		cornerRadius;
	tr::Property<Color>				leftBorderColor;
	tr::Property<Color>				topBorderColor;
	tr::Property<Color>				rightBorderColor;
	tr::Property<Color>				bottomBorderColor;
	tr::Property<BorderDirection>	borderDirection;

	//std::unordered_map<String, UIStyleAttribute<RefPtr<UIRenderElement>>>	m_renderElementMap;
};

/**
	@brief		
*/
class UIStyle
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static UIStylePtr Create();

public:
	UIStyle();
	virtual ~UIStyle();
	void Initialize();

	//void AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);


	//void AddValue(const StringRef& visualStateName, const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time = 0.0, EasingMode easingMode = EasingMode::Linear);
	
	// e.g) ScrollBar::Horizontal { ... }
	//void AddSubStateStyle(const StringRef& subStateName, UIStyle* style);
	//UIStyle* FindSubStateStyle(const StringRef& subStateName);

	UIStylePropertyTable* GetPropertyTable();
	UIStylePropertyTable* GetPropertyTable(const StringRef& visualStateName);

	void SetBaseOnStyle(UIStyle* style);

LN_INTERNAL_ACCESS:
	UIStylePropertyTable* FindStylePropertyTable(const String& visualStateName);
	//detail::InvalidateFlags UpdateInherit(UIStyle* parent);
	//void Apply(UIElement* targetElement);

	detail::InvalidateFlags MergeActiveStylePropertyTables(UIStylePropertyTable* store, const List<String>& visualStateNames);

public:	// TODO:
	using VisualStateStylePair = std::pair<String, RefPtr<UIStylePropertyTable>>;
	using SubStateStylePair = std::pair<String, RefPtr<UIStyle>>;


	RefPtr<UIStyle>	m_baseOn;

	RefPtr<UIStylePropertyTable>	m_basePropertyTable;

	//SortedArray<String, RefPtr<UIStylePropertyTable>>	m_propertyTableMap;
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIStyleTable();
	virtual ~UIStyleTable();

	//void AddStyle(const tr::TypeInfo* targetType, UIStyle* style);
	//void AddStyle(const tr::TypeInfo* targetType, const StringRef& subStateName, UIStyle* style);


	UIStyle* GetStyle(const StringRef& typeName);
	UIStyle* GetSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName);
	//UIStyle* GetStyle(const tr::TypeInfo* targetType, const StringRef& subStateName);

LN_INTERNAL_ACCESS:

	// 見つからなければ nullptr (ベースクラス情報も使用して検索する)
	UIStyle* FindStyle(const tr::TypeInfo* targetType/*, const StringRef& subControlOwnerName, const StringRef& subControlName*/);
	UIStyle* FindSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName);
	//UIStyle* FindStyle(const tr::TypeInfo* targetType, const StringRef& subStateName);

private:	
	//typedef const tr::TypeInfo* StyleKey;
	using StyleKey = size_t;

	SortedArray<StyleKey, RefPtr<UIStyle>>	m_table;
	SortedArray<StyleKey, RefPtr<UIStyle>>	m_subControlStyleTable;
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
	static const Color& GetColor(UIColorIndex index, int depth = 5);

	/** SolidColorBrush を取得します。*/
	static SolidColorBrush* GetBrush(UIColorIndex index, int depth = 5);
};

LN_NAMESPACE_END
