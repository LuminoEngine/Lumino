
#include "Internal.hpp"
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

namespace ln {

//==============================================================================
// WorldRenderView

WorldRenderView::WorldRenderView()
    : m_visibleGridPlane(false)
{
}

WorldRenderView::~WorldRenderView()
{
}

void WorldRenderView::init()
{
    RenderView::init();
    m_sceneRenderingPipeline = makeRef<detail::SceneRenderingPipeline>();
    m_sceneRenderingPipeline->init();
    m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();
    m_viewPoint = newObject<RenderViewPoint>();

    createGridPlane();
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

void WorldRenderView::render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, DepthBuffer* depthbuffer)
{
	if (m_camera)
	{
        FrameBuffer fb;
        fb.renderTarget[0] = renderTarget;
        fb.depthBuffer = depthbuffer;

        // TODO:
        detail::CameraInfo camera;
        {
            CameraComponent* cc = m_camera->cameraComponent();
            cc->updateMatrices();
            
            m_viewPoint->worldMatrix = m_camera->worldMatrix();
            m_viewPoint->viewPixelSize = camera.viewPixelSize = Size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());	// TODO: 必要？
            m_viewPoint->viewPosition = camera.viewPosition = m_camera->position();
            m_viewPoint->viewDirection = camera.viewDirection = cc->getDirectionInternal().xyz();
            m_viewPoint->viewMatrix = camera.viewMatrix = cc->getViewMatrix();
            m_viewPoint->projMatrix = camera.projMatrix = cc->getProjectionMatrix();
            m_viewPoint->viewProjMatrix = camera.viewProjMatrix = cc->getViewProjectionMatrix();
            m_viewPoint->viewFrustum = camera.viewFrustum = cc->getViewFrustum();

            m_viewPoint->nearClip = camera.nearClip = cc->getNearClip();
            m_viewPoint->farClip = camera.farClip = cc->getFarClip();

            //Size size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());
            //Vector3 pos = Vector3(5, 5, -5);
            //camera.makePerspective(pos, Vector3::normalize(Vector3::Zero - pos), Math::PI / 3.0f, size, 0.1f, 100.0f);

        }




        // DrawList 構築
        if (m_targetWorld) {
			detail::WorldSceneGraphRenderingContext* renderingContext = m_targetWorld->prepareRender(m_viewPoint);




			if (clearMode() == RenderViewClearMode::ColorAndDepth) {
				renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
			}

            m_targetWorld->renderObjects();

            if (m_targetWorld->physicsWorld()) {
                m_targetWorld->physicsWorld()->renderDebug(renderingContext);
            }
            if (m_targetWorld->physicsWorld2D()) {
                m_targetWorld->physicsWorld2D()->renderDebug(renderingContext);
            }

            // test
            //renderingContext->pushState();
            //renderingContext->setBlendMode(BlendMode::Alpha);
            //renderingContext->drawText(u"Lumino", Font::create(), Color::Blue);
            //renderingContext->popState();


            adjustGridPlane(m_viewPoint->viewFrustum, this);
            renderGridPlane(renderingContext, this);

        }



		m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
	}
}

void WorldRenderView::createGridPlane()
{
    // 適当な四角形メッシュ
    auto meshResource = newObject<MeshResource>();
    meshResource->resizeVertexBuffer(4);
    meshResource->resizeIndexBuffer(6);
    meshResource->setIndex(0, 0);
    meshResource->setIndex(1, 1);
    meshResource->setIndex(2, 2);
    meshResource->setIndex(3, 2);
    meshResource->setIndex(4, 1);
    meshResource->setIndex(5, 3);
    meshResource->addSection(0, 2, 0);

    auto meshContainer = newObject<MeshContainer>();
    meshContainer->setMeshResource(meshResource);

    m_gridPlane = newObject<StaticMeshModel>();
    m_gridPlane->addMeshContainer(meshContainer);

#if 1
	static const unsigned char data[] =
	{
#include "../Rendering/Resource/InfinitePlaneGrid.lcfx.inl"
	};
	static const size_t size = LN_ARRAY_SIZE_OF(data);
	MemoryStream stream(data, size);
	auto shader = newObject<Shader>(u"InfinitePlaneGrid", &stream);
#else
    auto shader = Shader::create(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/InfinitePlaneGrid.fx");
#endif
    auto material = newObject<Material>();
    material->setShader(shader);
    m_gridPlane->addMaterial(material);

    // 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
    SizeI gridTexSize(512, 512);
    auto gridTex = newObject<Texture2D>(gridTexSize.width, gridTexSize.height, TextureFormat::RGBA32, true, GraphicsResourceUsage::Dynamic);
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

    auto samplerState = newObject<SamplerState>();
    samplerState->setFilterMode(TextureFilterMode::Linear);
    gridTex->setSamplerState(samplerState);

    m_visibleGridPlane = true;  // TODO: test
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
    }
}

//void WorldRenderView::onRoutedEvent(UIEventArgs* e)
//{
//    if (m_camera)
//    {
//    }
//}

} // namespace ln

