#include "Internal.hpp"
#include <LuminoFramework/Engine/Application.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoFramework/UI/UIColors.hpp>
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/UICommand.hpp>
#include <LuminoFramework/UI/UIContainerElement.hpp>
#include <LuminoFramework/UI/UIEvents.hpp>
#include <LuminoFramework/UI/UIContext.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
#include <LuminoFramework/UI/UIFrameWindow.hpp>
#include <LuminoFramework/UI/UIActiveTimer.hpp>
#include <LuminoFramework/UI/UIRenderView.hpp>
#include "UIStyleInstance.hpp"
#include "UIEventArgsPool.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

/*
    [2020/4/10] Focus
    ----------
         +
        /|\
       / | \
      +  +  +
     /|\   /|\
    + + + + + +

         *
        /|\
       / | \
      +  +  *
     /|\   /|\
    + + + + + *
              ^
              InputFocus を与える

         *
        /|\
       / | \
      *  +  x
     /|\   /|\
    + * + + + x  <ルートからは独立するが、内部的にはフラグを持っている
      ^
      次はこっちに InputFocus を与える

         *
        /|\
       / | \
      x  +  *  < 1.ここに focus() する
     /|\   /|\
    + x + + + *  < 2. 子へフラグを探しに行って、ここから通常の focus() する

         *
        /|\
       / | \
      x  +  *  < フラグは子が持つのではなく、親が持つ。この Level では、同時に２つの子がフラグを持つことになってしまうため。
     /|\   /|\
    + x + + + *


*/

//==============================================================================
// UIManager

const float UIManager::MouseButtonClickTimeout = 0.3f;

UIManager::UIManager()
    : m_graphicsManager(nullptr)
    , m_application(nullptr)
    , m_primaryElement(nullptr)
    , m_mouseHoverElement(nullptr)
    , m_capturedElement(nullptr)
    , m_forcusedElement(nullptr) {
}

void UIManager::init(const Settings& settings) {
    LN_LOG_DEBUG("UIManager Initialization started.");

    m_graphicsManager = settings.graphicsManager;
    m_defaultThemeName = settings.defaultThemeName;
    m_eventArgsPool = makeRef<EventArgsPool>();
    // m_mainContext = makeObject_deprecated<UIContext>();

    // m_defaultLayout = makeObject_deprecated<UIFrameLayout>();

    m_commonInputCommands.left = makeObject_deprecated<UICommand>(_TT("left"));
    m_commonInputCommands.right = makeObject_deprecated<UICommand>(_TT("right"));
    m_commonInputCommands.up = makeObject_deprecated<UICommand>(_TT("up"));
    m_commonInputCommands.down = makeObject_deprecated<UICommand>(_TT("down"));
    m_commonInputCommands.submit = makeObject_deprecated<UICommand>(_TT("submit"));
    m_commonInputCommands.cancel = makeObject_deprecated<UICommand>(_TT("cancel"));
    m_commonInputCommands.menu = makeObject_deprecated<UICommand>(_TT("men_TT("));
    m_commonInputCommands.shift = makeObject_deprecated<UICommand>(_TT("shift"));
    m_commonInputCommands.pageUp = makeObject_deprecated<UICommand>(_TT("pageUp"));
    m_commonInputCommands.pageDown = makeObject_deprecated<UICommand>(_TT("pageDown"));
    m_inputCommands.add(m_commonInputCommands.left);
    m_inputCommands.add(m_commonInputCommands.right);
    m_inputCommands.add(m_commonInputCommands.up);
    m_inputCommands.add(m_commonInputCommands.down);
    m_inputCommands.add(m_commonInputCommands.submit);
    m_inputCommands.add(m_commonInputCommands.cancel);
    m_inputCommands.add(m_commonInputCommands.menu);
    m_inputCommands.add(m_commonInputCommands.shift);
    m_inputCommands.add(m_commonInputCommands.pageUp);
    m_inputCommands.add(m_commonInputCommands.pageDown);

    // TODO: Input とリンクするか、どちらかで一元管理する
    // → もしかしたら別管理のほうがいいかもしれない。特に、マウスクリックで項目選択するとき、
    // 何もないところをクリックして submit 扱いされるのはあんまりよくない。
    m_commonInputCommands.submit->addInputGesture(makeObject_deprecated<KeyGesture>(Keys::Z, ModifierKeys::None));
    m_commonInputCommands.cancel->addInputGesture(makeObject_deprecated<KeyGesture>(Keys::X, ModifierKeys::None));

    {
        m_finalDefaultStyle = makeRef<detail::UIStyleInstance>();
        m_styleContext = makeObject_deprecated<UIStyleContext>();
        // m_defaultStyle = makeObject_deprecated<UIStyle>();
        // m_defaultStyle->setupDefault();
        // m_finalDefaultStyle->setupDefault();

        if (String::compare(detail::EngineDomain::uiManager()->defaultThemeName(), _TT("Chocotelier"), CaseSensitivity::CaseInsensitive) == 0) {
            auto theme = makeObject_deprecated<UITheme>();
            theme->buildLumitelier();
            m_styleContext->addStyleSheet(theme->styleSheet());
            m_styleContext->mainTheme = theme;
        }
        else {
            setupDefaultStyle();
        }

        m_finalDefaultStyle->backgroundMaterial = makeObject_deprecated<Material>();
        detail::UIStyleInstance::updateStyleDataHelper(m_styleContext, nullptr, m_styleContext->mainTheme->defaultStyle(), m_finalDefaultStyle);
    }

    LN_LOG_DEBUG("UIManager Initialization finished.");
}

void UIManager::dispose() {
    LN_LOG_DEBUG("UIManager dispose started.");

    if (m_application) {
        m_application->finalizeInternal();
        m_application = nullptr;
    }

    m_eventArgsPool = nullptr;
    // m_mainContext = nullptr;
    m_styleContext = nullptr;
    m_finalDefaultStyle = nullptr;

    LN_LOG_DEBUG("UIManager dispose finished.");
}

void UIManager::resetApp(Application* app) {
    if (m_application == app) return;

    if (m_application) {
        m_application->finalizeInternal();
        m_application = nullptr;
    }

    m_application = app;
}

void UIManager::onElementDisposing(UIElement* element) {
    // if (m_forcusedElement == element) {
    //     m_forcusedElement = nullptr;
    // }
    // if (m_capturedElement == element) {
    //     m_capturedElement = nullptr;
    // }
    // if (m_mouseHoverElement == element) {
    //     m_mouseHoverElement = nullptr;
    // }
}

void UIManager::setPrimaryElement(UIControl* element) {
    m_primaryElement = element;
}

UIControl* UIManager::primaryElement() const {
    return m_primaryElement;
}
//
// void UIManager::setMainContext(UIContext* context)
//{
//    m_mainContext = context;
//}

void UIManager::updateMouseHover(UIRenderView* mouseEventSource, const Point& frameClientPosition) {
    if (LN_REQUIRE(mouseEventSource)) return;

#if 1
    UIElement* old = m_mouseHoverElement;

    UIElement* hoverdElement = mouseEventSource->onLookupMouseHoverElement(frameClientPosition);
    if (m_mouseHoverElement != hoverdElement) {
        clearMouseHover();

        if (hoverdElement) {
            m_mouseHoverElement = hoverdElement;
            auto args = UIMouseEventArgs::create(m_mouseHoverElement, UIEvents::MouseEnterEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, ModifierKeys::None, true);
            m_mouseHoverElement->raiseEvent(args);
        }

        // if (m_mouseHoverElement) {
        //     printf("m_mouseHoverElement: %p %s\n", m_mouseHoverElement, typeid(*(m_mouseHoverElement.get())).name());
        //     if (auto tt = dynamic_cast<UIText*>(m_mouseHoverElement.get())) {
        //         std::cout << tt->text() << std::endl;
        //     }
        // }
    }

#else
    UIElement* old = m_mouseHoverElement;

    // TODO:IME側のイベントを処理する
    // if ( m_pIme != NULL )
    //{
    //	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
    //	{
    //		goto EXIT;
    //	}
    //}

    // m_adornerLayer を調べる
    // m_mouseHoverElement = m_adornerLayer->checkMouseHoverElement(mousePos);
    // if (m_mouseHoverElement != nullptr) {
    //	goto EXIT;
    //}

    // Popup を調べる
    // for (auto& popup : m_popupContainers)
    //{
    //    m_mouseHoverElement = popup->GetPopup()->checkMouseHoverElement(mousePos);
    //    if (m_mouseHoverElement != nullptr) {
    //        goto EXIT;
    //    }
    //}

    // 通常のウィンドウのイベントを処理する
    // if (m_rootElement != NULL)
    {
        m_mouseHoverElement = mouseEventSource->onLookupMouseHoverElement(frameClientPosition);
        if (m_mouseHoverElement != nullptr) {
            goto EXIT;
        }
    }

    m_mouseHoverElement = nullptr;

EXIT:
    // 新旧それぞれの Element に MouseLeave、MouseEnter イベントを送る
    if (m_mouseHoverElement != old) {
        if (old) {
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseLeaveEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, true);
            old->raiseEvent(args);
        }

        if (m_mouseHoverElement) {
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseEnterEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, true);
            m_mouseHoverElement->raiseEvent(args);
        }
    }

    return false;
#endif
}

void UIManager::retainCapture(UIElement* element) {
    m_capturedElement = element;
}

void UIManager::releaseCapture(UIElement* element) {
    if (m_capturedElement == element) {
        m_capturedElement = nullptr;
    }
}

void UIManager::grabCursor(UIElement* element) {
    if (UIFrameWindow* window = static_cast<UIFrameWindow*>(element->getFrameWindow())) {
        window->platformWindow()->grabCursor();
    }
}

void UIManager::releaseCursor(UIElement* element) {
    if (UIFrameWindow* window = static_cast<UIFrameWindow*>(element->getFrameWindow())) {
        window->platformWindow()->releaseCursor();
    }
}

void UIManager::clearFocus(UIElement* element) {
    if (m_forcusedElement == element) {
        deactivateElement(m_forcusedElement);
        m_forcusedElement = nullptr;
    }
}

void UIManager::tryGetInputFocus(UIElement* element) {
    activateTree(element);
    m_forcusedElement = element;
}

void UIManager::activateTree(UIElement* element) {
    m_activationCache.clear();

    // VisualTree を遡ってすべて列挙
    UIElement* e = element;
    while (e) {
        m_activationCache.add(e);
        e = e->m_visualParent;
    }

    // activation と deactivation の分岐点を探しつつ deactivate する
    UIElement* branchRoot = nullptr;
    e = m_forcusedElement;
    while (e) {
        if (m_activationCache.contains(e)) {
            branchRoot = e;
            break;
        }

        // deactivate
        deactivateElement(e);

        e = e->m_visualParent;
    }

    // 基点から分岐点までを activate
    e = element;
    while (e) {
        if (e == branchRoot) {
            break;
        }

        // activate
        if (e->specialElementFlags().hasFlag(detail::UISpecialElementFlags::Control)) {
            static_cast<UIControl*>(e)->activateInternal();
        }
        if (e->focusable()) {
            auto args = UIEventArgs::create(e, UIEvents::GotFocusEvent, true);
            e->raiseEvent(args, UIEventRoutingStrategy::Direct);
        }

        if (e->m_visualParent) {
            e->m_visualParent->m_focusedVisualChild = e;
        }

        e = e->m_visualParent;
    }

    m_activationCache.clear();
}

void UIManager::postEvent(UIElement* target, UIEventArgs* e) {
    m_eventQueue.push_back({ target, e });
}

void UIManager::dispatchPostedEvents() {
    int count = m_eventQueue.size(); // 以下で発行したイベント内からさらに postEvent されたものは次のフレームで処理したいので、今回フレームで処理する数を決める
    while (count > 0) {
        auto& item = m_eventQueue.front();
        item.target->raiseEvent(item.args);
        m_eventQueue.pop_front();
        count--;
    }
}

void UIManager::handleGlobalRoutedEvent(UIEventArgs* e) {
    if (m_application) {
        ApplicationHelper::callOnRoutedEvent(m_application, e);
    }
}

void UIManager::addFrameWindow(UIFrameWindow* value) {
    LN_DCHECK(value);
    m_frameWindows.push(value);
}

void UIManager::removeFrameWindow(UIFrameWindow* value) {
    LN_DCHECK(value);
    m_frameWindows.remove(value);
}

void UIManager::registerActiveTimer(UIActiveTimer* timer) {
    m_activeTimers.add(timer);
}

void UIManager::unregisterActiveTimer(UIActiveTimer* timer) {
    m_activeTimers.remove(timer);
}

void UIManager::updateFrame(float elapsedSeconds) {
    if (m_application) {
        m_application->updateInertnal();
    }

    for (auto& timer : m_activeTimers) {
        timer->tick(elapsedSeconds);
    }

    // ユーザーコードの update に渡す前に、UI レイアウトを再計算しておき、
    // onUpdate 時点で Viewport サイズなどを正しく参照できるようにする。
    for (UIFrameWindow* value : m_frameWindows) {
        value->updateLayoutIfNeeded();
    }

    for (UIFrameWindow* value : m_frameWindows) {
        value->updateFrame(elapsedSeconds);
    }
}

void UIManager::clearMouseHover() {
    if (m_mouseHoverElement) {
        auto args = UIMouseEventArgs::create(m_mouseHoverElement, UIEvents::MouseLeaveEvent, MouseButtons::None, 0, 0, 0, ModifierKeys::None, true);
        m_mouseHoverElement->raiseEvent(args);
        m_mouseHoverElement = nullptr;
    }
}

void UIManager::clearFocus() {
    m_forcusedElement = nullptr;
}

void UIManager::handleDetachFromUITree(UIElement* element) {
    if (m_mouseHoverElement == element) {
        clearMouseHover();
    }

    if (m_forcusedElement == element) {
        clearFocus();
    }
}

bool UIManager::handleCommonInputCommands(UIEventArgs* e) {
    for (const auto& c : m_inputCommands) {
        if (c->testInputEvent(e)) {
            e->handled = true;
            UICommandEventArgs::raiseExecute(forcusedElement(), c);
            return true;
        }
    }
    return false;
}

// void UIManager::handleRootBubbleEvent(UIEventArgs* e)
//{
//     if (m_application) {
//
//     }
// }

void UIManager::deactivateElement(UIElement* element) {
    if (element->specialElementFlags().hasFlag(detail::UISpecialElementFlags::Control)) {
        static_cast<UIControl*>(element)->deactivateInternal();
    }
    if (element->focusable()) {
        auto args = UIEventArgs::create(element, UIEvents::LostFocusEvent, true);
        element->raiseEvent(args, UIEventRoutingStrategy::Direct);
    }
}

void UIManager::setupDefaultStyle() {
    auto defaultStyle = makeObject_deprecated<UIStyle>();
    defaultStyle->setupDefault();
    defaultStyle->fontSize = 15;

    auto theme = makeObject_deprecated<UITheme>();
    theme->setDefaultStyle(defaultStyle);

    theme->setSpacing(8); // MUI default
    theme->add(_TT("control.background"), UIColors::get(UIColorHues::Grey, 2));
    theme->add(_TT("collection.hoverBackground"), UIColors::get(UIColorHues::LightGreen, 0));
    theme->add(_TT("collection.selectedBackground"), UIColors::get(UIColorHues::LightGreen, 2));
    theme->add(_TT("tab.activeBackground"), UIColors::get(UIColorHues::White));
    theme->add(_TT("tab.inactiveBackground"), UIColors::get(UIColorHues::Grey, 3));

    Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

    // Color containerBackground = UIColors::get(UIColorHues::Grey, 3);
    // Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

    theme->setColor(UIThemeConstantPalette::DefaultBackgroundColor, Color::White.withAlpha(0.5f));
    theme->setColor(UIThemeConstantPalette::DefaultMainColor, UIColors::get(UIColorHues::Grey, 2));
    theme->setColor(UIThemeConstantPalette::DefaultTextColor, Color::Black);
    theme->setColor(UIThemeConstantPalette::PrimaryMainColor, UIColors::get(UIColorHues::LightGreen, 5));
    theme->setColor(UIThemeConstantPalette::PrimaryTextColor, Color::White);
    theme->setColor(UIThemeConstantPalette::SecondaryMainColor, UIColors::get(UIColorHues::Orange, 5));
    theme->setColor(UIThemeConstantPalette::SecondaryTextColor, Color::White);
    theme->setColor(UIThemeConstantPalette::ErrorMainColor, UIColors::get(UIColorHues::Red, 5));
    theme->setColor(UIThemeConstantPalette::ErrorTextColor, Color::White);
    theme->setColor(UIThemeConstantPalette::WarningMainColor, UIColors::get(UIColorHues::Orange, 3));
    theme->setColor(UIThemeConstantPalette::WarningTextColor, Color::Black);
    theme->setColor(UIThemeConstantPalette::InfoMainColor, UIColors::get(UIColorHues::Blue, 3));
    theme->setColor(UIThemeConstantPalette::InfoTextColor, Color::White);
    theme->setColor(UIThemeConstantPalette::SuccessMainColor, UIColors::get(UIColorHues::Green, 3));
    theme->setColor(UIThemeConstantPalette::SuccessTextColor, Color::White);

    auto sheet = makeObject_deprecated<UIStyleSheet>();

    {
        //--------------------------------
        // UIButton
        {
            auto e = sheet->addStyleSet(_TT("UIButton"));
            {
                auto s = e->mainStyleClass()->mainStyle();
                s->minWidth = 64; //
                s->minHeight = theme->lineContentHeight();
                s->setMargin(Thickness(8)); // TODO: spacing?
                s->setPadding(theme->spacing(1));
                //s->hAlignment = UIHAlignment::Center;
                //s->vAlignment = UIVAlignment::Center;
                s->horizontalContentAlignment = UIHAlignment::Center;
                s->verticalContentAlignment = UIVAlignment::Center;
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 3);
                s->cornerRadius = CornerRadius(4);
                s->shadowBlurRadius = 4;
                // s->shadowSpreadRadius = -1;
                s->shadowOffsetY = 1;
                s->shadowColor = Color(0, 0, 0, 0.5);
            }
            //// UIButton.test
            //{
            //	auto s = makeObject_deprecated<UIStyle>();
            //	s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
            //	c->addClassStyle(_TT("test", s);
            //}
            // UIButton:MouseOver
            {
                auto s = makeObject_deprecated<UIStyle>();
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
                e->mainStyleClass()->addStateStyle(_TT("MouseOver"), s);
            }
            //
            if (auto s = sheet->obtainStyle(_TT("UIButton:Pressed"))) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
                e->mainStyleClass()->addStateStyle(_TT("Pressed"), s);
                s->shadowBlurRadius = 0;
                s->shadowOffsetY = 0;
            }
        }
        //--------------------------------
        // UIToggleButton
        {
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton"))) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 2);
            }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton:MouseOver"))) {
            }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton:Pressed"))) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 6);
            }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton:Checked"))) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
            }
        }
        //--------------------------------
        // UIThumb
        {
            auto e = sheet->addStyleSet(_TT("UIThumb"));
            // UIThumb
            {
                auto s = e->mainStyleClass()->mainStyle();
            }
            // UIThumb.UITrack-Thumb
            {
                auto s = makeObject_deprecated<UIStyle>();

                s->setMargin(Thickness(2));
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
                s->cornerRadius = CornerRadius(4);
                //s->hAlignment = UIHAlignment::Stretch;
                //s->vAlignment = UIVAlignment::Stretch;

                s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
                e->mainStyleClass()->addStateStyle(_TT("UITrack-Thumb"), s);
            }
            if (auto s = sheet->obtainStyle(_TT("UIThumb.SplitterBar"))) {
                s->backgroundColor = Color(0, 1, 0, 0.2); // debug
                s->setMargin(Thickness(-2, -2, -2, -2));
            }
        }
        //--------------------------------
        // UITrack
        {
            if (auto s = sheet->obtainStyle(_TT("UITrack"))) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 2);
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton"))) {
                s->backgroundColor = Color(1, 1, 1, 0);
                s->cornerRadius = CornerRadius(0);
                s->shadowBlurRadius = 0;
                s->shadowOffsetY = 0;
                //s->hAlignment = UIHAlignment::Stretch;
                //s->vAlignment = UIVAlignment::Stretch;
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton:MouseOver"))) { // ベース要素である UIButton の VisualState を全て上書きする必要がある。CSS と同じ動作。
                s->backgroundColor = Color::Transparency;
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton:Pressed"))) {
                s->backgroundColor = Color::Transparency;
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton"))) {
                s->backgroundColor = Color(1, 1, 1, 0);
                s->cornerRadius = CornerRadius(0);
                s->shadowBlurRadius = 0;
                s->shadowOffsetY = 0;
                //s->hAlignment = UIHAlignment::Stretch;
                //s->vAlignment = UIVAlignment::Stretch;
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton:MouseOver"))) {
                s->backgroundColor = Color::Transparency;
            }
            if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton:Pressed"))) {
                s->backgroundColor = Color::Transparency;
            }
        }
        //--------------------------------
        // UIListView
        {
            if (auto s = sheet->obtainStyle(_TT("UIListView"))) {
                s->backgroundColor = theme->get(_TT("control.background"));
                s->setPadding(Thickness(0, theme->spacing(1)));
            }
        }
        //--------------------------------
        // UIListViewItem
        {
            if (auto s = sheet->obtainStyle(_TT("UIListViewItem"))) {
                s->minHeight = 30;
                s->setPadding(theme->spacing(1));
            }
            if (auto s = sheet->obtainStyle(_TT("UIListViewItem:Selected"))) {
                s->backgroundColor = theme->get(_TT("collection.selectedBackground"));
            }
        }
        //--------------------------------
        // UIListBoxItem
        {
            if (auto s = sheet->obtainStyle(_TT("UIListBoxItem"))) {
                // s->backgroundColor = Color::Green;
            }
            if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:MouseOver"))) {
                s->backgroundColor = theme->get(_TT("collection.hoverBackground"));
            }
            if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:Selected"))) {
                s->backgroundColor = theme->get(_TT("collection.selectedBackground"));
            }
        }
        //--------------------------------
        // UIComboBox
        {
            if (auto s = sheet->obtainStyle(_TT("UIComboBox"))) {
                s->minHeight = theme->lineContentHeight();
                s->setBorderColor(Color::Gray);
                s->borderThickness = 1;
                s->setPadding(theme->spacing(1));
                //s->hAlignment = UIHAlignment::Center;
                //s->vAlignment = UIVAlignment::Center;

                auto icon = makeObject_deprecated<UIStyleDecorator>();
                icon->setIconName(_TT("angle-down"), 15);
                icon->m_hAlignment = UIHAlignment::Right;
                icon->m_margin = Thickness(0, 0, theme->spacing(1), 0);
                icon->m_color = Color::Gray;
                s->decorators.add(icon);
            }
        }
        //--------------------------------
        // UIComboBoxItem
        {
            if (auto s = sheet->obtainStyle(_TT("UIComboBoxItem"))) {
            }
            if (auto s = sheet->obtainStyle(_TT("UIComboBoxItem:MouseOver"))) {
                s->backgroundColor = theme->get(_TT("collection.hoverBackground"));
            }
            if (auto s = sheet->obtainStyle(_TT("UIComboBoxItem:Selected"))) {
                s->backgroundColor = theme->get(_TT("collection.selectedBackground"));
            }
        }
        //--------------------------------
        // UITreeItem
        {
            if (auto s = sheet->obtainStyle(_TT("UITreeItem"))) {
                s->minHeight = 30;
                //s->hAlignment = UIHAlignment::Stretch;
                //s->vAlignment = UIVAlignment::Top;
                // s->borderThickness = 1;
                // s->setBorderColor(Color::Gray);
            }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander"))) { // VisualState によらず常に有効。個別にしたければ:Normalを付ける。
                s->width = 16;
                s->height = 16;
                //s->hAlignment = UIHAlignment::Center;
                //s->vAlignment = UIVAlignment::Center;
                s->backgroundColor = Color::Transparency;
            }
            // if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:MouseOver")) {
            // }
            // if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Pressed")) {
            // }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Checked"))) {
                auto icon = makeObject_deprecated<UIStyleDecorator>();
                icon->setIconName(_TT("angle-down"), 15);
                s->decorators.add(icon);
            }
            if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Unchecked"))) {
                auto icon = makeObject_deprecated<UIStyleDecorator>();
                icon->setIconName(_TT("angle-right"), 15);
                s->decorators.add(icon);
            }
        }
        //--------------------------------
        // UITabBar
        {
            if (auto s = sheet->obtainStyle(_TT("UITabBar"))) {
                s->backgroundColor = theme->get(_TT("control.background"));
            }
        }
        //--------------------------------
        // UITabItem
        {
            if (auto s = sheet->obtainStyle(_TT("UITabItem"))) {
                s->setPadding(theme->spacing(1));
                s->backgroundColor = theme->get(_TT("tab.inactiveBackground"));
            }
            if (auto s = sheet->obtainStyle(_TT("UITabItem:Selected"))) {
                s->backgroundColor = theme->get(_TT("tab.activeBackground"));
            }
        }
        //--------------------------------
        // UITextField
        {
            if (auto s = sheet->obtainStyle(_TT("UITextField"))) {
                s->setPadding(Thickness(4));
                s->borderThickness = Thickness(1);
                s->setBorderColor(Color::Gray);
                // s->backgroundColor = theme->get(_TT("tab.inactiveBackground");
            }
        }
        //--------------------------------
        // UIPopup
        {
            if (auto s = sheet->obtainStyle(_TT("UIPopup"))) {
                s->minWidth = 8;
                s->minHeight = 8;
                s->setPadding(theme->spacing(1));
                s->backgroundColor = UIColors::get(UIColorHues::White);
                s->cornerRadius = CornerRadius(4);
                s->shadowBlurRadius = 4;
                s->shadowOffsetY = 2;
            }
        }
        //--------------------------------
        // UIDialog
        {
            if (auto s = sheet->obtainStyle(_TT("UIDialog"))) {
                s->minWidth = 200;
                s->minHeight = 200;
                s->setPadding(theme->spacing(1));
                s->backgroundColor = UIColors::get(UIColorHues::White);
                s->cornerRadius = CornerRadius(4);
                s->shadowBlurRadius = 4;
                s->shadowOffsetY = 2;
            }
        }
    }

    m_styleContext->addStyleSheet(sheet);
    m_styleContext->mainTheme = theme;
    // m_styleContext->build();
}

} // namespace detail
} // namespace ln
