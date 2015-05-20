#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_VertexBuffer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_VertexBuffer, BasicTriangle)
{
	RefPtr<ByteBuffer> code(FileUtils::ReadAllBytes(LOCALFILE("TestData/ColorPos.glsl")));
	RefPtr<Shader> shader(Shader::Create((char*)code->GetData(), code->GetSize()));

	
	// 頂点レイアウト・頂点バッファ
	VertexElement elements[] =
	{
		{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
		{ 0, VertexElementType_Color4, VertexElementUsage_Color, 0 },
	};
	struct Vertex
	{
		Vector3		Pos;
		uint32_t	Color;
	};
	Vertex vertices[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), 0xFFFF0000 },	// 左下 青
		{ Vector3(1.0f, -1.0f, 0.0f), 0xFF00FF00 },		// 右下 緑
		{ Vector3(0.0f, 1.0f, 0.0f), 0xFF0000FF },		// 頂点 赤
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		elements, LN_ARRAY_SIZE_OF(elements), LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));

	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	//while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(1, 1, 1, 1), 1.0f);
		r->SetVertexBuffer(vb);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
		swap->Present();

		//::Sleep(10);
	}

	ASSERT_TRUE(TestEnvironment::EqualsBitmapFile(swap->GetBackBuffer()->Lock(), LOCALFILE("TestData/Test_Graphics_VertexBuffer.BasicTriangle.png")));
	//swap->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
	//swap->GetBackBuffer()->Unlock();
}
