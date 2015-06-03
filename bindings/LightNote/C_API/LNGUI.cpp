
#pragma once
#include "LNInternal.h"
#include "LNGUI.h"

//=============================================================================
// LNGUIElement
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_API LNResult LNGUIElement_SetSizeWH(LN_HANDLE(LNGUIRootPane) rootPane, int width, int height)
{
	LN_CHECK_ARG_HANDLE(rootPane);
	TO_REFOBJ(GUI::RootPane, rootPane)->SetSize(SizeF(width, height));
	return ::LN_OK;
}

//=============================================================================
// LNGUIContentControl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_API LNResult LNGUIContentControl_SetContent(LN_HANDLE(LNGUIContentControl) contentControl, LN_HANDLE(LNGUIElement) element)
{
	LN_CHECK_ARG_HANDLE(contentControl);
	LN_CHECK_ARG_HANDLE(element);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(GUI::ContentControl, contentControl)->SetContent(TO_REFOBJ(GUI::UIElement, element));
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNGUIRootPane
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_API LNResult LNGUIRootPane_Create(LN_HANDLE(LNGUIRootPane)* rootPane)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<GUI::RootPane> obj(LFManager::Application->GetGUIManager()->CreateRootPane());
	obj.SafeAddRef();
	*rootPane = LFManager::CheckRegisterObject(obj);
	LN_FUNC_TRY_END_RETURN;

}


//=============================================================================
// LNGUIButton
//=============================================================================

class LNGUIButtonIF : public GUI::Button
{
public:


};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_API LNResult LNGUIButton_Create(LN_HANDLE(LNGUIButton)* button)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<GUI::Button> obj(LN_NEW GUI::Button(LFManager::Application->GetGUIManager()));
	obj.SafeAddRef();
	*button = LFManager::CheckRegisterObject(obj);
	LN_FUNC_TRY_END_RETURN;

}