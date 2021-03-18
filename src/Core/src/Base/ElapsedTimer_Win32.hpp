#pragma once

namespace ln {

static uint64_t ticksToNanoseconds(uint64_t ticks, uint64_t freq)
{
	uint64_t seconds = ticks / freq;
	uint64_t nanoSeconds = (ticks - seconds * freq) * 1000000000 / freq;
	return seconds * 1000000000 + nanoSeconds;
}

static uint64_t getElapsedNS(uint64_t start, uint64_t freq)
{
	LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);
	return ticksToNanoseconds(counter.QuadPart - start, freq);
}

ElapsedTimer::ElapsedTimer() LN_NOEXCEPT
	: m_value1(0)
	, m_value2(1)
	, m_tag(nullptr)
{
	LARGE_INTEGER freq;
	::QueryPerformanceFrequency(&freq);
	m_value2 = freq.QuadPart;
	start();
}

void ElapsedTimer::start() LN_NOEXCEPT
{
	LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);
	m_value1 = counter.QuadPart;
}

uint64_t ElapsedTimer::elapsedNanoseconds() const LN_NOEXCEPT
{
	return getElapsedNS(m_value1, m_value2);
}

} // namespace ln
