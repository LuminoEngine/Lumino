#include "Test_Math_Common.hpp"

class Test_ViewFrustum : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, Basic)
{
	// コンストラクタ
	{
		ViewFrustum v1(Matrix::Identity);

		Matrix m2 = Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::makePerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000);
		ViewFrustum v2(m2);
		ASSERT_PLANE_NEAR(-0.000000, -0.000000, -1.000000, 1.000000, v2.plane(FrustumPlane::Near));
		ASSERT_PLANE_NEAR(0.000000, 0.000000, 1.000000, -1000.000610, v2.plane(FrustumPlane::Far));
		ASSERT_PLANE_NEAR(-0.875370, -0.000000, -0.483454, -0.000000, v2.plane(FrustumPlane::Left));
		ASSERT_PLANE_NEAR(0.875370, 0.000000, -0.483454, 0.000000, v2.plane(FrustumPlane::Right));
		ASSERT_PLANE_NEAR(0.000000, 0.923880, -0.382683, 0.000000, v2.plane(FrustumPlane::Top));
		ASSERT_PLANE_NEAR(-0.000000, -0.923880, -0.382683, -0.000000, v2.plane(FrustumPlane::Bottom));
		
		// Ortho
		ViewFrustum v3(Matrix::makeLookAtRH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::makeOrthoRH(640, 480, 0, 1000));
		ASSERT_PLANE_NEAR(-0.000000, -0.000000, -1.000000, -0.000000, v3.plane(FrustumPlane::Near));
		ASSERT_PLANE_NEAR(0.000000, 0.000000, 1.000000, -999.999939, v3.plane(FrustumPlane::Far));
		ASSERT_PLANE_NEAR(1.000000, -0.000000, -0.000000, -320.000000, v3.plane(FrustumPlane::Left));
		ASSERT_PLANE_NEAR(-1.000000, 0.000000, 0.000000, -320.000000, v3.plane(FrustumPlane::Right));
		ASSERT_PLANE_NEAR(0.000000, 1.000000, 0.000000, -239.999985, v3.plane(FrustumPlane::Top));
		ASSERT_PLANE_NEAR(-0.000000, -1.000000, -0.000000, -239.999985, v3.plane(FrustumPlane::Bottom));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, intersects)
{
	Matrix proj1 = Matrix::makePerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000);

	// Z+ 向き
	{
		Matrix view = Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
		ViewFrustum vf1(view * proj1);

		Vector3 pos1(0, 0, 10);
		Vector3 pos2(10, 0, 0);
		ASSERT_TRUE(vf1.intersects(pos1));
		ASSERT_FALSE(vf1.intersects(pos2));

		Vector3 sppos1(0, 0, 1002);
		float spr1 = 1;
		ASSERT_FALSE(vf1.intersects(sppos1, spr1));

		Vector3 sppos2(0, 0, 1000);
		float spr2 = 5;
		ASSERT_TRUE(vf1.intersects(sppos2, spr2));	
	}

	// Ortho (2D 想定)
	{
		Matrix vp;
		vp = Matrix::makeLookAtLH(Vector3(320, 240, 0), Vector3(320, 240, 1), Vector3(0, 1, 0));
		vp *= Matrix::makeOrthoLH(640, 480, 0, 1000);
		ViewFrustum vf(vp);

		ASSERT_TRUE(vf.intersects(Vector3(0, 0, 0)));
		ASSERT_TRUE(vf.intersects(Vector3(1, 0, 0)));
		ASSERT_FALSE(vf.intersects(Vector3(-1, 0, 0)));
		ASSERT_TRUE(vf.intersects(Vector3(639, 0, 0)));	// このあたりはどうしても誤差が出る。
		ASSERT_TRUE(vf.intersects(Vector3(640, 0, 0)));	// とりあえず、0～639までOKならそれで良しとする。
		ASSERT_FALSE(vf.intersects(Vector3(641, 0, 0)));// 以下、Y,Z軸についても同様。

		ASSERT_TRUE(vf.intersects(Vector3(0, 0, 0)));
		ASSERT_FALSE(vf.intersects(Vector3(0, -1, 0)));
		ASSERT_TRUE(vf.intersects(Vector3(0, 479, 0)));
		ASSERT_FALSE(vf.intersects(Vector3(0, 481, 0)));

		ASSERT_TRUE(vf.intersects(Vector3(0, 0, 0)));
		ASSERT_FALSE(vf.intersects(Vector3(0, 0, -1)));
		ASSERT_TRUE(vf.intersects(Vector3(0, 0, 999)));
		ASSERT_FALSE(vf.intersects(Vector3(0, 0, 1001)));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, intersects_Box)
{
	Matrix view = Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	Matrix proj1 = Matrix::makePerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000);
	ViewFrustum vf1(view * proj1);

	IntersectResult r;
	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ 0, 0, 1001 }, 1)));
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 1000 }, 1)));
	ASSERT_EQ(IntersectResult::Inside, vf1.intersects(Box({ 0, 0, 999 }, 1)));

	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ 2000, 0, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ -2000, 0, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ 0, 2000, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ 0, -2000, 500 }, 1)));

	ASSERT_EQ(IntersectResult::Inside, vf1.intersects(Box({ 20, 0, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Inside, vf1.intersects(Box({ -20, 0, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Inside, vf1.intersects(Box({ 0, 20, 500 }, 1)));
	ASSERT_EQ(IntersectResult::Inside, vf1.intersects(Box({ 0, -20, 500 }, 1)));

	// 非常に縦長
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 500 }, 1, 10000, 1)));
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 500 }, 10000, 1, 1)));
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 500 }, 1, 1, 10000)));

	// 視点の後ろ
	ASSERT_EQ(IntersectResult::Outside, vf1.intersects(Box({ 0, 0, -1 }, 1)));

	// 視点と同じ位置に大きなオブジェクトがある場合
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 0 }, 100)));

	// Flustum 全体を覆うオブジェクトの場合
	ASSERT_EQ(IntersectResult::Intersect, vf1.intersects(Box({ 0, 0, 0 }, 10000)));
}

//------------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, getCornerPoints)
{
	// <Test> 左手系の PerspectiveFov
	{
		ViewFrustum vf(Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::makePerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000));
		Vector3 points[8];
		vf.getCornerPoints(points);
		ASSERT_VEC3_NEAR(-0.552284837, 0.414213598, 1.00000000, points[0]);
		ASSERT_VEC3_NEAR(-0.552284837, -0.414213598, 1.00000000, points[1]);
		ASSERT_VEC3_NEAR(0.552284837, -0.414213598, 1.00000000, points[2]);
		ASSERT_VEC3_NEAR(0.552284837, 0.414213598, 1.00000000, points[3]);
		ASSERT_VEC3_NEAR(-552.324890, 414.243622, 1000.07251, points[4]);
		ASSERT_VEC3_NEAR(-552.324890, -414.243622, 1000.07251, points[5]);
		ASSERT_VEC3_NEAR(552.324890, -414.243622, 1000.07251, points[6]);
		ASSERT_VEC3_NEAR(552.324890, 414.243622, 1000.07251, points[7]);
	}
	// <Test> 左手系の Ortho
	{
		ViewFrustum vf(Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::makeOrthoLH(640, 480, 0, 1000));
		Vector3 points[8];
		vf.getCornerPoints(points);
		ASSERT_VEC3_NEAR(-320.000000, 239.999985, 0.000000, points[0]);
		ASSERT_VEC3_NEAR(-320.000000, -239.999985, 0.000000, points[1]);
		ASSERT_VEC3_NEAR(320.000000, -239.999985, 0.000000, points[2]);
		ASSERT_VEC3_NEAR(320.000000, 239.999985, 0.000000, points[3]);
		ASSERT_VEC3_NEAR(-320.000000, 239.999985, 1000.000000, points[4]);
		ASSERT_VEC3_NEAR(-320.000000, -239.999985, 1000.000000, points[5]);
		ASSERT_VEC3_NEAR(320.000000, -239.999985, 1000.000000, points[6]);
		ASSERT_VEC3_NEAR(320.000000, 239.999985, 1000.000000, points[7]);
	}
}


//------------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, BoxTransform)	// TODO: move
{
	Box b1(1);
	Box r;
	r = Box::transform(b1, Matrix::makeRotationY(Math::PI / 4));
	ASSERT_FLOAT_NEAR(1.41421354, r.width);
	ASSERT_FLOAT_NEAR(1.0, r.height);
	ASSERT_FLOAT_NEAR(1.41421354, r.depth);

	r = Box::transform(b1, Matrix::makeRotationY(Math::PI / 4) * Matrix::makeTranslation(5, 0, -5));
	ASSERT_FLOAT_NEAR(5, r.center.x);
	ASSERT_FLOAT_NEAR(0, r.center.y);
	ASSERT_FLOAT_NEAR(-5, r.center.z);
	ASSERT_FLOAT_NEAR(1.41421318, r.width);
	ASSERT_FLOAT_NEAR(1.0, r.height);
	ASSERT_FLOAT_NEAR(1.41421318, r.depth);
}
