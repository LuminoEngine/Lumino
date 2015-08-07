
#pragma once
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{
class GUIManager;

class ResourceDictionary;
class UIElement;
class Control;
class RootFrame;

class Storyboard;


class ContentPresenter;
class Button;
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
class ItemsPresenter;
class ListBox;
class ListBoxChrome;

typedef RefPtr<ContentPresenter>		ContentPresenterPtr;
typedef RefPtr<Button>					ButtonPtr;
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
typedef RefPtr<ItemsPresenter>			ItemsPresenterPtr;
typedef RefPtr<ListBox>					ListBoxPtr;
typedef RefPtr<ListBoxChrome>			ListBoxChromePtr;

// internal
class CombinedLocalResource;
class VisualStateInstance;

typedef CoreObject* (*ObjectFactory)(GUIManager* manager);

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



class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

} // namespace GUI
} // namespace Lumino
