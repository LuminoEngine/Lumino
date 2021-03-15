
#include <math.h>
#include <assert.h>
#include <string.h>
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Vector3.hpp>
#include <LuminoCore/Math/Vector4.hpp>
#include <LuminoCore/Math/Quaternion.hpp>
#include <LuminoCore/Math/AttitudeTransform.hpp>
#include <LuminoCore/Math/Plane.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include "Asm.h"

//#define USE_D3DX9MATH
#ifdef USE_D3DX9MATH
#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib")
#endif

namespace ln {

//==============================================================================
// Matrix

const Matrix Matrix::Identity = Matrix();

Matrix::Matrix()
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

Matrix::Matrix(
    float m11,
    float m12,
    float m13,
    float m14,
    float m21,
    float m22,
    float m23,
    float m24,
    float m31,
    float m32,
    float m33,
    float m34,
    float m41,
    float m42,
    float m43,
    float m44)
{
    m[0][0] = m11;
    m[0][1] = m12;
    m[0][2] = m13;
    m[0][3] = m14;
    m[1][0] = m21;
    m[1][1] = m22;
    m[1][2] = m23;
    m[1][3] = m24;
    m[2][0] = m31;
    m[2][1] = m32;
    m[2][2] = m33;
    m[2][3] = m34;
    m[3][0] = m41;
    m[3][1] = m42;
    m[3][2] = m43;
    m[3][3] = m44;
}

Matrix::Matrix(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
{
    *((Vector4*)this->m[0]) = row1;
    *((Vector4*)this->m[1]) = row2;
    *((Vector4*)this->m[2]) = row3;
    *((Vector4*)this->m[3]) = row4;
}

//Matrix::Matrix(const Quaternion& q)
//{
//	float xx = q.X * q.X;
//	float yy = q.Y * q.Y;
//	float zz = q.Z * q.Z;
//	float xy = q.X * q.Y;
//	float zw = q.Z * q.W;
//	float zx = q.Z * q.X;
//	float yw = q.Y * q.W;
//	float yz = q.Y * q.Z;
//	float xw = q.X * q.W;
//	M11 = 1.0f - (2.0f * (yy + zz));
//	M12 = 2.0f * (xy + zw);
//	M13 = 2.0f * (zx - yw);
//	M14 = 0.0f;
//	M21 = 2.0f * (xy - zw);
//	M22 = 1.0f - (2.0f * (zz + xx));
//	M23 = 2.0f * (yz + xw);
//	M24 = 0.0f;
//	M31 = 2.0f * (zx + yw);
//	M32 = 2.0f * (yz - xw);
//	M33 = 1.0f - (2.0f * (yy + xx));
//	M34 = 0.0f;
//	M41 = 0.0f;
//	M42 = 0.0f;
//	M43 = 0.0f;
//	M44 = 1.0f;
//}

Matrix::Matrix(const AttitudeTransform& transform)
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;

    scale(transform.scale);
    rotateQuaternion(transform.rotation);
    translate(transform.translation);
}

void Matrix::set(
    float m11,
    float m12,
    float m13,
    float m14,
    float m21,
    float m22,
    float m23,
    float m24,
    float m31,
    float m32,
    float m33,
    float m34,
    float m41,
    float m42,
    float m43,
    float m44)
{
    m[0][0] = m11;
    m[0][1] = m12;
    m[0][2] = m13;
    m[0][3] = m14;
    m[1][0] = m21;
    m[1][1] = m22;
    m[1][2] = m23;
    m[1][3] = m24;
    m[2][0] = m31;
    m[2][1] = m32;
    m[2][2] = m33;
    m[2][3] = m34;
    m[3][0] = m41;
    m[3][1] = m42;
    m[3][2] = m43;
    m[3][3] = m44;
}

bool Matrix::isIdentity() const
{
    return (memcmp(this, &Matrix::Identity, sizeof(Matrix)) == 0);
}

void Matrix::translate(float x, float y, float z)
{
    m[3][0] += x;
    m[3][1] += y;
    m[3][2] += z;
}

void Matrix::translate(const Vector3& vec)
{
    m[3][0] += vec.x;
    m[3][1] += vec.y;
    m[3][2] += vec.z;
}

void Matrix::rotateX(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);

    /* 普通の行列計算
	m[0][0], m[0][1], m[0][2], m[0][3]   1, 0, 0, 0
	m[1][0], m[1][1], m[1][2], m[1][3]   0, c,-s, 0
	m[2][0], m[2][1], m[2][2], m[2][3] * 0, s, c, 0
	m[3][0], m[3][1], m[3][2], m[3][3]   0, 0, 0, 1
	*/
    /* 計算イメージ
	m[0][0] = m[0][0] * 1 + m[0][1] * 0 + m[0][2] * 0 + m[0][3] * 0;
	m[0][1] = m[0][0] * 0 + m[0][1] * c + m[0][2] * s + m[0][3] * 0;
	m[0][2] = m[0][0] * 0 + m[0][1] *-s + m[0][2] * c + m[0][3] * 0;
	m[0][3] = m[0][0] * 0 + m[0][1] * 0 + m[0][2] * 0 + m[0][3] * 1;

	m[1][0] = m[1][0] * 1 + m[1][1] * 0 + m[1][2] * 0 + m[1][3] * 0;
	m[1][1] = m[1][0] * 0 + m[1][1] * c + m[1][2] * s + m[1][3] * 0;
	m[1][2] = m[1][0] * 0 + m[1][1] *-s + m[1][2] * c + m[1][3] * 0;
	m[1][3] = m[1][0] * 0 + m[1][1] * 0 + m[1][2] * 0 + m[1][3] * 1;

	m[2][0] = m[2][0] * 1 + m[2][1] * 0 + m[2][2] * 0 + m[2][3] * 0;
	m[2][1] = m[2][0] * 0 + m[2][1] * c + m[2][2] * s + m[2][3] * 0;
	m[2][2] = m[2][0] * 0 + m[2][1] *-s + m[2][2] * c + m[2][3] * 0;
	m[2][3] = m[2][0] * 0 + m[2][1] * 0 + m[2][2] * 0 + m[2][3] * 1;

	m[3][0] = m[3][0] * 1 + m[3][1] * 0 + m[3][2] * 0 + m[3][3] * 0;
	m[3][1] = m[3][0] * 0 + m[3][1] * c + m[3][2] * s + m[3][3] * 0;
	m[3][2] = m[3][0] * 0 + m[3][1] *-s + m[3][2] * c + m[3][3] * 0;
	m[3][3] = m[3][0] * 0 + m[3][1] * 0 + m[3][2] * 0 + m[3][3] * 1;
	*/
    /* 正しく計算できるようにしたもの
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + m[0][3] * 0;
	m[0][1] = mx0 * 0 + mx1 * c + mx2 * s + m[0][3] * 0;
	m[0][2] = mx0 * 0 + mx1 *-s + mx2 * c + m[0][3] * 0;
	m[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[0][3] * 1;

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + m[1][3] * 0;
	m[1][1] = mx0 * 0 + mx1 * c + mx2 * s + m[1][3] * 0;
	m[1][2] = mx0 * 0 + mx1 *-s + mx2 * c + m[1][3] * 0;
	m[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[1][3] * 1;

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + m[2][3] * 0;
	m[2][1] = mx0 * 0 + mx1 * c + mx2 * s + m[2][3] * 0;
	m[2][2] = mx0 * 0 + mx1 *-s + mx2 * c + m[2][3] * 0;
	m[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[2][3] * 1;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + m[3][3] * 0;
	m[3][1] = mx0 * 0 + mx1 * c + mx2 * s + m[3][3] * 0;
	m[3][2] = mx0 * 0 + mx1 *-s + mx2 * c + m[3][3] * 0;
	m[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[3][3] * 1;
	*/
    /* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0;
	m[0][1] = mx1 * c + mx2 * s;
	m[0][2] = mx1 *-s + mx2 * c;
	m[0][3] = m[0][3];

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0;
	m[1][1] = mx1 * c + mx2 * s;
	m[1][2] = mx1 *-s + mx2 * c;
	m[1][3] = m[1][3];

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0;
	m[2][1] = mx1 * c + mx2 * s;
	m[2][2] = mx1 *-s + mx2 * c;
	m[2][3] = m[2][3];

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0;
	m[3][1] = mx1 * c + mx2 * s;
	m[3][2] = mx1 *-s + mx2 * c;
	m[3][3] = m[3][3];
	*/

    // 自分自身を代入しているところを切る
    float mx1 = m[0][1];
    m[0][1] = mx1 * c + m[0][2] * -s;
    m[0][2] = mx1 * s + m[0][2] * c;

    mx1 = m[1][1];
    m[1][1] = mx1 * c + m[1][2] * -s;
    m[1][2] = mx1 * s + m[1][2] * c;

    mx1 = m[2][1];
    m[2][1] = mx1 * c + m[2][2] * -s;
    m[2][2] = mx1 * s + m[2][2] * c;

    mx1 = m[3][1];
    m[3][1] = mx1 * c + m[3][2] * -s;
    m[3][2] = mx1 * s + m[3][2] * c;

    /* OpenGL
	lnFloat mx1 = m[0][1];
	m[0][1] = mx1 * c + m[0][2] * s;
	m[0][2] = mx1 *-s + m[0][2] * c;

	mx1 = m[1][1];
	m[1][1] = mx1 * c + m[1][2] * s;
	m[1][2] = mx1 *-s + m[1][2] * c;

	mx1 = m[2][1];
	m[2][1] = mx1 * c + m[2][2] * s;
	m[2][2] = mx1 *-s + m[2][2] * c;

	mx1 = m[3][1];
	m[3][1] = mx1 * c + m[3][2] * s;
	m[3][2] = mx1 *-s + m[3][2] * c;
	*/
}

void Matrix::rotateY(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);

    /* 普通の行列計算
	m[0][0], m[0][1], m[0][2], m[0][3]    c, 0, s, 0
	m[1][0], m[1][1], m[1][2], m[1][3]    0, 1, 0, 0
	m[2][0], m[2][1], m[2][2], m[2][3] * -s, 0, c, 0
	m[3][0], m[3][1], m[3][2], m[3][3]    0, 0, 0, 1
	*/
    /* 計算イメージ
	m[0][0] = m[0][0] * c + m[0][1] * 0 + m[0][2] *-s + m[0][3] * 0;
	m[0][1] = m[0][0] * 0 + m[0][1] * 1 + m[0][2] * 0 + m[0][3] * 0;
	m[0][2] = m[0][0] * s + m[0][1] * 0 + m[0][2] * c + m[0][3] * 0;
	m[0][3] = m[0][0] * 0 + m[0][1] * 0 + m[0][2] * 0 + m[0][3] * 1;

	m[1][0] = m[1][0] * c + m[1][1] * 0 + m[1][2] *-s + m[1][3] * 0;
	m[1][1] = m[1][0] * 0 + m[1][1] * 1 + m[1][2] * 0 + m[1][3] * 0;
	m[1][2] = m[1][0] * s + m[1][1] * 0 + m[1][2] * c + m[1][3] * 0;
	m[1][3] = m[1][0] * 0 + m[1][1] * 0 + m[1][2] * 0 + m[1][3] * 1;

	m[2][0] = m[2][0] * c + m[2][1] * 0 + m[2][2] *-s + m[2][3] * 0;
	m[2][1] = m[2][0] * 0 + m[2][1] * 1 + m[2][2] * 0 + m[2][3] * 0;
	m[2][2] = m[2][0] * s + m[2][1] * 0 + m[2][2] * c + m[2][3] * 0;
	m[2][3] = m[2][0] * 0 + m[2][1] * 0 + m[2][2] * 0 + m[2][3] * 1;

	m[3][0] = m[3][0] * c + m[3][1] * 0 + m[3][2] *-s + m[3][3] * 0;
	m[3][1] = m[3][0] * 0 + m[3][1] * 1 + m[3][2] * 0 + m[3][3] * 0;
	m[3][2] = m[3][0] * s + m[3][1] * 0 + m[3][2] * c + m[3][3] * 0;
	m[3][3] = m[3][0] * 0 + m[3][1] * 0 + m[3][2] * 0 + m[3][3] * 1;
	*/
    /* 正しく計算できるようにしたもの
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * c + mx1 * 0 + mx2 *-s + m[0][3] * 0;
	m[0][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + m[0][3] * 0;
	m[0][2] = mx0 * s + mx1 * 0 + mx2 * c + m[0][3] * 0;
	m[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[0][3] * 1;

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * c + mx1 * 0 + mx2 *-s + m[1][3] * 0;
	m[1][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + m[1][3] * 0;
	m[1][2] = mx0 * s + mx1 * 0 + mx2 * c + m[1][3] * 0;
	m[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[1][3] * 1;

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * c + mx1 * 0 + mx2 *-s + m[2][3] * 0;
	m[2][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + m[2][3] * 0;
	m[2][2] = mx0 * s + mx1 * 0 + mx2 * c + m[2][3] * 0;
	m[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[2][3] * 1;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * c + mx1 * 0 + mx2 *-s + m[3][3] * 0;
	m[3][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + m[3][3] * 0;
	m[3][2] = mx0 * s + mx1 * 0 + mx2 * c + m[3][3] * 0;
	m[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[3][3] * 1;
	*/
    /* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * c + mx2 *-s;
	m[0][1] = mx1;
	m[0][2] = mx0 * s + mx2 * c;
	m[0][3] = m[0][3];

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * c + mx2 *-s;
	m[1][1] = mx1;
	m[1][2] = mx0 * s + mx2 * c;
	m[1][3] = m[1][3];

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * c + mx2 *-s;
	m[2][1] = mx1;
	m[2][2] = mx0 * s + mx2 * c;
	m[2][3] = mx3;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * c + mx2 *-s;
	m[3][1] = mx1;
	m[3][2] = mx0 * s + mx2 * c;
	m[3][3] = m[3][3];
	*/

    // 自分自身を代入しているところを切る
    float mx0 = m[0][0];
    m[0][0] = mx0 * c + m[0][2] * s;
    m[0][2] = mx0 * -s + m[0][2] * c;

    mx0 = m[1][0];
    m[1][0] = mx0 * c + m[1][2] * s;
    m[1][2] = mx0 * -s + m[1][2] * c;

    mx0 = m[2][0];
    m[2][0] = mx0 * c + m[2][2] * s;
    m[2][2] = mx0 * -s + m[2][2] * c;

    mx0 = m[3][0];
    m[3][0] = mx0 * c + m[3][2] * s;
    m[3][2] = mx0 * -s + m[3][2] * c;

    /* OpenGL
	lnFloat mx0 = m[0][0];
	m[0][0] = mx0 * c + m[0][2] *-s;
	m[0][2] = mx0 * s + m[0][2] * c;

	mx0 = m[1][0];
	m[1][0] = mx0 * c + m[1][2] *-s;
	m[1][2] = mx0 * s + m[1][2] * c;

	mx0 = m[2][0];
	m[2][0] = mx0 * c + m[2][2] *-s;
	m[2][2] = mx0 * s + m[2][2] * c;

	mx0 = m[3][0];
	m[3][0] = mx0 * c + m[3][2] *-s;
	m[3][2] = mx0 * s + m[3][2] * c;
	*/
}

void Matrix::rotateZ(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);

    /* 基本の計算イメージ
	m[0][0] = m[0][0] * c + m[0][1] * s + m[0][2] * 0 + m[0][3] * 0;
	m[0][1] = m[0][0] *-s + m[0][1] * c + m[0][2] * 0 + m[0][3] * 0;
	m[0][2] = m[0][0] * 0 + m[0][1] * 0 + m[0][2] * 1 + m[0][3] * 0;
	m[0][3] = m[0][0] * 0 + m[0][1] * 0 + m[0][2] * 0 + m[0][3] * 1;

	m[1][0] = m[1][0] * c + m[1][1] * s + m[1][2] * 0 + m[1][3] * 0;
	m[1][1] = m[1][0] *-s + m[1][1] * c + m[1][2] * 0 + m[1][3] * 0;
	m[1][2] = m[1][0] * 0 + m[1][1] * 0 + m[1][2] * 1 + m[1][3] * 0;
	m[1][3] = m[1][0] * 0 + m[1][1] * 0 + m[1][2] * 0 + m[1][3] * 1;

	m[2][0] = m[2][0] * c + m[2][1] * s + m[2][2] * 0 + m[2][3] * 0;
	m[2][1] = m[2][0] *-s + m[2][1] * c + m[2][2] * 0 + m[2][3] * 0;
	m[2][2] = m[2][0] * 0 + m[2][1] * 0 + m[2][2] * 1 + m[2][3] * 0;
	m[2][3] = m[2][0] * 0 + m[2][1] * 0 + m[2][2] * 0 + m[2][3] * 1;

	m[3][0] = m[3][0] * c + m[3][1] * s + m[3][2] * 0 + m[3][3] * 0;
	m[3][1] = m[3][0] *-s + m[3][1] * c + m[3][2] * 0 + m[3][3] * 0;
	m[3][2] = m[3][0] * 0 + m[3][1] * 0 + m[3][2] * 1 + m[3][3] * 0;
	m[3][3] = m[3][0] * 0 + m[3][1] * 0 + m[3][2] * 0 + m[3][3] * 1;
	*/
    /* 正しく計算できるようにしたもの
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * c + mx1 * s + mx2 * 0 + m[0][3] * 0;
	m[0][1] = mx0 *-s + mx1 * c + mx2 * 0 + m[0][3] * 0;
	m[0][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + m[0][3] * 0;
	m[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[0][3] * 1;

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * c + mx1 * s + mx2 * 0 + m[1][3] * 0;
	m[1][1] = mx0 *-s + mx1 * c + mx2 * 0 + m[1][3] * 0;
	m[1][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + m[1][3] * 0;
	m[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[1][3] * 1;

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * c + mx1 * s + mx2 * 0 + m[2][3] * 0;
	m[2][1] = mx0 *-s + mx1 * c + mx2 * 0 + m[2][3] * 0;
	m[2][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + m[2][3] * 0;
	m[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[2][3] * 1;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * c + mx1 * s + mx2 * 0 + m[3][3] * 0;
	m[3][1] = mx0 *-s + mx1 * c + mx2 * 0 + m[3][3] * 0;
	m[3][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + m[3][3] * 0;
	m[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + m[3][3] * 1;
	*/
    /* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * c + mx1 * s;
	m[0][1] = mx0 *-s + mx1 * c;
	m[0][2] = mx2;
	m[0][3] = m[0][3];

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * c + mx1 * s;
	m[1][1] = mx0 *-s + mx1 * c;
	m[1][2] = mx2;
	m[1][3] = m[1][3];

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * c + mx1 * s;
	m[2][1] = mx0 *-s + mx1 * c;
	m[2][2] = mx2;
	m[2][3] = m[2][3];

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * c + mx1 * s;
	m[3][1] = mx0 *-s + mx1 * c;
	m[3][2] = mx2;
	m[3][3] = m[3][3];
	*/
    // 自分自身を代入しているところを切る
    float mx0 = m[0][0];
    m[0][0] = mx0 * c + m[0][1] * -s;
    m[0][1] = mx0 * s + m[0][1] * c;

    mx0 = m[1][0];
    m[1][0] = mx0 * c + m[1][1] * -s;
    m[1][1] = mx0 * s + m[1][1] * c;

    mx0 = m[2][0];
    m[2][0] = mx0 * c + m[2][1] * -s;
    m[2][1] = mx0 * s + m[2][1] * c;

    mx0 = m[3][0];
    m[3][0] = mx0 * c + m[3][1] * -s;
    m[3][1] = mx0 * s + m[3][1] * c;

    // OpenGL
    /*
	lnFloat mx0 = m[0][0];
	m[0][0] = mx0 * c + m[0][1] * s;
	m[0][1] = mx0 *-s + m[0][1] * c;

	mx0 = m[1][0];
	m[1][0] = mx0 * c + m[1][1] * s;
	m[1][1] = mx0 *-s + m[1][1] * c;

	mx0 = m[2][0];
	m[2][0] = mx0 * c + m[2][1] * s;
	m[2][1] = mx0 *-s + m[2][1] * c;

	mx0 = m[3][0];
	m[3][0] = mx0 * c + m[3][1] * s;
	m[3][1] = mx0 *-s + m[3][1] * c;
	*/
}

void Matrix::rotateEulerAngles(float x, float y, float z, RotationOrder order)
{
    rotateEulerAngles(Vector3(x, y, z), order);
}

void Matrix::rotateEulerAngles(const Vector3& angles, RotationOrder order)
{
    switch (order) {
        case RotationOrder::XYZ:
            rotateX(angles.x);
            rotateY(angles.y);
            rotateZ(angles.z);
            break;
        case RotationOrder::YZX:
            rotateY(angles.y);
            rotateZ(angles.z);
            rotateX(angles.x);
            break;
        case RotationOrder::ZXY:
            rotateZ(angles.z);
            rotateX(angles.x);
            rotateY(angles.y);
            break; // RotationYawPitchRoll
        default:
            assert(0);
            break;
    }
}

void Matrix::rotateAxis(const Vector3& axis_, float r)
{
    Vector3 axis = axis_;
    if (axis.lengthSquared() != 1.0f) {
        axis.mutatingNormalize();
    }

    float s, c;
    Asm::sincos(r, &s, &c);
    float mc = 1.0f - c;

    /* 計算イメージ
	_00 = ( axis_.x * axis_.x ) * mc + c;
	_01 = ( axis_.x * axis_.y ) * mc + ( axis_.z * s );
	_02 = ( axis_.x * axis_.z ) * mc - ( axis_.y * s );
	_03 = 0;

	_10 = ( axis_.x * axis_.y ) * mc - ( axis_.z * s );
	_11 = ( axis_.y * axis_.y ) * mc + c;
	_12 = ( axis_.y * axis_.z ) * mc + ( axis_.x * s );
	_13 = 0;

	_20 = ( axis_.x * axis_.z ) * mc + ( axis_.y * s );
	_21 = ( axis_.y * axis_.z ) * mc - ( axis_.x * s );
	_22 = ( axis_.z * axis_.z ) * mc + c;
	_23 = 0;

	_30 = _31 = _32 = 0;
	_33 = 1;

	m[0][0] = m[0][0] * _00 + m[0][1] * _10 + m[0][2] * _20 + m[0][3] * _30;
	m[0][1] = m[0][0] * _01 + m[0][1] * _11 + m[0][2] * _21 + m[0][3] * _31;
	m[0][2] = m[0][0] * _02 + m[0][1] * _12 + m[0][2] * _22 + m[0][3] * _32;
	m[0][3] = m[0][0] * _03 + m[0][1] * _13 + m[0][2] * _23 + m[0][3] * _33;

	m[1][0] = m[1][0] * _00 + m[1][1] * _10 + m[1][2] * _20 + m[1][3] * _30;
	m[1][1] = m[1][0] * _01 + m[1][1] * _11 + m[1][2] * _21 + m[1][3] * _31;
	m[1][2] = m[1][0] * _02 + m[1][1] * _12 + m[1][2] * _22 + m[1][3] * _32;
	m[1][3] = m[1][0] * _03 + m[1][1] * _13 + m[1][2] * _23 + m[1][3] * _33;

	m[2][0] = m[2][0] * _00 + m[2][1] * _10 + m[2][2] * _20 + m[2][3] * _30;
	m[2][1] = m[2][0] * _01 + m[2][1] * _11 + m[2][2] * _21 + m[2][3] * _31;
	m[2][2] = m[2][0] * _02 + m[2][1] * _12 + m[2][2] * _22 + m[2][3] * _32;
	m[2][3] = m[2][0] * _03 + m[2][1] * _13 + m[2][2] * _23 + m[2][3] * _33;

	m[3][0] = m[3][0] * _00 + m[3][1] * _10 + m[3][2] * _20 + m[3][3] * _30;
	m[3][1] = m[3][0] * _01 + m[3][1] * _11 + m[3][2] * _21 + m[3][3] * _31;
	m[3][2] = m[3][0] * _02 + m[3][1] * _12 + m[3][2] * _22 + m[3][3] * _32;
	m[3][3] = m[3][0] * _03 + m[3][1] * _13 + m[3][2] * _23 + m[3][3] * _33;
	*/

    /* 正しく計算できるようにしたもの
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + m[0][3] * _30;
	m[0][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + m[0][3] * _31;
	m[0][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + m[0][3] * _32;
	m[0][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + m[0][3] * _33;

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + m[1][3] * _30;
	m[1][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + m[1][3] * _31;
	m[1][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + m[1][3] * _32;
	m[1][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + m[1][3] * _33;

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + m[2][3] * _30;
	m[2][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + m[2][3] * _31;
	m[2][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + m[2][3] * _32;
	m[2][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + m[2][3] * _33;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + m[3][3] * _30;
	m[3][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + m[3][3] * _31;
	m[3][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + m[3][3] * _32;
	m[3][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + m[3][3] * _33;
	*/

    /* 0 を乗算してるところと、*1 で自分自身代入しているところを切る
	lfloat mx0 = m[0][0];
	lfloat mx1 = m[0][1];
	lfloat mx2 = m[0][2];
	m[0][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	m[0][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	m[0][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = m[1][0];
	mx1 = m[1][1];
	mx2 = m[1][2];
	m[1][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	m[1][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	m[1][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = m[2][0];
	mx1 = m[2][1];
	mx2 = m[2][2];
	m[2][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	m[2][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	m[2][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = m[3][0];
	mx1 = m[3][1];
	mx2 = m[3][2];
	m[3][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	m[3][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	m[3][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;
	*/

    // ※古いコードのなので変数名が 0 スタートになっている点に注意
    float _00 = (axis.x * axis.x) * mc + c;
    float _01 = (axis.x * axis.y) * mc + (axis.z * s);
    float _02 = (axis.x * axis.z) * mc - (axis.y * s);

    float _10 = (axis.x * axis.y) * mc - (axis.z * s);
    float _11 = (axis.y * axis.y) * mc + c;
    float _12 = (axis.y * axis.z) * mc + (axis.x * s);

    float _20 = (axis.x * axis.z) * mc + (axis.y * s);
    float _21 = (axis.y * axis.z) * mc - (axis.x * s);
    float _22 = (axis.z * axis.z) * mc + c;

    float mx0 = m[0][0];
    float mx1 = m[0][1];
    m[0][0] = mx0 * _00 + mx1 * _10 + m[0][2] * _20;
    m[0][1] = mx0 * _01 + mx1 * _11 + m[0][2] * _21;
    m[0][2] = mx0 * _02 + mx1 * _12 + m[0][2] * _22;

    mx0 = m[1][0];
    mx1 = m[1][1];
    m[1][0] = mx0 * _00 + mx1 * _10 + m[1][2] * _20;
    m[1][1] = mx0 * _01 + mx1 * _11 + m[1][2] * _21;
    m[1][2] = mx0 * _02 + mx1 * _12 + m[1][2] * _22;

    mx0 = m[2][0];
    mx1 = m[2][1];
    m[2][0] = mx0 * _00 + mx1 * _10 + m[2][2] * _20;
    m[2][1] = mx0 * _01 + mx1 * _11 + m[2][2] * _21;
    m[2][2] = mx0 * _02 + mx1 * _12 + m[2][2] * _22;

    mx0 = m[3][0];
    mx1 = m[3][1];
    m[3][0] = mx0 * _00 + mx1 * _10 + m[3][2] * _20;
    m[3][1] = mx0 * _01 + mx1 * _11 + m[3][2] * _21;
    m[3][2] = mx0 * _02 + mx1 * _12 + m[3][2] * _22;
}

void Matrix::rotateQuaternion(const Quaternion& q)
{
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float zw = q.z * q.w;
    float zx = q.z * q.x;
    float yw = q.y * q.w;
    float yz = q.y * q.z;
    float xw = q.x * q.w;

    // ※古いコードのなので変数名が 0 スタートになっている点に注意
    float _00 = 1.0f - (2.0f * (yy + zz));
    float _01 = 2.0f * (xy + zw);
    float _02 = 2.0f * (zx - yw);

    float _10 = 2.0f * (xy - zw);
    float _11 = 1.0f - (2.0f * (zz + xx));
    float _12 = 2.0f * (yz + xw);

    float _20 = 2.0f * (zx + yw);
    float _21 = 2.0f * (yz - xw);
    float _22 = 1.0f - (2.0f * (yy + xx));

    float mx0 = m[0][0];
    float mx1 = m[0][1];
    m[0][0] = mx0 * _00 + mx1 * _10 + m[0][2] * _20;
    m[0][1] = mx0 * _01 + mx1 * _11 + m[0][2] * _21;
    m[0][2] = mx0 * _02 + mx1 * _12 + m[0][2] * _22;

    mx0 = m[1][0];
    mx1 = m[1][1];
    m[1][0] = mx0 * _00 + mx1 * _10 + m[1][2] * _20;
    m[1][1] = mx0 * _01 + mx1 * _11 + m[1][2] * _21;
    m[1][2] = mx0 * _02 + mx1 * _12 + m[1][2] * _22;

    mx0 = m[2][0];
    mx1 = m[2][1];
    m[2][0] = mx0 * _00 + mx1 * _10 + m[2][2] * _20;
    m[2][1] = mx0 * _01 + mx1 * _11 + m[2][2] * _21;
    m[2][2] = mx0 * _02 + mx1 * _12 + m[2][2] * _22;

    mx0 = m[3][0];
    mx1 = m[3][1];
    m[3][0] = mx0 * _00 + mx1 * _10 + m[3][2] * _20;
    m[3][1] = mx0 * _01 + mx1 * _11 + m[3][2] * _21;
    m[3][2] = mx0 * _02 + mx1 * _12 + m[3][2] * _22;
}

void Matrix::scale(float x, float y, float z)
{
    m[0][0] *= x;
    m[0][1] *= y;
    m[0][2] *= z;
    m[1][0] *= x;
    m[1][1] *= y;
    m[1][2] *= z;
    m[2][0] *= x;
    m[2][1] *= y;
    m[2][2] *= z;
    m[3][0] *= x;
    m[3][1] *= y;
    m[3][2] *= z;
}

void Matrix::scale(const Vector3& vec)
{
    scale(vec.x, vec.y, vec.z);
}

void Matrix::scale(float xyz)
{
    scale(xyz, xyz, xyz);
}

void Matrix::inverse()
{
    (*this) = Matrix::makeInverse(*this);
}

void Matrix::transpose()
{
    (*this) = Matrix::makeTranspose(*this);
}

void Matrix::decompose(Vector3* scale, Quaternion* rot, Vector3* trans) const
{
    Matrix scaleMat;
    Matrix rotMat;
    Matrix transMat;
    decomposeMatrices(&scaleMat, &rotMat, &transMat);
    if (scale) {
        scale->set(scaleMat.m[0][0], scaleMat.m[1][1], scaleMat.m[2][2]);
    }
    if (rot) {
        *rot = Quaternion::makeFromRotationMatrix(rotMat);
    }
    if (trans) {
        trans->set(transMat.m[3][0], transMat.m[3][1], transMat.m[3][2]);
    }
}

void Matrix::decomposeMatrices(Matrix* scale, Matrix* rot, Matrix* trans) const
{
    if (trans) {
        *trans = Matrix::makeTranslation(position());
    }

    Vector3 sc(
        sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
        sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
        sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]));
    if (scale) {
        *scale = Matrix::makeScaling(sc);
    }

    if (rot) {
        *rot = Identity;
        rot->m[0][0] = (sc.x != 0.0f) ? m[0][0] / sc.x : 0.0f;
        rot->m[0][1] = (sc.x != 0.0f) ? m[0][1] / sc.x : 0.0f;
        rot->m[0][2] = (sc.x != 0.0f) ? m[0][2] / sc.x : 0.0f;
        rot->m[0][3] = 0.0f;
        rot->m[1][0] = (sc.y != 0.0f) ? m[1][0] / sc.y : 0.0f;
        rot->m[1][1] = (sc.y != 0.0f) ? m[1][1] / sc.y : 0.0f;
        rot->m[1][2] = (sc.y != 0.0f) ? m[1][2] / sc.y : 0.0f;
        rot->m[1][3] = 0.0f;
        rot->m[2][0] = (sc.z != 0.0f) ? m[2][0] / sc.z : 0.0f;
        rot->m[2][1] = (sc.z != 0.0f) ? m[2][1] / sc.z : 0.0f;
        rot->m[2][2] = (sc.z != 0.0f) ? m[2][2] / sc.z : 0.0f;
        rot->m[2][3] = 0.0f;
        rot->m[3][0] = 0.0f;
        rot->m[3][1] = 0.0f;
        rot->m[3][2] = 0.0f;
        rot->m[3][3] = 1.0f;
    }
}

#if 0
void Matrix::decompose(Vector3* scale, Matrix* rot, Vector3* trans)
{
	if (trans)
	{
		trans->Set(m[3][0], m[3][1], m[3][2]);
	}

	Vector3 sc(
		sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
		sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
		sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]));
	if (scale)
	{
		*scale = sc;
	}

	if (rot)
	{
		rot->m[0][0] = (sc.X != 0.0f) ? m[0][0] / sc.X : 0.0f;
		rot->m[0][1] = (sc.X != 0.0f) ? m[0][1] / sc.X : 0.0f;
		rot->m[0][2] = (sc.X != 0.0f) ? m[0][2] / sc.X : 0.0f;
		rot->m[0][3] = 0.0f;
		rot->m[1][0] = (sc.Y != 0.0f) ? m[1][0] / sc.Y : 0.0f;
		rot->m[1][1] = (sc.Y != 0.0f) ? m[1][1] / sc.Y : 0.0f;
		rot->m[1][2] = (sc.Y != 0.0f) ? m[1][2] / sc.Y : 0.0f;
		rot->m[1][3] = 0.0f;
		rot->m[2][0] = (sc.Z != 0.0f) ? m[2][0] / sc.Z : 0.0f;
		rot->m[2][1] = (sc.Z != 0.0f) ? m[2][1] / sc.Z : 0.0f;
		rot->m[2][2] = (sc.Z != 0.0f) ? m[2][2] / sc.Z : 0.0f;
		rot->m[2][3] = 0.0f;
		rot->m[3][0] = 0.0f;
		rot->m[3][1] = 0.0f;
		rot->m[3][2] = 0.0f;
		rot->m[3][3] = 1.0f;
	}
}

void Matrix::decompose(Vector3* scale, Quaternion* rot, Vector3* trans) const
{
	if (rot)
	{
		Matrix rotMat;
		decompose(scale, &rotMat, trans);
		*rot =
	}
	else
	{
		decompose(scale, NULL, trans);
	}
}
#endif

static bool EulerAnglesXYZ(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
    static const float Threshold = 0.0001f;

    if (mat.m[0][2] > 1.0f - Threshold || mat.m[0][2] < -1.0f + Threshold) // ジンバルロック判定
    {
        *xRot = 0.0f;
        *yRot = (mat.m[0][2] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        *zRot = -atan2f(-mat.m[1][0], mat.m[1][1]);
        return false;
    }

    *yRot = -asinf(mat.m[0][2]);
    *xRot = asinf(mat.m[1][2] / cosf(*yRot));

    if (Math::isNaN(*xRot)) // ジンバルロック判定
    {
        *xRot = 0.0f;
        *yRot = (mat.m[0][2] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        *zRot = -atan2f(-mat.m[1][0], mat.m[1][1]);
        return false;
    }

    if (mat.m[2][2] < 0.0f) {
        *xRot = Math::PI - (*xRot);
    }

    *zRot = atan2f(mat.m[0][1], mat.m[0][0]);
    return true;
}

static bool EulerAnglesYZX(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
    static const float Threshold = 0.0001f;

    if (mat.m[1][0] > 1.0f - Threshold || mat.m[1][0] < -1.0f + Threshold) // ジンバルロック判定
    {
        *xRot = -atan2f(-mat.m[2][1], mat.m[2][2]);
        *yRot = 0.0f;
        *zRot = (mat.m[1][0] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        return false;
    }

    *zRot = -asinf(mat.m[1][0]);
    *yRot = asinf(mat.m[2][0] / cosf(*zRot));

    if (Math::isNaN(*yRot)) // ジンバルロック判定
    {
        *xRot = -atan2f(-mat.m[2][1], mat.m[2][2]);
        *yRot = 0.0f;
        *zRot = (mat.m[1][0] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        return false;
    }

    if (mat.m[0][0] < 0.0f) {
        *yRot = Math::PI - (*yRot);
    }

    *xRot = atan2f(mat.m[1][2], mat.m[1][1]);
    return true;
}

static bool EulerAnglesZXY(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
    static const float Threshold = 0.0001f;
    //if (locked) { *locked = true; }		// 最初にジンバルロック発生状態にしておく

    if (mat.m[2][1] > 1.0f - Threshold || mat.m[2][1] < -1.0f + Threshold) // ジンバルロック判定
    {
        *xRot = (mat.m[2][1] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        *yRot = (float)atan2f(-mat.m[0][2], mat.m[0][0]);
        *zRot = 0.0f;
        return false;
    }

    *xRot = -(float)asinf(mat.m[2][1]);
    *zRot = (float)asinf(mat.m[0][1] / cosf(*xRot));

    if (Math::isNaN(*zRot)) // ジンバルロック判定
    {
        *xRot = (mat.m[2][1] < 0 ? Math::PIDiv2 : -Math::PIDiv2);
        *yRot = (float)atan2f(-mat.m[0][2], mat.m[0][0]);
        *zRot = 0.0f;
        return false;
    }
    if (mat.m[1][1] < 0.0f) {
        *zRot = Math::PI - (*zRot);
    }

    *yRot = (float)atan2f(mat.m[2][0], mat.m[2][2]);

    //if (locked) { *locked = false; }	// ジンバルロックは発生しなかった
    //return Vector3(xRot, yRot, zRot);
    return true;
}

Vector3 Matrix::toEulerAngles(RotationOrder order, bool* locked_) const
{
    bool locked;
    float xRot, yRot, zRot;
    switch (order) {
        case RotationOrder::XYZ:
            locked = !EulerAnglesXYZ(*this, &xRot, &yRot, &zRot);
            break;
        //case RotationOrder_XZY: break;
        //case RotationOrder_YXZ: break;
        case RotationOrder::YZX:
            locked = !EulerAnglesYZX(*this, &xRot, &yRot, &zRot);
            break;
        case RotationOrder::ZXY:
            locked = !EulerAnglesZXY(*this, &xRot, &yRot, &zRot); // RotationYawPitchRoll
            break;
        //case RotationOrder_ZYX: break;
        default:
            assert(0);
            return Vector3();
    }

    if (locked_ != NULL) {
        *locked_ = locked;
    }
    return Vector3(xRot, yRot, zRot);
}

Matrix Matrix::getRotationMatrix() const
{
    return Matrix(
        m[0][0], m[0][1], m[0][2], 0.0f, m[1][0], m[1][1], m[1][2], 0.0f, m[2][0], m[2][1], m[2][2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

bool Matrix::isNaNOrInf() const
{
    return Math::isNaNOrInf(m[0][0]) || Math::isNaNOrInf(m[0][1]) || Math::isNaNOrInf(m[0][2]) || Math::isNaNOrInf(m[0][3]) ||
           Math::isNaNOrInf(m[1][0]) || Math::isNaNOrInf(m[1][1]) || Math::isNaNOrInf(m[1][2]) || Math::isNaNOrInf(m[1][3]) ||
           Math::isNaNOrInf(m[2][0]) || Math::isNaNOrInf(m[2][1]) || Math::isNaNOrInf(m[2][2]) || Math::isNaNOrInf(m[2][3]) ||
           Math::isNaNOrInf(m[3][0]) || Math::isNaNOrInf(m[3][1]) || Math::isNaNOrInf(m[3][2]) || Math::isNaNOrInf(m[3][3]);
}

String Matrix::toString() const
{
    const std::size_t BufferLength = 4096;
    char text[BufferLength];
    StringHelper::vsprintf2(
        text, BufferLength, "Matrix((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]);
    return String::fromCString(text);
}

const float& Matrix::operator()(int row, int column) const
{
	LN_CHECK(0 <= row && row <= 3);
	LN_CHECK(0 <= column && column <= 3);
	return m[row][column];
}

float & Matrix::operator()(int row, int column)
{
	LN_CHECK(0 <= row && row <= 3);
	LN_CHECK(0 <= column && column <= 3);
	return m[row][column];
}

// static
Matrix Matrix::multiply(const Matrix& mat1, const Matrix& mat2)
{
    Matrix out;

    out.m[0][0] = mat1.m[0][0] * mat2.m[0][0] + mat1.m[0][1] * mat2.m[1][0] + mat1.m[0][2] * mat2.m[2][0] + mat1.m[0][3] * mat2.m[3][0];
    out.m[0][1] = mat1.m[0][0] * mat2.m[0][1] + mat1.m[0][1] * mat2.m[1][1] + mat1.m[0][2] * mat2.m[2][1] + mat1.m[0][3] * mat2.m[3][1];
    out.m[0][2] = mat1.m[0][0] * mat2.m[0][2] + mat1.m[0][1] * mat2.m[1][2] + mat1.m[0][2] * mat2.m[2][2] + mat1.m[0][3] * mat2.m[3][2];
    out.m[0][3] = mat1.m[0][0] * mat2.m[0][3] + mat1.m[0][1] * mat2.m[1][3] + mat1.m[0][2] * mat2.m[2][3] + mat1.m[0][3] * mat2.m[3][3];

    out.m[1][0] = mat1.m[1][0] * mat2.m[0][0] + mat1.m[1][1] * mat2.m[1][0] + mat1.m[1][2] * mat2.m[2][0] + mat1.m[1][3] * mat2.m[3][0];
    out.m[1][1] = mat1.m[1][0] * mat2.m[0][1] + mat1.m[1][1] * mat2.m[1][1] + mat1.m[1][2] * mat2.m[2][1] + mat1.m[1][3] * mat2.m[3][1];
    out.m[1][2] = mat1.m[1][0] * mat2.m[0][2] + mat1.m[1][1] * mat2.m[1][2] + mat1.m[1][2] * mat2.m[2][2] + mat1.m[1][3] * mat2.m[3][2];
    out.m[1][3] = mat1.m[1][0] * mat2.m[0][3] + mat1.m[1][1] * mat2.m[1][3] + mat1.m[1][2] * mat2.m[2][3] + mat1.m[1][3] * mat2.m[3][3];

    out.m[2][0] = mat1.m[2][0] * mat2.m[0][0] + mat1.m[2][1] * mat2.m[1][0] + mat1.m[2][2] * mat2.m[2][0] + mat1.m[2][3] * mat2.m[3][0];
    out.m[2][1] = mat1.m[2][0] * mat2.m[0][1] + mat1.m[2][1] * mat2.m[1][1] + mat1.m[2][2] * mat2.m[2][1] + mat1.m[2][3] * mat2.m[3][1];
    out.m[2][2] = mat1.m[2][0] * mat2.m[0][2] + mat1.m[2][1] * mat2.m[1][2] + mat1.m[2][2] * mat2.m[2][2] + mat1.m[2][3] * mat2.m[3][2];
    out.m[2][3] = mat1.m[2][0] * mat2.m[0][3] + mat1.m[2][1] * mat2.m[1][3] + mat1.m[2][2] * mat2.m[2][3] + mat1.m[2][3] * mat2.m[3][3];

    out.m[3][0] = mat1.m[3][0] * mat2.m[0][0] + mat1.m[3][1] * mat2.m[1][0] + mat1.m[3][2] * mat2.m[2][0] + mat1.m[3][3] * mat2.m[3][0];
    out.m[3][1] = mat1.m[3][0] * mat2.m[0][1] + mat1.m[3][1] * mat2.m[1][1] + mat1.m[3][2] * mat2.m[2][1] + mat1.m[3][3] * mat2.m[3][1];
    out.m[3][2] = mat1.m[3][0] * mat2.m[0][2] + mat1.m[3][1] * mat2.m[1][2] + mat1.m[3][2] * mat2.m[2][2] + mat1.m[3][3] * mat2.m[3][2];
    out.m[3][3] = mat1.m[3][0] * mat2.m[0][3] + mat1.m[3][1] * mat2.m[1][3] + mat1.m[3][2] * mat2.m[2][3] + mat1.m[3][3] * mat2.m[3][3];

    return out;
}

// static
Matrix Matrix::makeTranslation(float x, float y, float z)
{
    return Matrix(
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f);
}

// static
Matrix Matrix::makeTranslation(const Vector3& vec)
{
    return Matrix(
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, vec.x, vec.y, vec.z, 1.0f);
}

// static
Matrix Matrix::makeRotationX(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);
    return Matrix(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c, s, 0.0f,
        0.0f,-s, c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeRotationY(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);
    return Matrix(
        c, 0.0f, -s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, s, 0.0f, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeRotationZ(float r)
{
    float c, s;
    Asm::sincos(r, &s, &c);
    return Matrix(
        c, s, 0.0f, 0.0f,
        -s, c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeRotationAxis(const Vector3& axis_, float r)
{
    Vector3 axis = axis_;
    if (axis.lengthSquared() != 1.0f) {
        axis.mutatingNormalize();
    }

    float s, c;
    Asm::sincos(r, &s, &c);
    float mc = 1.0f - c;

    return Matrix(
        (axis.x * axis.x) * mc + c, (axis.x * axis.y) * mc + (axis.z * s), (axis.x * axis.z) * mc - (axis.y * s), 0.0f, (axis.x * axis.y) * mc - (axis.z * s), (axis.y * axis.y) * mc + c, (axis.y * axis.z) * mc + (axis.x * s), 0.0f, (axis.x * axis.z) * mc + (axis.y * s), (axis.y * axis.z) * mc - (axis.x * s), (axis.z * axis.z) * mc + c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeRotationQuaternion(const Quaternion& qua)
{
    float xx = qua.x * qua.x;
    float yy = qua.y * qua.y;
    float zz = qua.z * qua.z;
    float xy = qua.x * qua.y;
    float zw = qua.z * qua.w;
    float zx = qua.z * qua.x;
    float yw = qua.y * qua.w;
    float yz = qua.y * qua.z;
    float xw = qua.x * qua.w;

    return Matrix(
        1.0f - (2.0f * (yy + zz)), 2.0f * (xy + zw), 2.0f * (zx - yw), 0.0f, 2.0f * (xy - zw), 1.0f - (2.0f * (zz + xx)), 2.0f * (yz + xw), 0.0f, 2.0f * (zx + yw), 2.0f * (yz - xw), 1.0f - (2.0f * (yy + xx)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeRotationEulerAngles(const Vector3& angles, RotationOrder order)
{
    Matrix m;
    switch (order) {
        case RotationOrder::XYZ:
            m.rotateX(angles.x);
            m.rotateY(angles.y);
            m.rotateZ(angles.z);
            break;
        //case RotationOrder_XZY:
        //	m.rotateX(angles.X); m.rotateZ(angles.Z); m.rotateY(angles.Y); break;
        //case RotationOrder_YXZ:
        //	m.rotateY(angles.Y); m.rotateX(angles.X); m.rotateZ(angles.Z); break;
        case RotationOrder::YZX:
            m.rotateY(angles.y);
            m.rotateZ(angles.z);
            m.rotateX(angles.x);
            break;
        case RotationOrder::ZXY:
            m.rotateZ(angles.z);
            m.rotateX(angles.x);
            m.rotateY(angles.y);
            break; // RotationYawPitchRoll
        //case RotationOrder_ZYX:
        //	m.rotateZ(angles.Z); m.rotateY(angles.Y); m.rotateX(angles.X); break;
        default:
            assert(0);
            return m;
    }
    return m;
}

// static
Matrix Matrix::makeRotationEulerAngles(float x, float y, float z, RotationOrder order)
{
    Matrix m;
    switch (order) {
        case RotationOrder::XYZ:
            m.rotateX(x);
            m.rotateY(y);
            m.rotateZ(z);
            break;
        case RotationOrder::YZX:
            m.rotateY(y);
            m.rotateZ(z);
            m.rotateX(y);
            break;
        case RotationOrder::ZXY:
            m.rotateZ(z);
            m.rotateX(x);
            m.rotateY(y);
            break; // RotationYawPitchRoll
        default:
            assert(0);
            return m;
    }
    return m;
}

// static
Matrix Matrix::makeRotationYawPitchRoll(float yaw, float pitch, float roll)
{
    Quaternion q = Quaternion::makeFromYawPitchRoll(yaw, pitch, roll);
    return Matrix::makeRotationQuaternion(q);
}

// static
Matrix Matrix::makeScaling(float x, float y, float z)
{
    return Matrix(
        x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeScaling(const Vector3& vec)
{
    return Matrix(
        vec.x, 0.0f, 0.0f, 0.0f, 0.0f, vec.y, 0.0f, 0.0f, 0.0f, 0.0f, vec.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeScaling(float xyz)
{
    return Matrix(
        xyz, 0.0f, 0.0f, 0.0f, 0.0f, xyz, 0.0f, 0.0f, 0.0f, 0.0f, xyz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// static
Matrix Matrix::makeInverse(const Matrix& mat)
{
#if 1 // 速度は D3DXMatrixInverse の 2～3倍 (100000 回で 3000us。コンストラクタとかが足引っ張ってそうな気がする…)
    float coef00 = mat.m[2][2] * mat.m[3][3] - mat.m[3][2] * mat.m[2][3];
    float coef02 = mat.m[1][2] * mat.m[3][3] - mat.m[3][2] * mat.m[1][3];
    float coef03 = mat.m[1][2] * mat.m[2][3] - mat.m[2][2] * mat.m[1][3];

    float coef04 = mat.m[2][1] * mat.m[3][3] - mat.m[3][1] * mat.m[2][3];
    float coef06 = mat.m[1][1] * mat.m[3][3] - mat.m[3][1] * mat.m[1][3];
    float coef07 = mat.m[1][1] * mat.m[2][3] - mat.m[2][1] * mat.m[1][3];

    float coef08 = mat.m[2][1] * mat.m[3][2] - mat.m[3][1] * mat.m[2][2];
    float coef10 = mat.m[1][1] * mat.m[3][2] - mat.m[3][1] * mat.m[1][2];
    float coef11 = mat.m[1][1] * mat.m[2][2] - mat.m[2][1] * mat.m[1][2];

    float coef12 = mat.m[2][0] * mat.m[3][3] - mat.m[3][0] * mat.m[2][3];
    float coef14 = mat.m[1][0] * mat.m[3][3] - mat.m[3][0] * mat.m[1][3];
    float coef15 = mat.m[1][0] * mat.m[2][3] - mat.m[2][0] * mat.m[1][3];

    float coef16 = mat.m[2][0] * mat.m[3][2] - mat.m[3][0] * mat.m[2][2];
    float coef18 = mat.m[1][0] * mat.m[3][2] - mat.m[3][0] * mat.m[1][2];
    float coef19 = mat.m[1][0] * mat.m[2][2] - mat.m[2][0] * mat.m[1][2];

    float coef20 = mat.m[2][0] * mat.m[3][1] - mat.m[3][0] * mat.m[2][1];
    float coef22 = mat.m[1][0] * mat.m[3][1] - mat.m[3][0] * mat.m[1][1];
    float coef23 = mat.m[1][0] * mat.m[2][1] - mat.m[2][0] * mat.m[1][1];

    Vector4 fac0(coef00, coef00, coef02, coef03);
    Vector4 fac1(coef04, coef04, coef06, coef07);
    Vector4 fac2(coef08, coef08, coef10, coef11);
    Vector4 fac3(coef12, coef12, coef14, coef15);
    Vector4 fac4(coef16, coef16, coef18, coef19);
    Vector4 fac5(coef20, coef20, coef22, coef23);

    Vector4 vec0(mat.m[1][0], mat.m[0][0], mat.m[0][0], mat.m[0][0]);
    Vector4 vec1(mat.m[1][1], mat.m[0][1], mat.m[0][1], mat.m[0][1]);
    Vector4 vec2(mat.m[1][2], mat.m[0][2], mat.m[0][2], mat.m[0][2]);
    Vector4 vec3(mat.m[1][3], mat.m[0][3], mat.m[0][3], mat.m[0][3]);

    Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
    Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
    Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
    Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

    Vector4 signA(+1, -1, +1, -1);
    Vector4 signB(-1, +1, -1, +1);
    Matrix inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

    Vector4 dot0(mat.m[0][0] * inverse.m[0][0], mat.m[0][1] * inverse.m[1][0], mat.m[0][2] * inverse.m[2][0], mat.m[0][3] * inverse.m[3][0]);
    float dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

    if (dot1 == 0.0) {
        // 計算できない。D3D に合わせて、単位行列で返す
        return Identity;
    }

    float oneOverDeterminant = 1.0f / dot1;

    return inverse * oneOverDeterminant;
#endif
#if 0 // 速度は D3DXMatrixInverse の 10倍
	int i, j;
	float buf;

	Matrix	tm(mat_);
	identity(out_);

	// 掃き出し法
	for (i = 0; i < 4; ++i)
	{
		//if (tm.m[ i ][ i ] != 0)
		buf = 1.0f / tm.m[i][i];	// 0除算の可能性がある
		//else
		//	buf = 0.0;
		for (j = 0; j < 4; ++j)
		{
			tm.m[i][j] *= buf;
			out_->m[i][j] *= buf;
		}
		for (j = 0; j < 4; ++j)
		{
			if (i != j)
			{
				buf = tm.m[j][i];
				for (int k = 0; k < 4; ++k)
				{
					tm.m[j][k] -= tm.m[i][k] * buf;
					out_->m[j][k] -= out_->m[i][k] * buf;
				}
			}
		}
	}
#endif
#ifdef USE_D3DX9MATH
    Matrix out;
    D3DXMatrixInverse((D3DXMATRIX*)&out, NULL, (D3DXMATRIX*)&mat);
    return out;
#endif
#if 0
	// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

	float m0011 = mat_.m[0][0] * mat_.m[1][1];
	float m0012 = mat_.m[0][0] * mat_.m[1][2];
	float m0021 = mat_.m[0][0] * mat_.m[2][1];
	float m0022 = mat_.m[0][0] * mat_.m[2][2];

	float m0110 = mat_.m[0][1] * mat_.m[1][0];
	float m0112 = mat_.m[0][1] * mat_.m[1][2];
	float m0120 = mat_.m[0][1] * mat_.m[2][0];
	float m0122 = mat_.m[0][1] * mat_.m[2][2];

	float m0210 = mat_.m[0][2] * mat_.m[1][0];
	float m0211 = mat_.m[0][2] * mat_.m[1][1];
	float m0220 = mat_.m[0][2] * mat_.m[2][0];
	float m0221 = mat_.m[0][2] * mat_.m[2][1];

	float m1021 = mat_.m[1][0] * mat_.m[2][1];
	float m1022 = mat_.m[1][0] * mat_.m[2][2];

	float m1120 = mat_.m[1][1] * mat_.m[2][0];
	float m1122 = mat_.m[1][1] * mat_.m[2][2];

	float m1220 = mat_.m[1][2] * mat_.m[2][0];
	float m1221 = mat_.m[1][2] * mat_.m[2][1];


	float m1122_m1221 = m1122 - m1221;
	float m1220_m1022 = m1220 - m1022;
	float m1021_m1120 = m1021 - m1120;


	float delta = mat_.m[0][0] * (m1122_m1221)+mat_.m[0][1] * (m1220_m1022)+mat_.m[0][2] * (m1021_m1120);
	float rcpDelta = 1.f / delta;

	Matrix mat;

	mat.m[0][0] = (m1122_m1221)* rcpDelta;
	mat.m[1][0] = (m1220_m1022)* rcpDelta;
	mat.m[2][0] = (m1021_m1120)* rcpDelta;
	mat.m[0][1] = (m0221 - m0122) * rcpDelta;
	mat.m[1][1] = (m0022 - m0220) * rcpDelta;
	mat.m[2][1] = (m0120 - m0021) * rcpDelta;
	mat.m[0][2] = (m0112 - m0211) * rcpDelta;
	mat.m[1][2] = (m0210 - m0012) * rcpDelta;
	mat.m[2][2] = (m0011 - m0110) * rcpDelta;

	float t03 = mat_.m[0][3];
	float t13 = mat_.m[1][3];
	mat.m[0][3] = -(mat.m[0][0] * t03 + mat.m[0][1] * t13 + mat.m[0][2] * mat_.m[2][3]);
	mat.m[1][3] = -(mat.m[1][0] * t03 + mat.m[1][1] * t13 + mat.m[1][2] * mat_.m[2][3]);
	mat.m[2][3] = -(mat.m[2][0] * t03 + mat.m[2][1] * t13 + mat.m[2][2] * mat_.m[2][3]);

	return mat;
#endif
}

// static
Matrix Matrix::makeTranspose(const Matrix& mat)
{
    return Matrix(
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0], mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1], mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2], mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

// static
Matrix Matrix::makeReflection(const Plane& plane_)
{
    Plane plane = Plane::normalize(plane_);
    float x = plane.normal.x;
    float y = plane.normal.y;
    float z = plane.normal.z;
    float x2 = -2.0f * x;
    float y2 = -2.0f * y;
    float z2 = -2.0f * z;

    return Matrix(
        (x2 * x) + 1.0f, y2 * x, z2 * x, 0.0f, x2 * y, (y2 * y) + 1.0f, z2 * y, 0.0f, x2 * z, y2 * z, (z2 * z) + 1.0f, 0.0f, x2 * plane.distance, y2 * plane.distance, z2 * plane.distance, 1.0f);
}

// static
Matrix Matrix::makeLookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up)
{
    Vector3 xaxis, yaxis, zaxis;
    // 注視点からカメラ位置までのベクトルをZ軸とする
    zaxis = lookAt;
    zaxis -= position;
    zaxis.mutatingNormalize();
    // Z軸と上方向のベクトルの外積をとるとX軸が分かる
    xaxis = Vector3::cross(up, zaxis);
    xaxis.mutatingNormalize();
    // 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
    yaxis = Vector3::cross(zaxis, xaxis);

    return Matrix(
        xaxis.x, yaxis.x, zaxis.x, 0.0f,
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        -(xaxis.x * position.x + xaxis.y * position.y + xaxis.z * position.z),
        -(yaxis.x * position.x + yaxis.y * position.y + yaxis.z * position.z),
        -(zaxis.x * position.x + zaxis.y * position.y + zaxis.z * position.z),
        1.0f);
}

// static
Matrix Matrix::makeLookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3& up)
{
    Vector3 xaxis, yaxis, zaxis;
    // 注視点からカメラ位置までのベクトルをZ軸とする
    zaxis = position;
    zaxis -= lookAt;
    zaxis.mutatingNormalize();
    // Z軸と上方向のベクトルの外積をとるとX軸が分かる
    xaxis = Vector3::cross(up, zaxis);
    xaxis.mutatingNormalize();
    // 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
    yaxis = Vector3::cross(zaxis, xaxis);

    return Matrix(
        xaxis.x, yaxis.x, zaxis.x, 0.0f, 
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        -(xaxis.x * position.x + xaxis.y * position.y + xaxis.z * position.z),
        -(yaxis.x * position.x + yaxis.y * position.y + yaxis.z * position.z),
        -(zaxis.x * position.x + zaxis.y * position.y + zaxis.z * position.z),
        1.0f);
}

// static
Matrix Matrix::makePerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ)
{
    float h = 1.f / tanf(fovY * 0.5f); // cot(fovY/2)
    return Matrix(
        h / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, h, 0.0f, 0.0f,
        0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f,
        0.0f, 0.0f, (-nearZ * farZ) / (farZ - nearZ), 0.0f);
}

// static
Matrix Matrix::makePerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ)
{
    float h = 1.f / tanf(fovY * 0.5f); // cot(fovY/2)
    return Matrix(
        h / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, h, 0.0f, 0.0f,
        0.0f, 0.0f, farZ / (nearZ - farZ), -1.0f,
        0.0f, 0.0f, (nearZ * farZ) / (nearZ - farZ), 0.0f);
}

// static
Matrix Matrix::makeOrthoLH(float width, float height, float nearZ, float farZ)
{
    return Matrix(
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f / (farZ - nearZ),
        0.0f, 0.0f, 0.0f, -nearZ / (nearZ - farZ), 1.0f);
}

// static
Matrix Matrix::makeOrthoRH(float width, float height, float nearZ, float farZ)
{
    return Matrix(
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f / (nearZ - farZ), 0.0f,
        0.0f, 0.0f, nearZ / (nearZ - farZ), 1.0f);
}

// static
Matrix Matrix::makePerspective2DLH(float width, float height, float nearZ, float farZ)
{
    return Matrix(
        2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f / (farZ - nearZ), 0.0f,
		-1.0f, 1.0f, -nearZ / (nearZ - farZ) + 1.0f, 1.0f);
}

// static
Matrix Matrix::makePerspective2DRH(float width, float height, float nearZ, float farZ)
{
    return Matrix(
        -2.0f / width,0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f / (nearZ - farZ), 0.0f,
		-1.0f, 1.0f, nearZ / (nearZ - farZ)/* + 1.0f*/, 1.0f);
}

// static
Matrix Matrix::makeAffineTransformation(const Vector3& scaling, const Vector3& rotationCenter, const Quaternion& rotation, const Vector3& translation)
{
    Matrix m = Matrix::makeScaling(scaling);
    m.translate(-rotationCenter);
    m.rotateQuaternion(rotation);
    m.translate(rotationCenter);
    m.translate(translation);
    return m;
}

Matrix Matrix::extractRotation(const Matrix& mat)
{
    Matrix result;
    auto& d = result.m;
    const auto& s = mat.m;

    float scaleX = 1.0f / Vector3(s[0][0], s[1][0], s[2][0]).length();
    float scaleY = 1.0f / Vector3(s[0][1], s[1][1], s[2][1]).length();
    float scaleZ = 1.0f / Vector3(s[0][2], s[1][2], s[2][2]).length();

    d[0][0] = s[0][0] * scaleX;
    d[1][0] = s[1][0] * scaleX;
    d[2][0] = s[2][0] * scaleX;
    
    d[0][1] = s[0][1] * scaleY;
    d[1][1] = s[1][1] * scaleY;
    d[2][1] = s[2][1] * scaleY;

    d[0][2] = s[0][2] * scaleZ;
    d[1][2] = s[1][2] * scaleZ;
    d[2][2] = s[2][2] * scaleZ;

    return result;
}

// Note: inverse(lookAtLH) と同じ結果 (誤差はあるけど)
Matrix Matrix::makeAffineLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    if (target == eye) return Matrix::Identity;

    // left-hand coord
    Vector3 f = Vector3::normalize(target - eye);


    Vector3 s = Vector3::cross(up, f);
    if (Vector3::nearEqual(s, Vector3::Zero))
    {
        if (Math::nearEqual(std::abs(up.z), 1.0f)) {
            f.x += 0.0001f;
        }
        else {
            f.z += 0.0001f;
        }

        f.mutatingNormalize();
        s = Vector3::cross(up, f);
    }

    s.mutatingNormalize();

    Vector3 u = Vector3::cross(f, s);
    return Matrix(
        s.x, s.y, s.z, 0.0f,
        u.x, u.y, u.z, 0.0f,
        f.x, f.y, f.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix& Matrix::operator*=(const Matrix& matrix)
{
    float mx0 = m[0][0];
    float mx1 = m[0][1];
    float mx2 = m[0][2];
    m[0][0] = mx0 * matrix.m[0][0] + mx1 * matrix.m[1][0] + mx2 * matrix.m[2][0] + m[0][3] * matrix.m[3][0];
    m[0][1] = mx0 * matrix.m[0][1] + mx1 * matrix.m[1][1] + mx2 * matrix.m[2][1] + m[0][3] * matrix.m[3][1];
    m[0][2] = mx0 * matrix.m[0][2] + mx1 * matrix.m[1][2] + mx2 * matrix.m[2][2] + m[0][3] * matrix.m[3][2];
    m[0][3] = mx0 * matrix.m[0][3] + mx1 * matrix.m[1][3] + mx2 * matrix.m[2][3] + m[0][3] * matrix.m[3][3];
    mx0 = m[1][0];
    mx1 = m[1][1];
    mx2 = m[1][2];
    m[1][0] = mx0 * matrix.m[0][0] + mx1 * matrix.m[1][0] + mx2 * matrix.m[2][0] + m[1][3] * matrix.m[3][0];
    m[1][1] = mx0 * matrix.m[0][1] + mx1 * matrix.m[1][1] + mx2 * matrix.m[2][1] + m[1][3] * matrix.m[3][1];
    m[1][2] = mx0 * matrix.m[0][2] + mx1 * matrix.m[1][2] + mx2 * matrix.m[2][2] + m[1][3] * matrix.m[3][2];
    m[1][3] = mx0 * matrix.m[0][3] + mx1 * matrix.m[1][3] + mx2 * matrix.m[2][3] + m[1][3] * matrix.m[3][3];
    mx0 = m[2][0];
    mx1 = m[2][1];
    mx2 = m[2][2];
    m[2][0] = mx0 * matrix.m[0][0] + mx1 * matrix.m[1][0] + mx2 * matrix.m[2][0] + m[2][3] * matrix.m[3][0];
    m[2][1] = mx0 * matrix.m[0][1] + mx1 * matrix.m[1][1] + mx2 * matrix.m[2][1] + m[2][3] * matrix.m[3][1];
    m[2][2] = mx0 * matrix.m[0][2] + mx1 * matrix.m[1][2] + mx2 * matrix.m[2][2] + m[2][3] * matrix.m[3][2];
    m[2][3] = mx0 * matrix.m[0][3] + mx1 * matrix.m[1][3] + mx2 * matrix.m[2][3] + m[2][3] * matrix.m[3][3];
    mx0 = m[3][0];
    mx1 = m[3][1];
    mx2 = m[3][2];
    m[3][0] = mx0 * matrix.m[0][0] + mx1 * matrix.m[1][0] + mx2 * matrix.m[2][0] + m[3][3] * matrix.m[3][0];
    m[3][1] = mx0 * matrix.m[0][1] + mx1 * matrix.m[1][1] + mx2 * matrix.m[2][1] + m[3][3] * matrix.m[3][1];
    m[3][2] = mx0 * matrix.m[0][2] + mx1 * matrix.m[1][2] + mx2 * matrix.m[2][2] + m[3][3] * matrix.m[3][2];
    m[3][3] = mx0 * matrix.m[0][3] + mx1 * matrix.m[1][3] + mx2 * matrix.m[2][3] + m[3][3] * matrix.m[3][3];
    return (*this);
}

Matrix operator*(const Matrix& mat1, const Matrix& mat2)
{
    return Matrix::multiply(mat1, mat2);
}

Matrix operator*(const Matrix& mat1, float v)
{
    return Matrix(
        mat1.m[0][0] * v, mat1.m[0][1] * v, mat1.m[0][2] * v, mat1.m[0][3] * v, mat1.m[1][0] * v, mat1.m[1][1] * v, mat1.m[1][2] * v, mat1.m[1][3] * v, mat1.m[2][0] * v, mat1.m[2][1] * v, mat1.m[2][2] * v, mat1.m[2][3] * v, mat1.m[3][0] * v, mat1.m[3][1] * v, mat1.m[3][2] * v, mat1.m[3][3] * v);
}

static bool equals(const Matrix& value1, const Matrix& value2)
{
    return (
        value1.m11 == value2.m11 && value1.m12 == value2.m12 && value1.m13 == value2.m13 && value1.m14 == value2.m14 &&
        value1.m21 == value2.m21 && value1.m22 == value2.m22 && value1.m23 == value2.m23 && value1.m24 == value2.m24 &&
        value1.m31 == value2.m31 && value1.m32 == value2.m32 && value1.m33 == value2.m33 && value1.m34 == value2.m34 &&
        value1.m41 == value2.m41 && value1.m42 == value2.m42 && value1.m43 == value2.m43 && value1.m44 == value2.m44);
}

bool Matrix::operator==(const Matrix& mat) const
{
    return equals(*this, mat);
}

bool Matrix::operator!=(const Matrix& mat) const
{
    return !equals(*this, mat);
}

} // namespace ln
