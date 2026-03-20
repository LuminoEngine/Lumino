
namespace ln {

class ConditionEvent::Impl
{
public:
	Impl(bool locked)
		: m_handle(NULL)
	{
		BOOL t = (locked) ? FALSE : TRUE;	// TRUE:signaled, FALSE:un signaled
		m_handle = CreateEvent(NULL, TRUE, t, NULL);
	}

	~Impl()
	{
		if (m_handle)
		{
			::CloseHandle(m_handle);
			m_handle = NULL;
		}
	}

	void lock()
	{
		if (m_handle)
		{
			::ResetEvent(m_handle);
		}
	}

	void unlock()
	{
		if (m_handle)
		{
			::SetEvent(m_handle);
		}
	}

	bool isLocked() const
	{
		if (m_handle)
		{
			return (::WaitForSingleObject(m_handle, 0) == WAIT_OBJECT_0);
		}
		return true;
	}

	void wait()
	{
		if (m_handle)
		{
			// Returns control when "signaled".
			::WaitForSingleObject(m_handle, INFINITE);
		}
	}

private:
	HANDLE m_handle;
};

} // namespace ln
