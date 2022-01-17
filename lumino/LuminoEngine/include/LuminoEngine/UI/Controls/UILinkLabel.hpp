#pragma once
#include "../UIContainerElement.hpp"

namespace ln {

/**
 * クリックすることで RequestNavigate イベントを発行するコントロールです。
 *
 * RequestNavigate イベントは Bubble で親要素へ通知されます。
 */
LN_CLASS()
class UILinkLabel
	: public UIControl
{
	LN_OBJECT;
public:
	/** set text. */
	LN_METHOD()
    void setText(const StringView& value);
	
	/** set url. */
	LN_METHOD()
    void setUrl(const StringView& value);

LN_CONSTRUCT_ACCESS:
	UILinkLabel();
	bool init();

protected:
    void onRoutedEvent(UIEventArgs* e) override;

private:
	void raiseRequestNavigateEvent();

    Ref<UIText> m_textContent;
	String m_url;
	bool m_isPressed;
};

} // namespace ln

