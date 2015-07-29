
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>

namespace Lumino
{
namespace GUI
{




//=============================================================================
// UIElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElement, CoreObject);


LN_PROPERTY_IMPLEMENT(UIElement, SizeF, Size, m_size, SizeF::Zero);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::HorizontalAlignment, HorizontalAlignment, m_horizontalAlignment, HorizontalAlignment::Stretch);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::VerticalAlignment, VerticalAlignment, m_verticalAlignment, VerticalAlignment::Stretch);
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsHitTest, m_isHitTest, true);

////const PropertyID	UIElement::SizeProperty(_T("Size"));
//LN_DEFINE_PROPERTY_2(UIElement, SizeF, SizeProperty, "Size", SizeF::Zero, &UIElement::SetSize, &UIElement::GetSize);
//LN_DEFINE_PROPERTY_ENUM_2(UIElement, HorizontalAlignment, HorizontalAlignmentProperty, "HorizontalAlignment", HorizontalAlignment::Stretch, &UIElement::SetHorizontalAlignment, &UIElement::GetHorizontalAlignment);
//LN_DEFINE_PROPERTY_ENUM_2(UIElement, VerticalAlignment, VerticalAlignmentProperty, "VerticalAlignment", VerticalAlignment::Stretch, &UIElement::SetVerticalAlignment, &UIElement::GetVerticalAlignment);

//const PropertyID	UIElement::HorizontalAlignmentProperty(_T("HorizontalAlignment"));
//const PropertyID	UIElement::VerticalAlignmentProperty(_T("VerticalAlignment"));

//const RoutedEvent*	UIElement::MouseMoveEvent(_T("MouseMove"));
//const RoutedEvent*	UIElement::MouseLeaveEvent(_T("MouseLeave"));
//const RoutedEvent*	UIElement::MouseEnterEvent(_T("MouseEnter"));
//const RoutedEvent*	UIElement::MouseDownEvent(_T("MouseDown"));
//const RoutedEvent*	UIElement::MouseUpEvent(_T("MouseUp"));
//
//const RoutedEvent*	UIElement::CanExecuteRoutedCommandEvent(_T("CanExecuteRoutedCommandEvent"));
//const RoutedEvent*	UIElement::ExecuteRoutedCommandEvent(_T("ExecuteRoutedCommandEvent"));
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);

LN_DEFINE_ROUTED_EVENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent, "CanExecuteRoutedCommand", Handler_CanExecuteRoutedCommandEvent);
LN_DEFINE_ROUTED_EVENT(UIElement, ExecuteRoutedCommandEventArgs, ExecuteRoutedCommandEvent, "ExecuteRoutedCommand", Handler_ExecuteRoutedCommandEvent);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
	, m_parent(NULL)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
	, m_horizontalAlignment(HorizontalAlignment::Stretch)
	, m_verticalAlignment(VerticalAlignment::Stretch)
	, m_rootLogicalParent(NULL)
	, m_templateParent(NULL)
{
	LN_SAFE_ADDREF(m_manager);

	// イベントの登録
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent, [](UIElement* target, MouseEventArgs* e) { target->MouseMove(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseDownEvent,	[](UIElement* target, MouseEventArgs* e) { target->MouseDown(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseUpEvent,		[](UIElement* target, MouseEventArgs* e) { target->MouseUp(e); });
	//
	//LN_DEFINE_ROUTED_EVENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent,	[](UIElement* target, CanExecuteRoutedCommandEventArgs* e) { target->Handler_CanExecuteRoutedCommandEvent(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, ExecuteRoutedCommandEventArgs,	ExecuteRoutedCommandEvent,		[](UIElement* target, ExecuteRoutedCommandEventArgs* e) { target->Handler_ExecuteRoutedCommandEvent(e); });

	// 削除予定
	//m_eventDataStore.Add(MouseMoveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseLeaveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseEnterEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_localResource);
	LN_SAFE_RELEASE(m_combinedLocalResource);
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::InitializeComponent()
{
	// この時点では何かの子要素にはなっていないはずなので、
	// m_combinedLocalResource はグローバルリソースとなる。
	UpdateLocalResource();

	// インスタンス化したクラス型情報で Style を検索する。
	// 無かった場合はベースクラスの Style を検索する…ということはしない。(WPF の仕様)
	m_style = m_combinedLocalResource->FindStyle(GetThisTypeInfo());
	if (m_style != NULL) {
		m_style->Apply(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::CaptureMouse() 
{
	m_manager->CaptureMouse(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	m_manager->ReleaseMouseCapture(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate()
{
	//CombinedLocalResource* parentResource = NULL;
	//if (m_parent != NULL) {
	//	parentResource = m_parent->m_combinedLocalResource;
	//}
	//else {
	//	parentResource = m_manager->GetRootCombinedResource();	// 親要素が無ければ Manager のリソースを使う
	//}
	ApplyTemplateHierarchy(/*parentResource*/);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLocalResource()
{
	// 親は？
	CombinedLocalResource* parentResource = NULL;
	if (m_parent != NULL) {
		parentResource = m_parent->m_combinedLocalResource;
	}
	else {
		parentResource = m_manager->GetRootCombinedResource();	// 親要素が無ければ Manager のリソースを使う
	}

	if (m_combinedLocalResource != NULL && parentResource != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parentResource, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parentResource);
	}

	// 自分のテンプレートを更新する必要がある場合
	//if (m_templateModified)
	//{
	//	// 親は？
	//	CombinedLocalResource* parentResource = NULL;
	//	if (m_parent != NULL) {
	//		parentResource = m_parent->m_combinedLocalResource;
	//	}
	//	else {
	//		parentResource = m_manager->GetRootCombinedResource();	// 親要素が無ければ Manager のリソースを使う
	//	}

	//	// 結合
	//	if (m_combinedLocalResource != NULL && parentResource != m_combinedLocalResource) {
	//		m_combinedLocalResource->Combine(parentResource, m_localResource);
	//	}
	//	else {
	//		LN_REFOBJ_SET(m_combinedLocalResource, parentResource);
	//	}
	//	// ControlTemplate や DataTemplate はこのオーバーライドで Apply される
	//	OnApplyTemplate(m_combinedLocalResource);

	//	m_templateModified = false;			// 自分のテンプレートを更新した
	//	m_childTemplateModified = true;		// 自分を更新したら、子も更新する必要がある
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// 子要素を優先
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	UIElement* e = child->CheckMouseHoverElement(globalPt);
	//	if (e != NULL) { return e; }
	//}
	//if (m_templateChild != NULL)
	//{
	//	UIElement* e = m_templateChild->CheckMouseHoverElement(globalPt);
	//	if (e != NULL) { return e; }
	//	// 子論理要素は UIElement の担当ではない。
	//	// ContetntControl 等でこの関数をオーバーライドし、そちらに実装する。
	//}
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	for (int i = m_visualChildren.GetCount() - 1; i >= 0; i--)
	{
		UIElement* e = m_visualChildren[i]->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}

	if (m_finalGlobalRect.Contains(globalPt)) {
		return this;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// ① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm


	m_desiredSize = MeasureOverride(availableSize);

	//const SizeF& size = GetSize();
	//m_desiredSize.Width = std::min(size.Width, m_desiredSize.Width);
	//m_desiredSize.Height = std::min(size.Height, m_desiredSize.Height);

	// 子要素
	//if (m_templateChild != NULL) {
	//	m_templateChild->MeasureLayout(m_desiredSize);
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->MeasureLayout(m_desiredSize);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	SizeF renderSize = ArrangeOverride(finalLocalRect.GetSize());
	m_finalLocalRect.X = finalLocalRect.X;
	m_finalLocalRect.Y = finalLocalRect.Y;
	m_finalLocalRect.Width = renderSize.Width;
	m_finalLocalRect.Height = renderSize.Height;

	// 子要素 (もし複数あれば m_finalLocalRect の領域に重ねられるように配置される)
	//if (m_templateChild != NULL) {
	//	RectF rect(0, 0, m_finalLocalRect.Width, m_finalLocalRect.Height);
	//	m_templateChild->ArrangeLayout(rect);
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->ArrangeLayout(m_finalLocalRect;
	//}

	LayoutUpdated(NULL);
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UIElement::MeasureOverride(const SizeF& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	SizeF size;
	size.Width = Math::IsNaNOrInf(m_size.Width) ? 0.0f : size.Width;
	size.Height = Math::IsNaNOrInf(m_size.Height) ? 0.0f : size.Height;
	size.Width = std::min(size.Width, constraint.Width);
	size.Height = std::min(size.Height, constraint.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/)
{
	//if (m_rootLogicalParent == NULL)
	//{
	//	// TODO: 解除は？
	//	rootLogicalParent->PropertyChanged.AddHandler(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
	//	m_rootLogicalParent = rootLogicalParent;
	//}
	//else if (m_rootLogicalParent != rootLogicalParent) {
	//	// あってはならない。
	//	// VisualTree 要素が異なる ルート Logcal 要素にバインドしようとした。
	//	LN_THROW(0, InvalidOperationException);
	//}

	TemplateBindingInfo info;
	info.ThisProp = thisProp;
	info.SourcePropPath = srcPropPath;
	m_templateBindingInfoList.Add(info);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateTemplateLogicalParentHierarchy(UIElement* logicalParent)
{
	if (m_rootLogicalParent == logicalParent) { return; }

	// 古いほうからイベントハンドラを解除する
	// (m_rootLogicalParent に何かセットされているのだから、m_templateBindingHandler は NULL ではないはず)
	if (m_rootLogicalParent != NULL) {
		m_rootLogicalParent->PropertyChanged -= m_templateBindingHandler;
	}

	// delegate が未作成であれば作る (初回時)
	if (m_templateBindingHandler.IsEmpty()) {
		m_templateBindingHandler = LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged);
	}

	// 要素を保持する
	m_rootLogicalParent = logicalParent;

	// 新しい方にイベントハンドラを登録する
	if (m_rootLogicalParent) {
		m_rootLogicalParent->PropertyChanged += m_templateBindingHandler;
	}

	// 子要素へも同じ論理親要素をセットする
	for (UIElement* child : m_visualChildren) {
		child->UpdateTemplateLogicalParentHierarchy(logicalParent);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnPropertyChanged(const String& name, const Variant& newValue)
{
	CoreObject::OnPropertyChanged(name, newValue);
	if (m_style != NULL)
	{
		RefPtr<PropertyChangedEventArgs> args(m_manager->GetEventArgsPool()->Create<PropertyChangedEventArgs>(name, newValue));
		m_style->InvoleTriggers(CoreObject::PropertyChangedEvent, args, this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateTransformHierarchy()
{
	if (m_parent != NULL)
	{
		m_finalGlobalRect.X = m_parent->m_finalGlobalRect.X + m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_parent->m_finalGlobalRect.Y + m_finalLocalRect.Y;
	}
	else
	{
		m_finalGlobalRect.X = m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_finalLocalRect.Y;
	}
	m_finalGlobalRect.Width = m_finalLocalRect.Width;
	m_finalGlobalRect.Height = m_finalLocalRect.Height;

	for(UIElement* child : m_visualChildren) {
		child->UpdateTransformHierarchy();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	// 子要素
	//if (m_templateChild != NULL) {
	//	m_templateChild->Render();
	//}
	LN_FOREACH(UIElement* child, m_visualChildren) {
		child->Render();
	}

	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO
	OnRender(&painter);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(EventType type, RoutedEventArgs* args)
{
	// 子要素
	//if (m_templateChild != NULL) {
	//	if (m_templateChild->OnEvent(type, args)) { return true; }
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	if (child->OnEvent(type, args)) { return true; }
	//}
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	for (int i = m_visualChildren.GetCount() - 1; i >= 0; i--)
	{
		if (m_visualChildren[i]->OnEvent(type, args)) { return true; }
	}

	switch (type)
	{
	case Lumino::GUI::EventType_Unknown:
		break;
	case Lumino::GUI::EventType_MouseMove:
		OnMouseMove(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonDown:
		OnMouseDown(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonUp:
		OnMouseUp(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseWheel:
		break;
	case Lumino::GUI::EventType_KeyDown:
		break;
	case Lumino::GUI::EventType_KeyUp:
		break;
	case Lumino::GUI::EventType_ElapsedTime:
		break;
	case Lumino::GUI::EventType_MouseEnter:
		//RaiseEvent(MouseEnterEvent, this, args);
		break;
	case Lumino::GUI::EventType_MouseLeave:
		//RaiseEvent(MouseLeaveEvent, this, args);
		break;
	default:
		break;
	}

	//if (type == EventType_MouseMove) {
	//	//RaiseEvent<MouseEventArgs>(MouseMoveEvent, this, (MouseEventArgs*)args);
	//}
	return args->Handled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/)
{
	UpdateLocalResource();

	// 自分のテンプレートを更新する必要がある場合
	if (m_templateModified)
	{
		//if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		//	m_combinedLocalResource->Combine(parent, m_localResource);
		//}
		//else {
		//	LN_REFOBJ_SET(m_combinedLocalResource, parent);
		//}
		// ControlTemplate や DataTemplate はこのオーバーライドで Apply される
		OnApplyTemplate(m_combinedLocalResource);

		m_templateModified = false;			// 自分のテンプレートを更新した
		m_childTemplateModified = true;		// 自分を更新したら、子も更新する必要がある
	}

	// 子要素のテンプレートを更新する必要がある場合 (パフォーマンスのため)
	if (m_childTemplateModified)
	{
		LN_FOREACH(UIElement* child, m_visualChildren) {
			child->ApplyTemplate();
			//child->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
		}
		m_childTemplateModified = false;	// 子要素のテンプレートも更新した
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIElement::UpdateTemplateHierarchy()
//{
//	//// 自分のテンプレート更新が必要
//	//if (m_templateModified)
//	//{
//	//	ApplyTemplate();
//	//}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateChild(UIElement* child)
{
	//LN_VERIFY_RETURN(child != NULL);

	// 古い要素があればいろいろ解除する
	if (m_templateChild != NULL)
	{
		m_visualChildren.Remove(m_templateChild);
		// TODO: templateBinding 用の PropertyChanged がひつようかもしれない。
		//PropertyChanged.Clear();
		//rootLogicalParent->PropertyChanged(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
	}
	if (child != NULL)
	{
		RefPtr<UIElement> t(child, true);
		m_templateChild = child;
		m_visualChildren.Add(child);
		child->m_parent = this;
		child->m_templateParent = this;
		child->UpdateTemplateLogicalParentHierarchy(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIElement::AddVisualChild(UIElement* child)
//{
//	LN_VERIFY_RETURN(child != NULL);
//	LN_VERIFY_RETURN(child->m_parent == NULL);
//
//	RefPtr<UIElement> t(child, true);
//	m_visualChildren.Add(t);
//	child->m_parent = this;
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_CanExecuteRoutedCommandEvent(CanExecuteRoutedCommandEventArgs* e)
{
	// TODO: リバースイテレータでないと、コマンドのオーバーライドに対応できない
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			e->CanExecute = command->CanExecute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled が false なら親 UIElement の Handler_CanExecuteRoutedCommandEvent へ流れる。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_ExecuteRoutedCommandEvent(ExecuteRoutedCommandEventArgs* e)
{
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			command->Execute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled が false なら親 UIElement の Handler_ExecuteRoutedCommandEvent へ流れる。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::TemplateBindingSource_PropertyChanged(/*CoreObject* sender, */PropertyChangedEventArgs* e)
{
	LN_FOREACH(TemplateBindingInfo& info, m_templateBindingInfoList)
	{
		if (info.SourcePropPath == e->PropertyName) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
}


} // namespace GUI
} // namespace Lumino
