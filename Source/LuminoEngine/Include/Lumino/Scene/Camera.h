
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
class CameraViewportLayer2;


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

public:	// internal

	/// 各行列を更新する (SceneNode::updateFrameHierarchy() の後で呼び出すこと)
	void updateMatrices(const Size& viewSize);

	// 向きの取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	const Vector4& getDirectionInternal() const { return m_direction; }

	// 行列の取得 (シェーダ設定用。updateMatrices() の後で呼び出すこと)
	const Matrix& getViewMatrix() const { return m_viewMatrix; }
	const Matrix& getProjectionMatrix() const { return m_projMatrix; }
	const Matrix& getViewProjectionMatrix() const { return m_viewProjMatrix; }
	const Matrix& getViewMatrixI() const { return m_viewMatrixI; }
	const Matrix& getProjectionMatrixI() const { return m_projMatrixI; }
	const Matrix& getViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	const Matrix& getViewMatrixT() const { return m_viewMatrixT; }
	const Matrix& getProjectionMatrixT() const { return m_projMatrixT; }
	const Matrix& getViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	const Matrix& getViewMatrixIT() const { return m_viewMatrixIT; }
	const Matrix& getProjectionMatrixIT() const { return m_projMatrixIT; }
	const Matrix& getViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

	//void DoMouseMoveR(float dx, float dy, float width, float height);
	//void DoMouseMoveM(float offsetX, float offsetY);
	//void DoMouseWheel(int pos);

protected:

	// Component interface
	virtual void onUpdate() override;
	virtual void onUIEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	CameraComponent();
	virtual ~CameraComponent();
	void initialize(CameraProjection proj);
	CameraProjection getProjectionMode() const { return m_projectionMode; }

	void setReflectionPlane(const Plane& plane) { m_reflectionPlane = plane; }

	CameraViewportLayer2*	m_ownerLayer;

	CameraProjection	m_projectionMode;

private:
	CameraDirection		m_directionMode;
	Vector3				m_lookAt;
	Vector3				m_upDirection;
	float				m_fovY;
	float				m_nearClip;
	float				m_farClip;
	ZSortDistanceBase	m_zSortDistanceBase;
	CameraBehavior*		m_cameraBehavior;

	Matrix				m_viewMatrix;		///< ビュー行列
	Matrix				m_projMatrix;		///< プロジェクション行列
	Matrix				m_viewProjMatrix;	///< ビュー行列とプロジェクション行列の積
	Vector4				m_direction;		///< 向き
	ViewFrustum			m_viewFrustum;		// 視錐台カリング用 (3D,2D共用)

	// 以下はシェーダ変数への設定用。ライトは個々のノードに比べて参照される回数が多いので
	// 必要になるたびに計算するのではなく、あらかじめ計算しておく。
	Matrix				m_viewMatrixI;		///< ビュー行列 (inverse)
	Matrix				m_projMatrixI;		///< プロジェクション行列 (inverse)
	Matrix				m_viewProjMatrixI;	///< ビュー行列とプロジェクション行列の積 (inverse)
	Matrix				m_viewMatrixT;		///< ビュー行列 (transpose)
	Matrix				m_projMatrixT;		///< プロジェクション行列 (transpose)
	Matrix				m_viewProjMatrixT;	///< ビュー行列とプロジェクション行列の積 (transpose)
	Matrix				m_viewMatrixIT;		///< ビュー行列 (inverse * transpose)
	Matrix				m_projMatrixIT;		///< プロジェクション行列 (inverse * transpose)
	Matrix				m_viewProjMatrixIT;	///< ビュー行列とプロジェクション行列の積 (inverse * transpose)

	Plane				m_reflectionPlane;
};

/**
	@brief
*/
class CameraViewportLayer2
	: public UIViewportLayer
{
public:
	void setDebugDrawFlags(WorldDebugDrawFlags flags);

	virtual void render() override;
	virtual void executeDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	CameraViewportLayer2();
	virtual ~CameraViewportLayer2();
	void initialize(World* targetWorld, CameraComponent* hostingCamera);
	const Size& getViewSize() const;

private:
	World*								m_targetWorld;
	RefPtr<CameraComponent>				m_hostingCamera;
	RefPtr<detail::SceneRenderer>		m_internalRenderer;
	RefPtr<WorldRenderView>				m_mainRenderView;
	WorldDebugDrawFlags					m_debugDrawFlags;
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
	LN_OBJECT();
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
	LN_OBJECT();
public:
	CameraComponent* getCameraComponent() const;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void initialize(CameraProjection proj);

LN_INTERNAL_ACCESS:

private:
	RefPtr<CameraComponent>	m_component;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
