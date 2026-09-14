#include <LuminoBase/math/Math.hpp>
using namespace ln;

class Test_Quaternion : public ::testing::Test {};

static constexpr float kPi = 3.14159265358979323846f;

static void expectQuatNear(const Quaternion& actual, const Quaternion& expected, float eps = 1e-5f) {
    EXPECT_NEAR(actual.x, expected.x, eps);
    EXPECT_NEAR(actual.y, expected.y, eps);
    EXPECT_NEAR(actual.z, expected.z, eps);
    EXPECT_NEAR(actual.w, expected.w, eps);
}

TEST(Test_Quaternion, FromEuler_Identity) {
    expectQuatNear(Quaternion::fromEuler(0, 0, 0), Quaternion{0, 0, 0, 1});
}

// pitch=X, yaw=Y, roll=Z の軸割り当てを検証する。
// fromEuler(0, 0, 0) は軸が入れ替わっていても単位クォータニオンになるため、
// 各軸を個別に回転させて確認する必要がある。
TEST(Test_Quaternion, FromEuler_AxisAssignment) {
    const float a = kPi * 0.5f;
    expectQuatNear(Quaternion::fromEuler(a, 0, 0), Quaternion::fromAxisAngle(Vector3::unitX(), a));
    expectQuatNear(Quaternion::fromEuler(0, a, 0), Quaternion::fromAxisAngle(Vector3::unitY(), a));
    expectQuatNear(Quaternion::fromEuler(0, 0, a), Quaternion::fromAxisAngle(Vector3::unitZ(), a));
}

// 合成順は roll(Z) -> pitch(X) -> yaw(Y)、すなわち q = Qy(yaw) * Qx(pitch) * Qz(roll)。
TEST(Test_Quaternion, FromEuler_CompositionOrder) {
    const float pitch = 0.3f, yaw = 0.7f, roll = -0.4f;
    Quaternion actual = Quaternion::fromEuler(pitch, yaw, roll);
    Quaternion expected = Quaternion::fromAxisAngle(Vector3::unitY(), yaw) *
                          Quaternion::fromAxisAngle(Vector3::unitX(), pitch) *
                          Quaternion::fromAxisAngle(Vector3::unitZ(), roll);
    expectQuatNear(actual, expected);
}

TEST(Test_Quaternion, Slerp_Endpoints) {
    Quaternion a{0, 0, 0, 1};
    // fromAxisAngle はラジアンを取る。90 度は pi/2。
    Quaternion b = Quaternion::fromAxisAngle({0, 1, 0}, kPi * 0.5f);

    expectQuatNear(Quaternion::slerp(a, b, 0.0f), a);
    expectQuatNear(Quaternion::slerp(a, b, 1.0f), b, 1e-4f);
}

TEST(Test_Quaternion, ToMatrix_Identity) {
    Quaternion q{0, 0, 0, 1};
    Matrix4x4 m = Matrix4x4::fromQuaternion(q);
    Matrix4x4 id = Matrix4x4::identity();
    for (int i = 0; i < 16; ++i) {
        EXPECT_NEAR(m.m[i], id.m[i], 1e-5f);
    }
}