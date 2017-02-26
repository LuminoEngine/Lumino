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
#include "EventArgsPool.h"
#include "UIManager.h"

// for style definition
#include <Lumino/Graphics/Brush.h>
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UIListBox.h>

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
	LN_CHECK_ARG(settings.fileManager != nullptr);
	LN_CHECK_ARG(settings.animationManager != nullptr);
	LN_CHECK_ARG(settings.platformManager != nullptr);
	LN_CHECK_ARG(settings.graphicsManager != nullptr);
	LN_CHECK_ARG(settings.assetsManager != nullptr);
	LN_CHECK_ARG(settings.mainWindow != nullptr);

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

	m_mainWindow = LN_NEW UIMainWindow();
	m_mainWindow->Initialize(this, settings.mainWindow);


	if (g_uiManager == nullptr)
	{
		g_uiManager = this;
	}
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
void UIManager::MakeDefaultStyle(UIStyleTable* table)
{
	{
		auto test = UIStyle::Create();
		test->AddValue(_T(""), UITextBlock::FontSizeId, 20);
		test->AddValue(_T(""), UITextBlock::foregroundId, ColorBrush::Black);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<UITextBlock>(), test);
	}
	// UIButton
	{
		auto brush = RefPtr<TextureBrush>::MakeRef();
		brush->Create(m_defaultSkinTexture);
		brush->SetSourceRect(0, 0, 32, 32);
		brush->SetBorderThickness(8, 8, 8, 8);
		brush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush->SetWrapMode(BrushWrapMode::Stretch);

		auto test = UIStyle::Create();
		test->AddValue(_T(""), UIElement::backgroundId, brush);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<UIButton>(), test);
	}
	{
		auto brush = RefPtr<TextureBrush>::MakeRef();
		brush->Create(m_defaultSkinTexture);
		brush->SetSourceRect(0, 32, 32, 32);
		brush->SetBorderThickness(8, 8, 8, 8);
		brush->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush->SetWrapMode(BrushWrapMode::Stretch);

		auto style = UIStyle::Create();
		style->AddValue(tr::UIListBox::NormalState, UIElement::backgroundId, brush);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<tr::UIListBox>(), style);
	}

	{
		auto style = UIStyle::Create();
		style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorBackgroundId, ColorBrush::Green);
		style->AddValue(tr::UIListBoxItem::NormalState, UIElement::decoratorOpacityId, 0.0f, 0.3);
		style->AddValue(tr::UIListBoxItem::MouseOverState, UIElement::decoratorOpacityId, 1.0f, 0.3);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<tr::UIListBoxItem>(), style);
	}
	// UIThumb
	{
		auto test = UIStyle::Create();
		test->AddValue(_T(""), UIElement::backgroundId, ColorBrush::DimGray);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<UIThumb>(), test);
	}
	// UIScrollBar
	{
		auto test = UIStyle::Create();
		//test->AddValue(_T(""), UIElement::heightId, 20);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<UIScrollBar>(), test);	// TODO: typeInfo じゃなくて名前指定したい

	}
	// UIScrollBar::Horizontal
	{
		auto test = UIStyle::Create();
		test->AddValue(_T(""), UIElement::heightId, 30);
		table->AddStyle(tr::TypeInfo::GetTypeInfo<UIScrollBar>(), _T("Horizontal"), test);	// TODO: typeInfo じゃなくて名前指定したい

	}
}


} // namespace detail
LN_NAMESPACE_END
