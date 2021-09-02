#pragma once
#include "String.hpp"

namespace ln {
struct DateTime;

struct TimeSpan
{
public:
	TimeSpan(int hours, int minutes, int seconds);

private:
	int64_t m_span;	// min resolution = 1s (time_t)

	friend struct DateTime;
};

inline TimeSpan::TimeSpan(int hours, int minutes, int seconds)
{
	if (hours != 0 || minutes != 0) {
		LN_NOTIMPLEMENTED();
	}
	m_span = seconds;
}


struct DateTime
{
public:
	static DateTime fromEpochTime(time_t time);
	

	bool operator == (const DateTime& dateTime) const;
	bool operator != (const DateTime& dateTime) const;
	bool operator <  (const DateTime& dateTime) const;
	bool operator <= (const DateTime& dateTime) const;
	bool operator >  (const DateTime& dateTime) const;
	bool operator >= (const DateTime& dateTime) const;

	DateTime  operator +  (const TimeSpan& span) const;
	DateTime  operator -  (const TimeSpan& span) const;

private:
	DateTime(time_t t) : m_time(t) {}
	time_t	m_time;	// min resolution = 1s
};

inline DateTime DateTime::fromEpochTime(time_t time)
{
	return DateTime(time);
}

inline bool DateTime::operator == (const DateTime& dateTime) const
{
	return m_time == dateTime.m_time;
}

inline bool DateTime::operator != (const DateTime& dateTime) const
{
	return m_time != dateTime.m_time;
}

inline bool DateTime::operator < (const DateTime& dateTime) const
{
	return m_time < dateTime.m_time;
}

inline bool DateTime::operator <= (const DateTime& dateTime) const
{
	return m_time <= dateTime.m_time;
}

inline bool DateTime::operator > (const DateTime& dateTime) const
{
	return m_time > dateTime.m_time;
}

inline bool DateTime::operator >= (const DateTime& dateTime) const
{
	return m_time >= dateTime.m_time;
}

inline DateTime DateTime::operator + (const TimeSpan& span) const
{
	return DateTime(m_time + span.m_span);
}

inline DateTime DateTime::operator - (const TimeSpan& span) const
{
	return DateTime(m_time - span.m_span);
}

} // namespace ln

