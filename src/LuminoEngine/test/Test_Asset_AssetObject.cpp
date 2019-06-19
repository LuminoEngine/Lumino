#include "Common.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

class Test_Asset_AssetObject : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetObject, Basic)
{
	auto obj = AssetObject::create(makeObject<SpriteFrame>());
	auto prop = obj->findProperty(u"SourceRect");

}
