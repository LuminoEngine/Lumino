#include <TestConfig.h>
#include "../../Lumino/src/Imaging/TextRenderer.h"

class Test_Graphics_SpriteRenderer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_SpriteRenderer, DrawRequest2D)
{
	RefPtr<Texture> tex1(Texture::Create(LOCALFILE("TestData/img1_BYTE_R8G8B8A8_20x20.png")));
	//RefPtr<Texture> tex1(Texture::Create(Size(256, 256)));

	RefPtr<Font> font1(Font::Create(TestEnv::Manager->GetFontManager()));
	font1->SetName(_T("MS PGothic"));
	font1->SetSize(20);
//	font1->LookupGlyphData('i', NULL);
	FontGlyphLocation* loc = font1->AdvanceKerning('i', NULL);
	loc = font1->AdvanceKerning('j', loc);
	
	//Bitmap* tmp = tex1->Lock();
	//Imaging::TextRenderer tr(tmp, TestEnv::Manager->GetFontManager()->GetDefaultFont());
	//tr.SetAreaBox(Rect(0, 0, tex1->GetSize()));
	//tr.DrawText(_T("‚â‚Á‚Æ‘‚¯‚½B(EƒÖEM)"), -1);
	//tmp->Save(LOCALFILE("TestData/tmp2.png"));
	//tex1->Unlock();


	RefPtr<SpriteRenderer> sr(SpriteRenderer::Create(512, TestEnv::Manager));

	Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;

	r->SetRenderTarget(0, swap->GetBackBuffer());
	r->SetDepthBuffer(swap->GetBackBufferDepth());
	r->Clear(true, true, ColorF(0, 1, 1, 1), 1.0f);

	Size size = TestEnv::Application->GetMainWindow()->GetSize();
	Matrix proj = Matrix::Perspective2DLH((float)size.Width, (float)size.Height, 0.f, 1000.f);

	sr->SetViewProjMatrix(Matrix::Identity, proj);
	sr->SetViewPixelSize(size);

	//sr->DrawRequest2D(Vector3::Zero, Vector3::Zero, Vector2(20, 20), tex1, RectF(0, 0, 20, 20), NULL);
	sr->DrawRequest2D(Vector3::Zero, Vector3::Zero, Vector2(256, 256), tex1, RectF(0, 0, 256, 256), NULL);

	sr->Flash();
	swap->Present();

	TestEnv::SaveScreenShot(LOCALFILE("TestData/Test_Graphics_SpriteRenderer.DrawRequest2D.png"));
	//ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_GeometryRenderer.DrawRect.png")));
}
