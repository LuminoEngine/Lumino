#include "Common.hpp"

//==============================================================================
class Test_Rendering_RenderingContext : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_RenderingContext, Basic)
{



    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Result/Test_Rendering_ShapeRenderer-Basic-2.png"));
    LN_TEST_CLEAN_SCENE;
}

