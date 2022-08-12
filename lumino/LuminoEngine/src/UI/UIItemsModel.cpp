
#include "Internal.hpp"
#include <LuminoEngine/Base/Regex.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>

namespace ln {

//==============================================================================
// UICollectionItemViewModel

UICollectionItemViewModel::UICollectionItemViewModel()
	//: m_owner(nullptr)
	//, m_row(0)
	//, m_column(0)
	//, m_parent(nullptr)
	//, m_data(nullptr)
{
}

//void UICollectionItemModel::init(UICollectionViewModel* owner)
//{
//	Object::init();
//	m_owner = owner;
//}
//
//void UICollectionItemModel::init(UICollectionViewModel* owner, int row, int column, UICollectionItemModel* parent, Variant* data)
//{
//	Object::init();
//	m_owner = owner;
//	m_row = row;
//	m_column = column;
//	m_parent = parent;
//	m_data = data;
//}
//
//String UICollectionItemModel::getData(const String& role)
//{
//	return m_owner->getData(this, role);
//}
//
//int UICollectionItemModel::getChildrenCount()
//{
//    return m_owner->getRowCount(this);
//}
//
//==============================================================================
// UICollectionViewModel
// https://doc.qt.io/qt-5/qabstractitemmodel.html#hasChildren

UICollectionViewModel::UICollectionViewModel()
{
}

void UICollectionViewModel::init()
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

void UIFileSystemCollectionModel::setRootPath(const Path& path)
{
    m_rootNode = makeObject_deprecated<FileSystemNode>(this, path);
    m_rootNode->attemptConstructChildNodes(this, false);
    //m_rootModel = makeObject_deprecated<FileSystemNode>(this, 0, 0, nullptr, makeVariant(m_rootNode));
    notify(UINotifyPropertyChangedEventArgs::create(nullptr, UIEvents::NotifyPropertyChanged, UICollectionChangedAction::Reset, 0, 0));
    //return m_rootModel;
}

void UIFileSystemCollectionModel::setExcludeFilters(List<String>* value)
{
    m_excludeFilters = value;
}

int UIFileSystemCollectionModel::getItemCount()
{
    if (m_rootNode)
        return m_rootNode->children().size();
    else
        return 0;
}

Ref<UICollectionItemViewModel> UIFileSystemCollectionModel::getItem(int row)
{
    if (!m_rootNode)
        return nullptr;
    else {
        auto& node = m_rootNode->children().at(row);
        node->attemptConstructChildNodes(this, false);
        return node;
    }
}

//String UIFileSystemCollectionModel::getData(Variant* index, int column, const String& role)
//{
//    auto node = index->getAsObject<FileSystemNode>();
//	return node->path().fileName();
//}

Path UIFileSystemCollectionModel::filePath(UICollectionItemViewModel* index)
{
    auto* node = dynamic_cast<FileSystemNode*>(index);
    if (!node) {
        LN_NOTIMPLEMENTED();
    }
    return node->path();
    //auto node = index->getAsObject<FileSystemNode>();
    //return node->path();
}

void UIFileSystemCollectionModel::refresh()
{
    if (m_rootNode) {
        m_rootNode->refreshHierarchical(this);
        notify();   // TODO: rootModel の分もここへ流すか・・・？いっそ rootModel 廃止した方がいいような気もする
    }
}

bool UIFileSystemCollectionModel::onTestFilter(const Path& path)
{
    return true;
}

//UIFileSystemCollectionModel::FileSystemNode* UIFileSystemCollectionModel::getNode(UICollectionItemModel* index)
//{
//    if (!index) {
//        return m_rootNode;
//    }
//	auto node = index->data()->getObject<FileSystemNode>();
//	constructChildNodes(node, false);
//	return node;
//}

//Ref<UIFileSystemCollectionModel::FileSystemNode> UIFileSystemCollectionModel::makeNode(const Path& path)
//{
//	auto node = makeRef<FileSystemNode>(path);
//	constructChildNodes(node, false);
//	return node;
//}

void UIFileSystemCollectionModel::FileSystemNode::attemptConstructChildNodes(UIFileSystemCollectionModel* owner, bool forceReset)
{
    if (m_dirty || forceReset) {
        m_children.clear();

        if (FileSystem::existsDirectory(m_path)) {
            auto dirs = FileSystem::getDirectories(m_path, StringView(), SearchOption::TopDirectoryOnly);
            for (auto& dir : dirs) {
                if (owner->testFilter(dir)) {
                    m_children.add(makeObject_deprecated<FileSystemNode>(m_owner, dir));
                }
            }
            auto files = FileSystem::getFiles(m_path, StringView(), SearchOption::TopDirectoryOnly);
            for (auto& file : files) {
                if (owner->testFilter(file)) {
                    m_children.add(makeObject_deprecated<FileSystemNode>(m_owner, file));
                }
            }
        }
        m_dirty = false;
    }
}

// TODO: サブツリー全部捜査しようとする。ツリーで展開されている部分や、リストで表示されている部分に絞るなど、対策しておく。
void UIFileSystemCollectionModel::FileSystemNode::refreshHierarchical(UIFileSystemCollectionModel* owner)
{
    attemptConstructChildNodes(owner, true);
    notify();

    for (auto& child : m_children) {
        child->refreshHierarchical(owner);
    }

    //int childCount = model->getChildrenCount();
    //for (int i = 0; i < childCount; i++) {
    //    auto child = getIndex(i, 0, model);
    //    refreshHierarchical(child);
    //}
}

//void UIFileSystemCollectionModel::constructChildNodes(FileSystemNode* node, bool reset)
//{
//	if (node->dirty || reset)
//	{
//        node->children.clear();
//
//		if (FileSystem::existsDirectory(node->path)) {
//			auto& path = node->path;
//			auto dirs = FileSystem::getDirectories(path, StringView(), SearchOption::TopDirectoryOnly);
//			for (auto& dir : dirs) {
//                if (testFilter(dir)) {
//                    node->children.add(makeRef<FileSystemNode>(dir));
//                }
//			}
//			auto files = FileSystem::getFiles(path, StringView(), SearchOption::TopDirectoryOnly);
//			for (auto& file : files) {
//               if (testFilter(file)) {
//                    node->children.add(makeRef<FileSystemNode>(file));
//                }
//			}
//		}
//		node->dirty = false;
//	}
//}

//// TODO: サブツリー全部捜査しようとする。ツリーで展開されている部分や、リストで表示されている部分に絞るなど、対策しておく。
//void UIFileSystemCollectionModel::refreshHierarchical(UICollectionItemModel* model)
//{
//    constructChildNodes(getNode(model), true);
//    model->notify();
//    
//    int childCount = model->getChildrenCount();
//    for (int i = 0; i < childCount; i++) {
//        auto child = getIndex(i, 0, model);
//        refreshHierarchical(child);
//    }
//}

bool UIFileSystemCollectionModel::testFilter(const Path& path)
{
    if (m_excludeFilters) {
        for (auto& f : m_excludeFilters) {
            if (StringHelper::match(f.c_str(), path.c_str())) {
                return false;
            }
        }
    }

    return onTestFilter(path);
}

} // namespace ln

