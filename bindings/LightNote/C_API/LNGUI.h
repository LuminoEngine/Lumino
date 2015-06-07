
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

extern "C" {

/**
	@brief	
*/
LN_MODULE(GUI)

//=============================================================================
/**
	@brief	TODO
*/
LN_CLASS(LNEventArgs)

	/**
		@brief		TODO
		@param[in]	button		: TODO
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNEventArgs_GetHandlerOwner(LN_HANDLE(LNEventArgs) hEventArgs, LN_OUT LN_HANDLE(LNObject)* hObject);

LN_CLASS_END

//=============================================================================
/**
	@brief	TODO
*/
LN_CLASS(LNMouseEventArgs)
LN_CLASS_END

//=============================================================================
/**
	@brief	TODO
*/
LN_CLASS(LNGUIElement)

	/**
		@brief		TODO
		@param[in]	button		: TODO
	*/
	LN_INSTANCE_API
	LNResult LNGUIElement_SetSizeWH(LN_HANDLE(LNGUIElement) element, int width, int height);

	/**
		@brief		TODO
		@param[in]	element		: TODO
		@param[in]	handler		: 
	*/
	LN_INSTANCE_API
	LN_EVENT
	LNResult LNGUIElement_AddMouseMoveEventHandler(LN_HANDLE(LNGUIElement) element, LNMouseEventHandler handler);

	/**
		@brief		TODO
		@param[in]	element		: TODO
		@param[in]	handler		: 
	*/
	LN_INSTANCE_API
	LN_EVENT
	LNResult LNGUIElement_RemoveMouseMoveEventHandler(LN_HANDLE(LNGUIElement) element, LNMouseEventHandler handler);
	

LN_CLASS_END

//=============================================================================
/**
@brief	TODO
*/
LN_CLASS(LNGUIControl, LNGUIElement)

LN_CLASS_END

//=============================================================================
/**
@brief	TODO
*/
LN_CLASS(LNGUIContentControl, LNGUIElement)

	/**
		@brief		TODO
		@param[out]	contentControl		: TODO
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNGUIContentControl_SetContent(LN_HANDLE(LNGUIContentControl) contentControl, LNVariant* value);

LN_CLASS_END

//=============================================================================
/**
@brief	TODO
*/
LN_CLASS(LNGUIRootPane, LNGUIContentControl)

	/**
		@brief		TODO
		@param[out]	button		: TODO
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNGUIRootPane_GetDefaultRootPane(LN_OUT LN_HANDLE(LNGUIRootPane)* rootPane);


LN_CLASS_END

//=============================================================================
/**
@brief	TODO
*/
LN_CLASS(LNGUIButton, LNGUIContentControl)

	/**
		@brief		TODO
		@param[out]	button		: TODO
	*/
	LN_INSTANCE_API
	LN_CONSTRUCTOR
	LNResult LNGUIButton_Create(LN_OUT LN_HANDLE(LNGUIButton)* button);


LN_CLASS_END



LN_MODULE_END

} // extern "C"

