
#include "Internal.hpp"
#include <LuminoEngine/UI/UIItemsModel.hpp>

namespace ln {

//==============================================================================
// UICollectionItemModel

UICollectionItemModel::UICollectionItemModel()
	: m_row(0)
	, m_column(0)
	, m_parent(nullptr)
	, m_data(nullptr)
{
}

void UICollectionItemModel::init()
{
	Object::init();
}

void UICollectionItemModel::init(int row, int column, UICollectionItemModel* parent, Variant* data)
{
	Object::init();
	m_row = row;
	m_column = column;
	m_parent = parent;
	m_data = data;
}

//==============================================================================
// UICollectionModel
// https://doc.qt.io/qt-5/qabstractitemmodel.html#hasChildren

UICollectionModel::UICollectionModel()
{
}

void UICollectionModel::init()
{
	Object::init();
}

//==============================================================================
// UIFileSystemCollectionModel

UIFileSystemCollectionModel::UIFileSystemCollectionModel()
{
}

void UIFileSystemCollectionModel::init()
{
	Object::init();
}

Ref<UICollectionItemModel> UIFileSystemCollectionModel::setRootPath(const Path& path)
{
	m_rootNode = makeNode(path);
    m_rootModel = makeObject<UICollectionItemModel>(0, 0, nullptr, makeVariant(m_rootNode));
    return m_rootModel;
}

int UIFileSystemCollectionModel::getRowCount(UICollectionItemModel* index)
{
	auto node = getNode(index);
	return node->children.size();
}

Ref<UICollectionItemModel> UIFileSystemCollectionModel::getIndex(int row, int column, UICollectionItemModel* parent)
{
	auto parentNode = getNode(parent);
	return makeObject<UICollectionItemModel>(row, column, parent, makeVariant(parentNode->children[row]));
}

String UIFileSystemCollectionModel::getData(UICollectionItemModel* index, const String& role)
{
	auto node = getNode(index);
	return node->path.fileName();
}

UIFileSystemCollectionModel::FileSystemNode* UIFileSystemCollectionModel::getNode(UICollectionItemModel* index)
{
    if (!index) {
        return m_rootNode;
    }
	auto node = index->data()->getObject<FileSystemNode>();
	constructChildNodes(node);
	return node;
}

Ref<UIFileSystemCollectionModel::FileSystemNode> UIFileSystemCollectionModel::makeNode(const Path& path) const
{
	auto node = makeRef<FileSystemNode>(path);
	constructChildNodes(node);
	return node;
}

void UIFileSystemCollectionModel::constructChildNodes(FileSystemNode* node) const
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

