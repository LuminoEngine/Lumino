
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>

LN_NAMESPACE_BEGIN
class UIEventArgs;
using UIEventArgsPtr = RefPtr<UIEventArgs>;
class UIRoutedCommand;

/**
	@brief		
*/
class UIEventArgs
	: public tr::ReflectionEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static UIEventArgsPtr Create(Object* sender, bool caching = true);

	UIEventArgs();
	virtual ~UIEventArgs();

public:
	tr::ReflectionObject*	sender;
};

/**
	@brief	マウスイベントの引数です。
*/
class UIMouseEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIMouseEventArgs();
	UIMouseEventArgs(MouseButtons button, float x, float y, int clickCount);
	virtual ~UIMouseEventArgs();

	/** 指定した要素から見た相対的なマウス ポインターの位置を返します。*/
	PointF GetPosition(UIElement* relativeTo);

public:
	MouseButtons	button;			/**< ボタンの種類 */
	float			x;				/**< マウスイベント生成時のマウスの X 座標 (ルートウィンドウ内の座標系) */
	float			y;				/**< マウスイベント生成時のマウスの Y 座標 (ルートウィンドウ内の座標系) */
	int				clickCount;		/**< ボタンがクリックされた回数。ダブルクリックやトリプルクリックを区別するために使用する。最大3まで。 */
};

/**
	@brief	キーボードイベントの引数です。
*/
class UIKeyEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIKeyEventArgs();
	UIKeyEventArgs(Keys keyCode, ModifierKeys modifierKeys);
	virtual ~UIKeyEventArgs();

public:
	Keys			keyCode;		/**< キーコード */
	ModifierKeys	modifierKeys;	/** 修飾キー */
	TCHAR			charCode;		/**< TextInput イベントにより通知された場合、その文字コードが格納されます。*/
};

/**
	@brief	マウスホイールの移動イベントの引数です。
*/
class UIMouseWheelEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIMouseWheelEventArgs();
	UIMouseWheelEventArgs(int delta);
	virtual ~UIMouseWheelEventArgs();

public:
	int			wheel;			/**< マウスホイールの回転回数 (正または負の回数) */
};

/**
	@brief	
*/
class UICommandEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UICommandEventArgs();
	virtual ~UICommandEventArgs();

public:
	UIRoutedCommand*	command;
};

LN_NAMESPACE_END
