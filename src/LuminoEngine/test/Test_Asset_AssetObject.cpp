#include "Common.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

class Test_Asset_AssetObject : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetObject, Basic)
{
	auto obj = AssetObject::create(makeObject<SpriteFrame>());
	auto prop = obj->findProperty(u"SourceRect");

    Rect v1(1, 2, 3, 4);
    prop->setValue<Rect>(v1);

    Rect v2 = prop->getValue<Rect>();
    ASSERT_EQ(v1, v2);
}
