#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <cmath>

using namespace ln;

// ---------------------------------------------------------------------------
// Helpers: fake Material pointers for sort-key testing (never dereferenced)
// ---------------------------------------------------------------------------
static Material* fakeMat(uintptr_t id) { return reinterpret_cast<Material*>(id); }

// ---------------------------------------------------------------------------
// DrawCommandBuffer tests
// ---------------------------------------------------------------------------

class Test_DrawCommandBuffer : public ::testing::Test {
protected:
    DrawCommandBuffer m_buf;
    void SetUp() override { m_buf.clear(); }
};

TEST_F(Test_DrawCommandBuffer, Empty) {
    EXPECT_TRUE(m_buf.commands().empty());
}

TEST_F(Test_DrawCommandBuffer, DrawSprite) {
    Material* mat = fakeMat(0x1000);
    m_buf.drawSprite(mat, 5,
        Vector3{1, 2, 3}, Vector2{10, 20},
        Vector2{0.1f, 0.2f}, Vector2{0.5f, 0.6f},
        Color{1, 0, 0, 1}, 0.5f);

    ASSERT_EQ(m_buf.commands().size(), 1u);
    const auto& cmd = m_buf.commands()[0];
    EXPECT_EQ(cmd.type, DrawCommandType::Sprite);
    EXPECT_EQ(cmd.zIndex, 5);
    EXPECT_EQ(cmd.material, mat);
    EXPECT_NEAR(cmd.sprite.position.x, 1.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.position.y, 2.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.position.z, 3.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.size.x, 10.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.size.y, 20.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.uvOffset.x, 0.1f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.uvSize.x, 0.5f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.color.r, 1.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.rotation, 0.5f, 1e-5f);
}

TEST_F(Test_DrawCommandBuffer, DrawSubMesh) {
    Material* mat = fakeMat(0x2000);
    Mesh* mesh = reinterpret_cast<Mesh*>(0x3000); // never dereferenced in buffer
    Transform t;
    t.position = {4, 5, 6};

    m_buf.drawSubMesh(mesh, 2, mat, t, 10);

    ASSERT_EQ(m_buf.commands().size(), 1u);
    const auto& cmd = m_buf.commands()[0];
    EXPECT_EQ(cmd.type, DrawCommandType::SubMesh);
    EXPECT_EQ(cmd.zIndex, 10);
    EXPECT_EQ(cmd.material, mat);
    EXPECT_EQ(cmd.submesh.mesh, mesh);
    EXPECT_EQ(cmd.submesh.submeshIndex, 2u);
    EXPECT_NEAR(cmd.submesh.transform.position.x, 4.0f, 1e-5f);
}

TEST_F(Test_DrawCommandBuffer, Clear) {
    Material* mat = fakeMat(0x1000);
    m_buf.drawSprite(mat, 0, {}, {1,1}, {}, {1,1}, Color::white());
    EXPECT_EQ(m_buf.commands().size(), 1u);
    m_buf.clear();
    EXPECT_TRUE(m_buf.commands().empty());
}

// ---------------------------------------------------------------------------
// SortKey tests
// ---------------------------------------------------------------------------

class Test_SortKey : public ::testing::Test {};

TEST_F(Test_SortKey, ZIndexPrimary) {
    DrawCommand a{}, b{};
    a.type = DrawCommandType::Sprite;
    b.type = DrawCommandType::Sprite;
    a.material = fakeMat(0x1000);
    b.material = fakeMat(0x1000);
    a.zIndex = -10;
    b.zIndex = 10;

    EXPECT_LT(a.sortKey(), b.sortKey());
}

TEST_F(Test_SortKey, MaterialSecondary) {
    // Same zIndex, different materials — should produce different sort keys
    DrawCommand a{}, b{};
    a.type = DrawCommandType::Sprite;
    b.type = DrawCommandType::Sprite;
    a.zIndex = 0;
    b.zIndex = 0;
    a.material = fakeMat(0x1000);
    b.material = fakeMat(0x2000);

    // We don't care about the order, just that same-material commands
    // get the same key and different materials get different keys.
    EXPECT_NE(a.sortKey(), b.sortKey());

    // Same material should give same key
    DrawCommand c = a;
    EXPECT_EQ(a.sortKey(), c.sortKey());
}

TEST_F(Test_SortKey, TypeTertiary) {
    // Same zIndex + same material, different type
    DrawCommand a{}, b{};
    a.zIndex = 0;
    b.zIndex = 0;
    a.material = fakeMat(0x1000);
    b.material = fakeMat(0x1000);
    a.type = DrawCommandType::Sprite;
    b.type = DrawCommandType::SubMesh;

    EXPECT_NE(a.sortKey(), b.sortKey());
    // Sprite (0) < SubMesh (1) in the lowest bit
    EXPECT_LT(a.sortKey(), b.sortKey());
}

// ---------------------------------------------------------------------------
// Sort tests
// ---------------------------------------------------------------------------

class Test_BatchSort : public ::testing::Test {};

TEST_F(Test_BatchSort, SortByZIndex) {
    std::vector<DrawCommand> cmds(3);
    for (auto& c : cmds) {
        c.type = DrawCommandType::Sprite;
        c.material = fakeMat(0x1000);
    }
    cmds[0].zIndex = 10;
    cmds[1].zIndex = -5;
    cmds[2].zIndex = 0;

    BatchProcessor::sortCommands(cmds);

    EXPECT_EQ(cmds[0].zIndex, -5);
    EXPECT_EQ(cmds[1].zIndex, 0);
    EXPECT_EQ(cmds[2].zIndex, 10);
}

TEST_F(Test_BatchSort, SameZIndex_GroupByMaterial) {
    std::vector<DrawCommand> cmds(4);
    Material* matA = fakeMat(0x1000);
    Material* matB = fakeMat(0x2000);

    // Interleaved materials: A, B, A, B
    for (auto& c : cmds) {
        c.type = DrawCommandType::Sprite;
        c.zIndex = 0;
    }
    cmds[0].material = matA;
    cmds[1].material = matB;
    cmds[2].material = matA;
    cmds[3].material = matB;

    BatchProcessor::sortCommands(cmds);

    // After sort, same materials should be adjacent
    EXPECT_EQ(cmds[0].material, cmds[1].material);
    EXPECT_EQ(cmds[2].material, cmds[3].material);
    EXPECT_NE(cmds[0].material, cmds[2].material);
}

TEST_F(Test_BatchSort, MixedTypes_SeparatedByType) {
    std::vector<DrawCommand> cmds(4);
    Material* mat = fakeMat(0x1000);

    cmds[0].type = DrawCommandType::SubMesh;
    cmds[1].type = DrawCommandType::Sprite;
    cmds[2].type = DrawCommandType::SubMesh;
    cmds[3].type = DrawCommandType::Sprite;

    for (auto& c : cmds) {
        c.zIndex = 0;
        c.material = mat;
    }

    BatchProcessor::sortCommands(cmds);

    // Sprites (type=0) should come before SubMesh (type=1)
    EXPECT_EQ(cmds[0].type, DrawCommandType::Sprite);
    EXPECT_EQ(cmds[1].type, DrawCommandType::Sprite);
    EXPECT_EQ(cmds[2].type, DrawCommandType::SubMesh);
    EXPECT_EQ(cmds[3].type, DrawCommandType::SubMesh);
}
