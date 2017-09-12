
#pragma once
#include <vector>
#include "Exception.h"
#include "../Reflection/ReflectionObject.h"

LN_NAMESPACE_BEGIN
namespace detail { class AbstractListObject; };
namespace detail { template<typename T> class ListObject; };

/**
	@brief
*/
template<typename T>
class SharedList
{
public:
	typedef typename std::vector<T>			core;
	typedef typename core::value_type		value_type;
	typedef typename core::size_type		size_type;
	typedef typename core::difference_type	difference_type;
	typedef typename core::pointer			pointer;
	typedef typename core::const_pointer	const_pointer;
	typedef typename core::iterator			iterator;
	typedef typename core::const_iterator	const_iterator;
	typedef typename core::reference		reference;
	typedef typename core::const_reference	const_reference;

	SharedList()
		: m_core(nullptr)
	{}

	~SharedList()
	{
		if (m_core) m_core->release();
	}

	SharedList(const SharedList& list)
	{
		m_core = list.m_core;
		if (m_core) m_core->addRef();
	}

	SharedList(SharedList&& list) LN_NOEXCEPT
	{
		m_core = list.m_core;
		list.m_core = nullptr;
	}

	SharedList& operator=(const SharedList& list)
	{
		if (m_core != list.m_core)
		{
			release();
			m_core = list.m_core;
			if (m_core) m_core->addRef();
		}
		return *this;
	}

	// m_core->release() が noexcept ではないため実装できない
	// SharedList& operator=(SharedList&& list) LN_NOEXCEPT;

	/** 初期化子リストからリストを構築します。*/
	SharedList(std::initializer_list<T> list)
	{
		m_core = LN_NEW detail::ListObject<T>();
		m_core->assign(list.begin(), list.end());
	}

	/** 格納されている要素の数を取得します。*/
	int size() const
	{
		if (LN_REQUIRE(m_core)) return 0;
		return static_cast<int>(m_core->size());
	}

	/** 指定したインデックスにある要素への参照を取得します。*/
	T& operator[](size_type i)
	{
		LN_FATAL(m_core);
		return m_core->at(i);
	}

	/** 指定したインデックスにある要素への参照を取得します。*/
	const T& operator[](size_type i) const
	{
		LN_FATAL(m_core);
		return m_core->at(i);
	}

	T* data() { return (m_core) ? m_core->data() : nullptr; }
	const T* data() const { return (m_core) ? m_core->data() : nullptr; }

	iterator begin()
	{
		if (LN_REQUIRE(m_core)) return iterator();
		return m_core->begin();
	}

	const_iterator begin() const
	{
		if (LN_REQUIRE(m_core)) return const_iterator();
		return m_core->begin();
	}

	iterator end()
	{
		if (LN_REQUIRE(m_core)) return iterator();
		return m_core->end();
	}

	const_iterator end() const
	{
		if (LN_REQUIRE(m_core)) return const_iterator();
		return m_core->end();
	}

private:
	void release()
	{
		if (m_core)
		{
			m_core->release();
			m_core = nullptr;
		}
	}

	detail::ListObject<T>*	m_core;
};

//==============================================================================
// List
//==============================================================================
template<typename T> inline bool operator==(const SharedList<T>& lhs, std::nullptr_t) LN_NOEXCEPT { return lhs.data() == nullptr; }
template<typename T> inline bool operator==(std::nullptr_t, const SharedList<T>& rhs) LN_NOEXCEPT { return rhs.data() == nullptr; }
template<typename T> inline bool operator!=(const SharedList<T>& lhs, std::nullptr_t) LN_NOEXCEPT { return lhs.data() != nullptr; }
template<typename T> inline bool operator!=(std::nullptr_t, const SharedList<T>& rhs) LN_NOEXCEPT { return rhs.data() != nullptr; }

namespace detail {

class AbstractListObject
	: public Object
{};

template<typename T> class ListObject
	: public AbstractListObject
	, public std::vector<T>
{
};

} // namespace detail

LN_NAMESPACE_END
