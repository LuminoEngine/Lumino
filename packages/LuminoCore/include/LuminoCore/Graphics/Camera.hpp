#pragma once
#include <LuminoBase/math/Math.hpp>

namespace ln {

/** 
 * 3Dレンダリング用カメラ。ビュー行列とプロジェクション行列を保持します。
 */
class Camera {
public:
    Camera();

    /** ビュー行列を直接設定します。 */
    void setViewMatrix(const Matrix4x4& m) { m_viewMatrix = m; }

    /** プロジェクション行列を直接設定します。 */
    void setProjectionMatrix(const Matrix4x4& m) { m_projMatrix = m; }

    /** Helper: 視点の位置/注視点/上方向からビューマトリックスを作成し、それを設定します。 */
    void setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3::unitY());

    /** Helper: ターゲットの周りを回転してビュー行列を作成します。 */
    void setOrbit(const Vector3& target, float distance, float yaw, float pitch);

    /** Helper: パースペクティブ投影行列を作成し、それを設定します。 */
    void setPerspective(float fovY, float aspect, float nearClip, float farClip);

    /** Helper: 正射影投影行列を作成し、それを設定します。 */
    void setOrthographic(float width, float height, float nearClip, float farClip);

    const Matrix4x4& viewMatrix() const { return m_viewMatrix; }
    const Matrix4x4& projectionMatrix() const { return m_projMatrix; }
    Matrix4x4 viewProjectionMatrix() const;

private:
    Matrix4x4 m_viewMatrix = Matrix4x4::identity();
    Matrix4x4 m_projMatrix = Matrix4x4::identity();
};

} // namespace ln
