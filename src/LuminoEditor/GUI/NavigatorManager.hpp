#pragma once

class AssetBrowserNavigator;

class NavigationBar : public ln::UIItemsControl
{
public:
    static const int ItemSize = 60;

    void init();
	void addItem(ln::UIElement* element);

protected:
	//virtual UIControl* generateItem(UIElement* content) override;

private:
};

class Navigator
	: public ln::Object
{
public:
	Navigator();

    virtual ln::UIElement* createNavigationBarItem() = 0;

    // View 部分の作成。必要になったときに作成する遅延実行にしたいので、Navigator のサブクラスとはしない。
    // インスタンスは実装側で保持が必要。
    virtual ln::UIElement* createView() = 0;

protected:

private:
};

class NavigatorManager
    : public ln::UIControl
{
public:
    std::function<void()> navigationViewOpen;
    std::function<void()> navigationViewClose;

	NavigatorManager();
    void init();
    void resetNavigators();

protected:

private:
    void setCurrent(Navigator* nav);

    Ref<ln::UIStackLayout> m_layout;
    Ref<NavigationBar> m_navigationBar;

    // default navi
    Ref<AssetBrowserNavigator> m_assetBrowserNavigator;
};
