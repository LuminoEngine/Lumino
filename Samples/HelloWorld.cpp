#include <Lumino.h>
using namespace ln;

namespace ln  {

	namespace detail {
		extern int g_g_calls;
	}
	extern int g_g_calls_drawBoxBackground;
}
void Main_HelloWorld()
{
	while (Engine::update())
	{
		printf("---- %d %d\n", ln::detail::g_g_calls, ln::g_g_calls_drawBoxBackground);
		//Engine::getMainWindow()->GetRenderDiagnostic()->print();
		ln::detail::g_g_calls = 0;
		ln::g_g_calls_drawBoxBackground = 0;
	}
}

