
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include "Common.hpp"
using namespace ln;

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GlobalLogger::addStdErrAdapter();
	Engine::initialize();

	auto shader = Shader::create("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/simple.vert", "C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/simple.frag");
	shader->setVector("g_color", Vector4(1, 0, 0, 1));

	Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);

	while (Engine::update())
	{

	}

	Engine::terminate();

	return 0;
}

