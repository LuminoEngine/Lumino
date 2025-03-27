#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include "TestEnv.hpp"

class Test_TextRendering : public ::testing::Test {};

TEST_F(Test_TextRendering, Basic) {

    LNHandle surfaceContext = TestEnv::surfaceContext;
    LNHandle colorBuffer = LN_NULL_HANDLE;
    LNHandle depthBuffer = LN_NULL_HANDLE;
    LNHandle commandList = LN_NULL_HANDLE;
    ASSERT_EQ(
        LN_OK, LNGraphicsContext_PrepareFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer, &commandList));


    ASSERT_EQ(LN_OK, LNCommandList_Reset(commandList));

    ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(surfaceContext, commandList));
    TestEnv::present();
    TestEnv::present();

    auto* m = ln::detail::RuntimeManager::instance();
    ln::SurfaceContext* sc = static_cast<ln::SurfaceContext*>(m->getObjectEntry(surfaceContext)->object);
    ln::GraphicsTestHelper::saveScreenShot(
        TestEnv::getTestDataPath(U"Test_TextRendering_Basic.png"), sc->context(), sc->context()->currentBackbuffer());

    //ln::GraphicsContext* context = LN_HANDLE_TO_OBJECT(ln::GraphicsContext, surfaceContext);

}
