#pragma once
#include <functional>
#include "Common.hpp"
#include "../Platform/PlatformEvent.hpp"

namespace ln {
class UIElement;
class UIEventArgs;

/**
	@brief		特定のイベントデータを持たない、UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
LN_DELEGATE()
using UIEventHandler = std::function<void(UIEventArgs* e)>;

using UIEventType = int32_t;


/** */
// TODO: このへん拡張しやすいような仕組みを考え直したい
class UIEvents
{
public:
    /** GotFocusEvent イベントの識別子 (UIEventArgs) */
    static UIEventType	GotFocusEvent;

    /** LostFocusEvent イベントの識別子 (UIEventArgs) */
    static UIEventType	LostFocusEvent;

    /** MouseMove イベントの識別子 (UIMouseEventArgs) */
    static UIEventType	MouseMoveEvent;

    /** MouseDown イベントの識別子 (UIMouseEventArgs) */
    static UIEventType	MouseDownEvent;

    /** MouseUp イベントの識別子 (UIMouseEventArgs) */
    static UIEventType	MouseUpEvent;

    /** MouseWheel イベントの識別子 (UIMouseWheelEventArgs) */
    static UIEventType	MouseWheelEvent;

    /** MouseEnterEvent イベントの識別子 (UIMouseEventArgs) */
    static UIEventType	MouseEnterEvent;

    /** MouseLeaveEvent イベントの識別子 (UIMouseEventArgs) */
    static UIEventType	MouseLeaveEvent;

    /** KeyDownEvent イベントの識別子 (UIKeyEventArgs) */
    static UIEventType	KeyDownEvent;

    /** KeyUpEvent イベントの識別子 (UIKeyEventArgs) */
    static UIEventType	KeyUpEvent;

    /** TextInputEvent イベントの識別子 (UIKeyEventArgs) */
    static UIEventType	TextInputEvent;

    /** Checked イベントの識別子 (UIEventArgs) */
    static UIEventType	CheckedEvent;

    /** Unchecked イベントの識別子 (UIEventArgs) */
    static UIEventType	UncheckedEvent;

    /** DragStartedEvent イベントの識別子 (UIEventArgs) マウスドラッグを開始したときに発生するイベントを表します。*/
    static UIEventType	ScrollDragStartedEvent;

    /** DragDeltaEvent イベントの識別子 (UIEventArgs) マウスドラッグ中に発生するイベントを表します。 */
    static UIEventType	ScrollDragDeltaEvent;

    /** DragCompletedEvent イベントの識別子 (UIEventArgs) マウスドラッグが完了したときに発生するイベントを表します。 */
    static UIEventType	ScrollDragCompletedEvent;

    /** DragCanceledEvent イベントの識別子 (UIEventArgs) マウスドラッグを中断したときに発生するイベントを表します。 */
    static UIEventType	ScrollDragCanceledEvent;

    /** ScrollEvent イベントの識別子 (UIEventArgs) */
    static UIEventType	ScrollEvent;

    /** DragEnter イベントの識別子 (UIDragDropEventArgs) */
    static UIEventType	DragEnterEvent;

    /** DragDrop イベントの識別子 (UIDragDropEventArgs) */
    static UIEventType	DragDropEvent;
};

/**
	@brief		
*/
class UIEventArgs
	: public Object
{
	LN_OBJECT;
public:
	static Ref<UIEventArgs> create(UIElement* sender, UIEventType type, bool caching = true);

	UIEventType type() const { return m_type; }

LN_CONSTRUCT_ACCESS:
	UIEventArgs();
	virtual ~UIEventArgs();
	void initialize(UIElement* sender, UIEventType type);

public:
    bool handled;

private:
    UIElement* m_sender;
	UIEventType m_type;
};

/**
	@brief	マウスイベントの引数です。
*/
class UIMouseEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:

	/** UIMouseEventArgs のインスタンスを作成します。*/
	static Ref<UIMouseEventArgs> create(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching = true);

	/** ボタンの種類を取得します。*/
	MouseButtons getMouseButtons() const { return m_button; }

	/** マウスイベント生成時のマウスの座標を取得します。*/
	const Point& getPosition() const { return m_position; }

	/** ボタンがクリックされた回数を取得します。(ダブルクリックやトリプルクリックを区別するために使用する。最大3まで) */
	int getClickCount() const { return m_clickCount; }

	/** 指定した要素から見た相対的なマウス ポインターの位置を返します。*/
	Point getPosition(UIElement* relativeTo) const;

LN_CONSTRUCT_ACCESS:
	UIMouseEventArgs();
	virtual ~UIMouseEventArgs();
	void initialize(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount);

private:
	MouseButtons	m_button;
	Point			m_position;
	int				m_clickCount;
};

/**
	@brief	キーボードイベントの引数です。
*/
class UIKeyEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:

	/** UIKeyEventArgs のインスタンスを作成します。 */
	static Ref<UIKeyEventArgs> create(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode, bool caching = true);

	/** キーコードを取得します。 */
	Keys getKey() const { return m_keyCode; }

	/** 修飾キーを取得します。 */
	ModifierKeys getModifierKeys() const { return m_modifierKeys; }

	/** TextInput イベントにより通知された場合、その文字コードを取得します。 */
	Char getCharCode() const { return m_charCode; }

LN_CONSTRUCT_ACCESS:
	UIKeyEventArgs();
	virtual ~UIKeyEventArgs();
	void initialize(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode);

private:
	Keys			m_keyCode;
	ModifierKeys	m_modifierKeys;
	Char			m_charCode;
};

/**
	@brief	マウスホイールの移動イベントの引数です。
*/
class UIMouseWheelEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:

	/** UIMouseWheelEventArgs のインスタンスを作成します。 */
	static Ref<UIMouseWheelEventArgs> create(UIElement* sender, UIEventType type, int delta, bool caching = true);

	/** マウスホイールの回転回数を取得します。 (正または負の回数) */
	int getDelta() const { return m_delta; }

LN_CONSTRUCT_ACCESS:
	UIMouseWheelEventArgs();
	virtual ~UIMouseWheelEventArgs();
	void initialize(UIElement* sender, UIEventType type, int delta);

public:
	int		m_delta;
};

/** ドラッグアンドドロップイベントの引数です。 */
class UIDragDropEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:

	/** ドラッグイベントに関連付けられているデータを取得します。 */
	DataObject* data() const { return m_data; }

	/** ドラッグ アンド ドロップ操作の効果を取得します。 */
	DragDropEffects effect() const { return m_effect; }

	/** ドラッグ アンド ドロップ操作の効果を取得します。 */
	void setEffect(DragDropEffects effect) { m_effect = effect; }

LN_CONSTRUCT_ACCESS:
	UIDragDropEventArgs();
	virtual ~UIDragDropEventArgs();
	void initialize(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect);

LN_INTERNAL_ACCESS:
	static Ref<UIDragDropEventArgs> create(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect, bool caching = true);

public:
	DataObject* m_data;
	DragDropEffects	m_effect;
};

} // namespace ln

