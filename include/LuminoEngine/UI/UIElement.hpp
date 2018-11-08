#pragma once
#include "UILayoutElement.hpp"

namespace ln {
class UIRenderingContext;
class UIRenderView;
namespace detail { class UIManager; }

class UIElement
	: public Object
    , public detail::ILayoutElement
{
public:
    UIElement();
	void initialize();

protected:


    /**
        @brief		この要素を表示するために必要なサイズを計測します。
        @params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
        @return		この要素のレイアウトの際に必要となる最低限のサイズ。
                    この要素のサイズと、全ての子要素のサイズに基づき決定します。NaN や Inf であってはなりません。
        @details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
    */
    //virtual Size measureOverride(const Size& constraint) override;

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
    //virtual Size arrangeOverride(const Size& finalSize) override;


	/** この要素内の子ビジュアル要素の数を取得します。 */
	virtual int getVisualChildrenCount() const;

	/** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	virtual UIElement* getVisualChild(int index) const;

	virtual void onRender(UIRenderingContext* context);



    // TODO: internal
    void updateLayout(const Size& size);
    virtual void render(UIRenderingContext* context);

private:
    detail::UIManager* m_manager;

    friend class UIRenderView;
};

} // namespace ln

