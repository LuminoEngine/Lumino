#include "Test_Math_Common.hpp"

class Test_Vector3 : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Vector3, Basic)
{
	Vector3 sample1(1, 2, 3);

	// const
	{
		ASSERT_VEC3_NEAR(0, 0, 0, Vector3::Zero);
		ASSERT_VEC3_NEAR(1, 0, 0, Vector3::UnitX);
		ASSERT_VEC3_NEAR(0, 1, 0, Vector3::UnitY);
		ASSERT_VEC3_NEAR(0, 0, 1, Vector3::UnitZ);
		ASSERT_VEC3_NEAR(1, 1, 1, Vector3::Ones);
	}
	// コンストラクタ
	{
		Vector3 v1;
		Vector3 v2(1, 2, 3);
		Vector3 v3(Vector2(1, 2), 3);
		Vector3 v4 = v3;
		ASSERT_VEC3_NEAR(0, 0, 0, v1);
		ASSERT_VEC3_NEAR(1, 2, 3, v2);
		ASSERT_VEC3_NEAR(1, 2, 3, v3);
		ASSERT_VEC3_NEAR(1, 2, 3, v4);
	}
	// this->Set
	{
		Vector3 v1;
		v1.set(1, 2 ,3);
		ASSERT_VEC3_NEAR(1, 2, 3, v1);
	}
	// this->xy
	{
		Vector2 v = sample1.xy();
		ASSERT_VEC2_NEAR(1, 2, v);
	}
	// this->GetLength / lengthSquared
	{
		ASSERT_NEAR(3.741657, sample1.length(), LN_FLOAT_THRESHOLD);
		ASSERT_NEAR(14.000000, sample1.lengthSquared(), LN_FLOAT_THRESHOLD);
	}
	// isZero
	{
		ASSERT_EQ(true, Vector3(0, 0.0f, -0.0f).isZero());
		ASSERT_EQ(false, Vector3(0, 1.0f, -1.0f).isZero());
	}
	// this->normalize
	{
		Vector3 v1 = sample1;
		v1.mutatingNormalize();
		ASSERT_VEC3_NEAR(0.267261, 0.534522, 0.801784, v1);
	}
	// this->Clamp
	{
		Vector3 v1(1, 2, 3);
		v1.clamp(Vector3(0, 4, 2), Vector3(0.5, 5, 3));
		ASSERT_VEC3_NEAR(0.5, 4, 3, v1);
	}
	// this->transformCoord
	{
		Matrix m =
			Matrix::makeRotationYawPitchRoll(1, 2, 3) *
			Matrix::makeLookAtLH(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(0, 1, 0)) *
			Matrix::makePerspectiveFovLH(1.3f, 1.5f, 1.0f, 100.0f);
		Vector3 v1 = sample1;
		v1.transformCoord(m);
		ASSERT_VEC3_NEAR(0.058951, 0.021860, 1.160729, v1);
	}
	// this->IsNaNOrInf
	{
		Vector3 v(1, 2, 3);
		ASSERT_FALSE(v.isNaNOrInf());
		volatile  float d = 0.0f;
		v.x /= d;
		ASSERT_TRUE(v.isNaNOrInf());
	}

	// Vector3::normalize
	{
		Vector3 v1 = Vector3::normalize(sample1);
		Vector3 v2 = Vector3::normalize(1, 2, 3);
		ASSERT_VEC3_NEAR(0.267261, 0.534522, 0.801784, v1);
		ASSERT_VEC3_NEAR(0.267261, 0.534522, 0.801784, v2);
	}
	// Vector3::distance
	{
		Vector3 v1(1, 2, 3);
		Vector3 v2(4, 5, 6);
		ASSERT_NEAR((v2 - v1).length(), Vector3::distance(v1, v2), LN_FLOAT_THRESHOLD);
		ASSERT_NEAR((v2 - v1).length(), Vector3::distance(v2, v1), LN_FLOAT_THRESHOLD);
	}
	// Vector3::distanceSquared
	{
		Vector3 v1(1, 2, 3);
		Vector3 v2(4, 5, 6);
		ASSERT_NEAR((v2 - v1).lengthSquared(), Vector3::distanceSquared(v1, v2), LN_FLOAT_THRESHOLD);
		ASSERT_NEAR((v2 - v1).lengthSquared(), Vector3::distanceSquared(v2, v1), LN_FLOAT_THRESHOLD);
	}
	// Vector3::dot
	{
		Vector3 v1(5, 6, 7);
		float d = Vector3::dot(sample1, v1);
		ASSERT_FLOAT_EQ(38.000000, d);
	}
	// Vector3::cross
	{
		Vector3 v1 = Vector3::cross(sample1, Vector3(5, 6, 7));
		ASSERT_VEC3_NEAR(-4.000000, 8.000000, -4.000000, v1);
	}
	// Vector3::min / max
	{
		Vector3 v1 = Vector3::min(Vector3(1, 3, 5), Vector3(4, 2, 6));
		ASSERT_VEC3_NEAR(1, 2, 5, v1);

		Vector3 v2 = Vector3::max(Vector3(1, 3, 5), Vector3(4, 2, 6));
		ASSERT_VEC3_NEAR(4, 3, 6, v2);
	}
	// Vector3::reflect
	{
		Vector3 v1 = Vector3::reflect(Vector3(1, -1, 1), Vector3(0, 1, 0));
		ASSERT_VEC3_NEAR(1, 1, 1, v1);
	}
	// Vector3::slide
	{
		Vector3 v1 = Vector3::slide(Vector3(1, -1, 1), Vector3(0, 1, 0));
		ASSERT_VEC3_NEAR(1, 0, 1, v1);
	}
	// Vector3::transform
	{
		Matrix m = 
			Matrix::makeRotationYawPitchRoll(1, 2, 3) *
			Matrix::makeLookAtLH(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(0, 1, 0)) *
			Matrix::makePerspectiveFovLH(1.3f, 1.5f, 1.0f, 100.0f);
		Vector4 v1 = Vector3::transform(sample1, m);
		ASSERT_VEC4_NEAR(-0.395318, -0.146594, -7.783754, -6.705917, v1);
	}
	// Vector3::transformCoord
	{
		Matrix m =
			Matrix::makeRotationYawPitchRoll(1, 2, 3) *
			Matrix::makeLookAtLH(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(0, 1, 0)) *
			Matrix::makePerspectiveFovLH(1.3f, 1.5f, 1.0f, 100.0f);
		Vector3 v1 = Vector3::transformCoord(sample1, m);
		ASSERT_VEC3_NEAR(0.058951, 0.021860, 1.160729, v1);
	}
	// Vector3::Lerp()
	{
		Vector3 v1 = Vector3::lerp(
			Vector3(1, 2, 3),
			Vector3(3, 4, 7),
			0.75);
		ASSERT_VEC3_NEAR(2.500000, 3.500000, 6.000000, v1);
	}
	// Vector3::Hermite()
	{
		Vector3 v1 = Vector3::hermite(
			Vector3(1, 2, 3),
			Vector3(3, 4, 7),
			Vector3(0.3f, 0.4f, -0.5f),
			Vector3(0.03f, 0.04f, -1.5f),
			0.75);
		ASSERT_VEC3_NEAR(0.545781, 0.831875, 0.585938, v1);
	}
	// Vector3::CatmullRom()
	{
		Vector3 v1 = Vector3::catmullRom(
			Vector3(1, 2, 3),
			Vector3(3, 4, 7),
			Vector3(0.3f, 0.4f, -0.5f),
			Vector3(0.03f, 0.04f, -1.5f),
			0.75);
		ASSERT_VEC3_NEAR(0.914297, 1.203437, 1.187500, v1);
	}
	// Vector3::project / unproject
	{
		Matrix m =
			Matrix::makeRotationYawPitchRoll(1, 2, 3) *
			Matrix::makeLookAtLH(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(0, 1, 0)) *
			Matrix::makePerspectiveFovLH(1.3f, 1.5f, 1.0f, 100.0f);
		Vector3 v1(1, 2, 3);
		Vector3 v;
		v = Vector3::project(v1, m, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR(338.864197, 234.753510, 1.160729, v);
		v = Vector3::unproject(v1, m, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR(1.590889, -1.387909, -2.265153, v);

		Matrix m2 =
			Matrix::makeLookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 10), Vector3(0, 1, 0)) *
			Matrix::makePerspectiveFovLH(Math::PI / 3, 640.0f / 480.0f, 1.0f, 1000.0f);

		v = Vector3::unproject(Vector3(320, 240, 0), m2, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR(0, 0, 1.0f, v);
		v = Vector3::unproject(Vector3(320, 240, 1), m2, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR_THRESHOLD(0, 0, 1000.01288f, v, 1.0f);
		v = Vector3::unproject(Vector3(0, 0, 0), m2, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR(-0.769800305, 0.577350199, 1.0f, v);
		v = Vector3::unproject(Vector3(640, 0, 0), m2, 0, 0, 640, 480);
		ASSERT_VEC3_NEAR(0.769800305, 0.577350199, 1.0f, v);
	}
	// assign operator
	{
		Vector3 v1;

		v1.set(1, 2, 3);
		v1 += Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(2, 4, 6, v1);
		v1.set(1, 2, 3);
		v1 += 5;
		ASSERT_VEC3_NEAR(6, 7, 8, v1);

		v1.set(1, 2, 3);
		v1 -= Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(0, 0, 0, v1);
		v1.set(1, 2, 3);
		v1 -= 5;
		ASSERT_VEC3_NEAR(-4, -3, -2, v1);

		v1.set(1, 2, 3);
		v1 *= Vector3(5, 6, 7);
		ASSERT_VEC3_NEAR(5, 12, 21, v1);
		v1.set(1, 2, 3);
		v1 *= 5;
		ASSERT_VEC3_NEAR(5, 10, 15, v1);

		v1.set(1, 2, 3);
		v1 /= Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(1, 1, 1, v1);
		v1.set(10, 20, 30);
		v1 /= 5;
		ASSERT_VEC3_NEAR(2, 4, 6, v1);
	}
	// binary operator
	{
		Vector3 v1;

		v1 = Vector3(1, 2, 3) + Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(2, 4, 6, v1);
		v1 = Vector3(1, 2, 3) + 5;
		ASSERT_VEC3_NEAR(6, 7, 8, v1);
		v1 = 6 + Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(7, 8, 9, v1);

		v1 = Vector3(1, 2, 3) - Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(0, 0, 0, v1);
		v1 = Vector3(1, 2, 3) - 5;
		ASSERT_VEC3_NEAR(-4, -3, -2, v1);
		v1 = 6 - Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(5, 4, 3, v1);

		v1 = Vector3(1, 2, 3) * Vector3(5, 6, 7);
		ASSERT_VEC3_NEAR(5, 12, 21, v1);
		v1 = Vector3(1, 2, 3) * 5;
		ASSERT_VEC3_NEAR(5, 10, 15, v1);
		v1 = 6 * Vector3(1, 2, 3);
		ASSERT_VEC3_NEAR(6, 12, 18, v1);

		v1 = Vector3(10, 20, 30) / Vector3(10, 20, 30);
		ASSERT_VEC3_NEAR(1, 1, 1, v1);
		v1 = Vector3(10, 20, 30) / 5;
		ASSERT_VEC3_NEAR(2, 4, 6, v1);
		v1 = 20 / Vector3(1, 2, 4);
		ASSERT_VEC3_NEAR(20, 10, 5, v1);

		v1.set(2, 4, 6);
		ASSERT_TRUE(v1 == Vector3(2, 4, 6));
		ASSERT_FALSE(v1 != Vector3(2, 4, 6));
	}
	// unary operator
	{
		Vector3 v1(1, 2, 3);
		v1 = -v1;
		ASSERT_VEC3_NEAR(-1, -2, -3, v1);
	}

#ifdef D3DX9_TEST
	D3DXVECTOR3 dxsample1(1, 2, 3);

	// D3DXVec3Length / D3DXVec3LengthSq
	{
		//dumpFLOAT("D3DXVec3Length", D3DXVec3Length(&dxsample1));
		//dumpFLOAT("D3DXVec3LengthSq", D3DXVec3LengthSq(&dxsample1));
	}
	// D3DXVec3Normalize
	{
		D3DXVECTOR3 v1;
		D3DXVec3Normalize(&v1, &dxsample1);
		//dumpD3DXVECTOR3("D3DXVec3Normalize", v1);
	}
	// D3DXVec3Dot
	{
		D3DXVECTOR3 v1(5, 6, 7);
		//dumpFLOAT("D3DXVec3Dot", D3DXVec3Dot(&dxsample1, &v1));
	}
	// D3DXVec3Cross
	{
		D3DXVECTOR3 v1(5, 6, 7);
		D3DXVECTOR3 v;
		D3DXVec3Cross(&v, &dxsample1, &v1);
		//dumpD3DXVECTOR3("D3DXVec3Cross", v);
	}
	// D3DXVec3Transform
	{
		D3DXMATRIX world;
		D3DXMatrixRotationYawPitchRoll(&world, 1, 2, 3);

		D3DXVECTOR3 pos(1, 2, 3);
		D3DXVECTOR3 look(4, 5, 6);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMATRIX view;
		D3DXMatrixLookAtLH(&view, &pos, &look, &up);

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, 1.3f, 1.5f, 1.0f, 100.0f);

		D3DXMATRIX wvp = world * view * proj;

		D3DXVECTOR4 v1;
		D3DXVec3Transform(&v1, &dxsample1, &wvp);
		//dumpD3DXVECTOR4("D3DXVec3Transform", v1);
	}
	// D3DXVec3TransformCoord
	{
		D3DXMATRIX world;
		D3DXMatrixRotationYawPitchRoll(&world, 1, 2, 3);

		D3DXVECTOR3 pos(1, 2, 3);
		D3DXVECTOR3 look(4, 5, 6);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMATRIX view;
		D3DXMatrixLookAtLH(&view, &pos, &look, &up);

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, 1.3f, 1.5f, 1.0f, 100.0f);

		D3DXMATRIX wvp = world * view * proj;

		D3DXVECTOR3 v1;
		D3DXVec3TransformCoord(&v1, &dxsample1, &wvp);
		//dumpD3DXVECTOR3("D3DXVec3TransformCoord", v1);
	}
	// D3DXVec3Project / D3DXVec3Unproject
	{
		D3DXMATRIX world;
		D3DXMatrixRotationYawPitchRoll(&world, 1, 2, 3);

		D3DXVECTOR3 pos(1, 2, 3);
		D3DXVECTOR3 look(4, 5, 6);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMATRIX view;
		D3DXMatrixLookAtLH(&view, &pos, &look, &up);

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, 1.3f, 1.5f, 1.0f, 100.0f);

		D3DXMATRIX wvp = world * view * proj;

		D3DXVECTOR3 v1(1, 2, 3);
		D3DVIEWPORT9 vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = 640;
		vp.Height = 480;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		D3DXVECTOR3 v;
		D3DXVec3Project(&v, &v1, &vp, &proj, &view, &world);
		//dumpD3DXVECTOR3("D3DXVec3Project", v);
		D3DXVec3Unproject(&v, &v1, &vp, &proj, &view, &world);
		//dumpD3DXVECTOR3("D3DXVec3Unproject", v);
	}
#endif
}
