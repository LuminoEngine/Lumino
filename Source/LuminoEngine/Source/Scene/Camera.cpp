
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Rendering/SceneRenderer.h>
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
CameraComponent* CameraComponent::getMain3DCamera()
{
	return detail::EngineDomain::getDefaultWorld3D()->getMainCamera()->getCameraComponent();
}

//------------------------------------------------------------------------------
CameraComponent* CameraComponent::getMain2DCamera()
{
	return detail::EngineDomain::getDefaultWorld2D()->getMainCamera()->getCameraComponent();
}

//------------------------------------------------------------------------------
CameraComponent::CameraComponent()
	: SceneNode()
	, m_projectionMode()
	, m_directionMode(CameraDirection::transform)
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
}

//------------------------------------------------------------------------------
void CameraComponent::initialize(CameraProjection proj)
{
	SceneNode::initialize();
	m_projectionMode = proj;

	if (m_projectionMode == CameraProjection_2D)
	{
		m_nearClip = 0.0f;	// TODO
		m_farClip = 1000.0f;
		m_zSortDistanceBase = ZSortDistanceBase::CameraScreenDistance;
	}
	else if (m_projectionMode == CameraProjection_3D)
	{
		m_nearClip = 0.3f;
		m_farClip = 1000.0f;
		m_zSortDistanceBase = ZSortDistanceBase::CameraDistance;
	}
}

//------------------------------------------------------------------------------
void CameraComponent::setCameraBehavior(CameraBehavior* behavior)
{
	if (m_cameraBehavior != nullptr) {
		m_cameraBehavior->setTargetCamera(nullptr);
	}
	LN_REFOBJ_SET(m_cameraBehavior, behavior);
	m_cameraBehavior->setTargetCamera(this);
}

//------------------------------------------------------------------------------
Vector3 CameraComponent::worldToViewportPoint(const Vector3& position) const
{
	const Size& size = m_ownerLayer->getViewSize();
	return Vector3::project(position, m_viewProjMatrix, 0.0f, 0.0f, size.width, size.height, m_nearClip, m_farClip);
}

//------------------------------------------------------------------------------
Vector3 CameraComponent::viewportToWorldPoint(const Vector3& position) const
{
	const Size& size = m_ownerLayer->getViewSize();
	Vector3 v;
	v.x = (((position.x - 0) / size.width) * 2.0f) - 1.0f;
	v.y = -((((position.y - 0) / size.height) * 2.0f) - 1.0f);
	v.z = (position.z - m_nearClip) / (m_farClip - m_nearClip);
	return Vector3::transformCoord(v, m_viewProjMatrixI);
}

//------------------------------------------------------------------------------
void CameraComponent::updateMatrices(const Size& viewSize)
{
	const Matrix& worldMatrix = getOwnerObject()->transform.getWorldMatrix();

	// 2D モード
	if (m_projectionMode == CameraProjection_2D)
	{
		// 正面方向
		Vector3 direction = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);
		m_direction = Vector4(direction, 0.0f);

		m_viewMatrix = worldMatrix;
		m_projMatrix = Matrix::makePerspective2DLH(viewSize.width, viewSize.height, m_nearClip, m_farClip);

		
		//Perspective2DLH(viewSize.width, viewSize.height, m_nearClip, m_farClip, &m_projMatrix);
		m_viewProjMatrix = m_viewMatrix * m_projMatrix;

		auto a1 = Vector3::transformCoord(
			Vector3(48, 0, 10), m_viewProjMatrix);
		
		auto a2 = Vector3::transformCoord(
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
		if (m_directionMode == CameraDirection::lookAt &&
			worldMatrix.getPosition() != m_lookAt)	// 位置と注視点が同じだと、Matrix::MakeLookAt で NAN になる
		{
			lookAt = m_lookAt;
		}
		else
		{
			lookAt = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);
		}

		// ビュー行列
		m_viewMatrix = Matrix::makeLookAtLH(worldMatrix.getPosition(), lookAt, m_upDirection);

		if (m_reflectionPlane.Normal != Vector3::Zero)
		{
			m_viewMatrix = Matrix::makeReflection(m_reflectionPlane) * m_viewMatrix;
		}

		// プロジェクション行列の更新
		// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
		m_projMatrix = Matrix::makePerspectiveFovLH(m_fovY, viewSize.width / viewSize.height, m_nearClip, m_farClip);

		m_viewProjMatrix = m_viewMatrix * m_projMatrix;

		// 正面方向
		Vector3 d = lookAt - worldMatrix.getPosition();
		d.normalize();
		m_direction = Vector4(d, 0.0f);
	}

	m_viewFrustum = ViewFrustum(m_viewProjMatrix);

	m_viewMatrixI = Matrix::makeInverse(m_viewMatrix);
	m_projMatrixI = Matrix::makeInverse(m_projMatrix);
	m_viewProjMatrixI = Matrix::makeInverse(m_viewProjMatrix);
	m_viewMatrixT = Matrix::makeTranspose(m_viewMatrix);
	m_projMatrixT = Matrix::makeTranspose(m_projMatrix);
	m_viewProjMatrixT = Matrix::makeTranspose(m_viewProjMatrix);
	m_viewMatrixIT = Matrix::makeTranspose(m_viewMatrixI);
	m_projMatrixIT = Matrix::makeTranspose(m_projMatrixI);
	m_viewProjMatrixIT = Matrix::makeTranspose(m_viewProjMatrixI);
}

//------------------------------------------------------------------------------
void CameraComponent::onUpdate(float deltaSceonds)
{
	SceneNode::onUpdate(deltaSceonds);
}

//------------------------------------------------------------------------------
void CameraComponent::onUIEvent(UIEventArgs* e)
{
	if (e->getType() == UIEvents::MouseDownEvent)
	{
		if (getCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->getPosition(me->sender);
			getCameraBehavior()->injectMouseButtonDown(me->getMouseButtons(), pos.x, pos.y);
		}
	}
	else if (e->getType() == UIEvents::MouseUpEvent)
	{
		if (getCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->getPosition(me->sender);
			getCameraBehavior()->injectMouseButtonUp(me->getMouseButtons(), pos.x, pos.y);
		}
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		if (getCameraBehavior() != nullptr)
		{
			auto* me = static_cast<UIMouseEventArgs*>(e);
			auto pos = me->getPosition(me->sender);
			getCameraBehavior()->injectMouseMove(pos.x, pos.y);
		}
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		if (getCameraBehavior() != nullptr)
		{
			if (getCameraBehavior() != nullptr) {
				auto* me = static_cast<UIMouseWheelEventArgs*>(e);
				getCameraBehavior()->injectMouseWheel(me->getDelta());
			}
		}
	}
	else
	{
		Component::onUIEvent(e);
	}
}

//==============================================================================
// CameraMouseMoveBehavior
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CameraMouseMoveBehavior, Behavior);

//------------------------------------------------------------------------------
CameraMouseMoveBehavior::CameraMouseMoveBehavior()
	: m_targetCamera(nullptr)
	, m_onUIEventConnection()
	, m_prevPos()
	, m_RDrag(false)
	, m_MDrag(false)
{
}

//------------------------------------------------------------------------------
CameraMouseMoveBehavior::~CameraMouseMoveBehavior()
{
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::initialize()
{
	Behavior::initialize();
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::onAttached()
{
	m_targetCamera = dynamic_cast<Camera*>(getOwnerObject());
	LN_ASSERT(m_targetCamera != nullptr);	// TODO: この辺はアタッチ前に TypeInfo とかでチェックしたい。そうしないとエディタで使えない。
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::onDetaching()
{
	m_targetCamera = nullptr;
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::onAttachedWorld(World* world)
{
	m_onUIEventConnection = world->connectOnUIEvent(createDelegate(this, &CameraMouseMoveBehavior::World_onUIEvent));
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::onDetachedWorld(World* world)
{
	m_onUIEventConnection.disconnect();
}

//------------------------------------------------------------------------------
void CameraMouseMoveBehavior::World_onUIEvent(UIEventArgs* e)
{
	if (e->getType() == UIEvents::MouseDownEvent)
	{
		auto* me = static_cast<UIMouseEventArgs*>(e);
		auto pos = me->getPosition(me->sender);
		injectMouseButtonDown(me->getMouseButtons(), pos.x, pos.y);
	}
	else if (e->getType() == UIEvents::MouseUpEvent)
	{
		auto* me = static_cast<UIMouseEventArgs*>(e);
		auto pos = me->getPosition(me->sender);
		injectMouseButtonUp(me->getMouseButtons(), pos.x, pos.y);
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		auto* me = static_cast<UIMouseEventArgs*>(e);
		auto pos = me->getPosition(me->sender);
		injectMouseMove(pos.x, pos.y);
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		auto* me = static_cast<UIMouseWheelEventArgs*>(e);
		injectMouseWheel(me->getDelta());
	}
	else
	{
		Component::onUIEvent(e);
	}
}

//------------------------------------------------------------------------------
bool CameraMouseMoveBehavior::injectMouseMove(int x, int y)
{
	if (m_RDrag)
	{
		//Camera* camera = getTargetCamera();
		//Vector3 pos = camera->GetPosition();
		//Vector3 lookAt = camera->getLookAt();
		//Vector3 localPos = pos - lookAt;

		//Matrix m = Matrix::LookAtLH(pos, lookAt, Vector3::UnitY);
		//Vector3 r = m.toEulerAngles(RotationOrder_XYZ);

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
		//mat.rotateX(rotY);
		//mat.rotateY(rotX);
		//Vector3 newPos(0, 0, localPos.GetLength());
		//newPos.transformCoord(mat);
		//camera->setPosition(newPos);

		float s = 0.01f;
		float dx = s * (x - m_prevPos.x);
		float dy = s * (y - m_prevPos.y);

		//CameraComponent* camera = getTargetCamera();

		CameraComponent* cameraComponent = m_targetCamera->getCameraComponent();
		Vector3 view;
		Vector3 vup = cameraComponent->getUpDirection();
		Vector3 pos = m_targetCamera->getPosition();//camera->getTransform()->position;
		Vector3 look_at = cameraComponent->getLookAt();
		vup.normalize();

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

			m.rotateY(d);
			view.transformCoord(m);
		}
		if (dy != 0)
		{
			// 球タイプ
			if (1)
			{
				Vector3 vaxis = Vector3::cross(vup, view);

				vaxis.normalize();
				d = -(Math::PI * dy/* / height*/);


				//if (d > Math::PI / 2.0f) {
				//	d = Math::PI / 2.0f;
				//}
				//if (d < -Math::PI / 2.0f) {
				//	d = -Math::PI / 2.0f;
				//}

				m = Matrix::makeRotationAxis(vaxis, d);
				view.transformCoord(m);


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

		//camera->getTransform()->position = pos;

		//camera->getTransform()->lookAt(look_at);	// TODO: tmp

		m_targetCamera->setPosition(pos);
		m_targetCamera->transform.lookAt(look_at);	// TODO: tmp

		m_prevPos.x = x;
		m_prevPos.y = y;
		return true;
	}
	if (m_MDrag)
	{
		//CameraComponent* camera = getTargetCamera();
		//Vector3 pos = camera->getPosition();
		//Vector3 lookAt = camera->getLookAt();
		CameraComponent* cameraComponent = m_targetCamera->getCameraComponent();
		Vector3 pos = m_targetCamera->getPosition();
		Vector3 lookAt = cameraComponent->getLookAt();

		// 1px に付きどの程度移動するか
		float s = 0.00175f * (pos - lookAt).getLength();
		float dx = s * (x - m_prevPos.x);
		float dy = s * (y - m_prevPos.y);

		Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸(正面方向)とする
		zaxis = lookAt;
		zaxis -= pos;
		zaxis.normalize();
		// Z軸と上方向のベクトルの外積をとると右方向が分かる
		xaxis = Vector3::cross(Vector3::UnitY, zaxis);
		xaxis.normalize();
		// 2つの軸がわかったので、その2つの外積は残りの上方向になる
		yaxis = Vector3::cross(zaxis, xaxis);

		// 横方向と縦方向にそれぞれ移動
		xaxis *= -dx;
		yaxis *= dy;

		// 移動分を1つのベクトルにまとめて、位置と注視点を平行移動する
		Vector3 offset;
		offset = xaxis + yaxis;
		m_targetCamera->setPosition(pos + offset);
		cameraComponent->setLookAt(lookAt + offset);


		m_prevPos.x = x;
		m_prevPos.y = y;

		//Vector3 view;
		//Matrix mat = Matrix::inverse(getTargetCamera()->GetViewMatrix());
		//mat.M41 = mat.M42 = mat.M43 = 0.0f;
		//view.X = -dx * s;
		//view.Y = dy * s;
		//view.Z = 0.f;
		//view.transformCoord(mat);

		//camera->setPosition(pos + view);
		//camera->setLookAt(look_at + view);
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
bool CameraMouseMoveBehavior::injectMouseButtonDown(MouseButtons button, int x, int y)
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
bool CameraMouseMoveBehavior::injectMouseButtonUp(MouseButtons button, int x, int y)
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
bool CameraMouseMoveBehavior::injectMouseWheel(int delta)
{
	//CameraComponent* cameraComponent = m_targetCamera->getCameraComponent();
	//Vector3 view = m_targetCamera->getReferenceCount() - camera->getLookAt();
	//if (delta >= 0) {
	//	view *= 0.9f;
	//}
	//else {
	//	view *= 1.1f;
	//}
	//camera->getTransform()->position = (camera->getLookAt() + view);
	return true;
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
//CameraViewportLayerPtr CameraViewportLayer::create(Camera* camera)
//{
//	auto ptr = CameraViewportLayerPtr::MakeRef();
//	ptr->initialize(SceneGraphManager::Instance, camera);
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
void CameraViewportLayer::initialize(SceneGraphManager* manager, World* targetWorld, Camera* hostingCamera)
{
	m_targetWorld = targetWorld;
	m_hostingCamera = hostingCamera;
	m_hostingCamera->m_ownerLayer = this;

	if (m_hostingCamera->GetProjectionMode() == CameraProjection_3D)
	{
		auto internalRenderer = Ref<detail::ForwardShadingRenderer>::makeRef();
		internalRenderer->initialize(manager->GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}
	else
	{
		auto internalRenderer = Ref<detail::NonShadingRenderer>::makeRef();
		internalRenderer->initialize(manager->GetGraphicsManager());
		m_internalRenderer = internalRenderer;
	}

	//auto pass = Ref<detail::RenderingPass2>::MakeRef();
	//pass->initialize(manager->getGraphicsManager());
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
	m_gizmo = Ref<tr::GizmoModel>::makeRef();
	m_gizmo->initialize(detail::EngineDomain::GetGraphicsManager());
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
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(m_hostingCamera.get());
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
	//m_internalRenderer->render(
	//	m_hostingCamera->GetOwnerSceneGraph()->getRenderer()->getDrawElementList(),
	//	cameraInfo,
	//	renderTarget,
	//	depthBuffer);
	//m_internalRenderer->render(
	//	m_hostingCamera->GetOwnerSceneGraph()->GetDebugRenderer()->getDrawElementList(),
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
void CameraViewportLayer2::initialize(World* targetWorld, CameraComponent* hostingCamera)
{
	m_targetWorld = targetWorld;
	m_hostingCamera = hostingCamera;
	m_hostingCamera->m_ownerLayer = this;

	if (m_hostingCamera->getProjectionMode() == CameraProjection_3D)
	{
		auto internalRenderer = Ref<detail::ForwardShadingRenderer>::makeRef();
		internalRenderer->initialize(detail::EngineDomain::getGraphicsManager());
		m_internalRenderer = internalRenderer;
	}
	else
	{
		auto internalRenderer = Ref<detail::NonShadingRenderer>::makeRef();
		internalRenderer->initialize(detail::EngineDomain::getGraphicsManager());
		m_internalRenderer = internalRenderer;
	}

	this->setLayerCullingMask(0xFFFFFFFF);	// TODO:
	//this->m_lists.add(m_targetWorld->getInsideWorldRenderer()->getDrawElementList());
	this->m_lists.add(m_targetWorld->getRenderer()->getDrawElementList());
	this->m_lists.add(m_targetWorld->GetDebugRenderer()->getDrawElementList());
	this->setSceneRenderer(m_internalRenderer);
}

//------------------------------------------------------------------------------
CameraViewportLayer2::~CameraViewportLayer2()
{
	m_hostingCamera->m_ownerLayer = nullptr;
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::setDebugDrawFlags(WorldDebugDrawFlags flags)
{
	m_debugDrawFlags = flags;
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	// カメラ行列の更新
	m_hostingCamera->updateMatrices(getViewSize());
	this->setViewSize(getViewSize());

	//m_targetWorld->renderRoot(m_hostingCamera, m_debugDrawFlags, this);
	m_targetWorld->renderRoot(this, m_debugDrawFlags);




	// TODO: float
	Size targetSize((float)renderTarget->getWidth(), (float)renderTarget->getHeight());
	m_hostingCamera->updateMatrices(targetSize);

	bool clearColorBuffer = (getClearMode() == RenderLayerClearMode::ColorDepth || getClearMode() == RenderLayerClearMode::Color);

	//detail::CameraInfo cameraInfo;
	this->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(m_hostingCamera.get());
	this->m_cameraInfo.viewPixelSize = targetSize;
	this->m_cameraInfo.viewPosition = m_hostingCamera->getTransform()->getWorldMatrix().getPosition();
	this->m_cameraInfo.viewDirection = m_hostingCamera->getDirectionInternal().GetXYZ();
	this->m_cameraInfo.viewMatrix = m_hostingCamera->getViewMatrix();
	this->m_cameraInfo.projMatrix = m_hostingCamera->getProjectionMatrix();
	this->m_cameraInfo.viewProjMatrix = m_hostingCamera->getViewProjectionMatrix();
	this->m_cameraInfo.viewFrustum = m_hostingCamera->getViewFrustum();
	this->m_cameraInfo.zSortDistanceBase = m_hostingCamera->getZSortDistanceBase();
	m_internalRenderer->render(this, renderTarget, depthBuffer, nullptr, clearColorBuffer, getBackgroundColor());	// TODO: diag
}

//------------------------------------------------------------------------------
void CameraViewportLayer2::onRoutedEvent(UIEventArgs* e)
{
	m_targetWorld->onUIEvent(e);
	if (e->handled) return;

	RenderLayer::onRoutedEvent(e);
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
void Camera::initialize(CameraProjection proj)
{
	WorldObject::initialize();
	m_component = newObject<CameraComponent>(proj);
	addComponent(m_component);
	transform.lookAt(Vector3::Zero);

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
CameraComponent* Camera::getCameraComponent() const
{
	return m_component;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
