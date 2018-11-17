
#pragma once
#include <LuminoEngine/Visual/CameraComponent.hpp>
#include "WorldObject.hpp"

namespace ln {
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
    void setFovY(float value) { m_component->setFovY(value); }

    /** Y 方向視野角の取得 */
    float fovY() const { return m_component->getFovY(); }

    /** 最も近いビュープレーン位置を設定します。(0 は無効値です) */
    void setNearClip(float value) { m_component->setNearClip(value); }

    /** 最も近いビュープレーン位置を取得します。 */
    float getNearClip() const { return m_component->getNearClip(); }

    /** 最も遠いビュープレーン位置を設定します。 */
    void setFarClip(float value) { m_component->setFarClip(value); }

    /** 最も遠いビュープレーン位置を取得します。 */
    float getFarClip() const { return m_component->getFarClip(); }


	CameraComponent* cameraComponent() const;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void initialize(/*CameraWorld proj, bool defcmp*/);

//LN_INTERNAL_ACCESS:
//	void setCameraComponent(CameraComponent* component);

private:
	Ref<CameraComponent>	m_component;
};


} // namespace ln
