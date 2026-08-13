// clang-format off
#include <LuminoBase/math/Math.hpp>
#include <cmath>
#include <iostream>
#define GLM_TEST 0

using namespace ln;

class Test_Matrix4x4 : public ::testing::Test {};

static constexpr float kPi = 3.14159265358979323846f;
static constexpr float kEps = 1e-5f;

// float の演算方法がコンパイラによって異なり、結果がわずかにずれることがある。
// その差を吸収するため、比較する桁数を指定する。
// (具体的にどう異なるのかは未調査。VS2010 と VS2013 では Vector3::project() の誤差が最も大きかった)
#define LN_FLOAT_THRESHOLD 0.0001f

// Helper: 列優先行列とベクトルの積 (out = M * v, 列ベクトル規約)。
// 列優先では要素 (row, col) は m[col * 4 + row] に格納される。
static Vector4 mulMV(const Matrix4x4& M, const Vector4& v) {
    Vector4 r;
    r.x = M.m[0] * v.x + M.m[4] * v.y + M.m[8]  * v.z + M.m[12] * v.w;
    r.y = M.m[1] * v.x + M.m[5] * v.y + M.m[9]  * v.z + M.m[13] * v.w;
    r.z = M.m[2] * v.x + M.m[6] * v.y + M.m[10] * v.z + M.m[14] * v.w;
    r.w = M.m[3] * v.x + M.m[7] * v.y + M.m[11] * v.z + M.m[15] * v.w;
    return r;
}

static bool matNear(const Matrix4x4& a, const Matrix4x4& b, float eps = kEps) {
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
    auto mat1 = glm::perspectiveRH_ZO(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
#else
    Matrix4x4 mat1 = Matrix4x4::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
#endif
    // 深度 0..1 (ZO)。m[10]=far/(near-far), m[14]=-(far*near)/(far-near)。
    ASSERT_MAT(
        1.358, 0, 0, 0,
        0, 2.41421, 0, 0,
        0, 0, -1.001, -1,
        0, 0, -0.1001, 0,
        mat1
    );
}

TEST_F(Test_Matrix4x4, ViewProjectionRH) {
#if GLM_TEST
    auto view = glm::lookAtRH(glm::vec3(10, 20, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto proj = glm::perspectiveRH_ZO(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
    auto vp = proj * view;
#else
    Matrix4x4 view = Matrix4x4::lookAtRH(Vector3(10, 20, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));
    Matrix4x4 proj = Matrix4x4::perspectiveRH(glm::radians(45.f), 16.f/9.f, 0.1f, 100.f);
    Matrix4x4 vp = proj * view;
#endif
    // 深度 0..1 (ZO) の射影を使用。
    ASSERT_MAT(
        1.288307, -0.408076, -0.267529, -0.267261,
        0.000000, 2.040383, -0.535058, -0.534522,
        -0.429436, -1.224230, -0.802586, -0.801784,
        -0.000001, 0.000002, 37.353924, 37.416573,
        vp
    );
}

// ---- transformCoord (列ベクトル規約 out = M * (v, 1)) ----

TEST_F(Test_Matrix4x4, TransformCoord_Translation) {
    Matrix4x4 t = Matrix4x4::translate(Vector3(3, 4, 5));
    Vector3 v = t.transformCoord(Vector3(0, 0, 0));
    ASSERT_NEAR(v.x, 3.f, kEps);
    ASSERT_NEAR(v.y, 4.f, kEps);
    ASSERT_NEAR(v.z, 5.f, kEps);
}

TEST_F(Test_Matrix4x4, TransformCoord_TRS) {
    // world = T(10,0,0) * Rz(90deg) * S(2) を点 (1,0,0) に適用:
    //   S -> (2,0,0), Rz90 -> (0,2,0), T -> (10,2,0)
    Matrix4x4 M = Matrix4x4::translate(Vector3(10, 0, 0))
                * Matrix4x4::rotateZ(kPi / 2)
                * Matrix4x4::scale(Vector3(2, 2, 2));
    Vector3 v = M.transformCoord(Vector3(1, 0, 0));
    ASSERT_NEAR(v.x, 10.f, kEps);
    ASSERT_NEAR(v.y, 2.f, kEps);
    ASSERT_NEAR(v.z, 0.f, kEps);
}

// ---- レイアウト (列優先: 要素 (row, col) は m[col * 4 + row]) ----

TEST_F(Test_Matrix4x4, Identity) {
    Matrix4x4 m = Matrix4x4::identity();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(m.m[i * 4 + j], (i == j) ? 1.0f : 0.0f);
}

TEST_F(Test_Matrix4x4, ColumnMajorLayout) {
    // 列優先では平行移動成分は m[12], m[13], m[14] に入る。
    // (行優先の m[3], m[7], m[11] ではない)
    Matrix4x4 t = Matrix4x4::translate(Vector3{10, 20, 30});
    EXPECT_FLOAT_EQ(t.m[12], 10.0f);
    EXPECT_FLOAT_EQ(t.m[13], 20.0f);
    EXPECT_FLOAT_EQ(t.m[14], 30.0f);
    // 行優先で平行移動が入るはずの位置は 0 のまま。
    EXPECT_FLOAT_EQ(t.m[3], 0.0f);
    EXPECT_FLOAT_EQ(t.m[7], 0.0f);
    EXPECT_FLOAT_EQ(t.m[11], 0.0f);
    // 対角は 1。
    EXPECT_FLOAT_EQ(t.m[0], 1.0f);
    EXPECT_FLOAT_EQ(t.m[5], 1.0f);
    EXPECT_FLOAT_EQ(t.m[10], 1.0f);
    EXPECT_FLOAT_EQ(t.m[15], 1.0f);
}

TEST_F(Test_Matrix4x4, ScaleLayout) {
    Matrix4x4 s = Matrix4x4::scale(Vector3{2, 3, 4});
    EXPECT_FLOAT_EQ(s.m[0], 2.0f);
    EXPECT_FLOAT_EQ(s.m[5], 3.0f);
    EXPECT_FLOAT_EQ(s.m[10], 4.0f);
    EXPECT_FLOAT_EQ(s.m[15], 1.0f);
}

// ---- 回転 (列優先での要素配置を検証) ----

TEST_F(Test_Matrix4x4, RotateX_90) {
    Matrix4x4 r = Matrix4x4::rotateX(kPi / 2);
    // RotX(90): +Y -> +Z, +Z -> -Y。
    // 数学的な (1,1)=cos, (2,1)=sin, (1,2)=-sin, (2,2)=cos。
    // 列優先では (2,1) -> m[6], (1,2) -> m[9]。
    EXPECT_NEAR(r.m[5], 0.0f, kEps);   // (1,1)=cos
    EXPECT_NEAR(r.m[6], 1.0f, kEps);   // (2,1)=sin
    EXPECT_NEAR(r.m[9], -1.0f, kEps);  // (1,2)=-sin
    EXPECT_NEAR(r.m[10], 0.0f, kEps);  // (2,2)=cos
}

TEST_F(Test_Matrix4x4, RotateY_90) {
    Matrix4x4 r = Matrix4x4::rotateY(kPi / 2);
    // RotY(90): +X -> -Z, +Z -> +X。
    // 数学的な (0,0)=cos, (0,2)=sin, (2,0)=-sin, (2,2)=cos。
    // 列優先では (2,0) -> m[2], (0,2) -> m[8]。
    EXPECT_NEAR(r.m[0], 0.0f, kEps);   // (0,0)=cos
    EXPECT_NEAR(r.m[2], -1.0f, kEps);  // (2,0)=-sin
    EXPECT_NEAR(r.m[8], 1.0f, kEps);   // (0,2)=sin
    EXPECT_NEAR(r.m[10], 0.0f, kEps);  // (2,2)=cos
}

TEST_F(Test_Matrix4x4, RotateZ_90) {
    Matrix4x4 r = Matrix4x4::rotateZ(kPi / 2);
    // RotZ(90): +X -> +Y, +Y -> -X。
    // 数学的な (0,0)=cos, (1,0)=sin, (0,1)=-sin, (1,1)=cos。
    // 列優先では (1,0) -> m[1], (0,1) -> m[4]。
    EXPECT_NEAR(r.m[0], 0.0f, kEps);   // (0,0)=cos
    EXPECT_NEAR(r.m[1], 1.0f, kEps);   // (1,0)=sin
    EXPECT_NEAR(r.m[4], -1.0f, kEps);  // (0,1)=-sin
    EXPECT_NEAR(r.m[5], 0.0f, kEps);   // (1,1)=cos
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateX) {
    float angle = 0.7f;
    Matrix4x4 mr = Matrix4x4::rotateX(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitX(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateY) {
    float angle = 1.2f;
    Matrix4x4 mr = Matrix4x4::rotateY(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitY(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

TEST_F(Test_Matrix4x4, FromQuaternion_MatchesRotateZ) {
    float angle = -0.5f;
    Matrix4x4 mr = Matrix4x4::rotateZ(angle);
    Matrix4x4 mq = Matrix4x4::fromQuaternion(Quaternion::fromAxisAngle(Vector3::unitZ(), angle));
    EXPECT_TRUE(matNear(mr, mq));
}

// ---- 射影 ----

TEST_F(Test_Matrix4x4, PerspectiveRH_Layout) {
    const float nearZ = 0.1f, farZ = 100.0f;
    Matrix4x4 p = Matrix4x4::perspectiveRH(kPi / 4, 16.0f / 9.0f, nearZ, farZ);
    // 列優先では w 除算成分 (3,2)=-1 は m[11] に入る (行優先の m[14] ではない)。
    EXPECT_FLOAT_EQ(p.m[11], -1.0f);
    // (3,3)=0。
    EXPECT_FLOAT_EQ(p.m[15], 0.0f);
    // 深度 0..1 (ZO): (2,2)=far/(near-far) は m[10]、z 平行移動 (2,3) は m[14]。
    EXPECT_NEAR(p.m[10], farZ / (nearZ - farZ), kEps);
    EXPECT_NEAR(p.m[14], -(farZ * nearZ) / (farZ - nearZ), kEps);
}

TEST_F(Test_Matrix4x4, Ortho_Layout) {
    // 非対称な範囲を使い、平行移動成分が列優先の位置に入ることを確認する。
    Matrix4x4 o = Matrix4x4::ortho(0, 4, 0, 3, 0.1f, 100.0f);
    // tx = -(right+left)/(right-left) = -4/4 = -1 -> m[12]
    // ty = -(top+bottom)/(top-bottom) = -3/3 = -1 -> m[13]
    EXPECT_NEAR(o.m[12], -1.0f, kEps);
    EXPECT_NEAR(o.m[13], -1.0f, kEps);
    // 行優先で平行移動が入るはずの m[3], m[7] は 0 のまま。
    EXPECT_NEAR(o.m[3], 0.0f, kEps);
    EXPECT_NEAR(o.m[7], 0.0f, kEps);
    EXPECT_FLOAT_EQ(o.m[15], 1.0f);
}

// ---- 乗算 ----

TEST_F(Test_Matrix4x4, MultiplyIdentity) {
    Matrix4x4 m = Matrix4x4::rotateY(0.5f);
    Matrix4x4 id = Matrix4x4::identity();
    EXPECT_TRUE(matNear(m * id, m));
    EXPECT_TRUE(matNear(id * m, m));
}

// ---- 点の変換 (列ベクトル規約 out = M * v) ----

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
    // (1,0,0) を Z 軸まわりに 90 度回転すると (0,1,0)。
    Vector4 v = mulMV(r, Vector4{1, 0, 0, 1});
    EXPECT_NEAR(v.x, 0.0f, kEps);
    EXPECT_NEAR(v.y, 1.0f, kEps);
    EXPECT_NEAR(v.z, 0.0f, kEps);
}

TEST_F(Test_Matrix4x4, TRS_Composition) {
    // T * R * S を点に適用する。
    Matrix4x4 T = Matrix4x4::translate(Vector3{10, 0, 0});
    Matrix4x4 R = Matrix4x4::rotateZ(kPi / 2);
    Matrix4x4 S = Matrix4x4::scale(Vector3{2, 2, 2});
    Matrix4x4 M = T * R * S;

    // (1,0,0,1): scale -> (2,0,0), rotateZ 90 -> (0,2,0), translate -> (10,2,0)
    Vector4 v = mulMV(M, Vector4{1, 0, 0, 1});
    EXPECT_NEAR(v.x, 10.0f, kEps);
    EXPECT_NEAR(v.y, 2.0f, kEps);
    EXPECT_NEAR(v.z, 0.0f, kEps);
}

// ---- 逆行列 ----

TEST_F(Test_Matrix4x4, Inverse_Identity) {
    Matrix4x4 id = Matrix4x4::identity();
    Matrix4x4 inv = id.inversed();
    for (int i = 0; i < 16; ++i)
        EXPECT_NEAR(id.m[i], inv.m[i], 1e-5f);
}

TEST_F(Test_Matrix4x4, Inverse_Translation) {
    Matrix4x4 t = Matrix4x4::translate(Vector3{5.0f, -3.0f, 7.0f});
    Matrix4x4 inv = t.inversed();
    // translate(a,b,c) の逆行列は translate(-a,-b,-c)。
    // 列優先なので平行移動成分は m[12], m[13], m[14]。
    EXPECT_NEAR(inv.m[12], -5.0f, 1e-5f);
    EXPECT_NEAR(inv.m[13], 3.0f, 1e-5f);
    EXPECT_NEAR(inv.m[14], -7.0f, 1e-5f);
}

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

// ---- 転置 ----

TEST_F(Test_Matrix4x4, Transpose_RoundTrip) {
    Matrix4x4 m = Matrix4x4::rotateY(0.7f);
    m.m[3] = 5.0f;  // 非対称にする
    EXPECT_TRUE(matNear(m.transposed().transposed(), m));
}

TEST_F(Test_Matrix4x4, Transpose_SwapsRowCol) {
    // m[1] に 7 を入れて転置すると m[4] へ移動する。
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

    // ビュー行列は視点を原点へ写す。
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

    // 注視点 (原点) はビュー空間で負の Z にある (右手系では手前が -Z)。
    Vector4 vtarget = mulMV(V, Vector4{0, 0, 0, 1});
    EXPECT_NEAR(vtarget.x, 0.0f, kEps);
    EXPECT_NEAR(vtarget.y, 0.0f, kEps);
    EXPECT_TRUE(vtarget.z < 0.0f);
}
