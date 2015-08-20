
#pragma once
#include "../Internal.h"

#define LN_CALL_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		Helper::GetPrimaryRenderingCommandList(m_manager)->AddCommand<command>(m_internal, __VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}
	
