
#include <Lumino.h>
using namespace ln;

void Main()
{
	Engine::Initialize();

	auto sprite = Sprite2D::Create("Assets/lnicon256.png");

	while (Engine::UpdateFrame())
	{
		
	}

	Engine::Finalize();
}
