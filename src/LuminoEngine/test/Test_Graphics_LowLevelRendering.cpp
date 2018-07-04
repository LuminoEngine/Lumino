#include "Common.hpp"

class Test_Graphics_LowLevelRendering : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, BasicTriangle)
{
	auto shader = Shader::create(LN_ASSETFILE("simple.vert"), LN_ASSETFILE("simple.frag"));
	shader->setVector("g_color", Vector4(1, 0, 0, 1));

	Vector4 v[] = {
		Vector4(0, 1, 0, 1),
		Vector4(-1, 1, 0, 1),
		Vector4(0, -1, 0, 1),
	};

	auto vertexBuffer = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto vertexDecl = newObject<VertexDeclaration>();
	vertexDecl->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vertexDecl);
	ctx->setVertexBuffer(0, vertexBuffer);
	ctx->setShaderPass(shader->techniques()[0]->passes()[0]);
	ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

	//Engine::update();

	auto* swapChain = Engine::mainWindow()->swapChain();
	swapChain->wait();

	auto bmp = swapChain->colorBuffer()->readData();
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

}
