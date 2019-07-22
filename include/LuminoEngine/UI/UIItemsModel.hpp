#pragma once
#include "../Base/Variant.hpp"
#include "UIContainerElement.hpp"

namespace ln {
class Material;



// row, column, parent を使って、Model 内のデータを一意に識別するためのキー
class UICollectionItemModel
	: public Object
{
public:
	int row() const { return m_row; }
	int column() const { return m_column; }
	UICollectionItemModel* parent() const { return m_parent; }
	Variant* data() const { return m_data; }

	bool isRoot() const { return !m_parent; }

LN_CONSTRUCT_ACCESS:
	UICollectionItemModel();
	void init();
	void init(int row, int column, UICollectionItemModel* parent = nullptr, Variant* data = nullptr);

private:
	int m_row;
	int m_column;
	Ref<UICollectionItemModel> m_parent;
	Ref<Variant> m_data;
};

class UICollectionModel	// TODO: naming, WPF の CollectionView や CollectionViewSource に近い
    : public Object
{
public:
	// index が示すデータが持つ、子 row の数
	virtual int getRowCount(UICollectionItemModel* index) = 0;

	virtual Ref<UICollectionItemModel> getIndex(int row, int column, UICollectionItemModel* parent) = 0;

	// index が示すデータ
	//virtual Ref<Variant> getData(const UICollectionItemModel* index, const String& role) const;
	virtual String getData(UICollectionItemModel* index, const String& role) = 0;

LN_CONSTRUCT_ACCESS:
	UICollectionModel();
	void init();


private:
};

class UIFileSystemCollectionModel
	: public UICollectionModel
{
public:
	Ref<UICollectionItemModel> setRootPath(const Path& path);

	virtual int getRowCount(UICollectionItemModel* index) override;
	virtual Ref<UICollectionItemModel> getIndex(int row, int column, UICollectionItemModel* parent) override;
	virtual String getData(UICollectionItemModel* index, const String& role) override;

LN_CONSTRUCT_ACCESS:
	UIFileSystemCollectionModel();
	void init();

protected:

private:
	class FileSystemNode : public RefObject
	{
	public:
		FileSystemNode(const ln::Path& p) : path(p) {}
		Path path;
		List<Ref<FileSystemNode>> children;
		bool dirty = true;
	};

	FileSystemNode* getNode(UICollectionItemModel* index);
	Ref<FileSystemNode> makeNode(const Path& path) const;
	void constructChildNodes(FileSystemNode* node) const;

	Ref<FileSystemNode> m_rootNode;
    Ref<UICollectionItemModel> m_rootModel;
};

// https://docs.microsoft.com/ja-jp/dotnet/api/system.windows.data.collectionview?view=netframework-4.8
// WPF の CollectionView 相当。
class UIItemsModelProxy
{
public:

private:
};

} // namespace ln

