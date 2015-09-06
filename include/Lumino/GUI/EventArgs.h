
#pragma once
#include "../Common.h"
#include "../Variant.h"
#include "../RoutedEvent.h"

namespace Lumino
{
namespace GUI
{
class UIElement;

/// 内部イベントの種類
enum EventType
{
	EventType_Unknown = 0,
	EventType_MouseMove,
	EventType_MouseButtonDown,
	EventType_MouseButtonUp,
	EventType_MouseWheel,

	EventType_KeyDown,
	EventType_KeyUp,
	EventType_Char,
	EventType_ElapsedTime,


	EventType_MouseEnter,
	EventType_MouseLeave,
};



/**
	@brief	マウスイベントの引数を表します。
*/
class MouseEventArgs
	: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

public:
	MouseButton	Button;		///< ボタン番号
	int			Wheel;		///< マウスホイールの回転回数 (正または負の回数)
	float		X;			///< マウスイベント生成時のマウスの X 座標 (グローバル座標。クライアント領域外は -1)
	float		Y;			///< マウスイベント生成時のマウスの Y 座標 (グローバル座標。クライアント領域外は -1)
	//short		MoveX;	    ///< X 座標の移動量
	//short		MoveY;      ///< Y 座標の移動量
	int			ClickCount;	///< ボタンがクリックされた回数。ダブルクリックやトリプルクリックを区別するために使用する。最大3まで。
};

/**
	@brief	キーボードイベントの引数を表します。	
*/
class KeyEventArgs
	: public RoutedEventArgs
{
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

	char GetChar() const;

public:
	Key			KeyCode;	///< キーコード
	bool		IsAlt;		///< Alt キーが押されている場合 true
	bool		IsShift;	///< Shift キーが押されている場合 true
	bool		IsControl;	///< Ctrl キーが押されている場合 true
	TCHAR		Char;		///< 
};

///**
//	@brief	スクロールイベントの引数を表します。
//*/
//class ScrollEventArgs
//	: public RoutedEventArgs
//{
//	LN_CORE_OBJECT_TYPE_INFO_DECL();
//public:
//	ScrollEventArgs(float newValue);
//	virtual ~ScrollEventArgs();
//
//public:
//	double			NewValue;	///< 新しい値
//	//ScrollEventType	EventType;	///< スクロールイベントの種類
//};


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
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount);	// TODO: やめる
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);	// TODO: やめる

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* Create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(Find(TEventArgs::GetClassTypeInfo()));
		if (e == NULL) {
			e = LN_NEW TEventArgs(args...);
			Register(e);
		}
		else {
			e->~TEventArgs();
			new (e)TEventArgs(args...);
		}
		e->Handled = false;
		e->AddRef();
		return e;
	}

private:
	typedef Array<RoutedEventArgs*>					EventArgsList;
	typedef SortedArray<TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	RoutedEventArgs* Find(TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.TryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->GetRefCount() == 1) {	// このリストからしか参照されていなければ返す
					return e;
				}
			}
		}
		return NULL;
	}

	void Register(RoutedEventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.TryGetValue(GetTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.Add(GetTypeInfo(e), list);
		}
		list->Add(e);
	}


	//Array<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T FindFreeObject(const Array<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	Array<MouseEventArgs*>	m_mouseEventArgsPool;
	Array<KeyEventArgs*>	m_keyEventArgsPool;
};




} // namespace GUI
} // namespace Lumino
