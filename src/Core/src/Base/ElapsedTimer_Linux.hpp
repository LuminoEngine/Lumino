#pragma once

namespace ln {

//struct _timespec
//{
//	uint64_t tv_sec;
//	uint64_t tv_nsec;
//};

#if 1
static void _clock_gettime(clockid_t clock, struct timespec* ts)
{
	clock_gettime(clock, ts);
}
#else
static void _clock_gettime(int, struct timespec* ts)
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
}
#endif

static void _getTime(uint64_t* sec, uint64_t* nsec)
{
	timespec ts;
	_clock_gettime(CLOCK_REALTIME, &ts);
	*sec = ts.tv_sec;
	*nsec = ts.tv_nsec;
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
	_getTime(&m_value1, &m_value2);
}

uint64_t ElapsedTimer::elapsedNanoseconds() const LN_NOEXCEPT
{
	uint64_t sec, nsec;
	_getTime(&sec, &nsec);
	uint64_t dsec = sec - m_value1;
	uint64_t dnsec = nsec - m_value2;
	return (dsec * 1000000000) + dnsec;
}

} // namespace ln

