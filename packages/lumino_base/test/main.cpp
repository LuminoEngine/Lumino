//------------------------------------------------------------------------------
// Environment: 初期化・終了処理
//------------------------------------------------------------------------------
class LuminoBaseEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

//------------------------------------------------------------------------------
// Math tests
//------------------------------------------------------------------------------
#include <lumino_base/Math.hpp>
#include <cmath>

using namespace ln;

TEST(MathTest, Vector3_Normalize) {
    Vector3 v{3.0f, 0.0f, 4.0f};
    Vector3 n = v.normalized();
    float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    EXPECT_NEAR(len, 1.0f, 1e-5f);
    EXPECT_NEAR(n.x, 0.6f, 1e-5f);
    EXPECT_NEAR(n.z, 0.8f, 1e-5f);
}

TEST(MathTest, Matrix4x4_Identity) {
    Matrix4x4 m = Matrix4x4::identity();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(m.m[i * 4 + j], (i == j) ? 1.0f : 0.0f);
}

TEST(MathTest, Matrix4x4_Translate) {
    Matrix4x4 t = Matrix4x4::translate(Vector3{1.0f, 2.0f, 3.0f});
    // Column-major: translation is in m[12], m[13], m[14]
    EXPECT_FLOAT_EQ(t.m[12], 1.0f);
    EXPECT_FLOAT_EQ(t.m[13], 2.0f);
    EXPECT_FLOAT_EQ(t.m[14], 3.0f);
}

TEST(MathTest, Matrix4x4_Inverse_Identity) {
    Matrix4x4 id = Matrix4x4::identity();
    Matrix4x4 inv = id.inversed();
    for (int i = 0; i < 16; ++i)
        EXPECT_NEAR(id.m[i], inv.m[i], 1e-5f);
}

TEST(MathTest, Matrix4x4_Inverse_Translation) {
    Matrix4x4 t = Matrix4x4::translate(Vector3{5.0f, -3.0f, 7.0f});
    Matrix4x4 inv = t.inversed();
    // Inverse of translate(a,b,c) should be translate(-a,-b,-c)
    EXPECT_NEAR(inv.m[12], -5.0f, 1e-5f);
    EXPECT_NEAR(inv.m[13],  3.0f, 1e-5f);
    EXPECT_NEAR(inv.m[14], -7.0f, 1e-5f);
}

TEST(MathTest, Matrix4x4_Transpose) {
    Matrix4x4 m = Matrix4x4::identity();
    m.m[1] = 2.0f; // row 1, col 0 (column-major)
    Matrix4x4 t = m.transposed();
    EXPECT_FLOAT_EQ(t.m[4], 2.0f); // should be at row 0, col 1
    EXPECT_FLOAT_EQ(t.m[1], 0.0f);
}

TEST(MathTest, Quaternion_FromEuler_Identity) {
    Quaternion q = Quaternion::fromEuler(0, 0, 0);
    EXPECT_NEAR(q.x, 0.0f, 1e-5f);
    EXPECT_NEAR(q.y, 0.0f, 1e-5f);
    EXPECT_NEAR(q.z, 0.0f, 1e-5f);
    EXPECT_NEAR(q.w, 1.0f, 1e-5f);
}

TEST(MathTest, Quaternion_Slerp_Endpoints) {
    Quaternion a{0, 0, 0, 1};
    Quaternion b = Quaternion::fromAxisAngle({0, 1, 0}, 90.0f);

    Quaternion s0 = Quaternion::slerp(a, b, 0.0f);
    EXPECT_NEAR(s0.x, a.x, 1e-5f);
    EXPECT_NEAR(s0.y, a.y, 1e-5f);
    EXPECT_NEAR(s0.z, a.z, 1e-5f);
    EXPECT_NEAR(s0.w, a.w, 1e-5f);

    Quaternion s1 = Quaternion::slerp(a, b, 1.0f);
    EXPECT_NEAR(s1.x, b.x, 1e-4f);
    EXPECT_NEAR(s1.y, b.y, 1e-4f);
    EXPECT_NEAR(s1.z, b.z, 1e-4f);
    EXPECT_NEAR(s1.w, b.w, 1e-4f);
}

TEST(MathTest, Quaternion_ToMatrix_Identity) {
    Quaternion q{0, 0, 0, 1};
    Matrix4x4 m = Matrix4x4::fromQuaternion(q);
    Matrix4x4 id = Matrix4x4::identity();
    for (int i = 0; i < 16; ++i)
        EXPECT_NEAR(m.m[i], id.m[i], 1e-5f);
}

//------------------------------------------------------------------------------
// エントリポイント
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoBaseEnvironment());
    return RUN_ALL_TESTS();
}
