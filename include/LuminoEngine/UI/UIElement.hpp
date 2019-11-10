#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "../Rendering/Drawing.hpp"
#include "UILayoutElement.hpp"
#include "UIEvents.hpp"

namespace ln {
class UILayoutContext;
class UIRenderingContext;
class UIFrameRenderView;
class UIContext;
class UIEventArgs;
class UIStyle;
class UIStyleClass;
class UIStyleContext;
class UIVisualStateManager;
class UIAction;
class UIContainerElement;
class UIControl;
class UIFrameWindow;
class UIAdornerLayer;
enum class BlendMode : uint8_t;
struct Color;
struct ColorTone;
namespace detail { class UIManager; }
namespace detail
{
class UIStyleInstance;
//struct UIInheritStyleAttribute
//{
//	Color textColor;
//	String fontFamily;
//	float fontSize;
//	UIFontStyle fontStyle;
//};



struct GridLayoutInfo
{
    int		layoutRow = -1; // -1 は自動割り当て
    int		layoutColumn = -1;
    int		layoutRowSpan = 1;
    int		layoutColumnSpan = 1;

    float   layoutWeight = 0;   // CSS FW (Materil-UI) や Android 参考。 0=Auto, 1~=star, finalStyle.width/height が nan でなければ direct 
};

}

//class UILayoutContext
//	: public Object
//{
//public:
//	float m_dpiScaleFactor;
//};


class UIViewModel
	: public Object
{
public:
    virtual void notify(const StringRef& propertyName = StringRef());
    virtual void notify(UINotifyPropertyChangedEventArgs* e);

	void subscribe(UIElement* observer)	// TODO: connection 返すようにした方がいいかも？
	{
		m_observers.add(observer);
	}

    void unsubscribe(UIElement* observer)
    {
        m_observers.remove(observer);
    }

private:
	List<UIElement*> m_observers;
};


class UILayoutContext
    : public Object
{
public:
    float dpiScale() const { return m_dpiScale; }
    
LN_CONSTRUCT_ACCESS:
    UILayoutContext();
    void init();

private:
    float m_dpiScale;
    friend class UIFrameWindow;
};



LN_CLASS()
class UIElement
	: public UILayoutElement
{
    LN_OBJECT;
public:
    void setWidth(float value);
    float width() const;

    void setHeight(float value);
    float height() const;


    /** 要素の margin 値 (外側の余白) を設定します。 */
    void setMargin(const Thickness& margin);

    /** 要素の margin 値 (外側の余白) を取得します。 */
    const Thickness& margin() const;

    /** 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
    void setPadding(const Thickness& padding);

    /** 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
    const Thickness& padding() const;

	/** 要素の横方向の配置方法を設定します。 */
	void setHorizontalAlignment(HAlignment value);

	/** 要素の横方向の配置方法を取得します。 */
	HAlignment horizontalAlignment() const;

	/** 要素の縦方向の配置方法を設定します。 */
	void setVerticalAlignment(VAlignment value);

	/** 要素の縦方向の配置方法を取得します。 */
	VAlignment verticalAlignment() const;


    
	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(Property)
	void setPosition(const Vector3& pos);

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setPosition(float x, float y, float z = 0.0f) { setPosition(Vector3(x, y, z)); }

	/** このオブジェクトの位置を位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& position() const;

	/** このオブジェクトの回転を設定します。 */
	LN_METHOD(Property)
	void setRotation(const Quaternion& rot);

	/** このオブジェクトの回転をオイラー角から設定します。(radian) */
	LN_METHOD()
	void setEulerAngles(float x, float y, float z) { setRotation(Quaternion::makeFromEulerAngles(Vector3(x, y, z))); }

	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& rotation() const;

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void setScale(const Vector3& scale);

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "S")
	void setScale(float xyz) { setScale(Vector3(xyz, xyz, 1)); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "XY")
	void setScale(float x, float y) { setScale(Vector3(x, y, 1)); }

	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& scale() const;
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
	LN_METHOD(Property)
	void setCenterPoint(const Vector3& value);
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
    LN_METHOD(OverloadPostfix = "XYZ")
	void setCenterPoint(float x, float y, float z = 0.0f) { setCenterPoint(Vector3(x, y, z)); }

	/** このオブジェクトのローカルの中心位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& centerPoint() const;



    /** 背景の描画モードを設定します。*/
    void setBackgroundDrawMode(BrushImageDrawMode value);

    /** 背景の描画モードを取得します。*/
    BrushImageDrawMode backgroundDrawMode() const;

	/** 背景の色を設定します。 */
	void setBackgroundColor(const Color& value);

	/** 背景の色を取得します。 */
	const Color& backgroundColor() const;

	/** 背景の描画に使用する画像を設定します。 */
	void setBackgroundImage(Texture* value);

	/** 背景の描画に使用する画像を取得します。 */
	Texture* backgroundImage() const;

	/** 背景の描画に使用するシェーダを設定します。 */
	void setBackgroundShader(Shader* value);

	/** 背景の描画に使用するシェーダを取得します。 */
	Shader* backgroundShader() const;

    /** 背景の描画に使用する画像の転送元の範囲を示す矩形を設定します。(Unit: px) */
    void setBackgroundImageRect(const Rect& value);

    /** 背景の描画に使用する画像の転送元の範囲を示す矩形を取得します。(Unit: px) */
    const Rect& backgroundImageRect() const;

    /** 背景の描画モードが BoxFrame である場合に参照される、転送元の枠の太さを設定します。(Unit: px) */
    void setBackgroundImageBorder(const Thickness& value);

    /** 背景の描画モードが BoxFrame である場合に参照される、転送元の枠の太さを取得します。(Unit: px) */
    const Thickness& backgroundImageBorder() const;



	/** 枠線の太さを設定します。 */
	void setBorderThickness(const Thickness& value);

	/** 枠線の太さを取得します。 */
	const Thickness& borderThickness() const;

	/** 枠線の色を設定します。 */
	void setBorderColor(const Color& value);

	/** 枠線の色を取得します。 */
	const Color& borderColor() const;



	/** テキストの色を設定します。*/
	void setTextColor(const Color& value);

	/** テキストの色を取得します。*/
	const Color& textColor() const;

	/** フォントファミリ名を設定します。*/
	void setFontFamily(const String& value);

	/** フォントファミリ名を取得します。*/
	const String& fontFamily() const;

	/** フォントサイズを設定します。*/
	void setFontSize(float value);

	/** フォントサイズを取得します。*/
	float fontSize() const;

	/** フォントの太さを設定します。*/
	void setFontWeight(UIFontWeight value);

	/** フォントの太さを取得します。*/
	UIFontWeight fontWeight() const;

	/** フォントのスタイルを設定します。*/
	void setFontStyle(UIFontStyle value);

	/** フォントのスタイルを取得します。*/
	UIFontStyle fontStyle() const;



    
	/** 可視状態を設定します。(default: UIVisibility::Visible) */
    //LN_METHOD(Property)
    void setVisibility(UIVisibility value);

	/** 可視状態を取得します。*/
    //LN_METHOD(Property)
    UIVisibility isVisibility() const;

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    void setBlendMode(const Optional<BlendMode>& value);

    /** 合成方法を取得します。*/
    BlendMode blendMode() const;

    /** 不透明度を設定します。(default: 1.0) */
    void setOpacity(float value);

    /** 不透明度を取得します。 */
    float opacity() const;

    /** カラースケールを設定します。(default: Color(1, 1, 1, 1)) */
    void setColorScale(const Color& value);

    /** カラースケールを取得します。 */
    const Color& colorScale() const;

    /** ブレンドカラーを設定します。(default: Color(0, 0, 0, 0)) */
    void setBlendColor(const Color& value);

    /** ブレンドカラーを取得します。 */
    const Color& blendColor() const;

    /** 色調を設定します。(default: Tone(0, 0, 0, 0)) */
    void setTone(const ColorTone& value);

    /** 色調を取得します。 */
    const ColorTone& tone() const;

    // TODO: ↑の WorldObject 的なものは、派生クラスの UIVisual 的なクラスにユーティリティとして持っていく。
    // UIElement としては RenderTransform, Style 扱いにしたい。

    UIContext* getContext() const;

    void addClass(const StringRef& className);
	void setViewModel(UIViewModel* value);
    virtual void setContent(UIElement* content);
    virtual void setContent(const String& content);
	/** Add element to container. */
	LN_METHOD()
    virtual void addChild(UIElement* child);
    virtual void addChild(const String& child);

    void addAction(UIAction* action);

    void invalidateStyle() { invalidate(detail::UIElementDirtyFlags::Style, true); }
	void invalidateLayout() { invalidate(detail::UIElementDirtyFlags::Layout, true); }
	void invalidateVisual() { invalidate(detail::UIElementDirtyFlags::Render, true); }

	/** 入力フォーカスを得ることができるかどうかを設定します。(default: false) */
	void setFocusable(bool value) { m_focusable = true; }

	/** ウィンドウを前面にしてアクティブ化することを試みます。 */
	void activate();

    UIElement();
    virtual ~UIElement();
	void init();

public: // TODO: internal
    void setRenderPriority(int value);
    void updateFrame(float elapsedSeconds);
    void raiseEvent(UIEventArgs* e);
    void postEvent(UIEventArgs* e);
    virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition);
	const Ref<detail::UIStyleInstance>& finalStyle() const { return m_finalStyle; }
	UIElement* getFrameWindow();
    UIFrameRenderView* getRenderView();

public:	// TODO: internal protected
    void focus();
	void retainCapture();
	void releaseCapture();
	void addVisualChild(UIElement* element);
	void removeVisualChild(UIElement* element);

    virtual const String& elementName() const { return String::Empty; }
	
    //virtual void onSetup(); // インスタンス構築直後。VisualTree や Style, Layout は構築されているとは限らない。初回 update 前に this のプロパティを設定するために使う。
	virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel);
    virtual void onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e);
    virtual void onLoaded();    // インスタンス作成後、UIツリーに追加されていない場合は呼ばれない
    virtual void onUpdateFrame(float elapsedSeconds);

    // この中で addVisualChild することができる。関数を抜けた後に、それらのスタイルは更新される。
	virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle);

    /**
        @brief		この要素を表示するために必要なサイズを計測します。
        @params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
        @return		この要素のレイアウトの際に必要となる最低限のサイズ。
                    この要素のサイズと、全ての子要素のサイズに基づき決定します。NaN や Inf であってはなりません。
        @details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。

		このメソッドはフレームワークから呼び出されます。直接呼び出しても正しい結果は得られません。
		このメソッドの実装から子要素の measure を行う場合は measureLayout() を呼び出します。

        複数の子要素を並べてレイアウトしたい場合、サイズの合計を返します。
        例えば 2 つの UITextBlock を縦に並べる場合は、2つの measureLayout() の結果について、
        - width は 大きい方を返す
        - height は 2 つの合計を返す

		constraint に padding と border は含まれていません。
		通常、padding と border を加算したサイズを返すように実装します。
		border については、inset または outset の場合分けが必要です。

        Note: レイアウトのコツとしては、constraint から減算するのではなく、子要素を加算してくこと。constraint は NaN や Inf が含まれることがある。
    */
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;

    /**
        @brief		Visual 子要素の配置を確定し、この要素の最終サイズを返します。
        @param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
        @return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
        @details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
                    また、finalSize には padding および border プロパティの余白は考慮されません。
                    余白を正しく反映するためには派生クラスで padding および border プロパティを参照し、子要素の位置を計算します。

                    親要素は、各子要素の Arrange を呼び出し、適切に配置する必要があります。
                    そうでない場合、子要素はレンダリングされません。(UIElement::arrangeOverride() は、子要素の配置は行いません)
    */
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;


	/** この要素内の子ビジュアル要素の数を取得します。 */
	//virtual int getVisualChildrenCount() const;
	// マウスのヒットテスト (Hierarchical)
	// スタイル更新 (Hierarchical)
	// フレーム更新 (Hierarchical)
	// 描画 (Hierarchical)
	// ※レイアウトは対象外

	/**
	 * 子ビジュアル要素を取得します。
	 *
	 * 装側は、奥にある要素が先、手前にある要素が後になるようにZオーダーやアクティブ状態を考慮する必要があります。
	 */
	//virtual UIElement* getVisualChild(int index) const;

	virtual void onRender(UIRenderingContext* context);

    virtual void onRoutedEvent(UIEventArgs* e);

    virtual bool onHitTest(const Point& frameClientPosition);

	virtual void updateFinalRects(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);

    bool isMouseHover() const;

    // TODO: internal
	void updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
    void updateFinalLayoutHierarchical(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);
    virtual void render(UIRenderingContext* context);

	Flags<detail::ObjectManagementFlags>& objectManagementFlags() { return m_objectManagementFlags; }
	Flags<detail::UISpecialElementFlags>& specialElementFlags() { return m_specialElementFlags; }
    void setLogicalParent(UIControl* parent) { m_logicalParent = parent; }
    UIControl* logicalParent() const { return m_logicalParent; }
    void removeFromLogicalParent();


    UIVisualStateManager* getVisualStateManager();

public: // TODO: internal
    void raiseEventInternal(UIEventArgs* e);
    virtual void invalidate(detail::UIElementDirtyFlags flags, bool toAncestor);
    detail::GridLayoutInfo* getGridLayoutInfo();
    bool isRenderVisible() const;
    bool isHitTestVisibleCore() const { return m_isHitTestVisible && isRenderVisible(); }

	void activateInternal();
	void deactivateInternal();
	void moveVisualChildToForeground(UIElement* child);

    void handleDetachFromUITree();

    detail::UIManager* m_manager;
	Flags<detail::ObjectManagementFlags> m_objectManagementFlags;
	Flags<detail::UISpecialElementFlags> m_specialElementFlags;
    
    // TODO: ↓ UIRenderView にまとめてしまっていいかも
    // TODO: ↓ UILayoutContext や UIStyleContxt 経由でもらう
    UIContext* m_context;       // ルート要素 (ほとんどの場合は UIFrameWindow) が値を持つ。それ以外は基本的に null. もしウィンドウ内で別のコンテキストに属したい場合はセットする。
    UIFrameRenderView* m_renderView = nullptr; // ルート要素が値を持つ。
    
    UIElement* m_visualParent;
    UIControl* m_logicalParent;    // TODO: Layout も親となりえる。
	Ref<List<Ref<UIElement>>> m_visualChildren;
	Ref<List<UIElement*>> m_orderdVisualChildren;
    Ref<List<String>> m_classList;
    Ref<List<Ref<UIAction>>> m_actions;
	Ref<UIViewModel> m_viewModel;
    std::unique_ptr<detail::GridLayoutInfo> m_gridLayoutInfo;

    Ref<UIVisualStateManager> m_visualStateManager;
    Ref<UIStyleClass> m_localStyle;
	Ref<UIStyle> m_combinedStyle;
	Ref<detail::UIStyleInstance> m_finalStyle;
	Matrix m_combinedFinalRenderTransform;
	UIVisibility m_internalVisibility;
    int m_renderPriority;
    bool m_isHitTestVisible;	// TODO: flags
	bool m_focusable;			// TODO: flags

    friend class UIContext;
    friend class UIFrameRenderView;
    friend class UIFrameWindow;
    friend class UIViewModel;

	int getVisualChildrenCount() const { return (m_visualChildren) ? m_visualChildren->size() : 0; }
	UIElement* getVisualChild(int index) const { return (m_visualChildren) ? m_visualChildren->at(index) : nullptr; }
private:

    Flags<detail::UIElementDirtyFlags> m_dirtyFlags;
};

} // namespace ln

