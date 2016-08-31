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
void UIStylePropertyTable::AddValue(const tr::Property* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
{
	m_attributes.Add(UIStyleAttribute(targetProperty, value, time, easingMode));
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTable::UpdateInherit(UIStylePropertyTable* parent)
{
	bool changed = false;

	// parent が持っている値のうち、同じ targetProperty のものを探す。そんなに数は多くないはずなので線形探索。
	for (UIStyleAttribute& parentAttr : parent->m_attributes)
	{
		parentAttr.m_mergedMark = false;

		for (UIStyleAttribute& attr : m_attributes)
		{
			if (attr.m_targetProperty == parentAttr.m_targetProperty)
			{
				changed |= attr.UpdateInherit(parentAttr);
				parentAttr.m_mergedMark = true;
				break;
			}
		}
	}

	// parent が持っている値のうち、同じ targetProperty ではなかったものを単なる参照として継承する。
	m_parentRefAttributes.Clear();
	for (UIStyleAttribute& parentAttr : parent->m_attributes)
	{
		if (!parentAttr.m_mergedMark)
		{
			m_parentRefAttributes.Add(&parentAttr);
			changed = true;	// TODO: ほとんど毎回更新されたことになってしまう。リビジョンカウント必須かな・・
		}
	}

	return (changed) ? detail::InvalidateFlags::All : detail::InvalidateFlags::None;
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::Apply(UIElement* targetElement, bool useTransitionAnimation)
{
	for (UIStyleAttribute& setter : m_attributes)
	{
		ApplyInternal(targetElement, setter, useTransitionAnimation);
	}
	for (UIStyleAttribute* setter : m_parentRefAttributes)
	{
		ApplyInternal(targetElement, *setter, useTransitionAnimation);
	}
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::ApplyInternal(UIElement* targetElement, const UIStyleAttribute& setter, bool useTransitionAnimation)
{
	if (!useTransitionAnimation || setter.time == 0.0)
	{
		tr::Property::SetPropertyValue(targetElement, setter.m_targetProperty, setter.value);
	}
	else
	{
		// アニメーション
		if (setter.value.GetType() == tr::VariantType::Float)
		{
			float now = tr::Property::GetPropertyValueDirect<float>(targetElement, setter.m_targetProperty);
			float target = tr::Variant::Cast<float>(setter.value);

			auto anim = ValueEasingCurve<float>::Create(target, setter.time, setter.easingMode);
			AnimationClock* ac = targetElement->GetManager()->GetAnimationManager()->StartPropertyAnimation(targetElement);
			ac->AddAnimationCurve(anim.Get(), targetElement, setter.m_targetProperty, now);
		}
		else
		{
			LN_NOTIMPLEMENTED();
		}
	}
}


//==============================================================================
// UIStyle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyle, Object);

//------------------------------------------------------------------------------
UIStylePtr UIStyle::Create()
{
	return UIStylePtr::MakeRef();
}

//------------------------------------------------------------------------------
UIStyle::UIStyle()
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
UIStyle::~UIStyle()
{
}

//------------------------------------------------------------------------------
void UIStyle::AddValue(const StringRef& visualStateName, const tr::Property* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
{
	RefPtr<UIStylePropertyTable> table;
	if (!m_propertyTableMap.TryGetValue(visualStateName, &table))
	{
		table = RefPtr<UIStylePropertyTable>::MakeRef();
		table->Initialize(visualStateName);
		m_propertyTableMap.Add(visualStateName, table);
	}

	table->AddValue(targetProperty, value, time, easingMode);
}

//------------------------------------------------------------------------------
UIStylePropertyTable* UIStyle::FindStylePropertyTable(const String& visualStateName)
{
	RefPtr<UIStylePropertyTable> table;
	if (m_propertyTableMap.TryGetValue(visualStateName, &table))
	{
		return table;
	}
	return false;
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

//------------------------------------------------------------------------------
void UIStyleTable::AddStyle(const tr::TypeInfo* targetType, UIStyle* style)
{
	LN_CHECK_ARG(style != nullptr);
	m_table.Add(targetType, style);
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindStyle(const tr::TypeInfo* targetType)
{
	LN_CHECK_ARG(targetType != nullptr);

	RefPtr<UIStyle>* s = m_table.Find(targetType);
	if (s != nullptr)
	{
		return s->Get();
	}
	else if (targetType->GetBaseClass() != nullptr)
	{
		// ベースクラスで再帰検索
		return FindStyle(targetType->GetBaseClass());
	}
	return nullptr;
}

LN_NAMESPACE_END
