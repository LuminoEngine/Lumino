#include <TestConfig.h>
#include "../../Lumino/src/Imaging/TextRenderer.h"

class Test_Graphics_TextRenderer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_TextRenderer, DrawRequest2D)
{
	RefPtr<Font> font1(Font::Create(TestEnv::Manager->GetFontManager()));
	font1->SetName(_T("MS PGothic"));
	font1->SetSize(20);
	

	Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;

	RefPtr<Graphics::TextRenderer> tr(Graphics::TextRenderer::Create(TestEnv::Manager));
	tr->SetFont(font1);

	while (TestEnv::Application->DoEvents())
	{
		Renderer* r = TestEnv::BeginRendering();

		Size size = TestEnv::Application->GetMainWindow()->GetSize();
		Matrix proj = Matrix::Perspective2DLH(size.Width, size.Height, 0, 1000.0f);
		tr->SetViewProjection(Matrix::Identity, proj, size);
		
		//tr->DrawChar('A');
		tr->DrawText(_T("ABCDE"));
		tr->Flash();

		TestEnv::EndRendering();
		Threading::Thread::Sleep(10);
	}

	swap->Present();

}
