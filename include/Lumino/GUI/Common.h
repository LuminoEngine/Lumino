
#pragma once
#include "../Variant.h"

#define LN_NAMESPACE_GUI_BEGIN	//namespace Scene {
#define LN_NAMESPACE_GUI_END		//}

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

class GUIContext;
class RoutedEventArgs;

class CursorImage;






class GUIManagerImpl;

class ResourceDictionary;
class UIElement;
class Control;
class RootFrame;

class Storyboard;

class Style;

class ContentPresenter;
class UIButton;
class ButtonChrome;
class Image;
class StackPanel;
class Grid;
class ColumnDefinition;
class RowDefinition;
class Thumb;
class ThumbChrome;
class Track;
class ScrollBar;
class ScrollContentPresenter;
class ScrollViewer;
class ItemList;
class ItemsControl;
class ItemsPresenter;
class GroupItem;
class ListBox;
class ListBoxChrome;
class ListBoxItem;

class AnimationClock;

class VisualState;
class VisualStateGroup;

class Command;

typedef RefPtr<Style>					StylePtr;

typedef RefPtr<UIElement>				UIElementPtr;
typedef RefPtr<ContentPresenter>		ContentPresenterPtr;
//typedef RefPtr<Button>					ButtonPtr;
typedef RefPtr<ButtonChrome>			ButtonChromePtr;
typedef RefPtr<Image>					ImagePtr;
typedef RefPtr<StackPanel>				StackPanelPtr;
typedef RefPtr<Grid>					GridPtr;
typedef RefPtr<ColumnDefinition>		ColumnDefinitionPtr;
typedef RefPtr<RowDefinition>			RowDefinitionPtr;
typedef RefPtr<Thumb>					ThumbPtr;
typedef RefPtr<ThumbChrome>				ThumbChromePtr;
typedef RefPtr<Track>					TrackPtr;
typedef RefPtr<ScrollBar>				ScrollBarPtr;
typedef RefPtr<ScrollContentPresenter>	ScrollContentPresenterPtr;
typedef RefPtr<ScrollViewer>			ScrollViewerPtr;
typedef RefPtr<ItemList>				ItemListPtr;
typedef RefPtr<ItemsControl>			ItemsControlPtr;
typedef RefPtr<ItemsPresenter>			ItemsPresenterPtr;
typedef RefPtr<GroupItem>				GroupItemPtr;
typedef RefPtr<ListBox>					ListBoxPtr;
typedef RefPtr<ListBoxChrome>			ListBoxChromePtr;
typedef RefPtr<ListBoxItem>				ListBoxItemPtr;

typedef RefPtr<VisualState>				VisualStatePtr;
typedef RefPtr<VisualStateGroup>		VisualStateGroupPtr;


// internal
class CombinedLocalResource;
class VisualStateInstance;

typedef CoreObject* (*ObjectFactory)(GUIManagerImpl* manager);

/// 縦方向の表示位置を示します。
LN_ENUM(VerticalAlignment)
{
	Bottom = 0,			///< 子要素を、親のレイアウト スロットの下端に揃えて配置します。
	Center,				///< 子要素を、親のレイアウト スロットの中央に揃えて配置します。
	Stretch,			///< 子要素を、親のレイアウト スロット全体に引き伸ばします。
	Top,				///< 子要素を、親のレイアウト スロットの上端に揃えて配置します。
};
LN_ENUM_DECLARE(VerticalAlignment);

/// 横方向の表示位置を示します。
LN_ENUM(HorizontalAlignment)
{
	Left = 0,			///< 子要素を、親のレイアウト スロットの左側に揃えて配置します。
	Center,				///< 子要素を、親のレイアウト スロットの中央に揃えて配置します。
	Right,				///< 子要素を、親のレイアウト スロットの右側に揃えて配置します。
	Stretch,			///< 子要素を、親のレイアウト スロット全体に引き伸ばします。
};
LN_ENUM_DECLARE(HorizontalAlignment);

/// コントロールのレイアウト方向を示します。
LN_ENUM(Orientation)
{
	Vertical = 0,		///< 垂直方向に配置します。
	Horizontal,			///< 水平方向に配置します。
};
LN_ENUM_DECLARE(Orientation);

/** 規定のマウスカーソルイメージの種類を示します。*/
LN_ENUM(CommonCursorImage)
{
	Arrow = 0,			/**< 矢印カーソル */

	TERMINATOR,
};
LN_ENUM_REFLECTION(CommonCursorImage, Arrow)
LN_ENUM_DECLARE(CommonCursorImage);

/** 修飾キーを表します。*/
LN_ENUM_FLAGS(ModifierKeys)
{
	None	= 0x00,		/**< 修飾キー無し */
	Alt		= 0x01,		/**< Alt キー */
	Control = 0x02,		/**< Control キー */
	Shift	= 0x04,		/**< Shift キー */
};
LN_ENUM_FLAGS_DECLARE(ModifierKeys);


class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
