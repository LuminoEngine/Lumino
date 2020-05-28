
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

void RenderingContext::setMaterial(Material* material)
{
    m_builder->setMaterial(material);
}

void RenderingContext::setRenderPhase(RenderPhaseClass value)
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

void RenderingContext::setFont(Font* value)
{
	m_builder->setFont(value);
}

void RenderingContext::setTextColor(const Color& value)
{
	m_builder->setTextColor(value);
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
			return static_cast<detail::ClearRenderFeature*>(renderFeature)->clear(batchList, flags, color, z, stencil);
		}
	};

    m_builder->advanceFence();

	auto* element = m_builder->addNewDrawElement<Clear>(m_manager->clearRenderFeature(), nullptr);
	element->addFlags(detail::RenderDrawElementTypeFlags::Clear);
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
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(&data);
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
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(&data);
        }
    };

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<DrawPlane>(
        m_manager->meshGeneraterRenderFeature(),
        m_builder->meshGeneraterRenderFeatureStageParameters());
    element->data.size.set(width, depth);
    element->data.setColor(color);
    //element->data.setTransform(/*element->combinedWorldMatrix()*/);
}

void RenderingContext::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
    class DrawSphere : public detail::RenderDrawElement
    {
    public:
        detail::RegularSphereMeshFactory data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
            return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(&data);
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
    element->data.setTransform(/*element->combinedWorldMatrix() * */localTransform);

	// TODO: bouding box
}

void RenderingContext::drawBox(const Box& box, const Color& color, const Matrix& localTransform)
{
	class DrawBox : public detail::RenderDrawElement
	{
	public:
		detail::RegularBoxMeshFactory data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(&data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<DrawBox>(
		m_manager->meshGeneraterRenderFeature(),
		m_builder->meshGeneraterRenderFeatureStageParameters());
	// TODO: box.center
	element->data.m_size = Vector3(box.width, box.height, box.depth);
	element->data.setColor(color);
	element->data.setTransform(/*element->combinedWorldMatrix() **/ localTransform);

	// TODO: bouding box
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
    element->targetPhase = RenderPhaseClass::Geometry;

    m_builder->advanceFence();

}

//void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//    blit(source, destination, nullptr);
//}
//
//void RenderingContext::blit(RenderTargetTexture* source, RenderTargetTexture* destination, Material* material) 
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
//    element->targetPhase = RenderPhaseClass::ImageEffect;
//    element->source = source;
//
//    setRenderTarget(0, oldTarget);
//
//    m_builder->advanceFence();
//}

void RenderingContext::blit(Material* source, RenderTargetTexture* destination, RenderPhaseClass phase)
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
    RenderTargetTexture* oldTarget = nullptr;
    if (destination)
    {
        RenderTargetTexture* oldTarget = renderTarget(0);
        setRenderTarget(0, destination);
    }

	//clear();

	m_builder->setMaterial(source);

	m_builder->advanceFence();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);	// TODO: この辺りは RenderFeature の描き方に依存するので、そっちからもらえるようにした方がいいかも
	auto* element = m_builder->addNewDrawElement<Blit>(
		m_manager->blitRenderFeature(),
		m_builder->blitRenderFeatureStageParameters());
	element->targetPhase = phase;

    if (destination)
    {
        setRenderTarget(0, oldTarget);
    }

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
	Material* material)
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
			return static_cast<detail::SpriteRenderFeature2*>(renderFeature)->drawRequest(
				batchList, context, combinedWorldMatrix() * transform, size, anchorRatio, srcRect, color, baseDirection, billboardType, flipFlags);
		}
	};

    // old 保持して戻すのではなく、Stage 確定時にオーバーライドして直接設定するような仕組みにできないか？
    auto* oldMat = m_builder->material();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	m_builder->setMaterial(material);
	auto* element = m_builder->addNewDrawElement<DrawSprite>(
		m_manager->spriteRenderFeature2(),
		m_builder->spriteRenderFeatureStageParameters2());
	element->transform = transform;
	element->size.set(size.width, size.height);
	element->anchorRatio = anchor;
	element->srcRect = srcRect;
	element->color = color;
	element->baseDirection = baseDirection;
	element->billboardType = billboardType;
    element->flipFlags = flipFlags;

    m_builder->setMaterial(oldMat);
	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawPrimitive(VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveTopology topology, int startVertex, int primitiveCount)
{
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

void RenderingContext::drawMesh(Mesh* mesh, int sectionIndex)
{
    class DrawMesh : public detail::RenderDrawElement
    {
    public:
        Ref<Mesh>  mesh;
        int sectionIndex;

		// SkinnedMesh の場合に、親インスタンスが破棄されないように参照を保持しておく
		//Ref<SkinnedMeshModel> skinnedMeshModel;

        virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
        {
            // TODO: boneTexture を送る仕組み
            return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, context, mesh, sectionIndex, nullptr);
        }
    };

    //if (meshResource->isInitialEmpty()) return;

    m_builder->setPrimitiveTopology(mesh->sections()[sectionIndex].topology);
    auto* element = m_builder->addNewDrawElement<DrawMesh>(
        m_manager->meshRenderFeature(),
        m_builder->meshRenderFeatureStageParameters());
    element->mesh = mesh;
    element->sectionIndex = sectionIndex;

    // TODO: bounding
}

void RenderingContext::drawSkinnedMesh(Mesh* mesh, int sectionIndex, MeshArmature* skeleton)
{
	class DrawSkinnedMesh : public detail::RenderDrawElement
	{
	public:
		Ref<Mesh>  mesh;
		int sectionIndex;

		// SkinnedMesh の場合に、親インスタンスが破棄されないように参照を保持しておく
		Ref<MeshArmature> skeleton;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			// TODO: boneTexture を送る仕組み
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, context, mesh, sectionIndex, skeleton);
		}
	};

	m_builder->setPrimitiveTopology(mesh->sections()[sectionIndex].topology);
	auto* element = m_builder->addNewDrawElement<DrawSkinnedMesh>(
		m_manager->meshRenderFeature(),
		m_builder->meshRenderFeatureStageParameters());
	element->mesh = mesh;
	element->sectionIndex = sectionIndex;
	element->skeleton = skeleton;

	// TODO: bounding
}

void RenderingContext::drawMeshInstanced(InstancedMeshList* list)
{
	class DrawMeshInstanced : public detail::RenderDrawElement
	{
	public:
		Ref<InstancedMeshList> list;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
		{
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMeshInstanced(batchList, context, list);
		}
	};

	if (list->instanceCount() <= 0) return;

	m_builder->setPrimitiveTopology(list->mesh()->sections()[list->sectionIndex()].topology);
	auto* element = m_builder->addNewDrawElement<DrawMeshInstanced>(
		m_manager->meshRenderFeature(),
		m_builder->meshRenderFeatureStageParameters());
	element->list = list;
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

void RenderingContext::drawTextSprite(const StringRef& text, const Color& color, const Vector2& anchor, SpriteBaseDirection baseDirection, detail::FontRequester* font)
{
	if (text.isEmpty()) return;

	// TODO: cache
	auto formattedText = makeRef<detail::FormattedText>();
	formattedText->text = text;
	formattedText->font = m_builder->font();
	formattedText->color = color;	// TODO: m_builder->textColor(); の方がいいか？
	formattedText->area = Rect();
	formattedText->textAlignment = TextAlignment::Forward;
	formattedText->fontRequester = font;

	if (!formattedText->font) {
		formattedText->font = m_manager->fontManager()->defaultFont();
	}

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(
		m_manager->spriteTextRenderFeature(),
		m_builder->spriteTextRenderFeatureStageParameters());
	element->formattedText = formattedText;
	element->anchor = anchor;
	element->baseDirection = baseDirection;

	if (baseDirection != SpriteBaseDirection::Basic2D) {
		// is 3D.
		element->samplerState = detail::EngineDomain::graphicsManager()->linearSamplerState();
	}
}

void RenderingContext::drawText(const StringRef& text, const Rect& area, TextAlignment alignment/*, TextCrossAlignment crossAlignment*//*, const Color& color, Font* font*/)
{

    // TODO: cache
    auto formattedText = makeRef<detail::FormattedText>();
    formattedText->text = text;
    formattedText->font = m_builder->font();
	formattedText->color = m_builder->textColor();
	formattedText->area = area;
	formattedText->textAlignment = alignment;

    if (!formattedText->font) {
        formattedText->font = m_manager->fontManager()->defaultFont();
    }

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(
        m_manager->spriteTextRenderFeature(),
        m_builder->spriteTextRenderFeatureStageParameters());
    element->formattedText = formattedText;
	element->baseDirection = SpriteBaseDirection::Basic2D;

    // TODO
    //detail::Sphere sphere;
    //detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
    //ptr->setLocalBoundingSphere(sphere);
}

void RenderingContext::drawChar(uint32_t codePoint, const Color& color, Font* font, const Matrix& transform)
{
	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawCharElement>(
		m_manager->spriteTextRenderFeature(),
		m_builder->spriteTextRenderFeatureStageParameters());
	element->codePoint = codePoint;
	element->color = color;
    element->transform = transform;

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

void RenderingContext::invokeExtensionRendering(INativeGraphicsExtension* extension)
{
    class InvokeExtensionRendering : public detail::RenderDrawElement
    {
    public:
        INativeGraphicsExtension* extension;

        virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
        {
            return static_cast<detail::ExtensionRenderFeature*>(renderFeature)->invoke(context, batchList, extension);
            //context->drawExtension(extension);
        }
    };

    auto* element = m_builder->addNewDrawElement<InvokeExtensionRendering>(
        m_manager->extensionRenderFeature(),
        m_builder->extensionRenderFeatureStageParameters());
    element->extension = extension;

    // TODO: bounding
}

void RenderingContext::addAmbientLight(const Color& color, float intensity)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeAmbientLightInfo(color, intensity));
}

void RenderingContext::addHemisphereLight(const Color& skyColor, const Color& groundColor, float intensity)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeHemisphereLightInfo(skyColor, groundColor, intensity));
}

void RenderingContext::addDirectionalLight(const Color& color, float intensity, const Vector3& direction, bool mainLight)
{
	m_builder->targetList()->addDynamicLightInfo(detail::DynamicLightInfo::makeDirectionalLightInfo(color, intensity, direction, mainLight));
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

const Matrix& RenderingContext::baseTransform() const
{
	return m_builder->baseTransform();
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

void RenderingContext::setAdditionalElementFlags(detail::RenderDrawElementTypeFlags value)
{
	m_builder->setAdditionalElementFlags(value);
}

//detail::RenderDrawElement* RenderingContext::lastRenderDrawElement() const
//{
//	return m_builder->targetList()->lastElement();
//}

} // namespace ln

