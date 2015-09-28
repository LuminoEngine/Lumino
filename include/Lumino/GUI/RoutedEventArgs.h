
#pragma once
#include "../Common.h"
#include "../Variant.h"
#include "../RoutedEvent.h"
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
class UIElement;




/** ルーティングイベントの種類を表します。*/
enum class RoutedEventType
{
	Unknown = 0,

	MouseMove,			/**< マウスポインタが移動した */
	MouseButtonDown,	/**< マウスボタンが押された */
	MouseButtonUp,		/**< マウスボタンが離された */
	MouseWheel,			/**< マウスホイールが操作された */

	KeyDown,			/**< キーが押された */
	KeyUp,				/**< キーが押された */
	TextInput,			/**< 文字入力が発生した */
	ElapsedTime,		/**< 時間の経過が通知された */

	MouseEnter,			/**< UI要素にマウスカーソルが乗った */
	MouseLeave,			/**< UI要素からマウスカーソルが離れた */
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
	MouseButton	Button;		/**< ボタンの種類 */
	int			Wheel;		/**< マウスホイールの回転回数 (正または負の回数) */
	float		X;			/**< マウスイベント生成時のマウスの X 座標 (コンテキスト内の座標系) */
	float		Y;			/**< マウスイベント生成時のマウスの Y 座標 (コンテキスト内の座標系) */
	int			ClickCount;	/**< ボタンがクリックされた回数。ダブルクリックやトリプルクリックを区別するために使用する。最大3まで。 */
};

/**
	@brief	キーボードイベントの引数を表します。	
*/
class KeyEventArgs
	: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

public:
	Key			KeyCode;	/**< キーコード */
	bool		IsAlt;		/**< Alt キーが押されている場合 true */
	bool		IsShift;	/**< Shift キーが押されている場合 true */
	bool		IsControl;	/**< Ctrl キーが押されている場合 true */
	TCHAR		Char;		/**< TextInput イベントにより通知された場合、その文字コードが格納されます。*/
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
//	double			NewValue;	/**< 新しい値 */
//	//ScrollEventType	EventType;	/**< スクロールイベントの種類 */
//};


LN_NAMESPACE_GUI_END
} // namespace Lumino
