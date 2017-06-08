
#include "Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/OffscreenWorldView.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// OffscreenWorldView
/*
■[MME メモ] DefaultEffect アノテーションのためのグループ分け
	ひとつの OffscreenScene 内でのエフェクトファイルの割り当て方法や
	描画そのものの有無を決めるため、ワイルドカード指定可能なオブジェクト名の
	マッチング判定が行われる。
	ただ、この処理を毎フレーム行うのはかなり処理時間的にキツイ。

	そこで、一度マッチングを行った後はどのエフェクトを割り当てるかを示す
	整数値を持たせて、以降これを使って割り当てを決める。
	例えば以下のようになる。

	string DefaultEffect =
	"self = hide;"				… グループ 0
	"Mirror*.x = hide;"			… グループ 0
	"Player1=MirrorMain.fx;";	… グループ 1
	"Player2=MirrorMain.fx;";	… グループ 1
	"*=MirrorObject.fx;";		… グループ 2

	もしいずれにもマッチしなければグループ -1。
	グループ -1 は非表示を表す。
*/
//==============================================================================
//------------------------------------------------------------------------------
OffscreenWorldView::OffscreenWorldView()
	: m_id(0)
{
}

//------------------------------------------------------------------------------
OffscreenWorldView::~OffscreenWorldView()
{
}

//------------------------------------------------------------------------------
void OffscreenWorldView::Initialize()
{
	RenderView::Initialize();
	m_renderer = NewObject<DrawList>(detail::EngineDomain::GetGraphicsManager());

	//m_renderView = RefPtr<RenderView>::MakeRef();
	//m_renderView->
	m_lists.Add(m_renderer->GetDrawElementList());
}

//------------------------------------------------------------------------------
RenderTargetTexture* OffscreenWorldView::GetRenderTarget() const
{
	return m_renderTarget;
}

//------------------------------------------------------------------------------
void OffscreenWorldView::HideVisual(VisualComponent* renderObject)
{
	auto* filterInfo = UpdateRenderObjectFilterInfo(renderObject);
	filterInfo->effectGroup = -1;
}

//------------------------------------------------------------------------------
Matrix OffscreenWorldView::CalculateViewMatrix(CameraComponent* mainViewCamera)
{
	return Matrix::MakeReflection(Plane(Vector3::UnitY)) * mainViewCamera->GetViewMatrix();
}

//------------------------------------------------------------------------------
Matrix OffscreenWorldView::CalculateProjectionMatrix(CameraComponent* mainViewCamera)
{
	return mainViewCamera->GetProjectionMatrix();;
}

//------------------------------------------------------------------------------
void OffscreenWorldView::RenderWorld(World* world, CameraComponent* mainViewCamera, RenderView* mainRenderView)
{
	m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(mainViewCamera) + 1;
	m_cameraInfo.viewPixelSize = mainRenderView->GetViewSize();
	m_cameraInfo.viewPosition = mainViewCamera->GetTransform()->GetWorldMatrix().GetPosition();
	m_cameraInfo.viewMatrix = CalculateViewMatrix(mainViewCamera);
	m_cameraInfo.projMatrix = CalculateProjectionMatrix(mainViewCamera);
	m_cameraInfo.viewProjMatrix = m_cameraInfo.viewMatrix * m_cameraInfo.projMatrix;
	m_cameraInfo.viewFrustum = mainViewCamera->GetViewFrustum();	// TODO: この View 独自処理にしたい
	m_cameraInfo.zSortDistanceBase = mainViewCamera->GetZSortDistanceBase();








	m_renderer->BeginMakeElements();



	RenderTargetTexture* backbuffer = m_renderer->GetRenderTarget(0);

	if (m_renderTarget == nullptr)
	{
		m_renderTarget = NewObject<RenderTargetTexture>(SizeI::FromFloatSize(mainRenderView->GetViewSize()), 1, TextureFormat::R8G8B8X8);
	}
	//else if (m_renderTarget->GetSize() != backbuffer->GetSize())
	//{
	//	LN_NOTIMPLEMENTED();
	//}

	// TODO: 深度バッファやクリア方法の指定
	m_renderer->SetRenderTarget(0, m_renderTarget);
	//g->SetDepthBuffer(m_depthBuffer);
	m_renderer->Clear(ClearFlags::All, Color::White, 1.0f, 0);
	

	world->Render(m_renderer, mainViewCamera, WorldDebugDrawFlags::None, this);	// TODO: debugdraw の指定


	// 戻す
	m_renderer->SetRenderTarget(0, backbuffer);




	// user override
	//OnUpdateRenderViewPoint(m_renderView);

	DrawList* r = world->GetRenderer();
	r->RenderSubView(this);
}

//------------------------------------------------------------------------------
bool OffscreenWorldView::FilterRenderObject(VisualComponent* renderObject)
{
	auto* filterInfo = UpdateRenderObjectFilterInfo(renderObject);
	return filterInfo->effectGroup >= 0;
}

//------------------------------------------------------------------------------
detail::OffscreenFilterInfo* OffscreenWorldView::UpdateRenderObjectFilterInfo(VisualComponent* renderObject)
{
	auto* filterInfo = renderObject->GetOffscreenFilterInfo(m_id);

	// オーナーが this ではない。再割り当てする。
	// (renderObject が新しく作成されたオブジェクトであるか、前の Offscreen が解放され ID が返却された後、新たに作成された Offscreen が同じ ID を取得した場合)
	if (filterInfo->ownerOffscreenView != this)
	{
		filterInfo->ownerOffscreenView = this;
		filterInfo->effectGroup = 0;
	}
	return filterInfo;
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
	m_offscreen->HideVisual(this);

	m_material = NewObject<Material>();
	//m_material->SetMaterialTexture(Texture2D::GetBlackTexture());
	//m_material->SetMaterialTexture(Texture2D::GetWhiteTexture());
	//m_material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	auto shader = ln::Shader::Create("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Mirror.fx");
	m_material->SetShader(shader);

	auto tex = ln::Texture2D::Create("D:/Proj/LN/HC1/Assets/Data/waterbump.png");
	m_material->SetTextureParameter(_T("xWaterBumpMap"), tex);
}
float g_time = 0;
//------------------------------------------------------------------------------
void MirrorComponent::OnRender2(DrawList* renderer)
{
	g_time += 0.001;
	m_material->SetMaterialTexture(m_offscreen->GetRenderTarget());
	m_material->SetVectorParameter("xCamPos", Vector4(renderer->GetCurrentCamera()->GetTransform()->position.Get(), 1.0));
	m_material->SetFloatParameter("time", g_time);

	// TODO: 法泉が入っていない？
	renderer->DrawSquare(10, 10, 1, 1, Color::White, Matrix::Identity, m_material);
}


LN_NAMESPACE_END
