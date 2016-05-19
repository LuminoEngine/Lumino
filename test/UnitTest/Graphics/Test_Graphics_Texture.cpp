#include <TestConfig.h>
#include "../../../../src/Graphics/GraphicsManager.h"

class Test_Graphics_Texture : public ::testing::Test
{
protected:
	
	virtual void SetUp() 
	{
	}
	virtual void TearDown() 
	{
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, SetSubData)
{
	RefPtr<Font> font(Font::CreateBuiltInBitmapFontInternal(GraphicsManager::GetInstance()->GetFontManager(), 7));
	FontGlyphBitmap* fb = font->LookupGlyphBitmap('S', 0);

	auto texture = Texture2D::Create(32, 32);
	auto sprite = Sprite2D::Create(texture);



	Engine::UpdateFrame();

	TestEnv::SaveScreenShot(LN_TEMPFILE("test.png"));
}


