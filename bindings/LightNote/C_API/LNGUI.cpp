
#pragma once
#include "LFInternal.h"
#include "LNGUI.h"

//=============================================================================
// LNGUIElement
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGUIElement_SetSizeWH(LN_HANDLE(LNGUIRootPane) rootPane, int width, int height)
{
	LN_CHECK_ARG_HANDLE(rootPane);
	TO_REFOBJ(GUI::RootPane, rootPane)->SetSize(SizeF(width, height));
	return ::LN_OK;
}


//=============================================================================
// LNGUIElement
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGUIRootPane_Create(LN_HANDLE(LNGUIRootPane)* rootPane)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGUIButton_Create(LN_HANDLE(LNGUIButton)* button)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<GUI::Button> obj(LN_NEW GUI::Button(LFManager::Application->GetGUIManager()));
	obj.SafeAddRef();
	*button = LFManager::CheckRegisterObject(obj);
	LN_FUNC_TRY_END_RETURN;

}