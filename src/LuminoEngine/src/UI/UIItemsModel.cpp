
#include "Internal.hpp"
#include <LuminoEngine/UI/UIItemsModel.hpp>

namespace ln {

//==============================================================================
// UIModelIndex

UIModelIndex::UIModelIndex()
	: m_row(0)
	, m_column(0)
	, m_parent(nullptr)
	, m_data(nullptr)
{
}

void UIModelIndex::init(int row, int column, UIModelIndex* parent, Variant* data)
{
	Object::init();
	m_row = row;
	m_column = column;
	m_parent = parent;
	m_data = data;
}

//==============================================================================
// UIItemsViewModel
// https://doc.qt.io/qt-5/qabstractitemmodel.html#hasChildren

UIItemsViewModel::UIItemsViewModel()
{
}

void UIItemsViewModel::init()
{
	Object::init();
}

//==============================================================================
// UIFileSystemItemsViewModel

UIFileSystemItemsViewModel::UIFileSystemItemsViewModel()
{
}

void UIFileSystemItemsViewModel::init()
{
	Object::init();
}

} // namespace ln

