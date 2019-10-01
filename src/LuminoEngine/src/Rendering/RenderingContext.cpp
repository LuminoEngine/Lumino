
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../Font/FontManager.hpp"
#include "../Mesh/MeshGenerater.hpp"
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

void RenderingContext::setTransfrom(const Matrix& value)
{
	m_builder->setTransfrom(value);
}

void RenderingContext::setBlendMode(Optional<BlendMode> value)
{
	m_builder->setBlendMode(value);
}

void RenderingContext::setShadingModel(Optional<ShadingModel> value)
{
    m_builder->setShadingModel(value);
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

void RenderingContext::setMaterial(AbstractMaterial* material)
{
    m_builder->setMaterial(material);
}

void RenderingContext::setRenderPhase(RendringPhase value)
{
    m_builder->setRenderPhase(value);
}

void RenderingContext::setColorScale(const Color& value)
{
    m_builder->setColorScale(value);
}

void RenderingContext::setBlendColor(const Color& value)
{
    m_builder->setBlendColor(value);
}

void RenderingContext::setTone(const ColorTone& value)
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

void RenderingContext::clear(Flags<ClearFlags> flags, const Color& color, float z, uint8_t stencil)
{
	class Clear : public detail::RenderDrawElement
	{
	public:
		ClearFlags flags;
		Color color;
		float z;
		uint8_t stencil;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
#ifdef LN_RENDERING_MIGRATION
			return static_cast<detail::ClearRenderFeature*>(renderFeature)->clear(batchList, flags, color, z, stencil);
#else
			context->clear(flags, color, z, stencil);
#endif
		}
	};

    m_builder->advanceFence();

	auto* element = m_builder->addNewDrawElement<Clear>(m_manager->clearRenderFeature(), nullptr);
	element->elementType = detail::RenderDrawElementType::Clear;
	element->flags = flags;
	element->color = color;
	element->z = z;
	element->stencil = stencil;

    m_builder->advanceFence();
}

void RenderingContext::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
    class DrawLine : public detail::RenderDrawElement
    {
    public:
        detail::SingleLineGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
        {
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater<detail::SingleLineGenerater>(batchList, context, data);
        }
    };

	m_builder->setPrimitiveTopology(PrimitiveTopology::LineList);
    auto* element = m_builder->addNewDrawElement<DrawLine>(
        m_manager->meshGeneraterRenderFeature(),
        m_builder->meshGeneraterRenderFeatureStageParameters());
    element->data.point1 = from;
    element->data.point1Color = fromColor;
    element->data.point2 = to;
    element->data.point2Color = toColor;
    // TODO:
    //ptr->makeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}

void RenderingContext::drawPlane(float width, float depth, const Color& color)
{
    class DrawPlane : public detail::RenderDrawElement
    {
    public:
        detail::PlaneMeshGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater<detail::PlaneMeshGenerater>(batchList, context, data);
        }
    };

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<DrawPlane>(
        m_manager->meshGeneraterRenderFeature(),
        m_builder->meshGeneraterRenderFeatureStageParameters());
    element->data.size.set(width, depth);
    element->data.setColor(color);
    element->data.setTransform(element->combinedWorldMatrix());
}

void RenderingContext::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
    class DrawSphere : public detail::RenderDrawElement
    {
    public:
        detail::RegularSphereMeshFactory data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater<detail::RegularSphereMeshFactory>(batchList, context, data);
        }
    };

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<DrawSphere>(
        m_manager->meshGeneraterRenderFeature(),
        m_builder->meshGeneraterRenderFeatureStageParameters());
    element->data.m_radius = radius;
    element->data.m_slices = slices;
    element->data.m_stacks = stacks;
    element->data.setColor(color);
    element->data.setTransform(element->combinedWorldMatrix() * localTransform);

	// TODO: bouding box
}

void RenderingContext::drawBox(const Box& box, const Color& color, const Matrix& localTransform)
{
	LN_NOTIMPLEMENTED();
}

void RenderingContext::drawScreenRectangle()
{
    class DrawScreenRectangle : public detail::RenderDrawElement
    {
    public:
		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
            return static_cast<detail::BlitRenderFeature*>(renderFeature)->blit(batchList, context);
        }
    };

    m_builder->advanceFence();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);	// TODO: この辺りは RenderFeature の描き方に依存するので、そっちからもらえるようにした方がいいかも
    auto* element = m_builder->addNewDrawElement<DrawScreenRectangle>(
        m_manager->blitRenderFeature(),
        m_builder->blitRenderFeatureStageParameters());
    element->targetPhase = RendringPhase::Default;

    m_builder->advanceFence();

}

//void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//    blit(source, destination, nullptr);
//}
//
//void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination, AbstractMaterial* material) 
//{
//    class Blit : public detail::RenderDrawElement
//    {
//    public:
//        Ref<RenderTargetTexture> source;
//
//        virtual void onSubsetInfoOverride(detail::SubsetInfo* subsetInfo)
//        {
//            if (source) {
//                subsetInfo->materialTexture = source;
//            }
//        }
//
//        virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
//        {
//            static_cast<detail::BlitRenderFeature*>(renderFeatures)->blit(context);
//        }
//    };
//
//    // TODO: scoped_gurad
//    RenderTargetTexture* oldTarget = renderTarget(0);
//    setRenderTarget(0, destination);
//
//    m_builder->setMaterial(material);
//
//    m_builder->advanceFence();
//
//    auto* element = m_builder->addNewDrawElement<Blit>(
//        m_manager->blitRenderFeature(),
//        m_builder->blitRenderFeatureStageParameters());
//    element->targetPhase = RendringPhase::ImageEffect;
//    element->source = source;
//
//    setRenderTarget(0, oldTarget);
//
//    m_builder->advanceFence();
//}

void RenderingContext::blit(AbstractMaterial* source, RenderTargetTexture* destination)
{
	class Blit : public detail::RenderDrawElement
	{
	public:
		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			return static_cast<detail::BlitRenderFeature*>(renderFeature)->blit(batchList, context);
		}
	};

	// TODO: scoped_gurad
	RenderTargetTexture* oldTarget = renderTarget(0);
	setRenderTarget(0, destination);

	m_builder->setMaterial(source);

	m_builder->advanceFence();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);	// TODO: この辺りは RenderFeature の描き方に依存するので、そっちからもらえるようにした方がいいかも
	auto* element = m_builder->addNewDrawElement<Blit>(
		m_manager->blitRenderFeature(),
		m_builder->blitRenderFeatureStageParameters());
	element->targetPhase = RendringPhase::ImageEffect;

	setRenderTarget(0, oldTarget);

	m_builder->advanceFence();
}

void RenderingContext::drawSprite(
	const Matrix& transform,
	const Size& size,
	const Vector2& anchor,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
    const Flags<detail::SpriteFlipFlags>& flipFlags,
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
        detail::SpriteFlipFlags flipFlags;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
#ifdef LN_RENDERING_MIGRATION
			return static_cast<detail::SpriteRenderFeature2*>(renderFeature)->drawRequest(
				batchList, context, combinedWorldMatrix() * transform, size, anchorRatio, srcRect, color, baseDirection, billboardType, flipFlags);
#else
			static_cast<detail::SpriteRenderFeature*>(renderFeatures)->drawRequest(
				context, transform, size, anchorRatio, srcRect, color, baseDirection, billboardType, flipFlags);
#endif
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	m_builder->setMaterial(material);
#ifdef LN_RENDERING_MIGRATION
	auto* element = m_builder->addNewDrawElement<DrawSprite>(
		m_manager->spriteRenderFeature2(),
		m_builder->spriteRenderFeatureStageParameters2());
#else
	auto* element = m_builder->addNewDrawElement<DrawSprite>(
		m_manager->spriteRenderFeature(),
		m_builder->spriteRenderFeatureStageParameters());
#endif
	element->transform = transform;
	element->size.set(size.width, size.height);
	element->anchorRatio = anchor;
	element->srcRect = srcRect;
	element->color = color;
	element->baseDirection = baseDirection;
	element->billboardType = billboardType;
    element->flipFlags = flipFlags;
	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawPrimitive(VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveTopology topology, int startVertex, int primitiveCount)
{
#ifdef LN_RENDERING_MIGRATION
	if (primitiveCount <= 0) return;

	class DrawPrimitive : public detail::RenderDrawElement
	{
	public:
		Ref<VertexLayout> vertexLayout;
		Ref<VertexBuffer> vertexBuffer;
		int startVertex;
		int primitiveCount;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			return static_cast<detail::PrimitiveRenderFeature*>(renderFeature)->drawPrimitive(batchList, vertexLayout, vertexBuffer, startVertex, primitiveCount);
		}
	};

	m_builder->setPrimitiveTopology(topology);
	auto* element = m_builder->addNewDrawElement<DrawPrimitive>(
		m_manager->primitiveRenderFeature(),
		m_builder->meshGeneraterRenderFeatureStageParameters());	// TODO: めんどいので共有。あどで消すし…
	element->vertexLayout = vertexDeclaration;
	element->vertexBuffer = vertexBuffer;
	element->startVertex = startVertex;
	element->primitiveCount = primitiveCount;
#else
	class DrawPrimitive : public detail::RenderDrawElement
	{
	public:
		Ref<VertexLayout> vertexDeclaration;
		Ref<VertexBuffer> vertexBuffer;
		//PrimitiveTopology topology;
		int startVertex;
		int primitiveCount;

		virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures, const detail::SubsetInfo* subsetInfo) override
		{
			context->setVertexLayout(vertexDeclaration);
			context->setVertexBuffer(0, vertexBuffer);
			//context->setPrimitiveTopology(topology);
			context->drawPrimitive(startVertex, primitiveCount);
		}
	};

	m_builder->setPrimitiveTopology(topology);
	auto* element = m_builder->addNewDrawElement<DrawPrimitive>(nullptr, nullptr);
	element->vertexDeclaration = vertexDeclaration;
	element->vertexBuffer = vertexBuffer;
	//element->topology = topology;
	element->startVertex = startVertex;
	element->primitiveCount = primitiveCount;
#endif
}

// LOD なし。というか直接描画
void RenderingContext::drawMesh(MeshResource* meshResource, int sectionIndex)
{
    class DrawMesh : public detail::RenderDrawElement
    {
    public:
        Ref<MeshResource> meshResource;
        int sectionIndex;

        //virtual void onElementInfoOverride(detail::ElementInfo* elementInfo, detail::ShaderTechniqueClass_MeshProcess* meshProcess) override
        //{
        //    if (elementInfo->boneTexture && elementInfo->boneLocalQuaternionTexture) {
        //        if (MeshContainer* container = meshResource->ownerContainer()) {
        //            if (StaticMeshModel* model = container->meshModel()) {
        //                if (model->meshModelType() == detail::InternalMeshModelType::SkinnedMesh) {
        //                    //elementInfo->boneTexture->map()
        //                    printf("skinned\n");
        //                    *meshProcess = detail::ShaderTechniqueClass_MeshProcess::SkinnedMesh;
        //                    Bitmap2D* bmp1 = elementInfo->boneTexture->map(MapMode::Write);
        //                    Bitmap2D* bmp2 = elementInfo->boneLocalQuaternionTexture->map(MapMode::Write);
        //                    static_cast<SkinnedMeshModel*>(model)->writeSkinningMatrices(
        //                        reinterpret_cast<Matrix*>(bmp1->data()),
        //                        reinterpret_cast<Quaternion*>(bmp2->data()));
        //                }
        //            }
        //        }
        //    }
        //}

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			// TODO: boneTexture を送る仕組み
            return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, context, meshResource, sectionIndex);
        }
    };

    if (meshResource->isInitialEmpty()) return;

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<DrawMesh>(
        m_manager->meshRenderFeature(),
        m_builder->meshRenderFeatureStageParameters());
    element->meshResource = meshResource;
    element->sectionIndex = sectionIndex;

    // TODO: bounding
}

//void RenderingContext::drawMesh(MeshContainer* meshContainer, int sectionIndex)
//{
//	class DrawMesh : public detail::RenderDrawElement
//	{
//	public:
//		Ref<MeshContainer> meshContainer;
//		int sectionIndex;
//
//		virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
//		{
//			// TODO: LOD Level
//			MeshResource* mesh = meshContainer->selectLODResource(0);
//
//			static_cast<detail::MeshRenderFeature*>(renderFeatures)->drawMesh(context, mesh, sectionIndex);
//		}
//	};
//
//
//	MeshResource* mesh = meshContainer->meshResource();
//	int materialIndex = mesh->sections()[sectionIndex].materialIndex;
//
//	m_builder->setMaterial(
//		meshContainer->meshModel()->materials()[materialIndex]);
//
//	auto* element = m_builder->addNewDrawElement<DrawMesh>(
//		m_manager->meshRenderFeature(),
//		m_builder->meshRenderFeatureStageParameters());
//	element->meshContainer = meshContainer;
//	element->sectionIndex = sectionIndex;
//	// TODO
//	//detail::Sphere sphere;
//	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
//	//ptr->setLocalBoundingSphere(sphere);
//}

void RenderingContext::drawText(const StringRef& text, const Color& color, Font* font)
{

    // TODO: cache
    auto formattedText = makeRef<detail::FormattedText>();
    formattedText->text = text;
    formattedText->font = font;
	formattedText->color = color;

    if (!formattedText->font) {
        formattedText->font = m_manager->fontManager()->defaultFont();
    }

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(
        m_manager->spriteTextRenderFeature(),
        m_builder->spriteTextRenderFeatureStageParameters());
    element->formattedText = formattedText;

    // TODO
    //detail::Sphere sphere;
    //detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
    //ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawChar(uint32_t codePoint, const Color& color, Font* font)
{
	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawCharElement>(
		m_manager->spriteTextRenderFeature(),
		m_builder->spriteTextRenderFeatureStageParameters());
	element->codePoint = codePoint;
	element->color = color;

	if (font)
		element->font = font;
	else
		element->font = m_manager->fontManager()->defaultFont();


	//element->flexText = makeRef<detail::FlexText>();	// TODO: cache
	//element->flexText->copyFrom(text);

	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawFlexGlyphRun(detail::FlexGlyphRun* glyphRun)
{
	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(
		m_manager->spriteTextRenderFeature(),
		m_builder->spriteTextRenderFeatureStageParameters());
	element->glyphRun = glyphRun;
	//element->flexText = makeRef<detail::FlexText>();	// TODO: cache
	//element->flexText->copyFrom(text);

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

Size RenderingContext::measureTextSize(Font* font, const StringRef& text) const
{
	if (LN_REQUIRE(font)) return Size::Zero;
	if (text.isEmpty()) return Size::Zero;
	return font->measureRenderSize(text, viewPoint()->dpiScale);
}

Size RenderingContext::measureTextSize(Font* font, uint32_t codePoint) const
{
	if (LN_REQUIRE(font)) return Size::Zero;
	if (codePoint == 0) return Size::Zero;
	return font->measureRenderSize(codePoint, viewPoint()->dpiScale);
}

RenderViewPoint* RenderingContext::viewPoint() const
{
    return m_builder->viewPoint();
}

void RenderingContext::setBaseTransfrom(const Optional<Matrix>& value)
{
    m_builder->setBaseTransfrom(value);
}

void RenderingContext::setRenderPriority(int value)
{
    m_builder->setRenderPriority(value);
}

void RenderingContext::setBaseBuiltinEffectData(const Optional<detail::BuiltinEffectData>& value)
{
    m_builder->setBaseBuiltinEffectData(value);
}

void RenderingContext::setViewPoint(RenderViewPoint* value)
{
    m_builder->setViewPoint(value);
}

} // namespace ln

