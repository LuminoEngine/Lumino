
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		複数の UIElement を論理的な子要素として追加できるコントロールです。
*/
class UIContentsControl
	: public UIControl
{
	LN_OBJECT;
public:
	void addChild(UIElement* element);
	void removeChild(UIElement* element);
	void clearChildren();

	// TODO: temp
	const Ref<UIElementCollection>& getItems() const { return m_logicalChildren; }

	//void setContent(UIElement* element);

protected:
	virtual int getLogicalChildrenCount() const override;
	virtual UIElement* getLogicalChild(int index) override;

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIContentsControl();
	virtual ~UIContentsControl();
	void initialize();
	//virtual void onUpdatingLayout() override;


private:
	Ref<UIElementCollection>	m_logicalChildren;
};

/**
	@brief	複数のコントロールを組み合わせたコントロールを作成するためのクラスです。
*/
class UIUserControl
	: public UIContentsControl
{
	LN_OBJECT;
	
public:

	/** UIUserControl のインスタンスを作成します。 */
	static Ref<UIUserControl> create();

LN_CONSTRUCT_ACCESS:
	UIUserControl();
	virtual ~UIUserControl();
	void initialize();
};

LN_NAMESPACE_END
