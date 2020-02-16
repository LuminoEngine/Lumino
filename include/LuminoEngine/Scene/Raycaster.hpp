
#pragma once
#include "Common.hpp"

namespace ln {

/** レイキャスティングを支援するためのクラスです。 */
class Raycaster
	: public Object
{
public:
	// 0.0 ~ view actual size
	// インスタンス作成と setFrom2DView() のユーティリティ。
	// グローバルなインスタンスを共有する。
	static Raycaster* fromScreen(Camera* camera, const Point& point);
	static Raycaster* fromScreen(const Point& point);

	// -1.0~1.0
	//static Raycaster* fromViewport(Camera* camera, const Vector2& coords);

	void setFrom2DView(Camera* camera, const Point& point);
	//void setFromViewport(Camera* camera, const Point& point);

	//bool isHit() const { return m_isHit; }
	//
	///** ワールド座標上の交差点 */
	//const Vector3& point() const { return m_point; }

	RaycastResult* intersectPlane(float normalX, float normalY, float normalZ) const { return intersectPlane(Plane(Vector3(normalX, normalY, normalZ), 0)); }
	RaycastResult* intersectPlane(const Plane& plane) const;

LN_CONSTRUCT_ACCESS:
	Raycaster();
	virtual ~Raycaster() = default;
	bool init();

private:
	Ray m_ray;
	//Vector3 m_point;
	//bool m_isHit;
	Ref<RaycastResult> m_result;
};

/** レイキャスティングを支援するためのクラスです。 */
class RaycastResult
	: public Object
{
public:
	/** ワールド座標上の交差点 */
	const Vector3& point() const { return m_point; }

LN_CONSTRUCT_ACCESS:
	RaycastResult();
	virtual ~RaycastResult() = default;
	bool init();

private:
	Vector3 m_point;
	//bool m_isHit;

	friend class Raycaster;
};

} // namespace ln

