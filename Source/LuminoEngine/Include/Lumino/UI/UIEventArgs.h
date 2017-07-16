
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>

LN_NAMESPACE_BEGIN
class UIEventArgs;
using UIEventArgsPtr = Ref<UIEventArgs>;
class UIEventInfo;
using UIEventType = const UIEventInfo*;

/**
	@brief		
*/
class UIEventArgs
	: public tr::ReflectionEventArgs
{
	LN_OBJECT();
public:
	static UIEventArgsPtr create(UIEventType type, Object* sender, bool caching = true);

	UIEventType getType() const { return m_type; }

	UIEventArgs();
	virtual ~UIEventArgs();
	void initialize(UIEventType type);

public:
	UIElement*	sender;

private:
	UIEventType m_type;

	friend class UIElement;
};

/**
	@brief	マウスイベントの引数です。
*/
class UIMouseEventArgs
	: public UIEventArgs
{
	LN_OBJECT();
public:

	/** UIMouseEventArgs のインスタンスを作成します。*/
	static Ref<UIMouseEventArgs> create(UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching = true);

	/** ボタンの種類を取得します。*/
	MouseButtons getMouseButtons() const { return m_button; }

	/** マウスイベント生成時のマウスの座標を取得します。*/
	const PointF& getPosition() const { return m_position; }

	/** ボタンがクリックされた回数を取得します。(ダブルクリックやトリプルクリックを区別するために使用する。最大3まで) */
	int getClickCount() const { return m_clickCount; }

	/** 指定した要素から見た相対的なマウス ポインターの位置を返します。*/
	PointF getPosition(UIElement* relativeTo) const;

LN_CONSTRUCT_ACCESS:
	UIMouseEventArgs();
	virtual ~UIMouseEventArgs();
	void initialize(UIEventType type, MouseButtons button, float x, float y, int clickCount);

private:
	MouseButtons	m_button;
	PointF			m_position;
	int				m_clickCount;
};

/**
	@brief	キーボードイベントの引数です。
*/
class UIKeyEventArgs
	: public UIEventArgs
{
	LN_OBJECT();
public:

	/** UIKeyEventArgs のインスタンスを作成します。 */
	static Ref<UIKeyEventArgs> create(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode, bool caching = true);

	/** キーコードを取得します。 */
	Keys getKey() const { return m_keyCode; }

	/** 修飾キーを取得します。 */
	ModifierKeys getModifierKeys() const { return m_modifierKeys; }

	/** TextInput イベントにより通知された場合、その文字コードを取得します。 */
	TCHAR getCharCode() const { return m_charCode; }

LN_CONSTRUCT_ACCESS:
	UIKeyEventArgs();
	virtual ~UIKeyEventArgs();
	void initialize(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode);

private:
	Keys			m_keyCode;
	ModifierKeys	m_modifierKeys;
	TCHAR			m_charCode;
};

/**
	@brief	マウスホイールの移動イベントの引数です。
*/
class UIMouseWheelEventArgs
	: public UIEventArgs
{
	LN_OBJECT();
public:

	/** UIMouseWheelEventArgs のインスタンスを作成します。 */
	static Ref<UIMouseWheelEventArgs> create(UIEventType type, int delta, bool caching = true);

	/** マウスホイールの回転回数を取得します。 (正または負の回数) */
	int getDelta() const { return m_delta; }

LN_CONSTRUCT_ACCESS:
	UIMouseWheelEventArgs();
	virtual ~UIMouseWheelEventArgs();
	void initialize(UIEventType type, int delta);

public:
	int		m_delta;
};

LN_NAMESPACE_END
