#pragma once
#include <mach/mach_time.h>

namespace ln {

static mach_timebase_info_data_t g_gimebaseInfo = { 0, 0 };

static uint64_t ticksToNanoseconds(uint64_t tick)
{
	if (g_gimebaseInfo.denom == 0) {
		mach_timebase_info(&g_gimebaseInfo);
	}

	return tick * g_gimebaseInfo.numer / g_gimebaseInfo.denom;
}

ElapsedTimer::ElapsedTimer() LN_NOEXCEPT
	: m_value1(0)
	, m_value2(0)
	, m_tag(nullptr)
{
	start();
}

void ElapsedTimer::start() LN_NOEXCEPT
{
	m_value1 = mach_absolute_time();
}

uint64_t ElapsedTimer::elapsedNanoseconds() const LN_NOEXCEPT
{
	uint64_t tick = mach_absolute_time();
	return ticksToNanoseconds(tick - m_value1);
}

} // namespace ln
