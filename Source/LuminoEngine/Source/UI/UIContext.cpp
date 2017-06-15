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
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIContext
//==============================================================================

//------------------------------------------------------------------------------
UIContext* UIContext::getMainContext()
{
	return detail::UIManager::getInstance()->getMainWindow()->getMainUIContext();
}

//------------------------------------------------------------------------------
UIContext::UIContext()
	: m_manager(nullptr)
	, m_rootStyleTable(nullptr)
	//, m_mainWindowView(nullptr)
	, m_focusElement(nullptr)
{
}

//------------------------------------------------------------------------------
UIContext::~UIContext()
{
	//LN_SAFE_RELEASE(m_mainWindowView);
	LN_SAFE_RELEASE(m_rootStyleTable);
}

//------------------------------------------------------------------------------
void UIContext::initialize(detail::UIManager* manager)
{
	m_manager = manager;

	LN_REFOBJ_SET(m_rootStyleTable, m_manager->getDefaultStyleTable());

	//m_mainWindowView = LN_NEW UILayoutView();
	//m_mainWindowView->initialize(this, m_manager->getMainWindow()->getPlatformWindow());
}

//------------------------------------------------------------------------------
void UIContext::setFocusElement(UIElement* element)
{
	if (element != nullptr)
	{
		if (LN_CHECK_STATE(element->isFocusable())) return;
	}

	UIElement* focusedBranchRoot = UIHelper::findVisualAncestor(element, true, [](UIElement* e) { return e->hasFocus() || e->getSpcialUIElementType() == detail::SpcialUIElementType::LayoutRoot; });
	if (LN_CHECK_STATE(focusedBranchRoot != nullptr)) return;
	
	if (m_focusElement != nullptr)
	{
		if (m_focusElement->isFocusable() && m_focusElement->hasFocus()) m_focusElement->callOnLostFocus();
		UIHelper::findVisualAncestor(m_focusElement, false, [focusedBranchRoot](UIElement* e)
		{
			if (e == focusedBranchRoot) return true;
			if (e->isFocusable() && e->hasFocus()) e->callOnLostFocus();
			return false;
		});
	}

	if (element->isFocusable() && !element->hasFocus()) element->callOnGotFocus();
	UIHelper::findVisualAncestor(element, false, [focusedBranchRoot](UIElement* e)
	{
		if (e == focusedBranchRoot) return true;
		if (e->isFocusable() && !e->hasFocus()) e->callOnGotFocus();
		return false;
	});

	// 初回用
	if (!focusedBranchRoot->hasFocus())
	{
		focusedBranchRoot->callOnGotFocus();
	}

	m_focusElement = element;


	//if (m_focusElement != element)
	//{
	//	if (m_focusElement != nullptr)
	//	{
	//		m_focusElement->callOnLostFocus();
	//	}

	//	m_focusElement = element;

	//	if (m_focusElement != nullptr)
	//	{
	//		m_focusElement->callOnGotFocus();
	//	}
	//}
}

//------------------------------------------------------------------------------
void UIContext::injectElapsedTime(float elapsedTime)
{
}

//------------------------------------------------------------------------------
//void UIContext::render()
//{
//	//auto* g = m_manager->getGraphicsManager()->GetGraphicsContext();
//	//auto* d = g->BeginDrawingContext();
//
//	//d->SetViewProjection(Matrix::Identity, Matrix::Perspective2DLH(640, 480, 0, 1));
//
//	//d->drawRectangle(RectF(10, 10, 20, 30), ColorF::Red);
//
//	//g->Flush();
//}

LN_NAMESPACE_END
