
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/Common.h>

#define LN_CALL_COMMAND(func, command, ...) \
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded) { \
		m_manager->getPrimaryRenderingCommandList()->addCommand<command>(m_internal, __VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}
	
#define LN_CALL_RENDERER_COMMAND(func, command, ...) \
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded) { \
		m_primaryCommandList->addCommand<command>(__VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}

#define LN_CALL_TEXTURE_COMMAND(func, command, ...) \
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded) { \
		m_manager->getRenderer()->m_primaryCommandList->addCommand<command>(m_rhiObject, __VA_ARGS__); \
	} \
	else { \
		m_rhiObject->func(__VA_ARGS__); \
	}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_END

