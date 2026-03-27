//------------------------------------------------------------------------------
// Environment: 初期化・終了処理
//------------------------------------------------------------------------------
class LuminoCoreEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

//------------------------------------------------------------------------------
// Graphics module tests (CPU-only, no GPU)
//------------------------------------------------------------------------------
#include <lumino_core/graphics/Vertex.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <lumino_core/graphics/Camera.hpp>
#include <lumino_base/math/Math.hpp>
#include <cmath>

using namespace ln;

TEST(VertexTest, Size) {
    EXPECT_EQ(sizeof(Vertex), 64u);
}

TEST(VertexTest, StandardLayout) {
    rhi::VertexBufferLayout layout = standardVertexLayout();
    EXPECT_EQ(layout.stride, 64u);
    EXPECT_EQ(layout.attributes.size(), 5u);
    // position at offset 0
    EXPECT_EQ(layout.attributes[0].offset, 0u);
    // normal at offset 12
    EXPECT_EQ(layout.attributes[1].offset, 12u);
}

TEST(TransformTest, IdentityMatrix) {
    Transform t;
    Matrix4x4 m = t.matrix();
    Matrix4x4 id = Matrix4x4::identity();
    for (int i = 0; i < 16; ++i)
        EXPECT_NEAR(m.m[i], id.m[i], 1e-5f);
}

TEST(TransformTest, Translation) {
    Transform t;
    t.position = Vector3{1, 2, 3};
    Matrix4x4 m = t.matrix();
    EXPECT_NEAR(m.m[12], 1.0f, 1e-5f);
    EXPECT_NEAR(m.m[13], 2.0f, 1e-5f);
    EXPECT_NEAR(m.m[14], 3.0f, 1e-5f);
}

TEST(TransformTest, Scale) {
    Transform t;
    t.scale = Vector3{2, 3, 4};
    Matrix4x4 m = t.matrix();
    EXPECT_NEAR(m.m[0], 2.0f, 1e-5f);
    EXPECT_NEAR(m.m[5], 3.0f, 1e-5f);
    EXPECT_NEAR(m.m[10], 4.0f, 1e-5f);
}

TEST(CameraTest, ViewProjectionNotIdentity) {
    Camera cam;
    cam.setPerspective(1.0472f, 1280.0f / 720.0f, 0.1f, 100.0f);
    cam.setLookAt(Vector3{0, 0, 5}, Vector3{0, 0, 0});

    Matrix4x4 vp = cam.viewProjectionMatrix();
    Matrix4x4 id = Matrix4x4::identity();
    bool allSame = true;
    for (int i = 0; i < 16; ++i) {
        if (std::abs(vp.m[i] - id.m[i]) > 1e-5f) {
            allSame = false;
            break;
        }
    }
    EXPECT_FALSE(allSame);
}

//------------------------------------------------------------------------------
// エントリポイント
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoCoreEnvironment());
    return RUN_ALL_TESTS();
}
