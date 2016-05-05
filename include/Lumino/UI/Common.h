
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Common.h"

LN_NAMESPACE_BEGIN
class UITypeInfo;
class UIEventArgs;
class UIStyle;
class UIStyleTable;
class UIContext;
class UILayoutView;
class UIElement;
class UIWindow;
class UIFrameWindow;
class UIMainWindow;
class UILayoutRoot;
class UIPanel;


/** 縦方向の表示位置を示します。*/
LN_ENUM(VerticalAlignment)
{
	Top = 0,			/**< 子要素を、親のレイアウト スロットの上端に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Bottom,				/**< 子要素を、親のレイアウト スロットの下端に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
};
LN_ENUM_DECLARE(VerticalAlignment);

/** 横方向の表示位置を示します。*/
LN_ENUM(HorizontalAlignment)
{
	Left = 0,			/**< 子要素を、親のレイアウト スロットの左側に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Right,				/**< 子要素を、親のレイアウト スロットの右側に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
};
LN_ENUM_DECLARE(HorizontalAlignment);

namespace detail
{
class UIManager;

enum class UIInternalEventType
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

LN_ENUM_FLAGS(InvalidateFlags)
{
	None = 0x0000,
	Layout = 0x0001,		// レイアウト系プロパティに変更がある
	Font = 0x0002,			// フォント系プロパティに変更がある
	Rendering = 0x004,			// 描画系プロパティに変更がある
	VisualState = 0x0008,	// VisualState に変更がある
};
LN_ENUM_FLAGS_DECLARE(InvalidateFlags);

} // namespace detail

LN_NAMESPACE_END
