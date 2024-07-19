#include "TestEnv.hpp"

class Test_TextRendering : public ::testing::Test {};

TEST_F(Test_TextRendering, Basic) {

    LNHandle graphicsContext = TestEnv::graphicsContext;
    LNHandle renderingCommandList = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNGraphicsCommandList_Create(graphicsContext, &renderingCommandList));

    ASSERT_EQ(LN_OK, LNGraphicsCommandList_Reset(renderingCommandList, TestEnv::viewPoint));

    ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(graphicsContext, renderingCommandList));
    TestEnv::present();
    TestEnv::present();

    auto context = static_cast<ln::GraphicsContext*>(ln::Runtime::getObject(graphicsContext));
    ln::GraphicsTestHelper::saveScreenShot(TestEnv::getTestDataPath(U"Test_TextRendering_Basic.png"), context, context->currentBackbuffer());

    //ln::GraphicsContext* context = LN_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext);

    LNObject_Release(renderingCommandList);
}
