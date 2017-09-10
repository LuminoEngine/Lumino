/*
	Skin Texture
		- Button Background (Normal)
		- Button Background (On mouse)
		- Button Background (Push down)
		- Button Decorator (Focused)
*/
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/UI/UICommands.h>
#include "EventArgsPool.h"
#include "UIManager.h"
#include "../Platform/PlatformManager.h"
#include "UIHelper.h"

// for style definition
#include <Lumino/Graphics/Brush.h>
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UIScrollViewer.h>

LN_NAMESPACE_BEGIN
namespace detail
{
	
//==============================================================================
// UIManager
//==============================================================================
static UIManager* g_uiManager = nullptr;
const float UIManager::MouseButtonClickTimeout = 0.3f;

//------------------------------------------------------------------------------
UIManager* UIManager::getInstance(UIManager* priority)
{
	return (priority != nullptr) ? priority : g_uiManager;
}

//------------------------------------------------------------------------------
UIManager::UIManager()
	: m_eventArgsPool(nullptr)
	, m_fileManager(nullptr)
	, m_animationManager(nullptr)
	, m_platformManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_assetsManager(nullptr)
	, m_defaultStyleTable(nullptr)
	, m_mainWindow(nullptr)
	, m_mouseCapturedElement(nullptr)
{
}

//------------------------------------------------------------------------------
UIManager::~UIManager()
{
}

//------------------------------------------------------------------------------
void UIManager::initialize(const Settings& settings)
{
	if (LN_REQUIRE(settings.fileManager != nullptr)) return;
	if (LN_REQUIRE(settings.animationManager != nullptr)) return;
	if (LN_REQUIRE(settings.platformManager != nullptr)) return;
	if (LN_REQUIRE(settings.graphicsManager != nullptr)) return;
	if (LN_REQUIRE(settings.assetsManager != nullptr)) return;

	m_eventArgsPool = LN_NEW EventArgsPool();
	m_fileManager = settings.fileManager;
	m_animationManager = settings.animationManager;
	m_platformManager = settings.platformManager;
	m_graphicsManager = settings.graphicsManager;
	m_assetsManager = settings.assetsManager;

	// default UI skin
	if (settings.defaultSkinFilePath.isEmpty())
	{
		static const byte_t defaultSkin_data[] =
		{
#include "Resource/DefaultSkin.png.h"
		};
		MemoryStream defaultSkinStream(defaultSkin_data, LN_ARRAY_SIZE_OF(defaultSkin_data));
		m_defaultSkinTexture = Ref<Texture2D>::makeRef();
		m_defaultSkinTexture = newObject<Texture2D>(&defaultSkinStream, TextureFormat::R8G8B8A8, false);
	}
	else
	{
		m_defaultSkinTexture = newObject<Texture2D>(settings.defaultSkinFilePath, TextureFormat::R8G8B8A8, false);
	}

	m_defaultStyleTable = LN_NEW UIStyleTable();
	makeDefaultStyle(m_defaultStyleTable);

	if (g_uiManager == nullptr)
	{
		g_uiManager = this;
	}

	createGlobalObjects();
}

//------------------------------------------------------------------------------
void UIManager::dispose()
{
	auto deleteList = m_windows;
	for (auto& p : deleteList)
	{
		p->dispose();
	}

	LN_SAFE_RELEASE(m_mainWindow);
	LN_SAFE_RELEASE(m_defaultStyleTable);
	LN_SAFE_DELETE(m_eventArgsPool);

	if (g_uiManager == this)
	{
		g_uiManager = nullptr;
	}
}

//------------------------------------------------------------------------------
void UIManager::createGameModeMainFrame(World2D* defaultWorld2D, World3D* defaultWorld3D)
{
	if (LN_REQUIRE(m_mainWindow == nullptr)) return;

	m_mainWindow = LN_NEW UIMainWindow();
	m_mainWindow->initialize(m_platformManager->getMainWindow(), defaultWorld2D, defaultWorld3D);
}

//------------------------------------------------------------------------------
//void UIManager::CreateWrapModeMainFrame(void* window, World2D* defaultWorld2D, World3D* defaultWorld3D)
//{
//	if (LN_REQUIRE(m_mainWindow == nullptr)) return;
//
//	m_mainWindow = LN_NEW UINativeHostWindow();
//	m_mainWindow->initialize(this, )
//}

//------------------------------------------------------------------------------
void UIManager::releaseGameModeMainFrame()
{
	// TODO: MainWindow は UIManager の中で作るのではなく Engine のほうがいい気がしてきた。
	LN_SAFE_RELEASE(m_mainWindow);
}

//------------------------------------------------------------------------------
void UIManager::captureMouse(UIElement* element)
{
	m_mouseCapturedElement = element;
}

//------------------------------------------------------------------------------
void UIManager::releaseMouseCapture(UIElement* element)
{
	m_mouseCapturedElement = nullptr;
}

//------------------------------------------------------------------------------
void UIManager::createGlobalObjects()
{
	UIEvents::MouseMoveEvent = UIEventManager::RegisterEvent();
	UIEvents::MouseDownEvent = UIEventManager::RegisterEvent();
	UIEvents::MouseUpEvent = UIEventManager::RegisterEvent();
	UIEvents::MouseWheelEvent = UIEventManager::RegisterEvent();
	UIEvents::MouseEnterEvent = UIEventManager::RegisterEvent();
	UIEvents::MouseLeaveEvent = UIEventManager::RegisterEvent();
	UIEvents::KeyDownEvent = UIEventManager::RegisterEvent();
	UIEvents::KeyUpEvent = UIEventManager::RegisterEvent();
	UIEvents::TextInputEvent = UIEventManager::RegisterEvent();
	UIEvents::CheckedEvent = UIEventManager::RegisterEvent();
	UIEvents::UncheckedEvent = UIEventManager::RegisterEvent();

	auto cmd = newObject<UIRoutedCommand>();
	m_allGlobalCommands.add(cmd);
	UIApplicationCommands::Paste = cmd;
}

//------------------------------------------------------------------------------
void UIManager::makeDefaultStyle(UIStyleTable* table)
{
	//{
	//	auto test = UIStyle::create();
	//	test->AddValue(_LT(""), UITextBlock::FontSizeId, 20);
	//	test->AddValue(_LT(""), UITextBlock::foregroundId, Brush::Black);
	//	table->AddStyle(tr::TypeInfo::getTypeInfo<UITextBlock>(), test);
	//}

	auto containerBackground = SolidColorBrush::create(Color::White, 0.3);
	auto containerItemMouseOver = SolidColorBrush::create(UIColors::getColor(UIColorIndex::LightGreen, 5), 0.5f);

	auto boarderNormalBrush = TextureBrush::create(m_defaultSkinTexture);
	boarderNormalBrush->setSourceRect(0, 32, 32, 32);
	boarderNormalBrush->getBorderThickness(8, 8, 8, 8);
	boarderNormalBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
	boarderNormalBrush->setWrapMode(BrushWrapMode::Stretch);


	// UIButton
	{
		auto buttonNormalBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonNormalBrush->setSourceRect(0, 0, 32, 32);
		buttonNormalBrush->getBorderThickness(8, 8, 8, 8);
		buttonNormalBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonNormalBrush->setWrapMode(BrushWrapMode::Stretch);

		auto buttonMouseOverBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonMouseOverBrush->setSourceRect(32, 0, 32, 32);
		buttonMouseOverBrush->getBorderThickness(8, 8, 8, 8);
		buttonMouseOverBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonMouseOverBrush->setWrapMode(BrushWrapMode::Stretch);

		auto buttonFocusedBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonFocusedBrush->setSourceRect(64, 0, 32, 32);
		buttonFocusedBrush->getBorderThickness(8, 8, 8, 8);
		buttonFocusedBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonFocusedBrush->setWrapMode(BrushWrapMode::Stretch);

		auto buttonPressedBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonPressedBrush->setSourceRect(96, 0, 32, 32);
		buttonPressedBrush->getBorderThickness(8, 8, 8, 8);
		buttonPressedBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonPressedBrush->setWrapMode(BrushWrapMode::Stretch);

		auto* s = table->getStyle(tr::TypeInfo::getTypeInfo<UIButton>()->getName());
		// base
		{
			auto* props = s->getPropertyTable();
			//st->background = Ref<Brush>::StaticCast(buttonNormalBrush);
			props->borderThickness = Thickness(1);
			props->cornerRadius = 4;

			//props->background = UIColors::getBrush(UIColorIndex::LightGreen);
		}
		// UIButton.Focused
		{
			auto* st = s->getPropertyTable(UIVisualStates::FocusedState);
			st->background = Ref<Brush>::staticCast(buttonFocusedBrush);
		}
		// UIButton.MouseOver
		{
			auto* st = s->getPropertyTable(UIVisualStates::MouseOverState);
			st->background = UIColors::getBrush(UIColorIndex::Grey, 3);//Ref<Brush>::staticCast(buttonMouseOverBrush);
		}
		// UIButton.Pressed
		{
			auto* st = s->getPropertyTable(UIVisualStates::PressedState);
			st->background = Ref<Brush>::staticCast(buttonPressedBrush);
		}
	}

	// UIToggleButton : UIButton
	{
		auto* style = table->getStyle(_LT("UIToggleButton"));
		style->setBaseOnStyle(table->getStyle(_LT("UIButton")));

		// UIToggleButton.Checked
		{
			auto* props = style->getPropertyTable(UIToggleButton::CheckedState);
			props->background = Brush::Blue;
		}
		// UIToggleButton.Unchecked
		{
			auto* props = style->getPropertyTable(UIToggleButton::UncheckedState);
			props->background = Brush::Red;
		}
	}


	// UIThumb
	{
		auto thumbBrush = TextureBrush::create(m_defaultSkinTexture);
		thumbBrush->setSourceRect(32, 64, 32, 32);
		thumbBrush->getBorderThickness(8, 8, 8, 8);
		thumbBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		thumbBrush->setWrapMode(BrushWrapMode::Stretch);

		auto* style = table->getStyle(tr::TypeInfo::getTypeInfo<UIThumb>()->getName());
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(thumbBrush);
		}
	}

	// UITrack
	{
		auto* style = table->getStyle(_LT("UITrack"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Brush::DimGray;
		}
	}
	// UITrack::DecreaseButton
	{
		auto* style = table->getSubControlStyle(_LT("UITrack"), _LT("DecreaseButton"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = nullptr;	// transparency
		}
	}
	// UITrack::IncreaseButton
	{
		auto* style = table->getSubControlStyle(_LT("UITrack"), _LT("IncreaseButton"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = nullptr;	// transparency
		}
	}

	// UIScrollBar
	{
		auto* style = table->getStyle(tr::TypeInfo::getTypeInfo<UIScrollBar>()->getName());
		// UIScrollBar
		{
			auto* props = style->getPropertyTable();
			props->background = Brush::DimGray;
		}
		// UIScrollBar.Horizontal
		{
			auto* props = style->getPropertyTable(UIScrollBar::HorizontalState);
			props->height = 16.0f;
			//props->background = Brush::Blue;
		}
		// UIScrollBar.Vertical
		{
			auto* props = style->getPropertyTable(UIScrollBar::VerticalState);
			props->width = 16.0f;
			//props->background = Brush::Black;
		}
	}
	// UIScrollBar::LineUpButton
	{
		auto buttonNormalBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonNormalBrush->setSourceRect(0, 32, 32, 32);
		buttonNormalBrush->getBorderThickness(8, 8, 8, 8);
		buttonNormalBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);

		auto buttonMouseOverBrush = TextureBrush::create(m_defaultSkinTexture);
		buttonMouseOverBrush->setSourceRect(32, 32, 32, 32);
		buttonMouseOverBrush->getBorderThickness(8, 8, 8, 8);
		buttonMouseOverBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);

		auto* style = table->getSubControlStyle(_LT("UIScrollBar"), _LT("LineUpButton"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(buttonNormalBrush);
		}
		// UIScrollBar::LineUpButton.MouseOver
		{
			auto* props = style->getPropertyTable(UIVisualStates::MouseOverState);
			props->background = Ref<Brush>::staticCast(buttonMouseOverBrush);
		}
	}
	// UIScrollBar::LineDownButton
	{
		auto* style = table->getSubControlStyle(_LT("UIScrollBar"), _LT("LineDownButton"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = nullptr;
		}
	}
	// UIListBox
	{
		auto* style = table->getStyle(_LT("UIListBox"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(containerBackground);
			props->borderThickness = Thickness(1);
		}
		// UIListBox.MouseOver
		//{
		//	auto* props = style->getPropertyTable(UIVisualStates::MouseOverState);
		//	props->background = Ref<Brush>::StaticCast(containerItemMouseOver);
		//}
	}
	// UIListBoxItem
	{
		auto* style = table->getStyle(_LT("UIListBoxItem"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->padding = Thickness(10);
		}
		// UIListBoxItem.MouseOver
		{
			auto* props = style->getPropertyTable(UIVisualStates::MouseOverState);
			props->background = Ref<Brush>::staticCast(containerItemMouseOver);
		}
	}

	// UITextField
	{
		auto* style = table->getStyle(_LT("UITextField"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(boarderNormalBrush);
		}
	}
	// UIWindow
	{
		auto* style = table->getStyle(_LT("UIWindow"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(boarderNormalBrush);
		}
	}
	// UIWindow::LineDownButton
	{
		auto* style = table->getSubControlStyle(_LT("UIWindow"), _LT("ContentHost"));
		// base
		{
			auto* props = style->getPropertyTable();
			//props->background = nullptr;
		}
	}

	// UISlider
	{
		auto* style = table->getStyle(_LT("UISlider"));
		// base
		{
			auto* props = style->getPropertyTable();
			//props->background = SolidColorBrush::Blue;
		}
	}
	// UISlider::Track
	{
		auto* style = table->getSubControlStyle(_LT("UISlider"), _LT("Track"));
		// base
		{
			auto* props = style->getPropertyTable();
			//props->background = SolidColorBrush::Gray;
		}
		// UISlider::Track.Horizontal
		{
			auto* props = style->getPropertyTable(_LT("Horizontal"));
			//props->height = 4.0f;
		}
	}
	// UISlider::Thumb
	{
		auto* style = table->getSubControlStyle(_LT("UISlider"), _LT("Thumb"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->width = 16;
			props->height = 16;
			props->cornerRadius = 8;
			props->background = UIColors::getBrush(UIColorIndex::LightGreen);
		}
	}
	// UISlider::DecreaseButton
	{
		auto* style = table->getSubControlStyle(_LT("UISlider"), _LT("DecreaseButton"));
		// base
		{
			auto* props = style->getPropertyTable();

			auto r1 = newObject<UIImageRenderElement>();
			r1->m_height = 2.0f;
			r1->m_brush = UIColors::getBrush(UIColorIndex::LightGreen);
			props->m_renderElements.add(r1);
		}
	}
	// UISlider::IncreaseButton
	{
		auto* style = table->getSubControlStyle(_LT("UISlider"), _LT("IncreaseButton"));
		// base
		{
			auto* props = style->getPropertyTable();

			auto r1 = newObject<UIImageRenderElement>();
			r1->m_height = 2.0f;
			r1->m_brush = UIColors::getBrush(UIColorIndex::Grey);
			props->m_renderElements.add(r1);
		}
	}

	//------------------------------------------------------------------------------
	// UITreeView
	{
		auto* style = table->getStyle(_LT("UITreeView"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Ref<Brush>::staticCast(containerBackground);
			props->borderThickness = Thickness(1);
		}
	}
	// UITreeViewItem
	{
		auto* style = table->getStyle(_LT("UITreeViewItem"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->padding = Thickness(3);
		}
		// UITreeViewItem.MouseOver
		{
			auto* props = style->getPropertyTable(UIVisualStates::MouseOverState);
			props->background = Ref<Brush>::staticCast(containerItemMouseOver);
		}
	}
	// UITreeViewItem::ExpanderButton
	{
		auto* style = table->getSubControlStyle(_LT("UITreeViewItem"), _LT("ExpanderButton"));
		{
			auto* props = style->getPropertyTable();
		}
		// ExpanderButton.Checked
		{
			auto* props = style->getPropertyTable(UIToggleButton::CheckedState);
			auto r1 = newObject<UIGlyphIconRenderElement>();
			r1->setGlyph(_LT("fa-angle-down"), 16);
			props->m_renderElements.add(r1);
		}
		// ExpanderButton.Unchecked
		{
			auto* props = style->getPropertyTable(UIToggleButton::UncheckedState);
			auto r1 = newObject<UIGlyphIconRenderElement>();
			r1->setGlyph(_LT("fa-angle-right"), 16);
			props->m_renderElements.add(r1);
		}
	}

	//------------------------------------------------------------------------------
	// UIContextMenu
	{
		auto* style = table->getStyle(_LT("UIContextMenu"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = Brush::White;
			props->borderThickness = 1;
			props->padding = Thickness(3);
		}
	}

	//------------------------------------------------------------------------------
	// UIMenuItem
	{
		auto* style = table->getStyle(_LT("UIMenuItem"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->padding = Thickness(3);
		}
		// UITreeViewItem.MouseOver
		{
			auto* props = style->getPropertyTable(UIVisualStates::MouseOverState);
			props->background = Ref<Brush>::staticCast(containerItemMouseOver);
		}
	}

	//{
	//	auto brush = TextureBrush::create(m_defaultSkinTexture);
	//	brush->setSourceRect(0, 32, 32, 32);
	//	brush->getBorderThickness(8, 8, 8, 8);
	//	brush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
	//	brush->setWrapMode(BrushWrapMode::Stretch);

	//	auto style = UIStyle::create();
	//	style->AddValue(tr::UIListBox::NormalState, UIElement::backgroundId, brush);
	//	table->AddStyle(tr::TypeInfo::getTypeInfo<tr::UIListBox>(), style);
	//}

	//{
	//	auto style = UIStyle::create();
 //          		style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorBackgroundId, Brush::Green);
	//	style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorOpacityId, 0.0f, 0.3);
	//	style->AddValue(tr::UIListBoxItem::MouseOverState, UIElement::decoratorOpacityId, 1.0f, 0.3);
	//	table->AddStyle(tr::TypeInfo::getTypeInfo<tr::UIListBoxItem>(), style);
	//}
	//
	

}


} // namespace detail
LN_NAMESPACE_END
