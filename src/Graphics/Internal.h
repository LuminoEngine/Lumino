
#pragma once
#include "../Internal.h"

#define LN_CALL_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		Helper::GetPrimaryRenderingCommandList(m_manager)->AddCommand<command>(m_internal, __VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}
	
#define LN_CALL_RENDERER_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		m_primaryCommandList->AddCommand<command>(__VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}

#define LN_CALL_TEXTURE_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		m_manager->GetRenderer()->m_primaryCommandList->AddCommand<command>(m_deviceObj, __VA_ARGS__); \
	} \
	else { \
		m_deviceObj->func(__VA_ARGS__); \
	}
