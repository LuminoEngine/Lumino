#include "Common.hpp"
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>

class Test_Graphics_HlslEffect : public ::testing::Test
{
public:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Basic)
{
	struct PosColor
	{
		Vector4 pos;
		Vector4 color;
	};
#ifdef LN_COORD_RH
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 1 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 1 } },
	};
#else
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 1 } },
	};
#endif
	
	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vd1 = makeObject<VertexLayout>();
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	//* [ ] Basic rendering
	{
		auto shader1 = makeObject<Shader>(LN_ASSETFILE("Basic.fx"));
		auto descriptorLayout1 = shader1->descriptorLayout();
		auto shaderPass1 = shader1->techniques()[0]->passes()[0];

		auto ctx = TestEnv::beginFrame();
		auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		auto shd = ctx->allocateShaderDescriptor(shaderPass1);
		shd->setVector(descriptorLayout1->findUniformMemberIndex(_TT("g_color")), Vector4(1, 0, 0, 1));
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vd1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setShaderPass(shaderPass1);
		ctx->setShaderDescriptor(shd);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-Basic-1.png"), cbb);
	}
	
	//* [ ] Nested struct
	{
		auto shader2 = makeObject<Shader>(LN_ASSETFILE("NestedStruct.fx"));
		auto descriptorLayout2 = shader2->descriptorLayout();
		auto shaderPass2 = shader2->techniques()[0]->passes()[0];

		auto ctx = TestEnv::beginFrame();
		auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		auto shd = ctx->allocateShaderDescriptor(shaderPass2);
		shd->setVector(descriptorLayout2->findUniformMemberIndex(_TT("g_color")), Vector4(0, 1, 0, 1));
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vd1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setShaderPass(shaderPass2);
		ctx->setShaderDescriptor(shd);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-Basic-3.png"), cbb);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Preprocess)
{
	struct PosColor
	{
		Vector4 pos;
		Vector4 color;
	};
#ifdef LN_COORD_RH
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 1 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 1 } },
	};
#else
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 1 } },
	};
#endif

	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vd1 = makeObject<VertexLayout>();
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);


	//* [ ] #if
	{
		auto props = makeObject<ShaderCompilationProperties>();
		props->addDefinition(_TT("GREEN=1"));
		auto shader2 = makeObject<Shader>(LN_ASSETFILE("PreprosessorTest2.fx"), props);
		auto shaderPass2 = shader2->techniques()[0]->passes()[0];

		auto ctx = TestEnv::beginFrame();
		auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexBuffer(0, vb1);
		ctx->setVertexLayout(vd1);
		ctx->setShaderPass(shaderPass2);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-Basic-4.png"), cbb);
	}

	//* [ ] #ifdef
	{
		auto props = makeObject<ShaderCompilationProperties>();
		props->addDefinition(_TT("BLUE"));
		auto shader2 = makeObject<Shader>(LN_ASSETFILE("PreprosessorTest2.fx"), props);
		auto shaderPass2 = shader2->techniques()[0]->passes()[0];

		auto ctx = TestEnv::beginFrame();
		auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexBuffer(0, vb1);
		ctx->setVertexLayout(vd1);
		ctx->setShaderPass(shaderPass2);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-Basic-5.png"), cbb);
	}

	//* [ ] #include
	{
		auto props = makeObject<ShaderCompilationProperties>();
		props->addIncludeDirectory(LN_ASSETFILE(""));
		auto shader2 = makeObject<Shader>(LN_ASSETFILE("PreprosessorTest.fx"), props);
		auto descriptorLayout2 = shader2->descriptorLayout();
		auto shaderPass2 = shader2->techniques()[0]->passes()[0];

		auto ctx = TestEnv::beginFrame();
		auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		auto shd = ctx->allocateShaderDescriptor(shaderPass2);
		shd->setVector(descriptorLayout2->findUniformMemberIndex(_TT("g_color")), Vector4(1, 0, 0, 1));
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexBuffer(0, vb1);
		ctx->setVertexLayout(vd1);
		ctx->setShaderPass(shaderPass2);
		ctx->setShaderDescriptor(shd);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-Basic-1.png"), cbb);	// 1 と同じ結果でよい
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, ShaderPassRenderState)
{
    auto shader = makeObject<Shader>(LN_ASSETFILE("ShaderPassRenderStateTest1.fx"));
    
    // pass0 は何もセットされていない
    // pass1 はすべてセットされている
    for (int i = 0; i < 2; i++)
    {
        auto pass = shader->techniques()[0]->passes()[i];
        auto state = detail::ShaderHelper::getShaderRenderState(pass);
        bool expect = (i == 0) ? false : true;
        ASSERT_EQ(expect, state->blendEnable.hasValue());
        ASSERT_EQ(expect, state->sourceBlend.hasValue());
        ASSERT_EQ(expect, state->destinationBlend.hasValue());
        ASSERT_EQ(expect, state->blendOp.hasValue());
        ASSERT_EQ(expect, state->sourceBlendAlpha.hasValue());
        ASSERT_EQ(expect, state->destinationBlendAlpha.hasValue());
        ASSERT_EQ(expect, state->blendOpAlpha.hasValue());

        ASSERT_EQ(expect, state->fillMode.hasValue());
        ASSERT_EQ(expect, state->cullMode.hasValue());

        ASSERT_EQ(expect, state->depthTestFunc.hasValue());
        ASSERT_EQ(expect, state->depthWriteEnabled.hasValue());

        ASSERT_EQ(expect, state->stencilEnabled.hasValue());
        ASSERT_EQ(expect, state->stencilReferenceValue.hasValue());
        ASSERT_EQ(expect, state->stencilFailOp.hasValue());
        ASSERT_EQ(expect, state->stencilDepthFailOp.hasValue());
        ASSERT_EQ(expect, state->stencilPassOp.hasValue());
        ASSERT_EQ(expect, state->stencilFunc.hasValue());
    }
}

//------------------------------------------------------------------------------
#if 0
TEST_F(Test_Graphics_HlslEffect, UnifiedShader)
{
	struct PosColor
	{
		Vector4 pos;
		Vector4 color;
	};
	PosColor v1[3] = {
		{ { -1, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { 0, 1, 0, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 0, 0, 1 },{ 0, 0, 1, 1 } },
	};

	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vd1 = makeObject<VertexLayout>();
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	auto ctx = TestEnv::graphicsContext();
	TestEnv::resetGraphicsContext(ctx);

	//* [ ] Basic rendering
	{
		auto shader = makeObject<Shader>(LN_ASSETFILE("Shader/FxcTest1.v1.lcfx"));
		shader->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(0, 1, 0, 1));

		ctx->setVertexDeclaration(vd1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setShaderPass(shader->techniques()[0]->passes()[0]);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ASSERT_SCREEN(LN_ASSETFILE("Shader/Result/Test_Graphics_HlslEffect-UnifiedShader-1.png"));
	}
}
#endif

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Sample)
{
#if 0
	auto shader1 = makeObject<Shader>(LN_ASSETFILE("Atmosphere.fx"));
	auto shader2 = makeObject<Shader>(LN_ASSETFILE("Cloud.fx"));

	Vector4 v1[] = {
		{ -1, 1, 1, 1 },
		{ 1, 1, 1, 1 },
		{ -1, -1, 1, 1 },
		{ 1, -1, 1, 1 },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

	auto vd1 = makeObject<VertexLayout>();
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	auto ctx = TestEnv::graphicsContext();
	TestEnv::resetGraphicsContext(ctx);
	ctx->setVertexDeclaration(vd1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setIndexBuffer(nullptr);


	float r = -5;
	while (Engine::update())
	{
		r += 0.05;
		//Vector4 lightPos(cos(r), sin(r), 0, 1);
		//Vector4 lightPos(0, cos(r), sin(r), 1);
		float lr = 1.6;
		Vector4 lightPos(0, cos(lr), sin(lr), 1);


		Vector4 cameraPos(0, r, 0, 1);
#ifdef LN_COORD_RH
		auto view = Matrix::makeLookAtRH(cameraPos.xyz(), cameraPos.xyz() + Vector3::normalize(-0.2, 0, 1), Vector3::UnitY);
		auto proj = Matrix::makePerspectiveFovRH(Math::PI / 3, 160.0 / 120.0, 0.1, 1000);
#else
		auto view = Matrix::makeLookAtLH(cameraPos.xyz(), cameraPos.xyz() + Vector3::normalize(-0.2, 0, 1), Vector3::UnitY);
		auto proj = Matrix::makePerspectiveFovLH(Math::PI / 3, 160.0 / 120.0, 0.1, 1000);
#endif
		ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);

		DepthStencilStateDesc state1;
		state1.depthTestEnabled = false;
		state1.depthWriteEnabled = false;
		ctx->setDepthStencilState(state1);

		{
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


			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		}

		{
			auto buf = shader2->findConstantBuffer("_Global");
			buf->findParameter("_VolumeUpper")->setFloat(20);
			buf->findParameter("_VolumeLower")->setFloat(10);
			buf->findParameter("_CameraPos")->setVector(Vector4(0, 0, 0, 0));
			buf->findParameter("_VPInv")->setMatrix(Matrix::makeInverse(view * proj));


			BlendStateDesc state1;
			state1.renderTargets[0].blendEnable = true;
			state1.renderTargets[0].sourceBlend = BlendFactor::One;
			state1.renderTargets[0].destinationBlend = BlendFactor::One;
			state1.renderTargets[0].blendOp = BlendOp::Add;
			ctx->setBlendState(state1);

			ctx->setShaderPass(shader2->techniques()[0]->passes()[0]);
			ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		}


		auto  vv = Vector4::transform(Vector4(1, 1, 1, 1), Matrix::makeInverse(view * proj));
		auto v3 = vv.xyz() / vv.w;
		auto dd = Vector3::dot(Vector3::normalize(0, 1, 1), Vector3(0, 1, 0));

		Engine::mainWindow()->present();
		Thread::sleep(32);
	}



	//ASSERT_SCREEN_S(LN_ASSETFILE("Result/Shader/Test_Graphics_HlslEffect-Sample-1.png"));
#endif
}
