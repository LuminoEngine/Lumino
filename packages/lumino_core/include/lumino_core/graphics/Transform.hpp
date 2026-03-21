#pragma once
#include <lumino_base/Math.hpp>

namespace ln {

/// TRS transform: position, rotation (quaternion), scale.
struct Transform {
    Vector3    position = Vector3::zero();
    Quaternion rotation = Quaternion::identity();
    Vector3    scale    = Vector3::one();

    /// Compose TRS into a model (world) matrix: T * R * S.
    Matrix4x4 matrix() const {
        return Matrix4x4::translate(position)
             * Matrix4x4::fromQuaternion(rotation)
             * Matrix4x4::scale(scale);
    }

    /// Inverse-transpose of the upper-left 3x3 for normal transformation.
    /// For uniform scale this equals the rotation matrix; for non-uniform
    /// scale the full inverse-transpose is needed.
    Matrix4x4 normalMatrix() const {
        return matrix().inversed().transposed();
    }

    static Transform identity() { return {}; }
};

} // namespace ln
