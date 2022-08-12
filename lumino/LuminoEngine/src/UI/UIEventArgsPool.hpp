
#pragma once
#include <unordered_map>

namespace ln {
class UIEventArgs;
namespace detail {

/*
    このクラスはイベント引数の頻繁な new を避けるために使用する。
	例えば MouseMove イベントは最悪毎フレームのように発生する可能性があり、new は大きなオーバーヘッドになる。
	それなら union を利用したイベント引数構造体を使うのもひとつの手かもしれない。
			
	しかし、本ライブラリは C# や Ruby 等、他の言語へのバインディングを想定している。
	当然 GUI モジュールも公開され、拡張されることが前提。
	C# 側で作成したユーザーコントロールが MouseEventArgs を使用することは想定しなければならない。

	union や struct にした場合、これはこれで言語別に余計なオーバーヘッドや合わせこみの実装が増えることになる。
	例えば C# でBは値渡しのままでよいのか、ref をつけるのか。Ruby で struct は使えないので結局 new する羽目になるがいいのか。

	Pool しておけば、若干直感的ではなくなるが、バインダ側の new も抑えることができる。

	後々イベントの種類が増えてきたときは拡張性のため、イベント名をキーにして Create するような仕組みが必要になるかもしれない。
*/
class EventArgsPool
    : public RefObject
{
public:
    using EventArgsTypeId = TypeInfo*;

	EventArgsPool() = default;
	~EventArgsPool()
	{
		for (auto& list : m_pool)
		{
			for (auto& e : (*list.second))
			{
                RefObjectHelper::release(e);
			}
			delete list.second;
		}
	}

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* create(TArgs... args)
	{
        EventArgsTypeId typeId = TypeInfo::getTypeInfo<TEventArgs>();
		TEventArgs* e = static_cast<TEventArgs* >(find(typeId));
		if (e == nullptr)
		{
            auto obj = makeObject_deprecated<TEventArgs>(args...);
            e = obj;
            RefObjectHelper::retain(e);
			registerObject(typeId, obj);
		}
		else
		{
            ObjectHelper::destructObject(e);
            placementNewObject<TEventArgs>(e, args...);
		}
		e->handled = false;
        RefObjectHelper::retain(e);
		return e;
	}

private:
	typedef List<UIEventArgs*>					EventArgsList;
	//typedef SortedArray<tr::TypeInfo*, EventArgsList*>	EventArgsMap;
	std::unordered_map<EventArgsTypeId, EventArgsList*>	m_pool;

    UIEventArgs* find(EventArgsTypeId typeId)
	{
        auto itr = m_pool.find(typeId);
        if (itr != m_pool.end())
        {
            for (auto e : *(itr->second))
            {
                if (RefObjectHelper::getReferenceCount(e) == 1) {	// このリストからしか参照されていなければ返す
                    return e;
                }
            }
        }
        return nullptr;
	}

	void registerObject(EventArgsTypeId typeId, UIEventArgs* e)
	{
        auto itr = m_pool.find(typeId);
        if (itr != m_pool.end())
        {
            itr->second->add(e);
        }
        else
        {
            EventArgsList* list = LN_NEW EventArgsList();
            list->add(e);
            m_pool.insert({ typeId, list });
        }
	}

	//template<class T>
	//T findFreeObject(const List<T>& pool)
	//{
	//	for (T a : pool)
	//	{
	//		if (a->getReferenceCount() == 1) {
	//			return a;
	//		}
	//	}
	//	return NULL;
	//}
};

} // namespace detail
} // namespace ln
