#include <TestConfig.h>
#include "../../src/Graphics/RendererImpl.h"

class Test_Graphics_RenderingContext : public ::testing::Test
{
protected:
	RefPtr<Shader> m_shader;

	virtual void SetUp()
	{
		//ByteBuffer code = FileSystem::ReadAllBytes(LOCALFILE("TestData/PosColor.lnsl"));
		//m_shader.Attach(Shader::Create((char*)code.GetData(), code.GetSize()));

		m_shader = Shader::Create(LN_LOCALFILE("TestData/PosColor.lnsl"));
	}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_RenderingContext, Clear)
{
	Engine::BeginRendering();
	auto* r = GraphicsContext::GetContext();
	r->Clear(ClearFlags::Color, Color::Red);
	Engine::EndRendering();
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Graphics_RenderingContext2.png")));
	//TestEnv::SaveScreenShot(LN_TEMPFILE("test.png"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_RenderingContext, PosColorVertex)
{
	// 反時計回りを表とする
	PosColorVertex vertices[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), Color::Blue },	// 左下 青
		{ Vector3(1.0f, -1.0f, 0.0f), Color::Green },	// 右下 緑
		{ Vector3(0.0f, 1.0f, 1.0f), Color::Red },		// 頂点 赤
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices), vertices), false);

	Engine::BeginRendering();
	auto* r = GraphicsContext::GetContext();
	r->SetShaderPass(m_shader->GetTechniques()[0]->GetPasses()[0]);
	r->DrawPrimitive(vb, PrimitiveType_TriangleList, 0, 1);
	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Graphics_RenderingContext1.png")));
	//TestEnv::SaveScreenShot(LN_TEMPFILE("test1.png"));

#if 0
	{
		do
		{
			if (Engine::BeginRendering())
			{
				auto* r = GraphicsContext::GetContext();
				r->SetVertexBuffer(vb);
				r->SetDepthBuffer(nullptr);
				r->Clear(ClearFlags::All, Color::Gray);
				r->SetShaderPass(m_shader->GetTechniques()[0]->GetPasses()[0]);
				r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);


				auto* r2 = GraphicsManager::Instance->GetRenderer();
				r2->SetVertexBuffer(vb);
				r2->SetDepthBuffer(nullptr);
				r2->Clear(ClearFlags::All, Color::Gray);
				////r->SetShaderPass(m_shader->GetTechniques()[0]->GetPasses()[0]);
				m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
				r2->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);

				Engine::EndRendering();
			}
		} while (Engine::UpdateFrame());

		TestEnv::SaveScreenShot(LN_TEMPFILE("test2.png"));
	}
//	bool r7 = TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Graphics_RenderingContext1.png"));
	printf("");
	//Renderer* r = TestEnv::BeginRendering();
	//r->SetVertexBuffer(vb);
	//m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	//r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
	//TestEnv::EndRendering();

	//SS_CHECK(SS_SAVE, "Test_Graphics_BasicRendering.PosColorVertex.png");
	////SS_CHECK("Test_Graphics_BasicRendering.PosColorVertex.png");
#endif
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_RenderingContext, Blt)
{
	auto t1 = Texture2D::Create(LN_LOCALFILE("TestData/Test_Graphics_RenderingContext1.png"));
	auto t2 = RenderTarget::Create(t1->GetSize());

	Engine::BeginRendering();
	auto* r = GraphicsContext::GetContext();

	r->Blt(t1, t2);

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::EqualsTexture(t2, LN_LOCALFILE("TestData/Test_Graphics_RenderingContext1.png")));
}
