#pragma once

namespace ln {

class AssetPicker
	: public UIContainerElement
{
public:

protected:

LN_CONSTRUCT_ACCESS:
    AssetPicker();
	void init();

private:
    Ref<UIHBoxLayout2> m_hbox;
    Ref<UIVBoxLayout2> m_vbox;
    Ref<UIToggleButton> m_button;
};

} // namespace ln
