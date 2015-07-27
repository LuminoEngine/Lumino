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
	ByteBuffer code(FileSystem::ReadAllBytes(LOCALFILE("TestData/ColorPos.glsl")));
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





	//Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;



	const int count = 500;
	RefPtr<Shader> shaders[count];

	while (TestEnv::Application->DoEvents())
	{
		Renderer* r = TestEnv::BeginRendering();
		r->SetVertexBuffer(vb);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
		TestEnv::EndRendering();

		for (int i = 0; i < count; ++i) {
			shaders[i].Attach(Shader::Create((char*)code.GetData(), code.GetSize()));
		}

		//::Sleep(10);
	}



	swap->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
	swap->GetBackBuffer()->Unlock();
}
