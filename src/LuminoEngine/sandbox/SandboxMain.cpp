
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include "Common.hpp"
using namespace ln;

using FuncP = int(*)(int* buf, int i);

int* g_buf;
int m_format;
FuncP g_fucnc;

int readFunc(int* buf, int i)
{
	return buf[i % 2048];
}
int readFuncB(int* buf, int i)
{
	return buf[i % 512];
}

int readPixel1(int i)
{
	switch (m_format)
	{
	case 0:
		return g_buf[i % 512];
		break;
	case 5:
		return g_buf[i % 64];
		break;
	case 4:
		return g_buf[i % 32];
		break;
	case 3:
		return g_buf[i % 128];
		break;
	case 2:
		return g_buf[i % 1024];
		break;
	case 10:
		return g_buf[i % 8];
		break;
	case 9:
		return g_buf[i % 512];
		break;
	case 50:
		return g_buf[i % 64];
		break;
	case 40:
		return g_buf[i % 32];
		break;
	case 30:
		return g_buf[i % 128];
		break;
	case 20:
		return g_buf[i % 1024];
		break;
	case 66:
		return g_buf[i % 8];
		break;
	default:
		break;
	}
	//if (m_format)
	//{
	//	return g_buf[i % 2048];
	//}
	//else
	//{
	//	return g_buf[i % 512];
	//}
}

int readPixel2(int i)
{
	return g_fucnc(g_buf, i);
}

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#if 0
	g_buf = (int*)malloc(2048);

	m_format = argc;
	if (m_format) g_fucnc = readFunc;
	else g_fucnc = readFuncB;

	for (int j = 0; j < 10; j++)
	{
		int sum = 0;
		ElapsedTimer timer1;
		for (int i = 0; i < 100000; i++)
		{
			sum += readPixel1(i);
		}
		uint64_t t1 = timer1.elapsedNanoseconds();

		ElapsedTimer timer2;
		for (int i = 0; i < 100000; i++)
		{
			sum += readPixel2(i);
		}
		uint64_t t2 = timer2.elapsedNanoseconds();

		printf("%llu : %llu %d\n", t1, t2, sum);
	}

	return 0;
#endif






	GlobalLogger::addStdErrAdapter();
	EngineSettings::setMainWindowSize(80, 60);
	EngineSettings::setMainBackBufferSize(80, 60);
	Engine::initialize();

	auto shader = Shader::create("C:/Proj/GitHub/Lumino/src/LuminoEngine/sandbox/Assets/simple.vert", "C:/Proj/GitHub/Lumino/src/LuminoEngine/sandbox/Assets/simple.frag");
	shader->setVector("g_color", Vector4(0, 1, 0, 1));

	Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);

	struct Vertex
	{
		Vector4 pos;
	};
	Vertex v[] = {
		Vector4(0, 1, 0, 1),
		Vector4(-1, 1, 0, 1),
		Vector4(0, -1, 0, 1),
	};

	auto vb = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto decl = newObject<VertexDeclaration>();
	decl->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	//auto renderTarget = newObject<RenderTargetTexture>(32, 32, TextureFormat::RGBX32, false);




	auto ctx = Engine::graphicsContext();
	//ctx->setColorBuffer(0, Engine::mainWindow()->swapChain()->colorBuffer());

	int loop = 0;
	while (Engine::update())
	{

		//ctx->clear(ClearFlags::Color, Color::PaleVioletRed, 1.0, 0);
		ctx->setVertexDeclaration(decl);
		ctx->setVertexBuffer(0, vb);
		ctx->setShaderPass(shader->techniques()[0]->passes()[0]);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);


		//auto* cb = Engine::mainWindow()->swapChain()->colorBuffer();

		loop++;
		if (loop > 2) {

			auto bmp = Engine::mainWindow()->swapChain()->colorBuffer()->readData();

			for (int y = 0; y < bmp->height(); y++)
			{
				for (int x = 0; x < bmp->width(); x++)
				{
					auto c = bmp->getColor32(x, y);
					if (c.g != 248)
						printf("#");
					else
						printf("-");
				}
				printf("\n");
			}


			break;
		}

	}

	Engine::terminate();

	return 0;
}

