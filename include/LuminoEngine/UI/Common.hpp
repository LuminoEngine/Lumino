#pragma once

#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {

/** コントロールのレイアウト方向を示します。*/
enum class Orientation
{
	Horizontal = 0,		/**< 水平方向に配置します。*/
	Vertical,			/**< 垂直方向に配置します。*/
	ReverseHorizontal,	/**< 水平方向（右から左）に配置します。*/
	ReverseVertical,	/**< 垂直方向（下から上）に配置します。*/
};

/** UI要素の表示状態を指定します。 */
enum class UIVisibility : uint8_t
{
    /** 要素を表示します。 */
    Visible,

    /** 要素を表示しませんが、その要素の領域をレイアウト内に予約します。 */
    Hidden,

    /** 要素を表示しません。また、その要素の領域もレイアウト内に予約しません。 */
    Collapsed,
};

/** イベントのルーティング方法を指定します。 */
enum class UIEventRoutingStrategy
{
    /** 親要素へルーティングします。 */
    Bubble,

    /** ルーティングを行いません。 */
    Direct,
};

// TODO: obsolete
enum class UICollectionChangedAction
{
    Add,
    Move,
    Remove,
    Replace,

    /** コレクションの内容が大幅に変わった */
    Reset,
};


namespace detail {

// TODO: UI モジュールではなく共通の場所に持っていく。WorldObject も似たような管理を行う。
enum class ObjectManagementFlags
{
	None = 0,
	AutoAddToActiveScene = 1 << 1,
};

enum class UISpecialElementFlags
{
	None = 0,
	FrameWindow = 1 << 1,

    // 子 Visual 要素のレイアウトと描画を行わない。
    // イベントルーティングは通常通り行う。
    // 子 Visual 要素のレイアウトと描画を別の場所で行うときに使う。
    //VisualLeaf = 1 << 2,

    // 通常の論理ツリー上に配置され、レイアウトと描画は Adorner など別のフェーズで行う
    Popup = 1 << 2,

    // 
    //LayoutInvisible = 1 << 3,
};

enum class UIElementDirtyFlags
{
    None = 0,
    InitialLoading = 1 << 0,    // インスタンス作成後、最初の Loaded イベントまで
    Style = 1 << 1,
    Layout = 1 << 2,
    Render = 1 << 3,
};
LN_FLAGS_OPERATORS(UIElementDirtyFlags);

} // namespace detail

} // namespace ln
