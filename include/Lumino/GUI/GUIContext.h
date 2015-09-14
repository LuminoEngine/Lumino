/**
	@file	GUIContext.h	
*/
#pragma once
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
	
/**
	@brief		
*/
class GUIContext
	: public RefObject
{
public:
	
	/**
		@brief	GUIContext のインスタンスを作成します。
	*/
	static GUIContext* Create();

private:
	GUIContext(GUIManagerImpl* manager);
	virtual ~GUIContext();

private:
	GUIManagerImpl*	m_manager;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
