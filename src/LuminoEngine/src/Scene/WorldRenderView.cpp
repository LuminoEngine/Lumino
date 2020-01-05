
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"
#include "../Mesh/MeshGenerater.hpp"
#include "SceneManager.hpp"
#include "../Effect/EffectManager.hpp"  // TODO: test

namespace ln {

//==============================================================================
// WorldRenderView

WorldRenderView::WorldRenderView()
    : m_visibleGridPlane(true)
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
    setClearMode(RenderViewClearMode::ColorAndDepth);

    m_sceneRenderingPipeline = makeRef<detail::SceneRenderingPipeline>();
    m_sceneRenderingPipeline->init();
    m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();
    m_viewPoint = makeObject<RenderViewPoint>();

    //m_clearRenderPass = makeObject<RenderPass>();

    m_clearMaterial = makeObject<Material>();
    m_clearMaterial->setShader(detail::EngineDomain::sceneManager()->atmosphereShader());

    createGridPlane();

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


		detail::MeshGeneraterBuffer buffer;
		auto vb = meshResource->requestVertexData(MeshResource::VBG_Basic);
		IndexBufferFormat fmt;
		auto ib = meshResource->requestIndexData(&fmt);
		buffer.setBuffer((Vertex*)vb, ib, fmt, 0);
		buffer.generate(&gen);


		auto meshContainer = makeObject<MeshContainer>();
		meshContainer->setMeshResource(meshResource);

		m_skyProjectionPlane = makeObject<StaticMeshModel>();
		m_skyProjectionPlane->addMeshContainer(meshContainer);

		m_skyProjectionPlane->addMaterial(m_clearMaterial);
	}

    m_transformControls = makeObject<TransformControls>();
}

void WorldRenderView::setTargetWorld(World* world)
{
    m_targetWorld = world;

    m_drawElementListCollector->addDrawElementList(/*RendringPhase::Default, */m_targetWorld->m_renderingContext->m_elementList);

    addDrawElementListManager(m_drawElementListCollector);
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


void WorldRenderView::render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget)
{
	if (m_camera)
	{
        //FrameBuffer fb;
        //fb.renderTarget[0] = graphicsContext->renderPass()->renderTarget(0);
        //fb.depthBuffer = graphicsContext->renderPass()->depthBuffer();

        // TODO:
        detail::CameraInfo camera;
        {
            CameraComponent* cc = m_camera->cameraComponent();
            cc->updateMatrices();
            
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

            //Size size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());
            //Vector3 pos = Vector3(5, 5, -5);
            //camera.makePerspective(pos, Vector3::normalize(Vector3::Zero - pos), Math::PI / 3.0f, size, 0.1f, 100.0f);

        }


        //ClearInfo clearInfo;
        //clearInfo.color = backgroundColor();
        //clearInfo.depth = 1.0f;
        //clearInfo.stencil = 0x00;
        //if (clearMode() == RenderViewClearMode::ColorAndDepth) {
        //    clearInfo.flags = ClearFlags::All;
        //}
        //else {
        //    clearInfo.flags = ClearFlags::Depth;
        //}


        // DrawList 構築
        if (m_targetWorld) {
			detail::WorldSceneGraphRenderingContext* renderingContext = m_targetWorld->prepareRender(m_viewPoint);




			if (clearMode() == RenderViewClearMode::ColorAndDepth) {
				renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
			}
			else if (clearMode() == RenderViewClearMode::Sky) {

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

                auto v = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
                ViewFrustum vf(v * m_viewPoint->projMatrix);
                Vector3 points[8];
                vf.getCornerPoints(points);
				frustumRayTR = Vector3::normalize(points[7] - points[3]);



                //m_clearMaterial->setVector(u"frustumRayTL", Vector4(frustumRayTL, 0));
                //m_clearMaterial->setVector(u"frustumRayTR", Vector4(frustumRayTR, 0));
                //m_clearMaterial->setVector(u"frustumRayBL", Vector4(frustumRayBL, 0));


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

				Matrix rot =Matrix::makeInverse(m_viewPoint->viewMatrix);//m_viewPoint->viewMatrix;//  _viewPoint->worldMatrix;// //// m_viewPoint->worldMatrix;//Matrix::makeLookAtLH
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
                m_clearMaterial->setMatrix(u"_localWorld", rot);
                m_clearMaterial->setMatrix(u"_scaleMatrix", ss);
    //            printf("----\n");
				//camera.viewDirection.print();
				//rot.front().print();
    //            p0.print();
    //            p1.print();
    //            p2.print();
    //            p3.print();
    //            printf("----\n");
                //lookAt.print();
                //rot.inverse();
                renderingContext->pushState();
                renderingContext->setTransfrom(mm);
                renderingContext->setDepthWriteEnabled(false);
                renderingContext->setMaterial(m_clearMaterial);
                //renderingContext->drawScreenRectangle();
				renderingContext->drawMesh(m_skyProjectionPlane->meshContainers()[0]->meshResource(), 0);
                renderingContext->popState();
			}

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
            //renderingContext->drawText(u"Lumino", Font::create(), Color::Blue);
            //renderingContext->popState();


            adjustGridPlane(m_viewPoint->viewFrustum, this);
            renderGridPlane(renderingContext, this);

            m_transformControls->setViewInfo(m_viewPoint->viewPosition, m_viewPoint->viewMatrix, m_viewPoint->projMatrix, m_viewPoint->viewPixelSize);
            m_transformControls->onRender(renderingContext);

            detail::EngineDomain::effectManager()->testDraw(renderingContext);
            //detail::EngineDomain::effectManager()->testDraw2(graphicsContext);
        }



		m_sceneRenderingPipeline->render(graphicsContext, renderTarget/*, clearInfo*/, &camera, &elementListManagers());
	}
}

void WorldRenderView::createGridPlane()
{
    // 適当な四角形メッシュ
    auto meshResource = makeObject<MeshResource>();
    meshResource->resizeVertexBuffer(4);
    meshResource->resizeIndexBuffer(6);
    meshResource->setIndex(0, 0);
    meshResource->setIndex(1, 1);
    meshResource->setIndex(2, 2);
    meshResource->setIndex(3, 2);
    meshResource->setIndex(4, 1);
    meshResource->setIndex(5, 3);
    meshResource->addSection(0, 2, 0);

    auto meshContainer = makeObject<MeshContainer>();
    meshContainer->setMeshResource(meshResource);

    m_gridPlane = makeObject<StaticMeshModel>();
    m_gridPlane->addMeshContainer(meshContainer);

#if 1
	static const unsigned char data[] =
	{
#include "../Rendering/Resource/InfinitePlaneGrid.lcfx.inl"
	};
	static const size_t size = LN_ARRAY_SIZE_OF(data);
	MemoryStream stream(data, size);
	auto shader = makeObject<Shader>(u"InfinitePlaneGrid", &stream);
#else
    auto shader = Shader::create(u"D:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/InfinitePlaneGrid.fx");
#endif
    auto material = makeObject<Material>();
    material->setShader(shader);
    m_gridPlane->addMaterial(material);

    // 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
    SizeI gridTexSize(512, 512);
    auto gridTex = makeObject<Texture2D>(gridTexSize.width, gridTexSize.height, TextureFormat::RGBA8);
	gridTex->setMipmapEnabled(true);
	gridTex->setResourceUsage(GraphicsResourceUsage::Dynamic);
    for (int x = 0; x < gridTexSize.width; ++x)
    {
        gridTex->setPixel(x, 0, Color(0, 0, 0, 0.5));
        gridTex->setPixel(x, gridTexSize.width - 1, Color(0, 0, 0, 0.5));
    }
    for (int y = 0; y < gridTexSize.height; ++y)
    {
        gridTex->setPixel(0, y, Color(0, 0, 0, 0.5));
        gridTex->setPixel(gridTexSize.height - 1, y, Color(0, 0, 0, 0.5));
    }
    //gridTex->clear(Color::Green);
    material->setMainTexture(gridTex);
    material->setBlendMode(BlendMode::Alpha);
    material->setDepthWriteEnabled(false);

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


        renderingContext->pushState();

        

        renderingContext->setRenderPhase(RendringPhase::BeforeTransparencies);
        //renderingContext->setBlendMode(BlendMode::Alpha);
        //renderingContext->setDepthWriteEnabled(false);
        renderingContext->setMaterial(m_gridPlane->materials()[0]);
        renderingContext->drawMesh(m_gridPlane->meshContainers()[0]->meshResource(), 0);
        //renderingContext->setRenderPhase(RendringPhase::Default);

        renderingContext->popState();

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

        MeshResource* mesh = m_gridPlane->meshContainers().front()->meshResource();
        mesh->setVertex(0, Vertex{ Vector3(minPos.x, 0, maxPos.z), Vector3::UnitY, Vector2(-1.0f, 1.0f), Color::White });
        mesh->setVertex(1, Vertex{ Vector3(maxPos.x, 0, maxPos.z), Vector3::UnitY, Vector2(1.0f, 1.0f), Color::White });
        mesh->setVertex(2, Vertex{ Vector3(minPos.x, 0, minPos.z), Vector3::UnitY, Vector2(-1.0f, -1.0f), Color::White });
        mesh->setVertex(3, Vertex{ Vector3(maxPos.x, 0, minPos.z), Vector3::UnitY, Vector2(1.0f, -1.0f), Color::White });

        //mesh->setVertex(0, Vertex{ Vector3(-1, 1, 1), Vector3::UnitY, Vector2(-1.0f, 1.0f), Color::White });
        //mesh->setVertex(1, Vertex{ Vector3(1, 1, 1), Vector3::UnitY, Vector2(1.0f, 1.0f), Color::White });
        //mesh->setVertex(2, Vertex{ Vector3(-1, -1, 1), Vector3::UnitY, Vector2(-1.0f, -1.0f), Color::White });
        //mesh->setVertex(3, Vertex{ Vector3(1, -1, 1), Vector3::UnitY, Vector2(1.0f, -1.0f), Color::White });
    }
}

void WorldRenderView::onRoutedEvent(UIEventArgs* e)
{
    if (m_transformControls)
    {
        m_transformControls->onRoutedEvent(e);
    }

    RenderView::onRoutedEvent(e);
}

} // namespace ln

