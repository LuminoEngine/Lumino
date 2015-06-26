#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_IndexBuffer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_IndexBuffer, BasicTriangle)
{
	ByteBuffer code(FileUtils::ReadAllBytes(LOCALFILE("TestData/PosColor.lnsl")));
	RefPtr<Shader> shader(Shader::Create((char*)code.GetData(), code.GetSize()));

	// 頂点バッファ
	PosColorVertex vertices[] =
	{
		{ Vector3(-0.8f, 0.8f, 0.0f),	ColorF::Blue },		// 左上 青
		{ Vector3(0.8f, 0.8f, 0.0f),	ColorF::White },	// 右上 白
		{ Vector3(-0.8f, -0.8f, 0.0f),	ColorF::Green },	// 左下 緑
		{ Vector3(0.8f, -0.8f, 0.0f),	ColorF::Red },		// 右下 赤
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		PosColorVertex::GetLayout(), 2, LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));

	// インデックスバッファ
	uint16_t indices[] =
	{
		0, 2, 1,	// 反時計回り (裏面。デフォルトでは描画されない)
		2, 1, 3,	// 時計回り (デフォルトの表面)
	};
	RefPtr<IndexBuffer> ib(IndexBuffer::Create(LN_ARRAY_SIZE_OF(indices), indices, IndexBufferFormat_UInt16, DeviceResourceUsage_Static));


	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	//while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(1, 1, 1, 1), 1.0f);
		r->SetVertexBuffer(vb);
		r->SetIndexBuffer(ib);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 2);
		swap->Present();

		//::Sleep(10);
	}

	//TestEnvironment::SaveScreenShot(LOCALFILE("test.png"));
	ASSERT_TRUE(TestEnvironment::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_IndexBuffer.BasicTriangle.png")));
}
