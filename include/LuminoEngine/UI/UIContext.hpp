#pragma once

namespace ln {
class UIElement;

class UIContext
	: public Object
{
public:

public: // TODO: internal
    void setLayoutRootElement(UIElement* element);
    void addElement(UIElement* element);
    bool updateMouseHover(const Point& mousePos);
    UIElement* mouseHoverElement() const { return m_mouseHoverElement; }

LN_CONSTRUCT_ACCESS:
    UIContext();
    virtual ~UIContext() = default;
	void initialize();

private:
    Ref<UIElement> m_layoutRootElement;
    UIElement* m_mouseHoverElement;
};

} // namespace ln

