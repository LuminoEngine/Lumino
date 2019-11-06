
#pragma once

namespace ln {


/**
 * 変更通知機能を持つ、Object 用の可変長配列のコンテナテンプレートクラスです。
 */
template<class T>
class ObjectCollection : public Object
{
public:
	typedef typename std::vector<Ref<T>>::value_type value_type;
	typedef typename std::vector<Ref<T>>::difference_type difference_type;
	typedef typename std::vector<Ref<T>>::pointer pointer;
	typedef typename std::vector<Ref<T>>::const_pointer const_pointer;
	typedef typename std::vector<Ref<T>>::iterator iterator;
	typedef typename std::vector<Ref<T>>::const_iterator const_iterator;
	typedef typename std::vector<Ref<T>>::reference reference;
	typedef typename std::vector<Ref<T>>::const_reference const_reference;

	/** 配列が空であるかを確認します。*/
	bool isEmpty() const LN_NOEXCEPT { return m_data.empty(); }

	/** 格納されている要素の数を取得します。*/
	int size() const LN_NOEXCEPT { return m_data.size(); }

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
			return itr - m_data.begin();
		}
		return -1;
	}

	/** 指定した条件と一致する最初の要素を検索し、その要素を指す Optional を返します。見つからなければ値を保持していません。 */
	template<typename TPred>
	Optional<T> findIf(TPred pred) const
	{
		auto itr = std::find_if(m_data.begin(), m_data.end(), pred);
		if (itr != end()) {
			return *itr;
		}
		return nullptr;
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

LN_CONSTRUCT_ACCESS:
	/** 空の配列で新しい配列を構築します。 */
	ObjectCollection() {}

protected:
	/** 指定したインデックスの位置に要素を挿入します。*/
	virtual void insertAt(int index, const value_type& item)
	{
		m_data.insert(m_data.begin() + index, item);
	}

	/** 全ての要素を削除します。*/
	virtual void clearItems()
	{
		m_data.clear();
	}

	/** 指定したインデックスにある要素を削除します。*/
	virtual void removeAt(int index)
	{
		m_data.erase(m_data.begin() + index);
	}

	/** 指定したインデックス位置にある要素を置き換えます。*/
	virtual void setItem(int index, const value_type& item)
	{
		m_data[index] = item;
	}

private:
	std::vector<Ref<T>> m_data;

	template<class T> friend
	Ref<ObjectCollection<T>> makeObjectCollection();
};

/** Ref<Collection> を構築します。 */
template<class TItem>
inline Ref<ObjectCollection<TItem>> makeObjectCollection()
{
	return makeObject<ObjectCollection<TItem>>();//Ref<ObjectCollection<TItem>>(LN_NEW ObjectCollection<TItem>(), false);
}

} // namespace ln
