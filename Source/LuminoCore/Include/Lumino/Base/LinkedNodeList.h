
#pragma once

LN_NAMESPACE_BEGIN

template<class TNode>
class LinkedNodeList;

class LinkedNode
{
protected:
	LinkedNode() = default;
	~LinkedNode() = default;

private:
	LinkedNode*	m_prev = nullptr;
	LinkedNode*	m_next = nullptr;

	template<class TNode>
	friend class LinkedNodeList;
};

// Node のインスタンス生成・破棄の管理は行わない
template<class TNode>
class LinkedNodeList
{
public:
	LinkedNodeList()
	{
		m_dummy = &m_dummyInstance;
		m_dummy->m_prev= m_dummy;
		m_dummy->m_next = m_dummy;
		m_size = 0;
	}

	~LinkedNodeList()
	{
		Clear();
	}

	int GetSize() const { return m_size; }

	void Clear()
	{
		LinkedNode* n = m_dummy->m_next;
		LinkedNode* t;
		while (n != m_dummy)
		{
			t = n->m_next;
			n->m_prev = nullptr;
			n->m_next = nullptr;
			n = t;
		}
		m_dummy->m_prev= m_dummy;
		m_dummy->m_next = m_dummy;
		m_size = 0;
	}

	void Add(TNode* node)
	{
		if (node == nullptr) return;
		LN_FAIL_CHECK_ARG(node->m_prev == nullptr && node->m_next == nullptr) return;	// already added

		LinkedNode* prev = m_dummy->m_prev;
		LinkedNode* next = m_dummy;
		node->m_prev = prev;
		node->m_next = next;
		prev->m_next = node;
		next->m_prev = node;
		m_size++;
	}

	void Remove(TNode* node)
	{
		if (node == nullptr) return;
		if (node->m_prev == nullptr || node->m_next == nullptr) return;	// already removed

		node->m_prev->m_next = node->m_next;
		node->m_next->m_prev = node->m_prev;
		node->m_prev = nullptr;
		node->m_next = nullptr;
		m_size--;
	}

	TNode* PopFront()
	{
		LinkedNode* node = m_dummy->m_next;
		if (node == m_dummy) return nullptr;	// empty

		node->m_prev->m_next = node->m_next;
		node->m_next->m_prev = node->m_prev;
		node->m_prev = nullptr;
		node->m_next = nullptr;
		m_size--;

		return static_cast<TNode*>(node);
	}

private:
	int			m_size;
	LinkedNode*	m_dummy;		// "front prev" or "last next"
	LinkedNode	m_dummyInstance;
};

LN_NAMESPACE_END
