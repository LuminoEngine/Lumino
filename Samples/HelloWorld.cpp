#include <Lumino.h>
using namespace ln;

void Main_HelloWorld()
{
	auto text = TextBlock2D::create("Hello, Lumino!");

	while (Engine::update())
	{
		//printf("----\n");
	}
}

