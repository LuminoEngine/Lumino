
#pragma once
#include "../World.h"
#include "../Graphics/Viewport.h"
#include "../Rendering/Rendering.h"
#include "../UI/UIViewport.h"
#include "SceneNode.h"
#include "WorldObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace tr { class GizmoModel; }
class WorldRenderView;
namespace detail { class ClusteredShadingSceneRenderer; }

/** カメラの投影方法 */
enum class ProjectionMode
{
	Perspective,	/**< 透視投影 */
	Orthographic,	/**< 平行投影 */
};

/**
	@brief
*/
class CameraComponent
	: public SceneNode
{
public:
	static CameraComponent* getMain3DCamera();

	static CameraComponent* getMain2DCamera();


public:

	// TODO: tansform に任せたので必要ない
	void setLookAt(const Vector3& position) { m_lookAt = position; }
	const Vector3& getLookAt() const { return m_lookAt; }

	void setUpDirection(const Vector3& up) { m_upDirection = up; }
	const Vector3& getUpDirection() const { return m_upDirection; }

	

	/// Y 方向視野角の設定
	void setFovY(float fov_y) { m_fovY = fov_y; }

	/// Y 方向視野角の取得
	float getFovY() const { return m_fovY; }

	/// 最も近いビュープレーン位置の設定 (0.0 以下にしないこと)
	void setNearClip(float nearClip) { m_nearClip = nearClip; }

	/// 最も近いビュープレーン位置の取得
	float getNearClip() const { return m_nearClip; }

	/// 最も遠いビュープレーン位置の設定
	void setFarClip(float farClip) { m_farClip = farClip; }

	/// 最も遠いビュープレーン位置の取得
	float getFarClip() const { return m_farClip; }

	const ViewFrustum& getViewFrustum() const { return m_viewFrustum; }


	void setZSortDistanceBase(ZSortDistanceBase type) { m_zSortDistanceBase = type; }
	ZSortDistanceBase getZSortDistanceBase() const { return m_zSortDistanceBase; }

	void setCameraBehavior(CameraBehavior* behavior);
	CameraBehavior* getCameraBehavior() const { return m_cameraBehavior; }

	// 3D→2D
	Vector3 worldToViewportPoint(const Vector3& position) const;
	// 2D→3D
	Vector3 viewportToWorldPoint(const Vector3& position) const;

	void setProjectionMode(ProjectionMode value) { m_projectionMode = value; }

	
	void setOrthographicSize(float value) { m_orthographicSize = value; }

public:	// internal

	/// 各行列を更新する (SceneNode::updateFrameHierarchy() の後で呼び出すこと)
	void updateMatrices(const Size& viewSize);

	// 向きの取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	const Vector4& getDirectionInternal() const { return m_direction; }

	// 行列の取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	const Matrix& getViewMatrix() const { return m_viewMatrix; }
	const Matrix& getProjectionMatrix() const { return m_projMatrix; }
	const Matrix& getViewProjectionMatrix() const { return m_viewProjMatrix; }

protected:

	// Component interface
	virtual void onUpdate(float deltaSceonds) override;
	virtual void onUIEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	CameraComponent();
	virtual ~CameraComponent();
	void initialize(CameraWorld proj);
	void setCameraDirection(CameraDirection mode) { m_directionMode = mode; }
	CameraWorld getCameraWorld() const { return m_cameraWorld; }

	void setReflectionPlane(const Plane& plane) { m_reflectionPlane = plane; }

	WorldRenderView*	m_ownerLayer;

	CameraWorld	m_cameraWorld;

private:
	CameraDirection		m_directionMode;
	ProjectionMode		m_projectionMode;
	Vector3				m_lookAt;
	Vector3				m_upDirection;
	float				m_fovY;
	float				m_nearClip;
	float				m_farClip;
	float				m_orthographicSize;	// 縦方向のサイズ。横はアスペクト比から求める
	ZSortDistanceBase	m_zSortDistanceBase;
	CameraBehavior*		m_cameraBehavior;

	Matrix				m_viewMatrix;		///< ビュー行列
	Matrix				m_projMatrix;		///< プロジェクション行列
	Matrix				m_viewProjMatrix;	///< ビュー行列とプロジェクション行列の積
	Vector4				m_direction;		///< 向き
	ViewFrustum			m_viewFrustum;		// 視錐台カリング用 (3D,2D共用)

	Matrix				m_viewProjMatrixI;

	Plane				m_reflectionPlane;
};

/**
	@brief
*/
class WorldRenderView
	: public WorldRenderViewBase
{
public:
	void setDebugDrawFlags(WorldDebugDrawFlags flags);

	virtual void renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	WorldRenderView();
	virtual ~WorldRenderView();
	void initialize(World* targetWorld, CameraComponent* hostingCamera);

	detail::ClusteredShadingSceneRenderer* getClusteredShadingSceneRenderer() const { return m_clusteredShadingSceneRenderer; }

private:
	World*									m_targetWorld;
	Ref<CameraComponent>					m_hostingCamera;
	Ref<detail::SceneRenderer>				m_internalRenderer;
	detail::ClusteredShadingSceneRenderer*	m_clusteredShadingSceneRenderer;
	WorldDebugDrawFlags						m_debugDrawFlags;
};

class OffscreenWorldRenderView
	: public WorldRenderView
{
public:
	void setRenderTarget(RenderTargetTexture* renderTarget);
	RenderTargetTexture* getRenderTarget() const;
	void render();

LN_INTERNAL_ACCESS :
	OffscreenWorldRenderView();
	virtual ~OffscreenWorldRenderView();
	void initialize(World* targetWorld, CameraComponent* hostingCamera);

private:
	Ref<RenderTargetTexture>	m_renderTarget;
	Ref<DepthBuffer>			m_depthBuffer;
};

/**
	@brief
*/
class CameraBehavior
	: public RefObject
{
public:
	CameraBehavior();
	virtual ~CameraBehavior();

	void setTargetCamera(CameraComponent* camera) { m_targetCamera = camera; }
	CameraComponent* getTargetCamera() const { return m_targetCamera; }

	virtual bool injectMouseMove(int x, int y) = 0;
	virtual bool injectMouseButtonDown(MouseButtons button, int x, int y) = 0;
	virtual bool injectMouseButtonUp(MouseButtons button, int x, int y) = 0;
	virtual bool injectMouseWheel(int delta) = 0;

private:
	CameraComponent* m_targetCamera;
};

///**
//	@brief
//*/
//class CylinderMouseMoveCameraBehavior
//	: public CameraBehavior
//{
//public:
//	CylinderMouseMoveCameraBehavior();
//	virtual ~CylinderMouseMoveCameraBehavior();
//
//	virtual bool injectMouseMove(int x, int y) override;
//	virtual bool injectMouseButtonDown(MouseButtons button, int x, int y) override;
//	virtual bool injectMouseButtonUp(MouseButtons button, int x, int y) override;
//	virtual bool injectMouseWheel(int delta) override;
//
//private:
//	PointI	m_prevPos;
//	bool	m_RDrag;
//	bool	m_MDrag;
//};


/**
	@brief
*/
class CameraMouseMoveBehavior
	: public Behavior
{
	LN_OBJECT;
LN_CONSTRUCT_ACCESS:
	CameraMouseMoveBehavior();
	virtual ~CameraMouseMoveBehavior();
	void initialize();

protected:
	virtual void onAttached() override;
	virtual void onDetaching() override;
	virtual void onAttachedWorld(World* world) override;
	virtual void onDetachedWorld(World* world) override;

private:
	void World_onUIEvent(UIEventArgs* e);
	bool injectMouseMove(int x, int y);
	bool injectMouseButtonDown(MouseButtons button, int x, int y);
	bool injectMouseButtonUp(MouseButtons button, int x, int y);
	bool injectMouseWheel(int delta);

	Camera*			m_targetCamera;
	EventConnection	m_onUIEventConnection;
	PointI			m_prevPos;
	bool			m_RDrag;
	bool			m_MDrag;
};


/**
	@brief
*/
class Camera
	: public WorldObject
{
	LN_OBJECT;
public:
	CameraComponent* getCameraComponent() const;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void initialize(CameraWorld proj, bool defcmp);

LN_INTERNAL_ACCESS:
	void setCameraComponent(CameraComponent* component);

private:
	Ref<CameraComponent>	m_component;
};











/** 正射投影カメラのコンポーネント。 */
class OrthographicCameraComponent
	: public CameraComponent
{
	LN_OBJECT;
public:

LN_INTERNAL_ACCESS:
	OrthographicCameraComponent();
	virtual ~OrthographicCameraComponent();
	void initialize();
};

/** 正射投影カメラのオブジェクト。 */
class OrthographicCamera
	: public Camera
{
	LN_OBJECT;
public:

	/** 既定の設定で正射投影カメラを作成します。 */
	static Ref<OrthographicCamera> create();

	/** 正射投影の高さを指定して正射投影カメラを作成します。 */
	static Ref<OrthographicCamera> create(float orthoSize);

public:
	
	/** 
		正射投影の高さを設定します。
	
		正射影幅はビューの比率に基づいて自動的に計算されます。
		例えば、2D シーンを dot by dot で描画する場合、この値を縦方向のピクセル数と一致させます。
	*/
	void setOrthographicSize(float value) { m_component->setOrthographicSize(value); }

LN_CONSTRUCT_ACCESS:
	OrthographicCamera();
	virtual ~OrthographicCamera();

	/** 既定の設定で正射投影カメラを作成します。 */
	void initialize();

	/** 正射投影の高さを指定して正射投影カメラを作成します。 */
	void initialize(float orthoSize);

private:
	Ref<OrthographicCameraComponent>	m_component;
};


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
