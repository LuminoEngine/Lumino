
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
void OffscreenWorldView::initialize()
{
	RenderView::initialize();
	m_renderer = NewObject<DrawList>(detail::EngineDomain::GetGraphicsManager());

	//m_renderView = RefPtr<RenderView>::MakeRef();
	//m_renderView->
	m_lists.add(m_renderer->GetDrawElementList());
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
	m_cameraInfo.viewPosition = mainViewCamera->GetTransform()->GetWorldMatrix().getPosition();
	m_cameraInfo.viewMatrix = CalculateViewMatrix(mainViewCamera);
	m_cameraInfo.projMatrix = CalculateProjectionMatrix(mainViewCamera);
	m_cameraInfo.viewProjMatrix = m_cameraInfo.viewMatrix * m_cameraInfo.projMatrix;
	m_cameraInfo.viewFrustum = mainViewCamera->GetViewFrustum();	// TODO: この View 独自処理にしたい
	m_cameraInfo.zSortDistanceBase = mainViewCamera->GetZSortDistanceBase();



	// TODO: tmp
	m_renderer->SetCurrentCamera(mainViewCamera);



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
	m_renderer->clear(ClearFlags::All, Color::White, 1.0f, 0);
	

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
// SkyComponent
//==============================================================================
//------------------------------------------------------------------------------
SkyComponent::SkyComponent()
{
}

//------------------------------------------------------------------------------
SkyComponent::~SkyComponent()
{
}

//------------------------------------------------------------------------------
void SkyComponent::initialize()
{
	VisualComponent::initialize();


	{

		auto shader = ln::Shader::create("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Sky.fx");
		m_skyMaterial = NewObject<Material>();
		m_skyMaterial->SetShader(shader);
	}
}

//------------------------------------------------------------------------------
void SkyComponent::OnRender2(DrawList* renderer)
{
	{
		auto* cam = renderer->GetCurrentCamera();
		Vector3 frustumRayTL = Vector3::Normalize(cam->ViewportToWorldPoint(Vector3(0, 0, 1)) - cam->ViewportToWorldPoint(Vector3(0, 0, 0)));
		Vector3 frustumRayTR = Vector3::Normalize(cam->ViewportToWorldPoint(Vector3(640, 0, 1)) - cam->ViewportToWorldPoint(Vector3(640, 0, 0)));
		Vector3 frustumRayBL = Vector3::Normalize(cam->ViewportToWorldPoint(Vector3(0, 480, 1)) - cam->ViewportToWorldPoint(Vector3(0, 480, 0)));
		m_skyMaterial->SetVectorParameter("frustumRayTL", Vector4(frustumRayTL, 0));
		m_skyMaterial->SetVectorParameter("frustumRayTR", Vector4(frustumRayTR, 0));
		m_skyMaterial->SetVectorParameter("frustumRayBL", Vector4(frustumRayBL, 0));



		Vector3 cameraPos = Vector3(0, 997, 0);// = cam->GetTransform()->position.Get();
											   //cameraPos.Normalize();
		Vector3 lightPos = 1.0f * Vector3::Normalize(1, -0, -1);//sunDirection.normalized();

		float fCameraHeight = cameraPos.getLength();
		float fCameraHeight2 = fCameraHeight * fCameraHeight;

		const float red = 0.680f;
		const float green = 0.550f;
		const float blue = 0.440f;
		const auto invWav = [](float waveLength) { return 1.0f / pow(waveLength, 4.0f); };

		Vector3 invWavelength = Vector3(invWav(red), invWav(green), invWav(blue));

		float fInnerRadius = 1000.0f;
		float fInnerRadius2 = fInnerRadius * fInnerRadius;

		float fOuterRadius = 1025.0f;
		float fOuterRadius2 = fOuterRadius * fOuterRadius;

		float fScale = 1.0f / (fOuterRadius - fInnerRadius);

		const float Kr = 0.0025f -0.00015f;//static_cast<float>(gui.slider(L"Kr").value);
		const float Km = 0.0010f +0.0015f;// static_cast<float>(gui.slider(L"Km").value);
		const float Esun = 1300.0f;

		float fKrESun = Kr*Esun;
		float fKmESun = Km*Esun;
		float fKr4PI = Kr*4.0f* Math::PI;
		float fKm4PI = Km*4.0f*Math::PI;

		const float rayleighScaleDepth = 0.25f;
		float fScaleDepth = rayleighScaleDepth;
		float fScaleOverScaleDepth = (1.0f / (fOuterRadius - fInnerRadius)) / rayleighScaleDepth;
		float g = -0.999f;
		float exposure = 0.05 + 0.03;// static_cast<float>(gui.slider(L"Exposure").value);


		m_skyMaterial->SetVectorParameter("v3CameraPos", Vector4(cameraPos, 0));
		m_skyMaterial->SetFloatParameter("fCameraHeight", fCameraHeight);
		m_skyMaterial->SetVectorParameter("v3LightPos", Vector4(lightPos, 0));
		m_skyMaterial->SetFloatParameter("fCameraHeight2", fCameraHeight2);
		m_skyMaterial->SetVectorParameter("v3InvWavelength", Vector4(invWavelength, 0));
		m_skyMaterial->SetFloatParameter("fScale", fScale);
		m_skyMaterial->SetFloatParameter("fOuterRadius", fOuterRadius);
		m_skyMaterial->SetFloatParameter("fOuterRadius2", fOuterRadius2);
		m_skyMaterial->SetFloatParameter("fInnerRadius", fInnerRadius);
		m_skyMaterial->SetFloatParameter("fInnerRadius2", fInnerRadius2);
		m_skyMaterial->SetFloatParameter("fKrESun", fKrESun);
		m_skyMaterial->SetFloatParameter("fKmESun", fKmESun);
		m_skyMaterial->SetFloatParameter("fKr4PI", fKr4PI);
		m_skyMaterial->SetFloatParameter("fKm4PI", fKm4PI);
		m_skyMaterial->SetFloatParameter("fScaleDepth", fScaleDepth);
		m_skyMaterial->SetFloatParameter("fScaleOverScaleDepth", fScaleOverScaleDepth);
		m_skyMaterial->SetFloatParameter("g", g);
		m_skyMaterial->SetFloatParameter("exposure", exposure);
		/*

		float3 v3CameraPos;		// The camera's current position
		float fCameraHeight;	// The camera's current height

		float3 v3LightPos;		// The direction vector to the light source
		float fCameraHeight2;	// fCameraHeight^2

		float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
		float fScale;			// 1 / (fOuterRadius - fInnerRadius)

		float fOuterRadius;		// The outer (atmosphere) radius
		float fOuterRadius2;	// fOuterRadius^2
		float fInnerRadius;		// The inner (planetary) radius
		float fInnerRadius2;	// fInnerRadius^2

		float fKrESun;			// Kr * ESun
		float fKmESun;			// Km * ESun
		float fKr4PI;			// Kr * 4 * PI
		float fKm4PI;			// Km * 4 * PI

		float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
		float fScaleOverScaleDepth;	// fScale / fScaleDepth
		float g;
		float exposure;
		*/

		renderer->Blit(nullptr, nullptr, m_skyMaterial);
	}



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
void MirrorComponent::initialize()
{
	VisualComponent::initialize();

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
	auto shader = ln::Shader::create("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Mirror.fx");
	m_material->SetShader(shader);

	auto tex = ln::Texture2D::create("D:/Proj/LN/HC1/Assets/Data/waterbump.png");
	m_material->SetTextureParameter(_T("xWaterBumpMap"), tex);

}
float g_time = 0;
//------------------------------------------------------------------------------
void MirrorComponent::OnRender2(DrawList* renderer)
{




	g_time += 0.001;
	m_material->SetMaterialTexture(m_offscreen->GetRenderTarget());
	m_material->SetVectorParameter("xCamPos", Vector4(renderer->GetCurrentCamera()->GetTransform()->position.get(), 1.0));
	m_material->SetFloatParameter("time", g_time);

	// TODO: 法泉が入っていない？
	renderer->DrawSquare(100, 100, 1, 1, Color::White, Matrix::Identity, m_material);
}


LN_NAMESPACE_END
