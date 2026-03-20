#include "Internal.hpp"
#include <LuminoFramework/Scene/Camera.hpp>
#include <LuminoFramework/Scene/Raycaster.hpp>
#include "../Engine/EngineManager.hpp"
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// Raycaster
/*

レイキャストは、C++ や C# だと次のように書けると便利。

```
HitResult result;
if (world.raycast(ray, &result)) {
}
```

しかし、Ruby など参照渡しができない言語向けにはちょっと厳しい。
あらかじめ new 必須。しかしそれでも、result が書き換わるイメージは以下のコードからはぱっと見わかりづらい。(C++ なら &, C# なら out とか付けられるけど)

```
result = HitResult.new
if world.raycast(ray, result)
end
```

レイキャストの要件として、単一 object を返すのか、複数 object を返すのかといった制御も欲しい。
複数 object 対応のためには、結果に Collection を含める必要がある。
こうなってくると、C++ で構造体をローカル変数で定義して使うのも難しくなる。（Binding 公開のため）



一方で単純な接地判定のために、レイ渡して bool 返すユーティリティはほしい。
こっちは詳細情報なしパターンとしてサポートしたほうがよさそう。

Raycaster を内部で作って返すパターンもありかな。UIEventArgs みたいに Pool する。

```
if (auto result = world.raycast(ray)) {
	for (auto& obj : result.objects) {
		...
	}
}
```

```
if (auto result = Raycaster::intersectFromCamera(Engine::camera(), Mouse::position(), Engine::world())) {
	for (auto& obj : result.objects) {
		...
	}
}
```

```
if (auto result = Raycaster::intersectFrom2DView(Mouse::position(), Vector3::UnitY)) {
	for (auto& obj : result.objects) {
		...
	}
}
```

```
if (auto result = Raycaster::intersectFrom2DView(Mouse::position(), Vector3::UnitY)) {
	for (auto& obj : result.objects) {
		...
	}
}
```

インスタンス生成と intersect は分けたほうがいいかもしれない。
生成は
- レイを直接指定
- カメラと2Dビュー位置を指定
- デフォルトのカメラを使って、2Dビュー位置だけ指定
などがある。
判定については
- ワールド内全オブジェクトと判定
- 指定したオブジェクトと判定
- Plane と判定
などがある。
ユーティリティ組むのはいいけどほどほどにしないと組み合わせ爆発する。



 */
	
LN_OBJECT_IMPLEMENT(Raycaster, Object) {}
	
Raycaster* Raycaster::fromScreen(Camera* camera, const Point& point)
{
	auto ptr = detail::EngineDomain::sceneManager()->raycasterCache();
	ptr->setFrom2DView(camera, point);
	return ptr;
}

Raycaster* Raycaster::fromScreen(const Point& point)
{
	return fromScreen(detail::EngineDomain::engineManager()->mainCamera(), point);
}

//Raycaster* Raycaster::fromViewport(Camera* camera, const Vector2& coords)
//{
//	auto ptr = detail::EngineDomain::sceneManager()->raycasterCache();
//	ptr->setFromViewport(camera, coords);
//	return ptr;
//}
//
Raycaster::Raycaster()
	: m_result(makeObject_deprecated<RaycastResult>())	// intersect の度に object 作って返すのが自然だと思うけど、Binding 作るときに pool が大変なのでこの仕様で行ってみる
{
}

Result<> Raycaster::init() {
	return Object::init();
}

void Raycaster::setFrom2DView(Camera* camera, const Point& point)
{
	m_ray = camera->screenToWorldRay(point);
}

//void Raycaster::setFromViewport(Camera* camera, const Point& point)
//{
//	LN_NOTIMPLEMENTED();
//}

RaycastResult* Raycaster::intersectPlane(const Plane& plane) const
{
	Vector3 pos;
	if (plane.intersects(m_ray, &pos)) {
		m_result->m_point = pos;
		return m_result;
	}
	else {
		return nullptr;
	}
}

//==============================================================================
// RaycastResult

LN_OBJECT_IMPLEMENT(RaycastResult, Object) {}

RaycastResult::RaycastResult()
{
}

Result<> RaycastResult::init() {
	return Object::init();
}

} // namespace ln
