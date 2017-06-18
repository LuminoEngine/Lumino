
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
	m_renderer = newObject<DrawList>(detail::EngineDomain::getGraphicsManager());

	//m_renderView = RefPtr<RenderView>::MakeRef();
	//m_renderView->
	m_lists.add(m_renderer->getDrawElementList());
}

//------------------------------------------------------------------------------
RenderTargetTexture* OffscreenWorldView::getRenderTarget() const
{
	return m_renderTarget;
}

//------------------------------------------------------------------------------
void OffscreenWorldView::hideVisual(VisualComponent* renderObject)
{
	auto* filterInfo = updateRenderObjectFilterInfo(renderObject);
	filterInfo->effectGroup = -1;
}

//------------------------------------------------------------------------------
Matrix OffscreenWorldView::calculateViewMatrix(CameraComponent* mainViewCamera)
{
	return Matrix::makeReflection(Plane(Vector3::UnitY)) * mainViewCamera->getViewMatrix();
}

//------------------------------------------------------------------------------
Matrix OffscreenWorldView::calculateProjectionMatrix(CameraComponent* mainViewCamera)
{
	return mainViewCamera->getProjectionMatrix();
}

//------------------------------------------------------------------------------
void OffscreenWorldView::renderWorld(World* world, CameraComponent* mainViewCamera, RenderView* mainRenderView)
{
	m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(mainViewCamera) + 1;
	m_cameraInfo.viewPixelSize = mainRenderView->getViewSize();
	m_cameraInfo.viewPosition = mainViewCamera->getTransform()->getWorldMatrix().getPosition();
	m_cameraInfo.viewMatrix = calculateViewMatrix(mainViewCamera);
	m_cameraInfo.projMatrix = calculateProjectionMatrix(mainViewCamera);
	m_cameraInfo.viewProjMatrix = m_cameraInfo.viewMatrix * m_cameraInfo.projMatrix;
	m_cameraInfo.viewFrustum = mainViewCamera->getViewFrustum();	// TODO: この View 独自処理にしたい
	m_cameraInfo.zSortDistanceBase = mainViewCamera->getZSortDistanceBase();



	// TODO: tmp
	m_renderer->setCurrentCamera(mainViewCamera);



	m_renderer->beginMakeElements();



	RenderTargetTexture* backbuffer = m_renderer->getRenderTarget(0);

	if (m_renderTarget == nullptr)
	{
		m_renderTarget = newObject<RenderTargetTexture>(SizeI::fromFloatSize(mainRenderView->getViewSize()), 1, TextureFormat::R8G8B8X8);
	}
	//else if (m_renderTarget->GetSize() != backbuffer->GetSize())
	//{
	//	LN_NOTIMPLEMENTED();
	//}

	// TODO: 深度バッファやクリア方法の指定
	m_renderer->setRenderTarget(0, m_renderTarget);
	//g->setDepthBuffer(m_depthBuffer);
	m_renderer->clear(ClearFlags::All, Color::White, 1.0f, 0);
	

	world->render(m_renderer, mainViewCamera, WorldDebugDrawFlags::None, this);	// TODO: debugdraw の指定


	// 戻す
	m_renderer->setRenderTarget(0, backbuffer);




	// user override
	//OnUpdateRenderViewPoint(m_renderView);

	DrawList* r = world->getRenderer();
	r->renderSubView(this);
}

//------------------------------------------------------------------------------
bool OffscreenWorldView::filterRenderObject(VisualComponent* renderObject)
{
	auto* filterInfo = updateRenderObjectFilterInfo(renderObject);
	return filterInfo->effectGroup >= 0;
}

//------------------------------------------------------------------------------
detail::OffscreenFilterInfo* OffscreenWorldView::updateRenderObjectFilterInfo(VisualComponent* renderObject)
{
	auto* filterInfo = renderObject->getOffscreenFilterInfo(m_id);

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
	setLayer(LayerMask::GetLayer(BuiltinLayers::Background));
	setOrderInLayer(SHRT_MIN);

	{

		auto shader = ln::Shader::create("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Sky.fx");
		m_skyMaterial = newObject<Material>();
		m_skyMaterial->setShader(shader);
	}
}

//------------------------------------------------------------------------------
void SkyComponent::onRender2(DrawList* renderer)
{
	{
		auto* cam = renderer->getCurrentCamera();
		Vector3 frustumRayTL = Vector3::normalize(cam->viewportToWorldPoint(Vector3(0, 0, 1)) - cam->viewportToWorldPoint(Vector3(0, 0, 0)));
		Vector3 frustumRayTR = Vector3::normalize(cam->viewportToWorldPoint(Vector3(640, 0, 1)) - cam->viewportToWorldPoint(Vector3(640, 0, 0)));
		Vector3 frustumRayBL = Vector3::normalize(cam->viewportToWorldPoint(Vector3(0, 480, 1)) - cam->viewportToWorldPoint(Vector3(0, 480, 0)));
		m_skyMaterial->setVectorParameter("frustumRayTL", Vector4(frustumRayTL, 0));
		m_skyMaterial->setVectorParameter("frustumRayTR", Vector4(frustumRayTR, 0));
		m_skyMaterial->setVectorParameter("frustumRayBL", Vector4(frustumRayBL, 0));



		Vector3 cameraPos = Vector3(0, 997, 0);// = cam->getTransform()->position.Get();
											   //cameraPos.normalize();
		//Vector3 cameraPos = Vector3(0, 0, 10);
		//Vector3 lightPos = 1.0f * Vector3::normalize(1, -0, -1);//sunDirection.normalized();
		//Vector3 lightPos = Vector3::normalize(Vector3(0.3, -0.1, 1));
		Vector3 lightPos = Vector3::normalize(Vector3(0, 1, 0));

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


		m_skyMaterial->setVectorParameter("v3CameraPos", Vector4(cameraPos, 0));
		m_skyMaterial->setFloatParameter("fCameraHeight", fCameraHeight);
		m_skyMaterial->setVectorParameter("v3LightPos", Vector4(lightPos, 0));
		m_skyMaterial->setFloatParameter("fCameraHeight2", fCameraHeight2);
		m_skyMaterial->setVectorParameter("v3InvWavelength", Vector4(invWavelength, 0));
		m_skyMaterial->setFloatParameter("fScale", fScale);
		m_skyMaterial->setFloatParameter("fOuterRadius", fOuterRadius);
		m_skyMaterial->setFloatParameter("fOuterRadius2", fOuterRadius2);
		m_skyMaterial->setFloatParameter("fInnerRadius", fInnerRadius);
		m_skyMaterial->setFloatParameter("fInnerRadius2", fInnerRadius2);
		m_skyMaterial->setFloatParameter("fKrESun", fKrESun);
		m_skyMaterial->setFloatParameter("fKmESun", fKmESun);
		m_skyMaterial->setFloatParameter("fKr4PI", fKr4PI);
		m_skyMaterial->setFloatParameter("fKm4PI", fKm4PI);
		m_skyMaterial->setFloatParameter("fScaleDepth", fScaleDepth);
		m_skyMaterial->setFloatParameter("fScaleOverScaleDepth", fScaleOverScaleDepth);
		m_skyMaterial->setFloatParameter("g", g);
		m_skyMaterial->setFloatParameter("exposure", exposure);
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

		renderer->blit(nullptr, nullptr, m_skyMaterial);

		//renderer->setCullingMode(CullingMode::None);
		//Matrix tt;
		////tt.scale(-1, -1, -1);
		//renderer->drawSphere(100, 8, 8, Color::White, tt, m_skyMaterial);
		//renderer->setCullingMode(CullingMode::Back);
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
	m_offscreen = newObject<OffscreenWorldView>();

	// TODO: Remove
	detail::EngineDomain::getDefaultWorld3D()->addOffscreenWorldView(m_offscreen);
	m_offscreen->hideVisual(this);

	m_material = newObject<Material>();
	//m_material->setMaterialTexture(Texture2D::getBlackTexture());
	//m_material->setMaterialTexture(Texture2D::getWhiteTexture());
	//m_material->setShader(Shader::getBuiltinShader(BuiltinShader::Sprite));
	auto shader = ln::Shader::create("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Scene/Resource/Mirror.fx");
	m_material->setShader(shader);

	auto tex = ln::Texture2D::create("D:/Proj/LN/HC1/Assets/Data/waterbump.png");
	m_material->setTextureParameter(_T("xWaterBumpMap"), tex);

}
float g_time = 0;
//------------------------------------------------------------------------------
void MirrorComponent::onRender2(DrawList* renderer)
{




	g_time += 0.001;
	m_material->setMaterialTexture(m_offscreen->getRenderTarget());
	m_material->setVectorParameter("xCamPos", Vector4(renderer->getCurrentCamera()->getTransform()->position.get(), 1.0));
	m_material->setFloatParameter("time", g_time);

	// TODO: 法泉が入っていない？
	renderer->drawSquare(100, 100, 1, 1, Color::White, Matrix::Identity, m_material);
}


LN_NAMESPACE_END
