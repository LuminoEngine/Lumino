#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/Material.hpp>

class Test_Material : public ::testing::Test {
protected:
    void SetUp() override {
        (void)ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
    }
    void TearDown() override {
        ln::CoreInstance::terminate();
    }
};

// null をセットしたときに白テクスチャがセットされることを確認するテスト
TEST_F(Test_Material, NullWhiteTexture) {
    auto* module = ln::CoreInstance::instance()->graphicsModule();
    auto matResult = ln::MaterialFactory::createUnlit(module);
    ASSERT_TRUE(matResult);
    auto material = *matResult;

    material->setTexture(nullptr);
    EXPECT_NE(material->baseTexture(), nullptr);
    EXPECT_EQ(material->baseTexture(), module->whiteTexture().get());

    material->setNamedTexture("u_testTex", nullptr);
    const auto& named = material->namedTextures();
    auto it = named.find("u_testTex");
    ASSERT_NE(it, named.end());
    EXPECT_EQ(it->second.get(), module->whiteTexture().get());
}
