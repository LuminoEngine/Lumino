
#pragma once
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btTransform.h>

namespace ln {
namespace detail {

// Bullet とのデータ変換などのユーティリティ
class BulletUtil
{
public:
	static const btVector3 Zero;

	static Vector3 btVector3ToLNVector3(const btVector3& v)
	{
		return Vector3(v.getX(), v.getY(), v.getZ());
	}

	static btVector3 LNVector3ToBtVector3(const Vector3& v)
	{
		return btVector3(v.x, v.y, v.z);
	}

	static Quaternion btQuaternionToLNQuaternion(const btQuaternion& q)
	{
		return Quaternion(q.getX(), q.getY(), q.getZ(), q.getW());
	}

	static btQuaternion LNQuaternionToBtQuaternion(const Quaternion& q)
	{
		return btQuaternion(q.x, q.y, q.z, q.w);
	}

	static Matrix BtTransformToLNMatrix(const btTransform& t)
	{
		Matrix out;
		t.getOpenGLMatrix((btScalar*)&out);
		return out;
	}

	static btTransform LNMatrixToBtTransform(const Matrix& t)
	{
		btTransform out;
		out.setFromOpenGLMatrix((btScalar*)&t);
		return out;
	}


	static void dumpBtVector3(const btVector3& v)
	{
		printf("Vector3\n%f, %f, %f\n", v.x(), v.y(), v.z());
	}

	static void dumpBtTransform(const btTransform& t)
	{
		printf("btTransform\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n",
			t.getBasis().getRow(0).x(), t.getBasis().getRow(0).y(), t.getBasis().getRow(0).z(),
			t.getBasis().getRow(1).x(), t.getBasis().getRow(1).y(), t.getBasis().getRow(1).z(),
			t.getBasis().getRow(2).x(), t.getBasis().getRow(2).y(), t.getBasis().getRow(2).z(),
			t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());
	}
};

} // namespace detail
} // namespace ln
