
#pragma once
#include <Lumino/Base/Event.h>
#include "../Base/GeometryStructs.h"
#include "../Graphics/Material.h"
#include "Common.h"
#include "Detail.h"
#include "UITypeInfo.h"
#include "UIEvent.h"
#include "LayoutElement.h"

LN_NAMESPACE_BEGIN
class DrawingContext;
class UIStylePropertyTable;
class UIVisualStateManager;
namespace detail {
class UIStylePropertyTableInstance;
class LayoutHelper2;

enum UICoreFlags
{
	UICoreFlags_None			= 0x0000,
	UICoreFlags_LayoutVisible	= 0x0001,
};

} // namespace detail

enum class UISpecialElementType
{
	None,
	FrameWindow,
};


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
	void registerVisualState(const StringRef& groupName, const StringRef& stateName)
	{
		Group* group = groups.find([groupName](const Group& g) { return g.name == groupName; });
		if (group == nullptr)
		{
			groups.add(Group{ groups.getCount(), groupName });
			activeStateNames.add(stateName);
			group = &groups.getLast();
		}
		group->stateNames.add(stateName);
	}

	void goToVisualState(const StringRef& stateName)
	{
		Group* group = findGroup(stateName);//groups.Find([stateName](const Group& g) { return g.stateNames.Contains([stateName](const String& name) { return name == stateName; }); });
		if (LN_CHECK_STATE(group != nullptr)) return;
		activeStateNames[group->index] = stateName;
	}

LN_CONSTRUCT_ACCESS:
	void initialize()
	{
	}

	Group* findGroup(const StringRef& stateName)
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
	const List<String>& getActiveStateNames() const { return activeStateNames; }
};

class UIVisualStates
{
public:
	static const String CommonGroup;
	static const String FocusGroup;
	//static const String ValidationStates;

	static const String NormalState;
	static const String MouseOverState;
	static const String PressedState;
	static const String DisabledState;
	static const String UnfocusedState;
	static const String FocusedState;
	static const String ValidState;
	static const String InvalidState;


	static const String OrientationGroup;
	static const String HorizontalState;
	static const String VerticalState;
};

/**
	@brief		
	@details	
*/
class UIElement
	: public RuntimeResource
	, public ILayoutElement
{
	LN_OBJECT;

public:
	Point				position;
	float					width;
	float					height;
	//Thickness		margin;
	//Thickness			padding;
	AlignmentAnchor		anchor;

	HAlignment			hAlignment;
	VAlignment			vAlignment;
	//BrushPtr				background;
	BrushPtr				foreground;
	BrushPtr				decoratorBackground;
	float					decoratorOpacity;

	// Border
	//Thickness			borderThickness;
	//CornerRadius		cornerRadius;
	//Color					leftBorderColor;
	//Color					topBorderColor;
	//Color					rightBorderColor;
	//Color					bottomBorderColor;
	//BorderDirection		borderDirection;

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

	void setPosition(const Point& value) { position = value; }
	const Point& getPosition() const { return position; }

	void setWidth(float value) { width = value; }
	float getWidth() const { return width; }

	void setHeight(float value) { height = value; }
	float getHeight() const { return height; }

	void setSize(const Size& value) { setWidth(value.width); setHeight(value.height); }
	Size getSize() const { return Size(width, height); }

	/** 要素の margin 値 (外側の余白) を設定します。 */
	void setMargin(const Thickness& margin);

	/** 要素の margin 値 (外側の余白) を取得します。 */
	const Thickness& getMargin() const;

	/** 要素の padding 値 (内側の余白) を設定します。 */
	void setPadding(const Thickness& padding);

	/** 要素の padding 値 (内側の余白) を取得します。 */
	const Thickness& getPadding() const;

	void setMinWidth(float value) { m_minSize.width = value; }

	void setMinHeight(float value) { m_minSize.height = value; }

	void setMaxWidth(float value) { m_maxSize.width = value; }

	void setMaxHeight(float value) { m_maxSize.height = value; }

	void setAnchor(AlignmentAnchor value) { anchor = value; }
	AlignmentAnchor getAnchor() const { return anchor; }

	void setHAlignment(HAlignment value) { hAlignment = value; }
	HAlignment getHAlignment() const { return hAlignment; }

	void setVAlignment(VAlignment value) { vAlignment = value; }
	VAlignment getVAlignment() const { return vAlignment; }

	void setBackground(Brush* value);
	Brush* getBackground() const;


	void setOpacity(float value) { m_builtinEffectData.setOpacity(value); }
	float getOpacity() const { return m_builtinEffectData.getOpacity(); }

	/** @} */


public:


	/** 要素の識別名を取得します。*/
	const String& getKeyName() const { return m_keyName; }

	/** 論理上の親要素を取得します。*/
	UIElement* getLogicalParent() const { return m_logicalParent; }

	//void setSize(const Size& size) { m_size = size; }

	void setFocusable(bool value) { m_isFocusable = value; }

	/** この要素がフォーカスを得ることができるかを確認します。*/
	bool isFocusable() const { return m_isFocusable; }

	void setHitTestVisible(bool value) { m_isHitTestVisible = value; }

	bool isHitTestVisible() const { return m_isHitTestVisible; }

	/** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
	const Size& getDesiredSize() const { return m_desiredSize; }

	/** この要素の最終的な描画サイズを取得します。この値は Arrange() で確定します。*/
	Size getActualSize() const { return m_finalLocalActualRect.getSize(); }

	/** この要素へのフォーカスの取得を試みます。*/
	void focus();

	/** マウスキャプチャを設定します。*/
	void captureMouse();

	/** マウスキャプチャを解除します。*/
	void releaseMouseCapture();

	/** この要素内の子ビジュアル要素の数を取得します。(論理要素も含めたすべての子要素) */
	virtual int getVisualChildrenCount() const override;

	/** Zオーダーやアクティブ状態を考慮した順で、子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。*/
	virtual ILayoutElement* getVisualChild(int index) const override;

	/** この要素が関連付けられている UILayoutView を取得します。*/
	//UILayoutView* GetOwnerLayoutView() const { return m_ownerLayoutView; }


	virtual void measureLayout(const Size& availableSize) override;
	virtual void arrangeLayout(const Rect& finalLocalRect) override;

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void raiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e);

	void applyTemplateHierarchy(UIStyleTable* styleTable, detail::UIStylePropertyTableInstance* parentStyle);

	float getActualWidth() const { return m_finalLocalActualRect.width; }
	float getActualHeight() const { return m_finalLocalActualRect.height; }

	bool hasFocus() const { return m_hasFocus; }

	//--------------------------------------------------------------------------
	/** @name Grid layout */
	/** @{ */

	void setLayoutColumn(int index);
	virtual int getLayoutColumn() const override;
	void setLayoutRow(int index);
	virtual int getLayoutRow() const override;
	void setLayoutColumnSpan(int span);
	virtual int getLayoutColumnSpan() const override;
	void setLayoutRowSpan(int span);
	virtual int getLayoutRowSpan() const override;

	/** @} */

	
	/** onGotFocus イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnGotFocus(UIEventHandler handler);
	
	/** onLostFocus イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnLostFocus(UIEventHandler handler);

protected:
	UIElement();
	virtual ~UIElement();
	void initialize();

	/** 要素の視覚状態を切り替えます。*/
	void goToVisualState(const StringRef& stateName);

	/** フォントによるレイアウト情報を無効化します。次のレイアウト更新パスで、フォント情報が再構築されます。*/
	void InvalidateFont() { m_invalidateFlags |= detail::InvalidateFlags::Font; }
	
	/**
		@brief		この要素を表示するために必要なサイズを計測します。
		@params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
		@return		この要素のレイアウトの際に必要となる最低限のサイズ。
					この要素のサイズと、全ての子要素のサイズに基づき決定します。NaN や Inf であってはなりません。
		@details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
	*/
	virtual Size measureOverride(const Size& constraint) override;

	/**
		@brief		Visual 子要素の配置を確定し、この要素の最終サイズを返します。
		@param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
		@return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
		@details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
					また、finalSize には padding プロパティの余白は考慮されません。
					この余白を正しく反映するためには派生クラスで padding プロパティを参照し、子要素の位置を計算します。

					親要素は、各子要素の Arrange を呼び出し、適切に配置する必要があります。
					そうでない場合、子要素はレンダリングされません。(UIElement::arrangeOverride() は、子要素の配置は行いません)
	*/
	virtual Size arrangeOverride(const Size& finalSize) override;

	virtual void onUpdateFrame();

	/** この要素のレイアウトの更新が完了した時に呼び出されます。*/
	virtual void onLayoutUpdated();

	/**
		@brief	この要素の描画を行います。
	*/
	virtual void onRender(DrawingContext* g);

	virtual void onMouseMove(UIMouseEventArgs* e);
	virtual void onMouseDown(UIMouseEventArgs* e);
	virtual void onMouseUp(UIMouseEventArgs* e);
	virtual void onMouseEnter(UIMouseEventArgs* e);
	virtual void onMouseLeave(UIMouseEventArgs* e);
	virtual void onKeyDown(UIKeyEventArgs* e);
	virtual void onKeyUp(UIKeyEventArgs* e);
	virtual void onTextInput(UIKeyEventArgs* e);
	virtual void onGotFocus(UIEventArgs* e);
	virtual void onLostFocus(UIEventArgs* e);

	virtual void onUpdateStyle(detail::UIStylePropertyTableInstance* localStyle, detail::InvalidateFlags invalidateFlags);
	virtual void onUpdatingLayout();

	virtual bool onHitTest(const Point& localPoint);

	//UIStylePropertyTable* GetLocalStyle() const { return m_localStyle; }

	virtual bool onEvent(detail::UIInternalEventType type, UIEventArgs* args);
	virtual void onRoutedEvent(UIEventArgs* e);
	virtual void updateLayout(const Size& viewSize) override;
	virtual detail::SpcialUIElementType getSpcialUIElementType() const;

public:
	virtual UIElement* checkMouseHoverElement(const Point& globalPt);

LN_INTERNAL_ACCESS:
	detail::UIManager* getManager() const { return m_manager; }
	UIContext* getContext() const;
	const Point& getPositionInternal() const { return position; }
	void setSizeInternal(const Size& size) { width = size.width; height = size.height; }
	Size getSizeInternal() const { return Size(width, height); }
	const Thickness& getMargineInternal() const;
	AlignmentAnchor getAnchorInternal() const { return anchor; }
	const BrushPtr& getForegroundInternal() const { return foreground; }
	void setLogicalParent(UIElement* parent);
	UIVisualStateManager* getVisualStateManager();
	void setStyleSubControlName(const StringRef& ownerControlName, const StringRef& subControlName) { m_styleSubControlOwnerName = ownerControlName; m_styleSubControlName = subControlName; }
	//const String& GetStyleSubControlName() const { return m_styleSubControlName; }
	//const String& GetCurrentVisualStateName() const { return m_currentVisualStateName; }
	//AnchorInfo* GetAnchorInfo() {return &m_anchorInfo; }
	detail::InvalidateFlags getInvalidateFlags() const { return m_invalidateFlags; }
	void callOnGotFocus();
	void callOnLostFocus();
	const Rect& getFinalGlobalRect() const { return m_finalGlobalRect; }
	UIElement* getVisualParent() const { return m_visualParent; }

	void setSpecialElementType(UISpecialElementType type) { m_specialElementType = type; }
	UISpecialElementType getSpecialElementType2() const { return m_specialElementType; }

	bool readCoreFlag(detail::UICoreFlags field) const;
	void writeCoreFlag(detail::UICoreFlags field, bool value);

	void updateFrame();
	void render(DrawingContext* g);

protected:
	virtual void updateTransformHierarchy(const Rect& parentGlobalRect) override;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual const HAlignment* getPriorityContentHAlignment();
	virtual const VAlignment* getPriorityContentVAlignment();
	//virtual void GetStyleClassName(String* outSubStateName);

	/** 指定した要素をこの要素にビジュアル子要素として追加します。*/
	void addVisualChild(UIElement* element);

	/** 指定した要素をこの要素のビジュアルツリーから削除します。*/
	void removeVisualChild(UIElement* element);

	void setRenderFrameThickness(const Thickness& value) { m_renderFrameThickness = value; }

private:
	// ILayoutElement interface
	virtual const Point& getLayoutPosition() const override;
	virtual Size getLayoutSize() const override;
	virtual const Thickness& getLayoutMargin() const override;
	virtual const Thickness& getLayoutPadding() const override;
	virtual AlignmentAnchor getLayoutAnchor() const override;
	virtual HAlignment getLayoutHAlignment() const override;
	virtual VAlignment getLayoutVAlignment() const override;
	virtual void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const override;
	virtual ILayoutElement* getLayoutParent() const override;
	virtual const HAlignment* getLayoutContentHAlignment() override;
	virtual const VAlignment* getLayoutContentVAlignment() override;
	virtual const Size& getLayoutDesiredSize() const override;
	virtual void setLayoutDesiredSize(const Size& size) override;
	virtual void setLayoutFinalLocalRect(const Rect& renderRect/*, const Rect& contentRect*/) override;
	virtual void getLayoutFinalLocalRect(Rect* outRenderRect/*, Rect* outContentRect*/) const override;
	virtual void setLayoutFinalGlobalRect(const Rect& rect) override;
	bool isLayoutVisible() const { return readCoreFlag(detail::UICoreFlags::UICoreFlags_LayoutVisible); }

private:
	void updateLocalStyleAndApplyProperties(UIStyleTable* styleTable, detail::UIStylePropertyTableInstance* parentStyleInstance);

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void raiseEventInternal(UIEventArgs* e);

	//Rect getLocalRenderRect() const;

	detail::UIManager*		m_manager;
	//UILayoutView*			m_ownerLayoutView;
	String					m_keyName;
	Size					m_minSize;
	Size					m_maxSize;
	UIElement*				m_logicalParent;
	//UIStylePropertyTable*	m_localStyle;			// 内部的に使用されるスタイル。親や VisualState から取得したスタイルをマージしたもの。
	//	m_localStyle;
	Ref<detail::UIStylePropertyTableInstance>	m_localStyle;
	Size					m_desiredSize;			// measureLayout() で決定されるこのコントロールの要求サイズ
	Rect					m_finalLocalActualRect;		// 最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	Thickness				m_renderFrameThickness;
	//Rect					m_finalLocalContentRect;
	Rect					m_finalGlobalRect;
	String					m_elementName;				// 要素名 ("UITextBlock" など) TODO: いらないかも
	Ref<UIVisualStateManager>	m_visualStateManager;
	String							m_styleSubControlOwnerName;
	String							m_styleSubControlName;

	UIElement*                m_visualParent;
	std::shared_ptr<List<Ref<UIElement>>>    m_visualChildren;

	detail::GridLayoutInfo	m_gridLayoutInfo;

	detail::BuiltinEffectData			m_builtinEffectData;
	detail::BuiltinEffectData			m_combinedBuiltinEffectData;

	UISpecialElementType			m_specialElementType;

	//Ref<Style>					m_style;
	float					m_combinedOpacity;
	//AnchorInfo				m_anchorInfo;
	detail::InvalidateFlags	m_invalidateFlags;
	detail::UICoreFlags		m_coreFlags;
	bool					m_isEnabled;
	bool					m_isMouseOver;
	bool					m_isHitTestVisible;
	bool					m_isFocusable;
	bool					m_hasFocus;

	UIEventHandler::EventType	m_onGotFocus;
	UIEventHandler::EventType	m_onLostFocus;

	friend class UILayoutView;
	friend class UIPopup;
	friend class UIContext;
	friend class UIHelper;
	friend class detail::LayoutHelper2;
};

LN_NAMESPACE_END
