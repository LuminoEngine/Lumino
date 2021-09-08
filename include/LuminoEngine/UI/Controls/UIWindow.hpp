#pragma once
#include "UIControl.hpp"

namespace ln {
    
/**
 * UIWindow
 */
LN_CLASS()
class UIWindow
    : public UIControl
{
    LN_OBJECT;
public:
    static Ref<UIWindow> create();

    /** ウィンドウを開き、視認できる状態にします。 */
    void open();

    /** ウィンドウを閉じます。見た目上閉じるだけで、インスタンスは破棄されません。 */
    void close();

LN_CONSTRUCT_ACCESS:
    UIWindow();

    /** init */
    LN_METHOD()
	bool init();

protected:
	virtual const String& elementName() const override { static String name = _TT("UIWindow"); return name; }

private:
    bool m_opend;
};

} // namespace ln

