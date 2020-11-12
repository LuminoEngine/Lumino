
#pragma once
#include <LuminoEngine/Visual/CameraComponent.hpp>
#include "WorldObject.hpp"

namespace ln {
class WorldRenderView;
class CameraComponent;

/**
 * カメラのクラスです。カメラは 3D シーンを描画する際の視点となります。
 */
LN_CLASS()
class Camera
	: public WorldObject
{
	LN_OBJECT;
public:
    /** 既定の設定で Camera を作成します。 */
    static Ref<Camera> create();

    /** Y 方向視野角の設定 */
    void setFov(float value) { m_component->setFovY(value); }

    /** Y 方向視野角の取得 */
    float fov() const { return m_component->getFovY(); }

    /** 最も近いビュープレーン位置を設定します。(0 は無効値です) */
    void setNearClip(float value) { m_component->setNearClip(value); }

    /** 最も近いビュープレーン位置を取得します。 */
    float getNearClip() const { return m_component->getNearClip(); }

    /** 最も遠いビュープレーン位置を設定します。 */
    void setFarClip(float value) { m_component->setFarClip(value); }

    /** 最も遠いビュープレーン位置を取得します。 */
    float getFarClip() const { return m_component->getFarClip(); }


    /** カメラの投影モードを設定します。(default: Perspective) */
    void setProjectionMode(ProjectionMode value) { m_component->setProjectionMode(value); }

    /** カメラの投影モードを取得します。 */
    ProjectionMode projectionMode() const { return m_component->projectionMode(); }

    /** 平行投影モード時の、ビューサイズに対するワールド空間内の距離を設定します。(default: (16.0, 12.0)) */
    void setOrthographicSize(const Size& size) { m_component->setOrthographicSize(size); }

    /** 平行投影モード時の、ビューサイズに対するワールド空間内の距離を設定します。(default: (16.0, 12.0)) */
    void setOrthographicSize(float width, float height) { setOrthographicSize(Size(width, height)); }

    /** 平行投影モード時の、ビューの縦幅に対するワールド空間内の距離を設定します。(default: (16.0, 12.0)) */
    void setOrthographicSize(float height) { setOrthographicSize(Size(0.0f, height)); }
    
    /** 平行投影モード時の、ビューサイズに対するワールド空間内の距離を取得します。 */
    const Size& orthographicSize() const { return m_component->orthographicSize(); }



    /** ビュー行列を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& viewMatrix() const;

    /** プロジェクション行列を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& projectionMatrix() const;

    /** ビュー行列とプロジェクション行列の積を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& viewProjectionMatrix() const;

    // 3D→2D(unit:dp)
    Vector3 worldToScreenPoint(const Vector3& position) const;
    // 2D(unit:dp)→3D
    Vector3 screenToWorldPoint(const Vector3& position) const;
	Ray screenToWorldRay(const Vector2& position) const;


	SceneClearMode clearMode() const;
	void setClearMode(SceneClearMode value);

	const Color& backgroundColor() const;
	void setBackgroundColor(const Color& value);

	CameraComponent* cameraComponent() const;

    WorldRenderView* renderView() const;

protected:
    // WorldObject interface
    virtual void onUpdate(float elapsedSeconds) override;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void init(/*CameraWorld proj, bool defcmp*/);

//LN_INTERNAL_ACCESS:
//	void setCameraComponent(CameraComponent* component);

private:
	Ref<CameraComponent> m_component;
    //WorldRenderView* m_ownerRenderView;

    //friend class WorldRenderView;
};

} // namespace ln
