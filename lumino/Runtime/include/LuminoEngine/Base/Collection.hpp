#pragma once
#include <LuminoCore/Runtime/Variant.hpp>
#include <LuminoEngine/Base/Serializer.hpp>

namespace ln {
class Variant;

///**
// * 変更通知機能を持つ、Object 用の可変長配列のコンテナテンプレートクラスです。
// */
//// Note: こんなの必要なの？
//// 1つめの用途は、WPF の ObservableCollection. 主に UI の ListBox などへバインドして、リストの内容を簡単にコントロールするために使用する。
//// 2つめの用途は、Binding 経由で公開する "列挙可能な" リストとして使うこと。こちらは set の利用はあまり想定していないが、WorldObject の子ノードを検索したりするときに欲しいもの。
//// TODO: obsolete
//template<class T>
//class ObjectCollection : public Object
//{
//public:
//	typedef typename std::vector<Ref<T>>::value_type value_type;
//	typedef typename std::vector<Ref<T>>::difference_type difference_type;
//	typedef typename std::vector<Ref<T>>::pointer pointer;
//	typedef typename std::vector<Ref<T>>::const_pointer const_pointer;
//	typedef typename std::vector<Ref<T>>::iterator iterator;
//	typedef typename std::vector<Ref<T>>::const_iterator const_iterator;
//	typedef typename std::vector<Ref<T>>::reference reference;
//	typedef typename std::vector<Ref<T>>::const_reference const_reference;
//
//	/** 配列が空であるかを確認します。*/
//	bool isEmpty() const LN_NOEXCEPT { return m_data.empty(); }
//
//	/** 格納されている要素の数を取得します。*/
//	int size() const LN_NOEXCEPT { return m_data.size(); }
//
//	/** 指定したインデックスにある要素への参照を取得します。*/
//	const_reference at(int index) const { return m_data.at(index); }
//
//	/** 指定したインデックスへ要素を設定します。*/
//	void setAt(int index, const value_type& item) { setItem(index, item); }
//
//	/** 末尾に要素を追加します。*/
//	void add(const value_type& item) { insertAt(size(), item); }
//
//	/** 全ての要素を削除します。*/
//	void clear() { clearItems(); }
//
//	/** item に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
//	bool remove(const value_type& item)
//	{
//		int index = indexOf(item);
//		if (index >= 0) {
//			removeAt(index);
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//
//	int indexOf(const value_type& item) const
//	{
//		if (isEmpty())
//			return -1;
//
//		const_iterator itr = std::find(m_data.begin(), m_data.end(), item);
//		if (itr != m_data.end()) {
//			return itr - m_data.begin();
//		}
//		return -1;
//	}
//
//	/** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
//	template<typename TPred>
//	Optional<T> findIf(TPred pred) const
//	{
//		auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
//		if (itr != end()) {
//			return *itr;
//		}
//		return nullptr;
//	}
//
//	//----------------------------------------------------------------------------
//	/** @name STL interface */
//	/** @{ */
//
//	/** 先頭要素を指すイテレータを取得します。 */
//	iterator begin() LN_NOEXCEPT { return m_data.begin(); }
//
//	/** 先頭要素を指すイテレータを取得します。 */
//	const_iterator begin() const LN_NOEXCEPT { return m_data.begin(); }
//
//	/** 末尾の次を指すイテレータを取得します。 */
//	iterator end() LN_NOEXCEPT { return m_data.end(); }
//
//	/** 末尾の次を指すイテレータを取得します。 */
//	const_iterator end() const LN_NOEXCEPT { return m_data.end(); }
//
//	/** 先頭要素を指す読み取り専用イテレータを取得します。 */
//	const_iterator cbegin() const LN_NOEXCEPT { return m_data.cbegin(); }
//
//	/** 末尾の次を指す読み取り専用イテレータを取得します。 */
//	const_iterator cend() const LN_NOEXCEPT { return m_data.cend(); }
//
//	/** @} */
//
//LN_CONSTRUCT_ACCESS:
//	/** 空の配列で新しい配列を構築します。 */
//	ObjectCollection() {}
//
//protected:
//	/** 指定したインデックスの位置に要素を挿入します。*/
//	virtual void insertAt(int index, const value_type& item)
//	{
//		m_data.insert(m_data.begin() + index, item);
//	}
//
//	/** 全ての要素を削除します。*/
//	virtual void clearItems()
//	{
//		m_data.clear();
//	}
//
//	/** 指定したインデックスにある要素を削除します。*/
//	virtual void removeAt(int index)
//	{
//		m_data.erase(m_data.begin() + index);
//	}
//
//	/** 指定したインデックス位置にある要素を置き換えます。*/
//	virtual void setItem(int index, const value_type& item)
//	{
//		m_data[index] = item;
//	}
//
//private:
//	std::vector<Ref<T>> m_data;
//
//	template<class T> friend
//	Ref<ObjectCollection<T>> makeObjectCollection();
//};
//
///** Ref<Collection> を構築します。 */
//template<class TItem>
//inline Ref<ObjectCollection<TItem>> makeObjectCollection()
//{
//	return makeObject<ObjectCollection<TItem>>();//Ref<ObjectCollection<TItem>>(LN_NEW ObjectCollection<TItem>(), false);
//}









/** This enum describes the action that caused a CollectionChanged event. */
enum class NotifyCollectionChangedAction
{
	/** The contents of the collection changed dramatically. */
	Reset,

	/** Item were added to the collection. */
	Add,

	/** Item were removed from the collection. */
	Remove,

	/** Item were replaced in the collection. */
	Replace,

	/** Item were moved within the collection. */
	Move,
};

struct NotifyCollectionChangedEventArgs
{
	NotifyCollectionChangedAction m_action = NotifyCollectionChangedAction::Reset;
	Ref<Variant> m_newItem;
	Ref<Variant> m_oldItem;
	int m_newStartingIndex = -1;
	int m_oldStartingIndex = -1;
};



class CollectionObject;

class INotifyCollectionChangedHandler
{
protected:
	virtual void onNotifyCollectionChanged(const NotifyCollectionChangedEventArgs* args) = 0;

	friend class CollectionObject;
};

// ListBox などにバインドしたりする ObservableCollection のベース。
// I/F は Variant. 
// NotifyCollectionChanged は、Model 側に持たせたいコレクションのI/F.
// Owner は、View 側に持たせたいコレクションのI/F.
// ListBoxItem や WorldObject などのリスト.
// Owner(ListBox) に変更通知を出すために使用する.
class CollectionObject : public Object
{
public:

	// TODO: internal
	virtual int getItemCount() const = 0;
	virtual const Variant* getItemAsVariant(int index) = 0;	// この Variant は共有インスタンスでかまわない

	void addNotifyCollectionChangedHandler(INotifyCollectionChangedHandler* handler);
	void removeNotifyCollectionChangedHandler(INotifyCollectionChangedHandler* handler);

protected:
	CollectionObject();
	virtual ~CollectionObject();
	void raiseCollectionChanged(const NotifyCollectionChangedEventArgs* args);

private:
	List<INotifyCollectionChangedHandler*> m_handlers;
};




/**
 * 変更通知機能を持つ、Object 用の可変長配列のコンテナテンプレートクラスです。
 */
// Note: こんなの必要なの？
// 1つめの用途は、WPF の ObservableCollection. 主に UI の ListBox などへバインドして、リストの内容を簡単にコントロールするために使用する。
// 2つめの用途は、Binding 経由で公開する "列挙可能な" リストとして使うこと。こちらは set の利用はあまり想定していないが、WorldObject の子ノードを検索したりするときに欲しいもの。
template<class T>
class Collection : public CollectionObject
{
public:
	typedef typename std::vector<T>::value_type value_type;
	typedef typename std::vector<T>::difference_type difference_type;
	typedef typename std::vector<T>::pointer pointer;
	typedef typename std::vector<T>::const_pointer const_pointer;
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::reference reference;
	typedef typename std::vector<T>::const_reference const_reference;

	/** 配列が空であるかを確認します。*/
	bool isEmpty() const LN_NOEXCEPT { return m_data.empty(); }

	/** 格納されている要素の数を取得します。*/
	int size() const LN_NOEXCEPT { return static_cast<int>(m_data.size()); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	const_reference at(int index) const { return m_data.at(index); }

	/** 指定したインデックスへ要素を設定します。*/
	void setAt(int index, const value_type& item) { setItem(index, item); }

	/** 末尾に要素を追加します。*/
	void add(const value_type& item) { insertAt(size(), item); }

	/** 全ての要素を削除します。*/
	void clear() { clearItems(); }

	/** item に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
	bool remove(const value_type& item)
	{
		int index = indexOf(item);
		if (index >= 0) {
			removeAt(index);
			return true;
		}
		else {
			return false;
		}
	}

	int indexOf(const value_type& item) const
	{
		if (isEmpty())
			return -1;

		const_iterator itr = std::find(m_data.begin(), m_data.end(), item);
		if (itr != m_data.end()) {
			return static_cast<int>(itr - m_data.begin());
		}
		return -1;
	}

	/** 指定した要素がこの配列内に存在するかどうかを判断します。*/
	bool contains(const value_type& item) const
	{
		return std::find(m_data.begin(), m_data.end(), item) != m_data.end();
	}

	/** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
	template<typename TPred>
	Optional_deprecated<T> findIf(TPred pred) const
	{
		auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
		if (itr != end()) {
			return *itr;
		}
		return nullptr;
	}

	List<T> toArray() const
	{
		return List<T>(m_data.begin(), m_data.end());
	}

	//----------------------------------------------------------------------------
	/** @name STL interface */
	/** @{ */

	/** 先頭要素を指すイテレータを取得します。 */
	iterator begin() LN_NOEXCEPT { return m_data.begin(); }

	/** 先頭要素を指すイテレータを取得します。 */
	const_iterator begin() const LN_NOEXCEPT { return m_data.begin(); }

	/** 末尾の次を指すイテレータを取得します。 */
	iterator end() LN_NOEXCEPT { return m_data.end(); }

	/** 末尾の次を指すイテレータを取得します。 */
	const_iterator end() const LN_NOEXCEPT { return m_data.end(); }

	/** 先頭要素を指す読み取り専用イテレータを取得します。 */
	const_iterator cbegin() const LN_NOEXCEPT { return m_data.cbegin(); }

	/** 末尾の次を指す読み取り専用イテレータを取得します。 */
	const_iterator cend() const LN_NOEXCEPT { return m_data.cend(); }

	/** @} */

	//----------------------------------------------------------------------------
	/** @name Binding interface */
	/** @{ */

	int getLength() const { return m_data.size(); }
	const_reference getItem(int index) const { return m_data.at(index); }

	/** @} */

LN_CONSTRUCT_ACCESS:
	/** 空の配列で新しい配列を構築します。 */
	Collection()
		: m_newItemCache(makeVariant())
		, m_oldItemCache(makeVariant())
	{
    }

    virtual ~Collection()
    {
    }

protected:
	/** 指定したインデックスの位置に要素を挿入します。*/
	virtual void insertAt(int index, const value_type& item)
	{
		m_data.insert(m_data.begin() + index, item);
		m_newItemCache->assign(item);
		NotifyCollectionChangedEventArgs e = { NotifyCollectionChangedAction::Add, m_newItemCache, nullptr, index, -1 };
		raiseCollectionChanged(&e);
	}

	/** 全ての要素を削除します。*/
	virtual void clearItems()
	{
		m_data.clear();
		NotifyCollectionChangedEventArgs e = { NotifyCollectionChangedAction::Reset, nullptr, nullptr, -1, -1 };
		raiseCollectionChanged(&e);
	}

	/** 指定したインデックスにある要素を削除します。*/
	virtual void removeAt(int index)
	{
		m_oldItemCache->assign(m_data[index]);
		m_data.erase(m_data.begin() + index);
		NotifyCollectionChangedEventArgs e = { NotifyCollectionChangedAction::Remove, nullptr, m_oldItemCache, -1, index };
		raiseCollectionChanged(&e);
	}

	/** 指定したインデックス位置にある要素を置き換えます。*/
	virtual void setItem(int index, const value_type& item)
	{
		m_oldItemCache->assign(m_data[index]);
		m_data[index] = item;
		m_newItemCache->assign(m_data[index]);
		NotifyCollectionChangedEventArgs e = { NotifyCollectionChangedAction::Replace, m_newItemCache, m_oldItemCache, index, index };
		raiseCollectionChanged(&e);
	}

	/** 要素を移動します。*/
	virtual void moveItem(int oldIndex, int newIndex)
	{
		m_oldItemCache->assign(m_data[oldIndex]);
		m_newItemCache->assign(m_data[oldIndex]);
		value_type item = m_data[oldIndex];
		m_data.erase(m_data.begin() + oldIndex);
		m_data.insert(m_data.begin() + newIndex, item);
		NotifyCollectionChangedEventArgs e = { NotifyCollectionChangedAction::Move, m_newItemCache, m_oldItemCache, newIndex, oldIndex };
		raiseCollectionChanged(&e);
	}

	int getItemCount() const override
	{
		return static_cast<int>(m_data.size());
	}

	const Variant* getItemAsVariant(int index) override
	{
		m_newItemCache->assign(m_data[index]);
		return m_newItemCache;
	}

	void serialize_deprecated(Serializer2_deprecated& ar) override
	{
		if (ar.isSaving()) {
			ar.beginWriteList();
			for (auto& v : m_data) {
				ar & v;
			}
			ar.endWriteList();
		}
		else {
			int size = 0;
			if (ar.beginReadList(&size)) {
				m_data.resize(size);
				for (auto& v : m_data) {
					ar & v;
				}
				ar.endReadList();
			}
		}
	}

	friend class Serializer2;

private:
	std::vector<T> m_data;
	Ref<Variant> m_newItemCache;
	Ref<Variant> m_oldItemCache;

	template<class TItem> friend
	Ref<Collection<TItem>> makeCollection();
};

/** Ref<Collection> を構築します。 */
template<class TItem>
inline Ref<Collection<TItem>> makeCollection()
{
	return makeObject<Collection<TItem>>();
}






} // namespace ln
