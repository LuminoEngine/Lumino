#include <lumino_base/math/Math.hpp>
using namespace ln;

class Test_Vector3 : public ::testing::Test {};

TEST(Test_Vector3, Normalize) {
    Vector3 v{3.0f, 0.0f, 4.0f};
    Vector3 n = v.normalized();
    float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    EXPECT_NEAR(len, 1.0f, 1e-5f);
    EXPECT_NEAR(n.x, 0.6f, 1e-5f);
    EXPECT_NEAR(n.z, 0.8f, 1e-5f);
}
