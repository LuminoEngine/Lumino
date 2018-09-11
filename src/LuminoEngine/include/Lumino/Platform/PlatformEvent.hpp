
#pragma once
#include <Lumino/Base/EnumFlags.hpp>

namespace ln {
class DataObject;


/** キーコード (NumPad はサポートしていない) */
enum class Keys
{
	Unknown = 0,

	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7,
	D8,
	D9,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	Space,	// 0x20 = ' ' (スペース)
	Escape,
	Up,
	Down,
	Left,
	Right,
	LShift,
	RShift,
	LCtrl,  // TODO: LeftControl
	RCtrl,
	LAlt,       // mac:option
	RAlt,
	Tab,
	Enter,
	BackSpace,
	Insert,
	Delete,
	PageUp,
	PageDown,
	Home,
	End,

	Colon,		///< :	[非推奨 GLFWでは;]
	Semicolon,	///< ;	[非推奨 GLFWでは=]
	Comma,		///< ,
	Period,		///< .
	Slash,		///< /
	Minus,		///< -
	Backslash,	///< \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
	Yen,		///< \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
	Caret,		///< ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
	LBracket,	///< [
	RBracket,	///< ]

	AndroidPlus,		///< Android '+'

	TERMINATOR,
};

/** 修飾キー */
enum class ModifierKeys
{
	None	= 0x0000,
	Alt		= 0x0001,	/**< Alt キー */
	Shift	= 0x0002,	/**< Shift キー */
	Control	= 0x0004,	/**< Ctrl キー */
};
LN_FLAGS_OPERATORS(ModifierKeys);

enum class DragDropEffects
{
	None,
	Copy,
};

namespace detail {

class PlatformWindow;

/** システムイベントの種類 */
enum class PlatformEventType
{
	Unknown,				/** 不明なイベント */

	Quit,					/** アプリ終了要求 */
	close,					/** ウィンドウが閉じられようとしている */
	WindowActivate,			/** ウィンドウがアクティブになった */
	WindowDeactivate,		/** ウィンドウが非アクティブになった */

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

	WindowSizeChanged,		/** ウィンドウサイズが変更された */


	DragEnter,				/** ドラッグドロップ開始 */
	DragDrop,				/** ドラッグドロップ確定 */

	//ANDROID_PAUSED = 101,		/** Android APP_CMD_RESUME */
	//ANDROID_RESUMED = 102,	/** Android APP_CMD_PAUSE */
};


/** マウスボタン */
enum class MouseButtons
{
	None = 0,		/**< 無効または押されていないことを示す */
	Left,			/**< 左ボタン */
	Right,			/**< 右ボタン */
	Middle,			/**< 中央ボタン */

	X1,				/**< 拡張ボタン1 */
	X2,				/**< 拡張ボタン2 */
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
			MouseButtons	button;	/** ボタン番号 */
			short			x;				/** マウスイベント生成時のマウスの X 座標 (クライアント領域外は -1) */
			short			y;				/** マウスイベント生成時のマウスの Y 座標 (クライアント領域外は -1) */
			short			moveX;			/** X 座標の移動量 */
			short			moveY;			/** Y 座標の移動量 */
			bool			inClientArea;	/** クライアント領域の内部であるか */

		} mouse;

		/** キーボードイベントの引数 */
		struct
		{
			Keys				keyCode;		/** キーコード */
			ModifierKeys	modifierKeys;	/** 修飾キー */
			char			keyChar;		/** 文字 */

		} key;

		/** サイズ情報 */
		struct
		{
			short			width;
			short			height;

		} size;

		/** マウスホイールの操作 */
		struct
		{
			short			delta;			/** マウスホイールの回転回数 (windows では 1 回につき 120 が格納されるが、これは正または負の回数で、1単位) */

		} wheel;

		/** ドラッグ&ドロップ情報 */
		struct
		{
			DataObject*			data;
			DragDropEffects*	effect;

		} dragDrop;
	};

public:
	/** 種類と送信元ウィンドウを指定して初期化する */
	PlatformEventArgs(PlatformEventType type_, PlatformWindow* sender_) { type = type_; sender = sender_; }
	PlatformEventArgs() {}

	static PlatformEventArgs makeClosingEvent(PlatformWindow* sender);
	static PlatformEventArgs makeWindowSizeChangedEvent(PlatformWindow* sender, int width, int height);
	static PlatformEventArgs makeActivateChangedEvent(PlatformWindow* sender, bool active);
	static PlatformEventArgs makeKeyEvent(PlatformWindow* sender, PlatformEventType type, Keys keyCode, ModifierKeys modifierKeys, char keyChar);
	static PlatformEventArgs makeMouseWheelEvent(PlatformWindow* sender, int delta);
	static PlatformEventArgs makeDragDropEvent(PlatformWindow* sender, PlatformEventType type, DataObject* data, DragDropEffects* effect);
};

class IPlatforEventListener
{
public:
	virtual bool onPlatformEvent(const PlatformEventArgs& e) = 0;
};

} // namespace detail
} // namespace ln

