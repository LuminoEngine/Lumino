// Copyright (c) 2018+ lriki. Distributed under the MIT license.

#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../Engine/Object.hpp"

namespace ln {
namespace detail {

struct EventConnectionDataBase
{
    bool m_active;
};

struct EventInternalDataBase
{
    virtual void onDisconnect(EventConnectionDataBase* conn) = 0;
};

} // namespace detail

/** イベントハンドラの状態を追跡します。必要に応じて、イベントから切断するために使用します。 */
class EventConnection
	: public Object
{
public:
    EventConnection()
        : m_internalData(nullptr)
        , m_connectionData(nullptr)
    {
    }

    /** イベントハンドラとイベントを切断します。*/
    void disconnect()
    {
        if (m_internalData != nullptr) {
            m_internalData->onDisconnect(m_connectionData);
        }
    }

	/** イベントハンドラとイベントが接続されているかを確認します。*/
	bool connected() const
	{
		return m_internalData != nullptr;
	}

private:
    EventConnection(const std::shared_ptr<detail::EventInternalDataBase>& internalData, detail::EventConnectionDataBase* connectionData)
        : m_internalData(internalData)
        , m_connectionData(connectionData)
    {
        m_connectionData->m_active = true;
    }

    std::shared_ptr<detail::EventInternalDataBase> m_internalData;
    detail::EventConnectionDataBase* m_connectionData;

    template<typename>
    friend class Event;
};

// base for specialization
template<typename>
class Event;

/** イベントがトリガーされたときに通知を受け取るメソッドコールバックを登録するためのクラスです。 */
template<class TReturn, class... TArgs>
class Event< std::function<TReturn(TArgs...)> >
{
public:
    using DelegateType = std::function<TReturn(TArgs...)>;

    Event()
        : m_internalData(std::make_shared<EventInternalData>())
    {
    }

    ~Event()
    {
        clear();
    }

    /** このイベントからの通知を受けるコールバックを登録します。 */
	Ref<EventConnection> connect(const DelegateType& handler)
    {
        return connectInternal(handler);
    }

    /** このイベントに登録されているコールバックをすべてクリアします。 */
    void clear() LN_NOEXCEPT
    {
        m_internalData->connectionDataList.clear();
    }

    /** このイベントに登録されているコールバックがあるかを確認します。 */
    bool isEmpty() const
    {
        return m_internalData->connectionDataList.empty();
    }

    /** 登録されているすべてのコールバックへイベントを通知します。 */
    void raise(TArgs... args)
    {
        if (!isEmpty()) {
            for (auto& data : m_internalData->connectionDataList) {
                if (data->m_active)
                    data->handler(std::forward<TArgs>(args)...);
            }
        }
    }

	/** このイベントからの通知を受けるコールバックを登録します。 */
	Ref<EventConnection> operator+=(const DelegateType& handler)
    {
        return connectInternal(handler);
    }

	/** 登録されているすべてのコールバックへイベントを通知します。 */
    void operator()(TArgs... args)
    {
        raise(std::forward<TArgs>(args)...);
    }

private:
    struct EventConnectionData : public detail::EventConnectionDataBase
    {
        DelegateType handler;
    };

    struct EventInternalData : public detail::EventInternalDataBase
    {
        std::vector<std::shared_ptr<EventConnectionData>> connectionDataList;

        virtual void onDisconnect(detail::EventConnectionDataBase* conn)
        {
            // check memory not released
            bool released = true;
            for (auto& data : connectionDataList) {
                if (data.get() == conn)
                    released = false;
            }
            if (!released) {
                conn->m_active = false;
            }
        }
    };

    Ref<EventConnection> connectInternal(const DelegateType& handler)
    {
        auto connectionData = requestFreeConnectionData();
        connectionData->handler = handler;

        return Ref<EventConnection>(LN_NEW EventConnection(m_internalData, connectionData.get()), false);
    }

    std::shared_ptr<EventConnectionData> requestFreeConnectionData()
    {
        for (auto& data : m_internalData->connectionDataList) {
            if (!data->m_active)
                return data;
        }

        auto newData = std::make_shared<EventConnectionData>();
        m_internalData->connectionDataList.push_back(newData);
        return newData;
    }

    std::shared_ptr<EventInternalData> m_internalData;
};

/** クラスメンバ関数を this で束縛するユーティリティです。 */
template<class TClass, typename TRet, typename... TArgs>
std::function<TRet(TArgs...)> bind(TClass* c, TRet (TClass::*m)(TArgs...))
{
    return [=](auto&&... args) { return (c->*m)(std::forward<decltype(args)>(args)...); };
}

/** @copydoc bind */
template<class TClass, typename TRet, typename... TArgs>
std::function<TRet(TArgs...)> bind(const TClass* c, TRet (TClass::*m)(TArgs...) const)
{
    return [=](auto&&... args) { return (c->*m)(std::forward<decltype(args)>(args)...); };
}

} // namespace ln
