#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_Shader : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Shader, Basic)
{
	ByteBuffer code(FileUtils::ReadAllBytes(LOCALFILE("TestData/ColorPos.glsl")));
	RefPtr<Shader> shader(Shader::Create((char*)code.GetData(), code.GetSize()));


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
	Vertex vertices[] = {
		{ Vector3(-0.5, 0.5, 0.0), 0xFF0000FF },
		{ Vector3(0.5, -0.5, 0.0), 0xFFFF00FF },
		{ Vector3(-0.5, -0.5, 0.0), 0xFF00FFFF },
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		elements, LN_ARRAY_SIZE_OF(elements), LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));





	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;



	const int count = 500;
	RefPtr<Shader> shaders[count];

	while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(0, 0, 1, 1), 1.0f);
		r->SetVertexBuffer(vb);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
		swap->Present();

		for (int i = 0; i < count; ++i) {
			shaders[i].Attach(Shader::Create((char*)code.GetData(), code.GetSize()));
		}

		//::Sleep(10);
	}



	swap->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
	swap->GetBackBuffer()->Unlock();
}
