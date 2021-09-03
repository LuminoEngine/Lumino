#include "Common.hpp"

//==============================================================================
//
class Test_Rendering_DefaultShading : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//
TEST_F(Test_Rendering_DefaultShading, Basic)
{
	auto box = BoxMesh::With().buildInto();

    // 環境光を切っておく
    Engine::mainLight()->setAmbientColor(Color(0,0,0));

    // 直上から唯一のライトを当てる
    Engine::mainLight()->setPosition(0, 10, 0);
    Engine::mainLight()->lookAt(0, 0, 0);

    // Box の上面が真っ白になる。
    // デフォルトのマテリアルは Metaric が 0.0 なので、物体の色が 100% 反映される。
    {
        Engine::mainCamera()->setPosition(0, 2, 0);
        Engine::mainCamera()->lookAt(0, 0, 0);

        TestEnv::updateFrame();

        auto bmp = TestEnv::capture();
        ASSERT_EQ(ColorI(255, 255, 255, 255), bmp->getPixel32(50, 50));
    }

    // Box の側面は、光線と法線が 90度異なるため光は全く当たらない。
    // 環境光も切ってあるので真っ黒になる。
    {
        Engine::mainCamera()->setPosition(0, 0, 2);
        Engine::mainCamera()->lookAt(0, 0, 0);

        TestEnv::updateFrame();

        auto bmp = TestEnv::capture();
        ASSERT_EQ(ColorI(0, 0, 0, 255), bmp->getPixel32(50, 50));
    }


    LN_TEST_CLEAN_SCENE;
}
