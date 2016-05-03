
#pragma once
#include "../Animation/Common.h"
#include "Common.h"
#include "Detail.h"
#include "../Base/Thickness.h"

LN_NAMESPACE_BEGIN
class UIStyle;
using UIStylePtr = RefPtr<UIStyle>;

template<typename T>
class UIStyleAttribute
{
public:
	UIStyleAttribute() : value(), isSet(false), m_modified(true) {}
	UIStyleAttribute(const T& v) : value(v), isSet(false), m_modified(true) {}
	UIStyleAttribute(const UIStyleAttribute& v) : value(v.value), isSet(v.isSet), m_modified(true) {}

	operator const T&() const { return val; }
	UIStyleAttribute& operator=(const T& v) { value = v; isSet = true; m_modified = true; return *this; }
	UIStyleAttribute& operator=(const UIStyleAttribute& other) { value = other.value; isSet = other.isSet; m_modified = true; return *this; }

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
			if (value != parent.value)
			{
				value = parent.value;
				changed = true;
			}
			return changed;
		}
	}

	T		value;
	bool	isSet;
	bool	m_modified;


	//EasingMode	easingMode;
	//double		time;
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

LN_INTERNAL_ACCESS:
	detail::InvalidateFlags UpdateInherit(UIStyle* parent);

public:	// TODO:
	//UIStyle*	m_lastUpdateParent;
	//int			m_revisionCount;


	UIStyleAttribute<ThicknessF>			m_margin;
	UIStyleAttribute<ThicknessF>			m_padding;
	UIStyleAttribute<VerticalAlignment>		m_verticalAlignment;
	UIStyleAttribute<HorizontalAlignment>	m_horizontalAlignment;
	UIStyleAttribute<BrushPtr>				m_background;
	UIStyleAttribute<BrushPtr>				m_foreground;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderLeft;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderRight;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderTop;
	//UIStyleAttribute<RefPtr<Pen>>			m_borderBottom;
	//UIStyleAttribute<FontPtr>				m_font;

	UIStyleAttribute<String>				m_fontFamily;
	UIStyleAttribute<int>					m_fontSize;
	UIStyleAttribute<bool>					m_fontBold;
	UIStyleAttribute<bool>					m_fontItalic;

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

	void AddStyle(const String& targetName, const String& targetState, UIStyle* style);

	// 見つからなければ nullptr
	// TODO: 名前だけじゃダメ。サブクラスに対応するスタイルが見つからなければ基底クラスのも探したい。UIElement のインスタンスを渡すようにする。
	UIStyle* FindStyle(const String& targetName, const String& targetState);

private:
	struct StyleKey
	{
		String	targetName;
		String	targetState;	// VisualState

		bool operator < (const StyleKey& other) const { return targetName < other.targetName && targetState < other.targetState; }
		bool operator == (const StyleKey& other) const { return targetName == other.targetName && targetState == other.targetState; }
	};
	SortedArray<StyleKey, RefPtr<UIStyle>>	m_table;
};

LN_NAMESPACE_END
