#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <cmath>

using namespace ln;

//------------------------------------------------------------------------------
// ヘルパー: ソートキーのテスト用の偽の Material ポインタ (参照はしない)
//------------------------------------------------------------------------------
static Material* fakeMat(uintptr_t id) { return reinterpret_cast<Material*>(id); }

//------------------------------------------------------------------------------
// DrawCommandBuffer のテスト
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
    Mesh* mesh = reinterpret_cast<Mesh*>(0x3000); // バッファ内では参照されない
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
// SortKey のテスト
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
    // 同一 zIndex かつ同一 type のスプライトは投入順 (sequence) でキーが決まる。
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
    // 同一 zIndex かつ同一マテリアルで、type だけが異なる
    DrawCommand a{}, b{};
    a.zIndex = 0;
    b.zIndex = 0;
    a.material = fakeMat(0x1000);
    b.material = fakeMat(0x1000);
    a.type = DrawCommandType::Sprite;
    b.type = DrawCommandType::SubMesh;

    EXPECT_NE(a.sortKey(), b.sortKey());
    // 最下位ビットで Sprite (0) < SubMesh (1)
    EXPECT_LT(a.sortKey(), b.sortKey());
}

//------------------------------------------------------------------------------
// ソートのテスト
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
    // 同一 zIndex の SubMesh 描画は、マテリアルのアドレスに関係なく投入順を保持しなければならない。
    // flushSubMeshGroup は各サブメッシュを個別に描画する (マテリアルでバッチ化しても得が無い) ため、
    // 並べ替えると深度テスト/書き込み無効の重ね描きのような順序依存の描画が壊れる。
    // スプライトもサブメッシュも sequence による同順位の決着で投入順を保持する。
    DrawCommandBuffer buf;
    Material* matA = fakeMat(0x2000);
    Material* matB = fakeMat(0x1000); // 意図的に matA より低いアドレス
    Mesh* mesh = reinterpret_cast<Mesh*>(0x3000);
    Transform t;

    buf.drawSubMesh(mesh, 0, matA, t, 0); // 1 番目に投入
    buf.drawSubMesh(mesh, 0, matB, t, 0); // 2 番目に投入

    std::vector<DrawCommand> cmds = buf.commands();
    BatchProcessor::sortCommands(cmds);

    EXPECT_EQ(cmds[0].material, matA); // 投入順が保持される
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
// SpriteMeshPool のテスト
//
// フレーム内の全スプライトフラッシュが単一の共有 DynamicMesh バッファを再利用していた
// バグの回帰防止。描画は 1 つのコマンドエンコーダに記録され submit 時にのみ実行されるため、
// バッファを共有すると後のフラッシュ (例: UI スプライト) が先のフラッシュ (例: 背景スプライト)
// を上書きし、背景が自身のジオメトリではなく最後のフラッシュのジオメトリで描かれていた。
// プールはフレーム内の各フラッシュに別々のスロットを渡し、フレームをまたいでスロットを
// 再利用しなければならない。
//------------------------------------------------------------------------------

class Test_SpriteMeshPool : public ::testing::Test {};

TEST_F(Test_SpriteMeshPool, DistinctSlotsWithinFrame) {
    SpriteMeshPool pool;

    // フレーム内の各フラッシュは、2 つのフラッシュがバッファを共有しないよう、
    // 別々の単調増加するスロットを受け取らなければならない。
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

    // 新しいフレーム: カーソルが巻き戻り、スロットは再確保ではなく再利用される。
    pool.resetFrame();
    EXPECT_EQ(pool.frameCursor(), 0u);
    EXPECT_EQ(pool.acquireSlot(), 0u);
    EXPECT_EQ(pool.acquireSlot(), 1u);
    EXPECT_EQ(pool.slotCount(), 2u); // 増えない: 同じ 2 つのスロットが再利用される
}

TEST_F(Test_SpriteMeshPool, SlotCountGrowsToPeakFlushesPerFrame) {
    SpriteMeshPool pool;

    // フレーム 1: フラッシュ 2 回。
    pool.acquireSlot();
    pool.acquireSlot();
    pool.resetFrame();

    // フレーム 2: フラッシュ 4 回 -> プールは新しいピークまで成長する。
    for (int i = 0; i < 4; ++i) pool.acquireSlot();
    EXPECT_EQ(pool.slotCount(), 4u);

    // フレーム 3: フラッシュ 1 回 -> プールはピーク時の容量を保持する (スロットは再利用のため保持)。
    pool.resetFrame();
    pool.acquireSlot();
    EXPECT_EQ(pool.slotCount(), 4u);
}

TEST_F(Test_SpriteMeshPool, GrowCapacityDoublesFromBase) {
    // 空のスロットは 256 スプライトの基準値まで成長し、以降は要求に収まるまで倍々になる。
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 1), 256u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 256), 256u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 257), 512u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(0, 1000), 1024u);

    // サイズ確保済みのスロットは、要求が容量を超えたときだけ成長する。
    EXPECT_EQ(SpriteMeshPool::growCapacity(512, 100), 512u);
    EXPECT_EQ(SpriteMeshPool::growCapacity(512, 600), 1024u);
}

TEST_F(Test_SpriteMeshPool, SlotCapacityRetainedAcrossFrames) {
    SpriteMeshPool pool;

    // フレーム 1: 最初のフラッシュに 300 スプライトが必要 -> 呼び出し側は容量 512 を確保する。
    uint32_t idx = pool.acquireSlot();
    auto& slot = pool.slotAt(idx);
    ASSERT_LT(slot.capacity, 300u); // 新規作成されたスロットは空で始まる
    slot.capacity = SpriteMeshPool::growCapacity(slot.capacity, 300);
    EXPECT_EQ(slot.capacity, 512u);

    // フレーム 2: 同じスロットをより小さいフラッシュに再利用 -> 再確保は不要。
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

    // Sprite (type=0) が SubMesh (type=1) より前に来るはず
    EXPECT_EQ(cmds[0].type, DrawCommandType::Sprite);
    EXPECT_EQ(cmds[1].type, DrawCommandType::Sprite);
    EXPECT_EQ(cmds[2].type, DrawCommandType::SubMesh);
    EXPECT_EQ(cmds[3].type, DrawCommandType::SubMesh);
}

//------------------------------------------------------------------------------
// BatchProcessor::spriteNormal のテスト
// drawSprite の頂点の法線は、transform の逆転置で (0, 0, 1) を変換したワールド座標の法線。
//------------------------------------------------------------------------------

// 3x3 の部分を列 (ローカルの x・y・z 軸の写り先) で指定して行列を作る。平行移動は適当な値。
static Matrix4x4 fromColumns(const Vector3& x, const Vector3& y, const Vector3& z) {
    return Matrix4x4(x.x, x.y, x.z, 0,
                     y.x, y.y, y.z, 0,
                     z.x, z.y, z.z, 0,
                     5, -3, 7, 1);
}

static void expectVecNear(const Vector3& a, const Vector3& b, float eps = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

// 列優先の m で、M の第 i 列 (ローカルの軸の写り先)
static Vector3 column(const Matrix4x4& m, int i) {
    return {m.m[i * 4 + 0], m.m[i * 4 + 1], m.m[i * 4 + 2]};
}

TEST(Test_SpriteNormal, LyridraFloor) {
    // y と z の入れ替え (行列式 -1)
    Matrix4x4 m = fromColumns({1, 0, 0}, {0, 0, 1}, {0, 1, 0});
    expectVecNear(BatchProcessor::spriteNormal(m), {0, 1, 0});
}

TEST(Test_SpriteNormal, LyridraStandingBoard) {
    // 行列式 -1
    Matrix4x4 m = fromColumns({1, 0, 0}, {0, -1, 1}, {0, 1, 0});
    const float s = 1.0f / std::sqrt(2.0f);
    expectVecNear(BatchProcessor::spriteNormal(m), {0, s, s});
}

TEST(Test_SpriteNormal, Transforms2DKeepUnitZ) {
    const Matrix4x4 cases[] = {
        Matrix4x4::identity(),
        Matrix4x4::rotateZ(0.8f),
        Matrix4x4::scale({2.0f, 0.5f, 1.0f}),
        Matrix4x4::scale({-1.0f, 1.0f, 1.0f}),
        Matrix4x4::scale({1.0f, -3.0f, 1.0f}),
        Matrix4x4::translate({100, 50, 0}) * Matrix4x4::rotateZ(-2.1f) * Matrix4x4::scale({-2.0f, 3.0f, 1.0f}),
    };
    for (const auto& m : cases) {
        expectVecNear(BatchProcessor::spriteNormal(m), {0, 0, 1});
    }
}

TEST(Test_SpriteNormal, GeneralMatrixPerpendicularToEdges) {
    // x 軸まわりの回転・せん断・非一様な拡大・鏡映を含む一般の M
    const Matrix4x4 shear = fromColumns({1, 0, 0}, {0.4f, 1, 0}, {0.2f, -0.3f, 1});
    const Matrix4x4 cases[] = {
        Matrix4x4::rotateX(0.7f) * shear * Matrix4x4::scale({2.0f, 0.5f, 3.0f}),
        Matrix4x4::rotateY(1.2f) * Matrix4x4::rotateX(-0.4f) * shear * Matrix4x4::scale({-1.5f, 2.0f, 0.7f}),
        fromColumns({1, 2, 0.5f}, {-0.3f, 1, 2}, {0.1f, 0.2f, -1}),
    };
    for (const auto& m : cases) {
        Vector3 n = BatchProcessor::spriteNormal(m);
        EXPECT_NEAR(n.length(), 1.0f, 1e-5f);
        EXPECT_NEAR(Vector3::dot(n, column(m, 0).normalized()), 0.0f, 1e-5f);
        EXPECT_NEAR(Vector3::dot(n, column(m, 1).normalized()), 0.0f, 1e-5f);
        // 逆転置の定義どおり: n と M の z 軸の写り先の内積は正 (ローカルの n・z = 1 > 0 が保たれる)
        EXPECT_GT(Vector3::dot(n, column(m, 2)), 0.0f);
    }
}

TEST(Test_SpriteNormal, MatchesDrawMeshNormalMatrix) {
    // drawMesh の経路 (Transform::normalMatrix() で (0, 0, 1) を変換) と同じ結果になること
    Transform cases[3];
    cases[0].rotation = Quaternion::fromAxisAngle({1, 0, 0}, 0.7f);
    cases[0].scale = {2.0f, 0.5f, 3.0f};
    cases[1].position = {1, 2, 3};
    cases[1].rotation = Quaternion::fromAxisAngle(Vector3{1, 1, 0}.normalized(), -1.3f);
    cases[1].scale = {-1.5f, 2.0f, 0.7f};
    cases[2].rotation = Quaternion::fromAxisAngle(Vector3{0.2f, -1, 0.5f}.normalized(), 2.4f);
    cases[2].scale = {0.3f, -4.0f, -2.0f};
    for (const auto& t : cases) {
        // normalMatrix * (0, 0, 1, 0) は第 2 列
        Vector3 expected = column(t.normalMatrix(), 2).normalized();
        expectVecNear(BatchProcessor::spriteNormal(t.matrix()), expected);
    }
}

TEST(Test_SpriteNormal, DegenerateReturnsUnitZ) {
    const Matrix4x4 cases[] = {
        Matrix4x4::scale({0.0f, 1.0f, 1.0f}),
        Matrix4x4::scale({1.0f, 0.0f, 1.0f}),
        Matrix4x4::scale({1.0f, 1.0f, 0.0f}),
        Matrix4x4::rotateX(0.7f) * Matrix4x4::scale({0.0f, 0.0f, 0.0f}),
        fromColumns({1, 0, 0}, {2, 0, 0}, {0, 0, 1}),
    };
    for (const auto& m : cases) {
        Vector3 n = BatchProcessor::spriteNormal(m);
        EXPECT_FALSE(std::isnan(n.x) || std::isnan(n.y) || std::isnan(n.z));
        expectVecNear(n, {0, 0, 1});
    }
}
