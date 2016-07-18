#include <TestConfig.h>
#include "../../../src/EngineManager.h"
#include "../../../src/Graphics/Device/GraphicsDriverInterface.h"

class Test_Graphics_Driver : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Driver, Clear)
{
	auto* device = EngineManager::GetInstance()->GetGraphicsManager()->GetGraphicsDevice();
	auto* r = device->GetRenderer();
	auto* swap = device->GetDefaultSwapChain();
	auto* bb = swap->GetBackBuffer();

	r->Begin();
	r->SetRenderTarget(0, bb);
	r->Clear(ClearFlags::Color, Color::Blue);
	r->End();
	swap->Present(bb);

	bb->Lock()->Save(LN_LOCALFILE("TestData/Test_Graphics_Driver.Clear.png"));
	bb->Unlock();
}
