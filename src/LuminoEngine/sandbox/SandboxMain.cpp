
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

	auto shader = Shader::create("C:/Proj/GitHub/Lumino/src/LuminoEngine/sandbox/Assets/simple.vert", "C:/Proj/GitHub/Lumino/src/LuminoEngine/sandbox/Assets/simple.frag");
	shader->setVector("g_color", Vector4(0, 1, 0, 1));

	Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);

	struct Vertex
	{
		Vector4 pos;
	};
	Vertex v[] = {
		Vector4(0, 0.5, 0, 1),
		Vector4(-0.5, 0, 0, 1),
		Vector4(0.25, 0, 0, 1),
	};

	auto vb = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto decl = newObject<VertexDeclaration>();
	decl->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	auto renderTarget = newObject<RenderTargetTexture>(32, 32, TextureFormat::RGBX32, false);


	auto ctx = Engine::graphicsContext();
	//ctx->setColorBuffer(0, Engine::mainWindow()->swapChain()->colorBuffer());

	int loop = 0;
	while (Engine::update())
	{
		//ctx->clear(ClearFlags::Color, Color::PaleVioletRed, 1.0, 0);
		//ctx->setVertexDeclaration(decl);
		//ctx->setVertexBuffer(0, vb);
		//ctx->setShaderPass(shader->techniques()[0]->passes()[0]);
		//ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		loop++;
		if (loop > 3) {
			break;
		}
	}

	Engine::terminate();

	return 0;
}

