
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN

/** システムイベントの種類 */
enum class PlatformEventType
{
	Unknown,				/** 不明なイベント */

	Quit,					/** アプリ終了要求 */
	Close,					/** ウィンドウが閉じられようとしている */
	//AppActivate,			/** ウィンドウがアクティブになった */
	//AppDeactivate = 4,	/** ウィンドウが非アクティブになった */

	MouseDown,				/** [MouseEventArgs] マウスボタンが押された */
	MouseUp,				/** [MouseEventArgs] マウスボタンが離された */
	MouseMove,				/** [MouseEventArgs] マウスが移動した */
	MouseWheel,				/** [MouseEventArgs] マウスホイールが操作された */

	KeyDown,				/** [MouseEventArgs] キー押下 */
	KeyUp,					/** [MouseEventArgs] キー押し上げ */
	KeyChar,				/** [MouseEventArgs] 文字入力 (CtrlやAltキーが押されず、日本語入力 OFF の状態でキーが押され場合) */

	//TOUCH_DOWN = 13,		/** [ LNEvent::Touch ] 画面がタッチされた */
	//TOUCH_UP = 14,		/** [ LNEvent::Touch ] 画面から離された */
	//TOUCH_MOVE = 15,		/** [ LNEvent::Touch ] タッチ中に移動した */

	//WINDOW_SIZE_CHANGED,		/** ウィンドウサイズが変更された */

	//ANDROID_PAUSED = 101,		/** Android APP_CMD_RESUME */
	//ANDROID_RESUMED = 102,	/** Android APP_CMD_PAUSE */
};

/** イベント引数 */
struct PlatformEventArgs
{
	PlatformEventType	type;			/** イベントの種類 */
	PlatformWindow*		sender;			/** イベントの送信元ウィンドウ */

	union
	{
		/** マウスイベントの引数 */
		struct
		{
			MouseButton::value_type	button;	/** ボタン番号 */
			short		wheelDelta;			/** マウスホイールの回転回数 (windows では 1 回につき 120 が格納されるが、これは正または負の回数で、1単位) */
			short		x;					/** マウスイベント生成時のマウスの X 座標 (クライアント領域外は -1) */
			short		y;					/** マウスイベント生成時のマウスの Y 座標 (クライアント領域外は -1) */
			short		moveX;				/** X 座標の移動量 */
			short		moveY;				/** Y 座標の移動量 */
			bool		inClientArea;		/** クライアント領域の内部であるか */

		} mouse;

		/** キーボードイベントの引数 */
		struct
		{
			Key			keyCode;	/** キーコード */
			bool		isAlt;		/** Alt キーが押されている場合 true */
			bool		isShift;	/** Shift キーが押されている場合 true */
			bool		isControl;	/** Ctrl キーが押されている場合 true */
			char		keyChar;	/** 文字 */

		} key;
	};

public:
	/** 種類と送信元ウィンドウを指定して初期化する */
	PlatformEventArgs(PlatformEventType type_, PlatformWindow* sender_) { type = type_; sender = sender_; }
	PlatformEventArgs() {}
};

LN_NAMESPACE_END

