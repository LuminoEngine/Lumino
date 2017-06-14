
#pragma once
#include <Lumino/Base/SortedArray.h>
#include <Lumino/UI/Common.h>
#include <Lumino/UI/UIEventArgs.h>

LN_NAMESPACE_BEGIN
namespace detail
{

	
class RefrectionObjectPool
{
public:
	RefrectionObjectPool() = default;
	~RefrectionObjectPool()
	{
		for (auto* obj : m_poolList)
		{
			obj->release();
		}
	}

	template<class TObject, typename ...TArgs>
	TObject* create(TArgs... args)
	{
		TObject* obj = static_cast<TObject* >(findFreeObject(tr::TypeInfo::getTypeInfo<TObject>()));
		if (obj == nullptr) {
			obj = LN_NEW TObject(args...);
			registerObject(obj);
		}
		else {
			obj->~TObject();
			new (obj)TObject(args...);
		}
		obj->handled = false;
		obj->addRef();
		return obj;
	}

private:
	typedef List<tr::ReflectionObject*>	ObjectList;
	ObjectList					m_poolList;

	tr::ReflectionObject* findFreeObject(tr::TypeInfo* typeId)
	{
		for (auto* e : m_poolList)
		{
			if (e->getReferenceCount() == 1) {	// このリストからしか参照されていなければ返す
				return e;
			}
		}
		return nullptr;
	}

	void registerObject(tr::ReflectionObject* obj)
	{
		m_poolList.add(obj);
	}
};

/**
	@brief	
	@note	このクラスはイベント引数の頻繁な new を避けるために使用する。
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
{
public:
	EventArgsPool() = default;
	~EventArgsPool()
	{
		for (auto list : m_pool)
		{
			for (auto e : (*list.second))
			{
				e->release();
			}
			delete list.second;
		}
	}

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(find(tr::TypeInfo::getTypeInfo<TEventArgs>()));
		if (e == nullptr) {
			auto eRef = newObject<TEventArgs>(args...);
			registerObject(eRef);
			e = eRef;
		}
		else {
			static_cast<UIEventArgs*>(e)->~UIEventArgs();
			placementNewObject<TEventArgs>(e, args...);
		}
		e->handled = false;
		e->addRef();
		return e;
	}

private:
	typedef List<UIEventArgs*>							EventArgsList;
	typedef SortedArray<tr::TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	UIEventArgs* find(tr::TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.tryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->getReferenceCount() == 1) {	// このリストからしか参照されていなければ返す
					return e;
				}
			}
		}
		return NULL;
	}

	void registerObject(UIEventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.tryGetValue(tr::TypeInfo::getTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.add(tr::TypeInfo::getTypeInfo(e), list);
			e->addRef();
		}
		list->add(e);
	}


	//List<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T findFreeObject(const List<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->getReferenceCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	//List<MouseEventArgs*>	m_mouseEventArgsPool;
	//List<KeyEventArgs*>	m_keyEventArgsPool;
};

} // namespace detail
LN_NAMESPACE_END
