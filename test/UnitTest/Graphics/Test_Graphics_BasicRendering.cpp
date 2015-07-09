#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_BasicRendering : public ::testing::Test
{
protected:
	RefPtr<Shader> m_shader;

	virtual void SetUp()
	{
		ByteBuffer code = FileSystem::ReadAllBytes(LOCALFILE("TestData/PosColor.lnsl"));
		m_shader.Attach(Shader::Create((char*)code.GetData(), code.GetSize()));
	}
	virtual void TearDown() {}

};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_BasicRendering, PosColorVertex)
{
	PosColorVertex vertices[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), ColorF::Blue },	// ç∂â∫ ê¬
		{ Vector3(1.0f, -1.0f, 0.0f), ColorF::Green },	// âEâ∫ óŒ
		{ Vector3(0.0f, 1.0f, 0.0f), ColorF::Red },		// í∏ì_ ê‘
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices), vertices));

	Renderer* r = TestEnv::BeginRendering();
	r->SetVertexBuffer(vb);
	m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
	TestEnv::EndRendering();

	SS_SAVE("Test_Graphics_BasicRendering.PosColorVertex.png");
	//SS_CHECK("Test_Graphics_BasicRendering.PosColorVertex.png");
}
