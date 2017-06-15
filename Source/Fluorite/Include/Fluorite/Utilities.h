
#pragma once

namespace ln
{
namespace fl
{
namespace detail
{

template<class T>
class StackedCache
{
public:
	StackedCache()
		: m_cache()
		, m_indexStack()
	{
	}

	~StackedCache()
	{
	}

	void initialize(int reserved)
	{
		Glow(reserved);
	}

	int AllocIndex()
	{
		// ŠÇ—”z—ñ‚ª‚·‚×‚Ä–„‚Ü‚Á‚Ä‚¢‚éê‡‚Í—Ìˆæ‚ð‘‚â‚·
		if (m_indexStack.isEmpty())
		{
			Glow(m_cache.getCount());
		}

		// ‹ó‚«êŠ‚ðŽæ“¾
		int newIndex = m_indexStack.getTop();
		m_indexStack.pop();
		return newIndex;
	}

	void FreeIndex(int index)
	{
		m_indexStack.push(index);
	}

	T& GetObject(int index)
	{
		return m_cache[index];
	}

private:
	void Glow(int count)
	{
		int last = m_cache.getCount();
		for (int i = count - 1; i >= 0; --i)
		{
			m_indexStack.push(last + i);
		}
		m_indexStack.resize(last + count);
	}
	
	Array<T>	m_cache;
	Stack<int>	m_indexStack;
};

} // namespace detail
} // namespace fl
} // namespace ln
