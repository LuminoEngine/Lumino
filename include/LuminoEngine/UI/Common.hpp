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
};

}

} // namespace ln
