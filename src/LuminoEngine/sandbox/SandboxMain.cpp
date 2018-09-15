
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "Common.hpp"
#include "../src/Engine/EngineDomain.hpp"
#include "../src/Rendering/RenderingManager.hpp"
#include "../src/Rendering/SpriteRenderFeature.hpp"
using namespace ln;

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif



	GlobalLogger::addStdErrAdapter();
	//EngineSettings::setMainWindowSize(80, 60);
	//EngineSettings::setMainBackBufferSize(80, 60);
	Engine::initialize();

	auto shader = Shader::create(LN_LOCALFILE("Assets/SpriteTest.hlsl"));

	//auto shader = Shader::create(
	//	LN_LOCALFILE("Assets/simple.vert"),
	//	LN_LOCALFILE("Assets/simple.frag"));
	//shader->setVector("g_color", Vector4(0, 1, 0, 1));

	//Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);


	auto ctx = Engine::graphicsContext();
	//ctx->setColorBuffer(0, Engine::mainWindow()->swapChain()->colorBuffer());

	auto sr = detail::EngineDomain::renderingManager()->spriteRenderFeature();
	

	int loop = 0;
	while (Engine::update())
	{
		//auto pt = Mouse::position();
		//std::cout << pt.x << ", " << pt.y << std::endl;

		if (Input::isTriggered(InputButtons::Left)) {
			std::cout << "Left" << std::endl;
		}
		if (Input::isOffTriggered(InputButtons::Left)) {
			std::cout << "isOffTriggered" << std::endl;
		}
		if (Input::isRepeated(InputButtons::Left)) {
			std::cout << "isRepeated" << std::endl;
		}

		if (Mouse::isPressed(MouseButtons::Left)) {
			//std::cout << "isPressed" << std::endl;
		}
		if (Mouse::isTriggered(MouseButtons::Left)) {
			std::cout << "isTriggered" << std::endl;
		}
		if (Mouse::isOffTriggered(MouseButtons::Left)) {
			std::cout << "isOffTriggered" << std::endl;
		}
		if (Mouse::isRepeated(MouseButtons::Left)) {
			std::cout << "isRepeated" << std::endl;
		}
		ctx->setShaderPass(shader->techniques()[0]->passes()[0]);
		//ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		sr->drawRequest(Matrix(), Vector2(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::Blue, SpriteBaseDirection::ZMinus, BillboardType::None);
		sr->flush();
	}

	Engine::terminate();

	return 0;
}

