
#include "Internal.hpp"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoFont/Font.hpp>
#include <LuminoEngine/Graphics/Shader.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include <LuminoEngine/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include "../Rendering/CommandListServer.hpp"
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderElement.hpp"
#include "../Rendering/RenderingPipeline.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "../Mesh/MeshGeneraters/MeshGenerater.hpp"
#include "../PostEffect/PostEffectRenderer.hpp"
#include "SceneManager.hpp"
#include "InternalSkyBox.hpp"
#include "../Effect/EffectManager.hpp"  // TODO: test

namespace ln {

//==============================================================================
// WorldRenderView

LN_OBJECT_IMPLEMENT(WorldRenderView, RenderView) {}

WorldRenderView::WorldRenderView()
    : m_visibleGridPlane(false)
    , m_physicsDebugDrawEnabled(false)
    , m_gizmoEnabled(false)
{
}

WorldRenderView::~WorldRenderView()
{
}

void WorldRenderView::init()
{
    RenderView::init();
    setClearMode(SceneClearMode::ColorAndDepth);

    m_sceneRenderingPipeline = makeRef<detail::SceneRenderingPipeline>();
    m_sceneRenderingPipeline->init();
    m_viewPoint = makeObject<RenderViewPoint>();

    //m_clearRenderPass = makeObject<RenderPass>();

    m_clearMaterial = makeObject<Material>();
    m_clearMaterial->setShader(detail::EngineDomain::sceneManager()->atmosphereShader());

    createGridPlane();

#if 0
	{
		detail::PlaneMeshGenerater2 gen;
		gen.size = Vector2(2, 2);
		gen.sliceH = 50;
		gen.sliceV = 50;
		gen.direction = detail::PlaneMeshDirection::ZMinus;


		auto meshResource = makeObject<MeshResource>();
		meshResource->resizeVertexBuffer(gen.vertexCount());
		meshResource->resizeIndexBuffer(gen.indexCount());
		meshResource->addSection(0, gen.indexCount() / 3, 0);


		detail::MeshGeneraterBuffer buffer(nullptr);
		auto vb = meshResource->requestVertexData(MeshResource::VBG_Basic);
		IndexBufferFormat fmt;
		auto ib = meshResource->requestIndexData(&fmt);
		buffer.setBuffer((Vertex*)vb, ib, fmt, 0);
		buffer.generate(&gen);

		auto meshContainer = makeObject<MeshContainer>();
		meshContainer->setMeshResource(meshResource);

		m_skyProjectionPlane = makeObject<MeshModel>();
		m_skyProjectionPlane->addMeshContainer(meshContainer);

		m_skyProjectionPlane->addMaterial(m_clearMaterial);
    }
#endif

    // TODO: 遅延作成
	m_internalSkyBox = makeObject<detail::InternalSkyBox>();
    m_internalSkyDome = makeRef<detail::InternalSkyDome>();
    if (!m_internalSkyDome->init()) {
        LN_ERROR();
        return;
    }

    m_finishingProcess = makeObject<FilmicPostEffect>();

    m_transformControls = makeObject<TransformControls>();
}

void WorldRenderView::setTargetWorld(World* world)
{
    m_targetWorld = world;
}

void WorldRenderView::setCamera(Camera* camera)
{
    if (camera) {
        if (LN_REQUIRE(camera->cameraComponent())) return;
        if (LN_REQUIRE(!camera->cameraComponent()->ownerRenderView())) return;
    }

    if (m_camera) {
        m_camera->cameraComponent()->setOwnerRenderView(nullptr);
    }

	m_camera = camera;

    if (m_camera) {
        m_camera->cameraComponent()->setOwnerRenderView(this);
    }
}

void WorldRenderView::addPostEffect(PostEffect* effect)
{
    auto* presenter = acquirePostEffectPresenter();
    presenter->addPostEffect(effect);
}

void WorldRenderView::removePostEffect(PostEffect* effect)
{
	if (m_imageEffectRenderer) {
		m_imageEffectRenderer->removePostEffect(effect);
	}
}

void WorldRenderView::render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget)
{
	if (m_camera)
	{
        {
            CameraComponent* cc = m_camera->cameraComponent();
            cc->updateMatrices();
            
            detail::CameraInfo camera;
            m_viewPoint->worldMatrix = m_camera->worldMatrix();
            m_viewPoint->viewPixelSize = camera.viewPixelSize = Size(renderTarget->width(), renderTarget->height());	// TODO: 必要？
            m_viewPoint->viewPosition = camera.viewPosition = m_camera->position();
            m_viewPoint->viewDirection = camera.viewDirection = cc->getDirectionInternal().xyz();
            m_viewPoint->viewMatrix = camera.viewMatrix = cc->getViewMatrix();
            m_viewPoint->projMatrix = camera.projMatrix = cc->getProjectionMatrix();
            m_viewPoint->viewProjMatrix = camera.viewProjMatrix = cc->getViewProjectionMatrix();
            m_viewPoint->viewFrustum = camera.viewFrustum = cc->getViewFrustum();

            m_viewPoint->fovY = m_camera->fov();
            m_viewPoint->nearClip = camera.nearClip = cc->getNearClip();
            m_viewPoint->farClip = camera.farClip = cc->getFarClip();

            makeViewProjections(camera, 1.0);   // TODO: dpiscale
        }


        ClearInfo clearInfo;
        clearInfo.color = backgroundColor();
        clearInfo.depth = 1.0f;
        clearInfo.stencil = 0x00;
        if (clearMode() == SceneClearMode::ColorAndDepth) {
            clearInfo.flags = ClearFlags::All;
        }
        else {
            clearInfo.flags = ClearFlags::Depth | ClearFlags::Stencil;
        }

        detail::SceneGlobalRenderParams sceneGlobalRenderParams = m_targetWorld->m_combinedSceneGlobalRenderParams;


        m_sceneRenderingPipeline->prepare(renderTarget);


        if (m_hdrEnabled) {
            m_hdrRenderTarget = RenderTargetTexture::realloc(m_hdrRenderTarget, m_viewPoint->viewPixelSize.width, m_viewPoint->viewPixelSize.height, TextureFormat::RGBA32F, false, SamplerState::pointClamp());
            //m_hdrRenderTarget->m_msaa = true;
        }


        // DrawList 構築
        if (m_targetWorld) {
			detail::WorldSceneGraphRenderingContext* renderingContext = m_targetWorld->prepareRender(m_viewPoint);
            renderingContext->baseRenderView = this;
            renderingContext->currentRenderView = this;
            renderingContext->clearPostEffects();


			if (clearMode() == SceneClearMode::ColorAndDepth) {
				//renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
			}
			else if (clearMode() == SceneClearMode::Sky) {
				//renderingContext->clear(ClearFlags::Depth | ClearFlags::Stencil, Color(), 1.0f, 0x00);

                if (m_targetWorld->mainLight()) {
                    m_internalSkyBox->setLightDirection(-Vector3::normalize(m_targetWorld->mainLight()->worldMatrix().front()));

                }

                m_internalSkyBox->render(renderingContext, m_viewPoint);

			}
			else if (clearMode() == SceneClearMode::Sky0) {

                //renderingContext->setBaseTransfrom(Matrix::Identity);
                //renderingContext->setTransfrom(Matrix::Identity);

                float viewWidth = m_viewPoint->viewPixelSize.width;
                float viewHeight = m_viewPoint->viewPixelSize.height;


                //Matrix ref;
                ln::Matrix refVP = m_viewPoint->viewProjMatrix;
                auto vtow = [refVP, viewWidth, viewHeight](const ln::Vector3& pos) { return ln::Vector3::unproject(pos, refVP, 0, 0, viewWidth, viewHeight, 0.3f, 1000); };


                Vector3 frustumRayTL;// = Vector3::normalize(vtow(Vector3(0, 0, 1)) - vtow(Vector3(0, 0, 0))/*cam->viewportToWorldPoint(Vector3(0, 0, 0))*/);
                Vector3 frustumRayTR;// = Vector3::normalize(vtow(Vector3(viewWidth, 0, 1)) - vtow(Vector3(viewWidth, 0, 0))/*cam->viewportToWorldPoint(Vector3(640, 0, 0))*/);
                Vector3 frustumRayBL;// = Vector3::normalize(vtow(Vector3(0, viewHeight, 1)) - vtow(Vector3(0, viewHeight, 0))/*cam->viewportToWorldPoint(Vector3(0, 480, 0))*/);
                //frustumRayTL = 
                //frustumRayTL = m_camera->screenToWorldRay(Vector2(0, 0)).direction;
                //frustumRayTR = m_camera->screenToWorldRay(Vector2(viewWidth, 0)).direction;
                //frustumRayBL = m_camera->screenToWorldRay(Vector2(0, viewHeight)).direction;
#ifdef LN_COORD_RH
                auto v = Matrix::makeLookAtRH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
#else
                auto v = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
#endif
                ViewFrustum vf(v * m_viewPoint->projMatrix);
                Vector3 points[8];
                vf.getCornerPoints(points);
				frustumRayTR = Vector3::normalize(points[7] - points[3]);



                //m_clearMaterial->setVector(_TT("frustumRayTL", Vector4(frustumRayTL, 0));
                //m_clearMaterial->setVector(_TT("frustumRayTR", Vector4(frustumRayTR, 0));
                //m_clearMaterial->setVector(_TT("frustumRayBL", Vector4(frustumRayBL, 0));


                static const float EARTH_RADIUS = 6370997.0f;
                static const float EARTH_ATMOSPHERE_RADIUS = EARTH_RADIUS * 1.025f;

                //Vector3 cameraPos = Vector3(0, EARTH_RADIUS* 1.01f, 0);// = cam->getTransform()->position.Get();
                //cameraPos.normalize();
                //Vector3 cameraPos = Vector3(0, 0, 0);
                Vector3 cameraPos = m_viewPoint->viewPosition;
                ;// cameraPos.y += EARTH_RADIUS;
                //Vector3 lightPos = 1.0f * Vector3::normalize(1, -0, -1);//sunDirection.normalized();
                //Vector3 lightPos = Vector3::normalize(Vector3(0.3, -0.1, 1));
                //Vector3 lightPos = Vector3::normalize(Vector3(0, 1, 0));
                //Vector3 lightPos = Vector3::normalize(Vector3(0, 0.15, 1));
                Vector3 lightPos = Vector3::normalize(Vector3(0, 0.5, 1));

                float fCameraHeight = cameraPos.length();
                float fCameraHeight2 = fCameraHeight * fCameraHeight;

                const float red = 0.680f;
                const float green = 0.550f;
                const float blue = 0.440f;
                const auto invWav = [](float waveLength) { return 1.0f / pow(waveLength, 4.0f); };

                Vector3 invWavelength = Vector3(invWav(red), invWav(green), invWav(blue));



                float fInnerRadius = EARTH_RADIUS;
                float fInnerRadius2 = fInnerRadius * fInnerRadius;

                float fOuterRadius = EARTH_ATMOSPHERE_RADIUS;
                float fOuterRadius2 = fOuterRadius * fOuterRadius;

                float fScale = 1.0f / (fOuterRadius - fInnerRadius);

                const float Kr = 0.0025f - 0.00015f;//static_cast<float>(gui.slider(L"Kr").value);
                const float Km = 0.0010f + 0.0015f;// static_cast<float>(gui.slider(L"Km").value);
                const float Esun = 1300.0f;

                float fKrESun = Kr * Esun;
                float fKmESun = Km * Esun;
                float fKr4PI = Kr * 4.0f* Math::PI;
                float fKm4PI = Km * 4.0f*Math::PI;

                const float rayleighScaleDepth = 0.25f;
                float fScaleDepth = rayleighScaleDepth;
                float fScaleOverScaleDepth = (1.0f / (fOuterRadius - fInnerRadius)) / rayleighScaleDepth;
                float g = -0.999f;
                float exposure = 0.05 + 0.03;// static_cast<float>(gui.slider(L"Exposure").value);


                m_clearMaterial->setVector(_T("v3CameraPos"), Vector4(cameraPos, 0));
                //m_clearMaterial->setFloat(_T("fCameraHeight"), fCameraHeight);
                //m_clearMaterial->setVector(_T("v3LightPos"), Vector4(lightPos, 0));
                //m_clearMaterial->setFloat(_T("fCameraHeight2"), fCameraHeight2);
                //m_clearMaterial->setVector(_T("v3InvWavelength"), Vector4(invWavelength, 0));
                //m_clearMaterial->setFloat(_T("fScale"), fScale);
                //m_clearMaterial->setFloat(_T("fOuterRadius"), fOuterRadius);
                //m_clearMaterial->setFloat(_T("fOuterRadius2"), fOuterRadius2);
                //m_clearMaterial->setFloat(_T("fInnerRadius"), fInnerRadius);
                //m_clearMaterial->setFloat(_T("fInnerRadius2"), fInnerRadius2);
                //m_clearMaterial->setFloat(_T("fKrESun"), fKrESun);
                //m_clearMaterial->setFloat(_T("fKmESun"), fKmESun);
                //m_clearMaterial->setFloat(_T("fKr4PI"), fKr4PI);
                //m_clearMaterial->setFloat(_T("fKm4PI"), fKm4PI);
                //m_clearMaterial->setFloat(_T("fScaleDepth"), fScaleDepth);
                //m_clearMaterial->setFloat(_T("fScaleOverScaleDepth"), fScaleOverScaleDepth);
                //m_clearMaterial->setFloat(_T("g"), g);
                //m_clearMaterial->setFloat(_T("exposure"), exposure);

				Matrix rot =Matrix::makeInverse(m_viewPoint->viewMatrix);
                rot.m41 = rot.m42 = rot.m43 = 0.0f;
                Matrix ss = Matrix::makeScaling(frustumRayTR.x, frustumRayTR.y, frustumRayTR.z);
				Matrix mm = rot * ss;// rot;
                auto p0r = Vector3::transformCoord(Vector3(-1, 1, 1), mm);
                auto p1r = Vector3::transformCoord(Vector3(1, 1, 1), mm);
                auto p2r = Vector3::transformCoord(Vector3(1, -1, 1), mm);
                auto p3r = Vector3::transformCoord(Vector3(-1, -1, 1), mm);
                auto p0 = Vector3::normalize(p0r);
                auto p1 = Vector3::normalize(p1r);
                auto p2 = Vector3::normalize(p2r);
				auto p3 = Vector3::normalize(p3r);
                m_clearMaterial->setMatrix(_TT("_localWorld"), rot);
                m_clearMaterial->setMatrix(_TT("_scaleMatrix"), ss);

                renderingContext->pushState();
                renderingContext->setTransfrom(mm);
                renderingContext->setDepthWriteEnabled(false);
                renderingContext->setMaterial(m_clearMaterial);
                //renderingContext->drawScreenRectangle();
#if 1
                LN_NOTIMPLEMENTED();
#else
				renderingContext->drawMesh(m_skyProjectionPlane->meshContainers()[0]->meshResource(), 0);
#endif
                renderingContext->popState();
			}
            else if (clearMode() == SceneClearMode::SkyDome) {
                m_internalSkyDome->setSkyColor(sceneGlobalRenderParams.skydomeSkyColor);
                m_internalSkyDome->setHorizonColor(sceneGlobalRenderParams.skydomeHorizonColor);
                m_internalSkyDome->setCloudColor(sceneGlobalRenderParams.skydomeCloudColor);
                m_internalSkyDome->setOverlayColor(sceneGlobalRenderParams.skydomeOverlayColor);

                m_internalSkyDome->update(0.016);   // TODO:
                m_internalSkyDome->render(renderingContext, m_viewPoint);

                //sceneGlobalRenderParams.fogColor = m_internalSkyDome->sceneColor();
            }


            m_targetWorld->prepareRender(this);

            for (auto& offscreen : m_targetWorld->collectedOffscreenRenderViews()) {
                renderingContext->currentRenderView = offscreen;
                offscreen->render(graphicsContext, m_targetWorld);
            }
            renderingContext->currentRenderView = this;

            m_targetWorld->renderObjects();

            if (m_physicsDebugDrawEnabled) {
                if (m_targetWorld->physicsWorld()) {
                    m_targetWorld->physicsWorld()->renderDebug(renderingContext);
                }
                if (m_targetWorld->physicsWorld2D()) {
                    m_targetWorld->physicsWorld2D()->renderDebug(renderingContext);
                }
            }

            if (m_gizmoEnabled) {
                m_targetWorld->renderGizmos(renderingContext);
            }

            // test
            //renderingContext->pushState();
            //renderingContext->setBlendMode(BlendMode::Alpha);
            //renderingContext->drawText(_TT("Lumino", Font::create(), Color::Blue);
            //renderingContext->popState();


            adjustGridPlane(m_viewPoint->viewFrustum, this);
            renderGridPlane(renderingContext, this);

            m_transformControls->setViewInfo(m_viewPoint->viewPosition, m_viewPoint->viewMatrix, m_viewPoint->projMatrix, m_viewPoint->viewPixelSize);
            m_transformControls->onRender(renderingContext);

            // TODO: ここでやると Offscreen に書かれないので別の場所の方がいいかも
            detail::EngineDomain::effectManager()->testDraw(renderingContext);
            //detail::EngineDomain::effectManager()->testDraw2(graphicsContext);


            {
                RenderView::m_sceneRenderingPipeline = m_sceneRenderingPipeline;

                if (!renderingContext->imageEffects().isEmpty()) {
                    acquirePostEffectPresenter();
                }

                if (m_imageEffectRenderer) {
                    m_imageEffectRenderer->applyInScenePostEffects(renderingContext->imageEffects());

                    RenderTargetTexture* actualInput = (m_hdrRenderTarget) ? m_hdrRenderTarget.get() : renderTarget;
                    m_imageEffectRenderer->render(renderingContext, actualInput, renderTarget);
                }

                RenderView::m_sceneRenderingPipeline = nullptr;
            }
        }

        RenderTargetTexture* actualTarget = (m_hdrRenderTarget) ? m_hdrRenderTarget.get() : renderTarget;
        m_sceneRenderingPipeline->render(
            graphicsContext, m_targetWorld->m_renderingContext,
            actualTarget, clearInfo, this,
            m_targetWorld->m_renderingContext->commandList()->elementList(),
            m_targetWorld->m_renderingContext->commandListServer(),
            &sceneGlobalRenderParams);
        
		//graphicsContext->resetState();

#if 0
        auto bitmap = detail::TextureInternal::readData(m_sceneRenderingPipeline->objectIdBuffer(), graphicsContext);
        auto d = (uint32_t*)bitmap->data();
        printf("%u\n", d[0]);
#endif
	}
}

WorldObject* WorldRenderView::findObjectInPoint(int x, int y)
{
    auto bitmap = detail::TextureInternal::readData(m_sceneRenderingPipeline->objectIdBuffer(), nullptr);
    auto data = (uint32_t*)bitmap->data();
    int id = data[bitmap->width() * y + x];
    if (id > 0) {
        return m_targetWorld->findObjectById(id);
    }
    else {
        return nullptr;
    }
}

detail::PostEffectRenderer* WorldRenderView::acquirePostEffectPresenter()
{
    if (!m_imageEffectRenderer) {
        m_imageEffectRenderer = makeRef<detail::PostEffectRenderer>();
    }
    return m_imageEffectRenderer;
}

void WorldRenderView::createGridPlane()
{
    // 適当な四角形メッシュ
    m_gridPlaneMesh = makeObject<MeshResource>();
    m_gridPlaneMesh->resizeVertexBuffer(4);
    m_gridPlaneMesh->resizeIndexBuffer(6);
    m_gridPlaneMesh->setIndex(0, 0);
    m_gridPlaneMesh->setIndex(1, 1);
    m_gridPlaneMesh->setIndex(2, 2);
    m_gridPlaneMesh->setIndex(3, 2);
    m_gridPlaneMesh->setIndex(4, 1);
    m_gridPlaneMesh->setIndex(5, 3);
    m_gridPlaneMesh->addSection(0, 2, 0);

    //auto meshContainer = makeObject<MeshContainer>();
    //meshContainer->setMeshResource(meshResource);

    //m_gridPlane = makeObject<MeshModel>();
    //m_gridPlane->addMeshContainer(meshContainer);

//#if 0
//	static const unsigned char data[] =
//	{
//#include "../Rendering/Resource/InfinitePlaneGrid.lcfx.inl"
//	};
//	static const size_t size = LN_ARRAY_SIZE_OF(data);
//	MemoryStream stream(data, size);
//	auto shader = makeObject<Shader>(_TT("InfinitePlaneGrid", &stream);
//#else
//    auto shader = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/InfinitePlaneGrid.fx");
//#endif
    auto shader = detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::InfinitePlaneGrid);

    m_gridPlaneMaterial = makeObject<Material>();
    m_gridPlaneMaterial->setShader(shader);
    //m_gridPlane->addMaterial(material);

    // 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
    SizeI gridTexSize(512, 512);
    auto gridTex = makeObject<Texture2D>(gridTexSize.width, gridTexSize.height, TextureFormat::RGBA8);
	gridTex->setMipmapEnabled(true);
	gridTex->setResourceUsage(GraphicsResourceUsage::Dynamic);
    for (int x = 0; x < gridTexSize.width; ++x)
    {
        gridTex->setPixel(x, 0, Color(0, 0, 0, 0.25));
        gridTex->setPixel(x, gridTexSize.width - 1, Color(0, 0, 0, 0.5));
    }
    for (int y = 0; y < gridTexSize.height; ++y)
    {
        gridTex->setPixel(0, y, Color(0, 0, 0, 0.25));
        gridTex->setPixel(gridTexSize.height - 1, y, Color(0, 0, 0, 0.5));
    }
    //gridTex->clear(Color::Green);
    m_gridPlaneMaterial->setMainTexture(gridTex);
    m_gridPlaneMaterial->setBlendMode(BlendMode::Alpha);
    m_gridPlaneMaterial->setDepthWriteEnabled(false);

    auto samplerState = makeObject<SamplerState>();
    samplerState->setFilterMode(TextureFilterMode::Linear);
	samplerState->setAnisotropyEnabled(true);
    gridTex->setSamplerState(samplerState);

    //m_visibleGridPlane = true;  // TODO: test
}

void WorldRenderView::renderGridPlane(RenderingContext* renderingContext, RenderView* renderView)
{
    if (m_visibleGridPlane)
    {

        //renderingContext->pushState();
        //renderingContext->drawLine(Vector3(0, 0, 0), Color::Red, Vector3(-1, 1, 1), Color::Red);
        //renderingContext->popState();

        CommandList* commandList = renderingContext->getCommandList(RenderPart::Gizmo);

        commandList->pushState();

        

        //commandList->setRenderPhase(RenderPart::Gizmo);
        //commandList->setBlendMode(BlendMode::Alpha);
        //commandList->setDepthWriteEnabled(false);
        commandList->setMaterial(m_gridPlaneMaterial);
        commandList->drawMesh(m_gridPlaneMesh, 0);
        //commandList->setRenderPhase(RenderPart::Default);

        commandList->setMaterial(nullptr);
        commandList->drawLine(Vector3(0, 0, 0), Color::Red, Vector3(10, 0, 0), Color::Red);
        commandList->drawLine(Vector3(0, 0, 0), Color::Green, Vector3(0, 10, 0), Color::Green);
        commandList->drawLine(Vector3(0, 0, 0), Color::Blue, Vector3(0, 0, 10), Color::Blue);

        commandList->popState();

        //adjustGridPlane(renderView);
        //renderingContext->setTransform(Matrix::Identity);

        //DrawElementMetadata metadata;
        //metadata.priority = (int)DepthPriority::Foreground;
        //renderingContext->pushMetadata(&metadata);
        //renderingContext->drawMesh(m_gridPlane->getMeshResource(0), 0, m_gridPlane->getMaterial(0));
        //renderingContext->popMetadata();
    }
}

void WorldRenderView::adjustGridPlane(const ViewFrustum& viewFrustum, RenderView* renderView)
{
    if (m_visibleGridPlane)
    {
        /*
        カメラの視錐台と、グリッドを描画したい平面との衝突点から、四角形メッシュを作る。
        これで視界に映る平面全体をカバーするメッシュができる。
        あとはシェーダで、頂点の位置を利用してグリッドテクスチャをサンプリングする。
        */

        struct Line
        {
            Vector3	from;
            Vector3	to;
        };

        Vector3 points[8];
        viewFrustum.getCornerPoints(points);

        Line lines[12] =
        {
            // near
            { points[0], points[1] },
            { points[1], points[2] },
            { points[2], points[3] },
            { points[3], points[0] },
            // near - far
            { points[0], points[4] },
            { points[1], points[5] },
            { points[2], points[6] },
            { points[3], points[7] },
            // far
            { points[4], points[5] },
            { points[5], points[6] },
            { points[6], points[7] },
            { points[7], points[4] },
        };

        Plane plane(0, 1, 0, 0);
        List<Vector3> hits;
        for (Line& li : lines)
        {
            Vector3 pt;
            if (plane.intersects(li.from, li.to, &pt))
            {
                hits.add(pt);
            }
        }

        Vector3 minPos, maxPos;
        for (Vector3& p : hits)
        {
            minPos = Vector3::min(p, minPos);
            maxPos = Vector3::max(p, maxPos);
        }

        m_gridPlaneMesh->setVertex(0, Vertex{ Vector3(minPos.x, 0, maxPos.z), Vector3::UnitY, Vector2(-1.0f, 1.0f), Color::White });
        m_gridPlaneMesh->setVertex(1, Vertex{ Vector3(maxPos.x, 0, maxPos.z), Vector3::UnitY, Vector2(1.0f, 1.0f), Color::White });
        m_gridPlaneMesh->setVertex(2, Vertex{ Vector3(minPos.x, 0, minPos.z), Vector3::UnitY, Vector2(-1.0f, -1.0f), Color::White });
        m_gridPlaneMesh->setVertex(3, Vertex{ Vector3(maxPos.x, 0, minPos.z), Vector3::UnitY, Vector2(1.0f, -1.0f), Color::White });

        //mesh->setVertex(0, Vertex{ Vector3(-1, 1, 1), Vector3::UnitY, Vector2(-1.0f, 1.0f), Color::White });
        //mesh->setVertex(1, Vertex{ Vector3(1, 1, 1), Vector3::UnitY, Vector2(1.0f, 1.0f), Color::White });
        //mesh->setVertex(2, Vertex{ Vector3(-1, -1, 1), Vector3::UnitY, Vector2(-1.0f, -1.0f), Color::White });
        //mesh->setVertex(3, Vertex{ Vector3(1, -1, 1), Vector3::UnitY, Vector2(1.0f, -1.0f), Color::White });
    }
}

void WorldRenderView::onUpdateFrame(float elapsedSeconds)
{
	if (m_imageEffectRenderer) {
		m_imageEffectRenderer->updateFrame(elapsedSeconds);
	}
}

void WorldRenderView::onRoutedEvent(UIEventArgs* e)
{
    if (m_transformControls) {
        m_transformControls->onRoutedEvent(e);
        if (e->handled) return;
    }

    RenderView::onRoutedEvent(e);
}

} // namespace ln

