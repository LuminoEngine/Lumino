
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	
*/
LN_MODULE(GUI)

//=============================================================================
/**
	@brief	TODO
*/
LN_CLASS(LNGUIElement)

	/**
		@brief		TODO
		@param[out]	button		: TODO
	*/
	LN_INSTANCE_API
	LN_CONSTRUCTOR
	LNResult LNGUIElement_SetSizeWH(LN_HANDLE(LNGUIRootPane)* rootPane, int width, int height);
	
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
	LN_INSTANCE_API
	LN_CONSTRUCTOR
	LNResult LNGUIRootPane_Create(LN_HANDLE(LNGUIRootPane)* rootPane);


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
	LNResult LNGUIButton_Create(LN_HANDLE(LNGUIButton)* button);


LN_CLASS_END



LN_MODULE_END

} // extern "C"

