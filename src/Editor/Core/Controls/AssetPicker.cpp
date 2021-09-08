
#include <Workspace.hpp>
#include "../Project/Project.hpp"
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
    m_button->setText(_TT("test"));
    m_button->connectOnChecked([this](UIEventArgs* e) { m_popup->open(); });
    m_button->connectOnUnchecked([this](UIEventArgs* e) { m_popup->close(); });
    m_hbox->addChild(m_button);

    m_popup = ln::makeObject<ln::UIPopup>();
    m_hbox->addChild(m_popup);
    m_popup->setPlacementTarget(m_button);
    {
        m_listView = ln::makeObject<ln::UIListView>();
        m_listView->getGridLayoutInfo()->layoutWeight = 1;
        //m_listView->connectOnItemClick(ln::bind(this, &TilesetList::listView_onItemClick));
        m_popup->addElement(m_listView);


        auto project = lna::Workspace::instance()->mainProject();
        auto assetRootDir = _TT("D:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets");//ln::Path(project->assetsDir(), _TT("Tilesets");

        m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
        m_model->setRootPath(assetRootDir);
        m_listView->setViewModel(m_model);
    }


    //auto popupContent = ln::makeObject<ln::UIText>();
    //popupContent->setText(_TT("POP");
    //layout->addChild(popup);
}



} // namespace lna
