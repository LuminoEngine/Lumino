#pragma once
#include <unistd.h>

namespace ln {
namespace detail {

class ThreadImpl
{
public:
	static void sleep(int milliseconds)
	{
		::usleep(milliseconds * 1000);
	}
};

} // namespace detail
} // namespace ln
