#include "Test_Math_Common.hpp"

class Test_Vector2 : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Vector2, Basic)
{
	// コンストラクタ
	{
		Vector2 v1;
		ASSERT_VEC2_NEAR(0, 0, v1);

		Vector2 v2(1, 2);
		ASSERT_VEC2_NEAR(1, 2, v2);
	}
	// this->Set()
	{
		Vector2 v1;
		v1.set(1, 2);
		ASSERT_VEC2_NEAR(1, 2, v1);
	}
	// this->GetLength()
	{
		Vector2 v1(1, 2);
		ASSERT_FLOAT_EQ(2.236068f, v1.length());
	}
	// this->lengthSquared()
	{
		Vector2 v1(1, 2);
		ASSERT_FLOAT_EQ(5, v1.lengthSquared());
	}
	// this->Normaize()
	{
		Vector2 v1(1, 2);
		v1.normalize();
		ASSERT_VEC2_NEAR(0.447214, 0.894427, v1);
	}
	// this->transformCoord()
	{
		Vector2 v1(1, 2);
		Matrix m = Matrix::makeRotationYawPitchRoll(1, 2, 3);
		v1.transformCoord(m);
		ASSERT_VEC2_NEAR(-2.094393, 0.765238, v1);
	}
	// this->IsNaNOrInf
	{
		Vector2 v(1, 2);
		ASSERT_FALSE(v.isNaNOrInf());
		volatile  float d = 0.0f;
		v.x /= d;
		ASSERT_TRUE(v.isNaNOrInf());
	}

	// Vector2::Normaize()
	{
		Vector2 v1 = Vector2::normalize(Vector2(1, 2));
		ASSERT_VEC2_NEAR(0.447214, 0.894427, v1);
	}
	// Vector2::dot()
	{
		float v1 = Vector2::dot(Vector2(1, 2), Vector2(3, 4));
		EXPECT_NEAR(11.0, v1, LN_FLOAT_THRESHOLD);
	}
	// Vector2::min / max
	{
		Vector2 v1 = Vector2::min(Vector2(1, 3), Vector2(4, 2));
		ASSERT_VEC2_NEAR(1, 2, v1);

		Vector2 v2 = Vector2::max(Vector2(1, 3), Vector2(4, 2));
		ASSERT_VEC2_NEAR(4, 3, v2);
	}
	// Vector2::transform()
	{
		Matrix m = Matrix::makeRotationYawPitchRoll(1, 2, 3);
		Vector4 v1 = Vector2::transform(Vector2(1, 2), m);
		ASSERT_VEC4_NEAR(-2.094393, 0.765238, 0.167121, 1.000000, v1);
	}
	// Vector2::transformCoord()
	{
		Matrix m = Matrix::makeRotationYawPitchRoll(1, 2, 3);
		Vector2 v1 = Vector2::transformCoord(Vector2(1, 2), m);
		ASSERT_VEC2_NEAR(-2.094393, 0.765238, v1);
	}
	// Vector2::Lerp()
	{
		Vector2 v1 = Vector2::lerp(Vector2(1, 2), Vector2(3, 4), 0.75);
		ASSERT_VEC2_NEAR(2.500000, 3.500000, v1);
	}
	// Vector2::Hermite()
	{
		Vector2 v1 = Vector2::hermite(Vector2(1, 2), Vector2(3, 4), Vector2(0.3f, 0.4f), Vector2(0.03f, 0.04f), 0.75);
		ASSERT_VEC2_NEAR(0.545781, 0.831875, v1);
	}
	// Vector2::CatmullRom()
	{
		Vector2 v1 = Vector2::catmullRom(Vector2(1, 2), Vector2(3, 4), Vector2(0.3f, 0.4f), Vector2(0.03f, 0.04f), 0.75);
		ASSERT_VEC2_NEAR(0.914297, 1.203438, v1);
	}


	{
		Vector2 v1(1, 2);
		v1 += v1;
		ASSERT_VEC2_NEAR(2.0, 4.0, v1);
		v1 += 1.0;
		ASSERT_VEC2_NEAR(3.0, 5.0, v1);
		v1 -= v1;
		ASSERT_VEC2_NEAR(0.0, 0.0, v1);
		v1 -= 1.0;
		ASSERT_VEC2_NEAR(-1.0, -1.0, v1);
		v1 *= v1;
		ASSERT_VEC2_NEAR(1.0, 1.0, v1);
		v1 *= 2.0;
		ASSERT_VEC2_NEAR(2.0, 2.0, v1);
		v1 /= v1;
		ASSERT_VEC2_NEAR(1.0, 1.0, v1);
		v1 /= 2.0;
		ASSERT_VEC2_NEAR(0.5, 0.5, v1);
	}
	{
		Vector2 v1;
		v1 = Vector2(1.0f, 2.0f) + Vector2(3.0f, 4.0f);
		ASSERT_VEC2_NEAR(4.0f, 6.0f, v1);
		v1 = Vector2(1.0f, 2.0f) + 2.0f;
		ASSERT_VEC2_NEAR(3.0f, 4.0f, v1);
		v1 = 2.0f + Vector2(1.0f, 2.0f);
		ASSERT_VEC2_NEAR(3.0f, 4.0f, v1);

		v1 = Vector2(1.0f, 2.0f) - Vector2(3.0f, 4.0f);
		ASSERT_VEC2_NEAR(-2.0f, -2.0f, v1);
		v1 = Vector2(1.0f, 2.0f) - 2.0f;
		ASSERT_VEC2_NEAR(-1.0f, 0.0f, v1);
		v1 = 2.0f - Vector2(1.0f, 2.0f);
		ASSERT_VEC2_NEAR(1.0f, 0.0f, v1);

		v1 = Vector2(1.0f, 2.0f) * Vector2(3.0f, 4.0f);
		ASSERT_VEC2_NEAR(3.0f, 8.0f, v1);
		v1 = Vector2(1.0f, 2.0f) * 2.0f;
		ASSERT_VEC2_NEAR(2.0f, 4.0f, v1);
		v1 = 2.0f * Vector2(1.0f, 2.0f);
		ASSERT_VEC2_NEAR(2.0f, 4.0f, v1);

		v1 = Vector2(4.0f, 2.0f) / Vector2(2.0f, 1.0f);
		ASSERT_VEC2_NEAR(2.0f, 2.0f, v1);
		v1 = Vector2(1.0f, 2.0f) / 2.0f;
		ASSERT_VEC2_NEAR(0.5f, 1.0f, v1);
		v1 = 2.0f / Vector2(1.0f, 2.0f);
		ASSERT_VEC2_NEAR(2.0f, 1.0f, v1);
	}
	{
		Vector2 v1(4.0f, 2.0f);
		v1 = -v1;
		ASSERT_VEC2_NEAR(-4.0f, -2.0f, v1);
		bool b1 = (v1 == Vector2(-4.0f, -2.0f));
		ASSERT_EQ(true, b1);
		bool b2 = (v1 != Vector2(-4.0f, -2.0f));
		ASSERT_EQ(false, b2);
	}

#ifdef D3DX9_TEST
	{
		D3DXVECTOR2 dxv1(1, 2);
		//dumpFLOAT("D3DXVec2Length", D3DXVec2Length(&dxv1));
	}
	{
		D3DXVECTOR2 dxv1(1, 2);
		//dumpFLOAT("D3DXVec2LengthSq", D3DXVec2LengthSq(&dxv1));
	}
	{
		D3DXVECTOR2 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVec2Normalize(&dxv, &dxv1);
		//dumpD3DXVECTOR2("D3DXVec2Normalize", dxv);
	}
	{
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVECTOR2 dxv2(3, 4);
		//dumpFLOAT("D3DXVec2Dot", D3DXVec2Dot(&dxv1, &dxv2));
	}
	{
		D3DXMATRIX  dxm;
		D3DXMatrixRotationYawPitchRoll(&dxm, 1, 2, 3);
		D3DXVECTOR4 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVec2Transform(&dxv, &dxv1, &dxm);
		//dumpD3DXVECTOR4("D3DXVec2Transform", dxv);
	}
	{
		D3DXMATRIX  dxm;
		D3DXMatrixRotationYawPitchRoll(&dxm, 1, 2, 3);
		D3DXVECTOR2 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVec2TransformCoord(&dxv, &dxv1, &dxm);
		//dumpD3DXVECTOR2("D3DXVec2TransformCoord", dxv);
	}
	{
		D3DXVECTOR2 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVECTOR2 dxv2(3, 4);
		D3DXVec2Lerp(&dxv, &dxv1, &dxv2, 0.75);
		//dumpD3DXVECTOR2("D3DXVec2Lerp", dxv);
	}
	{
		D3DXVECTOR2 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVECTOR2 dxv2(3, 4);
		D3DXVECTOR2 dxv3(0.3f, 0.4f);
		D3DXVECTOR2 dxv4(0.03f, 0.04f);
		D3DXVec2Hermite(&dxv, &dxv1, &dxv2, &dxv3, &dxv4, 0.75);
		//dumpD3DXVECTOR2("D3DXVec2Hermite", dxv);
	}
	{
		D3DXVECTOR2 dxv;
		D3DXVECTOR2 dxv1(1, 2);
		D3DXVECTOR2 dxv2(3, 4);
		D3DXVECTOR2 dxv3(0.3f, 0.4f);
		D3DXVECTOR2 dxv4(0.03f, 0.04f);
		D3DXVec2CatmullRom(&dxv, &dxv1, &dxv2, &dxv3, &dxv4, 0.75);
		//dumpD3DXVECTOR2("D3DXVec2CatmullRom", dxv);
	}
#endif
}
