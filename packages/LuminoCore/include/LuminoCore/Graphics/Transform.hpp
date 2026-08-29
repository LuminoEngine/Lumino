#pragma once
#include <LuminoBase/math/Math.hpp>

namespace ln {

/** TRS トランスフォーム: 位置、回転 (クォータニオン)、スケール。 */
struct Transform {
    Vector3    position = Vector3::zero();
    Quaternion rotation = Quaternion::identity();
    Vector3    scale    = Vector3::one();

    /** TRS を合成してモデル (ワールド) 行列にする: T * R * S。 */
    Matrix4x4 matrix() const {
        return Matrix4x4::translate(position)
             * Matrix4x4::fromQuaternion(rotation)
             * Matrix4x4::scale(scale);
    }

    /**
     * 法線変換用の、左上 3x3 の逆転置行列。
     * 均一スケールでは回転行列と等しい。非均一スケールでは
     * 完全な逆転置行列が必要になる。
     */
    Matrix4x4 normalMatrix() const {
        return matrix().inversed().transposed();
    }

    static Transform identity() { return {}; }
};

} // namespace ln
