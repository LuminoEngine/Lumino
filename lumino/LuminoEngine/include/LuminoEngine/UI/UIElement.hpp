#pragma once
#include <LuminoEngine/Base/Builder.hpp>
#include <LuminoGraphics/Rendering/Drawing.hpp>
#include "UILayoutElement.hpp"
#include "UIEvents.hpp"

namespace ln {
class Variant;
class UILayoutContext;
class UIRenderingContext;
class UIFrameRenderView;
//class UIContext;
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


enum class UIHitTestMode
{
	Visible,
	InvisibleControl,	// 子要素も含めて HitTest を無効にする
	InvisiblePanel,		// 自身の HitTest を行わないが、子要素の HitTest には影響しない。 レイアウトなど描画やインタラクションを目的としない Element で使用する。
};


struct GridLayoutInfo
{
    int		layoutRow = -1; // -1 は自動割り当て
    int		layoutColumn = -1;
    int		layoutRowSpan = 1;
    int		layoutColumnSpan = 1;

	// 自動割り当て解決済みのインデックス
	int actualLayoutRow = 0;
	int actualLayoutColumn = 0;

    float   layoutWeight = -1;   // CSS FW (Materil-UI) や Android 参考。 0=Auto, 1~=star, finalStyle.width/height が nan でなければ direct 
									// UIElement デフォルトではインスタンスがない。
									// BoxLayout では、weight省略時はデフォルトで Ratio にしたい。
									// -1 は Ratio というより、LayoutPanel のデフォルトに任せますよ、という意思表示。
};

} // namespace detail


// [2021/3/25] 本当に ViewModel なんてものが必要なの？
// ----------
// RetaindMode な GUI フレームワークなので、imgui のように毎フレーム UIElement の作成・削除が行われるのは避けたい。
// ただリアルタイムアプリという特徴から、要素の再描画は毎フレームの onRender でやってしまって構わない、というか、やらざるを得ない。
// そのためプロパティ単位で変更通知を出すことのメリットはかなり薄い。
// 一方 TreeView など Item(UIElement) を再構築するタイミングは通知してほしい。
class UIViewModel
	: public Object
{
public:
    virtual void notify(const StringView& propertyName = StringView());
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




/** UIElement */
LN_CLASS()
class UIElement
	: public UILayoutElement
{
    LN_OBJECT;
	LN_BUILDER;

public:
	//class Builder : public BuilderBase
	//{
	//public:
	//	Builder();
	//	Builder& height(float value);
	//	Builder& backgroundColor(const Color& value);
	//	Ref<UIElement> build();

	//protected:
	//	class Details : public BuilderDetailsBase
	//	{
	//	public:
	//		Optional_deprecated<float> height;
	//		Optional_deprecated<Color> backgroundColor;
	//		virtual Ref<Object> build() override;
	//	};

	//	Builder(Details* d);
	//};


    void setName(const String& value) { m_name = value; }
    const String& name() const { return m_name; }

	/** 要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります) */
	LN_METHOD()
	void setSize(const Size& size) { setWidth(size.width); setHeight(size.height); }

	/** 要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります) */
	LN_METHOD(OverloadPostfix = "WH")
	void setSize(float width, float height) { setSize(Size(width, height)); }

	/** setWidth */
	LN_METHOD(Property)
    void setWidth(float value);

	/** width */
	LN_METHOD(Property)
    float width() const;

	/** setHeight */
	LN_METHOD(Property)
    void setHeight(float value);

	/** height */
	LN_METHOD(Property)
    float height() const;


    /** 要素の margin 値 (外側の余白) を設定します。 */
	LN_METHOD(Property)
    void setMargin(const Thickness& margin);

    /** 要素の margin 値 (外側の余白) を取得します。 */
	LN_METHOD(Property)
    const Thickness& margin() const;

    /** 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
	LN_METHOD(Property)
    void setPadding(const Thickness& padding);

    /** 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
	LN_METHOD(Property)
    const Thickness& padding() const;

	/** 指定された配置方法に基づいて、スタイルの Margin を設定します。 */
	LN_METHOD()
	void setAlignments(UIAlignment value);
    
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


	
	/** 要素の有効状態を設定します。 */
	LN_METHOD(Property)
	void setEnabled(bool value);
    
	/** 要素の有効状態を取得します。 */
	LN_METHOD(Property)
	bool isEnabled() const;

	/** 任意のユーザーデータを設定します。 */
	LN_METHOD(Property)
	void setData(Variant* value);

	/** 任意のユーザーデータを取得します。 */
	LN_METHOD(Property)
	Variant* data() const;

	template<typename T> T dataAs() const { return m_data->get<T>(); }


    /** 背景の描画モードを設定します。*/
    void setBackgroundDrawMode(Sprite9DrawMode value);

    /** 背景の描画モードを取得します。*/
	Sprite9DrawMode backgroundDrawMode() const;

	/** 背景の色を設定します。 */
	LN_METHOD(Property)
	void setBackgroundColor(const Color& value);

	/** 背景の色を取得します。 */
	LN_METHOD(Property)
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
	LN_METHOD(Property)
	void setBorderThickness(const Thickness& value);

	/** 枠線の太さを取得します。 */
	LN_METHOD(Property)
	const Thickness& borderThickness() const;

	/** 枠線の色を設定します。 */
	LN_METHOD(Property)
	void setBorderColor(const Color& value);

	/** 枠線の色を取得します。 */
	LN_METHOD(Property)
	const Color& borderColor() const;
	
	/** 四角形の角の半径を設定します。 */
	LN_METHOD(Property)
	void setCornerRadius(const CornerRadius& value);

	/** 四角形の角の半径を取得します。 */
	LN_METHOD(Property)
	const CornerRadius& cornerRadius() const;


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
    LN_METHOD(Property)
    void setVisibility(UIVisibility value);

	/** 可視状態を取得します。*/
    LN_METHOD(Property)
    UIVisibility visibility() const;

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    void setBlendMode(const Optional_deprecated<BlendMode>& value);

    /** 合成方法を取得します。*/
    BlendMode blendMode() const;

    /** 不透明度を設定します。(default: 1.0) */
	LN_METHOD(Property)
    void setOpacity(float value);	// TODO: 子要素へ継承してほしい

    /** 不透明度を取得します。 */
	LN_METHOD(Property)
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

	UIStyle* style() const;

    //UIContext* getContext() const;

    void addClass(const StringView& className);
	void setViewModel(UIViewModel* value);
    virtual void setContent(UIElement* content);
    virtual void setContent(const String& content);
	/** Add element to container. 論理的な子要素として追加する。 */
	LN_METHOD()
    void addChild(UIElement* child);
    void addChild(const String& child);
	void add(UIElement* child) { addChild(child); }


    void invalidateStyle() { invalidate(detail::UIElementDirtyFlags::Style, true); }
	void invalidateLayout() { invalidate(detail::UIElementDirtyFlags::Layout, true); }
	void invalidateVisual() { invalidate(detail::UIElementDirtyFlags::Render, true); }

	/** 入力フォーカスを得ることができるかどうかを設定します。(default: false) */
	LN_METHOD(Property)
	void setFocusable(bool value) { m_focusable = value; }

	/** 入力フォーカスを得ることができるかどうかを取得します。 */
	LN_METHOD(Property)
	bool focusable() const { return isEnabled() && m_focusable; }

    void setClipToBounds(bool value) { m_clipToBounds = value; }
    bool clipToBounds() const { return m_clipToBounds; }

	/** ウィンドウを前面にしてアクティブ化することを試みます。 */
	//void activate();

	
	/** この UIElement を指定した UIElement へ子要素として追加します。省略した場合はデフォルトのルート UIElement へ追加します。 */
	LN_METHOD()
	void addInto(UIElement* parent = nullptr);

    UIElement();
    virtual ~UIElement();
	bool init();

public: // TODO: internal
    void setRenderPriority(int value);
    void updateFrame(float elapsedSeconds);
    void raiseEvent(UIEventArgs* e, UIEventRoutingStrategy strategy = UIEventRoutingStrategy::Bubble);
    void postEvent(UIEventArgs* e);
    virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition);
	const Ref<detail::UIStyleInstance>& finalStyle() const { return m_finalStyle; }
	UIElement* getFrameWindow();
    UIFrameRenderView* getRenderView();
	void setPartParent(UIElement* e) { m_partParent = e; }

public:	// TODO: internal protected
    void focus();
	void retainCapture();
	void releaseCapture();
	void grabCursor();
	void releaseCursor();

	// element を、この要素の Visual 子要素として追加する。
	// Visual 子要素は次のワークフローに組み込まれる。
	// - マウスのヒットテスト (Hierarchical)
	// - スタイル更新 (Hierarchical)
	// - フレーム更新 (Hierarchical)
	// - 描画 (Hierarchical)
	// 注意点として、レイアウトは含まれない。
	// このため通常は、 measureOverride, arrangeOverride を実装してレイアウトする必要がある。
	void addVisualChild(UIElement* element);
	void removeVisualChild(UIElement* element);
	void removeAllVisualChild();

    //virtual const String& elementName() const { return String::Empty; }
	virtual const String& elementName() const { static String name = _TT("UIElement"); return name; }

    virtual void onDispose(bool explicitDisposing) override;

    //virtual void onSetup(); // インスタンス構築直後。VisualTree や Style, Layout は構築されているとは限らない。初回 update 前に this のプロパティを設定するために使う。
	virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel);
    virtual void onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e);
    virtual void onLoaded();    // インスタンス作成後、UIツリーに追加されていない場合は呼ばれない
	virtual void onUpdateFrame(float elapsedSeconds);	// TODO: 他モジュールと合わせ onUpdate のほうがいいかも。または、引数 void お onUpdate にしてみるとか。

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
        例えば 2 つの UIText を縦に並べる場合は、2つの measureLayout() の結果について、
        - width は 大きい方を返す
        - height は 2 つの合計を返す

		constraint に padding と border は含まれていません。
		通常、padding と border を加算したサイズを返すように実装します。
		border については、inset または outset の場合分けが必要です。

        @note
			- レイアウトのコツとしては、constraint から減算するのではなく、子要素を加算してくこと。constraint は Inf が含まれることがある。
			- 実装では padding と border のサイズを考慮する必要があるが、これは VisualChild をレイアウトしてほしいためこうする必要がある。(LogicalChild ではなく)
    */
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;

    /**
        @brief		Visual 子要素の配置を確定し、この要素の最終サイズを返します。
        @param[in]	finalArea	: 親要素がこの要素に対して割り当てた領域のサイズ。
        @return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
        @details	派生クラスは finalArea よりも大きいサイズを返すと、描画時に見切れが発生します。
                    
					finalArea には padding および border プロパティの余白は考慮されません。
                    余白を正しく反映するためには派生クラスで padding および border プロパティを参照し、子要素の位置を計算します。

					finalArea.x と finalArea.y は、MarginedArea を基準としたオフセットで、通常は (0, 0) です。
					派生クラスで、ベースクラスのレイアウト処理をある特定のローカルエリア内で行いたい場合に指定されます。

                    親要素は、各子要素の Arrange を呼び出し、適切に配置する必要があります。
                    そうでない場合、子要素はレンダリングされません。(UIElement::arrangeOverride() は、子要素の配置は行いません)
    */
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

	virtual void arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect) override;

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

	/**
	 * Note: Border は含まない描画領域をアクティブとしている。HTML5 とは異なり、WPF と同じ仕様。0,0はBorderと重なる。
	 * - マウス座標系は、UIElement の境界座標系 (Border の外周) としたい。MouseEvent で、座標に負値が入ってくるのはちょっと良くない。HitTest するときに細工とか必要。
	 *	- 実際のところ、onRender を実装することよりも MouseEvent を利用することの方が圧倒的に多い。（見た目のカスタマイズはほとんど style で足りる）
	 * - 描画座標系は、UIElement の境界にするかコンテンツ矩形にするかは悩みどころだけど、実際にやってみたところ境界座標系に合わせる方が都合が良いことが多かった。
	 *	- clientRect() や contentRect() と同じ座標系なので、「Padding を考慮したところに描画したい」といった指定が比較的楽。
	 *	  - コンテンツ矩形とかにしてしまうと、clientRect() や contentRect() から領域を取り出すにしても一段計算を挟む必要がある。
	 *  - (0,0) が Border の上に乗ってしまうことは最初「ん？」ってなるかもしれないけど、結果として実装がやりやすくなるので境界領域を使ってみる。
	 */
	virtual void onRender(UIRenderingContext* context);

    virtual void onRoutedEvent(UIEventArgs* e);

    virtual bool onHitTest(const Point& frameClientPosition);

    virtual void onAddChild(UIElement* child);

	virtual void onEnabledChanged();

	//virtual void updateFinalRects(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);

    bool isMouseHover() const;

    // TODO: internal
	void updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
    void updateFinalLayoutHierarchical(UILayoutContext* layoutContext, const Matrix& parentCombinedRenderTransform);
    virtual void render(UIRenderingContext* context, const Matrix& parentTransform);
	void renderClient(UIRenderingContext* context, const Matrix& combinedTransform);

	Flags<detail::ObjectManagementFlags>& objectManagementFlags() { return m_objectManagementFlags; }
	Flags<detail::UISpecialElementFlags>& specialElementFlags() { return m_specialElementFlags; }
    void setLogicalParent(UIControl* parent) { m_logicalParent = parent; }
    UIControl* logicalParent() const { return m_logicalParent; }
	template<class T> T* logicalParentAs() const { return static_cast<T*>(m_logicalParent); }
    void removeFromLogicalParent();
	void attemptAddToPrimaryElement();

	Point frameClientPositionToLocalPosition(const Point& frameClientPosition) const;

    UIVisualStateManager* getVisualStateManager();

public: // TODO: internal
    void raiseEventInternal(UIEventArgs* e, UIEventRoutingStrategy strategy);
    virtual void invalidate(detail::UIElementDirtyFlags flags, bool toAncestor);
    detail::GridLayoutInfo* getGridLayoutInfo();
    bool isRenderVisible() const;
    //bool isHitTestVisibleCore() const { return m_hitte && isRenderVisible(); }

	void moveVisualChildToForeground(UIElement* child);

    void handleDetachFromUITree();
	UIElement* findFocusedVisualChildLeaf();

    detail::UIManager* m_manager;
    String m_name;
	Flags<detail::ObjectManagementFlags> m_objectManagementFlags;
	Flags<detail::UISpecialElementFlags> m_specialElementFlags;
    
    // TODO: ↓ UIRenderView にまとめてしまっていいかも
    // TODO: ↓ UILayoutContext や UIStyleContxt 経由でもらう
    //UIContext* m_context;       // ルート要素 (ほとんどの場合は UIFrameWindow) が値を持つ。それ以外は基本的に null. もしウィンドウ内で別のコンテキストに属したい場合はセットする。
    UIFrameRenderView* m_renderView = nullptr; // ルート要素が値を持つ。
    
    UIElement* m_visualParent;		// 必ず存在する。
    UIControl* m_logicalParent;    // 必ず存在するとは限らない。 TODO: Layout も親となりえる。
	Ref<List<Ref<UIElement>>> m_visualChildren;
	Ref<List<UIElement*>> m_orderdVisualChildren;
    Ref<List<String>> m_classList;
	Ref<UIViewModel> m_viewModel;
    std::unique_ptr<detail::GridLayoutInfo> m_gridLayoutInfo;

	// CheckBox や TreeItem、ScrollBar など、複数のサブパーツ要素からひとつの要素が構成される際の親要素。
	// m_partParent を持つ要素は、親の VisualState を引き継ぐ。
	// これは内部実装の量やメモリ効率を狙ったもので、この仕組みが無いと、すべてのサブパーツごとに VisualState の定義が必要になってしまう。
	// 必ずしも VisualTree 上での直接の親子関係とはかぎらない。(多くの場合、間に LayoutPanel が挟まる)
	UIElement* m_partParent = nullptr;

    Ref<UIVisualStateManager> m_visualStateManager;
    Ref<UIStyleClass> m_localStyle;
	Ref<UIStyle> m_combinedStyle;
	Ref<detail::UIStyleInstance> m_finalStyle;
	UIVisibility m_internalVisibility;
    int m_renderPriority;
	detail::UIHitTestMode m_hitTestMode;
	UIElement* m_focusedVisualChild = nullptr;
	bool m_focusable;			// TODO: flags
    bool m_clipToBounds;			// TODO: flags

	detail::BuiltinEffectData m_actuialBuiltinEffectData;

    //friend class UIContext;
    friend class UIFrameRenderView;
    friend class UIFrameWindow;
    friend class UIViewModel;

	int getVisualChildrenCount() const { return (m_visualChildren) ? m_visualChildren->size() : 0; }
	UIElement* getVisualChild(int index) const { return (m_visualChildren) ? m_visualChildren->at(index) : nullptr; }

private:
	void updateEnabledPropertyOnChildren();

    Flags<detail::UIElementDirtyFlags> m_dirtyFlags;
	Ref<Variant> m_data;
};

//==============================================================================
// UIElement::Builder

struct UIElement::BuilderDetails : public AbstractBuilderDetails
{
	LN_BUILDER_DETAILS(UIElement);

	Optional_deprecated<float> width;
	Optional_deprecated<float> height;
	Optional_deprecated<Color> backgroundColor;
	//Optional_deprecated<UIHAlignment> hAlignment;
	//Optional_deprecated<UIVAlignment> vAlignment;

	void apply(UIElement* p) const;
};

template<class T, class B, class D>
struct UIElement::BuilderCore : public AbstractBuilder<T, B, D>
{
	LN_BUILDER_CORE(AbstractBuilder);

	/** width property */
	B& width(float value) { d()->width = value; return self(); }

	/** height property */
	B& height(float value) { d()->height = value; return self(); }

	/** height property */
	B& backgroundColor(const Color& value) { d()->backgroundColor = value; return self(); }

	/** alignment property */
	//B& alignment(UIHAlignment h, UIVAlignment v) { d()->hAlignment = h; d()->vAlignment = v; return self(); }

	Ref<T> buildInto(UIElement* parent = nullptr) { auto p = AbstractBuilder<T, B, D>::build(); p->addInto(parent); return p; }
};

LN_BUILDER_IMPLEMENT(UIElement);

} // namespace ln

