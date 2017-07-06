#include <Lumino.h>
using namespace ln;

void Main_DebugAndTestSupport()
{
	Engine::getWorld3D()->setVisibleGridPlane(true);


	//auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::makeRef();
	//CameraComponent::getMain3DCamera()->setCameraBehavior(cb);

	auto b = newObject<CameraMouseMoveBehavior>();
	Engine::getCamera3D()->addComponent(b);


	while (Engine::update())
	{
		//printf("----\n");
	}
}

