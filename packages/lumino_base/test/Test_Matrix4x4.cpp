// clang-format off
#include <lumino_base/math/Math.hpp>
#include <cmath>
#include <iostream>
#define GLM_TEST 0

using namespace ln;

class Test_Matrix4x4 : public ::testing::Test {};

static constexpr f32 kPi = 3.14159265358979323846f;
static constexpr f32 kEps = 1e-5f;

// float の演算方法がコンパイラによって異なり、結果が微妙に違くなる。
// その差を吸収するため、比較する桁数を指定する。
// (具体的にどう異なるのかは調査中。VS2010 と VS2013 では Vector3::project() の誤差が一番大きかった)
#define LN_FLOAT_THRESHOLD 0.0001f

// Helper: multiply Matrix4x4 by Vector4 (M * v, column-vector convention).
static Vector4 mulMV(const Matrix4x4& M, const Vector4& v) {
    Vector4 r;
    r.x = M.m[0] * v.x + M.m[1] * v.y + M.m[2] * v.z + M.m[3] * v.w;
    r.y = M.m[4] * v.x + M.m[5] * v.y + M.m[6] * v.z + M.m[7] * v.w;
    r.z = M.m[8] * v.x + M.m[9] * v.y + M.m[10] * v.z + M.m[11] * v.w;
    r.w = M.m[12] * v.x + M.m[13] * v.y + M.m[14] * v.z + M.m[15] * v.w;
    return r;
}

static bool matNear(const Matrix4x4& a, const Matrix4x4& b, f32 eps = kEps) {
    for (int i = 0; i < 16; ++i)
        if (std::abs(a.m[i] - b.m[i]) > eps) return false;
    return true;
}

void printMat(const glm::mat4& mat) {
    float data[16]; 
    std::memcpy(data, &mat, sizeof(data));
    for (int i = 0; i < 16; i++) {
        printf("%.6f, ", data[i]);
        if ((i + 1) % 4 == 0) printf("\n");
    }
}

#define ASSERT_MAT(f0, f1, f2, f3, f4, f5, f6 ,f7, f8, f9, f10, f11, f12, f13, f14, f15, actual) \
    do { \
        float expected[16] = {f0, f1, f2, f3, f4, f5, f6 ,f7, f8, f9, f10, f11, f12, f13, f14, f15}; \
        float data[16]; \
        std::memcpy(data, &actual, sizeof(data)); \
        for (int i = 0; i < 16; ++i) { \
            ASSERT_NEAR(data[i], expected[i], LN_FLOAT_THRESHOLD) << "at index " << i; \
        } \
    } while (0)

TEST_F(Test_Matrix4x4, translate) {
#if GLM_TEST
    auto mat1 = glm::translate(glm::mat4(1.f), glm::vec3(10, 20, 30));
#else
    Matrix4x4 mat1 = Matrix4x4::translate(Vector3(10, 20, 30));
#endif
    ASSERT_MAT(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        10.f, 20.f, 30.f, 1.f,
        mat1
    );
}
TEST_F(Test_Matrix4x4, lookAtRH) {
#if GLM_TEST
    auto mat1 = glm::lookAtRH(glm::vec3(10, 20, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
#else
    Matrix4x4 mat1 = Matrix4x4::lookAtRH(Vector3(10, 20, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));
#endif
    ASSERT_MAT(
        0.948683, -0.169031, 0.267261, 0,
        0, 0.845154, 0.534522, 0,
        -0.316228, -0.507093, 0.801784, 0,
        0, 0, -37.4166, 1,
        mat1
    );
}

TEST_F(Test_Matrix4x4, perspectiveRH) {
#if GLM_TEST
    auto mat1 = glm::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
#else
    Matrix4x4 mat1 = Matrix4x4::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
#endif
    ASSERT_MAT(
        1.358, 0, 0, 0,
        0, 2.41421, 0, 0,
        0, 0, -1.002, -1,
        0, 0, -0.2002, 0,
        mat1
    );
}

TEST_F(Test_Matrix4x4, ViewProjectionRH) {
#if GLM_TEST
    auto view = glm::lookAtRH(glm::vec3(10, 20, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto proj = glm::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
    auto vp = proj * view;
#else
    Matrix4x4 view = Matrix4x4::lookAtRH(Vector3(10, 20, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));
    Matrix4x4 proj = Matrix4x4::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
    Matrix4x4 vp = proj * view;
#endif
    ASSERT_MAT(
        1.288307, -0.408076, -0.267796, -0.267261,
        0.000000, 2.040383, -0.535593, -0.534522,
        -0.429436, -1.224230, -0.803389, -0.801784,
        -0.000001, 0.000002, 37.291283, 37.416573,
        vp
    );
}

#if 0



TEST(MathTest, Matrix4x4_Identity) {
    Matrix4x4 m = Matrix4x4::identity();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(m.m[i * 4 + j], (i == j) ? 1.0f : 0.0f);
}
TEST(MathTest, Matrix4x4_Inverse_Identity) {
    Matrix4x4 id = Matrix4x4::identity();
    Matrix4x4 inv = id.inversed();
    for (int i = 0; i < 16; ++i)
        EXPECT_NEAR(id.m[i], inv.m[i], 1e-5f);
}

//TEST(MathTest, Matrix4x4_Inverse_Translation) {
//    Matrix4x4 t = Matrix4x4::translate(Vector3{5.0f, -3.0f, 7.0f});
//    Matrix4x4 inv = t.inversed();
//    // Inverse of translate(a,b,c) should be translate(-a,-b,-c)
//    EXPECT_NEAR(inv.m[3], -5.0f, 1e-5f);
//    EXPECT_NEAR(inv.m[7],  3.0f, 1e-5f);
//    EXPECT_NEAR(inv.m[11], -7.0f, 1e-5f);
//}

TEST(MathTest, Matrix4x4_Transpose) {
    Matrix4x4 m = Matrix4x4::identity();
    m.m[1] = 2.0f; // row 0, col 1 (row-major)
    Matrix4x4 t = m.transposed();
    EXPECT_FLOAT_EQ(t.m[4], 2.0f); // should be at row 0, col 1
    EXPECT_FLOAT_EQ(t.m[1], 0.0f);
}

TEST_F(Test_Matrix4x4, RowMajorLayout) {
    glm::mat4 identity = glm::mat4(1.0f);
    auto m1 = glm::translate(glm::mat4(1.f), glm::vec3(10, 20, 30));

    //auto* a = &identity;

    // mat4 を float[16] に変換する
    float dataGL[16];
    std::memcpy(dataGL, &identity, sizeof(float) * 16);
    std::memcpy(dataGL, &m1, sizeof(float) * 16);

    
    Matrix4x4 t2 = Matrix4x4::translate(Vector3{10, 20, 30});

    float dataLN[16];
    std::memcpy(dataLN, &t2, sizeof(float) * 16);
    //glm::lookAtRH();



    // In row-major, element(row, col) = m[row * 4 + col].
    // Construct a translation matrix and verify layout.
    Matrix4x4 t = Matrix4x4::translate(Vector3{10, 20, 30});
    // Translation is in column 3 of the mathematical matrix.
    // Row-major: (0,3)=m[3], (1,3)=m[7], (2,3)=m[11]
    EXPECT_FLOAT_EQ(t.m[3], 10.0f);
    EXPECT_FLOAT_EQ(t.m[7], 20.0f);
    EXPECT_FLOAT_EQ(t.m[11], 30.0f);
    // Diagonal should be 1.
    EXPECT_FLOAT_EQ(t.m[0], 1.0f);
    EXPECT_FLOAT_EQ(t.m[5], 1.0f);
    EXPECT_FLOAT_EQ(t.m[10], 1.0f);
    EXPECT_FLOAT_EQ(t.m[15], 1.0f);
    // Bottom row should be (0,0,0,1).
    EXPECT_FLOAT_EQ(t.m[12], 0.0f);
    EXPECT_FLOAT_EQ(t.m[13], 0.0f);
    EXPECT_FLOAT_EQ(t.m[14], 0.0f);
}

TEST_F(Test_Matrix4x4, ScaleLayout) {
    Matrix4x4 s = Matrix4x4::scale(Vector3{2, 3, 4});
    EXPECT_FLOAT_EQ(s.m[0], 2.0f);
    EXPECT_FLOAT_EQ(s.m[5], 3.0f);
    EXPECT_FLOAT_EQ(s.m[10], 4.0f);
    EXPECT_FLOAT_EQ(s.m[15], 1.0f);
}

// ---- Rotation ----

TEST_F(Test_Matrix4x4, RotateX_90) {
    Matrix4x4 r = Matrix4x4::rotateX(kPi / 2);
    // RotX(90): y -> z, z -> -y
    // Row 1: (0, cos, -sin, 0) = (0, 0, -1, 0)
    // Row 2: (0, sin,  cos, 0) = (0, 1,  0, 0)
    EXPECT_NEAR(r.m[5], 0.0f, kEps);
    EXPECT_NEAR(r.m[6], -1.0f, kEps);
    EXPECT_NEAR(r.m[9], 1.0f, kEps);
    EXPECT_NEAR(r.m[10], 0.0f, kEps);
}

TEST_F(Test_Matrix4x4, RotateY_90) {
    Matrix4x4 r = Matrix4x4::rotateY(kPi / 2);
    // RotY(90): x -> -z, z -> x
    // Row 0: (cos, 0, sin, 0) = (0, 0, 1, 0)
    // Row 2: (-sin, 0, cos, 0) = (-1, 0, 0, 0)
    EXPECT_NEAR(r.m[0], 0.0f, kEps);
    EXPECT_NEAR(r.m[2], 1.0f, kEps);
    EXPECT_NEAR(r.m[8], -1.0f, kEps);
    EXPECT_NEAR(r.m[10], 0.0f, kEps);
}

TEST_F(Test_Matrix4x4, RotateZ_90) {
    Matrix4x4 r = Matrix4x4::rotateZ(kPi / 2);
    // RotZ(90): x -> y, y -> -x
    // Row 0: (cos, -sin, 0, 0) = (0, -1, 0, 0)
    // Row 1: (sin,  cos, 0, 0) = (1,  0, 0, 0)
    EXPECT_NEAR(r.m[0], 0.0f, kEps);
    EXPECT_NEAR(r.m[1], -1.0f, kEps);
    EXPECT_NEAR(r.m[4], 1.0f, kEps);
    EXPECT_NEAR(r.m[5], 0.0f, kEps);
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateX) {
    f32 angle = 0.7f;
    Matrix4x4 mr = Matrix4x4::rotateX(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitX(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateY) {
    f32 angle = 1.2f;
    Matrix4x4 mr = Matrix4x4::rotateY(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitY(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateZ) {
    f32 angle = -0.5f;
    Matrix4x4 mr = Matrix4x4::rotateZ(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitZ(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

// ---- Projection ----

TEST_F(Test_Matrix4x4, PerspectiveRH_Layout) {
    Matrix4x4 p = Matrix4x4::perspectiveRH(kPi / 4, 16.0f / 9.0f, 0.1f, 100.0f);
    // m[14] should be -1 (the w-divide element at (3,2))
    EXPECT_FLOAT_EQ(p.m[14], -1.0f);
    // m[15] should be 0
    EXPECT_FLOAT_EQ(p.m[15], 0.0f);
    // m[11] = -(far*near)/(far-near) at (2,3)
    EXPECT_NEAR(p.m[11], -(100.0f * 0.1f) / (100.0f - 0.1f), kEps);
}

TEST_F(Test_Matrix4x4, Ortho_Layout) {
    Matrix4x4 o = Matrix4x4::ortho(-1, 1, -1, 1, 0.1f, 100.0f);
    // Translation at (0,3), (1,3), (2,3) = m[3], m[7], m[11]
    // For symmetric bounds (-1,1,-1,1): tx=0, ty=0
    EXPECT_NEAR(o.m[3], 0.0f, kEps);
    EXPECT_NEAR(o.m[7], 0.0f, kEps);
    EXPECT_FLOAT_EQ(o.m[15], 1.0f);
}

// ---- Multiply ----

TEST_F(Test_Matrix4x4, MultiplyIdentity) {
    Matrix4x4 m = Matrix4x4::rotateY(0.5f);
    Matrix4x4 id = Matrix4x4::identity();
    EXPECT_TRUE(matNear(m * id, m));
    EXPECT_TRUE(matNear(id * m, m));
}

// ---- Transform Point ----

TEST_F(Test_Matrix4x4, TransformPoint_Translation) {
    Matrix4x4 t = Matrix4x4::translate(Vector3{3, 4, 5});
    Vector4 v = mulMV(t, Vector4{0, 0, 0, 1});
    EXPECT_NEAR(v.x, 3.0f, kEps);
    EXPECT_NEAR(v.y, 4.0f, kEps);
    EXPECT_NEAR(v.z, 5.0f, kEps);
    EXPECT_NEAR(v.w, 1.0f, kEps);
}

TEST_F(Test_Matrix4x4, TransformPoint_Scale) {
    Matrix4x4 s = Matrix4x4::scale(Vector3{2, 3, 4});
    Vector4 v = mulMV(s, Vector4{1, 1, 1, 1});
    EXPECT_NEAR(v.x, 2.0f, kEps);
    EXPECT_NEAR(v.y, 3.0f, kEps);
    EXPECT_NEAR(v.z, 4.0f, kEps);
}

TEST_F(Test_Matrix4x4, TransformPoint_RotateZ90) {
    Matrix4x4 r = Matrix4x4::rotateZ(kPi / 2);
    // Rotating (1,0,0) by 90 about Z -> (0,1,0)
    Vector4 v = mulMV(r, Vector4{1, 0, 0, 1});
    EXPECT_NEAR(v.x, 0.0f, kEps);
    EXPECT_NEAR(v.y, 1.0f, kEps);
    EXPECT_NEAR(v.z, 0.0f, kEps);
}

TEST_F(Test_Matrix4x4, TRS_Composition) {
    // T * R * S applied to a point
    Matrix4x4 T = Matrix4x4::translate(Vector3{10, 0, 0});
    Matrix4x4 R = Matrix4x4::rotateZ(kPi / 2);
    Matrix4x4 S = Matrix4x4::scale(Vector3{2, 2, 2});
    Matrix4x4 M = T * R * S;

    // Transform (1,0,0,1): scale -> (2,0,0), rotate Z 90 -> (0,2,0), translate -> (10,2,0)
    Vector4 v = mulMV(M, Vector4{1, 0, 0, 1});
    EXPECT_NEAR(v.x, 10.0f, kEps);
    EXPECT_NEAR(v.y, 2.0f, kEps);
    EXPECT_NEAR(v.z, 0.0f, kEps);
}

// ---- Inverse ----

TEST_F(Test_Matrix4x4, Inverse_RoundTrip_Rotation) {
    Matrix4x4 r = Matrix4x4::rotateY(1.0f);
    Matrix4x4 product = r * r.inversed();
    EXPECT_TRUE(matNear(product, Matrix4x4::identity()));
}

TEST_F(Test_Matrix4x4, Inverse_RoundTrip_TRS) {
    Matrix4x4 M = Matrix4x4::translate(Vector3{1, 2, 3})
                 * Matrix4x4::rotateX(0.5f)
                 * Matrix4x4::scale(Vector3{2, 3, 4});
    Matrix4x4 product = M * M.inversed();
    EXPECT_TRUE(matNear(product, Matrix4x4::identity(), 1e-4f));
}

// ---- Transpose ----

TEST_F(Test_Matrix4x4, Transpose_RoundTrip) {
    Matrix4x4 m = Matrix4x4::rotateY(0.7f);
    m.m[3] = 5.0f;  // add some asymmetry
    EXPECT_TRUE(matNear(m.transposed().transposed(), m));
}

TEST_F(Test_Matrix4x4, Transpose_SwapsRowCol) {
    // Set element (0,1) = 7 in row-major -> m[1].
    // After transpose, element (1,0) = 7 -> m[4].
    Matrix4x4 m = Matrix4x4::identity();
    m.m[1] = 7.0f;
    Matrix4x4 t = m.transposed();
    EXPECT_FLOAT_EQ(t.m[4], 7.0f);
    EXPECT_FLOAT_EQ(t.m[1], 0.0f);
}

// ---- LookAt ----

TEST_F(Test_Matrix4x4, LookAtRH_TransformsEyeToOrigin) {
    Vector3 eye{0, 0, 5};
    Vector3 target{0, 0, 0};
    Vector3 up{0, 1, 0};
    Matrix4x4 V = Matrix4x4::lookAtRH(eye, target, up);

    // The view matrix should map eye to origin.
    Vector4 veye = mulMV(V, Vector4{eye, 1});
    EXPECT_NEAR(veye.x, 0.0f, kEps);
    EXPECT_NEAR(veye.y, 0.0f, kEps);
    EXPECT_NEAR(veye.z, 0.0f, kEps);
    EXPECT_NEAR(veye.w, 1.0f, kEps);
}

TEST_F(Test_Matrix4x4, LookAtRH_ForwardMapsToNegZ) {
    Vector3 eye{0, 0, 5};
    Vector3 target{0, 0, 0};
    Vector3 up{0, 1, 0};
    Matrix4x4 V = Matrix4x4::lookAtRH(eye, target, up);

    // A point on the target (origin) should be at negative Z in view space.
    Vector4 vtarget = mulMV(V, Vector4{0, 0, 0, 1});
    EXPECT_NEAR(vtarget.x, 0.0f, kEps);
    EXPECT_NEAR(vtarget.y, 0.0f, kEps);
    EXPECT_TRUE(vtarget.z < 0.0f);  // target is in front -> negative Z in RH
}
#endif
