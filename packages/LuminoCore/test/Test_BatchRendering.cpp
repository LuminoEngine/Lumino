#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <cmath>

using namespace ln;

//------------------------------------------------------------------------------
// Helpers: fake Material pointers for sort-key testing (never dereferenced)
//------------------------------------------------------------------------------
static Material* fakeMat(uintptr_t id) { return reinterpret_cast<Material*>(id); }

//------------------------------------------------------------------------------
// DrawCommandBuffer tests
//------------------------------------------------------------------------------

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
    Matrix4x4 xf = Matrix4x4::translate(Vector3{1, 2, 3});
    m_buf.drawSprite(mat, 5,
        xf,
        Vector2{7, 8},
        Vector2{10, 20},
        Vector2{0.25f, 0.75f},
        Vector2{0.1f, 0.2f}, Vector2{0.5f, 0.6f},
        Color{1, 0, 0, 1});

    ASSERT_EQ(m_buf.commands().size(), 1u);
    const auto& cmd = m_buf.commands()[0];
    EXPECT_EQ(cmd.type, DrawCommandType::Sprite);
    EXPECT_EQ(cmd.zIndex, 5);
    EXPECT_EQ(cmd.material, mat);
    // 平行移動成分 (列優先: m[12..14])
    EXPECT_NEAR(cmd.sprite.transform.m[12], 1.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.transform.m[13], 2.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.transform.m[14], 3.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.offset.x, 7.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.offset.y, 8.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.size.x, 10.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.size.y, 20.0f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.pivot.x, 0.25f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.pivot.y, 0.75f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.uvOffset.x, 0.1f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.uvSize.x, 0.5f, 1e-5f);
    EXPECT_NEAR(cmd.sprite.color.r, 1.0f, 1e-5f);
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
    m_buf.drawSprite(mat, 0, {}, {}, {1,1}, {0.5f,0.5f}, {}, {1,1}, Color::white());
    EXPECT_EQ(m_buf.commands().size(), 1u);
    m_buf.clear();
    EXPECT_TRUE(m_buf.commands().empty());
}

//------------------------------------------------------------------------------
// SortKey tests
//------------------------------------------------------------------------------

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

TEST_F(Test_SortKey, SequenceSecondary) {
    // 同一 zIndex・同一 type のスプライトは投入順 (sequence) でキーが決まる。
    // マテリアルのアドレスはキーに影響しない (旧実装のマテリアルハッシュ tiebreak を廃止し、
    // 「後から描いたものが前面」というペインターズアルゴリズムを保証するため)。
    DrawCommand a{}, b{};
    a.type = DrawCommandType::Sprite;
    b.type = DrawCommandType::Sprite;
    a.zIndex = 0;
    b.zIndex = 0;
    a.material = fakeMat(0x2000); // a の方がアドレスは大きいが…
    b.material = fakeMat(0x1000);
    a.sequence = 0;               // …先に投入された a のキーが小さくなる
    b.sequence = 1;

    EXPECT_LT(a.sortKey(), b.sortKey());

    // マテリアルが違っても sequence が同じならキーは等しい (マテリアルはキーに無関係)。
    DrawCommand c = a;
    c.material = fakeMat(0x9999);
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

//------------------------------------------------------------------------------
// Sort tests
//------------------------------------------------------------------------------

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

TEST_F(Test_BatchSort, SameZIndex_SpritePreservesSubmissionOrder) {
    // 同一 zIndex のスプライトはマテリアルで並べ替えず投入順を厳密に保持する。
    // 半透明スプライトでは「後から描いたものが前面」というペインターズアルゴリズムを
    // 守る必要があり、マテリアルアドレス順に並べ替えると前後関係が壊れるため。
    // 連続する同一マテリアルは flushSpriteGroup 側で 1 サブメッシュに束ねられるので、
    // 投入順を保ってもよくあるケースでは draw 数は増えない。
    DrawCommandBuffer buf;
    Material* matA = fakeMat(0x2000);
    Material* matB = fakeMat(0x1000); // 意図的に matA より低いアドレス

    // 交互に投入: A, B, A, B
    buf.drawSprite(matA, 0, {}, {}, {1, 1}, {0.5f, 0.5f}, {}, {1, 1}, Color::white());
    buf.drawSprite(matB, 0, {}, {}, {1, 1}, {0.5f, 0.5f}, {}, {1, 1}, Color::white());
    buf.drawSprite(matA, 0, {}, {}, {1, 1}, {0.5f, 0.5f}, {}, {1, 1}, Color::white());
    buf.drawSprite(matB, 0, {}, {}, {1, 1}, {0.5f, 0.5f}, {}, {1, 1}, Color::white());

    std::vector<DrawCommand> cmds = buf.commands();
    BatchProcessor::sortCommands(cmds);

    // 投入順 A, B, A, B のまま保持される (マテリアルでグループ化しない)。
    EXPECT_EQ(cmds[0].material, matA);
    EXPECT_EQ(cmds[1].material, matB);
    EXPECT_EQ(cmds[2].material, matA);
    EXPECT_EQ(cmds[3].material, matB);
}

TEST_F(Test_BatchSort, SameZIndex_SubMeshPreservesSubmissionOrder) {
    // SubMesh draws at the same zIndex must keep submission order, independent of
    // material address. flushSubMeshGroup draws each submesh individually (no
    // material batching to gain), and reordering would break order-dependent draws
    // such as depth-test/-write-disabled overdraw. Both sprites and submeshes now
    // preserve submission order via the sequence tiebreaker.
    DrawCommandBuffer buf;
    Material* matA = fakeMat(0x2000);
    Material* matB = fakeMat(0x1000); // intentionally lower address than matA
    Mesh* mesh = reinterpret_cast<Mesh*>(0x3000);
    Transform t;

    buf.drawSubMesh(mesh, 0, matA, t, 0); // submitted 1st
    buf.drawSubMesh(mesh, 0, matB, t, 0); // submitted 2nd

    std::vector<DrawCommand> cmds = buf.commands();
    BatchProcessor::sortCommands(cmds);

    EXPECT_EQ(cmds[0].material, matA); // submission order preserved
    EXPECT_EQ(cmds[1].material, matB);
}

// SortMode::BackToFront: 同一 zIndex 内をビュー平面からの距離が大きい順 (奥→手前) に並べる。
// viewDepth は flush() がビュー行列から算出するが、ここでは並べ替えロジックの検証のため直接設定する。
TEST_F(Test_BatchSort, DepthSort_BackToFront) {
    std::vector<DrawCommand> cmds(3);
    for (auto& c : cmds) { c.type = DrawCommandType::Sprite; c.zIndex = 0; c.material = fakeMat(0x1000); }
    cmds[0].sequence = 0; cmds[0].viewDepth = 5.0f;   // 中間
    cmds[1].sequence = 1; cmds[1].viewDepth = 10.0f;  // 最奥
    cmds[2].sequence = 2; cmds[2].viewDepth = 1.0f;   // 最手前

    BatchProcessor::sortCommands(cmds, SortMode::BackToFront);

    // 奥 (大きい viewDepth) から先に描く。
    EXPECT_FLOAT_EQ(cmds[0].viewDepth, 10.0f);
    EXPECT_FLOAT_EQ(cmds[1].viewDepth, 5.0f);
    EXPECT_FLOAT_EQ(cmds[2].viewDepth, 1.0f);
}

// SortMode::FrontToBack: 手前 (小さい viewDepth) から先に描く。
TEST_F(Test_BatchSort, DepthSort_FrontToBack) {
    std::vector<DrawCommand> cmds(3);
    for (auto& c : cmds) { c.type = DrawCommandType::Sprite; c.zIndex = 0; c.material = fakeMat(0x1000); }
    cmds[0].sequence = 0; cmds[0].viewDepth = 5.0f;
    cmds[1].sequence = 1; cmds[1].viewDepth = 10.0f;
    cmds[2].sequence = 2; cmds[2].viewDepth = 1.0f;

    BatchProcessor::sortCommands(cmds, SortMode::FrontToBack);

    EXPECT_FLOAT_EQ(cmds[0].viewDepth, 1.0f);
    EXPECT_FLOAT_EQ(cmds[1].viewDepth, 5.0f);
    EXPECT_FLOAT_EQ(cmds[2].viewDepth, 10.0f);
}

// zIndex は深度モードでも主キー。深度より優先される (エンジン指定のレイヤが常に勝つ)。
TEST_F(Test_BatchSort, DepthSort_ZIndexDominatesDepth) {
    std::vector<DrawCommand> cmds(2);
    for (auto& c : cmds) { c.type = DrawCommandType::Sprite; c.material = fakeMat(0x1000); }
    // zIndex が小さい方を、たとえ手前 (depth 大) でも先に描く。
    cmds[0].zIndex = 1; cmds[0].sequence = 0; cmds[0].viewDepth = 100.0f;
    cmds[1].zIndex = 0; cmds[1].sequence = 1; cmds[1].viewDepth = 0.0f;

    BatchProcessor::sortCommands(cmds, SortMode::BackToFront);

    EXPECT_EQ(cmds[0].zIndex, 0); // zIndex=0 が先 (深度に関わらず)
    EXPECT_EQ(cmds[1].zIndex, 1);
}

// 深度が等しい場合は投入順 (sequence) で安定化する。
TEST_F(Test_BatchSort, DepthSort_EqualDepthKeepsSubmissionOrder) {
    std::vector<DrawCommand> cmds(3);
    for (auto& c : cmds) {
        c.type = DrawCommandType::Sprite; c.zIndex = 0; c.material = fakeMat(0x1000);
        c.viewDepth = 3.0f; // 全て同じ距離
    }
    cmds[0].sequence = 0;
    cmds[1].sequence = 1;
    cmds[2].sequence = 2;

    BatchProcessor::sortCommands(cmds, SortMode::BackToFront);

    EXPECT_EQ(cmds[0].sequence, 0u);
    EXPECT_EQ(cmds[1].sequence, 1u);
    EXPECT_EQ(cmds[2].sequence, 2u);
}

//------------------------------------------------------------------------------
// SpriteMeshPool tests
//
// Regression guard for the bug where every sprite flush in a frame reused a single
// shared DynamicMesh buffer. Because draws are recorded into one command encoder
// and executed only at submit, a shared buffer let later flushes (e.g. UI sprites)
// overwrite earlier flushes (e.g. the background sprite), so the background drew
// the last flush's geometry instead of its own. The pool must hand each flush a
// distinct slot within a frame, and recycle slots across frames.
//------------------------------------------------------------------------------

class Test_SpriteMeshPool : public ::testing::Test {};

TEST_F(Test_SpriteMeshPool, DistinctSlotsWithinFrame) {
    SpriteMeshPool pool;

    // Each flush in a frame must get a distinct, monotonically increasing slot so
    // that no two flushes share a buffer.
    EXPECT_EQ(pool.acquireSlot(), 0u);
    EXPECT_EQ(pool.acquireSlot(), 1u);
    EXPECT_EQ(pool.acquireSlot(), 2u);
    EXPECT_EQ(pool.slotCount(), 3u);
    EXPECT_EQ(pool.frameCursor(), 3u);
}

TEST_F(Test_SpriteMeshPool, ResetFrameRecyclesSlots) {
    SpriteMeshPool pool;

    pool.acquireSlot();
    pool.acquireSlot();
    EXPECT_EQ(pool.slotCount(), 2u);

    // New frame: cursor rewinds and slots are reused rather than reallocated.
    pool.resetFrame();
    EXPECT_EQ(pool.frameCursor(), 0u);
    EXPECT_EQ(pool.acquireSlot(), 0u);
    EXPECT_EQ(pool.acquireSlot(), 1u);
    EXPECT_EQ(pool.slotCount(), 2u); // no growth: same two slots reused
}

TEST_F(Test_SpriteMeshPool, SlotCountGrowsToPeakFlushesPerFrame) {
    SpriteMeshPool pool;

    // Frame 1: 2 flushes.
    pool.acquireSlot();
    pool.acquireSlot();
    pool.resetFrame();

    // Frame 2: 4 flushes -> pool grows to the new peak.
    for (int i = 0; i < 4; ++i) pool.acquireSlot();
    EXPECT_EQ(pool.slotCount(), 4u);

    // Frame 3: 1 flush -> pool retains its peak capacity (slots kept for reuse).
    pool.resetFrame();
    pool.acquireSlot();
    EXPECT_EQ(pool.slotCount(), 4u);
}

TEST_F(Test_SpriteMeshPool, GrowCapacityDoublesFromBase) {
    // Empty slot grows to the 256-sprite base, then doubles to fit the request.
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 1), 256u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 256), 256u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 257), 512u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 1000), 1024u);

    // An already-sized slot only grows when the request exceeds it.
    EXPECT_EQ(SpriteMeshPool::growCapacity(512, 100), 512u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(512, 600), 1024u);
}

TEST_F(Test_SpriteMeshPool, SlotCapacityRetainedAcrossFrames) {
    SpriteMeshPool pool;

    // Frame 1: first flush needs 300 sprites -> caller allocates 512 capacity.
    uint32_t idx = pool.acquireSlot();
    auto& slot = pool.slotAt(idx);
    ASSERT_LT(slot.capacity, 300u); // freshly created slot starts empty
    slot.capacity = SpriteMeshPool::growCapacity(slot.capacity, 300);
    EXPECT_EQ(slot.capacity, 512u);

    // Frame 2: same slot reused for a smaller flush -> no reallocation needed.
    pool.resetFrame();
    uint32_t idx2 = pool.acquireSlot();
    EXPECT_EQ(idx2, idx);
    EXPECT_EQ(pool.slotAt(idx2).capacity, 512u);
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
