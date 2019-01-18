
#pragma once
#include <LuminoEngine/Visual/CameraComponent.hpp>
#include "WorldObject.hpp"

namespace ln {
class WorldRenderView;
class CameraComponent;

/**
	@brief
*/
class Camera
	: public WorldObject
{
	//LN_OBJECT;
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


    /** ビュー行列を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& viewMatrix() const;

    /** プロジェクション行列を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& projectionMatrix() const;

    /** ビュー行列とプロジェクション行列の積を取得します。カメラの姿勢に同期するように別のオブジェクトの更新を行う場合、onPostUpdate() でこの行列を取得します。onUpdate() 時点では最新の行列が返るとは限りません。 */
    const Matrix& viewProjectionMatrix() const;

    // 3D→2D
    Vector3 worldToViewportPoint(const Vector3& position) const;
    // 2D→3D
    Vector3 viewportToWorldPoint(const Vector3& position) const;


	RenderViewClearMode clearMode() const;
	void setClearMode(RenderViewClearMode value);

	const Color& backgroundColor() const;
	void setBackgroundColor(const Color& value);

	CameraComponent* cameraComponent() const;

    WorldRenderView* renderView() const { return m_ownerRenderView; }

protected:
    // WorldObject interface
    virtual void onUpdate(float elapsedSeconds) override;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void initialize(/*CameraWorld proj, bool defcmp*/);

//LN_INTERNAL_ACCESS:
//	void setCameraComponent(CameraComponent* component);

private:
	Ref<CameraComponent> m_component;
    WorldRenderView* m_ownerRenderView;

    friend class WorldRenderView;
};

} // namespace ln
