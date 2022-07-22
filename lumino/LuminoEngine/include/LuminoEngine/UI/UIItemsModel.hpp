#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class UICollectionViewModel;

// row, column, parent を使って、Model 内のデータを一意に識別するためのキー
// TODO: [2020/2/5] ViewModel 扱いじゃなくていいと思う。Qt だと Ploxy, Kivy は Adapter.
// Adapter は ItemsSource や Generator(Template) 等を持つ。
// ちなみに Unity は Adapter 相当のデータを内部に持っているみたい (コンストラクタで Generator とかを渡す)
// https://docs.unity3d.com/ScriptReference/UIElements.ListView.html
class UICollectionItemViewModel
	: public UIViewModel
{
public:
	//int row() const { return m_row; }
	//int column() const { return m_column; }
	//UICollectionItemViewModel* parent() const { return m_parent; }
	//Variant* data() const { return m_data; }

	//bool isRoot() const { return !m_parent; }

	virtual String getValue(const String& role) { return String::Empty; }
	virtual int getChildrenCount() const { return 0; }
	virtual Ref<UICollectionItemViewModel> getItem(int row) const { return nullptr; }

LN_CONSTRUCT_ACCESS:
	UICollectionItemViewModel();
	//void init(UICollectionViewModel* owner);
	//void init(UICollectionViewModel* owner, int row, int column, UICollectionItemModel* parent = nullptr, Variant* data = nullptr);

private:
	//UICollectionViewModel* m_owner;
	//int m_row;
	//int m_column;
	//Ref<UICollectionItemModel> m_parent;
	//Ref<Variant> m_data;
};

class UICollectionViewModel	// TODO: naming, WPF の CollectionView や CollectionViewSource に近い
    : public UIViewModel
{
public:
	// index が示すデータが持つ、子 row の数. nullptr は root.
	virtual int getItemCount() = 0;

	virtual Ref<UICollectionItemViewModel> getItem(int row) = 0;

	// index が示すデータ
	//virtual Ref<Variant> getData(const UICollectionItemModel* index, const String& role) const;
	//virtual String getData(UICollectionItemViewModel* index, int column, const String& role) = 0;

LN_CONSTRUCT_ACCESS:
	UICollectionViewModel();
	void init();


private:
};

class UIFileSystemCollectionModel
	: public UICollectionViewModel
{
public:
	void setRootPath(const Path& path);
    void setExcludeFilters(List<String>* value);

	virtual int getItemCount() override;
	virtual Ref<UICollectionItemViewModel> getItem(int row) override;

    virtual Path filePath(UICollectionItemViewModel* index);

    void refresh();

LN_CONSTRUCT_ACCESS:
	UIFileSystemCollectionModel();
	void init();

protected:
    virtual bool onTestFilter(const Path& path);

private:
	class FileSystemNode : public UICollectionItemViewModel
	{
	public:
		//FileSystemNode(const ln::Path& p) : m_path(p) {}
		bool init(UIFileSystemCollectionModel* owner, const ln::Path& p)
		{
			UICollectionItemViewModel::init();
			m_owner = owner;
			m_path = p; return true;
		}

		const Path& path() const { return m_path; }
		const List<Ref<FileSystemNode>>& children() const { return m_children; }

		void attemptConstructChildNodes(UIFileSystemCollectionModel* owner, bool forceReset);
		void refreshHierarchical(UIFileSystemCollectionModel* owner);

		String getValue(const String& role) override { return m_path.fileName(); }
		int getChildrenCount() const override  { return m_children.size(); }
		Ref<UICollectionItemViewModel> getItem(int row) const override
		{
			m_children[row]->attemptConstructChildNodes(m_owner, false);
			return m_children[row];
		}

	private:
		UIFileSystemCollectionModel* m_owner;
		Path m_path;
		List<Ref<FileSystemNode>> m_children;
		bool m_dirty = true;
	};

	//FileSystemNode* getNode(UICollectionItemModel* index);
	//Ref<FileSystemNode> makeNode(const Path& path);
	//void constructChildNodes(FileSystemNode* node, bool reset);
    //void refreshHierarchical(UICollectionItemModel* model);
    bool testFilter(const Path& path);

	Ref<FileSystemNode> m_rootNode;
    //Ref<UICollectionItemModel> m_rootModel;
    Ref<List<String>> m_excludeFilters;
};

// https://docs.microsoft.com/ja-jp/dotnet/api/system.windows.data.collectionview?view=netframework-4.8
// WPF の CollectionView 相当。
class UIItemsModelProxy
{
public:

private:
};

} // namespace ln

