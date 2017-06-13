
template<LN_EVENT_TEMPLATE_ARGS>
class LN_EVENT_CLASS_NAME
{
public:
	typedef LN_EVENT_DELEGATE_CLASS_NAME<LN_EVENT_TEMPLATE_TYPES> DelegateType;

public:
	void addHandler(const DelegateType& handler)
	{
		m_handlerList.add(handler);
	}

	void removeHandler(const DelegateType& handler)
	{
		m_handlerList.remove(handler);
	}

	void clear()
	{
		m_handlerList.clear();
	}

	void raise(LN_EVENT_ARGS_DECL)
	{
		LN_FOREACH(DelegateType& d, m_handlerList)
		{
			d.call(LN_EVENT_CALL_ARGS);
		}
	}

	bool isEmpty() const
	{
		return m_handlerList.isEmpty();
	}

	void operator += (const DelegateType& handler)
	{
		addHandler(handler);
	}

	void operator -= (const DelegateType& handler)
	{
		removeHandler(handler);
	}

	void operator () (LN_EVENT_ARGS_DECL)
	{
		raise(LN_EVENT_CALL_ARGS);
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
