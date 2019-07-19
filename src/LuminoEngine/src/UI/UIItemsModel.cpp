
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

void UIModelIndex::init()
{
	Object::init();
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

Ref<UIModelIndex> UIFileSystemItemsViewModel::setRootPath(const Path& path)
{
	m_rootNode = makeNode(path);
	return makeObject<UIModelIndex>(0, 0, nullptr, makeVariant(m_rootNode));
}

int UIFileSystemItemsViewModel::getRowCount(UIModelIndex* index)
{
	auto node = getNode(index);
	return node->children.size();
}

Ref<UIModelIndex> UIFileSystemItemsViewModel::getIndex(int row, int column, UIModelIndex* parent)
{
	auto parentNode = getNode(parent);
	return makeObject<UIModelIndex>(row, column, parent, makeVariant(parentNode->children[row]));
}

String UIFileSystemItemsViewModel::getData(UIModelIndex* index, const String& role)
{
	auto node = getNode(index);
	return node->path.fileName();
}

UIFileSystemItemsViewModel::FileSystemNode* UIFileSystemItemsViewModel::getNode(UIModelIndex* index)
{
	auto node = index->data()->getObject<FileSystemNode>();
	constructChildNodes(node);
	return node;
}

Ref<UIFileSystemItemsViewModel::FileSystemNode> UIFileSystemItemsViewModel::makeNode(const Path& path) const
{
	auto node = makeRef<FileSystemNode>(path);
	constructChildNodes(node);
	return node;
}

void UIFileSystemItemsViewModel::constructChildNodes(FileSystemNode* node) const
{
	if (node->dirty)
	{
		if (FileSystem::existsDirectory(node->path)) {
			auto& path = node->path;
			auto dirs = FileSystem::getDirectories(path, StringRef(), SearchOption::TopDirectoryOnly);
			for (auto& dir : dirs) {
				node->children.add(makeRef<FileSystemNode>(dir));
			}
			auto files = FileSystem::getFiles(path, StringRef(), SearchOption::TopDirectoryOnly);
			for (auto& file : files) {
				node->children.add(makeRef<FileSystemNode>(file));
			}
		}
		node->dirty = false;
	}
}

} // namespace ln

