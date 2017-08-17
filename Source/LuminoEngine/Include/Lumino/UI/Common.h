
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

/** 縦方向の表示位置を示します。*/
LN_ENUM(VAlignment)
{
	Top = 0,			/**< 子要素を、親のレイアウト スロットの上端に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Bottom,				/**< 子要素を、親のレイアウト スロットの下端に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
};
LN_ENUM_DECLARE(VAlignment);

/** 横方向の表示位置を示します。*/
LN_ENUM(HAlignment)
{
	Left = 0,			/**< 子要素を、親のレイアウト スロットの左側に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Right,				/**< 子要素を、親のレイアウト スロットの右側に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
};
LN_ENUM_DECLARE(HAlignment);


/** */
LN_ENUM_FLAGS(AlignmentAnchor)
{
	None			= 0x0000,			/**< */

	//CenterOffsets	= 0x0001,			/**< */
	LeftOffsets		= 0x0002,			/**< */
	TopOffsets		= 0x0004,			/**< */
	RightOffsets	= 0x0008,			/**< */
	BottomOffsets	= 0x0010,			/**< */

	//CenterRatios	= 0x0020,			/**< */
	LeftRatios		= 0x0040,			/**< */
	TopRatios		= 0x0080,			/**< */
	RightRatios		= 0x0100,			/**< */
	BottomRatios	= 0x0200,			/**< */

	HCenter = 0x0400,			/**< */
	VCenter = 0x0800,			/**< */
};
LN_ENUM_FLAGS_DECLARE(AlignmentAnchor);

/** コントロールのレイアウト方向を示します。*/
LN_ENUM(Orientation)
{
	Horizontal = 0,		/**< 水平方向に配置します。*/
	Vertical,			/**< 垂直方向に配置します。*/
	ReverseHorizontal,	/**< 水平方向（右から左）に配置します。*/
	ReverseVertical,	/**< 垂直方向（下から上）に配置します。*/
};
LN_ENUM_DECLARE(Orientation);

/** コンテンツのサイズを変更する方法を示します。*/
enum class StretchMode
{
	/** コンテンツの現在のサイズを維持します。*/
	None,

	/** 割り当てられた広さを埋めるようにコンテンツのサイズを変更します。 縦横比は維持されません。*/
	Fill,

	/** 割り当てられた広さに収まるようにコンテンツのサイズを変更しますが、元の縦横比が維持されます。*/
	Uniform,

	/** 割り当てられた広さを埋めるようにコンテンツのサイズを変更しますが、元の縦横比が維持されます。 ソース コンテンツの縦横比が対象の四角形の縦横比と異なる場合は、ソース コンテンツが対象の四角形に収まるように切り取られます。*/
	UniformToFill,
};

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

//struct AnchorInfo
//{
//	Vector2	left;
//	Vector2	top;
//	Vector2	right;
//	Vector2	bottom;
//	Vector2	center;
//};

LN_ENUM_FLAGS(InvalidateFlags)
{
	None = 0x0000,
	layout = 0x0001,		// レイアウト系プロパティに変更がある
	Font = 0x0002,			// フォント系プロパティに変更がある
	Rendering = 0x004,			// 描画系プロパティに変更がある
	VisualState = 0x0008,	// VisualState に変更がある

	Initializing = 0x0010,	// オブジェクト作成～最初の更新。完了したら Loaded イベント。この間にセットされるスタイルはアニメーションせず即適用。
	ParentChangedUpdating = 0x0020,

	All = 0xFFFFFFFF,
};
LN_ENUM_FLAGS_DECLARE(InvalidateFlags);

enum class SpcialUIElementType
{
	Common,
	LayoutRoot,
	PopupMenuRoot,
};

} // namespace detail

LN_NAMESPACE_END
