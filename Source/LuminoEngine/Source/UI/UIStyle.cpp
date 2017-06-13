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
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIStyle.h>
#include "../Animation/AnimationManager.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIStylePropertyTable
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIRenderElement, Object);

//------------------------------------------------------------------------------
UIRenderElement::UIRenderElement()
	: m_width(Math::NaN)
	, m_height(Math::NaN)
	, m_margin(ThicknessF::Zero)
	, m_hAlignment(HAlignment::Stretch)
	, m_vAlignment(VAlignment::Stretch)
{
}

//------------------------------------------------------------------------------
UIRenderElement::~UIRenderElement()
{
}

//------------------------------------------------------------------------------
void UIRenderElement::initialize()
{
}

//------------------------------------------------------------------------------
void UIRenderElement::LayoutAndRender(DrawingContext* context, const Size& parentRenderSize)
{
	Size areaSize;
	areaSize.width = parentRenderSize.width - (m_margin.Left + m_margin.Right);
	areaSize.height = parentRenderSize.height - (m_margin.Top + m_margin.Bottom);

	Size desiredSize;
	desiredSize.width = Math::isNaN(m_width) ? 0.0f : m_width;
	desiredSize.height = Math::isNaN(m_height) ? 0.0f : m_height;

	Rect localRect;
	detail::LayoutHelper::AdjustHorizontalAlignment(areaSize, desiredSize, Math::isNaN(m_width), m_hAlignment, &localRect);
	detail::LayoutHelper::AdjustVerticalAlignment(areaSize, desiredSize, Math::isNaN(m_height), m_vAlignment, &localRect);

	context->setBrush(m_brush);
	context->DrawBoxBackground(localRect, CornerRadius());
}

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
	//background.setValueSource(tr::PropertySetSource::ByInherit);
}

//------------------------------------------------------------------------------
UIStylePropertyTable::~UIStylePropertyTable()
{
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::initialize(const StringRef& visualStateName)
{
	m_visualStateName = visualStateName;
}

//------------------------------------------------------------------------------
//void UIStylePropertyTable::AddValue(const tr::PropertyInfo* targetProperty, const tr::Variant& value, double time, EasingMode easingMode)
//{
//	m_attributes.Add(UIStyleAttribute(targetProperty, value, time, easingMode));
//}

//------------------------------------------------------------------------------
//detail::InvalidateFlags UIStylePropertyTable::UpdateInherit(UIStylePropertyTable* parent)
//{
//	bool changed = false;
//
//	//// parent が持っている値のうち、同じ targetProperty のものを探す。そんなに数は多くないはずなので線形探索。
//	//for (UIStyleAttribute& parentAttr : parent->m_attributes)
//	//{
//	//	parentAttr.m_mergedMark = false;
//
//	//	for (UIStyleAttribute& attr : m_attributes)
//	//	{
//	//		if (attr.m_targetProperty == parentAttr.m_targetProperty)
//	//		{
//	//			changed |= attr.UpdateInherit(parentAttr);
//	//			parentAttr.m_mergedMark = true;
//	//			break;
//	//		}
//	//	}
//	//}
//
//	//// parent が持っている値のうち、同じ targetProperty ではなかったものを単なる参照として継承する。
//	//m_parentRefAttributes.Clear();
//	//for (UIStyleAttribute& parentAttr : parent->m_attributes)
//	//{
//	//	if (!parentAttr.m_mergedMark)
//	//	{
//	//		m_parentRefAttributes.Add(&parentAttr);
//	//		changed = true;	// TODO: ほとんど毎回更新されたことになってしまう。リビジョンカウント必須かな・・
//	//	}
//	//}
//
//	return (changed) ? detail::InvalidateFlags::All : detail::InvalidateFlags::None;
//}



//==============================================================================
// UIStylePropertyTableInstance
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
void UIStylePropertyTableInstance::ClearAvailableRenderElements()
{
	m_availableRenderElements.clear();
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTableInstance::InheritParentElementStyle(UIStylePropertyTableInstance* parent)
{
	// TODO: 親要素から継承するのはフォント情報だけ。
	return detail::InvalidateFlags::None;
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTableInstance::Merge(const UIStylePropertyTable* source, UIStyleAttributeInheritSourceType sourceType)
{
	detail::InvalidateFlags flags = detail::InvalidateFlags::None;
	{
		bool changed = false;
		changed |= width.inherit(source->width, sourceType);
		changed |= height.inherit(source->height, sourceType);
		if (changed) flags |= detail::InvalidateFlags::Layout;
	}
	{
		bool changed = false;
		changed |= background.inherit(source->background, sourceType);
		changed |= borderThickness.inherit(source->borderThickness);
		changed |= cornerRadius.inherit(source->cornerRadius);
		changed |= leftBorderColor.inherit(source->leftBorderColor);
		changed |= topBorderColor.inherit(source->topBorderColor);
		changed |= rightBorderColor.inherit(source->rightBorderColor);
		changed |= bottomBorderColor.inherit(source->bottomBorderColor);
		changed |= borderDirection.inherit(source->borderDirection);
		if (changed) flags |= detail::InvalidateFlags::Rendering;
	}

	{
		bool changed = false;
		//changed |= testDeco.inherit(source->testDeco, sourceType);
		//for (auto& pair : m_renderElementMap)
		//{
		//	auto itr = source->m_renderElementMap.find(pair.first);
		//	if (itr != source->m_renderElementMap.end())
		//	{
		//		changed |= pair.second.inherit(itr->second, sourceType);
		//	}
		//}

		for (auto& e : source->m_renderElements)
		{
			m_availableRenderElements.add(e);
		}

		if (changed) flags |= detail::InvalidateFlags::Rendering;
	}

	return flags;
}

//------------------------------------------------------------------------------
void UIStylePropertyTableInstance::apply(UIElement* targetElement, bool useTransitionAnimation)
{
	if (width.hasValue())
		targetElement->SetWidth(width);
	if (height.hasValue())
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
//		tr::PropertyInfo::setPropertyValue(targetElement, setter.m_targetProperty, setter.value);
//	}
//	else
//	{
//		// アニメーション
//		if (setter.value.GetType() == tr::VariantType::Float)
//		{
//			float now = tr::PropertyInfo::getPropertyValueDirect<float>(targetElement, setter.m_targetProperty);
//			float target = tr::Variant::cast<float>(setter.value);
//
//			auto anim = ValueEasingCurve<float>::create(target, setter.time, setter.easingMode);
//			AnimationClock* ac = targetElement->getManager()->getAnimationManager()->StartPropertyAnimation(targetElement);
//			ac->addAnimationCurve(anim.Get(), targetElement, static_cast<const tr::TypedPropertyInfo<float>*>(setter.m_targetProperty), now);
//		}
//		else
//		{
//			LN_NOTIMPLEMENTED();
//		}
//	}
//}

} // namespace detail

//==============================================================================
// UIStyle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyle, Object);

//------------------------------------------------------------------------------
UIStylePtr UIStyle::create()
{
	return newObject<UIStyle>();//UIStylePtr::MakeRef();
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
void UIStyle::initialize()
{
	m_basePropertyTable = RefPtr<UIStylePropertyTable>::makeRef();
	m_basePropertyTable->initialize(_T(""));
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
	auto* ptr = m_visualStatePropertyTableList.find([visualStateName](const VisualStateStylePair& pair) { return pair.first == visualStateName; });
	if (ptr != nullptr)
	{
		return ptr->second;
	}
	else
	{
		auto table = RefPtr<UIStylePropertyTable>::makeRef();
		table->initialize(visualStateName);
		m_visualStatePropertyTableList.add(VisualStateStylePair{ visualStateName, table });
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
	auto* ptr = m_visualStatePropertyTableList.find([visualStateName](const VisualStateStylePair& pair) { return pair.first == visualStateName; });
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
detail::InvalidateFlags UIStyle::MergeActiveStylePropertyTables(detail::UIStylePropertyTableInstance* store, const List<String>& visualStateNames)
{
	detail::InvalidateFlags invalidateFlags = detail::InvalidateFlags::None;

	// 継承元に再帰
	if (m_baseOn != nullptr)
	{
		invalidateFlags |= m_baseOn->MergeActiveStylePropertyTables(store, visualStateNames);
	}

	invalidateFlags |= store->Merge(m_basePropertyTable, UIStyleAttributeInheritSourceType::BaseStyle);

	// このあたりの処理で、あとから追加されたスタイルが優先されることになる
	UIStylePropertyTable* lastActiveStyle = nullptr;
	for (auto& pair : m_visualStatePropertyTableList)
	{
		const String& name = pair.first;
		if (visualStateNames.contains(name))
		{
			invalidateFlags |= store->Merge(pair.second, UIStyleAttributeInheritSourceType::StyleLocal);
		}
	}
	return invalidateFlags;
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
//void UIStyle::apply(UIElement* targetElement)
//{
//	LN_ASSERT(targetElement != nullptr);
//
//	RefPtr<UIStylePropertyTable> table;
//	if (m_propertyTableMap.TryGetValue(targetElement->GetCurrentVisualStateName(), &table))
//	{
//		table->apply(targetElement);
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
	StyleKey key = typeName.getHashCode()/* + subControlName.GetHashCode()*/;
	if (key == 0) return nullptr;

	RefPtr<UIStyle>* s = m_table.find(key);
	if (s == nullptr)
	{
		auto s2 = newObject<UIStyle>();
		m_table.add(key, s2);
		return s2;
	}
	else
	{
		return s->get();
	}
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::GetSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName)
{
	StyleKey key = subControlOwnerName.getHashCode() + subControlName.getHashCode();
	if (key == 0) return nullptr;

	RefPtr<UIStyle>* s = m_subControlStyleTable.find(key);
	if (s == nullptr)
	{
		auto s2 = newObject<UIStyle>();
		m_subControlStyleTable.add(key, s2);
		return s2;
	}
	else
	{
		return s->get();
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

	StyleKey key = targetType->getName().getHashCode();// +subControlName.GetHashCode();
	RefPtr<UIStyle>* s = m_table.find(key);
	if (s != nullptr)
	{
		return s->get();
	}
	else if (targetType->getBaseClass() != nullptr)
	{
		// ベースクラスで再帰検索
		return FindStyle(targetType->getBaseClass()/*, subControlName*/);
	}
	return nullptr;
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindSubControlStyle(const StringRef& subControlOwnerName, const StringRef& subControlName)
{
	StyleKey key = subControlOwnerName.getHashCode() + subControlName.getHashCode();

	RefPtr<UIStyle>* s = m_subControlStyleTable.find(key);
	if (s != nullptr)
		return s->get();
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

class GlobalSolidColorBrush
	: public SolidColorBrush
{
public:
	GlobalSolidColorBrush() = default;
	virtual ~GlobalSolidColorBrush() = default;
	void initialize(const Color& color) { SolidColorBrush::initialize(color); }
};

static bool						g_colorsInit = false;
static Color					g_colors[UIColors::MaxIndex][UIColors::MaxDepth];
static GlobalSolidColorBrush	g_brushes[UIColors::MaxIndex][UIColors::MaxDepth];

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
				g_brushes[i][j].initialize(g_colors[i][j]);
			}
		}

		g_colorsInit = true;
	}
}

//------------------------------------------------------------------------------
const Color& UIColors::getColor(UIColorIndex index, int depth)
{
	if (LN_CHECK_RANGE(depth, 0, UIColors::MaxDepth)) return Color::Black;
	InitColors();
	return g_colors[(int)index][depth];
}

//------------------------------------------------------------------------------
SolidColorBrush* UIColors::getBrush(UIColorIndex index, int depth)
{
	if (LN_CHECK_RANGE(depth, 0, UIColors::MaxDepth)) return nullptr;
	InitColors();
	return &g_brushes[(int)index][depth];
}
LN_NAMESPACE_END
