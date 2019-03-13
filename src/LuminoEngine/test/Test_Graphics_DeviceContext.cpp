#include "Common.hpp"

#if 0

//==============================================================================
// IGraphicsDeviceContext 周りのテスト。
// 新規 DeviceContext 実装時の最低限の動作を見る。
// 現状内部用途なので API は決まっていないため、ここのコードはそれなりの頻度で変更する可能性がある。
// 細かい動作を見るコードは、IGraphicsDeviceContext の上位の公開 API のテストで行う。
class Test_Graphics_DeviceContext : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DeviceContext, Clear)
{
    {
        while (1)
        {
            auto ctx = Engine::graphicsContext();
            TestEnv::resetGraphicsContext(ctx);
            //ctx->setDepthBuffer(nullptr);
            ctx->clear(ClearFlags::All, Color::Blue, 1.0f, 0);
            ctx->present(Engine::mainWindow()->swapChain());
            Thread::sleep(10);
        }
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-Clear-1.png"));
    }
    {
        auto ctx = Engine::graphicsContext();
        TestEnv::resetGraphicsContext(ctx);
        ctx->setDepthBuffer(nullptr);
        ctx->clear(ClearFlags::All, Color::Red, 1.0f, 0);
        ctx->present(Engine::mainWindow()->swapChain());
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-Clear-2.png"));
    }
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DeviceContext, BasicTriangle)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Graphics/SimplePosColor.lcfx"));

    struct PosColor
    {
        Vector3 pos;
        Vector4 color;
    };
    PosColor v1[] = {
        { { 0, 0.5, 0 }, { 1, 0, 0, 1 } },
        { { 0.5, -0.5, 0 }, { 0, 1, 0, 1 } },
        { {-0.5, -0.5, 0 }, { 0, 0, 1, 1 } },
    };

	auto vertexDecl1 = newObject<VertexDeclaration>();
	vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

    auto vertexBuffer = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);


    auto ctx = Engine::graphicsContext();
    TestEnv::resetGraphicsContext(ctx);
    {
        DepthStencilStateDesc state;
        state.depthWriteEnabled = false;
        state.depthTestFunc = ComparisonFunc::Always;
        ctx->setDepthStencilState(state);
    }
    {
        RasterizerStateDesc state;
        state.cullMode = CullMode::None;
        ctx->setRasterizerState(state);
    }
    ctx->setDepthBuffer(nullptr);
    ctx->setVertexDeclaration(vertexDecl1);
    ctx->setVertexBuffer(0, vertexBuffer);
    ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    //ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);
    ctx->drawPrimitive(0, 1);
    ctx->present(Engine::mainWindow()->swapChain());
    ASSERT_SCREEN_S(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-BasicTriangle-1.png"));
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DeviceContext, IndexBuffer)
{
	auto shader1 = Shader::create(LN_ASSETFILE("Graphics/SimplePosColor.lcfx"));

	struct PosColor
	{
		Vector3 pos;
		Vector4 color;
	};
	PosColor v1[] = {
		{ { -0.5, 0.5, 0 }, { 1, 0, 0, 1 } },
		{ { 0.5, 0.5, 0 }, { 0, 1, 0, 1 } },
		{ { -0.5, -0.5, 0 }, { 0, 0, 1, 1 } },
		{ { 0.5, -0.5, 0 }, { 1, 1, 1, 1 } },
	};
	uint16_t i1[] = { 0, 1, 2, 2, 1, 3 };

	auto vertexDecl1 = newObject<VertexDeclaration>();
	vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	auto vertexBuffer = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto indexBuffer = newObject<IndexBuffer>(LN_ARRAY_SIZE_OF(i1), IndexBufferFormat::UInt16, i1, GraphicsResourceUsage::Static);

	auto ctx = Engine::graphicsContext();
	TestEnv::resetGraphicsContext(ctx);

    ctx->setDepthBuffer(nullptr);
	ctx->setVertexDeclaration(vertexDecl1);
	ctx->setVertexBuffer(0, vertexBuffer);
	ctx->setIndexBuffer(indexBuffer);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
	ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);
	ctx->drawPrimitiveIndexed(0, 2);
	ctx->present(Engine::mainWindow()->swapChain());
	ASSERT_SCREEN_S(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-IndexBuffer-1.png"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DeviceContext, ConstantBuffer)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Graphics/SimpleConstantBuffer.lcfx"));
    shader1->findParameter("_Color")->setVector(Vector4(1, 0, 0, 1));

    Vector3 v1[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.5, 0 },
        {-0.5, -0.5, 0 },
    };

    auto vertexDecl1 = newObject<VertexDeclaration>();
    vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

    auto vertexBuffer = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

    auto ctx = Engine::graphicsContext();
    TestEnv::resetGraphicsContext(ctx);
    {
        DepthStencilStateDesc state;
        state.depthWriteEnabled = false;
        state.depthTestFunc = ComparisonFunc::Always;
        ctx->setDepthStencilState(state);
    }
    {
        RasterizerStateDesc state;
        state.cullMode = CullMode::None;
        ctx->setRasterizerState(state);
    }
    ctx->setDepthBuffer(nullptr);
    ctx->setVertexDeclaration(vertexDecl1);
    ctx->setVertexBuffer(0, vertexBuffer);
    ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    //ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);
    ctx->drawPrimitive(0, 1);
    ctx->present(Engine::mainWindow()->swapChain());
    ASSERT_SCREEN_S(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-ConstantBuffer-1.png"));
}

#endif