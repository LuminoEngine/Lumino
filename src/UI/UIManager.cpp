
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UIFrameWindow.h>
#include "EventArgsPool.h"
#include "UIManager.h"

// for style definition
#include <Lumino/UI/UITextBlock.h>

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
	LN_CHECK_ARG(settings.graphicsManager != nullptr);
	LN_CHECK_ARG(settings.assetsManager != nullptr);
	LN_CHECK_ARG(settings.mainWindow != nullptr);

	m_eventArgsPool = LN_NEW EventArgsPool();
	m_platformManager = settings.platformManager;
	m_graphicsManager = settings.graphicsManager;
	m_assetsManager = settings.assetsManager;

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
		test->AddValue(_T(""), UITextBlock::FontSizeProperty, 20);
		table->AddStyle(_T("UITextBlock"), test);
	}
	{
		auto test = UIStyle::Create();
		test->AddValue(_T(""), UIElement::BackgroundProperty, ColorBrush::Blue);
//		test->m_background = ColorBrush::Blue;
		table->AddStyle(_T("UIButton"), test);
	}
}


} // namespace detail
LN_NAMESPACE_END
