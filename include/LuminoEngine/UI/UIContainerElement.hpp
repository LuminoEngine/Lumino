#pragma once
#include "UIElement.hpp"

namespace ln {

class UIContainerElement
	: public UIElement
{
public:
	UIContainerElement();
	void init();
    virtual void dispose() override;

	void addElement(UIElement* element);

	void removeAllChildren();

protected:
	/** この要素内の子ビジュアル要素の数を取得します。 */
	virtual int getVisualChildrenCount() const;

	/** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	virtual UIElement* getVisualChild(int index) const;

private:
	List<Ref<UIElement>> m_children;
};

} // namespace ln

