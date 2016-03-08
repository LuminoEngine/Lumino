
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN

/// システムイベントの種類
enum EventType
{
	EventType_Unknown = 0,		///< 不明なイベント

	EventType_Quit,				///< アプリ終了要求
	EventType_Close,			///< ウィンドウが閉じられようとしている
	//EventType_AppActivate,		///< ウィンドウがアクティブになった
	//EventType_AppDeactivate = 4,	///< ウィンドウが非アクティブになった

	EventType_MouseDown,		///< [MouseEventArgs] マウスボタンが押された
	EventType_MouseUp,			///< [MouseEventArgs] マウスボタンが離された
	EventType_MouseMove,		///< [MouseEventArgs] マウスが移動した
	EventType_MouseWheel,		///< [MouseEventArgs] マウスホイールが操作された

	EventType_KeyDown,			///< [MouseEventArgs] キー押下
	EventType_KeyUp,			///< [MouseEventArgs] キー押し上げ
	EventType_KeyChar,			///< [MouseEventArgs] 文字入力 (CtrlやAltキーが押されず、日本語入力 OFF の状態でキーが押され場合)

	//EventType_TOUCH_DOWN = 13,	///< [ LNEvent::Touch ] 画面がタッチされた
	//EventType_TOUCH_UP = 14,	///< [ LNEvent::Touch ] 画面から離された
	//EventType_TOUCH_MOVE = 15,	///< [ LNEvent::Touch ] タッチ中に移動した

	//EventType_WINDOW_SIZE_CHANGED,		///< ウィンドウサイズが変更された

	//EventType_ANDROID_PAUSED = 101,	///< Android APP_CMD_RESUME
	//EventType_ANDROID_RESUMED = 102,	///< Android APP_CMD_PAUSE
};

/**
	@brief		イベント引数
	@note		以前は EventArgs クラスから派生した MouseEventArgs や KeyEventArgs を定義するスタイルを取っていた。
				しかし、メインスレッドと UI スレッドを分ける場合イベントは一度キューに積む必要があり、
				そのためにはメモリの動的確保(new)が必須となる。頻繁に飛んでくるメッセージに対して new はできればやりたくない。
				new しない場合にできなくなるのは継承を使ったオブジェクト指向的なユーザー拡張イベント引数の実装。
				…であるが、別の Lumino.GUI であればともかく、Platform イベントを拡張したい、しかもこのライブラリを使って、
				ということがあるのかというと、まずないと思われる。
				例えあったとしても相当レアケースなので、new のコストと天秤に掛けると却下するべき。ユーザーデータとして void* 持っておくだけで十分。
*/
struct PlatformEventArgs
{
	EventType		Type;			///< イベントの種類
	PlatformWindow*	Sender;			///< イベントの送信元ウィンドウ

	union
	{
		/// マウスイベントの引数
		struct
		{
			MouseButton::enum_type	Button;			///< ボタン番号
			short		WheelDelta;			///< マウスホイールの回転回数 (windows では 1 回につき 120 が格納されるが、これは正または負の回数で、1単位)
			short		X;				///< マウスイベント生成時のマウスの X 座標 (クライアント領域外は -1)
			short		Y;				///< マウスイベント生成時のマウスの Y 座標 (クライアント領域外は -1)
			short		MoveX;			///< X 座標の移動量
			short		MoveY;			///< Y 座標の移動量
			bool		InClientArea;	///< クライアント領域の内部であるか

		} Mouse;

		/// キーボードイベントの引数
		struct
		{
			Key			KeyCode;	///< キーコード
			bool		IsAlt;		///< Alt キーが押されている場合 true
			bool		IsShift;	///< Shift キーが押されている場合 true
			bool		IsControl;	///< Ctrl キーが押されている場合 true
			char		Char;

		} Key;
	};

public:

	/// 種類と送信元ウィンドウを指定して初期化する
	PlatformEventArgs(EventType type, PlatformWindow* sender) { Type = type; Sender = sender; }
	PlatformEventArgs() {}
};

LN_NAMESPACE_END

