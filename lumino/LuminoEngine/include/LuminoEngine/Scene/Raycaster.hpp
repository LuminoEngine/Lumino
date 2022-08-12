
#pragma once
#include "Common.hpp"

namespace ln {

/** レイキャスティングを支援するためのクラスです。 */
LN_CLASS()
class Raycaster
	: public Object
{
	LN_OBJECT;
public:
	// 0.0 ~ view actual size
	// インスタンス作成と setFrom2DView() のユーティリティ。
	// グローバルなインスタンスを共有する。
	static Raycaster* fromScreen(Camera* camera, const Point& point);

	/** メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。 */
	LN_METHOD()
	static Raycaster* fromScreen(const Point& point);

	// -1.0~1.0
	//static Raycaster* fromViewport(Camera* camera, const Vector2& coords);

	void setFrom2DView(Camera* camera, const Point& point);
	//void setFromViewport(Camera* camera, const Point& point);

	//bool isHit() const { return m_isHit; }
	//
	///** ワールド座標上の交差点 */
	//const Vector3& point() const { return m_point; }

	/** 指定した向きの平面との交差判定を行います。 */
	LN_METHOD()
	RaycastResult* intersectPlane(float normalX, float normalY, float normalZ) const { return intersectPlane(Plane(Vector3(normalX, normalY, normalZ), 0)); }

	RaycastResult* intersectPlane(const Plane& plane) const;

LN_CONSTRUCT_ACCESS:
	Raycaster();
	virtual ~Raycaster() = default;
	Result init();

private:
	Ray m_ray;
	//Vector3 m_point;
	//bool m_isHit;
	Ref<RaycastResult> m_result;
};

/** レイキャスティングの結果を表すクラスです。 */
LN_CLASS()
class RaycastResult
	: public Object
{
	LN_OBJECT;
public:
	/** ワールド座標上の交差点 */
	LN_METHOD(Property)
	const Vector3& point() const { return m_point; }

LN_CONSTRUCT_ACCESS:
	RaycastResult();
	virtual ~RaycastResult() = default;
	Result init();

private:
	Vector3 m_point;
	//bool m_isHit;



	friend class Raycaster;
};

} // namespace ln

