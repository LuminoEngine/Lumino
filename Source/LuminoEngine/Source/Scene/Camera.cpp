
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Mesh/GizmoModel.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/World.h>
#include <Lumino/UI/UIEvent.h>

// TODO: CameraViewportLayer::GetDefault2D() あたりを見直せばいらなくなる
#include <Lumino/UI/UIFrameWindow.h>
#include "../UI/UIManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// CameraComponent
//==============================================================================

//------------------------------------------------------------------------------
CameraComponent* CameraComponent::GetMain3DCamera()
{
	return detail::EngineDomain::GetDefaultWorld3D()->GetMainCamera()->GetCameraComponent();
}

//------------------------------------------------------------------------------
CameraComponent* CameraComponent::GetMain2DCamera()
{
	return detail::EngineDomain::GetDefaultWorld2D()->GetMainCamera()->GetCameraComponent();
}

//------------------------------------------------------------------------------
CameraComponent::CameraComponent()
	: SceneNode()
	, m_projectionMode()
	, m_directionMode(CameraDirection::LookAt)
	, m_upDirection(Vector3::UnitY)
	, m_fovY(Math::PI / 3.0f)	// Unity based.
	, m_nearClip(0.3f)			// Unity based.
	, m_farClip(1000.0f)
	, m_zSortDistanceBase(ZSortDistanceBase::CameraDistance)
	, m_cameraBehavior(nullptr)
{
	// ※ 2D では m_nearClip を0より大きくしたり、Z位置をマイナスにすると何も見えなくなるので注意。 
}

//------------------------------------------------------------------------------
CameraComponent::~CameraComponent()
{
	LN_SAFE_RELEASE(m_cameraBehavior);
	m_manager->GetAllCameraList()->Remove(this);
}

//------------------------------------------------------------------------------
void CameraComponent::Initialize(SceneGraph* owner, CameraProjection proj)
{
	SceneNode::Initialize(owner);
	m_projectionMode = proj;
	m_manager->GetAllCameraList()->Add(this);

	if (m_projectionMode == CameraProjection_2D)
	{
		m_nearClip = 0.0f;	// TODO
		m_farClip = 1000.0f;
		SetPosition(0, 0, 0);
		m_zSortDistanceBase = ZSortDistanceBase::CameraScreenDistance;
	}
	else if (m_projectionMode == CameraProjection_3D)
	{
		m_nearClip = 0.3f;
		m_farClip = 1000.0f;
		SetPosition(0, 1.0f, -10.0f);	// Unity based.
		m_zSortDistanceBase = ZSortDistanceBase::CameraDistance;
	}
}

//------------------------------------------------------------------------------
void CameraComponent::SetCameraBehavior(CameraBehavior* behavior)
{
	if (m_cameraBehavior != nullptr) {
		m_cameraBehavior->SetTargetCamera(nullptr);
	}
	LN_REFOBJ_SET(m_cameraBehavior, behavior);
	m_cameraBehavior->SetTargetCamera(this);
}

//------------------------------------------------------------------------------
Vector3 CameraComponent::WorldToViewportPoint(const Vector3& position) const
{
	const Size& size = m_ownerLayer->GetViewSize();
	return Vector3::Project(position, m_viewProjMatrix, 0.0f, 0.0f, size.width, size.height, m_nearClip, m_farClip);
}

//------------------------------------------------------------------------------
Vector3 CameraComponent::ViewportToWorldPoint(const Vector3& position) const
{
	const Size& size = m_ownerLayer->GetViewSize();
	Vector3 v;
	v.x = (((position.x - 0) / size.width) * 2.0f) - 1.0f;
	v.y = -((((position.y - 0) / size.height) * 2.0f) - 1.0f);
	v.z = (position.z - m_nearClip) / (m_farClip - m_nearClip);
	return Vector3::TransformCoord(v, m_viewProjMatrixI);
}

//------------------------------------------------------------------------------
void CameraComponent::UpdateMatrices(const Size& viewSize)
{
	const Matrix& worldMatrix = GetOwnerObject()->transform.GetWorldMatrix();

	// 2D モード
	if (m_projectionMode == CameraProjection_2D)
	{
		// 正面方向
		Vector3 direction = Vector3::TransformCoord(Vector3(0, 0, 1), worldMatrix);
		m_direction = Vector4(direction, 0.0f);

		m_viewMatrix = worldMatrix;
		m_projMatrix = Matrix::MakePerspective2DLH(viewSize.width, viewSize.height, m_nearClip, m_farClip);

		
		//Perspective2DLH(viewSize.width, viewSize.height, m_nearClip, m_farClip, &m_projMatrix);
		m_viewProjMatrix = m_viewMatrix * m_projMatrix;

		auto a1 = Vector3::TransformCoord(
			Vector3(48, 0, 10), m_viewProjMatrix);
		
		auto a2 = Vector3::TransformCoord(
			Vector3(48, 0, 100), m_viewProjMatrix);
		a2 = Vector3(48, 0, 10);
		//Matrix vp;
		//vp = Matrix::LookAtLH(Vector3(320, 240, 0), Vector3(320, 240, 1), Vector3(0, 1, 0));
		//vp *= Matrix::OrthoLH(640, 480, 0, 1000);
	}
	// 3D モード
	else
	{
		// 注視点
		Vector3 lookAt;
		if (m_directionMode == CameraDirection::LookAt &&
			worldMatrix.GetPosition() != m_lookAt)	// 位置と注視点が同じだと、Matrix::MakeLookAt で NAN になる
		{
			lookAt = m_lookAt;
		}
		else
		{
			lookAt = Vector3::TransformCoord(Vector3(0, 0, 1), worldMatrix);
		}

		// ビュー行列
		m_viewMatrix = Matrix::MakeLookAtLH(worldMatrix.GetPosition(), lookAt, m_upDirection);

		// プロジェクション行列の更新
		// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
		m_projMatrix = Matrix::MakePerspectiveFovLH(m_fovY, viewSize.width / viewSize.height, m_nearClip, m_farClip);

		m_viewProjMatrix = m_viewMatrix * m_projMatrix;

		// 正面方向
		Vector3 d = lookAt - worldMatrix.GetPosition();
		d.Normalize();
		m_direction = Vector4(d, 0.0f);
	}

	m_viewFrustum = ViewFrustum(m_viewProjMatrix);

	m_viewMatrixI = Matrix::MakeInverse(m_viewMatrix);
	m_projMatrixI = Matrix::MakeInverse(m_projMatrix);
	m_viewProjMatrixI = Matrix::MakeInverse(m_viewProjMatrix);
	m_viewMatrixT = Matrix::MakeTranspose(m_viewMatrix);
	m_projMatrixT = Matrix::MakeTranspose(m_projMatrix);
	m_viewProjMatrixT = Matrix::MakeTranspose(m_viewProjMatrix);
	m_viewMatrixIT = Matrix::MakeTranspose(m_viewMatrixI);
	m_projMatrixIT = Matrix::MakeTranspose(m_projMatrixI);
	m_viewProjMatrixIT = Matrix::MakeTranspose(m_viewProjMatrixI);
}

//------------------------------------------------------------------------------
void CameraComponent::OnOwnerSceneGraphChanged(SceneGraph* newOwner, SceneGraph* oldOwner)
{
	SceneNode::OnOwnerSceneGraphChanged(newOwner, oldOwner);

	// this が属する SceneGraph を付け替える
	if (oldOwner != nullptr) {
		oldOwner->GetAllCameraList()->Remove(this);
	}
	if (newOwner != nullptr) {
		newOwner->GetAllCameraList()->Add(this);
	}
}

//------------------------------------------------------------------------------
void CameraComponent::OnUpdate()
{
	SceneNode::OnUpdate();
}

//------------------------------------------------------------------------------
void CameraComponent::OnUIEvent(UIEventArgs* e)
{
	if (e->GetType() == UIEvents::MouseDownEvent)
	{
		if (GetCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->GetPosition(me->sender);
			GetCameraBehavior()->InjectMouseButtonDown(me->GetMouseButtons(), pos.x, pos.y);
		}
	}
	else if (e->GetType() == UIEvents::MouseUpEvent)
	{
		if (GetCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->GetPosition(me->sender);
			GetCameraBehavior()->InjectMouseButtonUp(me->GetMouseButtons(), pos.x, pos.y);
		}
	}
	else if (e->GetType() == UIEvents::MouseMoveEvent)
	{
		if (GetCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->GetPosition(me->sender);
			GetCameraBehavior()->InjectMouseMove(pos.x, pos.y);
		}
	}
	else if (e->GetType() == UIEvents::MouseMoveEvent)
	{
		if (GetCameraBehavior() != nullptr)
		{
			if (GetCameraBehavior() != nullptr) {
				auto* me = static_cast<UIMouseWheelEventArgs*>(e);
				GetCameraBehavior()->InjectMouseWheel(me->GetDelta());
			}
		}
	}
	else
	{
		Component::OnUIEvent(e);
	}
}

#if 0
//==============================================================================
// CameraViewportLayer
//==============================================================================

//------------------------------------------------------------------------------
CameraViewportLayer* CameraViewportLayer::GetDefault2D()
{
	return detail::EngineDomain::GetUIManager()->GetMainWindow()->GetDefault2DCameraViewportLayer();
}

//------------------------------------------------------------------------------
CameraViewportLayer* CameraViewportLayer::GetDefault3D()
{
	return detail::EngineDomain::GetUIManager()->GetMainWindow()->GetDefault3DCameraViewportLayer();
}

////------------------------------------------------------------------------------
//CameraViewportLayerPtr CameraViewportLayer::Create(Camera* camera)
//{
//	auto ptr = CameraViewportLayerPtr::MakeRef();
//	ptr->Initialize(SceneGraphManager::Instance, camera);
//	return ptr;
//}

//------------------------------------------------------------------------------
CameraViewportLayer::CameraViewportLayer()
	: m_targetWorld(nullptr)
	, m_hostingCamera(nullptr)
	, m_debugDrawFlags(WorldDebugDrawFlags::None)
{
}

//------------------------------------------------------------------------------
void CameraViewportLayer::Initialize(SceneGraphManager* manager, World* targetWorld, Camera* hostingCamera)
{
	m_targetWorld = targetWorld;
	m_hostingCamera = hostingCamera;
	m_hostingCamera->m_ownerLayer = this;

	if (m_hostingCamera->GetProjectionMode() == CameraProjection_3D)
	{
		auto internalRenderer = RefPtr<detail::ForwardShadingRenderer>::MakeRef();
		internalRenderer->Initialize(manager->GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}
	else
	{
		auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
		internalRenderer->Initialize(manager->GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}

	//auto pass = RefPtr<detail::RenderingPass2>::MakeRef();
	//pass->Initialize(manager->GetGraphicsManager());
	//AddRenderingPass(pass);
}

//------------------------------------------------------------------------------
CameraViewportLayer::~CameraViewportLayer()
{
	m_hostingCamera->m_ownerLayer = nullptr;
}

//------------------------------------------------------------------------------
void CameraViewportLayer::SetDebugDrawFlags(WorldDebugDrawFlags flags)
{
	m_debugDrawFlags = flags;
}

//------------------------------------------------------------------------------
tr::GizmoModel* CameraViewportLayer::CreateGizmo()
{
	m_gizmo = RefPtr<tr::GizmoModel>::MakeRef();
	m_gizmo->Initialize(detail::EngineDomain::GetGraphicsManager());
	return m_gizmo;
}

//------------------------------------------------------------------------------
void CameraViewportLayer::Render()
{
	// TODO: やめよう
	m_hostingCamera->GetOwnerSceneGraph()->GetRenderer()->SetCurrentCamera(m_hostingCamera);

	m_hostingCamera->GetOwnerSceneGraph()->GetRenderer()->Clear(ClearFlags::Depth, Color::White);

	// カメラ行列の更新
	m_hostingCamera->UpdateMatrices(GetSize());

	m_targetWorld->Render(m_hostingCamera, m_debugDrawFlags);

	if (m_gizmo != nullptr)
	{
		m_gizmo->Render(m_hostingCamera->GetOwnerSceneGraph()->GetRenderer());
	}
}

//------------------------------------------------------------------------------
void CameraViewportLayer::ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	// TODO: float
	Size targetSize((float)renderTarget->GetWidth(), (float)renderTarget->GetHeight());
	m_hostingCamera->UpdateMatrices(targetSize);

	detail::CameraInfo cameraInfo;
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(m_hostingCamera.Get());
	cameraInfo.viewPixelSize = targetSize;
	cameraInfo.viewPosition = m_hostingCamera->GetCombinedGlobalMatrix().GetPosition();
	cameraInfo.viewMatrix = m_hostingCamera->GetViewMatrix();
	cameraInfo.projMatrix = m_hostingCamera->GetProjectionMatrix();
	cameraInfo.viewProjMatrix = m_hostingCamera->GetViewProjectionMatrix();
	cameraInfo.viewFrustum = m_hostingCamera->GetViewFrustum();
	cameraInfo.zSortDistanceBase = m_hostingCamera->GetZSortDistanceBase();
	parentDrawList->RenderSubDrawList(
		m_hostingCamera->GetOwnerSceneGraph()->GetRenderer()->GetDrawElementList(),
		cameraInfo,
		m_internalRenderer,
		renderTarget,
		depthBuffer);
	parentDrawList->RenderSubDrawList(
		m_hostingCamera->GetOwnerSceneGraph()->GetDebugRenderer()->GetDrawElementList(),
		cameraInfo,
		m_internalRenderer,
		renderTarget,
		depthBuffer);
	//m_internalRenderer->Render(
	//	m_hostingCamera->GetOwnerSceneGraph()->GetRenderer()->GetDrawElementList(),
	//	cameraInfo,
	//	renderTarget,
	//	depthBuffer);
	//m_internalRenderer->Render(
	//	m_hostingCamera->GetOwnerSceneGraph()->GetDebugRenderer()->GetDrawElementList(),
	//	cameraInfo,
	//	renderTarget,
	//	depthBuffer);
	m_hostingCamera->GetOwnerSceneGraph()->GetRenderer()->EndFrame();
}

//------------------------------------------------------------------------------
void CameraViewportLayer::UpdateTransform(const Size& viewSize)
{
	ViewportLayer::UpdateTransform(viewSize);

	if (m_gizmo != nullptr)
	{
		m_gizmo->SetViewInfo(
			m_hostingCamera->GetCombinedGlobalMatrix().GetPosition(),
			m_hostingCamera->GetViewMatrix(),
			m_hostingCamera->GetProjectionMatrix(),
			SizeI(viewSize.width, viewSize.height));
	}
}

//------------------------------------------------------------------------------
bool CameraViewportLayer::OnPlatformEvent(const PlatformEventArgs& e)
{
	if (m_gizmo != nullptr)
	{
		switch (e.type)
		{
			case PlatformEventType::MouseDown:
				if (e.mouse.button == MouseButtons::Left)
				{
					if (m_gizmo->InjectMouseDown(e.mouse.x, e.mouse.y)) return true;
				}
				break;
			case PlatformEventType::MouseUp:
				if (e.mouse.button == MouseButtons::Left)
				{
					if (m_gizmo->InjectMouseUp(e.mouse.x, e.mouse.y)) return true;
				}
				break;
			case PlatformEventType::MouseMove:
				if (m_gizmo->InjectMouseMove(e.mouse.x, e.mouse.y)) return true;
				break;
			default:
				break;
		}
	}
	return false;
}
#endif

//==============================================================================
// CameraViewportLayer2
//==============================================================================

//------------------------------------------------------------------------------
CameraViewportLayer2::CameraViewportLayer2()
	: m_targetWorld(nullptr)
	, m_hostingCamera(nullptr)
	, m_debugDrawFlags(WorldDebugDrawFlags::None)
{
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::Initialize(World* targetWorld, CameraComponent* hostingCamera)
{
	m_targetWorld = targetWorld;
	m_hostingCamera = hostingCamera;
	m_hostingCamera->m_ownerLayer = this;

	if (m_hostingCamera->GetProjectionMode() == CameraProjection_3D)
	{
		auto internalRenderer = RefPtr<detail::ForwardShadingRenderer>::MakeRef();
		internalRenderer->Initialize(detail::EngineDomain::GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}
	else
	{
		auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
		internalRenderer->Initialize(detail::EngineDomain::GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}
}

//------------------------------------------------------------------------------
const Size& CameraViewportLayer2::GetViewSize() const
{
	return GetOwnerViewport()->GetViewSize();
}

//------------------------------------------------------------------------------
CameraViewportLayer2::~CameraViewportLayer2()
{
	m_hostingCamera->m_ownerLayer = nullptr;
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::SetDebugDrawFlags(WorldDebugDrawFlags flags)
{
	m_debugDrawFlags = flags;
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::Render()
{
	// TODO: やめよう
	m_targetWorld->GetRenderer()->SetCurrentCamera(m_hostingCamera);

	m_targetWorld->GetRenderer()->Clear(ClearFlags::Depth, Color::White);

	// カメラ行列の更新
	m_hostingCamera->UpdateMatrices(GetOwnerViewport()->GetViewSize());

	m_targetWorld->Render(m_hostingCamera, m_debugDrawFlags);
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	// TODO: float
	Size targetSize((float)renderTarget->GetWidth(), (float)renderTarget->GetHeight());
	m_hostingCamera->UpdateMatrices(targetSize);

	detail::CameraInfo cameraInfo;
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(m_hostingCamera.Get());
	cameraInfo.viewPixelSize = targetSize;
	cameraInfo.viewPosition = m_hostingCamera->GetTransform()->GetWorldMatrix().GetPosition();
	cameraInfo.viewMatrix = m_hostingCamera->GetViewMatrix();
	cameraInfo.projMatrix = m_hostingCamera->GetProjectionMatrix();
	cameraInfo.viewProjMatrix = m_hostingCamera->GetViewProjectionMatrix();
	cameraInfo.viewFrustum = m_hostingCamera->GetViewFrustum();
	cameraInfo.zSortDistanceBase = m_hostingCamera->GetZSortDistanceBase();
	parentDrawList->RenderSubDrawList(
		m_targetWorld->GetRenderer()->GetDrawElementList(),
		cameraInfo,
		m_internalRenderer,
		renderTarget,
		depthBuffer);
	parentDrawList->RenderSubDrawList(
		m_targetWorld->GetRenderer()->GetDrawElementList(),
		cameraInfo,
		m_internalRenderer,
		renderTarget,
		depthBuffer);
	m_targetWorld->GetRenderer()->EndFrame();
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::OnRoutedEvent(UIEventArgs* e)
{
	m_targetWorld->OnUIEvent(e);
	if (e->handled) return;

	UIViewportLayer::OnRoutedEvent(e);
}

//==============================================================================
// CameraBehavior
//==============================================================================
//------------------------------------------------------------------------------
CameraBehavior::CameraBehavior()
	: m_targetCamera(nullptr)
{
}

//------------------------------------------------------------------------------
CameraBehavior::~CameraBehavior()
{
}

//==============================================================================
// CylinderMouseMoveCameraBehavior
//==============================================================================
//------------------------------------------------------------------------------
CylinderMouseMoveCameraBehavior::CylinderMouseMoveCameraBehavior()
	: m_prevPos()
	, m_RDrag(false)
	, m_MDrag(false)
{
}

//------------------------------------------------------------------------------
CylinderMouseMoveCameraBehavior::~CylinderMouseMoveCameraBehavior()
{
}

//------------------------------------------------------------------------------
bool CylinderMouseMoveCameraBehavior::InjectMouseMove(int x, int y)
{
	if (m_RDrag)
	{
		//Camera* camera = GetTargetCamera();
		//Vector3 pos = camera->GetPosition();
		//Vector3 lookAt = camera->GetLookAt();
		//Vector3 localPos = pos - lookAt;

		//Matrix m = Matrix::LookAtLH(pos, lookAt, Vector3::UnitY);
		//Vector3 r = m.ToEulerAngles(RotationOrder_XYZ);

		//float lenXZ = Vector2(localPos.X, localPos.Z).GetLength();
		//float rotX = atan2f(localPos.Z, localPos.X);
		//float rotY = atan2f(localPos.Y, lenXZ);

		//rotX = r.X;
		//rotY = r.Y;
		//rotX += 0.0001f * (x - m_prevPos.X);
		//rotY += 0.0001f * (y - m_prevPos.Y);

		//while (rotX >= Math::PI) {
		//	rotX -= Math::PI * 2.0f;
		//}
		//while (rotX <= -Math::PI) {
		//	rotX += Math::PI * 2.0f;
		//}
		//if (rotY > Math::PI / 2.0f) {
		//	rotY = Math::PI / 2.0f;
		//}
		//if (rotY < -Math::PI / 2.0f) {
		//	rotY = -Math::PI / 2.0f;
		//}

		//Matrix mat;// = Matrix::RotationYawPitchRoll(rotX, rotY, 0);
		//mat.RotateX(rotY);
		//mat.RotateY(rotX);
		//Vector3 newPos(0, 0, localPos.GetLength());
		//newPos.TransformCoord(mat);
		//camera->SetPosition(newPos);

		float s = 0.01f;
		float dx = s * (x - m_prevPos.x);
		float dy = s * (y - m_prevPos.y);

		CameraComponent* camera = GetTargetCamera();
		Vector3 view;
		Vector3 vup = camera->GetUpDirection();
		Vector3 pos = camera->GetTransform()->position;
		Vector3 look_at = camera->GetLookAt();
		vup.Normalize();

		// 注視点中心回転
		if (1)
		{
			view = pos - look_at;
		}
		// 視点中心回転
		else
		{
			view = look_at - pos;
		}

		Matrix m;
		float d;

		if (dx != 0)
		{
			d = Math::PI * dx;// / width;
			// Y 軸周りの回転を逆にする場合 (右手系)
			if (0) // CAMTYPE_REV_ROT_Y
			{
				d = -d;
			}

			// vup を軸にする場合
			//if ( m_type & CAMTYPE_AROUND_UP )
			//	D3DXMatrixRotationAxis( &m, &vup, D3DXToRadian(d) );
			//else
			//	D3DXMatrixRotationY( &m, D3DXToRadian(d) );

			m.RotateY(d);
			view.TransformCoord(m);
		}
		if (dy != 0)
		{
			// 球タイプ
			if (1)
			{
				Vector3 vaxis = Vector3::Cross(vup, view);
				
				vaxis.Normalize();
				d = -(Math::PI * dy/* / height*/);


				//if (d > Math::PI / 2.0f) {
				//	d = Math::PI / 2.0f;
				//}
				//if (d < -Math::PI / 2.0f) {
				//	d = -Math::PI / 2.0f;
				//}
				//printf("%f\n", d);

				m = Matrix::MakeRotationAxis(vaxis, d);
				view.TransformCoord(m);


				//D3DXVec3Cross( &vaxis, &vup, &view );
				//D3DXVec3Normalize( &vaxis, &vaxis );
				//d = (float)180.0 * (float)dy / (float)prc->bottom;
				//D3DXMatrixRotationAxis( &m, &vaxis, D3DXToRadian(d) );
				//D3DXVec3TransformNormal( &view, &view, &m );
			}
			else
			{
				//if( m_type & CAMTYPE_AROUND_UP )
				//	view += ( vup * (float)dy );
				//else
				//	view.y += (float)dy;
			}
		}

		// 注視点中心回転
		if (1)
		{
			Vector3 old = pos;

			pos = look_at + view;

			// 上または下にドラッグし続けた場合、中天を通り過ぎたところで
			// XZ上の反対側の象限にカメラが移動して画面がちらちらするのを防ぐ処理
			//if (((old.X < 0 && pos.X > 0) || (old.X > 0 && pos.X < 0)) &&
			//	((old.Z < 0 && pos.Z > 0) || (old.Z > 0 && pos.Z < 0)))
			//{
			//	pos = old;
			//}

		}
		// 視点中心回転
		else
		{
			pos += view;
		}

		camera->GetTransform()->position = pos;

		m_prevPos.x = x;
		m_prevPos.y = y;
		return true;
	}
	if (m_MDrag)
	{
		CameraComponent* camera = GetTargetCamera();
		Vector3 pos = camera->GetPosition();
		Vector3 lookAt = camera->GetLookAt();

		// 1px に付きどの程度移動するか
		float s = 0.00175f * (pos - lookAt).GetLength();
		float dx = s * (x - m_prevPos.x);
		float dy = s * (y - m_prevPos.y);

		Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸(正面方向)とする
		zaxis = lookAt;
		zaxis -= pos;
		zaxis.Normalize();
		// Z軸と上方向のベクトルの外積をとると右方向が分かる
		xaxis = Vector3::Cross(Vector3::UnitY, zaxis);
		xaxis.Normalize();
		// 2つの軸がわかったので、その2つの外積は残りの上方向になる
		yaxis = Vector3::Cross(zaxis, xaxis);

		// 横方向と縦方向にそれぞれ移動
		xaxis *= -dx;
		yaxis *= dy;

		// 移動分を1つのベクトルにまとめて、位置と注視点を平行移動する
		Vector3 offset;
		offset = xaxis + yaxis;
		camera->SetPosition(pos + offset);
		camera->SetLookAt(lookAt + offset);


		m_prevPos.x = x;
		m_prevPos.y = y;

		//Vector3 view;
		//Matrix mat = Matrix::Inverse(GetTargetCamera()->GetViewMatrix());
		//mat.M41 = mat.M42 = mat.M43 = 0.0f;
		//view.X = -dx * s;
		//view.Y = dy * s;
		//view.Z = 0.f;
		//view.TransformCoord(mat);

		//camera->SetPosition(pos + view);
		//camera->SetLookAt(look_at + view);
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
bool CylinderMouseMoveCameraBehavior::InjectMouseButtonDown(MouseButtons button, int x, int y)
{
	// 右ボタン
	if (button == MouseButtons::Right)
	{
		m_RDrag = true;
		m_prevPos.x = x;
		m_prevPos.y = y;
		return true;

	}
	// 中ボタン
	else if (button == MouseButtons::Middle)
	{
		m_MDrag = true;
		m_prevPos.x = x;
		m_prevPos.y = y;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool CylinderMouseMoveCameraBehavior::InjectMouseButtonUp(MouseButtons button, int x, int y)
{
	// 右ボタン
	if (button == MouseButtons::Right)
	{
		m_RDrag = false;
		return true;

	}
	// 中ボタン
	else if (button == MouseButtons::Middle)
	{
		m_MDrag = false;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool CylinderMouseMoveCameraBehavior::InjectMouseWheel(int delta)
{
	CameraComponent* camera = GetTargetCamera();
	Vector3 view = camera->GetTransform()->position.Get() - camera->GetLookAt();
	if (delta >= 0) {
		view *= 0.9f;
	}
	else {
		view *= 1.1f;
	}
	camera->GetTransform()->position = (camera->GetLookAt() + view);
	return true;
}


//==============================================================================
// Camera
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Camera, WorldObject);

//------------------------------------------------------------------------------
Camera::Camera()
	: WorldObject()
	, m_component(nullptr)
{
}

//------------------------------------------------------------------------------
Camera::~Camera()
{
}

//------------------------------------------------------------------------------
void Camera::Initialize(SceneGraph* owner, CameraProjection proj)
{
	WorldObject::Initialize();
	m_component = NewObject<CameraComponent>(owner, proj);
	AddComponent(m_component);

	if (proj == CameraProjection_2D)
	{
		transform.position = Vector3(0, 0, 0);
	}
	else if (proj == CameraProjection_3D)
	{
		transform.position = Vector3(0, 1.0f, -10.0f);	// Unity based.
	}
}

//------------------------------------------------------------------------------
CameraComponent* Camera::GetCameraComponent() const
{
	return m_component;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
