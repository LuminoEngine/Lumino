
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "RenderingManager.hpp"
#include "DrawElementListBuilder.hpp"

namespace ln {

//==============================================================================
// RenderingContext

RenderingContext::RenderingContext()
	: m_manager(detail::EngineDomain::renderingManager())
	, m_builder(makeRef<detail::DrawElementListBuilder>())
{
}

RenderingContext::~RenderingContext()
{
}

void RenderingContext::setDrawElementList(detail::DrawElementList* list)
{
	m_builder->setTargetList(list);
}

void RenderingContext::resetForBeginRendering()
{
	m_builder->resetForBeginRendering();
}

void RenderingContext::setRenderTarget(int index, RenderTargetTexture * value)
{
	m_builder->setRenderTarget(index, value);
}

RenderTargetTexture* RenderingContext::renderTarget(int index) const
{
    return m_builder->renderTarget(index);
}

void RenderingContext::setDepthBuffer(DepthBuffer * value)
{
	m_builder->setDepthBuffer(value);
}

void RenderingContext::setViewportRect(const RectI & value)
{
	m_builder->setViewportRect(value);
}

void RenderingContext::setScissorRect(const RectI & value)
{
	m_builder->setScissorRect(value);
}

void RenderingContext::setBlendMode(Optional<BlendMode> value)
{
	m_builder->setBlendMode(value);
}

void RenderingContext::setCullingMode(Optional<CullMode> value)
{
	m_builder->setCullingMode(value);
}

void RenderingContext::setDepthTestEnabled(Optional<bool> value)
{
	m_builder->setDepthTestEnabled(value);
}

void RenderingContext::setDepthWriteEnabled(Optional<bool> value)
{
	m_builder->setDepthWriteEnabled(value);
}

void RenderingContext::setOpacity(float value)
{
    m_builder->setOpacity(value);
}

void RenderingContext::setColorScale(const Color& value)
{
    m_builder->setColorScale(value);
}

void RenderingContext::setBlendColor(const Color& value)
{
    m_builder->setBlendColor(value);
}

void RenderingContext::setTone(const ToneF& value)
{
    m_builder->setTone(value);
}

void RenderingContext::resetState()
{
    m_builder->reset2();
}

void RenderingContext::pushState(bool reset)
{
    m_builder->pushState(reset);
}

void RenderingContext::popState()
{
    m_builder->popState();
}

void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination)
{
    blit(source, destination, nullptr);
}

void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination, AbstractMaterial* material) 
{
    class Blit : public detail::RenderDrawElement
    {
    public:
        Ref<RenderTargetTexture> source;

        virtual void onSubsetInfoOverride(detail::SubsetInfo* subsetInfo)
        {
            if (source) {
                subsetInfo->materialTexture = source;
            }
        }

        virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
        {
            static_cast<detail::BlitRenderFeature*>(renderFeatures)->blit(context);
        }
    };

    // TODO: scoped_gurad
    RenderTargetTexture* oldTarget = renderTarget(0);
    setRenderTarget(0, destination);

    m_builder->setMaterial(material);
    auto* element = m_builder->addNewDrawElement<Blit>(
        m_manager->blitRenderFeature(),
        m_builder->blitRenderFeatureStageParameters());
    element->targetPhase = detail::RendringPhase::ImageEffect;
    element->source = source;

    setRenderTarget(0, oldTarget);
}

void RenderingContext::drawSprite(
	const Matrix& transform,
	const Size& size,
	const Vector2& anchor,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
	AbstractMaterial* material)
{
	class DrawSprite : public detail::RenderDrawElement
	{
	public:
		Matrix transform;
		Vector2 size;
		Vector2 anchorRatio;
		Rect srcRect;
		Color color;
		SpriteBaseDirection baseDirection;
		BillboardType billboardType;

		virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
		{
			static_cast<detail::SpriteRenderFeature*>(renderFeatures)->drawRequest(
				transform, size, anchorRatio, srcRect, color, baseDirection, billboardType);
		}
	};

	m_builder->setMaterial(material);
	auto* element = m_builder->addNewDrawElement<DrawSprite>(
		m_manager->spriteRenderFeature(),
		m_builder->spriteRenderFeatureStageParameters());
	element->transform = transform;
	element->size.set(size.width, size.height);
	element->anchorRatio = anchor;
	element->srcRect = srcRect;
	element->color = color;
	element->baseDirection = baseDirection;
	element->billboardType = billboardType;
	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawMesh(MeshContainer* meshContainer, int sectionIndex)
{
	class DrawMesh : public detail::RenderDrawElement
	{
	public:
		Ref<MeshContainer> meshContainer;
		int sectionIndex;

		virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
		{
			// TODO: LOD Level
			MeshResource* mesh = meshContainer->selectLODResource(0);

			static_cast<detail::MeshRenderFeature*>(renderFeatures)->drawMesh(context, mesh, sectionIndex);
		}
	};


	MeshResource* mesh = meshContainer->meshResource();
	int materialIndex = mesh->sections()[sectionIndex].materialIndex;

	m_builder->setMaterial(
		meshContainer->meshModel()->materials()[materialIndex]);

	auto* element = m_builder->addNewDrawElement<DrawMesh>(
		m_manager->meshRenderFeature(),
		m_builder->meshRenderFeatureStageParameters());
	element->meshContainer = meshContainer;
	element->sectionIndex = sectionIndex;
	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::addAmbientLight(const Color& color, float intensity)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeAmbientLightInfo(color, intensity));
}

void RenderingContext::addHemisphereLight(const Color& skyColor, const Color& groundColor, float intensity)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeHemisphereLightInfo(skyColor, groundColor, intensity));
}

void RenderingContext::addDirectionalLight(const Color& color, float intensity, const Vector3& direction)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeDirectionalLightInfo(color, intensity, direction));
}

void RenderingContext::addPointLight(const Color& color, float intensity, const Vector3& position, float range, float attenuation)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makePointLightInfo(color, intensity, position, range, attenuation));
}

void RenderingContext::addSpotLight(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeSpotLightInfo(color, intensity, position, direction, range, attenuation, spotAngle, spotPenumbra));
}

} // namespace ln

