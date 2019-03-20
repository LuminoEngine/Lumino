﻿#pragma once
#include "UIElement.hpp"

namespace ln {
class UILayoutPanel;

class UIContainerElement
	: public UIElement
{
public:
	UIContainerElement();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

	void addElement(UIElement* element);

	void removeAllChildren();


	/**
		子要素をレイアウトするための UILayoutPanel を設定します。
		この UILayoutPanel はこの要素のビジュアルツリーに組み込まれ、子要素の直接のビジュアル親要素となります。
		デフォルトは nullptr です。
	*/
	void setLayoutPanel(UILayoutPanel* panel);

	UILayoutPanel* layoutPanel() const;


protected:
	/** この要素内の子ビジュアル要素の数を取得します。 */
	virtual int getVisualChildrenCount() const;

	/** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	virtual UIElement* getVisualChild(int index) const;

	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	List<Ref<UIElement>> m_logicalChildren;
	Ref<UILayoutPanel> m_logicalChildrenHostPanel;

};

} // namespace ln

