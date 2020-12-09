
#include <LuminoEngine.hpp>
using namespace ln;

void Example_MainLoop()
{
	ln::Engine::init();

	while (ln::Engine::update())
	{
		
	}

	ln::Engine::finalize();
}
