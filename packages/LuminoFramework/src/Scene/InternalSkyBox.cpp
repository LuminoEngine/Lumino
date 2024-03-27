
#include "Internal.hpp"
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/GPU/SamplerState.hpp>
#include <LuminoEngine/GPU/Shader.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "SceneManager.hpp"
#include "InternalSkyBox.hpp"

namespace ln {
namespace detail {

//==============================================================================
// InternalSkyBox

InternalSkyBox::InternalSkyBox() {
}

void InternalSkyBox::init() {
    Object::init();
    auto shader = RenderingManager::instance()->builtinShader(BuiltinShader::SkyLowAltitudeOptimized);

    m_material = makeObject_deprecated<Material>();
    m_material->setShader(shader);
    m_material->setShadingModel(ShadingModel::Unlit);

    m_lightDirection = Vector3::UnitY;
}

void InternalSkyBox::setLightDirection(const Vector3& value) {
    m_lightDirection = value;
}

void InternalSkyBox::render(RenderingContext* context, const RenderViewPoint* viewPoint) {
    const float sunDistance = 400000;

    //m_material->setVector(_TT("_RayleighColorScale"), Vector4(0.1, 0.2, 0.6, 0.0));
    m_material->setVector(_TT("_RayleighColorScale"), Color(0.5, 0.2, 0.5).toVector4());
    //m_material->setFloat(_TT("turbidity"), 10.0f);
    m_material->setFloat(_TT("turbidity"), 1.0f);
    //m_material->setFloat(_TT("rayleigh"), 2.0f);
    m_material->setFloat(_TT("rayleigh"), 0.5f);
    m_material->setFloat(_TT("mieCoefficient"), 0.005f);
    m_material->setFloat(_TT("mieDirectionalG"), 0.8f);
    m_material->setFloat(_TT("luminance"), 1.0f);
    m_material->setVector(_TT("up"), Vector4(0, 1, 0, 0));
    m_material->setVector(_TT("sunPosition"), Vector4(m_lightDirection * sunDistance, 0));

    context->pushState();
    context->setBlendMode(BlendMode::Normal);
    context->setAdditionalElementFlags(detail::RenderDrawElementTypeFlags::BackgroundSky);
    //context->setRenderPhase(RenderPart::BackgroundSky);
    //context->setDepthTestEnabled(false);
    context->setDepthWriteEnabled(false);
    context->setBaseTransfrom(Matrix::makeTranslation(viewPoint->viewPosition));
    //context->setMaterial(m_material);
    context->setCullingMode(CullMode::Front);
    PrimitiveMeshRenderer::drawBox(context, m_material, Box(Vector3::Zero, 100));
    context->popState();
}

//==============================================================================
// InternalSkyDome

#include "SkyDomeMesh.inl"

InternalSkyDome::InternalSkyDome() {
}

bool InternalSkyDome::init() {
#if 0 // Dump
    m_model = MeshModel::load(_TT("Sphere.glb");
    auto mesh = m_model->meshContainers()[0]->mesh();
    {
        Vector3 m;
        for (int i = 0; i < mesh->vertexCount(); i++) {
            auto& v = mesh->vertex(i);
            m.x = std::max(m.x, std::abs(v.position.x));
            m.y = std::max(m.y, std::abs(v.position.y));
            m.z = std::max(m.z, std::abs(v.position.z));
        }
        float maxLen = std::max(m.x, std::max(m.y, m.z));

        StreamWriter writer(_TT("vertices.txt");
        for (int i = 0; i < mesh->vertexCount(); i++) {
            auto& v = mesh->vertex(i);
            writer.writeLineFormat(_TT("{{{{{0}, {1}, {2}}}, {{{3}, {4}, {5}}}, {{{6}, {7}}}}},",
                v.position.x / maxLen, v.position.y / maxLen, v.position.z / maxLen,
                v.normal.x, v.normal.y, v.normal.z,
                v.uv.x, v.uv.y);
        }

        StreamWriter writer2(_TT("indices.txt");
        for (int i = 0; i < mesh->indexCount() / 3; i++) {
            int index = i * 3;
            writer2.writeLineFormat(
                _TT("{0}, {1}, {2},",
                mesh->index(index + 0),
                mesh->index(index + 1),
                mesh->index(index + 2));
        }
    }
#endif
    {
        const int vertexCount = LN_ARRAY_SIZE_OF(s_skyDomeVertices);
        const int indexCount = LN_ARRAY_SIZE_OF(s_skyDomeIndices);
        auto mesh = makeObject_deprecated<MeshPrimitive>(
            vertexCount, indexCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);

        auto* vertices = static_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
        for (int i = 0; i < vertexCount; i++) {
            vertices[i].set(
                s_skyDomeVertices[i].pos,
                s_skyDomeVertices[i].normal,
                s_skyDomeVertices[i].uv,
                Color::White);
        }

        auto* indices = static_cast<uint16_t*>(mesh->acquireMappedIndexBuffer());
        memcpy(indices, s_skyDomeIndices, sizeof(uint16_t) * indexCount);

        mesh->addSection(0, indexCount / 3, 0, PrimitiveTopology::TriangleList);

        m_model = makeObject_deprecated<MeshModel>();
        auto node = m_model->addMeshContainerNode(mesh);
        m_model->addRootNode(0);

        m_material = makeObject_deprecated<Material>();
        m_model->addMaterial(m_material);
        m_model->updateNodeTransforms();
    }

    auto manager = detail::EngineDomain::sceneManager();

    auto samperState = ln::SamplerState::create(ln::TextureFilterMode::Linear, ln::TextureAddressMode::Repeat, true);

#if 1
    auto _mainCloudsTexture = manager->SkydomeCloudA;
    auto _secondCloudsTexture = manager->SkydomeCloudB;
    auto _detailCloudTexture = manager->SkydomeCloudC;
    auto _secondCloudPowerMap = manager->SkydomeCloudR;
#else
    auto _mainCloudsTexture = ln::Texture2D::load(Path(Assets::engineAssetsDirectory(), _TT("SkydomeCloudA.png")));
    auto _secondCloudsTexture = ln::Texture2D::load(Path(Assets::engineAssetsDirectory(), _TT("SkydomeCloudB.png")));
    auto _detailCloudTexture = ln::Texture2D::load(Path(Assets::engineAssetsDirectory(), _TT("SkydomeCloudC.png")));
    auto _secondCloudPowerMap = ln::Texture2D::load(Path(Assets::engineAssetsDirectory(), _TT("SkydomeCloudR.png")));
#endif
    _mainCloudsTexture->setSamplerState(samperState);
    _secondCloudsTexture->setSamplerState(samperState);
    _detailCloudTexture->setSamplerState(samperState);
    _secondCloudPowerMap->setSamplerState(samperState);

    m_material->setShader(manager->skydomeShader());
    m_material->setTexture(_TT("_thirdCloudTexture"), _mainCloudsTexture);
    m_material->setTexture(_TT("_detailCloudTexture"), _detailCloudTexture);
    m_material->setTexture(_TT("_secondCloudPowerMap"), _secondCloudPowerMap);
    m_material->setTexture(_TT("_mainCloudsTexture"), _mainCloudsTexture);
    m_material->setTexture(_TT("_secondCloudsTexture"), _secondCloudsTexture);
    m_material->setMainTexture(_secondCloudsTexture);

    {
        auto r = ln::KeyFrameAnimationCurve::create();
        r->addKeyFrame(0.0, 0.49134);
        r->addKeyFrame(5.0, 0.49134);
        r->addKeyFrame(6.5, 0.819287);
        r->addKeyFrame(9.086636, 0.507721);
        r->addKeyFrame(12.054127, 0.466585);
        r->addKeyFrame(16.886023, 0.50004);
        r->addKeyFrame(17.854126, 0.573377);
        r->addKeyFrame(18.687864, 0.454085);
        r->addKeyFrame(19.871872, 0.49134);
        r->addKeyFrame(24.0, 0.49134);
        m_backGroundSkyDomeColorR = r;

        auto g = ln::KeyFrameAnimationCurve::create();
        g->addKeyFrame(0.0, 0.369745);
        g->addKeyFrame(5.0, 0.369745);
        g->addKeyFrame(9.032509, 0.683189);
        g->addKeyFrame(16.831896, 0.720503);
        g->addKeyFrame(17.799999, 0.316957);
        g->addKeyFrame(18.633738, 0.185302);
        g->addKeyFrame(19.817745, 0.413315);
        g->addKeyFrame(24.0, 0.413315);
        m_backGroundSkyDomeColorG = g;

        auto b = ln::KeyFrameAnimationCurve::create();
        b->addKeyFrame(0.0, 0.441597);
        b->addKeyFrame(5.0, 0.441597);
        b->addKeyFrame(6.5, 0.839636);
        b->addKeyFrame(9.032509, 0.972142);
        b->addKeyFrame(12.0, 1.0);
        b->addKeyFrame(17.022861, 0.94726);
        b->addKeyFrame(18.633738, 0.75);
        b->addKeyFrame(19.817745, 0.441597);
        b->addKeyFrame(24.0, 0.441597);
        m_backGroundSkyDomeColorB = b;
    }

    {
        // とりあえず線形補間で近似
        auto r = ln::KeyFrameAnimationCurve::create();
        r->addKeyFrame(4.5, 2.0);
        r->addKeyFrame(5.53, 8.0);
        r->addKeyFrame(8.5, 2.82);
        r->addKeyFrame(12.0, 1.5);
        r->addKeyFrame(16.0, 3.0);
        r->addKeyFrame(17.0, 8.0);
        r->addKeyFrame(18.4, 5.6);
        r->addKeyFrame(19.2, 2.0);
        m_backGroundHorizonColorR = r;

        // とりあえず線形補間で近似
        auto g = ln::KeyFrameAnimationCurve::create();
        g->addKeyFrame(4.5, 1.9);
        g->addKeyFrame(5.021666, 0.74472);
        g->addKeyFrame(5.53, 3.0);
        g->addKeyFrame(8.5, 2.5);
        g->addKeyFrame(12.0, 2.7);
        g->addKeyFrame(16.0, 2.23);
        g->addKeyFrame(17.53, 1.345);
        g->addKeyFrame(18.34, 2.7);
        g->addKeyFrame(19.2, 1.9);
        m_backGroundHorizonColorG = g;

        // とりあえず線形補間で近似
        auto b = ln::KeyFrameAnimationCurve::create();
        b->addKeyFrame(4.6, 2.5);
        b->addKeyFrame(5.0, 0.828287);
        b->addKeyFrame(5.53, 0.549693);
        b->addKeyFrame(8.5, 3.0);
        b->addKeyFrame(12.0, 3.0);
        b->addKeyFrame(16.0, 2.5);
        b->addKeyFrame(17.53, 1.0);
        b->addKeyFrame(18.5, 0.76);
        b->addKeyFrame(19.2, 2.5);
        m_backGroundHorizonColorB = b;
    }

    {
        auto r = ln::KeyFrameAnimationCurve::create();
        r->addKeyFrame(5.0, 0.137255);
        r->addKeyFrame(5.592618, 0.75);
        r->addKeyFrame(6.225262, 0.95);
        r->addKeyFrame(8.996424, 0.674617);
        r->addKeyFrame(12.0, 0.633211);
        r->addKeyFrame(15.900076, 1.0);
        r->addKeyFrame(17.87228, 1.0);
        r->addKeyFrame(18.647556, 0.743711);
        r->addKeyFrame(19.5, 0.137255);
        m_allOverlayColorR = r;

        auto g = ln::KeyFrameAnimationCurve::create();
        g->addKeyFrame(5.0, 0.137255);
        g->addKeyFrame(5.592618, 0.18825);
        g->addKeyFrame(6.225262, 0.641275);
        g->addKeyFrame(8.996424, 0.762539);
        g->addKeyFrame(12.0, 0.808692);
        g->addKeyFrame(16.245581, 0.901319);
        g->addKeyFrame(17.87228, 0.47);
        g->addKeyFrame(18.647556, 0.108993);
        g->addKeyFrame(19.5, 0.137255);
        m_allOverlayColorG = g;

        auto b = ln::KeyFrameAnimationCurve::create();
        b->addKeyFrame(5.0, 0.156863);
        b->addKeyFrame(5.592618, 0.18825);
        b->addKeyFrame(6.225262, 0.35);
        b->addKeyFrame(9.05025, 0.843588);
        b->addKeyFrame(12.0, 1.0);
        b->addKeyFrame(16.245581, 1.0);
        b->addKeyFrame(17.87228, 0.3);
        b->addKeyFrame(18.647556, 0.10006);
        b->addKeyFrame(19.5, 0.156863);
        m_allOverlayColorB = b;

        // とりあえず線形補間で近似
        auto a = ln::KeyFrameAnimationCurve::create();
        a->addKeyFrame(6.5, 0.0);
        a->addKeyFrame(12.0, 0.65);
        a->addKeyFrame(18.0, 0.0);
        m_allOverlayColorA = a;
    }

    {
        auto r = ln::KeyFrameAnimationCurve::create();
        r->addKeyFrame(5.0, 0.12499);
        r->addKeyFrame(6.025473, 0.736755);
        r->addKeyFrame(8.295567, 0.936754);
        r->addKeyFrame(11.02476, 0.564322);
        r->addKeyFrame(14.645027, 0.565347);
        r->addKeyFrame(17.025473, 0.552442);
        r->addKeyFrame(17.768421, 0.736755);
        r->addKeyFrame(19.707409, 0.12499);
        m_baseCloudColorAndIntensityR = r;

        auto g = ln::KeyFrameAnimationCurve::create();
        g->addKeyFrame(5.0, 0.105382);
        g->addKeyFrame(6.025473, 0.133787);
        g->addKeyFrame(8.295567, 0.162505);
        g->addKeyFrame(11.02476, 0.70059);
        g->addKeyFrame(14.645027, 0.700712);
        g->addKeyFrame(17.025473, 0.657623);
        g->addKeyFrame(17.768421, 0.136755);
        g->addKeyFrame(19.707409, 0.105382);
        m_baseCloudColorAndIntensityG = g;

        auto b = ln::KeyFrameAnimationCurve::create();
        b->addKeyFrame(5.0, 0.285774);
        b->addKeyFrame(6.025473, 0.430111);
        b->addKeyFrame(8.295567, 0.256381);
        b->addKeyFrame(11.02476, 0.986755);
        b->addKeyFrame(14.645027, 0.986755);
        b->addKeyFrame(17.025473, 0.936754);
        b->addKeyFrame(17.768421, 0.578202);
        b->addKeyFrame(19.707409, 0.285774);
        m_baseCloudColorAndIntensityB = b;

        // とりあえず線形補間で近似
        auto a = ln::KeyFrameAnimationCurve::create();
        a->addKeyFrame(0.5, 0.6);
        a->addKeyFrame(12.0, 0.22);
        a->addKeyFrame(20.0, 0.6);
        m_baseCloudColorAndIntensityA = a;
    }

    m_timeOfDay = 12.0;
    m_skyColor.w = 0.0f;
    m_horizonColor.w = 0.0f;
    m_cloudColor.w = 0.0f;
    m_overlayColor.w = 0.0f;

    return true;
}

void InternalSkyDome::update(float elapsedTimer) {
    m_timeOfDay += 0.016; // TODO:
    m_timeOfDay = std::fmod(m_timeOfDay, 24.0f);

    float timeOfDay = m_timeOfDay;
    auto skyColor = Vector3(
        m_backGroundSkyDomeColorR->evaluate(timeOfDay),
        m_backGroundSkyDomeColorG->evaluate(timeOfDay),
        m_backGroundSkyDomeColorB->evaluate(timeOfDay));
    auto horizonColor = Vector3(
        m_backGroundHorizonColorR->evaluate(timeOfDay),
        m_backGroundHorizonColorG->evaluate(timeOfDay),
        m_backGroundHorizonColorB->evaluate(timeOfDay));
    auto baseCloudColorAndIntensity = Vector4(
        m_baseCloudColorAndIntensityR->evaluate(timeOfDay),
        m_baseCloudColorAndIntensityG->evaluate(timeOfDay),
        m_baseCloudColorAndIntensityB->evaluate(timeOfDay),
        m_baseCloudColorAndIntensityA->evaluate(timeOfDay));
    auto overlayColor = Vector4(
        m_allOverlayColorR->evaluate(timeOfDay),
        m_allOverlayColorG->evaluate(timeOfDay),
        m_allOverlayColorB->evaluate(timeOfDay),
        m_allOverlayColorA->evaluate(timeOfDay));

    //c *= overlayColor.xyz() * (overlayColor.w + 1.0);

    //c.mutatingNormalize();
    //m_sceneColor = Color(c.x, c.y, c.z, 1.0f);

    //skyColor.mutatingNormalize();
    //horizonColor.mutatingNormalize();

    m_fixedBackGroundSkyDomeColor = Color(Vector3::lerp(skyColor, m_skyColor.xyz(), m_skyColor.w), 1.0f);
    m_fixedBackGroundHorizonColor = Color(Vector3::lerp(horizonColor, m_horizonColor.xyz(), m_horizonColor.w), 1.0f);
    m_fixedBaseCloudColorAndIntensity = Color(Vector3::lerp(baseCloudColorAndIntensity.xyz(), m_cloudColor.xyz(), m_cloudColor.w), baseCloudColorAndIntensity.w);
    m_fixedAllOverlayColor = Color(Vector3::lerp(overlayColor.xyz(), m_overlayColor.xyz(), m_overlayColor.w), overlayColor.w);
}

void InternalSkyDome::render(RenderingContext* context, const RenderViewPoint* viewPoint) {
    {
        //float timeOfDay = m_timeOfDay;

        //auto BackGroundSkyDomeColor = ln::Color(
        //    m_backGroundSkyDomeColorR->evaluate(timeOfDay),
        //    m_backGroundSkyDomeColorG->evaluate(timeOfDay),
        //    m_backGroundSkyDomeColorB->evaluate(timeOfDay),
        //    1.0f);
        //auto BackGroundHorizonColor = ln::Color(
        //    m_backGroundHorizonColorR->evaluate(timeOfDay),
        //    m_backGroundHorizonColorG->evaluate(timeOfDay),
        //    m_backGroundHorizonColorB->evaluate(timeOfDay),
        //    1.0f);
        //auto AllOverlayColor = ln::Color(
        //    m_allOverlayColorR->evaluate(timeOfDay),
        //    m_allOverlayColorG->evaluate(timeOfDay),
        //    m_allOverlayColorB->evaluate(timeOfDay),
        //    m_allOverlayColorA->evaluate(timeOfDay));
        //auto BaseCloudColorAndIntensity = ln::Color(
        //    m_baseCloudColorAndIntensityR->evaluate(timeOfDay),
        //    m_baseCloudColorAndIntensityG->evaluate(timeOfDay),
        //    m_baseCloudColorAndIntensityB->evaluate(timeOfDay),
        //    m_baseCloudColorAndIntensityA->evaluate(timeOfDay));

        //BackGroundSkyDomeColor = Color::Gray;
        //BackGroundHorizonColor = Color::Gray;
        ////AllOverlayColor = Color::Gray;
        //BaseCloudColorAndIntensity = Color::Gray;

        m_material->setColor(_TT("_Curve_BackGroundSkyDomeColor"), m_fixedBackGroundSkyDomeColor);
        m_material->setColor(_TT("_Curve_BackGroundHorizonColor"), m_fixedBackGroundHorizonColor);
        m_material->setColor(_TT("_Curve_AllOverlayColor"), m_fixedAllOverlayColor);
        m_material->setColor(_TT("_Curve_BaseCloudColorAndIntensity"), m_fixedBaseCloudColorAndIntensity);

        m_material->setFloat(_TT("_Main_Clouds_Falloff_Intensity"), 0.8);
        m_material->setFloat(_TT("_Second_Clouds_Falloff_Intensity"), 0.8);
        m_material->setFloat(_TT("_ThirdCloudsIntensity"), 0.25);
        m_material->setFloat(_TT("_AllCloudsFalloffIntensity"), 0.95);
        m_material->setFloat(_TT("_AllCloudsIntensity"), 1.1);

        //m_material->setFloat(_TT("_Main_Clouds_Falloff_Intensity", 4.0);
        //m_material->setFloat(_TT("_Second_Clouds_Falloff_Intensity", 4.0);
        //m_material->setFloat(_TT("_ThirdCloudsIntensity", 0.0);
        //m_material->setFloat(_TT("_AllCloudsFalloffIntensity", 1.4);
        //m_material->setFloat(_TT("_AllCloudsIntensity", 0.6);

        //m_material->setFloat(_TT("_Main_Clouds_Falloff_Intensity", 0.0);
        //m_material->setFloat(_TT("_Second_Clouds_Falloff_Intensity", 0.0);
        //m_material->setFloat(_TT("_ThirdCloudsIntensity", 0.0);
        //m_material->setFloat(_TT("_AllCloudsFalloffIntensity", 0.0);
        //m_material->setFloat(_TT("_AllCloudsIntensity", 0.0);

        //m_material->setFloat(_TT("_Main_Clouds_Falloff_Intensity", 3.0);
        //m_material->setFloat(_TT("_Second_Clouds_Falloff_Intensity", 4.0);
        //m_material->setFloat(_TT("_AllCloudsFalloffIntensity", 1.15);
        //m_material->setFloat(_TT("_AllCloudsIntensity", 0.85);

        /*
            Super Heavy:
                Main Clouds Falloff Intensity: 0.8
                Second Clouds Falloff Intensity: 0.8
                Third Clouds  Intensity: 0.25
                All Clouds Falloff Intensity: 0.95
                All Clouds Intensity: 1.1

            Middle:
                Main Clouds Falloff Intensity: 3.0
                Second Clouds Falloff Intensity: 4.0
                Third Clouds  Intensity: 0.0
                All Clouds Falloff Intensity: 1.15
                All Clouds Intensity: 0.85

            Slight:
                Main Clouds Falloff Intensity: 4.0
                Second Clouds Falloff Intensity: 4.0
                Third Clouds  Intensity: 0.0
                All Clouds Falloff Intensity: 1.4
                All Clouds Intensity: 0.6

            Super Slight:
              Main Clouds Falloff Intensity: 5.0
              Second Clouds Falloff Intensity: 5.0
              All Clouds Falloff Intensity: 2.0
              Third Clouds Intensity: 0.0
              All Clouds Intensity: 0.35

            Clear:
              All Clouds Intensity: 0.0
        */
    }

    Matrix transform = Matrix::makeScaling(viewPoint->farClip - 0.0001); // 視界最遠まで拡大してみる
    transform.translate(viewPoint->viewPosition);

    for (const auto& node : m_model->meshNodes()) {
        if (node->meshContainerIndex() >= 0) {
            //context->setTransfrom(m_model->nodeGlobalTransform(node->index()));
            context->setTransfrom(transform);
            const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];

            for (const auto& mesh : meshContainer->meshPrimitives()) {
                for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                    context->setMaterial(m_model->materials()[mesh->sections()[iSection].materialIndex]);
                    context->drawMesh(mesh, iSection);
                }
            }
        }
    }
}

} // namespace detail
} // namespace ln
