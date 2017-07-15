#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(Support, DebugAndTestSupport, Sample1)
{
	Engine::getWorld3D()->setVisibleGridPlane(true);


	//auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::makeRef();
	//CameraComponent::getMain3DCamera()->setCameraBehavior(cb);

	auto b = newObject<CameraMouseMoveBehavior>();
	Engine::getCamera3D()->addComponent(b);


	while (LN_SAMPLE_UPDATE)
	{
		//printf("----\n");
	}
}

