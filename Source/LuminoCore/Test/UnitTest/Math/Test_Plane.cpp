#include "Common.h"

class Test_Plane : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Plane, Basic)
{
	// コンストラクタ
	{
		Plane pl1;
		ASSERT_PLANE_NEAR(0, 0, 0, 0, pl1);
		Plane pl2(1, 2, 3, 4);
		ASSERT_PLANE_NEAR(1, 2, 3, 4, pl2);
		Plane pl3(Vector3(1, 2, 3), Vector3::Normalize(4, 5, 6));
		ASSERT_PLANE_NEAR(0.455842, 0.569803, 0.683764, -3.646739, pl3);
		Plane pl4(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(-7, -8, -9));
		ASSERT_PLANE_NEAR(-0.408248, 0.816497, -0.408248, -0.000000, pl4);

		// pl5 の結果は (0, 1, 0)
		// つまり、左手座標系か右手座標系かで、正面が時計回りか反時計回りかが変わる。
		Plane pl5(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(1, 0, 0));
		ASSERT_PLANE_NEAR(0, 1, 0, 0, pl5);
	}
	// this->Normalize
	{
		Plane pl1(10, 20, 30, 40);
		pl1.Normalize();
		ASSERT_PLANE_NEAR(0.267261, 0.534522, 0.801784, 1.069045, pl1);
	}
	// this->CheckInside
	{
		Plane pl1(Vector3(0, 0, 0), Vector3(0, 1, 0));
		ASSERT_FALSE(pl1.CheckInside(Vector3(0, 10, 0)));	// 上の方
		ASSERT_TRUE(pl1.CheckInside(Vector3(0, -10, 0)));	// 下の方

		ASSERT_FALSE(pl1.CheckInside(Vector3(0, 10, 0), 5));	// 上の方
		ASSERT_TRUE(pl1.CheckInside(Vector3(0, -10, 0), 5));	// 下の方
		ASSERT_TRUE(pl1.CheckInside(Vector3(0, -10, 0), 15));	// 中心は内側。上が少しはみ出てる
		ASSERT_TRUE(pl1.CheckInside(Vector3(0, 10, 0), 15));	// 中心は外側。下が少しめり込んでる
	}
	// this->Intersects
	{
		Plane pl1(
			Vector3(1, 2, 3),
			Vector3(4, -5, 6),
			Vector3(-7, -8, -9));
		Vector3 v4(0, 10, 20);
		Vector3 v5(-10, -10, -20);

		Vector3 v6;
		bool r = pl1.Intersects(v4, v5, &v6);
		ASSERT_TRUE(r);
		ASSERT_VEC3_NEAR(-7.184465, -4.368931, -8.737862, v6);
		
		// 平行な線分
		Plane pl2(Vector3(0, 0, 0), Vector3(0, 1, 0));
		Vector3 start(1, 1, 1);
		Vector3 end(2, 1, 2);
		r = pl2.Intersects(start, end, &v6);
		ASSERT_FALSE(r);
	}
	// this->Intersects
	{
		Plane pl1(Vector3(2, 3, 4), Vector3::UnitX);

		Vector3 v;
		bool r;
		ASSERT_EQ(true, pl1.Intersects(Ray(Vector3(0, 0, 0), Vector3(1, 0, 0)), &v));	// 直行
		ASSERT_VEC3_NEAR(2, 0, 0, v);
		ASSERT_EQ(false, pl1.Intersects(Ray(Vector3(0, 0, 0), Vector3(0, 1, 0)), &v));	// 平行
		ASSERT_EQ(false, pl1.Intersects(Ray(Vector3(0, 0, 0), Vector3(0, 0, 1)), &v));	// 平行

		ASSERT_EQ(true, pl1.Intersects(Ray(Vector3(0, 0, 1), Vector3::Normalize(1, 0, -1)), &v));
		ASSERT_VEC3_NEAR(2, 0, -1, v);
	}
	// this->Transform
	{
		Vector3 v1(10, 0.5, -30);
		Vector3 v2 = Vector3::Normalize(4, 5, 6);
		Plane pl1(v1, v2);

		Matrix mat = Matrix::MakeRotationZ(Math::PI / 2);
		pl1.Transform(mat);
		ASSERT_PLANE_NEAR(-0.569803, 0.455842, 0.683764, 15.669580, pl1);
	}

	// Plane::Normalize
	{
		Plane pl1 = Plane::Normalize(Plane(10, 20, 30, 40));
		ASSERT_PLANE_NEAR(0.267261, 0.534522, 0.801784, 1.069045, pl1);
	}
	// Plane::Dot / DotCoord / DotNormal
	{
		Plane pl1(Vector3(1, 2, 3), Vector3(4, -5, 6), Vector3(-7, -8, -9));
		float v1 = Plane::Dot(pl1, Vector4(0, 10, 20, 0.5f));
		float v2 = Plane::DotCoord(pl1, Vector3(0, 10, 20));
		float v3 = Plane::DotNormal(pl1, Vector3(0, 10, 20));
		ASSERT_NEAR(-10.746395, v1, 0.000001);
		ASSERT_NEAR(-10.327704, v2, 0.000001);
		ASSERT_NEAR(-11.165086, v3, 0.000001);
	}
	// Plane::Transform
	{
		Vector3 v1(10, 0.5, -30);
		Vector3 v2 = Vector3::Normalize(4, 5, 6);
		Plane pl1(v1, v2);

		Matrix mat = Matrix::MakeRotationZ(Math::PI / 2);
		Plane pl2 = Plane::Transform(pl1, mat);
		ASSERT_PLANE_NEAR(-0.569803, 0.455842, 0.683764, 15.669580, pl2);
	}

#ifdef D3DX9_TEST
	// D3DXPlaneFromPointNormal
	{
		D3DXPLANE pl;
		D3DXVECTOR3 v1(1, 2, 3);
		D3DXVECTOR3 v2(4, 5, 6);
		D3DXVec3Normalize(&v2, &v2);
		D3DXPlaneFromPointNormal(&pl, &v1, &v2);
		//dumpD3DXPLANE("D3DXPlaneFromPointNormal", pl);
	}
	// D3DXPlaneFromPoints
	{
		D3DXPLANE pl;
		D3DXVECTOR3 v1(1, 2, 3);
		D3DXVECTOR3 v2(4, 5, 6);
		D3DXVECTOR3 v3(-7, -8, -9);
		D3DXPlaneFromPoints(&pl, &v1, &v2, &v3);
		//dumpD3DXPLANE("D3DXPlaneFromPoints", pl);
	}
	// D3DXPlaneNormalize
	{
		D3DXPLANE pl1(10, 20, 30, 40);
		D3DXPLANE pl2;
		D3DXPlaneNormalize(&pl2, &pl1);
		//dumpD3DXPLANE("D3DXPlaneNormalize", pl2);
	}
	// D3DXPlaneDot
	{
		D3DXPLANE pl;
		D3DXVECTOR3 v1(1, 2, 3);
		D3DXVECTOR3 v2(4, -5, 6);
		D3DXVECTOR3 v3(-7, -8, -9);
		D3DXPlaneFromPoints(&pl, &v1, &v2, &v3);

		D3DXVECTOR4 v4(0, 10, 20, 0.5f);
		//dumpFLOAT("D3DXPlaneDot", D3DXPlaneDot(&pl, &v4));
		D3DXVECTOR3 v5(0, 10, 20);
		//dumpFLOAT("D3DXPlaneDotCoord", D3DXPlaneDotCoord(&pl, &v5));
		//dumpFLOAT("D3DXPlaneDotNormal", D3DXPlaneDotNormal(&pl, &v5));
	}
	// D3DXPlaneIntersectLine
	{
		D3DXPLANE pl;
		D3DXVECTOR3 v1(1, 2, 3);
		D3DXVECTOR3 v2(4, -5, 6);
		D3DXVECTOR3 v3(-7, -8, -9);
		D3DXPlaneFromPoints(&pl, &v1, &v2, &v3);

		D3DXVECTOR3 v4(0, 10, 20);
		D3DXVECTOR3 v5(-10, -10, -20);

		D3DXVECTOR3 v6;
		if (D3DXPlaneIntersectLine(&v6, &pl, &v4, &v5) != NULL)
		{
			//dumpD3DXVECTOR3("D3DXPlaneIntersectLine", v6);
		}
	}
	// D3DXPlaneTransform
	{
		D3DXPLANE pl1;
		D3DXVECTOR3 v1(10, 0.5, -30);
		D3DXVECTOR3 v2(4, 5, 6);
		D3DXVec3Normalize(&v2, &v2);
		D3DXPlaneFromPointNormal(&pl1, &v1, &v2);

		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, Math::PI / 2);

		D3DXPLANE pl2;
		D3DXPlaneTransform(&pl2, &pl1, &mat);
		//dumpD3DXPLANE("D3DXPlaneTransform", pl2);
	}
	/*
	D3DXPlaneFromPointNormal
	D3DXPlaneFromPoints

	D3DXPlaneDot
	D3DXPlaneDotCoord
	D3DXPlaneDotNormal

	D3DXPlaneNormalize

	D3DXPlaneIntersectLine
	D3DXPlaneTransform
	×D3DXPlaneTransformArray
	*/
#endif
}
