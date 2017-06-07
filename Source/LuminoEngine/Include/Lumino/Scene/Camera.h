
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
	static CameraComponent* GetMain3DCamera();

	static CameraComponent* GetMain2DCamera();


public:

	void SetLookAt(const Vector3& position) { m_lookAt = position; }
	const Vector3& GetLookAt() const { return m_lookAt; }

	void SetUpDirection(const Vector3& up) { m_upDirection = up; }
	const Vector3& GetUpDirection() const { return m_upDirection; }

	

	/// Y 方向視野角の設定
	void SetFovY(float fov_y) { m_fovY = fov_y; }

	/// Y 方向視野角の取得
	float GetFovY() const { return m_fovY; }

	/// 最も近いビュープレーン位置の設定 (0.0 以下にしないこと)
	void SetNearClip(float nearClip) { m_nearClip = nearClip; }

	/// 最も近いビュープレーン位置の取得
	float GetNearClip() const { return m_nearClip; }

	/// 最も遠いビュープレーン位置の設定
	void SetFarClip(float farClip) { m_farClip = farClip; }

	/// 最も遠いビュープレーン位置の取得
	float GetFarClip() const { return m_farClip; }

	const ViewFrustum& GetViewFrustum() const { return m_viewFrustum; }


	void SetZSortDistanceBase(ZSortDistanceBase type) { m_zSortDistanceBase = type; }
	ZSortDistanceBase GetZSortDistanceBase() const { return m_zSortDistanceBase; }

	void SetCameraBehavior(CameraBehavior* behavior);
	CameraBehavior* GetCameraBehavior() const { return m_cameraBehavior; }

	// 3D→2D
	Vector3 WorldToViewportPoint(const Vector3& position) const;
	// 2D→3D
	Vector3 ViewportToWorldPoint(const Vector3& position) const;

public:	// internal

	/// 各行列を更新する (SceneNode::UpdateFrameHierarchy() の後で呼び出すこと)
	void UpdateMatrices(const Size& viewSize);

	// 向きの取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	const Vector4& GetDirectionInternal() const { return m_direction; }

	// 行列の取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix& GetProjectionMatrix() const { return m_projMatrix; }
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjMatrix; }
	const Matrix& GetViewMatrixI() const { return m_viewMatrixI; }
	const Matrix& GetProjectionMatrixI() const { return m_projMatrixI; }
	const Matrix& GetViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	const Matrix& GetViewMatrixT() const { return m_viewMatrixT; }
	const Matrix& GetProjectionMatrixT() const { return m_projMatrixT; }
	const Matrix& GetViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	const Matrix& GetViewMatrixIT() const { return m_viewMatrixIT; }
	const Matrix& GetProjectionMatrixIT() const { return m_projMatrixIT; }
	const Matrix& GetViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

	//void DoMouseMoveR(float dx, float dy, float width, float height);
	//void DoMouseMoveM(float offsetX, float offsetY);
	//void DoMouseWheel(int pos);

protected:

	// Component interface
	virtual void OnUpdate() override;
	virtual void OnUIEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	CameraComponent();
	virtual ~CameraComponent();
	void Initialize(CameraProjection proj);
	CameraProjection GetProjectionMode() const { return m_projectionMode; }

	void SetReflectionPlane(const Plane& plane) { m_reflectionPlane = plane; }

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
	Matrix				m_viewMatrixI;		///< ビュー行列 (Inverse)
	Matrix				m_projMatrixI;		///< プロジェクション行列 (Inverse)
	Matrix				m_viewProjMatrixI;	///< ビュー行列とプロジェクション行列の積 (Inverse)
	Matrix				m_viewMatrixT;		///< ビュー行列 (Transpose)
	Matrix				m_projMatrixT;		///< プロジェクション行列 (Transpose)
	Matrix				m_viewProjMatrixT;	///< ビュー行列とプロジェクション行列の積 (Transpose)
	Matrix				m_viewMatrixIT;		///< ビュー行列 (Inverse * Transpose)
	Matrix				m_projMatrixIT;		///< プロジェクション行列 (Inverse * Transpose)
	Matrix				m_viewProjMatrixIT;	///< ビュー行列とプロジェクション行列の積 (Inverse * Transpose)

	Plane				m_reflectionPlane;
};

/**
	@brief
*/
class CameraViewportLayer2
	: public UIViewportLayer
{
public:
	void SetDebugDrawFlags(WorldDebugDrawFlags flags);

	virtual void Render() override;
	virtual void ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;

protected:
	virtual void OnRoutedEvent(UIEventArgs* e) override;

LN_INTERNAL_ACCESS:
	CameraViewportLayer2();
	virtual ~CameraViewportLayer2();
	void Initialize(World* targetWorld, CameraComponent* hostingCamera);
	const Size& GetViewSize() const;

private:
	World*								m_targetWorld;
	RefPtr<CameraComponent>				m_hostingCamera;
	RefPtr<detail::SceneRenderer>		m_internalRenderer;
	RefPtr<RenderView>					m_mainRenderView;
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

	void SetTargetCamera(CameraComponent* camera) { m_targetCamera = camera; }
	CameraComponent* GetTargetCamera() const { return m_targetCamera; }

	virtual bool InjectMouseMove(int x, int y) = 0;
	virtual bool InjectMouseButtonDown(MouseButtons button, int x, int y) = 0;
	virtual bool InjectMouseButtonUp(MouseButtons button, int x, int y) = 0;
	virtual bool InjectMouseWheel(int delta) = 0;

private:
	CameraComponent* m_targetCamera;
};

/**
	@brief
*/
class CylinderMouseMoveCameraBehavior
	: public CameraBehavior
{
public:
	CylinderMouseMoveCameraBehavior();
	virtual ~CylinderMouseMoveCameraBehavior();

	virtual bool InjectMouseMove(int x, int y) override;
	virtual bool InjectMouseButtonDown(MouseButtons button, int x, int y) override;
	virtual bool InjectMouseButtonUp(MouseButtons button, int x, int y) override;
	virtual bool InjectMouseWheel(int delta) override;

private:
	PointI	m_prevPos;
	bool	m_RDrag;
	bool	m_MDrag;
};



/**
	@brief
*/
class Camera
	: public WorldObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	CameraComponent* GetCameraComponent() const;

LN_CONSTRUCT_ACCESS:
	Camera();
	virtual ~Camera();
	void Initialize(CameraProjection proj);

LN_INTERNAL_ACCESS:

private:
	RefPtr<CameraComponent>	m_component;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
