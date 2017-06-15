
#pragma once
#include <vector>
#include <algorithm>
#include "Common.h"
#include "Exception.h"
#include "StlHelper.h"

LN_NAMESPACE_BEGIN

/**
	@brief		キーの "operator<" の実装により値を並べ替えられた、キーと値のペアの配列です。
	@details	このクラスの役割は std::map と似ていますが、より高速に検索及び要素のイテレートを行うことができます。
				ただし、新しいキーを格納するたびに配列全体がソートされるため、キーの更新には時間がかかります。
*/
template<typename TKey, typename TValue, typename TComparer = detail::SortedArrayBasicLess<TKey>, typename TAllocator = detail::StlAllocator< std::pair<TKey, TValue> > >
class SortedArray
{
public:
	typedef typename std::pair<TKey, TValue>		value_type;
	typedef typename std::pair<TKey, TValue>		Pair;
	typedef typename std::vector<Pair, TAllocator>	InternalArray;
	typedef typename InternalArray::iterator		iterator;
	typedef typename InternalArray::const_iterator	const_iterator;

public:

	/**
		@brief	格納されているキー/値ペアの数を取得します。
	*/
	int getCount() const
	{
		return (int)m_vector.size();
	}

	void clear()
	{
		m_vector.clear();
	}

	/** 
		@brief		指定したキーと値のペアを追加します。
		@param[in]	key		: 要素のキー
		@param[in]	value	: 要素の値
		@exception	ArgumentException	キーが既に追加されています。
	*/
	void add(const TKey& key, const TValue& value)
	{
		LN_THROW(!ContainsKey(key), ArgumentException);
		struct Cmp
		{
			static bool CmpEventListener(const Pair& left, const Pair& right)
			{
				return left.first < right.first;
			}
		};
		m_vector.push_back(Pair(key, value));
		std::sort(m_vector.begin(), m_vector.end(), Cmp::CmpEventListener);
	}

	bool remove(const TKey& key)
	{
		typename InternalArray::iterator itr = m_vector.begin();
		for (; itr != m_vector.end();)
		{
			if (itr->first == key) {
				m_vector.erase(itr);
				return true;
			}
		}
		return false;
	}

	/**
		@brief	指定した値と一致する値を持つ全ての要素を削除します。
	*/
	void RemoveAllValue(const TValue& item)
	{
		typename InternalArray::iterator itr = m_vector.begin();
		for (; itr != m_vector.end();)
		{
			if (itr->second == item) {
				itr = m_vector.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	/**
		@brief	指定したキーが格納されているかを確認します。
		@return	格納されている場合は true。それ以外の場合は false。
	*/
	bool ContainsKey(const TKey& key) const
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			return true;
		}
		return false;
	}

	/**
		@brief		指定したキーに値を設定します。
		@details	キーが存在しない場合、指定したキーと値を持つ新しい要素を作成します。
	*/
	void setValue(const TKey& key, const TValue& value)
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			m_vector[index].second = value;
		}
		else {
			add(key, value);
		}
	}

	/**
		@brief		指定したキーに関連付けられている値を取得します。
		@exception	KeyNotFoundException
	*/
	const TValue& getValue(const TKey& key) const
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			return m_vector[index].second;
		}
		LN_THROW(0, KeyNotFoundException);
		return m_vector.end()->second;
	}

	/**
		@brief		指定したキーに関連付けられている値を取得します。
		@return		キーが格納されている場合は true。それ以外の場合は false。
		@details	GetValue() と似た動作をしますが、検索の成否を例外ではなく戻り値で返します。
	*/
	bool tryGetValue(const TKey& key, TValue* value) const
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			if (value != NULL) { *value = m_vector[index].second; }
			return true;
		}
		return false;
	}

	/**
		@brief		指定したキーの値の参照を取得します。
		@details	キーが存在しない場合、指定したキーを持つ新しい要素を作成します。
	*/
	TValue& operator[](const TKey& key)
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/) {
			
		}
		else {
			add(key, TValue());
			index = LowerBound(key);
		}
		return m_vector[index].second;
	}

	/**
		@brief		指定したキーの値の参照を取得します。
		@exception	KeyNotFoundException
	*/
	const TValue& operator[](const TKey& key) const
	{
		return getValue(key);
	}

	template<typename TOtherKey>
	const TValue* find(const TOtherKey& key) const
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			return &m_vector[index].second;
		}
		return nullptr;
	}

	template<typename TOtherKey>
	TValue* find(const TOtherKey& key)
	{
		int index = LowerBound(key);
		if (index < getCount() && equals(m_vector[index].first, key)/*m_vector[index].first == key*/)
		{
			return &m_vector[index].second;
		}
		return nullptr;
	}

public:

	/*-----------------------------------------------------------------------*/
	/** @name STL interface */
	/** @{ */

	iterator		begin() { return m_vector.begin(); }
	iterator		end() { return m_vector.end(); }
	const_iterator	begin() const { return m_vector.begin(); }
	const_iterator	end() const { return m_vector.end(); }

	/** @} */

private:

	template<typename TOtherKey>
	bool equals(const TKey& left, const TOtherKey& right) const
	{
		return !m_comparer(left, right) && !m_comparer(right, left);
	}


	template<typename TOtherKey>
	int LowerBound(const TOtherKey/*TKey*/& key) const
	{
		if (m_vector.empty()) { return getCount(); }

		int count = (int)m_vector.size();
		int first = 0;

		const Pair* pairs = &m_vector[0];

		while (0 < count)
		{
			int count2 = count / 2;
			int mid = first + count2;

			if (m_comparer(pairs[mid].first, key)/*pairs[mid].first < key*/) {
				first = ++mid;
				count -= count2 + 1;
			}
			else {
				count = count2;
			}
		}
		return first;
	}

private:
	std::vector<Pair, TAllocator>	m_vector;
	TComparer						m_comparer;
};


LN_NAMESPACE_END
