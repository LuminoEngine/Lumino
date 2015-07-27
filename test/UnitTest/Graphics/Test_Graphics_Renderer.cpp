#include <TestConfig.h>

class Test_Graphics_Renderer : public ::testing::Test
{
protected:
	RefPtr<Shader>			m_shader;
	RefPtr<VertexBuffer>	m_vbUpTriangle;		// ▲
	RefPtr<VertexBuffer>	m_vbDownTriangle;	// ▼

	virtual void SetUp()
	{
		m_shader = TestEnv::CreateShader(LOCALFILE("TestData/PosColor.lnsl"));

		// ▲
		PosColorVertex vertices1[] =
		{
			{ Vector3(-1.0f, -1.0f, 0.0f), ColorF::Blue },	// 左下 青
			{ Vector3(1.0f, -1.0f, 0.0f), ColorF::Green },	// 右下 緑
			{ Vector3(0.0f, 1.0f, 0.0f), ColorF::Red },		// 頂点 赤
		};
		m_vbUpTriangle.Attach(VertexBuffer::Create(PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices1), vertices1));

		// ▼
		PosColorVertex vertices2[] =
		{
			{ Vector3(-1.0f, 1.0f, 0.0f), ColorF::Blue },	// 左下 青
			{ Vector3(1.0f, 1.0f, 0.0f), ColorF::Green },	// 右下 緑
			{ Vector3(0.0f, -1.0f, 0.0f), ColorF::Red },	// 頂点 赤
		};
		m_vbDownTriangle.Attach(VertexBuffer::Create(PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices2), vertices2));


		Renderer* r = TestEnv::Renderer;
		SwapChain* swap = TestEnv::MainSwapChain;
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
	}

	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Renderer, SetDepthStencilState)
{
	Renderer* r = TestEnv::BeginRendering();

	// ステンシルバッファを作る
	DepthStencilState state1;
	state1.DepthEnable = true;
	state1.DepthWriteEnable = false;			// 深度バッファを更新する必要はなし
	state1.DepthFunc = CompareFunc::Never;		// 深度テストを全て失敗にし、カラーバッファへ書き込まない
	state1.StencilEnable = true;
	state1.StencilFunc = CompareFunc::Always;	// ステンシルテストは常に合格する
	state1.StencilReferenceValue = 0xFF;		// ステンシルバッファに書き込む値
	state1.StencilFailOp = StencilOp::Keep;		// (絶対失敗しないので省略してよいが)
	state1.StencilDepthFailOp = StencilOp::Replace;
	state1.StencilPassOp = StencilOp::Replace;
	r->SetDepthStencilState(state1);
	r->SetVertexBuffer(m_vbDownTriangle);
	m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);

	// ステンシルバッファを適用して描画する
	DepthStencilState state2;
	state2.DepthEnable = true;
	state2.DepthWriteEnable = true;
	state2.DepthFunc = CompareFunc::Less;		// デフォルト値に戻す
	state2.StencilEnable = true;
	state2.StencilFunc = CompareFunc::Equal;	// 先ほど描いた 0xFF と同じものを合格にする
	state2.StencilReferenceValue = 0xFF;		// 先ほど描いた 0xFF と同じものを合格にする
	state2.StencilFailOp = StencilOp::Keep;		// ステンシルバッファへは書き込まない
	state2.StencilDepthFailOp = StencilOp::Keep;// ステンシルバッファへは書き込まない
	state2.StencilPassOp = StencilOp::Keep;		// ステンシルバッファへは書き込まない
	r->SetDepthStencilState(state2);
	r->SetVertexBuffer(m_vbUpTriangle);
	m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);

	TestEnv::EndRendering();

	SS_CHECK(/*SS_SAVE*/SS_LOAD, "Test_Graphics_Renderer.SetDepthStencilState.png");
}
