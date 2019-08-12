
#include "Internal.hpp"
#include <LuminoEngine/Base/Regex.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>

namespace ln {

//==============================================================================
// UICollectionItemModel

UICollectionItemModel::UICollectionItemModel()
	: m_owner(nullptr)
	, m_row(0)
	, m_column(0)
	, m_parent(nullptr)
	, m_data(nullptr)
{
}

void UICollectionItemModel::init(UICollectionModel* owner)
{
	Object::init();
	m_owner = owner;
}

void UICollectionItemModel::init(UICollectionModel* owner, int row, int column, UICollectionItemModel* parent, Variant* data)
{
	Object::init();
	m_owner = owner;
	m_row = row;
	m_column = column;
	m_parent = parent;
	m_data = data;
}

String UICollectionItemModel::getData(const String& role)
{
	return m_owner->getData(this, role);
}

int UICollectionItemModel::getChildrenCount()
{
    return m_owner->getRowCount(this);
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
    m_rootModel = makeObject<UICollectionItemModel>(this, 0, 0, nullptr, makeVariant(m_rootNode));
    return m_rootModel;
}

void UIFileSystemCollectionModel::setExcludeFilters(List<String>* value)
{
    m_excludeFilters = value;
}

int UIFileSystemCollectionModel::getRowCount(UICollectionItemModel* index)
{
	auto node = getNode(index);
	return node->children.size();
}

Ref<UICollectionItemModel> UIFileSystemCollectionModel::getIndex(int row, int column, UICollectionItemModel* parent)
{
	auto parentNode = getNode(parent);
	return makeObject<UICollectionItemModel>(this, row, column, parent, makeVariant(parentNode->children[row]));
}

String UIFileSystemCollectionModel::getData(UICollectionItemModel* index, const String& role)
{
	auto node = getNode(index);
	return node->path.fileName();
}

Path UIFileSystemCollectionModel::filePath(UICollectionItemModel* itemModel)
{
    auto node = getNode(itemModel);
    return node->path;
}

void UIFileSystemCollectionModel::refresh()
{
    if (m_rootModel) {
        refreshHierarchical(m_rootModel);
        notify();   // TODO: rootModel の分もここへ流すか・・・？いっそ rootModel 廃止した方がいいような気もする
    }
}

UIFileSystemCollectionModel::FileSystemNode* UIFileSystemCollectionModel::getNode(UICollectionItemModel* index)
{
    if (!index) {
        return m_rootNode;
    }
	auto node = index->data()->getObject<FileSystemNode>();
	constructChildNodes(node, false);
	return node;
}

Ref<UIFileSystemCollectionModel::FileSystemNode> UIFileSystemCollectionModel::makeNode(const Path& path) const
{
	auto node = makeRef<FileSystemNode>(path);
	constructChildNodes(node, false);
	return node;
}

void UIFileSystemCollectionModel::constructChildNodes(FileSystemNode* node, bool reset) const
{
	if (node->dirty || reset)
	{
        node->children.clear();

		if (FileSystem::existsDirectory(node->path)) {
			auto& path = node->path;
			auto dirs = FileSystem::getDirectories(path, StringRef(), SearchOption::TopDirectoryOnly);
			for (auto& dir : dirs) {
				node->children.add(makeRef<FileSystemNode>(dir));
			}
			auto files = FileSystem::getFiles(path, StringRef(), SearchOption::TopDirectoryOnly);
			for (auto& file : files) {
               if (testFilter(file)) {
                    node->children.add(makeRef<FileSystemNode>(file));
                }
			}
		}
		node->dirty = false;
	}
}

// TODO: サブツリー全部捜査しようとする。ツリーで展開されている部分や、リストで表示されている部分に絞るなど、対策しておく。
void UIFileSystemCollectionModel::refreshHierarchical(UICollectionItemModel* model)
{
    constructChildNodes(getNode(model), true);
    model->notify();
    
    int childCount = model->getChildrenCount();
    for (int i = 0; i < childCount; i++) {
        auto child = getIndex(i, 0, model);
        refreshHierarchical(child);
    }
}

bool UIFileSystemCollectionModel::testFilter(const Path& path) const
{
    if (m_excludeFilters) {
        for (auto& f : m_excludeFilters) {
            if (StringHelper::match(f.c_str(), path.c_str())) {
                return false;
            }
        }
    }

    return true;
}

} // namespace ln

