
#include <LuminoFramework.hpp>
using namespace ln;

void Example_MainLoop()
{
	ln::Engine::initialize();

	while (ln::Engine::update())
	{
		
	}

	ln::Engine::terminate();
}
