
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
using namespace ln;

class FileSystemTreeViewItem
    : public UITreeViewItem
{
public:

protected:
    virtual void onExpanded() override;
    virtual void onCollapsed() override;

LN_CONSTRUCT_ACCESS:
    FileSystemTreeViewItem();
    void init();
};



class FileSystemTreeView
    : public UITreeView
{
public:
    void setRootPath(const Path& path);

LN_CONSTRUCT_ACCESS:
	FileSystemTreeView();
	void init();

private:
    class FileSystemNode : public RefObject
    {
    public:
        FileSystemNode(const ln::Path& p) : path(p) {}
        Path path;
        List<Ref<FileSystemNode>> children;
        bool dirty = true;
    };

    FileSystemNode* getNode(UITreeViewItem* item) const;
    void makeNode(UITreeViewItem* parent, const Path& path);
    void constructChildNodes(FileSystemNode* node) const;

    Ref<FileSystemNode> m_rootNode; // dummy

    friend class FileSystemTreeViewItem;
};

FileSystemTreeView::FileSystemTreeView()
{
}

void FileSystemTreeView::init()
{
    UITreeView::init();
}

void FileSystemTreeView::setRootPath(const Path& path)
{
    m_rootNode = makeRef<FileSystemNode>(path);
    constructChildNodes(m_rootNode);
    for (auto& node : m_rootNode->children) {
        makeNode(nullptr, node->path);
    }
}

FileSystemTreeView::FileSystemNode* FileSystemTreeView::getNode(UITreeViewItem* item) const
{
    assert(item);
    return item->data()->getObject<FileSystemTreeView::FileSystemNode>();
}

void FileSystemTreeView::makeNode(UITreeViewItem* parent, const Path& path)
{
    auto node = makeRef<FileSystemNode>(path);
    constructChildNodes(node);

    auto text = makeObject<UITextBlock>();
    text->setText(path.fileName());

    auto item = makeObject<FileSystemTreeViewItem>();
    item->setContent(text);
    item->setData(makeVariant(node));

    if (!parent) {
        addElement(item);
    }
    else {
        parent->addChild(item);
    }
}

void FileSystemTreeView::constructChildNodes(FileSystemNode* node) const
{
    if (node->dirty) {
        if (FileSystem::existsDirectory(node->path)) {
            auto& path = node->path;
            auto dirs = FileSystem::getDirectories(path, StringRef(), SearchOption::TopDirectoryOnly);
            for (auto& dir : dirs) {
                node->children.add(makeRef<FileSystemNode>(dir));
            }
            //auto files = FileSystem::getFiles(path, StringRef(), SearchOption::TopDirectoryOnly);
            //for (auto& file : files) {
            //    node->children.add(makeRef<FileSystemNode>(file));
            //}
        }
        node->dirty = false;
    }
}





FileSystemTreeViewItem::FileSystemTreeViewItem()
{
}

void FileSystemTreeViewItem::init()
{
    UITreeViewItem::init();
}

void FileSystemTreeViewItem::onExpanded()
{
    auto node = static_cast<FileSystemTreeView*>(treeView())->getNode(this);
    static_cast<FileSystemTreeView*>(treeView())->constructChildNodes(node);
    for (auto& n : node->children) {
        static_cast<FileSystemTreeView*>(treeView())->makeNode(this, n->path);
    }
}

void FileSystemTreeViewItem::onCollapsed()
{
}





class UISandboxApp : public Application
{
public:
    virtual void onCreate()
    {
		//auto vm = makeObject<UIFileSystemItemsViewModel>();
		//auto root = vm->setRootPath(u"C:/Proj/LN/Lumino");
		//int count = vm->getRowCount(root);
		//for (int i = 0; i < count; i++) {
		//	auto index = vm->getIndex(i, 0, root);
		//	auto data = vm->getData(index, u"");
		//	std::cout << data << std::endl;

		//	int count2 = vm->getRowCount(index);
		//	for (int i2 = 0; i2 < count2; i2++) {
		//		auto index2 = vm->getIndex(i2, 0, index);
		//		auto data2 = vm->getData(index2, u"");
		//		std::cout << "  " << data2 << std::endl;
		//	}
		//}
		
		//{
		//	auto button1 = makeObject<UIButton>();
		//	button1->setWidth(200);
		//	button1->setHeight(32);
		//	button1->setText(u"Push button");
		//	button1->setHorizontalAlignment(HAlignment::Right);
		//	Engine::mainUIRoot()->addElement(button1);
		//}

		{
			auto button1 = makeObject<UIToggleButton>();
			button1->setWidth(200);
			button1->setHeight(32);
			button1->setText(u"Toggle button");
			button1->setHorizontalAlignment(HAlignment::Right);
			button1->setVerticalAlignment(VAlignment::Center);
			Engine::mainUIRoot()->addElement(button1);
		}


		//auto thumb1 = makeObject<UIThumb>();
		//thumb1->setWidth(20);
		//thumb1->setHeight(20);
		
		//auto track = makeObject<UITrack>();
		//track->setOrientation(Orientation::Vertical);
		//track->setWidth(20);
		//track->setHeight(100);
		//track->setMaximum(50);
		//track->setViewportSize(10);
		//Engine::mainUIRoot()->addElement(track);

        //auto scrollbar = makeObject<UIScrollBar>();
        //scrollbar->setOrientation(Orientation::Vertical);
        //scrollbar->setWidth(20);
        //scrollbar->setHeight(100);
        //scrollbar->setMaximum(50);
        //scrollbar->setViewportSize(10);
        //Engine::mainUIRoot()->addElement(scrollbar);

        //auto scrollview = makeObject<UIScrollViewer>();
        //scrollview->setWidth(200);
        //scrollview->setHeight(300);
        //Engine::mainUIRoot()->addElement(scrollview);


        //m_button1 = makeObject<UIButton>();
        //m_button1->setWidth(300);
        //m_button1->setHeight(400);
        //m_button1->setText(u"Lumino");
        //scrollview->addElement(m_button1);

		{
			auto treeView = makeObject<FileSystemTreeView>();
            treeView->setRootPath(u"D:/Proj/LN/Lumino");
			treeView->setWidth(200);
			treeView->setHeight(300);
			treeView->setBackgroundColor(UIColors::get(UIColorHues::Grey, 2));
			Engine::mainUIRoot()->addElement(treeView);

			//treeView->addElement(makeObject<UITreeViewItem>());
		}

    }

private:
    Ref<UIButton> m_button1;

};

int UISandboxMain()
{
	UISandboxApp app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
	return 0;
}




