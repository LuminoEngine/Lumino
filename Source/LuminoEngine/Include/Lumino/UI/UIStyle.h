
#pragma once
#include "../Animation/Common.h"
#include "Common.h"
#include "Detail.h"
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN
class UIStyle;
using UIStylePtr = RefPtr<UIStyle>;

#if 1
template<typename T>
class UIStyleAttribute
{
public:
	UIStyleAttribute()
		: value()
		, isSet(false)
		, m_modified(true)
		, m_hasValue(false)
	{}

	UIStyleAttribute(const T& v)
		: value(v)
		, isSet(true)
		, m_modified(true)
		, m_hasValue(true)
	{}

	UIStyleAttribute(const UIStyleAttribute& v)
		: value(v.value)
		, isSet(v.isSet)
		, m_modified(true)
		, m_hasValue(v.m_hasValue)
	{}

	operator const T&() const { return value; }

	UIStyleAttribute& operator=(const T& v)
	{
		value = v;
		isSet = true;
		m_modified = true;
		m_hasValue = true;
		return *this;
	}

	UIStyleAttribute& operator=(const UIStyleAttribute& other)
	{
		value = other.value;
		isSet = other.isSet;
		m_modified = true;
		m_hasValue = other.m_hasValue;
		return *this;
	}

	bool UpdateInherit(const UIStyleAttribute& parent)
	{
		m_hasValue = false;

		if (isSet)
		{
			bool changed = m_modified;
			m_modified = false;
			m_hasValue = true;
			return changed;
		}
		else if (parent.m_hasValue)
		{
			bool changed = false;
			if (value != parent.value)
			{
				value = parent.value;
				changed = true;
			}
			m_hasValue = true;
			return changed;
		}
		else
		{
			return false;
		}
		//else if (!parent.isSet)
		//{
		//	return false;
		//}
		//else
		//{
		//	bool changed = false;
		//	if (value != parent.value)
		//	{
		//		value = parent.value;
		//		changed = true;
		//	}
		//	return changed;
		//}
	}

	bool HasValue() const { return m_hasValue; }

	T		value;
	bool	isSet;
	bool	m_modified;

	bool	m_hasValue;		// 直値or継承によりなんらかの値を持っているか


	//EasingMode	easingMode;
	//double		time;
};
#else
class UIStyleAttribute
{
public:
	//UIStyleAttribute() : value(), isSet(false), m_modified(true) {}
	UIStyleAttribute(const tr::PropertyInfo* targetProperty, const tr::Variant& v, double time_, EasingMode easingMode_)
		: m_targetProperty(targetProperty)
		, value(v)
		, isSet(false)
		, m_modified(true)
		, time(time_)
		, easingMode(easingMode_)
	{}
	//UIStyleAttribute(const UIStyleAttribute& v) : value(v.value), isSet(v.isSet), m_modified(true) {}

	//operator const T&() const { return value; }
	//UIStyleAttribute& operator=(const T& v) { value = v; isSet = true; m_modified = true; return *this; }
	//UIStyleAttribute& operator=(const UIStyleAttribute& other) { value = other.value; isSet = other.isSet; m_modified = true; return *this; }

	bool UpdateInherit(const UIStyleAttribute& parent)
	{
		if (isSet)
		{
			bool changed = m_modified;
			m_modified = false;
			return changed;
		}
		else if (!parent.isSet)
		{
			return false;
		}
		else
		{
			bool changed = false;
			if (value.Equals(parent.value))
			{
				value = parent.value;
				changed = true;
			}
			return changed;
		}
	}

	const tr::PropertyInfo*	m_targetProperty;
	tr::Variant			value;
	bool				isSet;
	bool				m_modified;


	EasingMode	easingMode;
	double		time;

	bool				m_mergedMark;	// UIStylePropertyTable::UpdateInherit の中で使う作業用変数
};
#endif




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
	detail::InvalidateFlags UpdateInherit(UIStylePropertyTable* parent);
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

	detail::InvalidateFlags MergeActiveStylePropertyTables(UIStylePropertyTable* store, const List<String>& visualStateNames)
	{
		detail::InvalidateFlags invalidateFlags = detail::InvalidateFlags::None;

		// 継承元に再帰
		if (m_baseOn != nullptr)
		{
			invalidateFlags |= m_baseOn->MergeActiveStylePropertyTables(store, visualStateNames);
		}

		invalidateFlags |= store->UpdateInherit(m_basePropertyTable);

		// このあたりの処理で、あとから追加されたスタイルが優先されることになる
		UIStylePropertyTable* lastActiveStyle = nullptr;
		for (auto& pair : m_visualStatePropertyTableList)
		{
			const String& name = pair.first;
			if (visualStateNames.Contains(name))
			{
				invalidateFlags |= store->UpdateInherit(pair.second);
			}
		}
		return invalidateFlags;
	}

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

LN_NAMESPACE_END
