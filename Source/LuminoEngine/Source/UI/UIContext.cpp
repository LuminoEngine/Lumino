/*
	UIContext って何者？
		アプリ内のUIシステムの境界。
		例えば、普通に使う2DのUIと、3Dワールド内に独立したUIを配置したいとき。
		3Dワールド内のUIでモーダルウィンドウ出しているとき、2Dのが操作不能になるのはちょっといただけない。
*/
#include "Internal.h"
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/UI/UIStyle.h>
#include "../Graphics/GraphicsManager.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIContext
//==============================================================================

//------------------------------------------------------------------------------
UIContext* UIContext::GetMainContext()
{
	return detail::UIManager::GetInstance()->GetMainWindow()->GetMainUIContext();
}

//------------------------------------------------------------------------------
UIContext::UIContext()
	: m_manager(nullptr)
	, m_rootStyleTable(nullptr)
	, m_mainWindowView(nullptr)
	, m_focusElement(nullptr)
{
}

//------------------------------------------------------------------------------
UIContext::~UIContext()
{
	LN_SAFE_RELEASE(m_mainWindowView);
	LN_SAFE_RELEASE(m_rootStyleTable);
}

//------------------------------------------------------------------------------
void UIContext::Initialize(detail::UIManager* manager)
{
	m_manager = manager;

	LN_REFOBJ_SET(m_rootStyleTable, m_manager->GetDefaultStyleTable());

	m_mainWindowView = LN_NEW UILayoutView();
	m_mainWindowView->Initialize(this, m_manager->GetMainWindow()->GetPlatformWindow());
}

//------------------------------------------------------------------------------
void UIContext::SetFocusElement(UIElement* element)
{
	if (element != nullptr)
	{
		LN_CHECK_STATE(element->IsFocusable());
	}
	m_focusElement = element;
}

//------------------------------------------------------------------------------
void UIContext::CaptureMouse(UIElement* element)
{
	// TODO: キャプチャしている element へマウスイベントを流すようにする
}

//------------------------------------------------------------------------------
void UIContext::ReleaseMouseCapture(UIElement* element)
{
	// TODO: キャプチャしている element へマウスイベントを流すようにする
}

//------------------------------------------------------------------------------
void UIContext::InjectElapsedTime(float elapsedTime)
{
}

//------------------------------------------------------------------------------
//void UIContext::Render()
//{
//	//auto* g = m_manager->GetGraphicsManager()->GetGraphicsContext();
//	//auto* d = g->BeginDrawingContext();
//
//	//d->SetViewProjection(Matrix::Identity, Matrix::Perspective2DLH(640, 480, 0, 1));
//
//	//d->DrawRectangle(RectF(10, 10, 20, 30), ColorF::Red);
//
//	//g->Flush();
//}

LN_NAMESPACE_END
