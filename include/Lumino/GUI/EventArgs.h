
#pragma once
#include "../Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

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
	//EventType_Char,
	EventType_ElapsedTime,


	EventType_MouseLeave,
	EventType_MouseEnter,
};

/**
	@brief		GUI イベントの引数の基本クラスです。
*/
class EventArgs
	: public CoreObject
{
public:
	EventArgs();
	virtual ~EventArgs();

protected:

};

/**
	@brief	マウスイベントの引数を表します。
*/
class MouseEventArgs
	: public EventArgs
{
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

public:
	MouseButton	Button;		///< ボタン番号
	short		Wheel;		///< マウスホイールの回転回数 (正または負の回数)
	short		X;			///< マウスイベント生成時のマウスの X 座標 (グローバル座標。クライアント領域外は -1)
	short		Y;			///< マウスイベント生成時のマウスの Y 座標 (グローバル座標。クライアント領域外は -1)
	//short		MoveX;	    ///< X 座標の移動量
	//short		MoveY;      ///< Y 座標の移動量
};

/**
	@brief	キーボードイベントの引数を表します。	
*/
class KeyEventArgs
	: public EventArgs
{
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

public:
	Key			KeyCode;	///< キーコード
	bool		IsAlt;		///< Alt キーが押されている場合 true
	bool		IsShift;	///< Shift キーが押されている場合 true
	bool		IsControl;	///< Ctrl キーが押されている場合 true
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
			例えば C# では値渡しのままでよいのか、ref をつけるのか。Ruby で struct は使えないので結局 new する羽目になるがいいのか。

			Pool しておけば、若干直感的ではなくなるが、バインダ側の new も抑えることができる。

			後々イベントの種類が増えてきたときは拡張性のため、イベント名をキーにして Create するような仕組みが必要になるかもしれない。
*/
class EventArgsPool
{
public:
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, short wheel, short x, short y);
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);

private:
	template<class T>
	T FindFreeObject(const ArrayList<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	ArrayList<MouseEventArgs*>	m_mouseEventArgsPool;
	ArrayList<KeyEventArgs*>	m_keyEventArgsPool;
};








template<typename A1, typename A2>
class Event02 : public RefObject
{
public:
	typedef Delegate02<A1, A2> DelegateType;

public:
	void AddHandler(const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void RemoveHandler(const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void operator += (const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void operator -= (const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void Raise(A1 a1, A2 a2)	// GUI の EventArgs は Handler を返したいときがあるので const 参照にはしない
	{
		LN_FOREACH(DelegateType& d, m_handlerList)
		{
			d.Call(a1, a2);
		}
	}

private:
	ArrayList<DelegateType> m_handlerList;
};

} // namespace GUI
} // namespace Lumino
