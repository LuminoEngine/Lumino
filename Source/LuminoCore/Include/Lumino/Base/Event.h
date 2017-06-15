
#pragma once
#include <memory>
#include "../Base/List.h"
#include "../Base/Delegate.h"


LN_NAMESPACE_BEGIN

namespace detail {

struct EventConnectionDataBase
{
	bool	m_active;
};

struct EventInternalDataBase
{
	virtual void onDisconnect(EventConnectionDataBase* conn) = 0;
};

} // namespace detail




/**
	@brief	イベントハンドラの状態を追跡します。必要に応じて、イベントから切断するために使用します。
*/
class EventConnection
{
public:
	EventConnection()
		: m_internalData(nullptr)
		, m_connectionData(nullptr)
	{}

	/** イベントハンドラとイベントを切断します。*/
	void Disconnect()
	{
		if (m_internalData != nullptr)
		{
			m_internalData->onDisconnect(m_connectionData);
		}
	}

private:
	EventConnection(const std::shared_ptr<detail::EventInternalDataBase>& internalData, detail::EventConnectionDataBase* connectionData)
		: m_internalData(internalData)
		, m_connectionData(connectionData)
	{
		m_connectionData->m_active = true;
	}

	std::shared_ptr<detail::EventInternalDataBase>	m_internalData;
	detail::EventConnectionDataBase*				m_connectionData;

	template<typename>
	friend class Event;
};



/**
	@brief	イベントがトリガーされたときに通知を受け取るメソッドコールバックを登録するためのクラスです。
*/
template<typename>
class Event {};
template<typename TRet, typename... TArgs>
class Event<TRet(TArgs...)>
{
public:
	typedef Delegate<TRet(TArgs...)> DelegateType;

	Event()
		: m_internalData(std::make_shared<EventInternalData>())
	{}

	~Event()
	{
		clear();
	}

	EventConnection connect(const DelegateType& handler)
	{
		return connectInternal(handler);
	}

	//EventConnection Connect(const std::function<TRet(TArgs...)>& handler)
	//{
	//	return ConnectInternal(handler);
	//}

	void clear() LN_NOEXCEPT
	{
		m_internalData->connectionDataList.clear();
	}

	bool isEmpty() const
	{
		return m_internalData->connectionDataList.isEmpty();
	}

	void raise(TArgs... args)
	{
		if (!isEmpty())
		{
			for (auto& data : m_internalData->connectionDataList)
			{
				if (data->m_active) data->handler.call(args...);
			}
		}
	}


	EventConnection operator += (const DelegateType& handler)
	{
		return connectInternal(handler);
	}

	EventConnection operator += (const std::function<TRet(TArgs...)>& handler)
	{
		return connectInternal(handler);
	}

	void operator () (TArgs... args)
	{
		raise(args...);
	}

private:
	struct EventConnectionData : public detail::EventConnectionDataBase
	{
		DelegateType	handler;
	};

	struct EventInternalData : public detail::EventInternalDataBase
	{
		List<std::shared_ptr<EventConnectionData>>	connectionDataList;

		virtual void onDisconnect(detail::EventConnectionDataBase* conn)
		{
			// check memory not released
			bool released = true;
			for (auto& data : connectionDataList)
			{
				if (data.get() == conn) released = false;
			}
			if (!released)
			{
				conn->m_active = false;
			}
		}
	};

	EventConnection connectInternal(const DelegateType& handler)
	{
		auto connectionData = requestFreeConnectionData();
		connectionData->handler = handler;
		return EventConnection(m_internalData, connectionData.get());
	}

	std::shared_ptr<EventConnectionData> requestFreeConnectionData()
	{
		for (auto& data : m_internalData->connectionDataList)
		{
			if (!data->m_active) return data;
		}

		auto newData = std::make_shared<EventConnectionData>();
		m_internalData->connectionDataList.add(newData);
		return newData;
	}

	std::shared_ptr<EventInternalData>	m_internalData;
};



LN_NAMESPACE_END
