
template<LN_EVENT_TEMPLATE_ARGS>
class LN_EVENT_CLASS_NAME
{
public:
	typedef LN_EVENT_DELEGATE_CLASS_NAME<LN_EVENT_TEMPLATE_TYPES> DelegateType;

public:
	void AddHandler(const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void RemoveHandler(const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void Clear()
	{
		m_handlerList.Clear();
	}

	void Raise(LN_EVENT_ARGS_DECL)
	{
		LN_FOREACH(DelegateType& d, m_handlerList)
		{
			d.Call(LN_EVENT_CALL_ARGS);
		}
	}

	bool IsEmpty() const
	{
		return m_handlerList.IsEmpty();
	}

	void operator += (const DelegateType& handler)
	{
		AddHandler(handler);
	}

	void operator -= (const DelegateType& handler)
	{
		RemoveHandler(handler);
	}

	void operator () (LN_EVENT_ARGS_DECL)
	{
		Raise(LN_EVENT_CALL_ARGS);
	}

private:
	List<DelegateType> m_handlerList;
};

#undef LN_EVENT_CLASS_NAME
#undef LN_EVENT_DELEGATE_CLASS_NAME
#undef LN_EVENT_TEMPLATE_ARGS
#undef LN_EVENT_ARGS_DECL
#undef LN_EVENT_CALL_ARGS
#undef LN_EVENT_TEMPLATE_TYPES
