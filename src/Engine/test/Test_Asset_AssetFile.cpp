#include "Common.hpp"

class Test_Asset_AssetFile : public ::testing::Test {};

#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetFile, SerializeLevel)
{
    World* world = Engine::activeWorld();
    Engine::setActiveWorld(nullptr);

    {
        auto level1 = makeObject<Level>();

        auto texture = Texture2D::load("Assets/dir/Sprite1.png");
        auto sprite = Sprite::create(texture);

        level1->addObject(sprite);
        Assets::saveAsset(level1, u"Assets/Level1");

        auto text = FileSystem::readAllText(LN_ASSETFILE("Assets/Level1.yml"));

        // 保存されたファイルに、少なくとも相対パスで画像ファイルが入っていること
        ASSERT_EQ(true, text.contains(u"dir/Sprite1.png"));
    }




    //auto level1 = makeObject<TestLevel>();
    //Assets::reloadAsset(u"Level1", level1);


    Engine::setActiveWorld(world);
}
#endif

