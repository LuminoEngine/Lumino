﻿#include "Common.hpp"

//==============================================================================
//# Testing Shader
class Test_Shader_Shader : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Shader_Shader, IndependentSamplerState)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/IndependentSamplerState.fx"));

    auto vertexDecl1 = newObject<VertexDeclaration>();
    vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
    vertexDecl1->addVertexElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);

    struct Vertex
    {
        Vector3 pos;
        Vector2 uv;
    };
    Vertex v[] = {
        { { -1, 1, 0 }, { 0, 0 }, },	// 0.5 で中央の face からサンプリングする
        { { 0, 1, 0 }, { 1, 0 }, },
        { { -1, 0, 0 }, { 0, 1 }, },
        { { 0, 0, 0 }, { 1, 1 }, },
    };
    auto vb1 = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto tex1 = newObject<Texture2D>(2, 2, TextureFormat::RGBA32, false);
    auto bmp1 = tex1->map(MapMode::Write);
    bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
    bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
    bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
    bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));

    // TODO: まだ SamplerState 直接指定をサポートしていないので Texture に対してセットする方法でテストケースだけ用意しておく。
    // 後でサポートするとき、shader1->findParameter("mySamplerState")->setSamplerState(samplerState); とかに書き換える。
    auto samplerState = newObject<SamplerState>();
    samplerState->setFilterMode(TextureFilterMode::Linear);
    tex1->setSamplerState(samplerState);

    shader1->findParameter("myTexture")->setTexture(tex1);

    auto ctx = Engine::graphicsContext();
    TestEnv::resetGraphicsContext(ctx);
    ctx->setVertexDeclaration(vertexDecl1);
    ctx->setVertexBuffer(0, vb1);
    ctx->setIndexBuffer(nullptr);
    ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

    // * [ ] default
    {
        ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
        ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

        ASSERT_SCREEN(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-IndependentSamplerState-1.png"));
    }
}
