/*
	[2016/5/13] m_image について
		WPF とかには用意されていないが、ゲーム向けUIとしては用意しておいたほうが便利なので持たせたもの。
		一般的なGUIフレームワークは色や線の種類でスタイルを表現するが、ゲームでは画像を使うことが多い。
		ただ、1枚の画像だけでフェードなどの遷移を行うと、透明度のブレンド中に一瞬後ろのオブジェクトが透けて見えることになる。
		背景と前景2種類用意し、背景は基本的に透明度を変更しない、前景はVisualStateの変更に合わせていろいろ変わるようにすると安定して見える。

		ちなみにQtだとimageプロパティがある。
		http://doc.qt.io/qt-4.8/stylesheet-examples.html
*/
#include "Internal.h"
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIStyle.h>
#include "../Animation/AnimationManager.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIStylePropertyTable
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStylePropertyTable, Object);

//------------------------------------------------------------------------------
UIStylePropertyTable::UIStylePropertyTable()
	//: m_lastInheritedParent(nullptr)
{
	// 初期状態を ByInherit にしておく。
	// こうすることで、MergeActiveStylePropertyTables() で 有効な VisualStyle が1つも無いときに各プロパティがデフォルト値に戻るようにする。
	background.SetValueSource(tr::PropertySetSource::ByInherit);
}

//------------------------------------------------------------------------------
UIStylePropertyTable::~UIStylePropertyTable()
{
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::Initialize(const StringRef& visualStateName)
{
	m_visualStateName = visualStateName;
}

//------------------------------------------------------------------------------
//void UIStylePropertyTable::AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
//{
//	m_attributes.Add(UIStyleAttribute(targetProperty, value, time, easingMode));
//}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTable::UpdateInherit(UIStylePropertyTable* parent)
{
	bool changed = false;

	//// parent が持っている値のうち、同じ targetProperty のものを探す。そんなに数は多くないはずなので線形探索。
	//for (UIStyleAttribute& parentAttr : parent->m_attributes)
	//{
	//	parentAttr.m_mergedMark = false;

	//	for (UIStyleAttribute& attr : m_attributes)
	//	{
	//		if (attr.m_targetProperty == parentAttr.m_targetProperty)
	//		{
	//			changed |= attr.UpdateInherit(parentAttr);
	//			parentAttr.m_mergedMark = true;
	//			break;
	//		}
	//	}
	//}

	//// parent が持っている値のうち、同じ targetProperty ではなかったものを単なる参照として継承する。
	//m_parentRefAttributes.Clear();
	//for (UIStyleAttribute& parentAttr : parent->m_attributes)
	//{
	//	if (!parentAttr.m_mergedMark)
	//	{
	//		m_parentRefAttributes.Add(&parentAttr);
	//		changed = true;	// TODO: ほとんど毎回更新されたことになってしまう。リビジョンカウント必須かな・・
	//	}
	//}

	return (changed) ? detail::InvalidateFlags::All : detail::InvalidateFlags::None;
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTable::Merge(UIStylePropertyTable* source)
{
	detail::InvalidateFlags flags = detail::InvalidateFlags::None;
	{
		bool changed = false;
		changed |= width.UpdateInherit(source->width);
		changed |= height.UpdateInherit(source->height);
		if (changed) flags |= detail::InvalidateFlags::Layout;
	}
	{
		bool changed = false;
		changed |= background.Inherit(source->background);
		changed |= borderThickness.Inherit(source->borderThickness);
		changed |= cornerRadius.Inherit(source->cornerRadius);
		changed |= leftBorderColor.Inherit(source->leftBorderColor);
		changed |= topBorderColor.Inherit(source->topBorderColor);
		changed |= rightBorderColor.Inherit(source->rightBorderColor);
		changed |= bottomBorderColor.Inherit(source->bottomBorderColor);
		changed |= borderDirection.Inherit(source->borderDirection);
		if (changed) flags |= detail::InvalidateFlags::Rendering;
	}
	return flags;
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::Apply(UIElement* targetElement, bool useTransitionAnimation)
{
	if (width.HasValue())
		targetElement->SetWidth(width);
	if (height.HasValue())
		targetElement->SetHeight(height);
	//if (background.HasValue())
	//	targetElement->SetBackground(background.value);
	//for (UIStyleAttribute& setter : m_attributes)
	//{
	//	ApplyInternal(targetElement, setter, useTransitionAnimation);
	//}
	//for (UIStyleAttribute* setter : m_parentRefAttributes)
	//{
	//	ApplyInternal(targetElement, *setter, useTransitionAnimation);
	//}
}

//------------------------------------------------------------------------------
//void UIStylePropertyTable::ApplyInternal(UIElement* targetElement, const UIStyleAttribute& setter, bool useTransitionAnimation)
//{
//	if (!useTransitionAnimation || setter.time == 0.0)
//	{
//		tr::PropertyInfo::SetPropertyValue(targetElement, setter.m_targetProperty, setter.value);
//	}
//	else
//	{
//		// アニメーション
//		if (setter.value.GetType() == tr::VariantType::Float)
//		{
//			float now = tr::PropertyInfo::GetPropertyValueDirect<float>(targetElement, setter.m_targetProperty);
//			float target = tr::Variant::Cast<float>(setter.value);
//
//			auto anim = ValueEasingCurve<float>::Create(target, setter.time, setter.easingMode);
//			AnimationClock* ac = targetElement->GetManager()->GetAnimationManager()->StartPropertyAnimation(targetElement);
//			ac->AddAnimationCurve(anim.Get(), targetElement, static_cast<const tr::TypedPropertyInfo<float>*>(setter.m_targetProperty), now);
//		}
//		else
//		{
//			LN_NOTIMPLEMENTED();
//		}
//	}
//}


//==============================================================================
// UIStyle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyle, Object);

//------------------------------------------------------------------------------
UIStylePtr UIStyle::Create()
{
	return NewObject<UIStyle>();//UIStylePtr::MakeRef();
}

//------------------------------------------------------------------------------
UIStyle::UIStyle()
	//: m_subStyleParent(nullptr)
	//, m_subStateStyles()
	//: m_lastUpdateParent(nullptr)
	//m_revisionCount(0)
	//: m_margin(ThicknessF(0, 0, 0, 0))
	//, m_padding(ThicknessF(0, 0, 0, 0))
	//, m_verticalAlignment(VerticalAlignment::Top)
	//, m_horizontalAlignment(HorizontalAlignment::Left)
	//, m_background(nullptr)
	//, m_foreground(nullptr)
	//, m_fontFamily(String::GetEmpty())
	//, m_fontSize(0)
	//, m_fontBold(false)
	//, m_fontItalic(false)
{
}

//------------------------------------------------------------------------------
void UIStyle::Initialize()
{
	m_basePropertyTable = RefPtr<UIStylePropertyTable>::MakeRef();
	m_basePropertyTable->Initialize(_T(""));
}

//------------------------------------------------------------------------------
UIStyle::~UIStyle()
{
}

////------------------------------------------------------------------------------
//void UIStyle::AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
//{
//	m_basePropertyTable->AddValue(targetProperty, value, time, easingMode);
//}
//
////------------------------------------------------------------------------------
//void UIStyle::AddValue(const StringRef& visualStateName, const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
//{
//	auto* table = GetPropertyTable(visualStateName);
//	table->AddValue(targetProperty, value, time, easingMode);
//}

//------------------------------------------------------------------------------
//void UIStyle::AddSubStateStyle(const StringRef& subStateName, UIStyle* style)
//{
//	LN_FAIL_CHECK_ARG(style != nullptr) return;
//	LN_FAIL_CHECK_ARG(style->m_subStyleParent == nullptr) return;
//	m_subStateStyles.RemoveIf(    // TODO: 削除じゃなくてマージしたほうがいいか？
//		[subStateName](const SubStateStylePair& pair) { return pair.first == subStateName; });
//	m_subStateStyles.Add({ subStateName, style });
//}
//
////------------------------------------------------------------------------------
//UIStyle* UIStyle::FindSubStateStyle(const StringRef& subStateName)
//{
//	SubStateStylePair* pair = m_subStateStyles.Find(
//		[subStateName](const SubStateStylePair& pair) { return pair.first == subStateName; });
//	if (pair == nullptr) return nullptr;
//	return pair->second;
//}

//------------------------------------------------------------------------------
UIStylePropertyTable* UIStyle::GetPropertyTable()
{
	return m_basePropertyTable;
}

//------------------------------------------------------------------------------
UIStylePropertyTable* UIStyle::GetPropertyTable(const StringRef& visualStateName)
{
	auto* ptr = m_visualStatePropertyTableList.Find([visualStateName](const VisualStateStylePair& pair) { return pair.first == visualStateName; });
	if (ptr != nullptr)
	{
		return ptr->second;
	}
	else
	{
		auto table = RefPtr<UIStylePropertyTable>::MakeRef();
		table->Initialize(visualStateName);
		m_visualStatePropertyTableList.Add(VisualStateStylePair{ visualStateName, table });
		return table;
	}
}

//------------------------------------------------------------------------------
void UIStyle::SetBaseOnStyle(UIStyle* style)
{
	m_baseOn = style;
}

//------------------------------------------------------------------------------
UIStylePropertyTable* UIStyle::FindStylePropertyTable(const String& visualStateName)
{
	auto* ptr = m_visualStatePropertyTableList.Find([visualStateName](const VisualStateStylePair& pair) { return pair.first == visualStateName; });
	if (ptr != nullptr)
	{
		return ptr->second;
	}
	else
	{
		return nullptr;
	}
}

//------------------------------------------------------------------------------
//detail::InvalidateFlags UIStyle::UpdateInherit(UIStyle* parent)
//{
//	// 親がないルート要素スタイルは継承による値変更は考慮する必要はない
//	if (parent == nullptr) return detail::InvalidateFlags::None;
//
//	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;
//
//	for (auto& pair : m_propertyTableMap)
//	{
//		// parent が持っている値のうち、同じ targetProperty のものを探す。そんなに数は多くないはずなので線形探索。
//		for (auto& parentPair : parent->m_propertyTableMap)
//		{
//			if (pair.first == parentPair.first)
//			{
//				invalidate |= pair.second->UpdateInherit(parentPair.second);
//				break;
//			}
//		}
//	}
//
//	//bool changed = false;
//	//changed |= m_background.UpdateInherit(parent->m_background);
//	////changed |= m_foreground.UpdateInherit(parent->m_foreground);
//	//if (changed) invalidate |= detail::InvalidateFlags::Rendering;
//
//	//changed = false;
//	//changed |= m_fontFamily.UpdateInherit(parent->m_fontFamily);
//	//changed |= m_fontSize.UpdateInherit(parent->m_fontSize);
//	//changed |= m_fontBold.UpdateInherit(parent->m_fontBold);
//	//changed |= m_fontItalic.UpdateInherit(parent->m_fontItalic);
//	//if (changed) invalidate |= detail::InvalidateFlags::Font;
//
//	return invalidate;
//}

//------------------------------------------------------------------------------
//void UIStyle::Apply(UIElement* targetElement)
//{
//	LN_ASSERT(targetElement != nullptr);
//
//	RefPtr<UIStylePropertyTable> table;
//	if (m_propertyTableMap.TryGetValue(targetElement->GetCurrentVisualStateName(), &table))
//	{
//		table->Apply(targetElement);
//	}
//}

//==============================================================================
// UIStyleTable
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyleTable, Object);

//------------------------------------------------------------------------------
UIStyleTable::UIStyleTable()
{
}

//------------------------------------------------------------------------------
UIStyleTable::~UIStyleTable()
{
}

////------------------------------------------------------------------------------
//void UIStyleTable::AddStyle(const tr::TypeInfo* targetType, UIStyle* style)
//{
//	m_table.Add(targetType, style);
//}
//
////------------------------------------------------------------------------------
//void UIStyleTable::AddStyle(const tr::TypeInfo* targetType, const StringRef& subStateName, UIStyle* style)
//{
//	RefPtr<UIStyle>* s = m_table.Find(targetType);
//	(*s)->AddSubStateStyle(subStateName, style);
//}
//------------------------------------------------------------------------------
UIStyle* UIStyleTable::GetStyle(const StringRef& typeName)
{
	StyleKey key = typeName.GetHashCode()/* + subControlName.GetHashCode()*/;
	if (key == 0) return nullptr;

	RefPtr<UIStyle>* s = m_table.Find(key);
	if (s == nullptr)
	{
		auto s2 = NewObject<UIStyle>();
		m_table.Add(key, s2);
		return s2;
	}
	else
	{
		return s->Get();
	}
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::GetSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName)
{
	StyleKey key = subControlOwnerName.GetHashCode() + subControlName.GetHashCode();
	if (key == 0) return nullptr;

	RefPtr<UIStyle>* s = m_subControlStyleTable.Find(key);
	if (s == nullptr)
	{
		auto s2 = NewObject<UIStyle>();
		m_subControlStyleTable.Add(key, s2);
		return s2;
	}
	else
	{
		return s->Get();
	}
}


//------------------------------------------------------------------------------
//UIStyle* UIStyleTable::GetStyle(const tr::TypeInfo* targetType, const StringRef& subStateName)
//{
//}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindStyle(const tr::TypeInfo* targetType/*, const StringRef& subControlName*/)
{
	if (LN_CHECK_ARG(targetType != nullptr)) return nullptr;

	StyleKey key = targetType->GetName().GetHashCode();// +subControlName.GetHashCode();
	RefPtr<UIStyle>* s = m_table.Find(key);
	if (s != nullptr)
	{
		return s->Get();
	}
	else if (targetType->GetBaseClass() != nullptr)
	{
		// ベースクラスで再帰検索
		return FindStyle(targetType->GetBaseClass()/*, subControlName*/);
	}
	return nullptr;
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName)
{
	StyleKey key = subControlOwnerName.GetHashCode() + subControlName.GetHashCode();

	RefPtr<UIStyle>* s = m_subControlStyleTable.Find(key);
	if (s != nullptr)
		return s->Get();
	else
		return nullptr;
}

//------------------------------------------------------------------------------
//UIStyle* UIStyleTable::FindStyle(const tr::TypeInfo* targetType, const StringRef& subStateName)
//{
//	UIStyle* style = FindStyle(targetType);
//	return style;
//	//if (style == nullptr) return nullptr;
//	//if (subStateName.IsEmpty()) return style;
//	//return style->FindSubStateStyle(subStateName);
//}
//

//==============================================================================
// UIColors
//==============================================================================

static bool				g_colorsInit = false;
static Color			g_colors[UIColors::MaxIndex][UIColors::MaxDepth];
static SolidColorBrush	g_brushes[UIColors::MaxIndex][UIColors::MaxDepth];

//------------------------------------------------------------------------------
static const void InitColors()
{
	if (!g_colorsInit)
	{
#define LN_COLOR_DEF(name, depth, r, g, b)	g_colors[(int)UIColorIndex::name][depth] = Color(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f, 1.0f);
#include "UIColorsDefine.inl"
#undef LN_COLOR_DEF

		for (int i = 0; i < UIColors::MaxDepth; i++) g_colors[(int)UIColorIndex::Black][i] = Color(0, 0, 0, 1);
		for (int i = 0; i < UIColors::MaxDepth; i++) g_colors[(int)UIColorIndex::White][i] = Color(1, 1, 1, 1);

		for (int i = 0; i < UIColors::MaxIndex; i++)
		{
			for (int j = 0; j < UIColors::MaxDepth; j++)
			{
				g_brushes[i][j].Initialize(g_colors[i][j]);
			}
		}

		g_colorsInit = true;
	}
}

//------------------------------------------------------------------------------
const Color& UIColors::GetColor(UIColorIndex index, int depth)
{
	if (LN_CHECK_RANGE(depth, 0, UIColors::MaxDepth)) return Color::Black;
	InitColors();
	return g_colors[(int)index][depth];
}

//------------------------------------------------------------------------------
SolidColorBrush* UIColors::GetBrush(UIColorIndex index, int depth)
{
	if (LN_CHECK_RANGE(depth, 0, UIColors::MaxDepth)) return nullptr;
	InitColors();
	return &g_brushes[(int)index][depth];
}
LN_NAMESPACE_END
