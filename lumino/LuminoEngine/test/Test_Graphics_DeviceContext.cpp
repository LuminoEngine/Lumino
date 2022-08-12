#include "Common.hpp"

#if 0

//==============================================================================
// IGraphicsDevice 周りのテスト。
// 新規 DeviceContext 実装時の最低限の動作を見る。
// 現状内部用途なので API は決まっていないため、ここのコードはそれなりの頻度で変更する可能性がある。
// 細かい動作を見るコードは、IGraphicsDevice の上位の公開 API のテストで行う。
class Test_Graphics_DeviceContext : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DeviceContext, Clear)
{
    auto shader1 = makeObject_deprecated<Shader>(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Graphics/Resource/VulkanSampleDeviceContext_26_shader_depth.lcfx");

    auto texture1 = makeObject_deprecated<Texture2D>(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Graphics/Resource/texture.jpg");

    struct Vertex {
        Vector3 pos;
        Vector3 color;
        Vector2 texCoord;
    };

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    auto vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * vertices.size(), vertices.data(), GraphicsResourceUsage::Static);
    auto indexBuffer = makeObject_deprecated<IndexBuffer>(indices.size(), IndexBufferFormat::UInt16, indices.data(), GraphicsResourceUsage::Static);

    VertexElement elements[] = {
        { 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float3, VertexElementUsage::Color, 0 },
        { 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
    };
    auto vertexDeclaration = makeObject_deprecated<VertexLayout>();
    vertexDeclaration->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
    vertexDeclaration->addElement(0, VertexElementType::Float3, VertexElementUsage::Color, 0);
    vertexDeclaration->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);


    {
        while (1)
        {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            Matrix model = Matrix::makeRotationY(time * 0.01);  //glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
#ifdef LN_COORD_RH
            Matrix view = Matrix::makeLookAtRH(Vector3(2.0f, 2.0f, 2.0f), Vector3::Zero, Vector3::UnitZ);//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            Matrix proj = Matrix::makePerspectiveFovRH(0.3, 160.0 / 120.0, 0.1f, 10.0f);// glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
#else
            Matrix view = Matrix::makeLookAtLH(Vector3(2.0f, 2.0f, 2.0f), Vector3::Zero, Vector3::UnitZ);//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            Matrix proj = Matrix::makePerspectiveFovLH(0.3, 160.0 / 120.0, 0.1f, 10.0f);// glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
#endif
            //model.transpose();
            //view.transpose();
            //proj.transpose();
            shader1->findParameter(u"model")->setMatrix(model);
            shader1->findParameter(u"view")->setMatrix(view);
            shader1->findParameter(u"proj")->setMatrix(proj);

            shader1->findParameter(u"g_texture1")->setTexture(texture1);
            shader1->findParameter(u"g_samplerState1")->setTexture(texture1);



            auto ctx = TestEnv::graphicsContext();
            TestEnv::resetGraphicsContext(ctx);
            //ctx->setDepthBuffer(nullptr);
            //ctx->clear(ClearFlags::All, Color::Blue, 1.0f, 0);
            ctx->setVertexDeclaration(vertexDeclaration);
            ctx->setVertexBuffer(0, vertexBuffer);
            ctx->setIndexBuffer(indexBuffer);
            ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
            ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
            ctx->drawPrimitiveIndexed(0, 4);
            ctx->present(Engine::mainWindow()->swapChain());
            Thread::sleep(10);
        }
        ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_DeviceContext-Clear-1.png"));
    }
    {
        auto ctx = TestEnv::graphicsContext();
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

	auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

    auto vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);


    auto ctx = TestEnv::graphicsContext();
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

	auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	auto vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto indexBuffer = makeObject_deprecated<IndexBuffer>(LN_ARRAY_SIZE_OF(i1), IndexBufferFormat::UInt16, i1, GraphicsResourceUsage::Static);

	auto ctx = TestEnv::graphicsContext();
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

    auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

    auto vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

    auto ctx = TestEnv::graphicsContext();
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