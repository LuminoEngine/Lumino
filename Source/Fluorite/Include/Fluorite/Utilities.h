
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

	void Initialize(int reserved)
	{
		Glow(reserved);
	}

	int AllocIndex()
	{
		// ŠÇ—”z—ñ‚ª‚·‚×‚Ä–„‚Ü‚Á‚Ä‚¢‚éê‡‚Í—Ìˆæ‚ð‘‚â‚·
		if (m_indexStack.IsEmpty())
		{
			Glow(m_cache.GetCount());
		}

		// ‹ó‚«êŠ‚ðŽæ“¾
		int newIndex = m_indexStack.GetTop();
		m_indexStack.Pop();
		return newIndex;
	}

	void FreeIndex(int index)
	{
		m_indexStack.Push(index);
	}

	T& GetObject(int index)
	{
		return m_cache[index];
	}

private:
	void Glow(int count)
	{
		int last = m_cache.GetCount();
		for (int i = count - 1; i >= 0; --i)
		{
			m_indexStack.Push(last + i);
		}
		m_indexStack.Resize(last + count);
	}
	
	Array<T>	m_cache;
	Stack<int>	m_indexStack;
};

} // namespace detail
} // namespace fl
} // namespace ln
