// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include "Common.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace ln {

/** ジオメトリの姿勢を位置、回転、スケールで表します。 */
struct LN_API AttitudeTransform
{
public:
    /** 拡大要素 */
    Vector3 scale;

    /** 回転要素 */
    Quaternion rotation;

    /** 位置要素 */
    Vector3 translation;

    /** (Vector3::One, Quaternion::Identity, Vector3::Zero) */
    static const AttitudeTransform Identity;

public:
    /** 各要素を 拡縮1.0、単位クォータニオン、位置(0, 0) でインスタンスを初期化します。 */
    AttitudeTransform();

    /** 各要素を指定してインスタンスを初期化します。 */
    AttitudeTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
};

} // namespace ln
