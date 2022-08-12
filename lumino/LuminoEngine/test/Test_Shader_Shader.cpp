#include "Common.hpp"
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>

//==============================================================================
//# Testing Shader
class Test_Shader_Shader : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Shader_Shader, IndependentSamplerState)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/IndependentSamplerState.fx"));
    auto descriptorLayout1 = shader1->descriptorLayout();
    auto shaderPass1 = shader1->techniques()[0]->passes()[0];

    auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
    vertexDecl1->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);

    struct Vertex
    {
        Vector3 pos;
        Vector2 uv;
    };
#ifdef LN_COORD_RH
    Vertex v[] = {
        { { -1, 1, 0 }, { 0, 0 }, },	// 0.5 で中央の face からサンプリングする
        { { -1, 0, 0 }, { 0, 1 }, },
        { { 0, 1, 0 }, { 1, 0 }, },
        { { 0, 0, 0 }, { 1, 1 }, },
    };
#else
    Vertex v[] = {
        { { -1, 1, 0 }, { 0, 0 }, },	// 0.5 で中央の face からサンプリングする
        { { 0, 1, 0 }, { 1, 0 }, },
        { { -1, 0, 0 }, { 0, 1 }, },
        { { 0, 0, 0 }, { 1, 1 }, },
    };
#endif
    auto vb1 = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto tex1 = makeObject_deprecated<Texture2D>(2, 2, TextureFormat::RGBA8);
    auto bmp1 = tex1->map(MapMode::Write);
    bmp1->setPixel32(0, 0, ColorI(255, 0, 0, 255));
    bmp1->setPixel32(1, 0, ColorI(255, 0, 255, 255));
    bmp1->setPixel32(0, 1, ColorI(0, 255, 0, 255));
    bmp1->setPixel32(1, 1, ColorI(0, 0, 255, 255));

    // TODO: まだ SamplerState 直接指定をサポートしていないので Texture に対してセットする方法でテストケースだけ用意しておく。
    // 後でサポートするとき、shader1->findParameter("mySamplerState")->setSamplerState(samplerState); とかに書き換える。
    auto samplerState = makeObject_deprecated<SamplerState>();
    samplerState->setFilterMode(TextureFilterMode::Linear);
    tex1->setSamplerState(samplerState);

    shader1->findParameter(_TT("_Texture"))->setTexture(tex1);


    // * [ ] default
    {
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass1);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture")), tex1);
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setIndexBuffer(nullptr);
        ctx->setShaderPass(shaderPass1);
        ctx->setShaderDescriptor_deprecated(shd);

		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
        ctx->drawPrimitive(0, 2);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-IndependentSamplerState-1.png"), cbb);
    }
}


//------------------------------------------------------------------------------
//## UniformBuffer
TEST_F(Test_Shader_Shader, UniformBuffer)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/UniformBufferTest-1.fx"));
    auto descriptorLayout1 = shader1->descriptorLayout();
    auto shaderPass1 = shader1->techniques()[0]->passes()[0];

    auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
#ifdef LN_COORD_RH
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { -0.5, -0.25, 0 },
        { 0.5, -0.25, 0 },
    };
#else
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
#endif
    auto vb1 = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto crp = TestEnv::renderPass();
    auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass1);

    // VertexShader からのみ参照されるパラメータ
    shd->setVector(descriptorLayout1->findUniformMemberIndex(_TT("_Color1")), Vector4(1, 0, 0, 1));
    // PixelShader からのみ参照されるパラメータ
    shd->setVector(descriptorLayout1->findUniformMemberIndex(_TT("_Color2")), Vector4(0, 0, 1, 1));

    crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->beginRenderPass(crp);
    ctx->setVertexLayout(vertexDecl1);
    ctx->setVertexBuffer(0, vb1);
    ctx->setShaderPass(shaderPass1);
    ctx->setShaderDescriptor_deprecated(shd);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    ctx->drawPrimitive(0, 1);
	ctx->endRenderPass();
	TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-UniformBuffer-1.png"), cbb);
}


//------------------------------------------------------------------------------
//## UniformBufferTest-WorldMatrix
TEST_F(Test_Shader_Shader, UniformBuffer_WorldMatrix)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/UniformBufferTest-WorldMatrix.fx"));
    auto descriptorLayout1 = shader1->descriptorLayout();
    auto shaderPass1 = shader1->techniques()[0]->passes()[0];

    auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
#ifdef LN_COORD_RH
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { -0.5, -0.25, 0 },
        { 0.5, -0.25, 0 },
    };
#else
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
#endif
    auto vb1 = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto crp = TestEnv::renderPass();
    auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass1);


    struct Element
    {
        Matrix _World;
    };
    Element bufferData;
    bufferData._World = Matrix::makeTranslation(0.5, 0, 0);
    //bufferData._World.transpose();

    auto ubIndex = descriptorLayout1->findUniformBufferRegisterIndex(_TT("Element"));
    shd->setUniformBufferData(ubIndex, &bufferData, sizeof(bufferData));

    crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
    ctx->beginRenderPass(crp);
    ctx->setVertexLayout(vertexDecl1);
    ctx->setVertexBuffer(0, vb1);
    ctx->setShaderPass(shaderPass1);
    ctx->setShaderDescriptor_deprecated(shd);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    ctx->drawPrimitive(0, 1);
    ctx->endRenderPass();
    TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-UniformBuffer_WorldMatrix-1.png"), cbb);
}

//------------------------------------------------------------------------------
//## MultiTechMultiTexture
TEST_F(Test_Shader_Shader, MultiTechMultiTexture)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/MultiTechMultiTexture-1.fx"));
    auto descriptorLayout1 = shader1->descriptorLayout();
    auto shaderPass1 = shader1->techniques()[0]->passes()[0];
    auto shaderPass2 = shader1->techniques()[1]->passes()[0];
    auto shaderPass3 = shader1->techniques()[2]->passes()[0];

    auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
#ifdef LN_COORD_RH
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { -0.5, -0.25, 0 },
        { 0.5, -0.25, 0 },
    };
#else
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
#endif
    auto vb1 = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto t1 = Texture2D::create(2, 2, TextureFormat::RGBA8);
    t1->clear(Color::Red);

    auto t2 = Texture2D::create(2, 2, TextureFormat::RGBA8);
    t2->clear(Color::Green);

	// _Texture1 のみ (赤)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass1);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture1")), t1);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture2")), t2);
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

        ctx->setShaderPass(shaderPass1);
        ctx->setShaderDescriptor_deprecated(shd);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-1.png"), cbb);
	}

	// _Texture2 のみ (緑)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass2);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture1")), t1);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture2")), t2);
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

		ctx->setShaderPass(shaderPass2);
        ctx->setShaderDescriptor_deprecated(shd);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-2.png"), cbb);
	}

	// _Texture1 + _Texture2 (黄)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        auto shd = ctx->allocateShaderDescriptor_deprecated(shaderPass3);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture1")), t1);
        shd->setTexture(descriptorLayout1->findTextureRegisterIndex(_TT("_Texture2")), t2);
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

        ctx->setShaderPass(shaderPass3);
        ctx->setShaderDescriptor_deprecated(shd);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-3.png"), cbb);
	}
}

//------------------------------------------------------------------------------
//## シェーダ側とホスト側で頂点レイアウトの過不足がある場合のテスト。必要な部分さえあれば描画は可能。
// NOTE: このテストは実行不能になった。DirectX12 では、VertexLayout と VSStageInput が一致していないと PipelineState の作成に失敗する。
#if 0
TEST_F(Test_Shader_Shader, NotProvidedVertexAttribute)
{
	auto shader1 = Shader::create(LN_ASSETFILE("Shader/NotProvidedVertexAttribute-1.fx"));
    auto descriptorLayout1 = shader1->descriptorLayout();
    auto shaderPass1 = shader1->techniques()[0]->passes()[0];

	auto vertexDecl1 = makeObject_deprecated<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
#ifdef LN_COORD_RH
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { -0.5, -0.25, 0 },
        { 0.5, -0.25, 0 },
    };
#else
    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
#endif
	auto vb1 = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto crp = TestEnv::renderPass();
    crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->beginRenderPass(crp);
	ctx->setVertexLayout(vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
    ctx->setShaderPass(shaderPass1);
	ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	ctx->drawPrimitive(0, 1);
	ctx->endRenderPass();
	TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-NotProvidedVertexAttribute-1.png"), cbb);
}
#endif

