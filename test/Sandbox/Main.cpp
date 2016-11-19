
#include <LuminoEngine.h>
using namespace ln;

void Main()
{
	Engine::Initialize();

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	while (Engine::Update())
	{

	}
}

