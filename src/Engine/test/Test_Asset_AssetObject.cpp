#include "Common.hpp"
#include <LuminoEngine/Asset/AssetModel.hpp>

#if 0
class Test_Asset_AssetObject : public ::testing::Test {};

class Test_Asset_AssetObject_Test : public SpriteFrame
{
    LN_OBJECT;
public:
    virtual void onSerialize(Serializer* ar) override
    {

    }

    int m_value;
};

LN_OBJECT_IMPLEMENT(Test_Asset_AssetObject_Test, SpriteFrame) {}

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetObject, Basic)
{
    EngineContext::current()->registerType<Test_Asset_AssetObject_Test>();

	auto model1 = makeObject<AssetModel>(makeObject<Test_Asset_AssetObject_Test>());

    Assets::saveAssetToLocalFile(model1, u"test.json");

	//auto prop = model1->findProperty(u"SourceRect");

 //   Rect v1(1, 2, 3, 4);
 //   prop->setValue<Rect>(v1);

 //   Rect v2 = prop->getValue<Rect>();
 //   ASSERT_EQ(v1, v2);

    //model1->saveInternal(LN_TEMPFILE("SpriteFrame.lnasset"));

    //auto model2 = makeObject<AssetModel>();
    //model2->loadInternal(LN_TEMPFILE("SpriteFrame.lnasset"));

    //ASSERT_EQ(true, dynamic_cast<SpriteFrame*>(model2->target()) != nullptr);
    //ASSERT_EQ(v1, static_cast<SpriteFrame*>(model2->target())->sourceRect());

    //String json = JsonSerializer::serialize(*model->target(), JsonFormatting::None);
    //auto str = Serializer::serialize<SpriteFrame>(Ref<SpriteFrame>(static_cast<SpriteFrame*>(model->target())));

    //auto obj2 = Serializer::deserialize<SpriteFrame>(str);
    //ASSERT_EQ(v1, obj2->sourceRect());
}
#endif

