#pragma once
#include <functional>
#include "Common.hpp"
#include <LuminoPlatform/PlatformEvent.hpp>

namespace ln {
class InputGesture;
class RenderView;
class UICommand;
class UIElement;
class UIEventArgs;

/** Scroll イベントの原因となった動作を表します。*/
enum class ScrollEventType
{
    ThumbTrack,			/** つまみがドラッグされている */
    EndScroll,			/** つまみのドラッグが終了した */
    SmallDecrement,		/** スクロールバーのデクリメントボタンがクリックされた */
    SmallIncrement,		/** スクロールバーのインクリメントボタンがクリックされた */
    LargeDecrement,		/** スクロールバーの PageUp 領域がクリックされた */
    LargeIncrement,		/** スクロールバーの PageDown 領域がクリックされた */
};

using UIEventType = int32_t;


/** */
// TODO: このへん拡張しやすいような仕組みを考え直したい
class UIEvents
{
public:
	static const UIEventType	Default;	// 型を詳細説明する必要が無いもの

    /** GotFocusEvent イベントの識別子 (UIEventArgs) */
    static const UIEventType	GotFocusEvent;

    /** LostFocusEvent イベントの識別子 (UIEventArgs) */
    static const UIEventType	LostFocusEvent;

    /** MouseMove イベントの識別子 (UIMouseEventArgs) */
    static const UIEventType	MouseMoveEvent;

    /** MouseDown イベントの識別子 (UIMouseEventArgs) */
    static const UIEventType	MouseDownEvent;

    /** MouseUp イベントの識別子 (UIMouseEventArgs) */
    static const UIEventType	MouseUpEvent;

    /** MouseWheel イベントの識別子 (UIMouseWheelEventArgs) */
    static const UIEventType	MouseWheelEvent;

    /** MouseEnterEvent イベントの識別子 (UIMouseEventArgs) */
    static const UIEventType	MouseEnterEvent;

    /** MouseLeaveEvent イベントの識別子 (UIMouseEventArgs) */
    static const UIEventType	MouseLeaveEvent;

    /** KeyDownEvent イベントの識別子 (UIKeyEventArgs) */
    static const UIEventType	KeyDownEvent;

    /** KeyUpEvent イベントの識別子 (UIKeyEventArgs) */
    static const UIEventType	KeyUpEvent;

    /** TextInputEvent イベントの識別子 (UIKeyEventArgs) */
    static const UIEventType	TextInputEvent;

    /** Checked イベントの識別子 (UIEventArgs) */
    static const UIEventType	CheckedEvent;

    /** Unchecked イベントの識別子 (UIEventArgs) */
    static const UIEventType	UncheckedEvent;

    /** DragStartedEvent イベントの識別子 (UIEventArgs) マウスドラッグを開始したときに発生するイベントを表します。*/
    static const UIEventType	ScrollDragStartedEvent;

    /** DragDeltaEvent イベントの識別子 (UIEventArgs) マウスドラッグ中に発生するイベントを表します。 */
    static const UIEventType	ScrollDragDeltaEvent;

    /** DragCompletedEvent イベントの識別子 (UIEventArgs) マウスドラッグが完了したときに発生するイベントを表します。 */
    static const UIEventType	ScrollDragCompletedEvent;

    /** DragCanceledEvent イベントの識別子 (UIEventArgs) マウスドラッグを中断したときに発生するイベントを表します。 */
    static const UIEventType	ScrollDragCanceledEvent;

    /** ScrollEvent イベントの識別子 (UIEventArgs) */
    static const UIEventType	ScrollEvent;

    /** DragEnter イベントの識別子 (UIDragDropEventArgs) */
    static const UIEventType	DragEnterEvent;

    /** DragDrop イベントの識別子 (UIDragDropEventArgs) */
    static const UIEventType	DragDropEvent;

	static const UIEventType	Selected;
	static const UIEventType	Unselected;

    static const UIEventType	Click;	// TODO: obsolete
	static const UIEventType	Submitted;

    static const UIEventType    SelectionChanged;

	static const UIEventType	Timer;

	static const UIEventType	Activated;
	static const UIEventType	Deactivated;

    static const UIEventType	ExecuteCommandEvent;
    static const UIEventType	CanExecuteCommandEvent;
    static const UIEventType	CanExecuteChangedEvent;

    static const UIEventType    NotifyPropertyChanged;


    static const UIEventType	RequestVisualUpdateEvent;
    static const UIEventType	RequestVisualRedrawEvent;

	static const UIEventType	RequestNavigate;

	//static const UIEventType	ComboBoxItemClicked;
};

/**
	@brief		
*/
LN_CLASS()
class UIEventArgs
	: public Object
{
	LN_OBJECT;
public:
	static Ref<UIEventArgs> create(UIElement* sender, UIEventType type, bool caching = true);

	/** イベントの発生元となった要素を取得します。 */
	LN_METHOD()
    UIElement* sender() const { return m_sender; }

	UIEventType type() const { return m_type; }
	
    static bool testInputGesture(UIEventArgs* e, InputGesture* gesture);

	UIEventRoutingStrategy strategy = UIEventRoutingStrategy::Bubble;

LN_CONSTRUCT_ACCESS:
	UIEventArgs();
	virtual ~UIEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type);

public:
    bool handled;

private:
    UIElement* m_sender;
	UIEventType m_type;
};

/**
 * マウスイベントの引数です。
 *
 * マウスイベントは任意の座標値を与えて発行することができます。
 * そのため座標値は UI 要素のヒットボックス外の値が入力されることがあります。
 */
class UIMouseEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:

	/** UIMouseEventArgs のインスタンスを作成します。*/
	static Ref<UIMouseEventArgs> create(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, ModifierKeys modifierKeys, bool caching = true);

	/** ボタンの種類を取得します。*/
	MouseButtons getMouseButtons() const { return m_button; }

	/** マウスイベント生成時のマウスの座標を取得します。*/
	const Point& getPosition() const { return m_position; }

	/** ボタンがクリックされた回数を取得します。(ダブルクリックやトリプルクリックを区別するために使用する。最大3まで) */
	int getClickCount() const { return m_clickCount; }

	/** 修飾キーを取得します。 */
	ModifierKeys modifierKeys() const { return m_modifierKeys; }

	/** 指定した要素から見た相対的なマウス ポインターの位置を返します。*/
	Point getPosition(UIElement* relativeTo) const;
	//Point getPosition(RenderView* relativeTo) const;

	float grabOffsetX = 0;
	float grabOffsetY = 0;

LN_CONSTRUCT_ACCESS:
	UIMouseEventArgs();
	virtual ~UIMouseEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, ModifierKeys modifierKeys);

private:
	MouseButtons m_button;
	Point m_position;
	int m_clickCount;
	ModifierKeys m_modifierKeys;
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
	Result<> init();
	bool init(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode);

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
	Result<> init();
	bool init(UIElement* sender, UIEventType type, int delta);

public:
	int		m_delta;
};

/** ドラッグが関係するイベント引数です。 */
class UIDragDeltaEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:
	static Ref<UIDragDeltaEventArgs> create(UIElement* sender, UIEventType type, float offsetX, float offsetY, bool caching = true);

	/** ドラッグ開始点からの水平距離 */
	float offsetX() const { return m_offsetX; }

	/** ドラッグ開始点からの垂直距離 */
	float offsetY() const { return m_offsetY; }

LN_CONSTRUCT_ACCESS:
	UIDragDeltaEventArgs();
	virtual ~UIDragDeltaEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type, float offsetX, float offsetY);

public:
	float m_offsetX;
	float m_offsetY;
};

/**  ScrollBar のスクロールイベントの引数です。 */
class UIScrollEventArgs
    : public UIEventArgs
{
    LN_OBJECT;
public:
    static Ref<UIScrollEventArgs> create(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType, bool caching = true);

    /** スクロール後の新しい値 */
    float newValue() const { return m_newValue; }

    /** スクロールイベントの原因 */
    ScrollEventType scrollType() const { return m_scrollType; }

LN_CONSTRUCT_ACCESS:
    UIScrollEventArgs();
    virtual ~UIScrollEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType);

public:
    float m_newValue;	
    ScrollEventType	m_scrollType;
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
	Result<> init();
	bool init(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect);

LN_INTERNAL_ACCESS:
	static Ref<UIDragDropEventArgs> create(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect, bool caching = true);

public:
	DataObject* m_data;
	DragDropEffects	m_effect;
};

/** クリックイベントの引数です。 */
class UIClickEventArgs
    : public UIEventArgs
{
    LN_OBJECT;
public:
    /** UIClickEventArgs のインスタンスを作成します。*/
    static Ref<UIClickEventArgs> create(UIElement* sender, UIEventType type, int clickCount, bool caching = true);

    int clickCount() const { return m_clickCount; }

LN_CONSTRUCT_ACCESS:
    UIClickEventArgs();
    virtual ~UIClickEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type, int clickCount);

private:
    int m_clickCount;
};

/** SelectionChanged イベントの引数です。 */
class UISelectionChangedEventArgs
    : public UIEventArgs
{
    LN_OBJECT;
public:
    /** UIClickEventArgs のインスタンスを作成します。*/
    static Ref<UISelectionChangedEventArgs> create(UIElement* sender, UIEventType type, bool caching = true);

    // TODO: RemovedItems
    // TODO: AddedItems

LN_CONSTRUCT_ACCESS:
    UISelectionChangedEventArgs();
    Result<> init();
	bool init(UIElement* sender, UIEventType type);

private:
};


/** コマンド実行可否判定及び実行イベントの引数です。 */
class UICommandEventArgs
    : public UIEventArgs
{
    LN_OBJECT;
public:
    static Ref<UICommandEventArgs> create(UIElement* sender, UIEventType type, UICommand* command, bool caching = true);

    UICommand* command() const { return m_command; }

    // default: true
    bool canExecute() const { return m_canExecute; }
    void setCanExecute(bool value) { m_canExecute = value; }

    // Utils
    static void raiseExecute(UIElement* target, UICommand* command);

LN_CONSTRUCT_ACCESS:
    UICommandEventArgs();
    virtual ~UICommandEventArgs() = default;
	Result<> init();
	bool init(UIElement* sender, UIEventType type, UICommand* command);

public:
    UICommand* m_command;
    bool m_canExecute;
};


/** PropertyChanged と CollectionChanged 共用。プロパティの場合は Reset 固定。 */
class UINotifyPropertyChangedEventArgs
    : public UIEventArgs
{
    LN_OBJECT;
public:
    static Ref<UINotifyPropertyChangedEventArgs> create(UIElement* sender, UIEventType type, const StringView& propertyName, bool caching = true);
    static Ref<UINotifyPropertyChangedEventArgs> create(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count, bool caching = true);

    UICollectionChangedAction action() const { return m_action; }
    const String& name() const { return m_name; }
    int startIndex() const { return m_startIndex; }
    int count() const { return m_count; }

LN_CONSTRUCT_ACCESS:
    UINotifyPropertyChangedEventArgs();
    Result<> init();
	bool init(UIElement* sender, UIEventType type, const StringView& propertyName);
	bool init(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count);

public:
    UICollectionChangedAction m_action;
    String m_name;
    int m_startIndex;
    int m_count;
};

/** Timer イベントの引数です。 */
class UITimerEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:
	/** UITimerEventArgs のインスタンスを作成します。*/
	static Ref<UITimerEventArgs> create(UIElement* sender, UIEventType type, float elapsedSeconds, bool caching = true);

	float elapsedSeconds() const { return m_elapsedSeconds; }

LN_CONSTRUCT_ACCESS:
	UITimerEventArgs();
	virtual ~UITimerEventArgs() = default;
	Result<> init();
	bool init(UIElement* sender, UIEventType type, float elapsedSeconds);

private:
	float m_elapsedSeconds;
};

/** RequestNavigate イベントの引数です。 */
class UIRequestNavigateEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
public:
	/** UITimerEventArgs のインスタンスを作成します。*/
	static Ref<UIRequestNavigateEventArgs> create(UIElement* sender, UIEventType type, const String& url, bool caching = true);

	const String& url() const { return m_url; }

LN_CONSTRUCT_ACCESS:
	UIRequestNavigateEventArgs();
	Result<> init();
	bool init(UIElement* sender, UIEventType type, const String& url);

private:
	String m_url;
};


/**
	@brief		特定のイベントデータを持たない、UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
LN_DELEGATE()
using UIGeneralEventHandler = Delegate<void(UIEventArgs* e)>;
// TODO: ↑ deprecated. UIEventHandler と統合し、引数無しイベントハンドラをやめる。
//
// Note: C や HSP などラムダ使えない Binding ではキャプチャによって任意のデータを関連付けることができない。
// そのため UIElement の data に Variant でデータを関連付けたいことが往々にしてあるのだが、
// そのためには UIEventArgs を引数にとるハンドラが必須となる。
// 従来、C++ での利便性のために引数無しハンドラを作っていたが、それだとカバーできない。
// 一応↓のように auto 使うことでこのくらいまで簡略化はできるので、これでひとまず進めてみる。
// element->connectOnSubmit([](auto x) { ... });
//

/**
 * Test delegate 1.
 *
 * @param[in] a : param 1.
 */
LN_DELEGATE()
using UIEventHandler = Delegate<void(void)>;

/**
	@brief		マウス操作が関係する UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
using UIMouseEventHandler = Delegate<void(UIMouseEventArgs* e)>;

/**
	@brief		ドラッグアンドドロップ操作が関係する UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
using UIDragDropEventHandler = Delegate<void(UIDragDropEventArgs* e)>;

using UIClickEventHandler = Delegate<void(UIClickEventArgs* e)>;
using UISelectionChangedEventHandler = Delegate<void(UISelectionChangedEventArgs* e)>;

using UICommandEventHandler = Delegate<void(UICommandEventArgs* e)>;

using UITimerEventHandler = Delegate<void(UITimerEventArgs* e)>;

} // namespace ln

