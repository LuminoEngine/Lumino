#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>

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

// サンプラー設定の既定値と、名前付き上書きの解決順を確認するテスト。
TEST_F(Test_Material, SamplerStateResolution) {
    auto* module = ln::CoreInstance::instance()->graphicsModule();
    auto matResult = ln::MaterialFactory::createUnlit(module);
    ASSERT_TRUE(matResult);
    auto material = *matResult;

    // 既定は Linear + ClampToEdge。
    // Repeat による意図しない回り込みを避けるため、既定は Clamp 側に寄せてある。
    EXPECT_EQ(ln::rhi::FilterMode::Linear, material->samplerState().filter);
    EXPECT_EQ(ln::rhi::AddressMode::ClampToEdge, material->samplerState().address);

    // マテリアル単位の設定は、名前付き上書きが無い全スロットに適用される。
    material->setSamplerState({ln::rhi::FilterMode::Nearest, ln::rhi::AddressMode::Repeat});
    EXPECT_EQ(ln::rhi::FilterMode::Nearest, material->resolveSamplerState("u_baseTexture").filter);
    EXPECT_EQ(ln::rhi::AddressMode::Repeat, material->resolveSamplerState("u_baseTexture").address);
    // 名前が空 (対応テクスチャ不明) の場合もマテリアル単位の設定にフォールバックする。
    EXPECT_EQ(ln::rhi::FilterMode::Nearest, material->resolveSamplerState("").filter);

    // 名前付き上書きはマテリアル単位の設定より優先される。
    material->setNamedSamplerState("u_noiseTexture",
        {ln::rhi::FilterMode::Linear, ln::rhi::AddressMode::MirroredRepeat});
    EXPECT_EQ(ln::rhi::FilterMode::Linear, material->resolveSamplerState("u_noiseTexture").filter);
    EXPECT_EQ(ln::rhi::AddressMode::MirroredRepeat, material->resolveSamplerState("u_noiseTexture").address);
    // 上書きしていないスロットは影響を受けない。
    EXPECT_EQ(ln::rhi::FilterMode::Nearest, material->resolveSamplerState("u_baseTexture").filter);
}

// rhi::SamplerDesc の既定値が ln::SamplerState の既定値と一致していることを確認するテスト。
// 片方だけ変更されると、SamplerDesc を直接組み立てる将来のコードだけが
// 異なる挙動になるため、ここで固定しておく。
TEST_F(Test_Material, SamplerDescDefaultMatchesSamplerState) {
    ln::SamplerState state;
    ln::rhi::SamplerDesc desc;

    EXPECT_EQ(desc.magFilter, state.filter);
    EXPECT_EQ(desc.minFilter, state.filter);
    EXPECT_EQ(desc.addressU, state.address);
    EXPECT_EQ(desc.addressV, state.address);
    EXPECT_EQ(desc.addressW, state.address);
}

// サンプラー設定を変えたら bindingVersion が上がることを確認するテスト。
// これが上がらないと Renderer 側の BindGroup キャッシュが無効化されず、
// 「設定したのに反映されない」不具合になる。
TEST_F(Test_Material, SamplerStateBumpsBindingVersion) {
    auto* module = ln::CoreInstance::instance()->graphicsModule();
    auto matResult = ln::MaterialFactory::createUnlit(module);
    ASSERT_TRUE(matResult);
    auto material = *matResult;

    uint64_t version = material->bindingVersion();
    material->setSamplerState({ln::rhi::FilterMode::Nearest, ln::rhi::AddressMode::ClampToEdge});
    EXPECT_GT(material->bindingVersion(), version);

    // 同じ値の再設定では上がらない (無駄な BindGroup 再構築を避ける)。
    version = material->bindingVersion();
    material->setSamplerState({ln::rhi::FilterMode::Nearest, ln::rhi::AddressMode::ClampToEdge});
    EXPECT_EQ(version, material->bindingVersion());

    version = material->bindingVersion();
    material->setNamedSamplerState("u_baseTexture",
        {ln::rhi::FilterMode::Linear, ln::rhi::AddressMode::Repeat});
    EXPECT_GT(material->bindingVersion(), version);

    version = material->bindingVersion();
    material->setNamedSamplerState("u_baseTexture",
        {ln::rhi::FilterMode::Linear, ln::rhi::AddressMode::Repeat});
    EXPECT_EQ(version, material->bindingVersion());
}

// paramVersion と bindingVersion が独立して動くことを確認するテスト。
// パラメータ変更で bindingVersion が上がってしまうと、色を毎フレーム変える
// だけのマテリアルで BindGroup が毎フレーム作り直される (B-1 の課題)。
TEST_F(Test_Material, ParamAndBindingVersionAreIndependent) {
    auto* module = ln::CoreInstance::instance()->graphicsModule();
    auto matResult = ln::MaterialFactory::createUnlit(module);
    ASSERT_TRUE(matResult);
    auto material = *matResult;

    // パラメータ変更は paramVersion だけを進める。
    uint64_t paramVer = material->paramVersion();
    uint64_t bindVer = material->bindingVersion();
    material->setColor(ln::Color(1.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_GT(material->paramVersion(), paramVer);
    EXPECT_EQ(bindVer, material->bindingVersion());

    // テクスチャ変更は bindingVersion だけを進める。
    paramVer = material->paramVersion();
    bindVer = material->bindingVersion();
    material->setTexture(nullptr);
    EXPECT_EQ(paramVer, material->paramVersion());
    EXPECT_GT(material->bindingVersion(), bindVer);

    // サンプラー変更も bindingVersion だけを進める。
    paramVer = material->paramVersion();
    bindVer = material->bindingVersion();
    material->setSamplerState({ln::rhi::FilterMode::Nearest, ln::rhi::AddressMode::Repeat});
    EXPECT_EQ(paramVer, material->paramVersion());
    EXPECT_GT(material->bindingVersion(), bindVer);
}

// Sampler バインディングから、それが担当するテクスチャ名を逆引きできることを確認するテスト。
// Unlit は "u_baseTexture" / "u_baseSampler" という接頭辞一致のペア命名を使っている。
TEST_F(Test_Material, SamplerBindingResolvesToTextureName) {
    auto* module = ln::CoreInstance::instance()->graphicsModule();
    auto matResult = ln::MaterialFactory::createUnlit(module);
    ASSERT_TRUE(matResult);
    auto material = *matResult;

    auto* pass = material->shaderPass();
    ASSERT_NE(nullptr, pass);

    const auto& layout = pass->materialLayoutDesc();
    const auto& names = pass->materialSamplerTextureNames();
    ASSERT_EQ(layout.entries.size(), names.size());

    bool foundSampler = false;
    for (size_t i = 0; i < layout.entries.size(); ++i) {
        if (layout.entries[i].type != ln::rhi::BindingType::Sampler) {
            // Sampler 以外のエントリは空文字列。
            EXPECT_TRUE(names[i].empty());
            continue;
        }
        foundSampler = true;
        EXPECT_EQ("u_baseTexture", names[i]);
    }
    EXPECT_TRUE(foundSampler) << "Unlit の $Material セットに Sampler バインディングが見つかりません";
}
