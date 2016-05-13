
#pragma once
#include "Common.h"
#include "Detail.h"
#include "UITypeInfo.h"
#include "UIEvent.h"

LN_NAMESPACE_BEGIN
class GraphicsContext;

/**
	@brief		
	@details	
*/
class UIElement
	: public tr::ReflectionObject
{
	LN_UI_TYPEINFO_DECLARE();

public:
	LN_TR_PROPERTY(VerticalAlignment,	VerticalAlignmentProperty);		/**< VerticalAlignment プロパティの識別子 */
	LN_TR_PROPERTY(HorizontalAlignment,	HorizontalAlignmentProperty);	/**< HorizontalAlignment プロパティの識別子 */
	LN_TR_PROPERTY(BrushPtr,			BackgroundProperty);			/**< Background プロパティの識別子 */
	LN_TR_PROPERTY(BrushPtr,			ForegroundProperty);			/**< Foreground プロパティの識別子 */

	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseEnterEvent);				/**< MouseEnter ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseLeaveEvent);				/**< MouseLeave ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseMoveEvent);				/**< MouseMove ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseDownEvent);				/**< MouseDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseUpEvent);					/**< MouseUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	KeyDownEvent);					/**< KeyDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	KeyUpEvent);					/**< KeyUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIKeyEventArgs,		TextInputEvent);				/**< TextInput ルーティングイベントの識別子 */

public:
	//--------------------------------------------------------------------------
	/** @name RoutedEvents */
	/** @{ */

	UIEvent<UIMouseEventArgs>		MouseEnter;		/**< マウスポインタがこの要素の境界内に入ったときに発生します。*/
	UIEvent<UIMouseEventArgs>		MouseLeave;		/**< マウスポインタがこの要素の境界から出たときに発生します。*/
	UIEvent<UIMouseEventArgs>		MouseMove;		/**< マウスポインタがこの要素上で移動すると発生します。 */
	UIEvent<UIMouseEventArgs>		MouseDown;		/**< マウスポインタがこの要素上にあるときに任意のマウスボタンが押されると発生します。*/
	UIEvent<UIMouseEventArgs>		MouseUp;		/**< マウスポインタがこの要素上にあるときに任意のマウスボタンが離されると発生します。*/
	UIEvent<UIKeyEventArgs>			KeyDown;		/**< この要素がフォーカスを持っている時にキーボードのキーが押されると発生します。*/
	UIEvent<UIKeyEventArgs>			KeyUp;			/**< この要素がフォーカスを持っている時にキーボードのキーが離されると発生します。*/
	UIEvent<UIKeyEventArgs>			TextInput;		/**< キー操作により文字が入力されると発生します。*/

	/** @} */

	//--------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetVerticalAlignment(VerticalAlignment value) { tr::Property::SetPropertyValueDirect<VerticalAlignment>(this, VerticalAlignmentProperty, value); }
	VerticalAlignment GetVerticalAlignment() const { return tr::Property::GetPropertyValueDirect<VerticalAlignment>(this, VerticalAlignmentProperty); }

	void SetHorizontalAlignment(HorizontalAlignment value) { tr::Property::SetPropertyValueDirect<HorizontalAlignment>(this, HorizontalAlignmentProperty, value); }
	HorizontalAlignment GetHorizontalAlignment() const { return tr::Property::GetPropertyValueDirect<HorizontalAlignment>(this, HorizontalAlignmentProperty); }


	/** @} */


public:




	/** 要素の識別名を取得します。*/
	const String& GetKeyName() const { return m_keyName; }

	/** 論理上の親要素を取得します。*/
	UIElement* GetParent() const { return m_parent; }

	void SetSize(const SizeF& size) { m_size = size; }

	/** この要素がフォーカスを得ることができるかを確認します。*/
	virtual bool IsFocusable() const { return false; }

	/** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

	/** この要素の最終的な描画サイズを取得します。この値は Arrange() で確定します。*/
	const SizeF& GetRenderSize() const { return m_finalLocalRect.GetSize(); }

	/** この要素へのフォーカスの取得を試みます。*/
	void Focus();

	///** マウスキャプチャを設定します。*/
	//void CaptureMouse();

	///** マウスキャプチャを解除します。*/
	//void ReleaseMouseCapture();

	/** この要素内の子ビジュアル要素の数を取得します。(論理要素も含めたすべての子要素) */
	virtual int GetVisualChildrenCount() const;

	/** この要素内の指定したインデックスにある子ビジュアル要素を取得します。*/
	//virtual UIElement* GetVisualChild(int index) const;

	/** Zオーダーやアクティブ状態を考慮した順で、子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。*/
	virtual UIElement* GetVisualChildOrderd(int index) const;

	/** この要素が関連付けられている UILayoutView を取得します。*/
	UILayoutView* GetOwnerLayoutView() const { return m_ownerLayoutView; }


	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e);

	void ApplyTemplateHierarchy(UIStyleTable* styleTable, UIStyle* parentStyle);

	float GetActualWidth() const { return m_finalLocalRect.width; }
	float GetActualHeight() const { return m_finalLocalRect.height; }

protected:
	UIElement();
	virtual ~UIElement();
	void Initialize(detail::UIManager* manager);

	/** 要素の視覚状態を切り替えます。*/
	void GoToVisualState(const StringRef& stateName);

	/** 指定した要素をこの要素にビジュアル子要素として追加します。*/
	//void AddVisualChild(UIElement* element);

	/** 指定した要素をこの要素のビジュアルツリーから削除します。*/
	//void RemoveVisualChild(UIElement* element);

	/** フォントによるレイアウト情報を無効化します。次のレイアウト更新パスで、フォント情報が再構築されます。*/
	void InvalidateFont() { m_invalidateFlags |= detail::InvalidateFlags::Font; }
	
	/**
		@brief		この要素を表示するために必要なサイズを計測します。
		@params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
		@return		この要素のレイアウトの際に必要となる最低限のサイズ。この要素のサイズと、全ての子要素のサイズに基づき決定します。Inf であってはなりません。
		@details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
	*/
	virtual SizeF MeasureOverride(const SizeF& constraint);

	/**
		@brief		子要素の配置を確定し、この要素の最終サイズを返します。
		@param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
		@return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
		@details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
	*/
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	/** この要素のレイアウトの更新が完了した時に呼び出されます。*/
	virtual void OnLayoutUpdated();

	/**
		@brief	この要素の描画を行います。
	*/
	virtual void OnRender(GraphicsContext* g);

	virtual void OnMouseMove(UIMouseEventArgs* e);
	virtual void OnMouseDown(UIMouseEventArgs* e);
	virtual void OnMouseUp(UIMouseEventArgs* e);
	virtual void OnMouseEnter(UIMouseEventArgs* e);
	virtual void OnMouseLeave(UIMouseEventArgs* e);
	virtual void OnKeyDown(UIKeyEventArgs* e);
	virtual void OnKeyUp(UIKeyEventArgs* e);
	virtual void OnTextInput(UIKeyEventArgs* e);

	virtual void OnUpdateStyle(UIStyle* localStyle, detail::InvalidateFlags invalidateFlags);
	virtual void OnUpdatingLayout();

	UIStyle* GetLocalStyle() const { return m_localStyle; }

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	void SetParent(UIElement* parent, UILayoutView* ownerLayoutView);
	UIElement* CheckMouseHoverElement(const PointF& globalPt);
	void ActivateInternal(UIElement* child);
	virtual bool OnEvent(detail::UIInternalEventType type, UIEventArgs* args);
	void UpdateLayout();
	void UpdateTransformHierarchy();
	void Render(GraphicsContext* g);

private:
	void UpdateLocalStyleAndApplyProperties(UIStyle* parentStyle, UIStyle* currentStateStyle);

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e);

	detail::UIManager*		m_manager;
	UILayoutView*			m_ownerLayoutView;
	String					m_keyName;
	UIElement*				m_parent;
	UIStyle*				m_localStyle;
	SizeF					m_desiredSize;			// MeasureLayout() で決定されるこのコントロールの要求サイズ
	RectF					m_finalLocalRect;		// 描画に使用する最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	RectF					m_finalGlobalRect;
	String					m_elementName;				// 要素名 ("UITextBlock" など)
	String					m_currentVisualStateName;
	UIStyle*				m_currentVisualStateStyle;

	// Property
	//		これらには直接値を設定しないこと。Property::SetValueDirect() を使う。
	//		これによって必要にアニメーションを止めたりできる。
	SizeF					m_size;
	ThicknessF				m_margin;
	ThicknessF				m_padding;
	HorizontalAlignment		m_horizontalAlignment;
	VerticalAlignment		m_verticalAlignment;
	BrushPtr				m_background;
	BrushPtr				m_foreground;
	//detail::BorderInfo				m_border;



	float							m_opacity;
	//ToneF							m_tone;

	//RefPtr<Style>					m_style;
	float					m_combinedOpacity;
	//Array< RefPtr<AnimationClock> >	m_animationClockList;
	detail::InvalidateFlags	m_invalidateFlags;
	bool					m_isEnabled;
	bool					m_isMouseOver;
};

LN_NAMESPACE_END
