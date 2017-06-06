
#include "Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/OffscreenWorldView.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// OffscreenWorldView
//==============================================================================
//------------------------------------------------------------------------------
OffscreenWorldView::OffscreenWorldView()
{
}

//------------------------------------------------------------------------------
OffscreenWorldView::~OffscreenWorldView()
{
}

//------------------------------------------------------------------------------
void OffscreenWorldView::Initialize()
{
	Object::Initialize();
	m_renderer = NewObject<DrawList>(detail::EngineDomain::GetGraphicsManager());

	//m_cameraInfo = NewObject<CameraComponent>(CameraProjection_3D);	// TODO: proj 指定方法
	//// TODO: CameraComponent を使うべきではない。ミラー描画用の特殊な視点。コンポーネントではない。
	//// CameraInterface のように抽象化するべきだろう。
	//m_cameraInfo->SetReflectionPlane(Plane(Vector3::UnitY));

	m_drawElementListSet = RefPtr<RenderView>::MakeRef();
	m_drawElementListSet->m_lists.Add(m_renderer->GetDrawElementList());
}

////------------------------------------------------------------------------------
//CameraComponent* OffscreenWorldView::GetCamera() const
//{
//	return m_cameraInfo;
//}

//------------------------------------------------------------------------------
RenderTargetTexture* OffscreenWorldView::GetRenderTarget() const
{
	return m_renderTarget;
}
static int g_flag = 0;
//------------------------------------------------------------------------------
void OffscreenWorldView::RenderWorld(World* world, CameraComponent* camera)
{
	m_renderer->BeginMakeElements();



	RenderTargetTexture* backbuffer = m_renderer->GetRenderTarget(0);

	if (m_renderTarget == nullptr)
	{
		// TODO: size 指定
		m_renderTarget = NewObject<RenderTargetTexture>(SizeI(640, 480)/*backbuffer->GetSize()*/, 1, TextureFormat::R8G8B8X8);
	}
	//else if (m_renderTarget->GetSize() != backbuffer->GetSize())
	//{
	//	LN_NOTIMPLEMENTED();
	//}

	// TODO: 深度バッファやクリア方法の指定
	m_renderer->SetRenderTarget(0, m_renderTarget);
	//g->SetDepthBuffer(m_depthBuffer);
	m_renderer->Clear(ClearFlags::All, Color::White, 1.0f, 0);
	

	g_flag = 1;
	world->Render(m_renderer, camera, WorldDebugDrawFlags::None);	// TODO: debugdraw の指定
	g_flag = 0;


	// 戻す
	m_renderer->SetRenderTarget(0, backbuffer);


	//// 姿勢コピー
	//m_cameraInfo->SetPosition(camera->GetPosition());
	//m_cameraInfo->SetRotation(camera->GetRotation());






	// TODO: Camera.cpp あたりと全く同じ処理
	auto* m_hostingCamera = camera;
	m_drawElementListSet->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(m_hostingCamera) + 1;
	m_drawElementListSet->m_cameraInfo.viewPixelSize = Size(640, 480);
	m_drawElementListSet->m_cameraInfo.viewPosition = m_hostingCamera->GetTransform()->GetWorldMatrix().GetPosition();
	//m_drawElementListSet->m_cameraInfo.viewMatrix = m_hostingCamera->GetViewMatrix();

	m_drawElementListSet->m_cameraInfo.viewMatrix = Matrix::MakeReflection(Plane(Vector3::UnitY)) *  m_hostingCamera->GetViewMatrix();

	m_drawElementListSet->m_cameraInfo.projMatrix = m_hostingCamera->GetProjectionMatrix();
	m_drawElementListSet->m_cameraInfo.viewProjMatrix = m_hostingCamera->GetViewProjectionMatrix();
	m_drawElementListSet->m_cameraInfo.viewFrustum = m_hostingCamera->GetViewFrustum();
	m_drawElementListSet->m_cameraInfo.zSortDistanceBase = m_hostingCamera->GetZSortDistanceBase();

	


	DrawList* r = world->GetRenderer();
	r->RenderSubView(m_drawElementListSet);


}

//==============================================================================
// MirrorComponent
//==============================================================================
//------------------------------------------------------------------------------
MirrorComponent::MirrorComponent()
{
}

//------------------------------------------------------------------------------
MirrorComponent::~MirrorComponent()
{
}

//------------------------------------------------------------------------------
void MirrorComponent::Initialize()
{
	VisualComponent::Initialize();

	// TODO: この辺の初期化は OnParentChanged() のようなコールバックで行う。
	// そうすれば、アタッチされた任意のワールドへ追加できる。
	m_offscreen = NewObject<OffscreenWorldView>();

	// TODO: Remove
	detail::EngineDomain::GetDefaultWorld3D()->AddOffscreenWorldView(m_offscreen);

	m_material = NewObject<Material>();
	//m_material->SetMaterialTexture(Texture2D::GetBlackTexture());
	//m_material->SetMaterialTexture(Texture2D::GetWhiteTexture());
	//m_material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	auto shader = ln::Shader::Create("C:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Mirror.fx");
	m_material->SetShader(shader);
}

//------------------------------------------------------------------------------
void MirrorComponent::OnRender2(DrawList* renderer)
{
	if (g_flag == 0)
	{
		m_material->SetMaterialTexture(m_offscreen->GetRenderTarget());
		// TODO: 法泉が入っていない？
		renderer->DrawSquare(10, 10, 1, 1, Color::White, Matrix::Identity, m_material);
	}
}


LN_NAMESPACE_END
