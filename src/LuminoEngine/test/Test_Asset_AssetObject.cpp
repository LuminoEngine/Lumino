#include "Common.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

class Test_Asset_AssetObject : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetObject, Basic)
{
	auto model = AssetModel::create(makeObject<SpriteFrame>());
	auto prop = model->findProperty(u"SourceRect");

    Rect v1(1, 2, 3, 4);
    prop->setValue<Rect>(v1);

    Rect v2 = prop->getValue<Rect>();
    ASSERT_EQ(v1, v2);


    String json = JsonSerializer::serialize(*model->target(), JsonFormatting::None);
    auto str = Serializer::serialize<SpriteFrame>(Ref<SpriteFrame>(static_cast<SpriteFrame*>(model->target())));

    auto obj2 = Serializer::deserialize<SpriteFrame>(str);
    ASSERT_EQ(v1, obj2->sourceRect());
}
