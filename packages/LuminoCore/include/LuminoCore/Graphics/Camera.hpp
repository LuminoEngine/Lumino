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

    /**
     * Helper: 2D 用正射影投影行列を作成し、それを設定します。
     * pivot (0..1) で原点 (スクリーン座標 (0,0)) の位置を指定します。
     * pivot=(0,0) で左上、(0.5,0.5) で画面中央、(1,1) で右下が原点になります。
     * 既定の pivot=(0,0) では (0,0) が左上、(width, height) が右下に対応します。
     * ビュー行列は単位行列に設定されます。
     * このカメラで描画するスプライトは、左上原点、Y軸下向きのレイアウトで自動生成されます。
     */
    void setOrthographic2D(float width, float height, float nearClip, float farClip,
                           float pivotX = 0.0f, float pivotY = 0.0f);

    /** このカメラが 2D モード (setOrthographic2D 使用) かどうか。 */
    bool is2D() const { return m_is2D; }

    /** このカメラの 2D モードフラグを直接設定します。 */
    void setIs2D(bool value) { m_is2D = value; }

    const Matrix4x4& viewMatrix() const { return m_viewMatrix; }
    const Matrix4x4& projectionMatrix() const { return m_projMatrix; }
    Matrix4x4 viewProjectionMatrix() const;

private:
    Matrix4x4 m_viewMatrix = Matrix4x4::identity();
    Matrix4x4 m_projMatrix = Matrix4x4::identity();
    bool      m_is2D       = false;
};

} // namespace ln
