#include "Common.hpp"

class Test_Graphics_HlslEffect : public ::testing::Test
{
public:
	virtual void SetUp() {}

	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Basic)
{
	auto shader1 = newObject<Shader>(LN_ASSETFILE("Basic.fx"));

	shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

	struct PosColor
	{
		Vector4 pos;
		Vector4 color;
	};
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 0 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 0 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 0 } }
	};
	auto vb1 = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vd1 = newObject<VertexDeclaration>();
	vd1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	vd1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vd1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
	ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

	ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_HlslEffect-Basic-1.png"));

	//* [ ] #include
	{
		auto shader2 = newObject<Shader>(LN_ASSETFILE("PreprosessorTest.fx"));
		shader2->findConstantBuffer("ConstBuff2")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

		ctx->setShaderPass(shader2->techniques()[0]->passes()[0]);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_HlslEffect-Basic-1.png"));	
	}
}

