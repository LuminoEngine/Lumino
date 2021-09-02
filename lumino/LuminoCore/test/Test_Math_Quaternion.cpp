#include "Test_Math_Common.hpp"

class Test_Quaternion : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};


//------------------------------------------------------------------------------
TEST_F(Test_Quaternion, Basic)
{
	const Quaternion sample1 = Quaternion::makeFromYawPitchRoll(0.5, 0.75, -1.0);
	const Quaternion sample2 = Quaternion(1, 2, 3, 4);

	// コンストラクタ
	{
		Quaternion q1;
		ASSERT_TRUE(q1.isIdentity());
		Quaternion q2(1, 2, 3, 4);
		ASSERT_QUA_NEAR(1, 2, 3, 4, q2);

		Vector3 axis(-1, 2, 3);
		Quaternion q3(axis, -0.75);
		ASSERT_QUA_NEAR(0.097890, -0.195781, -0.293671, 0.930508, q3);
	}
	// this->Set
	{
		Quaternion q1;
		q1.set(1, 2, 3, 4);
		ASSERT_QUA_NEAR(1, 2, 3, 4, q1);
	}
	// this->GetLength / lengthSquared
	{
		ASSERT_FLOAT_EQ(1.0f, sample1.length());
		ASSERT_FLOAT_EQ(1.0f, sample1.lengthSquared());

		Quaternion q1(1, 2, 3, 4);
		ASSERT_FLOAT_EQ(5.477226f, q1.length());
		ASSERT_FLOAT_EQ(30.000000, q1.lengthSquared());
	}
	// this->normalize
	{
		Quaternion q1 = sample1;
		Quaternion q2 = sample2;
		q1.normalize();
		q2.normalize();
		ASSERT_QUA_NEAR(0.201073, 0.372171, -0.511765, 0.747767, q1);
		ASSERT_QUA_NEAR(0.182574, 0.365148, 0.547723, 0.730297, q2);
	}
	// this->IsIdentity
	{
		Quaternion q1(0, 0, 0, 1);
		Quaternion q2(1, 0, 0, 1);
		ASSERT_TRUE(q1.isIdentity());
		ASSERT_FALSE(q2.isIdentity());
		ASSERT_TRUE(Quaternion::Identity.isIdentity());
	}
	// this->Multiply
	{
		Quaternion q1 = sample1;
		q1.multiply(sample2);
		ASSERT_QUA_NEAR(-0.587985, 4.099200, 0.166267, 3.580948, q1);

		Quaternion q2 = sample2;
		q2.multiply(sample1);
		ASSERT_QUA_NEAR(3.692100, 1.869235, 0.226216, 3.580948, q2);
	}
	// this->rotateX/Y/Z
	{
		Quaternion q1 = Quaternion::makeFromYawPitchRoll(0.5, 0.75, 1.0);

		// YawPitchRoll と同じ
		Quaternion q2;
		q2.rotateZ(1.0);
		q2.rotateX(0.75);
		q2.rotateY(0.5);
		ASSERT_QUA_NEAR(0.421811, 0.031888, 0.352717, 0.834655, q2);

		// X → Y → Z
		Quaternion q3;
		q3.rotateX(0.2f);
		q3.rotateY(0.3f);
		q3.rotateZ(0.4f);
		ASSERT_QUA_NEAR(0.067204, 0.165339, 0.180836, 0.967184, q3);
	}
	// this->toEulerAngles
	{
		Quaternion q1 = Quaternion::makeFromYawPitchRoll(0.5, 0.75, 1.0);
		Vector3 r1 = q1.toEulerAngles();
		Vector3 r2 = q1.toEulerAngles(RotationOrder::XYZ);
		Vector3 r3 = q1.toEulerAngles(RotationOrder::YZX);

		Quaternion q2 = Quaternion::makeFromYawPitchRoll(0, 0.75, 0);
		q2.multiply(Quaternion::makeFromYawPitchRoll(0.25, 0, 0));
		q2.multiply(Quaternion::makeFromYawPitchRoll(0, 0, 0.5));
		Vector3 r4 = q2.toEulerAngles(RotationOrder::XYZ);

		// X → Y → Z
		Quaternion q5 = Quaternion::makeFromRotationAxis(Vector3(1, 0, 0), 0.2f);
		q5.multiply(Quaternion::makeFromRotationAxis(Vector3(0, 1, 0), 0.3f));
		q5.multiply(Quaternion::makeFromRotationAxis(Vector3(0, 0, 1), 0.4f));
		bool locked;
		Vector3 r5 = q5.toEulerAngles(RotationOrder::XYZ, &locked);
		ASSERT_VEC3_NEAR(0.200000f, 0.300000f, 0.400000f, r5);
		ASSERT_EQ(false, locked);	// ジンバルロックしない

		// X → Y → Z
		Quaternion q6;
		q6.rotateX(0.2f);
		q6.rotateY(0.3f);
		q6.rotateZ(0.4f);
		Vector3 r6 = q6.toEulerAngles(RotationOrder::XYZ);
		ASSERT_VEC3_NEAR(0.200000f, 0.300000f, 0.400000f, r6);

		// Y → Z → X
		Quaternion q7;
		q7.rotateY(0.3f);
		q7.rotateZ(0.4f);
		q7.rotateX(0.2f);
		Vector3 r7 = q7.toEulerAngles(RotationOrder::YZX);
		ASSERT_VEC3_NEAR(0.200000f, 0.300000f, 0.400000f, r7);

		// Z → X → Y
		Quaternion q8;
		q8.rotateZ(0.4f);
		q8.rotateX(0.2f);
		q8.rotateY(0.3f);
		Vector3 r8 = q8.toEulerAngles(RotationOrder::ZXY);
		ASSERT_VEC3_NEAR(0.200000f, 0.300000f, 0.400000f, r8);
	}
	// this->toAxisAngle
	{
		Quaternion q = Quaternion::makeFromYawPitchRoll(1, 2, 3);
		Vector3 axis;
		float angle;
		q.toAxisAngle(&axis, &angle);

		ASSERT_VEC3_NEAR(0.345148, -0.798124, 0.493833, axis);
		ASSERT_NEAR(2.239399, angle, LN_FLOAT_THRESHOLD);
	}
	// this->IsNaNOrInf
	{
		Quaternion v(1, 2, 3, 4);
		ASSERT_FALSE(v.isNaNOrInf());
		volatile  float d = 0.0f;
		v.x /= d;
		ASSERT_TRUE(v.isNaNOrInf());
	}

	// Quaternion::conjugate
	{
		Quaternion q1 = Quaternion::conjugate(sample1);
		Quaternion q2 = Quaternion::conjugate(sample2);
		ASSERT_QUA_NEAR(-0.201073, -0.372171, 0.511765, 0.747767, q1);
		ASSERT_QUA_NEAR(-1.000000, -2.000000, -3.000000, 4.000000, q2);
	}
	// Quaternion::dot
	{
		float r = Quaternion::dot(sample1, sample2);
		ASSERT_FLOAT_EQ(2.401188f, r);
	}
	// Quaternion::normalize
	{
		Quaternion q1 = Quaternion::normalize(sample1);
		Quaternion q2 = Quaternion::normalize(sample2);
		ASSERT_QUA_NEAR(0.201073, 0.372171, -0.511765, 0.747767, q1);
		ASSERT_QUA_NEAR(0.182574, 0.365148, 0.547723, 0.730297, q2);
	}
	// Quaternion::makeInverse
	{
		Quaternion q1 = Quaternion::makeInverse(sample1);
		Quaternion q2 = Quaternion::makeInverse(sample2);
		ASSERT_QUA_NEAR(-0.201073, -0.372171, 0.511765, 0.747767, q1);
		ASSERT_QUA_NEAR(-0.033333, -0.066667, -0.100000, 0.133333, q2);
	}
	// Quaternion::Multiply
	{
		Quaternion q1 = Quaternion::multiply(sample1, sample2);
		Quaternion q2 = Quaternion::multiply(sample2, sample1);
		ASSERT_QUA_NEAR(-0.587985, 4.099200, 0.166267, 3.580948, q1);
		ASSERT_QUA_NEAR(3.692100, 1.869235, 0.226216, 3.580948, q2);
	}
	// Quaternion::makeFromRotationAxis
	{
		Vector3 axis1(0, 1, 0);
		Quaternion q1 = Quaternion::makeFromRotationAxis(axis1, 0.5);

		Vector3 axis2(-1, 2, 3);
		Quaternion q2 = Quaternion::makeFromRotationAxis(axis2, -0.75);

		ASSERT_QUA_NEAR(0.000000, 0.247404, 0.000000, 0.968912, q1);
		ASSERT_QUA_NEAR(0.097890, -0.195781, -0.293671, 0.930508, q2);
	}
	// Quaternion::makeFromYawPitchRoll
	{
		Quaternion q = Quaternion::makeFromYawPitchRoll(1, 2, 3);
		ASSERT_QUA_NEAR(0.310622, -0.718287, 0.444435, 0.435953, q);
	}
	// Quaternion::makeFromEulerAngles
	{
		// デフォルト引数は RotationYawPitchRoll と同じ
		Quaternion q1 = Quaternion::makeFromEulerAngles(Vector3(2, 1, 3));
		ASSERT_QUA_NEAR(0.310622, -0.718287, 0.444435, 0.435953, q1);

		// X → Y → Z
		Quaternion q2 = Quaternion::makeFromEulerAngles(Vector3(0.2f, 0.3f, 0.4f), RotationOrder::XYZ);
		ASSERT_QUA_NEAR(0.067204, 0.165339, 0.180836, 0.967184, q2);

		// Y → Z → X
		Quaternion q3 = Quaternion::makeFromEulerAngles(Vector3(0.2f, 0.3f, 0.4f), RotationOrder::YZX);
		ASSERT_QUA_NEAR(0.067204, 0.126117, 0.210079, 0.967184, q3);

		// Z → X → Y
		Quaternion q4 = Quaternion::makeFromEulerAngles(Vector3(0.2f, 0.3f, 0.4f), RotationOrder::ZXY);
		ASSERT_QUA_NEAR(0.126285, 0.126117, 0.180836, 0.967184, q4);
	}
	// Quaternion::slerp
	{
		Quaternion q1 = Quaternion::makeFromRotationAxis(Vector3(0, 1, 0), 0.5f);
		Quaternion q2 = Quaternion::makeFromRotationAxis(Vector3(0, 0, 1), -0.25f);
		Quaternion q3 = Quaternion::slerp(q1, q2, 0.75f);
		ASSERT_QUA_NEAR(0.000000, 0.062610, -0.094040, 0.993598, q3);
	}

	// assign operator
	{
		Quaternion q1;

		q1.set(1, 2, 3, 4);
		q1 += Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(2, 4, 6, 8, q1);
		q1.set(1, 2, 3, 4);
		q1 += 5;
		ASSERT_QUA_NEAR(6, 7, 8, 9, q1);

		q1.set(1, 2, 3, 4);
		q1 -= Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(0, 0, 0, 0, q1);
		q1.set(1, 2, 3, 4);
		q1 -= 5;
		ASSERT_QUA_NEAR(-4, -3, -2, -1, q1);

		// * は特別
		q1.set(1, 2, 3, 4);
		q1 *= Quaternion(5, 6, 7, 8);
		ASSERT_QUA_NEAR(32.000000, 32.000000, 56.000000, -6.000000, q1);
		q1.set(1, 2, 3, 4);
		q1 *= 5;
		ASSERT_QUA_NEAR(5, 10, 15, 20, q1);

		q1.set(10, 20, 30, 40);
		q1 /= 5;
		ASSERT_QUA_NEAR(2, 4, 6, 8, q1);
	}
	// binary operator
	{
		Quaternion q1;

		q1 = Quaternion(1, 2, 3, 4) + Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(2, 4, 6, 8, q1);
		q1 = Quaternion(1, 2, 3, 4) + 5;
		ASSERT_QUA_NEAR(6, 7, 8, 9, q1);
		q1 = 6 + Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(7, 8, 9, 10, q1);

		q1 = Quaternion(1, 2, 3, 4) - Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(0, 0, 0, 0, q1);
		q1 = Quaternion(1, 2, 3, 4) - 5;
		ASSERT_QUA_NEAR(-4, -3, -2, -1, q1);
		q1 = 6 - Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(5, 4, 3, 2, q1);

		// * は特別
		q1 = Quaternion(1, 2, 3, 4) * Quaternion(5, 6, 7, 8);
		ASSERT_QUA_NEAR(32.000000, 32.000000, 56.000000, -6.000000, q1);
		q1 = Quaternion(1, 2, 3, 4) * 5;
		ASSERT_QUA_NEAR(5, 10, 15, 20, q1);
		q1 = 6 * Quaternion(1, 2, 3, 4);
		ASSERT_QUA_NEAR(6, 12, 18, 24, q1);

		q1 = Quaternion(10, 20, 30, 40) / 5;
		ASSERT_QUA_NEAR(2, 4, 6, 8, q1);

		ASSERT_TRUE(q1 == Quaternion(2, 4, 6, 8));
		ASSERT_FALSE(q1 != Quaternion(2, 4, 6, 8));
	}
	// unary operator
	{
		Quaternion q1(1, 2, 3, 4);
		q1 = -q1;
		ASSERT_QUA_NEAR(-1, -2, -3, -4, q1);

	}

	{
		Matrix mat1;
		mat1.rotateX(Math::PI);
		mat1.rotateY(Math::PI / 2);

		//mat1.print(NULL, NULL);

		Quaternion q1;
		Quaternion q2;
		q1.rotateX(Math::PI);
		q2.rotateY(Math::PI / 2);

		//Quaternion q3 = Quaternion::multiply(q1, q2);
		//Quaternion q3 = q1 * q2;
		Quaternion q3 = q1;
		q3 *= q2;

		Matrix mat2 = Matrix::makeRotationQuaternion(q3);

		//mat2.print(NULL, NULL);

		//printf("");

	}

#ifdef D3DX9_TEST

	D3DXQUATERNION dxsample1;
	D3DXQuaternionRotationYawPitchRoll(&dxsample1, 0.5, 0.75, -1.0);
	D3DXQUATERNION dxsample2(1, 2, 3, 4);

	// D3DXQuaternionLength
	{
		FLOAT r = D3DXQuaternionLength(&dxsample1);
		//dumpFLOAT("D3DXQuaternionLength", r);
		r = D3DXQuaternionLengthSq(&dxsample1);
		//dumpFLOAT("D3DXQuaternionLengthSq", r);

		//D3DXQUATERNION q1(1, 2, 3, 4);
		//dumpFLOAT("D3DXQuaternionLength", D3DXQuaternionLength(&q1));
		//dumpFLOAT("D3DXQuaternionLengthSq", D3DXQuaternionLengthSq(&q1));
	}
	// D3DXQuaternionNormalize
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionNormalize(&q1, &dxsample1);
		D3DXQuaternionNormalize(&q2, &dxsample2);
		//dumpD3DXQUATERNION("D3DXQuaternionNormalize", q1);
		//dumpD3DXQUATERNION("D3DXQuaternionNormalize", q2);
	}
	// D3DXQuaternionConjugate
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionConjugate(&q1, &dxsample1);
		D3DXQuaternionConjugate(&q2, &dxsample2);
		//dumpD3DXQUATERNION("D3DXQuaternionConjugate", q1);
		//dumpD3DXQUATERNION("D3DXQuaternionConjugate", q2);
	}
	// D3DXQuaternionConjugate
	{
		//dumpFLOAT("D3DXQuaternionDot", D3DXQuaternionDot(&dxsample1, &dxsample2));
	}
	// D3DXQuaternionMultiply
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionMultiply(&q1, &dxsample1, &dxsample2);
		D3DXQuaternionMultiply(&q2, &dxsample2, &dxsample1);
		//dumpD3DXQUATERNION("D3DXQuaternionMultiply", q1);
		//dumpD3DXQUATERNION("D3DXQuaternionMultiply", q2);
	}
	// D3DXQuaternionInverse
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionInverse(&q1, &dxsample1);
		D3DXQuaternionInverse(&q2, &dxsample2);
		//dumpD3DXQUATERNION("D3DXQuaternionInverse", q1);
		//dumpD3DXQUATERNION("D3DXQuaternionInverse", q2);
	}
	// D3DXQuaternionRotationAxis
	{
		D3DXVECTOR3 axis1(0, 1, 0);
		D3DXQUATERNION q1;
		D3DXQuaternionRotationAxis(&q1, &axis1, 0.5);

		D3DXVECTOR3 axis2(-1, 2, 3);
		D3DXQUATERNION q2;
		D3DXQuaternionRotationAxis(&q2, &axis2, -0.75);

		//dumpD3DXQUATERNION("D3DXQuaternionRotationAxis", q1);
		//dumpD3DXQUATERNION("D3DXQuaternionRotationAxis", q2);
	}
	// D3DXQuaternionRotationYawPitchRoll
	{
		D3DXQUATERNION dxq;
		D3DXQuaternionRotationYawPitchRoll(&dxq, 1, 2, 3);
		//dumpD3DXQUATERNION("D3DXQuaternionRotationYawPitchRoll", dxq);
	}
	// D3DXQuaternionToAxisAngle
	{
		D3DXQUATERNION dxq;
		D3DXQuaternionRotationYawPitchRoll(&dxq, 1, 2, 3);
		D3DXVECTOR3 axis;
		FLOAT angle;
		D3DXQuaternionToAxisAngle(&dxq, &axis, &angle);
		//dumpD3DXVECTOR3("D3DXQuaternionToAxisAngle", axis);
		//dumpFLOAT("D3DXQuaternionToAxisAngle", angle);
		//dumpD3DXVECTOR3("D3DXQuaternionToAxisAngle N", *D3DXVec3Normalize(&axis, &axis));
	}
	// D3DXQuaternionRotationAxis
	{
		D3DXVECTOR3 axis1(0, 1, 0);
		D3DXQUATERNION q1;
		D3DXQuaternionRotationAxis(&q1, &axis1, 0.5);

		D3DXVECTOR3 axis2(0, 0, 1);
		D3DXQUATERNION q2;
		D3DXQuaternionRotationAxis(&q2, &axis2, -0.25f);

		D3DXQUATERNION q3;
		D3DXQuaternionSlerp(&q3, &q1, &q2, 0.75f);

		//dumpD3DXQUATERNION("D3DXQuaternionSlerp", q3);
	}
	/*
	D3DXQuaternionIdentity
	D3DXQuaternionLength
	D3DXQuaternionLengthSq
	D3DXQuaternionNormalize
	D3DXQuaternionConjugate

	D3DXQuaternionDot
	×D3DXQuaternionExp
	D3DXQuaternionInverse

	D3DXQuaternionIsIdentity
	×D3DXQuaternionLn
	D3DXQuaternionMultiply

	D3DXQuaternionRotationAxis
	D3DXQuaternionRotationMatrix
	D3DXQuaternionRotationYawPitchRoll
	
	D3DXQuaternionSlerp
	×D3DXQuaternionSquad
	×D3DXQuaternionSquadSetup
	D3DXQuaternionToAxisAngle
*/
	{
		D3DXQUATERNION dxq;
		D3DXQUATERNION dxq1(1, 2, 3, 4);
		D3DXQUATERNION dxq2(5, 6, 7, 8);
		//dumpD3DXQUATERNION("operator*", dxq1 * dxq2);

		dxq = dxq1;
		dxq *= dxq2;
		//dumpD3DXQUATERNION("operator*=", dxq);

		dxq = dxq1 * dxq2;
		//dumpD3DXQUATERNION("operator*", dxq);


		D3DXQuaternionMultiply(&dxq, &dxq1, &dxq2);
		//dumpD3DXQUATERNION("D3DXQuaternionMultiply", dxq);
	}
#endif
}
