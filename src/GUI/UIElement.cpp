
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/GUIContext.h>
#include <Lumino/GUI/UIElement.h>
#include "UIManagerImpl.h"
#include "GUIPainter.h"
#include "GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// UIElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElement, CoreObject);

// Register property
LN_PROPERTY_IMPLEMENT(UIElement, SizeF, SizeProperty, "Size", m_size, PropertyMetadata(SizeF::Zero));
LN_PROPERTY_IMPLEMENT(UIElement, ThicknessF, MarginProperty, "Margin", m_margin, PropertyMetadata(ThicknessF::Zero));
LN_PROPERTY_IMPLEMENT(UIElement, HorizontalAlignment, HorizontalAlignmentProperty, "HorizontalAlignment", m_horizontalAlignment, PropertyMetadata(HorizontalAlignment::Stretch));
LN_PROPERTY_IMPLEMENT(UIElement, VerticalAlignment, VerticalAlignmentProperty, "VerticalAlignment", m_verticalAlignment, PropertyMetadata(VerticalAlignment::Stretch));
LN_PROPERTY_IMPLEMENT(UIElement, float, OpacityProperty, "Opacity", m_opacity, PropertyMetadata(1.0f));
LN_PROPERTY_IMPLEMENT(UIElement, ToneF, ToneProperty, "Tone", m_tone, PropertyMetadata(ToneF()));
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsEnabledProperty, "IsEnabled", m_isEnabled, PropertyMetadata(true));
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsMouseOverProperty, "IsMouseOver", m_isMouseOver, PropertyMetadata(false));
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsHitTestProperty, "IsHitTest", m_isHitTest, PropertyMetadata(true));

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseEnterEvent, "MouseEnter", MouseEnter);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseLeaveEvent, "MouseLeave", MouseLeave);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, KeyEventArgs, KeyDownEvent, "KeyDown", KeyDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, KeyEventArgs, KeyUpEvent, "KeyUp", KeyUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, KeyEventArgs, TextInputEvent, "TextInput", TextInput);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent, "CanExecuteRoutedCommand", CanExecuteRoutedCommand);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, ExecuteRoutedCommandEventArgs, ExecuteRoutedCommandEvent, "ExecuteRoutedCommand", ExecuteRoutedCommand);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManagerImpl* manager)
	: m_ownerContext(NULL)
	, m_parent(NULL)
	, m_size(NAN, NAN)
	, m_margin()
	, m_opacity(1.0f)
	, m_combinedOpacity(1.0f)
	, m_animationClockList()
	, m_invalidateFlags(InvalidateFlags::None)
	, m_isEnabled(true)
	, m_isMouseOver(false)

	, m_manager(manager)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
	, m_horizontalAlignment(HorizontalAlignment::Stretch)
	, m_verticalAlignment(VerticalAlignment::Stretch)
	, m_rootLogicalParent(NULL)
	, m_templateParent(NULL)
{
	LN_SAFE_ADDREF(m_manager);
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
void UIElement::Focus()
{
	if (IsFocusable())
	{
		if (m_parent != NULL) { m_parent->ActivateInternal(this); }
		m_ownerContext->SetFocusElement(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::CaptureMouse() 
{
	m_ownerContext->CaptureMouse(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	m_ownerContext->ReleaseMouseCapture(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int UIElement::GetVisualChildrenCount() const
{
	return m_visualChildren.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::GetVisualChild(int index) const
{
	/*	GetVisualChildrenCount() と GetVisualChild() を組み合わせてビジュアルにアクセスするのは WPF と同じ動作。
		本ライブラリでは以前は m_visualChildren を protected で公開して、
		サブクラスも直接 m_visualChildren に追加操作を行っていた。
		しかしこの方法だと、ItemsControl や Panel 等でかなり頻繁に m_visualChildren への Add Remove が行われることになった。
		また、ItemsControl や Panel が握っている論理要素とビジュアル要素を常に同期する必要があり、漏れが心配。
	*/
	return m_visualChildren.GetAt(index);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::GetVisualChildOrderd(int index) const
{
	// ここではソートしないので GetVisualChild() と同じものを返す。
	// コンテナ系の要素はこの関数をオーバーライドする必要がある。
	return GetVisualChild(index);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext* UIElement::GetContext() const
{
	return m_ownerContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// 無効情報フラグをこの要素に伝播させる
	if (m_parent != NULL) {
		// フォントは MeasureOverride() の中で更新する
		m_invalidateFlags |= (m_parent->m_invalidateFlags & InvalidateFlags::Font);
	}

	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// ① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin を考慮する
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	SizeF localAvailableSize(
		std::max(availableSize.Width - marginWidth, 0.0f),
		std::max(availableSize.Height - marginHeight, 0.0f));

	m_desiredSize = MeasureOverride(localAvailableSize);

	// Margin を考慮する
	m_desiredSize.Width += marginWidth;
	m_desiredSize.Height += marginHeight;

	// フォントの無効フラグを落とす
	m_invalidateFlags &= ~InvalidateFlags::Font;
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

	// Margin を考慮する
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	SizeF arrangeSize;

	// この要素のサイズが明示的に指定されている場合はそちらを優先する
	arrangeSize.Width = Math::IsNaNOrInf(m_size.Width) ? finalLocalRect.Width : m_size.Width;
	arrangeSize.Height = Math::IsNaNOrInf(m_size.Height) ? finalLocalRect.Height : m_size.Height;

	// 0 以下には出来ない
	arrangeSize.Width = std::max(arrangeSize.Width - marginWidth, 0.0f);
	arrangeSize.Height = std::max(arrangeSize.Height - marginHeight, 0.0f);

	SizeF renderSize = ArrangeOverride(arrangeSize);
	m_finalLocalRect.X = finalLocalRect.X + m_margin.Left;
	m_finalLocalRect.Y = finalLocalRect.Y + m_margin.Top;
	m_finalLocalRect.Width = renderSize.Width;
	m_finalLocalRect.Height = renderSize.Height;

	OnLayoutUpdated();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size(
		Math::IsNaNOrInf(m_size.Width) ? GUIHelper::GetViewPixelSize(m_ownerContext).Width : m_size.Width,
		Math::IsNaNOrInf(m_size.Height) ? GUIHelper::GetViewPixelSize(m_ownerContext).Height : m_size.Height);

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnLayoutUpdated()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::AddVisualChild(UIElement* element)
{
	LN_CHECK_ARGS_RETURN(element != NULL);
	LN_THROW(element->GetParent() == NULL, InvalidOperationException);	// 既に親要素があった

	m_visualChildren.Add(element);		// m_visualChildren に追加したものは OnEvent や Render が呼ばれるようになる
	element->SetInheritanceParent(this);
	element->SetParent(this);			// Visualツリーでも、論理的な親の扱いは共通。
	element->SetTemplateModified(true);	// テンプレートを再構築する必要がありそう
	element->ChangeContext(m_ownerContext);	// この中でビジュアルツリーの再構築が行われる。Parent は決まっていなければならない。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::RemoveVisualChild(UIElement* element)
{
	LN_CHECK_ARGS_RETURN(element != NULL);
	LN_THROW(element->GetParent() == this, InvalidOperationException);	// this が親要素であるはず

	m_visualChildren.Remove(element);
	element->SetInheritanceParent(NULL);
	element->ChangeContext(NULL);
	element->SetParent(NULL);
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
	// NaN の場合、この要素として必要な最小サイズは 0 となる。
	size.Width = Math::IsNaNOrInf(m_size.Width) ? 0.0f : m_size.Width;
	size.Height = Math::IsNaNOrInf(m_size.Height) ? 0.0f : m_size.Height;
	size.Width = std::min(size.Width, constraint.Width);
	size.Height = std::min(size.Height, constraint.Height);

	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UIElement::ArrangeOverride(const SizeF& finalSize)
{
	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnRender(Painter* painter)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ActivateInternal(UIElement* child)
{
	if (m_parent != NULL) { m_parent->ActivateInternal(this); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ChangeContext(GUIContext* ownerContext)
{
	// 取り外す
	if (m_ownerContext != NULL && m_ownerContext != ownerContext)
	{
	}

	// 新しい Context に変更
	if (m_ownerContext != ownerContext)
	{
		m_ownerContext = ownerContext;
		ApplyTemplate();
	}

	// 子要素にも設定する
	int count = GetVisualChildrenCount();
	for (int i = 0; i < count; ++i) {
		UIElement* child = GetVisualChild(i);
		child->ChangeContext(m_ownerContext);
	}
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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnMouseEnter(MouseEventArgs* e)
{
	// 親にもマウスがはじめて乗ったのであれば親にも通知する
	if (m_parent != NULL && !m_parent->m_isMouseOver) {
		m_parent->OnMouseEnter(e);
	}

	m_isMouseOver = true;

	if (!e->Handled) { RaiseEvent(MouseEnterEvent, this, e); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnMouseLeave(MouseEventArgs* e)
{
	// 親にもマウスが乗ったことになっていれば、ヒットテストをした上で通知する
	if (m_parent != NULL && m_parent->m_isMouseOver)
	{
		if (!m_parent->m_finalGlobalRect.Contains(PointF(e->X, e->X))) {
			m_parent->OnMouseLeave(e);
		}
	}

	m_isMouseOver = false;

	if (!e->Handled) { RaiseEvent(MouseLeaveEvent, this, e); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	int count = GetVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		UIElement* e = GetVisualChildOrderd(i)->CheckMouseHoverElement(globalPt);
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
void UIElement::SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/)
{
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
		m_rootLogicalParent->PropertyChangedForTemplateBindings -= m_templateBindingHandler;
	}

	// delegate が未作成であれば作る (初回時)
	if (m_templateBindingHandler.IsEmpty()) {
		m_templateBindingHandler = Delegate<void(PropertyChangedEventArgs*)>(this, &UIElement::TemplateBindingSource_PropertyChanged);
	}

	// 要素を保持する
	m_rootLogicalParent = logicalParent;

	if (m_rootLogicalParent != NULL)
	{
		// 新しい方にイベントハンドラを登録する
		m_rootLogicalParent->PropertyChangedForTemplateBindings += m_templateBindingHandler;

		// TemplateBinding を更新する
		for (TemplateBindingInfo& info : m_templateBindingInfoList)
		{
			// TODO: TypeInfo::FindProperty(m_rootLogicalParent, name) のほうが自然？
			auto* prop = m_rootLogicalParent->GetThisTypeInfo()->FindProperty(info.SourcePropPath);
			if (prop != NULL)
			{
				info.ThisProp->SetValue(this, prop->GetValue(m_rootLogicalParent));
			}
		}
	}

	// 子要素へも同じ論理親要素をセットする
	GUIHelper::ForEachVisualChildren(this, [logicalParent](UIElement* child) { child->UpdateTemplateLogicalParentHierarchy(logicalParent); });
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnPropertyChanged(PropertyChangedEventArgs* e)
{
	CoreObject::OnPropertyChanged(e);

	if (e->ChangedProperty == IsEnabledProperty) {
		CoreObject::RaiseEvent(IsEnabledChanged, e);
	}
	
	// TemplateBinding に伝える
	CoreObject::RaiseEvent(PropertyChangedForTemplateBindings, e);
	
	// スタイルの持つトリガーに伝える
	if (m_style != NULL) {
		m_style->NortifyTargetObjectPropertyChanged(this, e);
	}
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
		m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
	}
	else
	{
		m_finalGlobalRect.X = m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_finalLocalRect.Y;
		m_combinedOpacity = m_opacity;
	}
	m_finalGlobalRect.Width = m_finalLocalRect.Width;
	m_finalGlobalRect.Height = m_finalLocalRect.Height;

	// 子要素
	GUIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	// TODO: Panel とか、描く必要の無いものは特殊あつかいにして、Painter 作らないようにしたい
	Internal::GUIPainter* painter = GUIHelper::GUIManager_GetGUIPainter(m_manager);
	painter->ResetState();
	painter->SetTransform(Matrix::Translation(Vector3(m_finalGlobalRect.X, m_finalGlobalRect.Y, 0)));	// TODO: 初期 Transform は DrawingContext とかベースクラスに作って隠したい。
	painter->SetOpacity(m_combinedOpacity);
	painter->SetTone(m_tone);	// TODO: combind
	OnRender(painter);

	// 子要素
	int count = GetVisualChildrenCount();
	for (int i = 0; i < count; ++i)
	{
		GetVisualChildOrderd(i)->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(RoutedEventType type, RoutedEventArgs* args)
{
	/* 今のところ、イベントを再帰で通知していく必要はない。
		マウスイベントは Hover しているものへ Manager が直接送り込む。
		キーイベントはフォーカスを持っているものへ Manager が直接送り込む。

		…というか、再帰で通知してはならない。マウスイベントとかは再帰してしまうと、
		マウスカーソルが乗っていない要素がイベントを受け取ってしまうことになる。
	*/
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	//int count = GetVisualChildrenCount();
	//for (int i = count - 1; i >= 0; i--)
	//{
	//	if (GetVisualChild(i)->OnEvent(type, args)) { return true; }
	//}


	switch (type)
	{
	case RoutedEventType::Unknown:
		break;
	case RoutedEventType::MouseMove:
		if (m_isEnabled) { OnMouseMove(static_cast<MouseEventArgs*>(args)); }
		break;
	case RoutedEventType::MouseButtonDown:
		if (m_isEnabled) { OnMouseDown(static_cast<MouseEventArgs*>(args)); }
		break;
	case RoutedEventType::MouseButtonUp:
		if (m_isEnabled) { OnMouseUp(static_cast<MouseEventArgs*>(args)); }
		break;
	case RoutedEventType::MouseWheel:
		break;
	case RoutedEventType::KeyDown:
		if (m_isEnabled) { OnKeyDown(static_cast<KeyEventArgs*>(args)); }
		break;
	case RoutedEventType::KeyUp:
		if (m_isEnabled) { OnKeyUp(static_cast<KeyEventArgs*>(args)); }
		break;
	case RoutedEventType::TextInput:
		if (m_isEnabled) { OnTextInput(static_cast<KeyEventArgs*>(args)); }
		break;
	case RoutedEventType::ElapsedTime:
		break;
	case RoutedEventType::MouseEnter:	// TODO: 親領域
		if (m_isEnabled) { OnMouseEnter(static_cast<MouseEventArgs*>(args)); }
		break;
	case RoutedEventType::MouseLeave:	// TODO: 親領域
		if (m_isEnabled) { OnMouseLeave(static_cast<MouseEventArgs*>(args)); }
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
		GUIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->ApplyTemplate(); });
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
	// 古い要素があればいろいろ解除する
	if (m_templateChild != NULL)
	{
		RemoveVisualChild(m_templateChild);
	}

	if (child != NULL)
	{
		AddVisualChild(child);
		m_templateChild = child;

		// テンプレートのルート論理要素が変わったことを、m_templateChild の子要素に通知する。
		// この中で TemplateBinding などのリンクが行われる。
		m_templateChild->UpdateTemplateLogicalParentHierarchy(this);
	}
}

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
		if (info.SourcePropPath == e->ChangedProperty->GetName()) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
}


LN_NAMESPACE_GUI_END
} // namespace Lumino
