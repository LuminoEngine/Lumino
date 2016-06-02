
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
			obj->Release();
		}
	}

	template<class TObject, typename ...TArgs>
	TObject* Create(TArgs... args)
	{
		TObject* obj = static_cast<TObject* >(FindFreeObject(tr::TypeInfo::GetTypeInfo<TObject>()));
		if (obj == nullptr) {
			obj = LN_NEW TObject(args...);
			Register(obj);
		}
		else {
			obj->~TObject();
			new (obj)TObject(args...);
		}
		obj->handled = false;
		obj->AddRef();
		return obj;
	}

private:
	typedef Array<tr::ReflectionObject*>	ObjectList;
	ObjectList					m_poolList;

	tr::ReflectionObject* FindFreeObject(tr::TypeInfo* typeId)
	{
		for (auto* e : m_poolList)
		{
			if (e->GetReferenceCount() == 1) {	// このリストからしか参照されていなければ返す
				return e;
			}
		}
		return nullptr;
	}

	void Register(tr::ReflectionObject* obj)
	{
		m_poolList.Add(obj);
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
				e->Release();
			}
			delete list.second;
		}
	}

	//MouseEventArgs* CreateMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount);	// TODO: やめる
	//KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);	// TODO: やめる

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* Create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(Find(tr::TypeInfo::GetTypeInfo<TEventArgs>()));
		if (e == NULL) {
			e = LN_NEW TEventArgs(args...);
			Register(e);
		}
		else {
			e->~TEventArgs();
			new (e)TEventArgs(args...);
		}
		e->handled = false;
		e->AddRef();
		return e;
	}

private:
	typedef Array<UIEventArgs*>							EventArgsList;
	typedef SortedArray<tr::TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	UIEventArgs* Find(tr::TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.TryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->GetReferenceCount() == 1) {	// このリストからしか参照されていなければ返す
					return e;
				}
			}
		}
		return NULL;
	}

	void Register(UIEventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.TryGetValue(tr::TypeInfo::GetTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.Add(tr::TypeInfo::GetTypeInfo(e), list);
		}
		list->Add(e);
	}


	//Array<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T FindFreeObject(const Array<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetReferenceCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	//Array<MouseEventArgs*>	m_mouseEventArgsPool;
	//Array<KeyEventArgs*>	m_keyEventArgsPool;
};

} // namespace detail
LN_NAMESPACE_END
