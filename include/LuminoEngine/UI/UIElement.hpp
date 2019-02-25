#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "UILayoutElement.hpp"

namespace ln {
class UIRenderingContext;
class UIRenderView;
class UIContext;
class UIEventArgs;
class UIStyle;
enum class BlendMode : uint8_t;
struct Color;
struct ToneF;
namespace detail { class UIManager; }

enum class UIFontWeight
{
	Normal,
	Bold,
};

enum class UIFontStyle
{
	Normal,
	Italic,
};

namespace detail
{
struct UIInheritStyleAttribute
{
	Color textColor;
	String fontFamily;
	float fontSize;
	UIFontStyle fontStyle;
};
}

class UIElement
	: public Object
    , public detail::ILayoutElement
{
public:
    /** 要素の margin 値 (外側の余白) を設定します。 */
    void setMargin(const Thickness& margin);

    /** 要素の margin 値 (外側の余白) を取得します。 */
    const Thickness& margin() const;

    /** 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
    void setPadding(const Thickness& padding);

    /** 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。 */
    const Thickness& padding() const;



    
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




    
	/** 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true) */
    LN_METHOD(Property)
    void setVisible(bool value);

	/** 可視状態を取得します。*/
    LN_METHOD(Property)
    bool isVisible() const;

    /** 合成方法を設定します。(default: BlendMode::Normal) */
    void setBlendMode(const Optional<BlendMode>& value);

    /** 合成方法を取得します。*/
    const BlendMode& blendMode() const;

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
    void setTone(const ToneF& value);

    /** 色調を取得します。 */
    const ToneF& tone() const;

    // TODO: ↑の WorldObject 的なものは、派生クラスの UIVisual 的なクラスにユーティリティとして持っていく。
    // UIElement としては RenderTransform, Style 扱いにしたい。

    UIContext* context() const { return m_context; }

    UIElement();
    virtual ~UIElement();
	void init();

public: // TODO: internal
    void setRenderPriority(int value);
    void updateFrame(float elapsedSeconds);
    const Rect& finalGlobalRect() const { return m_finalGlobalRect; }
    void raiseEvent(UIEventArgs* e);
    virtual UIElement* lookupMouseHoverElement(const Point& globalPt);

protected:
    virtual void onUpdateFrame(float elapsedSeconds);

    /**
        @brief		この要素を表示するために必要なサイズを計測します。
        @params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
        @return		この要素のレイアウトの際に必要となる最低限のサイズ。
                    この要素のサイズと、全ての子要素のサイズに基づき決定します。NaN や Inf であってはなりません。
        @details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
    */
    virtual Size measureOverride(const Size& constraint);

    /**
        @brief		Visual 子要素の配置を確定し、この要素の最終サイズを返します。
        @param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
        @return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
        @details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
                    また、finalSize には padding プロパティの余白は考慮されません。
                    余白を正しく反映するためには派生クラスで padding プロパティを参照し、子要素の位置を計算します。

                    親要素は、各子要素の Arrange を呼び出し、適切に配置する必要があります。
                    そうでない場合、子要素はレンダリングされません。(UIElement::arrangeOverride() は、子要素の配置は行いません)
    */
    virtual Size arrangeOverride(const Size& finalSize);


	/** この要素内の子ビジュアル要素の数を取得します。 */
	virtual int getVisualChildrenCount() const;

	/** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	virtual UIElement* getVisualChild(int index) const;

	virtual void onRender(UIRenderingContext* context);

    virtual void onRoutedEvent(UIEventArgs* e);

    virtual bool onHitTest(const Point& localPoint);

    // TODO: internal
    void updateLayoutHierarchical(const Size& size);
	void updateStyleHierarchical(const detail::UIInheritStyleAttribute& parentStyleAttribute);
    virtual void render(UIRenderingContext* context);

private:
    void raiseEventInternal(UIEventArgs* e);

    detail::UIManager* m_manager;
    UIContext* m_context;
    UIElement* m_visualParent;

    Ref<UIStyle> m_localStyle;
    Ref<UIStyle> m_actualStyle;
    int m_renderPriority;
    Rect m_finalGlobalRect;
    bool m_isHitTestVisible;

    friend class UIContext;
    friend class UIRenderView;
};

} // namespace ln

