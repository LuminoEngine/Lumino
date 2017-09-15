
#pragma once
#include <memory>
#include <iterator>
#include <list>
#include "../Base/Nullable.h"

LN_NAMESPACE_BEGIN
namespace tr {

namespace detail {

template<typename Itr>
using IteratorType = typename std::iterator_traits<Itr>::value_type;

template<typename T>
struct StreamProvider
{
public:
	struct Iterator;

	virtual ~StreamProvider() = default;

	virtual Nullable<T> get() = 0;

	bool Advance() { return AdvanceImpl(); }

	Iterator begin();
	Iterator end();

protected:
	virtual bool AdvanceImpl() = 0;
};



template<typename T>
using StreamProviderPtr = std::shared_ptr<StreamProvider<T>>;


// 外部に公開するイテレータ
template<typename T>
struct StreamProvider<T>::Iterator
	: public std::iterator<std::input_iterator_tag, T>
{
public:
	T& operator* ();
	Iterator& operator++ ();
	Iterator operator++ (int);
	bool operator== (Iterator& other);
	bool operator!= (Iterator& other);

private:
	enum class State
	{
		Initial,
		Consumed,	// 後置++ で返したイテレータを示す。このイテレータを操作することはできない
		Iterating,
		End,		// イテレート終了。あるいは end() で取得したイテレータ
	};

	Iterator(StreamProvider<T>* source, State state, Nullable<T> value);
	void UpdateInitial();
	void CheckConsumed() const;

	static Iterator MakeBegin(StreamProvider<T>* source);	// 先頭を示す Iterator を作成する
	static Iterator MakeEnd(StreamProvider<T>* source);		// 終端を示す Iterator を作成する

	StreamProvider<T>*	m_source;
	State				m_state;
	Nullable<T>			m_current;

	friend struct StreamProvider<T>;
};

//------------------------------------------------------------------------------
template<typename T>
StreamProvider<T>::Iterator::Iterator(StreamProvider<T>* source, State state, Nullable<T> value)
	: m_source(source)
	, m_state(state)
	, m_current(value)
{
}

//------------------------------------------------------------------------------
template<typename T>
T& StreamProvider<T>::Iterator::operator* ()
{
	UpdateInitial();
	return m_current.get();
}

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator& StreamProvider<T>::Iterator::operator++()
{
	UpdateInitial();
	CheckConsumed();
	if (m_source->Advance())
	{
		m_current = m_source->get();
	}
	else
	{
		m_state = State::End;
		m_current.reset();
	}
	return *this;
}

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator StreamProvider<T>::Iterator::operator++(int)
{
	UpdateInitial();
	CheckConsumed();
	Iterator result(nullptr, State::Consumed, m_current);
	if (m_source->Advance())
	{
		m_current = m_source->get();
	}
	else
	{
		m_state = State::End;
		m_current.reset();
	}
	return result;
}

//------------------------------------------------------------------------------
template<typename T>
bool StreamProvider<T>::Iterator::operator== (Iterator& other)
{
	UpdateInitial();
	CheckConsumed();
	other.UpdateInitial();
	other.CheckConsumed();
	if (m_state == State::End)
	{
		return other.m_state == State::End;
	}
	if (other.m_state == State::End)
	{
		return false;
	}
	return m_source == other.m_source && m_current == other.m_current;
}

//------------------------------------------------------------------------------
template<typename T>
bool StreamProvider<T>::Iterator::operator!= (Iterator& other)
{
	UpdateInitial();
	CheckConsumed();
	other.UpdateInitial();
	other.CheckConsumed();
	if (m_state == State::End)
	{
		return other.m_state != State::End;
	}
	if (other.m_state == State::End)
	{
		return true;
	}
	return m_source != other.m_source || m_current != other.m_current;
}

//------------------------------------------------------------------------------
template<typename T>
void StreamProvider<T>::Iterator::UpdateInitial()
{
	if (m_state == State::Initial)
	{
		if (m_source->Advance())
		{
			m_current = m_source->get();
			m_state = State::Iterating;
		}
		else
		{
			m_current.reset();
			m_state = State::End;
		}
	}
}

//------------------------------------------------------------------------------
template<typename T>
void StreamProvider<T>::Iterator::CheckConsumed() const
{
	if (LN_REQUIRE(m_state != State::Consumed)) return;
}

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator
StreamProvider<T>::Iterator::MakeBegin(StreamProvider<T>* source)
{
	return Iterator(source, State::Initial, nullptr);
}

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator
StreamProvider<T>::Iterator::MakeEnd(StreamProvider<T>* source)
{
	return Iterator(source, State::End, nullptr);
}


//==============================================================================
// StreamProvider
//==============================================================================

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator StreamProvider<T>::begin()
{
	return Iterator::MakeBegin(this);
}

//------------------------------------------------------------------------------
template<typename T>
typename StreamProvider<T>::Iterator StreamProvider<T>::end()
{
	return Iterator::MakeEnd(this);
}


//==============================================================================
// 入力されたイテレータをそのまま実行する
template<typename T, typename Itr>
class IteratorProvider
	: public StreamProvider<T>
{
public:
	IteratorProvider(Itr begin, Itr end)
		: m_first(true)
		, m_current(begin)
		, m_end(end)
	{}

	virtual Nullable<T> get() override
	{
		return *m_current;
	}

	bool AdvanceImpl() override
	{
		if (m_first)
		{
			m_first = false;
			return m_current != m_end;
		}
		++m_current;
		return m_current != m_end;
	}

private:
	bool	m_first = true;
	Itr		m_current;
	Itr		m_end;
};

//==============================================================================
// 関数オブジェクトでフィルタリングする
template<typename T, typename TPredicate>
class FilterProvider : public StreamProvider<T>
{
public:
	FilterProvider(const StreamProviderPtr<T>& source, TPredicate&& predicate)
		: m_source(source)
		, m_predicate(std::forward<TPredicate>(predicate))
	{}

	Nullable<T> get() override
	{
		return m_current;
	}

	bool AdvanceImpl() override
	{
		while (m_source->Advance())
		{
			m_current = m_source->get();
			if (m_predicate(m_current))
			{
				return true;
			}
		}
		m_current.reset();
		return false;
	}

private:
	StreamProviderPtr<T>	m_source;
	TPredicate				m_predicate;
	Nullable<T>				m_current;
};

//==============================================================================
// 2つの Provider を結合する
template<typename T>
class JoinProvider
	: public StreamProvider<T>
{
public:
	template<typename Iterator>
	JoinProvider(Iterator begin, Iterator end)
		: m_sources(begin, end)
	{}

	JoinProvider(const StreamProviderPtr<T>& first, const StreamProviderPtr<T>& second)
	{
		m_sources.push_back(first);
		m_sources.push_back(second);
	}

	Nullable<T> get() override
	{
		return m_current;
	}

	bool AdvanceImpl() override
	{
		while (!m_sources.empty())
		{
			auto& provider = m_sources.front();
			if (provider->Advance())
			{
				m_current = provider->get();
				return true;
			}
			m_sources.pop_front();
		}
		m_current.reset();
		return false;
	}

private:
	std::list<StreamProviderPtr<T>>	m_sources;
	Nullable<T>						m_current;
};

//==============================================================================
// 値を変換する
template<typename T, typename TTransform, typename TIn>
class MapProvider
	: public StreamProvider<T>
{
public:
	MapProvider(StreamProviderPtr<TIn> source, TTransform transform)
		: m_source(std::move(source))
		, m_transform(transform)
	{}

	Nullable<T> get() override
	{
		return m_current;
	}

	bool AdvanceImpl() override
	{
		if (m_source->Advance())
		{
			m_current = Nullable<T>(m_transform(m_source->get()));
			return true;
		}
		m_current.reset();
		return false;
	}

private:
	StreamProviderPtr<TIn>	m_source;
	TTransform				m_transform;
	Nullable<T>				m_current;
};

} // namespace detail

/**
	@brief		
*/
template<typename T>
class Enumerator
{
public:
	using iterator = typename detail::StreamProvider<T>::Iterator;
	
	iterator begin() { return m_source->begin(); }

	iterator end() { return m_source->end(); }

	template<typename Iterator>
	Enumerator(Iterator begin, Iterator end)
		: m_source(detail::StreamProviderPtr<T>(LN_NEW detail::IteratorProvider<T, Iterator>(begin, end)))
	{}

	
	//template<typename TPredicate>
	//Enumerator<T> Where(TPredicate pred)
	//{
	//	return Enumerator<T>(new Filter(m_source, pred));
	//}

	template<typename TPredicate>
	Enumerator<T>& Where(TPredicate&& pred)
	{
		m_source.reset(LN_NEW detail::FilterProvider<T, TPredicate>(m_source, std::forward<TPredicate>(pred)));
		return *this;
	}

	Enumerator<T>& Join(Enumerator<T>&& tail)
	{
		m_source.reset(LN_NEW detail::JoinProvider<T>(m_source, tail.m_source));
		return *this;
	}

	template<typename TContainer>
	Enumerator<T>& Join(TContainer&& container)
	{
		Enumerator<T> othor(container.begin(), container.end());
		m_source.reset(LN_NEW detail::JoinProvider<T>(m_source, othor.m_source));
		return *this;
	}

	template<typename TTransform, typename TResult = std::result_of<TTransform(T&&)>>
	Enumerator<TResult> Select(TTransform transform)
	{
		Enumerator<TResult> e(std::make_shared<detail::MapProvider<TResult, TTransform, T>>(m_source, transform));
		//e.m_source = (LN_NEW Map<TResult, TTransform, T>(m_source, transform));
		return e;
	}

	List<T> ToList()
	{
		return List<T>(begin(), end());
	}

	Enumerator(Enumerator<T>&& other)	// vs2013 default にできない
		: m_source(std::move(other.m_source))
	{}

	Enumerator(const Enumerator<T>& other) = default;

	Enumerator<T>& operator= (const Enumerator<T>& other) = default;


	Enumerator()
	{}


	Enumerator(detail::StreamProviderPtr<T> source)
		: m_source(std::move(source))
	{}
	
private:

	detail::StreamProviderPtr<T> m_source;
};

/**
	@brief		
*/
class MakeEnumerator
{
public:

	template<typename Iterator>
	static Enumerator<typename Iterator::value_type> from(Iterator begin, Iterator end)
	{
		return Enumerator<typename Iterator::value_type>(begin, end);
	}

	template<typename TContainer>
	static Enumerator<typename TContainer::value_type> from(const TContainer& container)
	{
		return Enumerator<typename TContainer::value_type>(container.begin(), container.end());
	}
};

} // namespace tr
LN_NAMESPACE_END
