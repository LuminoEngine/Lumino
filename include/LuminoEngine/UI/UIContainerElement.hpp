#pragma once
#include "UIElement.hpp"

namespace ln {
class UILayoutPanel;
class UILayoutPanel2;

class UIContainerElement
	: public UIElement
{
public:
	UIContainerElement();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

	void addElement(UIElement* element);
    void removeElement(UIElement* element);

	void removeAllChildren();


	/**
		子要素をレイアウトするための UILayoutPanel を設定します。

		デフォルトは nullptr です。
	*/
	void setLayoutPanel(UILayoutPanel2* panel);

	UILayoutPanel2* layoutPanel() const;


protected:
	/** この要素内の子ビジュアル要素の数を取得します。 */
	virtual int getVisualChildrenCount() const;

	/** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	virtual UIElement* getVisualChild(int index) const;

	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	void setLogicalChildrenHost(UILayoutPanel* panel);

	List<Ref<UIElement>> m_logicalChildren;
	//Ref<UILayoutPanel> m_logicalChildrenHost;
    Ref<UILayoutPanel2> m_layout;
	Size m_layoutDesiredSize;	// Layout is state-less
};


class UIFrame
    : public UIContainerElement
{
public:
    UIFrame();
    void init();

private:
};

} // namespace ln

