﻿#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "UIExtension.hpp"

class NavigatorManager;
class AssetBrowserNavigator;

class Navigator
    : public ln::Object
{
public:
    Navigator() {}
    virtual ln::UIElement* getNavigationMenuItem() = 0;
    virtual ln::UIElement* getNavigationPane() = 0;
    // View 部分の作成。必要になったときに作成する遅延実行にしたいので、Navigator のサブクラスとはしない。
    // インスタンスは実装側で保持が必要。

    // TOOD: name。エディタ状態復元などで、前回どれを開いていたかを識別したい

protected:

private:
};

class NavigationBarItem : public ln::UICollectionItem
{
public:
	void init(NavigatorManager* manager, Navigator* navigator);
    void addIcon(const ln::StringRef& iconName);

protected:
    virtual const ln::String& elementName() const  override { static ln::String name = u"NavigationBarItem"; return name; }
	virtual void onSelected(ln::UIEventArgs* e) override;
	virtual void onUnselected(ln::UIEventArgs* e) override;

private:
	NavigatorManager* m_navigatorManager;
    Navigator* m_navigator;
};

class NavigationBar : public ln::UIItemsControl
{
public:
    static const int ItemSize = 60;

    void init(NavigatorManager* manager);
    void addNavigator(Navigator* navigator);
	//void addItem(ln::IAssetNavigatorExtension* ext);

protected:
	//virtual UIControl* generateItem(UIElement* content) override;

private:
	NavigatorManager* m_navigatorManager;
    //Ref<ln::UIVBoxLayout2> m_layout;
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
    void addNavigator(Navigator* navigator);
	void setCurrent(Navigator* navigator);

    const Ref<NavigationBar>& navigationBar() const { return m_navigationBar; }

protected:

private:

    Ref<ln::UIBoxLayout3> m_layout;
	Ref<ln::UISwitchLayout> m_switchLayout;
    Ref<NavigationBar> m_navigationBar;
	ln::List<Navigator*> m_navigators;

    // default navi
    //Ref<AssetBrowserNavigator> m_assetBrowserNavigator;

	//float m_viewExpandingSize;
};
