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
	RefPtr<Font> font1(Font::Create(TestEnvironment::Manager->GetFontManager()));
	font1->SetName(_T("MS PGothic"));
	font1->SetSize(20);
	

	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	RefPtr<Graphics::TextRenderer> tr(Graphics::TextRenderer::Create(TestEnvironment::Manager));
	tr->SetFont(font1);

	while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF::Gray, 1.0f);

		Size size = TestEnvironment::Application->GetMainWindow()->GetSize();
		Matrix proj = Matrix::Perspective2DLH(size.Width, size.Height, 0, 1000);
		tr->SetViewProjection(Matrix::Identity, proj, size);
		
		//tr->DrawChar('A');
		tr->DrawText(_T("ABCDE"));
		tr->Flash();

		swap->Present();
		Threading::Thread::Sleep(10);
	}

	swap->Present();

}
