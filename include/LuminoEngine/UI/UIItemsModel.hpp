#pragma once
#include "../Base/Variant.hpp"
#include "UIContainerElement.hpp"

namespace ln {
class Material;

// row, column, parent を使って、Model 内のデータを一意に識別するためのキー
class UIModelIndex
	: public Object
{
public:
	int row() const { return m_row; }
	int column() const { return m_column; }
	UIModelIndex* parent() const { return m_parent; }
	Variant* data() const { return m_data; }

LN_CONSTRUCT_ACCESS:
	UIModelIndex();
	void init(int row, int column, UIModelIndex* parent = nullptr, Variant* data = nullptr);

private:
	int m_row;
	int m_column;
	Ref<UIModelIndex> m_parent;
	Ref<Variant> m_data;
};

class UIItemsViewModel	// TODO: naming, WPF の CollectionView や CollectionViewSource に近い
    : public Object
{
public:
	// index が示すデータが持つ、子 row の数
	virtual int getRowCount(const UIModelIndex* index) = 0;

	// index が示すデータ
	//virtual Ref<Variant> getData(const UIModelIndex* index, const String& role) const;
	virtual String getData(const UIModelIndex* index, const String& role) const = 0;

LN_CONSTRUCT_ACCESS:
	UIItemsViewModel();
	void init();


private:
};

// https://docs.microsoft.com/ja-jp/dotnet/api/system.windows.data.collectionview?view=netframework-4.8
// WPF の CollectionView 相当。
class UIItemsModelProxy
{
public:

private:
};

class UIFileSystemItemsViewModel
	: public UIItemsViewModel
{
public:


LN_CONSTRUCT_ACCESS:
	UIFileSystemItemsViewModel();
	void init();

protected:

private:
};

} // namespace ln

