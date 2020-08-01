// Copyright (c) 2018+ lriki. Distributed under the MIT license.

#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../Engine/Object.hpp"
#include "Delegate.hpp"

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
LN_CLASS()
class EventConnection
	: public Object
{
	LN_OBJECT;
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
class Event< Delegate<TReturn(TArgs...)> >
{
public:
    using DelegateType = Ref<Delegate<TReturn(TArgs...)>>;

    Event()
        : m_internalData()
    {
    }

    ~Event()
    {
        clear();
    }

    /** このイベントからの通知を受けるコールバックを登録します。 */
    void setPrimaryHandler(const DelegateType& handler)
    {
        m_primaryHandler = handler;
    }

    /** このイベントからの通知を受けるコールバックを登録します。 */
	Ref<EventConnection> connect(const DelegateType& handler)
    {
        return connectInternal(handler);
    }

    /** このイベントに登録されているコールバックをすべてクリアします。 */
    void clear() LN_NOEXCEPT
    {
        if (m_internalData) {
            m_internalData->connectionDataList.clear();
        }
    }

    /** このイベントに登録されているコールバックがあるかを確認します。 */
    bool isEmpty() const
    {
        if (m_primaryHandler) return false;
        if (m_internalData)
            return m_internalData->connectionDataList.empty();
        else
            return true;
    }

    /** 登録されているすべてのコールバックへイベントを通知します。 */
    void raise(TArgs... args)
    {
        if (m_primaryHandler) {
            m_primaryHandler->call(std::forward<TArgs>(args)...);
        }

        if (m_internalData) {
            for (auto& data : m_internalData->connectionDataList) {
                if (data->m_active)
                    data->handler->call(std::forward<TArgs>(args)...);
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
        if (!m_internalData) {
            m_internalData = std::make_shared<EventInternalData>();
        }

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

    DelegateType m_primaryHandler;
    std::shared_ptr<EventInternalData> m_internalData;

    // Note: ほとんどのケースではひとつのイベントに対してひとつのハンドラを登録して使うことになる。
    // この Event クラスは複数のハンドラを登録でき、それぞれ登録解除したいときは EventConnection を利用するが、
    // EventConnection を使うことはほぼ無かった。
    // それよりも通常の get/set のように単一登録でもかまわないので単純な設定ができるようにしたい要求が出てきた。(null セットで解除など)
    // m_primaryHandler はこのような単純な設定をしたいときに使う。
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
