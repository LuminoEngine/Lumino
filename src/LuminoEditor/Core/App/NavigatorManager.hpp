#pragma once
#include <LuminoEditor/Plugin.hpp>

class NavigatorManager;
class AssetBrowserNavigator;

class NavigationBarItem : public ln::UICollectionItem
{
public:
	void init(NavigatorManager* manager, ln::IAssetNavigatorExtension* navigator);

protected:
    virtual const ln::String& elementName() const  override { static ln::String name = u"NavigationBarItem"; return name; }
	virtual void onSelected(ln::UIEventArgs* e) override;
	virtual void onUnselected(ln::UIEventArgs* e) override;

private:
	NavigatorManager* m_navigatorManager;
	ln::IAssetNavigatorExtension* m_navigator;
};

class NavigationBar : public ln::UIItemsControl
{
public:
    static const int ItemSize = 60;

    void init(NavigatorManager* manager);
	void addItem(ln::IAssetNavigatorExtension* ext);

protected:
	//virtual UIControl* generateItem(UIElement* content) override;

private:
	NavigatorManager* m_navigatorManager;
    //Ref<ln::UIVBoxLayout2> m_layout;
};

//class Navigator
//	: public ln::Object
//{
//public:
//	Navigator();
//
//    virtual ln::UIElement* createNavigationBarItem() = 0;
//
//    // View 部分の作成。必要になったときに作成する遅延実行にしたいので、Navigator のサブクラスとはしない。
//    // インスタンスは実装側で保持が必要。
//    virtual ln::UIElement* createView() = 0;
//
//protected:
//
//private:
//};
//
class NavigatorManager
    : public ln::UIControl
{
public:
    std::function<void()> navigationViewOpen;
    std::function<void()> navigationViewClose;

	NavigatorManager();
    void init();
    void resetNavigators();
	void setCurrent(ln::IAssetNavigatorExtension* nav);

protected:

private:

    Ref<ln::UIStackLayout2> m_layout;
	Ref<ln::UISwitchLayout> m_switchLayout;
    Ref<NavigationBar> m_navigationBar;
	ln::List<ln::IAssetNavigatorExtension*> m_navigators;

    // default navi
    //Ref<AssetBrowserNavigator> m_assetBrowserNavigator;

	//float m_viewExpandingSize;
};
