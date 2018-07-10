#include "Common.hpp"

class Test_Graphics_LowLevelRendering : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_shader1 = Shader::create(LN_ASSETFILE("simple.vert"), LN_ASSETFILE("simple.frag"));

		m_vertexDecl1 = newObject<VertexDeclaration>();
		m_vertexDecl1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	}

	virtual void TearDown() {}

	Ref<Shader> m_shader1;
	Ref<VertexDeclaration> m_vertexDecl1;
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, BasicTriangle)
{
	// # 時計回り (左ねじ) で描画できること
	{
		m_shader1->setVector("g_color", Vector4(1, 0, 0, 1));

		Vector4 v[] = {
			Vector4(0, 0.5, 0, 1),
			Vector4(0.5, -0.25, 0, 1),
			Vector4(-0.5, -0.25, 0, 1),
		};
		auto vertexBuffer = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

		auto ctx = Engine::graphicsContext();
		ctx->setVertexDeclaration(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-BasicTriangle-1.png"));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, VertexBuffer)
{
	m_shader1->setVector("g_color", Vector4(1, 0, 0, 1));

	auto vb1 = newObject<VertexBuffer>(sizeof(Vector4) * 3, GraphicsResourceUsage::Static);
	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(m_vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

	// * [ ] まだ一度もレンダリングに使用されていないバッファを、更新できること (static)
	{
		Vector4 v1[] = {
			Vector4(0, 0.5, 0, 1),
			Vector4(0.5, -0.25, 0, 1),
			Vector4(-0.5, -0.25, 0, 1),
		};
		memcpy(vb1->map(MapMode::Write), v1, vb1->size());

		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		ASSERT_SCREEN_S(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-2.png"));
	}

	// * [ ] 一度レンダリングに使用されたバッファを、再更新できること (static)
	{
		Vector4 v2[] = {
			Vector4(0, 1, 0, 1),
			Vector4(1, -1, 0, 1),
			Vector4(-1, -1, 0, 1),
		};
		memcpy(vb1->map(MapMode::Write), v2, vb1->size());

		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		ASSERT_SCREEN_S(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-3.png"));
	}

	// TODO: 部分 lock

	// 拡張
}
