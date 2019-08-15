
#include "AssetPicker.hpp"

namespace ln {
    
AssetPicker::AssetPicker()
{
}

void AssetPicker::init()
{
    UIContainerElement::init();

    m_hbox = makeObject<UIHBoxLayout2>();
    addElement(m_hbox);

    m_vbox = makeObject<UIVBoxLayout2>();
    m_hbox->addChild(m_vbox);

    m_button = makeObject<UIToggleButton>();
    m_button->setText(u"test");
    m_hbox->addChild(m_button);
}



} // namespace lna
