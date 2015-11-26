
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_scene シーングラフ
	@brief		シーングラフ
	@{
*/
LN_MODULE(Scene)

//=============================================================================
/**
	@brief	シーングラフを構成するノードのベースクラスです。
*/
LN_CLASS(LNSceneNode, LNObject)

	
	
LN_CLASS_END

//=============================================================================
/**
	@brief	
*/
LN_CLASS(LNSprite, LNObject)

	
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
