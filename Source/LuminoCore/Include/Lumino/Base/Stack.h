
#pragma once
#include <vector>
#include "RefObject.h"
#include "StlHelper.h"

LN_NAMESPACE_BEGIN

/**
	@brief	参照カウントを持つスタックのクラスです。
*/
template<typename T, typename TAllocator = detail::StlAllocator<T> >
class Stack : public RefObject
{
//public:
	//typedef typename std::stack<T/*, TAllocator*/>	std_stack;

public:
	Stack() {}
	virtual ~Stack() {}

public:

	/// スタックが空であるかを判定します。
	bool IsEmpty() const { return m_stack.empty(); }

	/// 保持できる要素数を設定します。
	void Reserve(int size) { m_stack.reserve(size); }

	/// 全ての要素を削除します。
	void Clear() { m_stack.clear(); }

	/// 格納されている要素数を取得します。
	int GetCount() const { return (int)m_stack.size(); }

	/// スタックの先頭に要素を追加します。
	void Push(const T& value) { m_stack.push_back(value); }

	/// スタックの先頭の要素を削除します。
	void Pop(T* outTop = NULL) { if (outTop) *outTop = GetTop(); m_stack.pop_back(); }

	/// スタックの先頭の要素を取得します。
	T& GetTop() { return *(m_stack.rbegin()); }
	const T& GetTop() const { return *(m_stack.rbegin()); }

private:
	std::vector<T, TAllocator>	m_stack;
};

LN_NAMESPACE_END
