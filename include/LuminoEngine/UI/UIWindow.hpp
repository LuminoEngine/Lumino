#pragma once
#include "UIContainerElement.hpp"

namespace ln {

class UIWindow
    : public UIContainerElement
{
public:
    static Ref<UIWindow> create();

    /** ウィンドウを開き、視認できる状態にします。 */
    void open();

    /** ウィンドウを閉じます。見た目上閉じるだけで、インスタンスは破棄されません。 */
    void close();

LN_CONSTRUCT_ACCESS:
    UIWindow();
	void init();

protected:
	virtual const String& elementName() const override { static String name = u"UIWindow"; return name; }

private:
    bool m_opend;
};

} // namespace ln

