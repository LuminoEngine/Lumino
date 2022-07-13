#include "Common.hpp"

//==============================================================================
class Test_UI_UIFrameWindow : public LuminoSceneTest {};

TEST_F(Test_UI_UIFrameWindow, Resize) {
    UIFrameWindow* window = Engine::mainWindow();
    float oldWidth = window->width();
    float oldHeight = window->height();   

    window->setSize(200, 200);
    TestEnv::updateFrame();
    
    window->setSize(oldWidth, oldHeight);
    TestEnv::updateFrame();

	LN_TEST_CLEAN_SCENE;
}

