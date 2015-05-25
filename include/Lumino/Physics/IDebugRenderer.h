
#pragma once
#include <Lumino/Math/Vector3.h>

namespace Lumino
{
namespace Physics
{

/**
	@brief		デバッグ用の形状描画インターフェイスです。
*/
class IDebugRenderer
{
public:

	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;

};

} // namespace Physics
} // namespace Lumino
