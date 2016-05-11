
#pragma once
#include "LNInternal.h"
#include "LNGUI.h"

#if 0
//==============================================================================
// LNEventArgs
//==============================================================================

LN_TYPE_INFO_IMPL(LNEventArgs, RoutedEventArgs);

//LN_API LNResult LNMouseEventHandler_CreateInternal(LN_HANDLE(LNMouseEventHandler)* handler, GUI::MouseEventArgs* e)
//{
//
//}

/**
@brief		TODO
@param[in]	button		: TODO
*/
//LN_INSTANCE_API
//LN_ATTR_PROPERTY
//LNResult LNEventArgs_GetHandlerOwner(LN_HANDLE(LNEventArgs) hEventArgs, LN_OUT LN_HANDLE(LNObject)* hObject);
//LN_API LNResult LNEventArgs_GetHandlerOwner(LN_HANDLE(LNEventArgs) hEventArgs, LN_OUT LN_HANDLE(LNObject)* hObject)
//{
//	LN_CHECK_ARG_HANDLE(hEventArgs);
//	LN_CHECK_ARG_HANDLE(hObject);
//	*hObject = LFManager::CheckRegisterObject(TO_REFOBJ(GUI::EventArgs, hEventArgs)->HandlerOwner);
//	return ::LN_OK;
//}
//

//==============================================================================
// LNMouseEventArgs
//==============================================================================

LN_TYPE_INFO_IMPL(LNMouseEventArgs, GUI::MouseEventArgs);

//==============================================================================
// LNGUIElement
//==============================================================================


class LNGUIElementIF : public GUI::UIElement
{
public:
	LNGUIElementIF(GUI::GUIManager* manager)
		: GUI::UIElement(manager)
	{
		MouseMove.AddHandler(LN_CreateDelegate(EventWrap_MouseMove));
		MouseEnter.AddHandler(LN_CreateDelegate(EventWrap_MouseEnter));
		MouseLeave.AddHandler(LN_CreateDelegate(EventWrap_MouseLeave));
		m_LNMouseMoveHandler = NULL;
		m_LNMouseEnterHandler = NULL;
		m_LNMouseLeaveHandler = NULL;
	}
	LNMouseEventHandler	m_LNMouseMoveHandler;
	LNMouseEventHandler	m_LNMouseEnterHandler;
	LNMouseEventHandler	m_LNMouseLeaveHandler;

#define LN_CALLBACK_IMPL_LNGUIElementIF
	// コンストラクタでセットされる
	static void LN_STDCALL EventWrap_MouseMove(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseMoveHandler != NULL) {
			static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseMoveHandler(h);
		}
	}
	static void LN_STDCALL EventWrap_MouseEnter(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseEnterHandler != NULL) {
			static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseEnterHandler(h);
		}
	}
	static void LN_STDCALL EventWrap_MouseLeave(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseLeaveHandler != NULL) {
			static_cast<LNGUIElementIF*>(e->HandlerOwner)->m_LNMouseLeaveHandler(h);
		}
	}

	// Binder 側の関数ポインタをセットするために使用する。各 C_API から呼ばれる。
	virtual bool AddHandlerInternal(const String& key, void* nativeCFuncPtr)
	{
		if (key == MouseMoveEvent) { m_LNMouseMoveHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		else if (key == MouseEnterEvent) { m_LNMouseEnterHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		else if (key == MouseLeaveEvent) { m_LNMouseLeaveHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		return UIElement::AddHandlerInternal(key, nativeCFuncPtr);
	}
	virtual bool RemoveHandlerInternal(const String& key, void* nativeCFuncPtr)
	{
		// TODO
		return UIElement::RemoveHandlerInternal(key, nativeCFuncPtr);
	}
};

//------------------------------------------------------------------------------
LN_API LNResult LNGUIElement_SetSizeWH(LN_HANDLE(LNGUIRootFrame) rootPane, int width, int height)
{
	LN_CHECK_ARG_HANDLE(rootPane);
	TO_REFOBJ(GUI::RootFrame, rootPane)->SetSize(SizeF((float)width, (float)height));
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResult LNGUIElement_AddMouseMoveEventHandler(LN_HANDLE(LNGUIElement) element, LNMouseEventHandler handler)
{
	LN_CHECK_ARG_HANDLE(element);
	TO_REFOBJ(GUI::UIElement, element)->AddHandlerInternal(GUI::UIElement::MouseMoveEvent, handler);
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResult LNGUIElement_RemoveMouseMoveEventHandler(LN_HANDLE(LNGUIElement) element, LNMouseEventHandler handler)
{
	LN_CHECK_ARG_HANDLE(element);
	TO_REFOBJ(GUI::UIElement, element)->RemoveHandlerInternal(GUI::UIElement::MouseMoveEvent, handler);
	return ::LN_OK;
}

//==============================================================================
// LNGUIContentControl
//==============================================================================

//------------------------------------------------------------------------------
LN_API LNResult LNGUIContentControl_SetContent(LN_HANDLE(LNGUIContentControl) contentControl, LNVariant* value)
{
	LN_CHECK_ARG_HANDLE(contentControl);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(GUI::ContentControl, contentControl)->SetContent(TO_SAFE_VARIANT(value));
	LN_FUNC_TRY_END_RETURN;
}

//LN_API LNResult LNGUIContentControl_SetContent(LN_HANDLE(LNGUIContentControl) contentControl, LN_HANDLE(LNGUIElement) element)
//{
//	LN_CHECK_ARG_HANDLE(contentControl);
//	LN_CHECK_ARG_HANDLE(element);
//	LN_FUNC_TRY_BEGIN;
//	TO_REFOBJ(GUI::ContentControl, contentControl)->SetContent(TO_REFOBJ(GUI::UIElement, element));
//	LN_FUNC_TRY_END_RETURN;
//}

//==============================================================================
// LNGUIRootFrame
//==============================================================================

LN_TYPE_INFO_IMPL(LNGUIRootFrame, GUI::RootFrame);

//------------------------------------------------------------------------------
LN_API LNResult LNGUIRootFrame_GetDefaultRootFrame(LN_OUT LN_HANDLE(LNGUIRootFrame)* rootPane)
{
	LN_CHECK_ARG(rootPane != NULL);
	*rootPane = LFManager::CheckRegisterObject(LFManager::Engine->GetGUIManager()->GetDefaultRootFrame());
	return ::LN_OK;
}

/**
@brief		TODO
@param[out]	button		: TODO
*/
//LN_INSTANCE_API
//LN_CONSTRUCTOR
//LNResult LNGUIRootFrame_Create(LN_HANDLE(LNGUIRootFrame)* rootPane);
//------------------------------------------------------------------------------
//LN_API LNResult LNGUIRootFrame_Create(LN_HANDLE(LNGUIRootFrame)* rootPane)
//{
//	LN_FUNC_TRY_BEGIN;
//	RefPtr<GUI::RootFrame> obj(LFManager::Engine->GetGUIManager()->CreateRootFrame());
//	obj.SafeAddRef();
//	*rootPane = LFManager::CheckRegisterObject(obj);
//	LN_FUNC_TRY_END_RETURN;
//
//}


//==============================================================================
// LNGUIButton
//==============================================================================

LN_TYPE_INFO_IMPL(LNGUIButton, GUI::Button);


class LNGUIButtonIF : public GUI::Button
{
public:
	LNGUIButtonIF(GUI::GUIManager* manager)
		: GUI::Button(manager)
	{

		//AddHandler(MouseMoveEvent, LN_CreateDelegate(EventWrap_MouseMove));
		//AddHandler(MouseEnterEvent, LN_CreateDelegate(EventWrap_MouseEnter));
		//AddHandler(MouseLeaveEvent, LN_CreateDelegate(EventWrap_MouseLeave));
		MouseMove.AddHandler(LN_CreateDelegate(EventWrap_MouseMove));
		MouseEnter.AddHandler(LN_CreateDelegate(EventWrap_MouseEnter));
		MouseLeave.AddHandler(LN_CreateDelegate(EventWrap_MouseLeave));

		m_LNMouseMoveHandler = NULL;
		m_LNMouseEnterHandler = NULL;
		m_LNMouseLeaveHandler = NULL;

		m_overrideRender = NULL;
	}
	LNMouseEventHandler	m_LNMouseMoveHandler;
	LNMouseEventHandler	m_LNMouseEnterHandler;
	LNMouseEventHandler	m_LNMouseLeaveHandler;

#define LN_CALLBACK_IMPL_LNGUIElementIF
	// コンストラクタでセットされる
	static void LN_STDCALL EventWrap_MouseMove(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseMoveHandler != NULL) {
			static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseMoveHandler(h);
		}
	}
	static void LN_STDCALL EventWrap_MouseEnter(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseEnterHandler != NULL) {
			static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseEnterHandler(h);
		}
	}
	static void LN_STDCALL EventWrap_MouseLeave(GUI::MouseEventArgs* e)
	{
		LNHandle h = LFManager::CheckRegisterObject(e);
		if (static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseLeaveHandler != NULL) {
			static_cast<LNGUIButtonIF*>(e->HandlerOwner)->m_LNMouseLeaveHandler(h);
		}
	}


	// Binder 側の関数ポインタをセットするために使用する。各 C_API から呼ばれる。
	virtual bool AddHandlerInternal(const String& key, void* nativeCFuncPtr)
	{
		if (key == MouseMoveEvent) { m_LNMouseMoveHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		else if (key == MouseEnterEvent) { m_LNMouseEnterHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		else if (key == MouseLeaveEvent) { m_LNMouseLeaveHandler = reinterpret_cast<LNMouseEventHandler>(nativeCFuncPtr); return true; }
		return UIElement::AddHandlerInternal(key, nativeCFuncPtr);
	}
	virtual bool RemoveHandlerInternal(const String& key, void* nativeCFuncPtr)
	{
		// TODO
		return UIElement::RemoveHandlerInternal(key, nativeCFuncPtr);
	}


	// override
	LNCallback_Void_Void m_overrideRender;
	virtual void Render()
	{
		if (m_overrideRender != NULL) 
		{
			m_overrideRender(LFManager::CheckRegisterObject(this));
			return;// ここでは基底の処理を呼び出さない。呼び出すかどうかはBinder側が決める
		}
		Button::Render();
	}

};

//------------------------------------------------------------------------------
LN_API LNResult LNGUIButton_Create(LN_HANDLE(LNGUIButton)* button)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<LNGUIButtonIF> obj(LN_NEW LNGUIButtonIF(LFManager::Engine->GetGUIManager()));
	obj.SafeAddRef();
	*button = LFManager::CheckRegisterObject(obj);
	LN_FUNC_TRY_END_RETURN;
}

#endif
