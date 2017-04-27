/*
	Skin Texture
		- Button Background (Normal)
		- Button Background (On mouse)
		- Button Background (Push down)
		- Button Decorator (Focused)
*/
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/UI/UICommands.h>
#include "EventArgsPool.h"
#include "UIManager.h"
#include "../Platform/PlatformManager.h"

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
UIManager* UIManager::GetInstance(UIManager* priority)
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
{
}

//------------------------------------------------------------------------------
UIManager::~UIManager()
{
}

//------------------------------------------------------------------------------
void UIManager::Initialize(const Settings& settings)
{
	if (LN_CHECK_ARG(settings.fileManager != nullptr)) return;
	if (LN_CHECK_ARG(settings.animationManager != nullptr)) return;
	if (LN_CHECK_ARG(settings.platformManager != nullptr)) return;
	if (LN_CHECK_ARG(settings.graphicsManager != nullptr)) return;
	if (LN_CHECK_ARG(settings.assetsManager != nullptr)) return;

	m_eventArgsPool = LN_NEW EventArgsPool();
	m_fileManager = settings.fileManager;
	m_animationManager = settings.animationManager;
	m_platformManager = settings.platformManager;
	m_graphicsManager = settings.graphicsManager;
	m_assetsManager = settings.assetsManager;

	// default UI skin
	if (settings.defaultSkinFilePath.IsEmpty())
	{
		static const byte_t defaultSkin_data[] =
		{
#include "Resource/DefaultSkin.png.h"
		};
		MemoryStream defaultSkinStream(defaultSkin_data, LN_ARRAY_SIZE_OF(defaultSkin_data));
		m_defaultSkinTexture = RefPtr<Texture2D>::MakeRef();
		m_defaultSkinTexture->Initialize(&defaultSkinStream, TextureFormat::R8G8B8A8, false);
	}
	else
	{
		m_defaultSkinTexture = RefPtr<Texture2D>::MakeRef();
		m_defaultSkinTexture->Initialize(settings.defaultSkinFilePath, TextureFormat::R8G8B8A8, false);
	}

	m_defaultStyleTable = LN_NEW UIStyleTable();
	MakeDefaultStyle(m_defaultStyleTable);

	if (g_uiManager == nullptr)
	{
		g_uiManager = this;
	}

	CreateGlobalCommands();
}

//------------------------------------------------------------------------------
void UIManager::Finalize()
{
	LN_SAFE_RELEASE(m_mainWindow);
	LN_SAFE_RELEASE(m_defaultStyleTable);
	LN_SAFE_DELETE(m_eventArgsPool);

	if (g_uiManager == this)
	{
		g_uiManager = nullptr;
	}
}

//------------------------------------------------------------------------------
void UIManager::CreateGameModeMainFrame(World2D* defaultWorld2D, World3D* defaultWorld3D)
{
	if (LN_CHECK_STATE(m_mainWindow == nullptr)) return;

	m_mainWindow = LN_NEW UIMainWindow();
	m_mainWindow->Initialize(this, m_platformManager->GetMainWindow(), defaultWorld2D, defaultWorld3D);
}

//------------------------------------------------------------------------------
//void UIManager::CreateWrapModeMainFrame(void* window, World2D* defaultWorld2D, World3D* defaultWorld3D)
//{
//	if (LN_CHECK_STATE(m_mainWindow == nullptr)) return;
//
//	m_mainWindow = LN_NEW UINativeHostWindow();
//	m_mainWindow->Initialize(this, )
//}

//------------------------------------------------------------------------------
void UIManager::ReleaseGameModeMainFrame()
{
	// TODO: MainWindow は UIManager の中で作るのではなく Engine のほうがいい気がしてきた。
	LN_SAFE_RELEASE(m_mainWindow);
}

//------------------------------------------------------------------------------
void UIManager::CreateGlobalCommands()
{
	auto cmd = NewObject<UIRoutedCommand>();
	m_allGlobalCommands.Add(cmd);
	UIApplicationCommands::Paste = cmd;
}

//------------------------------------------------------------------------------
void UIManager::MakeDefaultStyle(UIStyleTable* table)
{
	//{
	//	auto test = UIStyle::Create();
	//	test->AddValue(_T(""), UITextBlock::FontSizeId, 20);
	//	test->AddValue(_T(""), UITextBlock::foregroundId, Brush::Black);
	//	table->AddStyle(tr::TypeInfo::GetTypeInfo<UITextBlock>(), test);
	//}

	auto containerBackground = SolidColorBrush::Create(Color::White, 0.3);
	auto containerItemMouseOver = SolidColorBrush::Create(UIColors::GetColor(UIColorIndex::LightGreen, 5), 0.5f);

	auto boarderNormalBrush = TextureBrush::Create(m_defaultSkinTexture);
	boarderNormalBrush->SetSourceRect(0, 32, 32, 32);
	boarderNormalBrush->SetBorderThickness(8, 8, 8, 8);
	boarderNormalBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
	boarderNormalBrush->SetWrapMode(BrushWrapMode::Stretch);


	// UIButton
	{
		auto buttonNormalBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonNormalBrush->SetSourceRect(0, 0, 32, 32);
		buttonNormalBrush->SetBorderThickness(8, 8, 8, 8);
		buttonNormalBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonNormalBrush->SetWrapMode(BrushWrapMode::Stretch);

		auto buttonMouseOverBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonMouseOverBrush->SetSourceRect(32, 0, 32, 32);
		buttonMouseOverBrush->SetBorderThickness(8, 8, 8, 8);
		buttonMouseOverBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonMouseOverBrush->SetWrapMode(BrushWrapMode::Stretch);

		auto buttonFocusedBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonFocusedBrush->SetSourceRect(64, 0, 32, 32);
		buttonFocusedBrush->SetBorderThickness(8, 8, 8, 8);
		buttonFocusedBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonFocusedBrush->SetWrapMode(BrushWrapMode::Stretch);

		auto buttonPressedBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonPressedBrush->SetSourceRect(96, 0, 32, 32);
		buttonPressedBrush->SetBorderThickness(8, 8, 8, 8);
		buttonPressedBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonPressedBrush->SetWrapMode(BrushWrapMode::Stretch);

		auto* s = table->GetStyle(tr::TypeInfo::GetTypeInfo<UIButton>()->GetName());
		// base
		{
			auto* props = s->GetPropertyTable();
			//st->background = RefPtr<Brush>::StaticCast(buttonNormalBrush);
			props->borderThickness = ThicknessF(1);
		}
		// UIButton.Focused
		{
			auto* st = s->GetPropertyTable(UIVisualStates::FocusedState);
			st->background = RefPtr<Brush>::StaticCast(buttonFocusedBrush);
		}
		// UIButton.MouseOver
		{
			auto* st = s->GetPropertyTable(UIVisualStates::MouseOverState);
			st->background = RefPtr<Brush>::StaticCast(buttonMouseOverBrush);
		}
		// UIButton.Pressed
		{
			auto* st = s->GetPropertyTable(UIVisualStates::PressedState);
			st->background = RefPtr<Brush>::StaticCast(buttonPressedBrush);
		}
	}

	// UIToggleButton : UIButton
	{
		auto* style = table->GetStyle(_T("UIToggleButton"));
		style->SetBaseOnStyle(table->GetStyle(_T("UIButton")));

		// UIToggleButton.Checked
		{
			auto* props = style->GetPropertyTable(UIToggleButton::CheckedState);
			props->background = Brush::Blue;
		}
		// UIToggleButton.Unchecked
		{
			auto* props = style->GetPropertyTable(UIToggleButton::UncheckedState);
			props->background = Brush::Red;
		}
	}


	// UIThumb
	{
		auto thumbBrush = TextureBrush::Create(m_defaultSkinTexture);
		thumbBrush->SetSourceRect(32, 64, 32, 32);
		thumbBrush->SetBorderThickness(8, 8, 8, 8);
		thumbBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		thumbBrush->SetWrapMode(BrushWrapMode::Stretch);

		auto* style = table->GetStyle(tr::TypeInfo::GetTypeInfo<UIThumb>()->GetName());
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = RefPtr<Brush>::StaticCast(thumbBrush);
		}
	}

	// UITrack
	{
		auto* style = table->GetStyle(_T("UITrack"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = Brush::DimGray;
		}
	}
	// UITrack::DecreaseButton
	{
		auto* style = table->GetSubControlStyle(_T("UITrack"), _T("DecreaseButton"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = nullptr;	// transparency
		}
	}
	// UITrack::IncreaseButton
	{
		auto* style = table->GetSubControlStyle(_T("UITrack"), _T("IncreaseButton"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = nullptr;	// transparency
		}
	}

	// UIScrollBar
	{
		auto* style = table->GetStyle(tr::TypeInfo::GetTypeInfo<UIScrollBar>()->GetName());
		// UIScrollBar
		{
			auto* props = style->GetPropertyTable();
			props->background = Brush::DimGray;
		}
		// UIScrollBar.Horizontal
		{
			auto* props = style->GetPropertyTable(UIScrollBar::HorizontalState);
			props->height = 16.0f;
			//props->background = Brush::Blue;
		}
		// UIScrollBar.Vertical
		{
			auto* props = style->GetPropertyTable(UIScrollBar::VerticalState);
			props->width = 16.0f;
			//props->background = Brush::Black;
		}
	}
	// UIScrollBar::LineUpButton
	{
		auto buttonNormalBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonNormalBrush->SetSourceRect(0, 32, 32, 32);
		buttonNormalBrush->SetBorderThickness(8, 8, 8, 8);
		buttonNormalBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);

		auto buttonMouseOverBrush = TextureBrush::Create(m_defaultSkinTexture);
		buttonMouseOverBrush->SetSourceRect(32, 32, 32, 32);
		buttonMouseOverBrush->SetBorderThickness(8, 8, 8, 8);
		buttonMouseOverBrush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);

		auto* style = table->GetSubControlStyle(_T("UIScrollBar"), _T("LineUpButton"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = RefPtr<Brush>::StaticCast(buttonNormalBrush);
		}
		// UIScrollBar::LineUpButton.MouseOver
		{
			auto* props = style->GetPropertyTable(UIVisualStates::MouseOverState);
			props->background = RefPtr<Brush>::StaticCast(buttonMouseOverBrush);
		}
	}
	// UIScrollBar::LineDownButton
	{
		auto* style = table->GetSubControlStyle(_T("UIScrollBar"), _T("LineDownButton"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = nullptr;
		}
	}
	// UIListBox
	{
		auto* style = table->GetStyle(_T("UIListBox"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = RefPtr<Brush>::StaticCast(containerBackground);
			props->borderThickness = ThicknessF(1);
		}
		// UIListBox.MouseOver
		//{
		//	auto* props = style->GetPropertyTable(UIVisualStates::MouseOverState);
		//	props->background = RefPtr<Brush>::StaticCast(containerItemMouseOver);
		//}
	}
	// UIListBoxItem
	{
		auto* style = table->GetStyle(_T("UIListBoxItem"));
		// base
		{
			auto* props = style->GetPropertyTable();
		}
		// UIListBoxItem.MouseOver
		{
			auto* props = style->GetPropertyTable(UIVisualStates::MouseOverState);
			props->background = RefPtr<Brush>::StaticCast(containerItemMouseOver);
		}
	}

	// UITextField
	{
		auto* style = table->GetStyle(_T("UITextField"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = RefPtr<Brush>::StaticCast(boarderNormalBrush);
		}
	}
	// UIWindow
	{
		auto* style = table->GetStyle(_T("UIWindow"));
		// base
		{
			auto* props = style->GetPropertyTable();
			props->background = RefPtr<Brush>::StaticCast(boarderNormalBrush);
		}
	}
	// UIWindow::LineDownButton
	{
		auto* style = table->GetSubControlStyle(_T("UIWindow"), _T("ContentHost"));
		// base
		{
			auto* props = style->GetPropertyTable();
			//props->background = nullptr;
		}
	}

	//{
	//	auto brush = TextureBrush::Create(m_defaultSkinTexture);
	//	brush->SetSourceRect(0, 32, 32, 32);
	//	brush->SetBorderThickness(8, 8, 8, 8);
	//	brush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
	//	brush->SetWrapMode(BrushWrapMode::Stretch);

	//	auto style = UIStyle::Create();
	//	style->AddValue(tr::UIListBox::NormalState, UIElement::backgroundId, brush);
	//	table->AddStyle(tr::TypeInfo::GetTypeInfo<tr::UIListBox>(), style);
	//}

	//{
	//	auto style = UIStyle::Create();
 //          		style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorBackgroundId, Brush::Green);
	//	style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorOpacityId, 0.0f, 0.3);
	//	style->AddValue(tr::UIListBoxItem::MouseOverState, UIElement::decoratorOpacityId, 1.0f, 0.3);
	//	table->AddStyle(tr::TypeInfo::GetTypeInfo<tr::UIListBoxItem>(), style);
	//}
	//
	

}


} // namespace detail
LN_NAMESPACE_END
