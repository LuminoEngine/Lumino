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


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Sample)
{
	auto shader1 = newObject<Shader>(LN_ASSETFILE("Atmosphere.fx"));

	Vector4 v1[] = {
		{ -1, 1, 1, 1 },
		{ 1, 1, 1, 1 },
		{ -1, -1, 1, 1 },
		{ 1, -1, 1, 1 },
	};
	auto vb1 = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

	auto vd1 = newObject<VertexDeclaration>();
	vd1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vd1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setIndexBuffer(nullptr);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	auto view = Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3::normalize(-0.2, 0, 1), Vector3::UnitY);
	auto proj = Matrix::makePerspectiveFovLH(Math::PI / 3, 160.0 / 120.0, 0.1, 1000);

	float r = 0;
	while (Engine::update())
	{
		r += 0.01;
		//Vector4 lightPos(cos(r), sin(r), 0, 1);
		Vector4 lightPos(0, cos(r), sin(r), 1);

		auto buf = shader1->findConstantBuffer("_Global");
		buf->findParameter("_SkyTint")->setVector(Vector4(.5, .5, .5, 1));
		buf->findParameter("_AtmosphereThickness")->setFloat(1.0);
		//buf->findParameter("_WorldSpaceLightPos0")->setVector(Vector4(0.1, 0.5, -0.8, 0));
		buf->findParameter("_WorldSpaceLightPos0")->setVector(lightPos);//Vector4(0.8, 0.8, 0.8, 0));
		buf->findParameter("_Exposure")->setFloat(1.3);
		buf->findParameter("_GroundColor")->setVector(Vector4(.369, .349, .341, 1));
		buf->findParameter("_SunSize")->setFloat(0.04);
		buf->findParameter("_LightColor0")->setVector(Vector4(1, 1, 1, 1));
		buf->findParameter("_VPInv")->setMatrix(Matrix::makeInverse(view * proj));

		DepthStencilStateDesc state1;
		state1.depthTestEnabled = false;
		state1.depthWriteEnabled = false;
		ctx->setDepthStencilState(state1);

		ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

		Engine::mainWindow()->present();
		Thread::sleep(32);
	}



	//ASSERT_SCREEN_S(LN_ASSETFILE("Result/Test_Graphics_HlslEffect-Sample-1.png"));
}
