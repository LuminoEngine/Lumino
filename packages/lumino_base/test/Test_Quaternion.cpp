#include <lumino_base/math/Math.hpp>
using namespace ln;

class Test_Quaternion : public ::testing::Test {};

TEST(Test_Quaternion, FromEuler_Identity) {
    Quaternion q = Quaternion::fromEuler(0, 0, 0);
    EXPECT_NEAR(q.x, 0.0f, 1e-5f);
    EXPECT_NEAR(q.y, 0.0f, 1e-5f);
    EXPECT_NEAR(q.z, 0.0f, 1e-5f);
    EXPECT_NEAR(q.w, 1.0f, 1e-5f);
}

TEST(Test_Quaternion, Slerp_Endpoints) {
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

TEST(Test_Quaternion, ToMatrix_Identity) {
    Quaternion q{0, 0, 0, 1};
    Matrix4x4 m = Matrix4x4::fromQuaternion(q);
    Matrix4x4 id = Matrix4x4::identity();
    for (int i = 0; i < 16; ++i) {
        EXPECT_NEAR(m.m[i], id.m[i], 1e-5f);
    }
}