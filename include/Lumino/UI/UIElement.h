
#pragma once
#include "Common.h"
#include "UITypeInfo.h"
#include "UIEvent.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	
*/
class UIElement
	: public tr::ReflectionObject
{
	LN_UI_TYPEINFO_DECLARE();

public:
	LN_ROUTED_EVENT(MouseEventArgs,		MouseEnterEvent);				/**< MouseEnter ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseLeaveEvent);				/**< MouseLeave ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseMoveEvent);				/**< MouseMove ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseDownEvent);				/**< MouseDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseUpEvent);					/**< MouseUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		KeyDownEvent);					/**< KeyDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		KeyUpEvent);					/**< KeyUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(KeyEventArgs,		TextInputEvent);				/**< TextInput ルーティングイベントの識別子 */

	//-------------------------------------------------------------------------
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

public:

	/** 要素の識別名を取得します。*/
	const String& GetKeyName() const { return m_keyName; }

	/** 論理上の親要素を取得します。*/
	UIElement* GetParent() const { return m_parent; }

	/** この要素がフォーカスを得ることができるかを確認します。*/
	virtual bool IsFocusable() const { return false; }

	/** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

	/** この要素の最終的な描画サイズを取得します。この値は Arrange() で確定します。*/
	const SizeF& GetRenderSize() const { return m_finalLocalRect.GetSize(); }

	/** この要素へのフォーカスの取得を試みます。*/
	void Focus();

	/** マウスキャプチャを設定します。*/
	void CaptureMouse();

	/** マウスキャプチャを解除します。*/
	void ReleaseMouseCapture();

	/** この要素内の子ビジュアル要素の数を取得します。*/
	virtual int GetVisualChildrenCount() const;

	/** この要素内の指定したインデックスにある子ビジュアル要素を取得します。*/
	virtual UIElement* GetVisualChild(int index) const;

	/** Zオーダーやアクティブ状態を考慮した順で、子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。*/
	virtual UIElement* GetVisualChildOrderd(int index) const;

	/** この要素が関連付けられている UILayoutView を取得します。*/
	UILayoutView* GetLayoutView() const;

	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e);
	
protected:
	UIElement();
	virtual ~UIElement();

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
	//virtual void OnRender(RenderingContext* painter);

	virtual void OnMouseMove(UIMouseEventArgs* e);
	virtual void OnMouseDown(UIMouseEventArgs* e);
	virtual void OnMouseUp(UIMouseEventArgs* e);
	virtual void OnMouseEnter(UIMouseEventArgs* e);
	virtual void OnMouseLeave(UIMouseEventArgs* e);
	virtual void OnKeyDown(UIKeyEventArgs* e);
	virtual void OnKeyUp(UIKeyEventArgs* e);
	virtual void OnTextInput(UIKeyEventArgs* e);

LN_INTERNAL_ACCESS:
	UIElement* CheckMouseHoverElement(const PointF& globalPt);
	void ActivateInternal(UIElement* child);
	virtual bool OnEvent(detail::UIInternalEventType type, UIEventArgs* args);
	void ApplyTemplate();
	void ApplyTemplateHierarchy();
	void UpdateLayout();
	void UpdateTransformHierarchy();

private:
	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e);

	UILayoutView*			m_ownerLayoutView;
	String					m_keyName;
	UIElement*				m_parent;
	SizeF					m_desiredSize;			// MeasureLayout() で決定されるこのコントロールの要求サイズ
	RectF					m_finalLocalRect;		// 描画に使用する最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	RectF					m_finalGlobalRect;

	// Property
	SizeF							m_size;
	ThicknessF						m_margin;
	float							m_opacity;
	//ToneF							m_tone;
	HorizontalAlignment				m_horizontalAlignment;
	VerticalAlignment				m_verticalAlignment;

	//RefPtr<Style>					m_style;
	float							m_combinedOpacity;
	//Array< RefPtr<AnimationClock> >	m_animationClockList;
	detail::InvalidateFlags			m_invalidateFlags;
	bool							m_isEnabled;
	bool							m_isMouseOver;
};

LN_NAMESPACE_END
