
#include <process.h>

namespace ln {
namespace detail {

class ThreadImpl
{
public:
	static void sleep(int milliseconds)
	{
		::Sleep(milliseconds);
	}
};

} // namespace detail
} // namespace ln
