#pragma once
#include <LuminoEngine/Base/Event.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>

struct YGConfig;
struct YGNode;
typedef struct YGConfig* YGConfigRef;
typedef struct YGNode* YGNodeRef;
#define LN_USE_YOGA 1

namespace ln {
class Material;
class UITheme;
class UIStyleContext;
class UICommand;
class UIElement;
class UILayoutPanel;
class UIDomainProvidor;
class UIViewport;
class UIControl;
class UIListItemsControl;
class UIListBox;
class UIText;
class UIPopup;

/** コントロールのレイアウト方向を示します。*/
LN_ENUM()
enum class UILayoutOrientation
{
    /** 水平方向に配置します。*/
	Horizontal,

    /** 垂直方向に配置します。*/
	Vertical,

    /** 水平方向（右から左）に配置します。*/
	ReverseHorizontal,

    /** 垂直方向（下から上）に配置します。*/
	ReverseVertical,
};

/** UI要素の表示状態を指定します。 */
LN_ENUM()
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
class UIStyleInstance;

// TODO: UI モジュールではなく共通の場所に持っていく。WorldObject も似たような管理を行う。
enum class ObjectManagementFlags : uint8_t
{
	None = 0,
	AutoAddToPrimaryElement = 1 << 1,
};

enum class UISpecialElementFlags : uint32_t
{
    None = 0,
    MainWindow = 1 << 1,
    FrameWindow = 1 << 2,

    // 子 Visual 要素のレイアウトと描画を行わない。
    // イベントルーティングは通常通り行う。
    // 子 Visual 要素のレイアウトと描画を別の場所で行うときに使う。
    //VisualLeaf = 1 << 2,

    // 通常の論理ツリー上に配置され、レイアウトと描画は Adorner など別のフェーズで行う
    Popup = 1 << 3,

    // UIControl. 
    Control = 1 << 4,

    // 
    //LayoutInvisible = 1 << 3,

    DragDropSource = 1 << 5,
    DragDropTarget = 1 << 6,

    Enabled = 1 << 7,
    InternalEnabled = 1 << 8,

    ListItem = 1 << 9,

    FloatingSiblingOrder = 1 << 10,
};
// TODO: ↑↓統合していいかも
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
