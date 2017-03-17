
#pragma once
#include "../Base/GeometryStructs.h"
#include "../Animation/AnimatableObject.h"
#include "Common.h"
#include "Detail.h"
#include "UITypeInfo.h"
#include "UIEvent.h"
#include "LayoutElement.h"

LN_NAMESPACE_BEGIN
class DrawList;
class UIStylePropertyTable;
class UIVisualStateManager;

class UIVisualStateManager
	: public Object
{
private:
	struct Group
	{
		int				index;
		String			name;
		List<String>	stateNames;
	};

	List<Group>		groups;
	List<String>	activeStateNames;

public:
	void RegisterVisualState(const StringRef& groupName, const StringRef& stateName)
	{
		Group* group = groups.Find([groupName](const Group& g) { return g.name == groupName; });
		if (group == nullptr)
		{
			groups.Add(Group{ groups.GetCount(), groupName });
			activeStateNames.Add(stateName);
			group = &groups.GetLast();
		}
		group->stateNames.Add(stateName);
	}

	void GoToVisualState(const StringRef& stateName)
	{
		Group* group = FindGroup(stateName);//groups.Find([stateName](const Group& g) { return g.stateNames.Contains([stateName](const String& name) { return name == stateName; }); });
		LN_FAIL_CHECK_STATE(group != nullptr) return;
		activeStateNames[group->index] = stateName;
	}

LN_CONSTRUCT_ACCESS:
	void Initialize()
	{
	}

	Group* FindGroup(const StringRef& stateName)
	{
		for (auto& g : groups)
		{
			for (auto& s : g.stateNames)
			{
				if (s == stateName)
					return &g;
			}
		}
		return nullptr;
	}

LN_INTERNAL_ACCESS:
	const List<String>& GetActiveStateNames() const { return activeStateNames; }
};

/**
	@brief		
	@details	
*/
class UIElement
	: public AnimatableObject
	, public ILayoutElement
{
	LN_UI_TYPEINFO_DECLARE();

public:
	LN_TR_PROPERTY(PointF,				position);				/**< Position プロパティの識別子 */
	LN_TR_PROPERTY(float,				width);
	LN_TR_PROPERTY(float,				height);
	LN_TR_PROPERTY(ThicknessF,			margin);
	LN_TR_PROPERTY(ThicknessF,			padding);
	LN_TR_PROPERTY(AlignmentAnchor,		anchor);				/**< Anchor プロパティの識別子 */
	LN_TR_PROPERTY(HAlignment,			hAlignment);			/**< HAlignment プロパティの識別子 */
	LN_TR_PROPERTY(VAlignment,			vAlignment);			/**< VAlignment プロパティの識別子 */
	LN_TR_PROPERTY(BrushPtr,			background);			/**< Background プロパティの識別子 */
	LN_TR_PROPERTY(BrushPtr,			foreground);			/**< Foreground プロパティの識別子 */
	LN_TR_PROPERTY(BrushPtr,			decoratorBackground);	/**< DecoratorBackground プロパティの識別子 */
	LN_TR_PROPERTY(float,				decoratorOpacity);		/**< Foreground プロパティの識別子 */
	tr::Property<PointF>				position;
	tr::Property<float>					width;
	tr::Property<float>					height;
	tr::Property<ThicknessF>			margin;
	tr::Property<ThicknessF>			padding;
	tr::Property<AlignmentAnchor>		anchor;

	tr::Property<HAlignment>			hAlignment;
	tr::Property<VAlignment>			vAlignment;
	tr::Property<BrushPtr>				background;
	tr::Property<BrushPtr>				foreground;
	tr::Property<BrushPtr>				decoratorBackground;
	tr::Property<float>					decoratorOpacity;

	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseEnterEvent);				/**< MouseEnter ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseLeaveEvent);				/**< MouseLeave ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseMoveEvent);				/**< MouseMove ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseDownEvent);				/**< MouseDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	MouseUpEvent);					/**< MouseUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	KeyDownEvent);					/**< KeyDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIMouseEventArgs,	KeyUpEvent);					/**< KeyUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIKeyEventArgs,		TextInputEvent);				/**< TextInput ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(UIEventArgs, GotFocusEvent);
	LN_ROUTED_EVENT(UIEventArgs, LostFocusEvent);

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
	UIEvent<UIEventArgs>			GotFocus;
	UIEvent<UIEventArgs>			LostFocus;

	/** @} */

	//--------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetPosition(const PointF& value) { tr::PropertyInfo::SetPropertyValueDirect<PointF>(this, positionId, value); }
	const PointF& GetPosition() const { return tr::PropertyInfo::GetPropertyValueDirect<PointF>(this, positionId); }

	void SetWidth(float value) { tr::PropertyInfo::SetPropertyValueDirect<float>(this, widthId, value); }
	float GetWidth() const { return tr::PropertyInfo::GetPropertyValueDirect<float>(this, widthId); }

	void SetHeight(float value) { tr::PropertyInfo::SetPropertyValueDirect<float>(this, heightId, value); }
	float GetHeight() const { return tr::PropertyInfo::GetPropertyValueDirect<float>(this, heightId); }

	void SetSize(const Size& value) { SetWidth(value.width); SetHeight(value.height); }

	void SetAnchor(AlignmentAnchor value) { tr::PropertyInfo::SetPropertyValueDirect<AlignmentAnchor>(this, anchorId, value); }
	AlignmentAnchor GetAnchor() const { return tr::PropertyInfo::GetPropertyValueDirect<AlignmentAnchor>(this, anchorId); }

	void SetHAlignment(HAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<HAlignment>(this, hAlignmentId, value); }
	HAlignment GetHAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<HAlignment>(this, hAlignmentId); }

	void SetVAlignment(VAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<VAlignment>(this, vAlignmentId, value); }
	VAlignment GetVAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<VAlignment>(this, vAlignmentId); }

	void SetBackground(Brush* value) { tr::PropertyInfo::SetPropertyValueDirect<BrushPtr>(this, backgroundId, value); }
	Brush* GetBackground() const { return tr::PropertyInfo::GetPropertyValueDirect<BrushPtr>(this, backgroundId); }


	/** @} */


public:


	/** 要素の識別名を取得します。*/
	const String& GetKeyName() const { return m_keyName; }

	/** 論理上の親要素を取得します。*/
	UIElement* GetParent() const { return m_parent; }

	//void SetSize(const Size& size) { m_size = size; }

	/** この要素がフォーカスを得ることができるかを確認します。*/
	virtual bool IsFocusable() const { return false; }

	/** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
	const Size& GetDesiredSize() const { return m_desiredSize; }

	/** この要素の最終的な描画サイズを取得します。この値は Arrange() で確定します。*/
	const Size& GetRenderSize() const { return m_finalLocalRect.GetSize(); }

	/** この要素へのフォーカスの取得を試みます。*/
	void Focus();

	/** マウスキャプチャを設定します。*/
	void CaptureMouse();

	/** マウスキャプチャを解除します。*/
	void ReleaseMouseCapture();

	/** この要素内の子ビジュアル要素の数を取得します。(論理要素も含めたすべての子要素) */
	virtual int GetVisualChildrenCount() const override;

	/** Zオーダーやアクティブ状態を考慮した順で、子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。*/
	virtual ILayoutElement* GetVisualChild(int index) const override;

	/** この要素が関連付けられている UILayoutView を取得します。*/
	//UILayoutView* GetOwnerLayoutView() const { return m_ownerLayoutView; }


	virtual void MeasureLayout(const Size& availableSize) override;
	virtual void ArrangeLayout(const RectF& finalLocalRect) override;

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e);

	void ApplyTemplateHierarchy(UIStyleTable* styleTable, UIStylePropertyTable* parentStyle);

	float GetActualWidth() const { return m_finalLocalRect.width; }
	float GetActualHeight() const { return m_finalLocalRect.height; }

	//--------------------------------------------------------------------------
	/** @name Grid layout */
	/** @{ */

	void SetLayoutColumn(int index);
	virtual int GetLayoutColumn() const override;
	void SetLayoutRow(int index);
	virtual int GetLayoutRow() const override;
	void SetLayoutColumnSpan(int span);
	virtual int GetLayoutColumnSpan() const override;
	void SetLayoutRowSpan(int span);
	virtual int GetLayoutRowSpan() const override;

	/** @} */

protected:
	UIElement();
	virtual ~UIElement();
	void Initialize(detail::UIManager* manager);

	/** 要素の視覚状態を切り替えます。*/
	void GoToVisualState(const StringRef& stateName);

	/** フォントによるレイアウト情報を無効化します。次のレイアウト更新パスで、フォント情報が再構築されます。*/
	void InvalidateFont() { m_invalidateFlags |= detail::InvalidateFlags::Font; }
	
	/**
		@brief		この要素を表示するために必要なサイズを計測します。
		@params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
		@return		この要素のレイアウトの際に必要となる最低限のサイズ。この要素のサイズと、全ての子要素のサイズに基づき決定します。Inf であってはなりません。
		@details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
	*/
	virtual Size MeasureOverride(const Size& constraint) override;

	/**
		@brief		子要素の配置を確定し、この要素の最終サイズを返します。
		@param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
		@return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
		@details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
					また、finalSize には padding プロパティの余白は考慮されません。
					この余白を正しく反映するためには派生クラスで padding プロパティを参照し、子要素の位置を計算します。

					親要素は、各子要素の Arrange を呼び出し、適切に配置する必要があります。
					そうでない場合、子要素はレンダリングされません。(UIElement::ArrangeOverride() は、子要素の配置は行いません)
	*/
	virtual Size ArrangeOverride(const Size& finalSize) override;

	/** この要素のレイアウトの更新が完了した時に呼び出されます。*/
	virtual void OnLayoutUpdated();

	/**
		@brief	この要素の描画を行います。
	*/
	virtual void OnRender(DrawList* g);

	virtual void OnMouseMove(UIMouseEventArgs* e);
	virtual void OnMouseDown(UIMouseEventArgs* e);
	virtual void OnMouseUp(UIMouseEventArgs* e);
	virtual void OnMouseEnter(UIMouseEventArgs* e);
	virtual void OnMouseLeave(UIMouseEventArgs* e);
	virtual void OnKeyDown(UIKeyEventArgs* e);
	virtual void OnKeyUp(UIKeyEventArgs* e);
	virtual void OnTextInput(UIKeyEventArgs* e);
	virtual void OnGotFocus(UIEventArgs* e);
	virtual void OnLostFocus(UIEventArgs* e);

	virtual void OnUpdateStyle(UIStylePropertyTable* localStyle, detail::InvalidateFlags invalidateFlags);
	virtual void OnUpdatingLayout();

	UIStylePropertyTable* GetLocalStyle() const { return m_localStyle; }

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	UIContext* GetContext() const;
	const PointF& GetPositionInternal() const { return position; }
	Size GetSizeInternal() const { return Size(width, height); }
	const ThicknessF& GetMargineInternal() const { return margin; }
	AlignmentAnchor GetAnchorInternal() const { return anchor; }
	const BrushPtr& GetForegroundInternal() const { return foreground; }
	void SetParent(UIElement* parent);
	UIVisualStateManager* GetVisualStateManager();
	void SetStyleSubControlName(const StringRef& ownerControlName, const StringRef& subControlName) { m_styleSubControlOwnerName = ownerControlName; m_styleSubControlName = subControlName; }
	//const String& GetStyleSubControlName() const { return m_styleSubControlName; }
	//const String& GetCurrentVisualStateName() const { return m_currentVisualStateName; }
	//AnchorInfo* GetAnchorInfo() {return &m_anchorInfo; }
	detail::InvalidateFlags GetInvalidateFlags() const { return m_invalidateFlags; }
	UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ActivateInternal(UIElement* child);
	virtual bool OnEvent(detail::UIInternalEventType type, UIEventArgs* args);
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e);
	void CallOnGotFocus();
	void CallOnLostFocus();


	void UpdateLayout(const Size& viewSize) override;
	void Render(DrawList* g);

protected:
	virtual void UpdateTransformHierarchy(const RectF& parentGlobalRect) override;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual const HAlignment* GetPriorityContentHAlignment();
	virtual const VAlignment* GetPriorityContentVAlignment();
	//virtual void GetStyleClassName(String* outSubStateName);

	/** 指定した要素をこの要素にビジュアル子要素として追加します。*/
	void AddVisualChild(UIElement* element);

	/** 指定した要素をこの要素のビジュアルツリーから削除します。*/
	void RemoveVisualChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	// ILayoutElement interface
	virtual const PointF& GetLayoutPosition() const override;
	virtual Size GetLayoutSize() const override;
	virtual const ThicknessF& GetLayoutMargin() const override;
	virtual const ThicknessF& GetLayoutPadding() const override;
	virtual AlignmentAnchor GetLayoutAnchor() const override;
	virtual HAlignment GetLayoutHAlignment() const override;
	virtual VAlignment GetLayoutVAlignment() const override;
	virtual ILayoutElement* GetLayoutParent() const override;
	virtual const HAlignment* GetLayoutContentHAlignment() override;
	virtual const VAlignment* GetLayoutContentVAlignment() override;
	virtual const Size& GetLayoutDesiredSize() const override;
	virtual void SetLayoutDesiredSize(const Size& size) override;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) override;
	virtual const RectF& GetLayoutFinalLocalRect() const override;
	virtual void SetLayoutFinalGlobalRect(const RectF& rect) override;


private:
	void UpdateLocalStyleAndApplyProperties(UIStyleTable* styleTable, UIStylePropertyTable* parentStyle);

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e);

	detail::UIManager*		m_manager;
	//UILayoutView*			m_ownerLayoutView;
	String					m_keyName;
	UIElement*				m_parent;	// TODO: logical parent
	UIStylePropertyTable*	m_localStyle;			// 内部的に使用されるスタイル。親や VisualState から取得したスタイルをマージしたもの。
	Size					m_desiredSize;			// MeasureLayout() で決定されるこのコントロールの要求サイズ
	RectF					m_finalLocalRect;		// 描画に使用する最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	RectF					m_finalGlobalRect;
	String					m_elementName;				// 要素名 ("UITextBlock" など) TODO: いらないかも
	RefPtr<UIVisualStateManager>	m_visualStateManager;
	String							m_styleSubControlOwnerName;
	String							m_styleSubControlName;
	//String					m_currentVisualStateName;
	UIStylePropertyTable*	m_currentVisualStateStyle;

	UIElement*                m_visualParent;
	std::shared_ptr<List<RefPtr<UIElement>>>    m_visualChildren;

	// Property
	//		これらには直接値を設定しないこと。Property::SetValueDirect() を使う。
	//		これによって必要にアニメーションを止めたりできる。
	//tr::Property<PointF>	m_position;
	//tr::Property<Size>		m_size;
	//tr::Property<AlignmentAnchor>	m_anchor;
	//tr::Property<HAlignment>		m_horizontalAlignment;
	//tr::Property<VAlignment>		m_verticalAlignment;
	detail::GridLayoutInfo	m_gridLayoutInfo;

	//tr::Property<BrushPtr>			m_background;
	//tr::Property<BrushPtr>			m_foreground;
	//detail::BorderInfo				m_border;



	float							m_opacity;
	//ToneF							m_tone;

	//tr::Property<BrushPtr>				m_decoratorBackground;
	//tr::Property<float>					m_decoratorOpacity;


	//RefPtr<Style>					m_style;
	float					m_combinedOpacity;
	//AnchorInfo				m_anchorInfo;
	detail::InvalidateFlags	m_invalidateFlags;
	bool					m_isEnabled;
	bool					m_isMouseOver;
};

LN_NAMESPACE_END
