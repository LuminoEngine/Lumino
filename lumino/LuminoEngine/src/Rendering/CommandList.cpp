
#include "Internal.hpp"
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>
#include "../../Font/src/FontManager.hpp"
#include "../Mesh/MeshModelInstance.hpp"
#include "RenderElement.hpp"
#include "DrawElementListBuilder.hpp"

namespace ln {

//==============================================================================
// CommandList

CommandList::CommandList()
	: m_manager(detail::EngineDomain::renderingManager())
	, m_elementList(makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager()))
	, m_builder(makeRef<detail::DrawElementListBuilder>())
{
	m_builder->setTargetList(m_elementList);
}

void CommandList::clearCommandsAndState()
{
	m_builder->resetForBeginRendering();
	m_elementList->clear();
}

void CommandList::setRenderTarget(int index, RenderTargetTexture* value)
{
	m_builder->setRenderTarget(index, value);
}

RenderTargetTexture* CommandList::renderTarget(int index) const
{
	return m_builder->renderTarget(index);
}

void CommandList::setDepthBuffer(DepthBuffer* value)
{
	m_builder->setDepthBuffer(value);
}

void CommandList::setViewportRect(const RectI& value)
{
	m_builder->setViewportRect(value);
}

void CommandList::setScissorRect(const RectI& value)
{
	m_builder->setScissorRect(value);
}

void CommandList::setTransfrom(const Matrix& value)
{
	m_builder->setTransfrom(value);
}

void CommandList::setBlendMode(Optional<BlendMode> value)
{
	m_builder->setBlendMode(value);
}

void CommandList::setShadingModel(Optional<ShadingModel> value)
{
	m_builder->setShadingModel(value);
}

void CommandList::setCullingMode(Optional<CullMode> value)
{
	m_builder->setCullingMode(value);
}

void CommandList::setDepthTestEnabled(Optional<bool> value)
{
	m_builder->setDepthTestEnabled(value);
}

void CommandList::setDepthWriteEnabled(Optional<bool> value)
{
	m_builder->setDepthWriteEnabled(value);
}

void CommandList::setOpacity(float value)
{
	m_builder->setOpacity(value);
}

void CommandList::setMaterial(Material* material)
{
	m_builder->setMaterial(material);
}

void CommandList::setRenderPhase(RenderPart value)
{
	m_builder->setRenderPhase(value);
}

void CommandList::setColorScale(const Color& value)
{
	m_builder->setColorScale(value);
}

void CommandList::setBlendColor(const Color& value)
{
	m_builder->setBlendColor(value);
}

void CommandList::setTone(const ColorTone& value)
{
	m_builder->setTone(value);
}

void CommandList::setFont(Font* value)
{
	m_builder->setFont(value);
}

void CommandList::setTextColor(const Color& value)
{
	m_builder->setTextColor(value);
}

void CommandList::resetState()
{
	m_builder->reset2();
}

void CommandList::pushState(bool reset)
{
	m_builder->pushState(reset);
}

void CommandList::popState()
{
	m_builder->popState();
}

void CommandList::clear(Flags<ClearFlags> flags, const Color& color, float z, uint8_t stencil)
{
	class Clear : public detail::RenderDrawElement
	{
	public:
		ClearFlags flags;
		Color color;
		float z;
		uint8_t stencil;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::ClearRenderFeature*>(renderFeature)->clear(batchList, *state, flags, color, z, stencil);
		}
	};

	m_builder->advanceFence();

	auto* element = m_builder->addNewDrawElement<Clear>(m_manager->clearRenderFeature());
	element->addFlags(detail::RenderDrawElementTypeFlags::Clear);
	element->flags = flags;
	element->color = color;
	element->z = z;
	element->stencil = stencil;

	m_builder->advanceFence();
}

void CommandList::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	class DrawLine : public detail::RenderDrawElement
	{
	public:
		detail::SingleLineGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::LineList);
	auto* element = m_builder->addNewDrawElement<DrawLine>(m_manager->meshGeneraterRenderFeature());
	element->data.point1 = from;
	element->data.point1Color = fromColor;
	element->data.point2 = to;
	element->data.point2Color = toColor;
	// TODO:
	//ptr->makeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}

void CommandList::drawLineList(const Vector3* points, int pointCount, const Color& color)
{
	struct DrawLineListPrimitive : public detail::RenderDrawElement
	{
		detail::LineListPrimitiveGenerater data;
		Color color;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::LineList);

	auto* element = m_builder->addNewDrawElement<DrawLineListPrimitive>(m_manager->meshGeneraterRenderFeature());

	size_t pointsSize = sizeof(Vector3) * pointCount;
	element->data.points = reinterpret_cast<Vector3*>(m_builder->targetList()->newFrameRawData(pointsSize, points));
	element->data.pointCount = pointCount;
	element->color = color;
	element->data.colors = &element->color;
	element->data.colorCount = 1;

	// TODO:
	//ptr->makeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}

void CommandList::drawLineStripPrimitive(int pointCount, const Vector3* points, const Color* colors)
{
	class DrawLineStripPrimitive : public detail::RenderDrawElement
	{
	public:
		detail::LineStripPrimitiveGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::LineStrip);



	auto* element = m_builder->addNewDrawElement<DrawLineStripPrimitive>(m_manager->meshGeneraterRenderFeature());

	size_t pointsSize = sizeof(Vector3) * pointCount;
	size_t colorsSize = sizeof(Color) * pointCount;
	void* buffer = m_builder->targetList()->newFrameRawData(pointsSize + colorsSize);
	auto* pointsBuffer = reinterpret_cast<Vector3*>(buffer);
	auto* colorsBuffer = reinterpret_cast<Color*>(static_cast<uint8_t*>(buffer) + pointsSize);
	memcpy(pointsBuffer, points, pointsSize);
	memcpy(colorsBuffer, colors, colorsSize);

	element->data.pointCount = pointCount;
	element->data.points = pointsBuffer;
	element->data.colors = colorsBuffer;
	// TODO:
	//ptr->makeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}


void CommandList::drawPlane(float width, float depth, const Color& color)
{
	drawPlane(width, depth, Vector2::Zero, Vector2::Ones, color);
}

void CommandList::drawPlane(float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color)
{
	class DrawPlane : public detail::RenderDrawElement
	{
	public:
		detail::PlaneMeshGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<DrawPlane>(m_manager->meshGeneraterRenderFeature());
	element->data.size.set(width, depth);
	element->data.uv[0] = uv1;
	element->data.uv[1] = uv2;
	element->data.setColor(color);
	//element->data.setTransform(/*element->combinedWorldMatrix()*/);
}

void CommandList::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
	class DrawSphere : public detail::RenderDrawElement
	{
	public:
		detail::RegularSphereMeshFactory data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<DrawSphere>(m_manager->meshGeneraterRenderFeature());
	element->data.m_radius = radius;
	element->data.m_slices = slices;
	element->data.m_stacks = stacks;
	element->data.setColor(color);
	element->data.setTransform(/*element->combinedWorldMatrix() * */localTransform);

	// TODO: bouding box
}

void CommandList::drawBox(const Box& box, const Color& color, const Matrix& localTransform)
{
	class DrawBox : public detail::RenderDrawElement
	{
	public:
		detail::RegularBoxMeshFactory data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<DrawBox>(m_manager->meshGeneraterRenderFeature());
	// TODO: box.center
	element->data.m_size = Vector3(box.width, box.height, box.depth);
	element->data.setColor(color);
	element->data.setTransform(/*element->combinedWorldMatrix() **/ localTransform);

	// TODO: bouding box
}

void CommandList::drawScreenRectangle()
{
	class DrawScreenRectangle : public detail::RenderDrawElement
	{
	public:
		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::BlitRenderFeature*>(renderFeature)->blit(batchList, *state);
		}
	};

	m_builder->advanceFence();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);	// TODO: この辺りは RenderFeature の描き方に依存するので、そっちからもらえるようにした方がいいかも
	auto* element = m_builder->addNewDrawElement<DrawScreenRectangle>(m_manager->blitRenderFeature());
	element->targetPhase = RenderPart::Geometry;

	m_builder->advanceFence();

}

//void CommandList::blit(RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//    blit(source, destination, nullptr);
//}
//
//void CommandList::blit(RenderTargetTexture* source, RenderTargetTexture* destination, Material* material) 
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
//        virtual void onDraw(GraphicsCommandList* context, RenderFeature* renderFeatures) override
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
//    element->targetPhase = RenderPart::PostEffect;
//    element->source = source;
//
//    setRenderTarget(0, oldTarget);
//
//    m_builder->advanceFence();
//}

void CommandList::blit(Material* source, RenderTargetTexture* destination, RenderPart phase)
{
	class Blit : public detail::RenderDrawElement
	{
	public:
		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::BlitRenderFeature*>(renderFeature)->blit(batchList, *state);
		}
	};

	// TODO: scoped_gurad
	RenderTargetTexture* oldTarget = nullptr;
	if (destination)
	{
		oldTarget = renderTarget(0);
		setRenderTarget(0, destination);
	}

	//clear();

	m_builder->setMaterial(source);

	m_builder->advanceFence();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);	// TODO: この辺りは RenderFeature の描き方に依存するので、そっちからもらえるようにした方がいいかも

	auto oldRenderPhase = m_builder->renderPhase();
	m_builder->setRenderPhase(phase);
	auto* element = m_builder->addNewDrawElement<Blit>(m_manager->blitRenderFeature());
	//element->targetPhase = phase;

	if (destination)
	{
		setRenderTarget(0, oldTarget);
	}

	m_builder->setRenderPhase(oldRenderPhase);

	m_builder->advanceFence();
}

void CommandList::drawSprite(
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

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::SpriteRenderFeature2*>(renderFeature)->drawRequest(
				batchList, *state, context, combinedWorldMatrix() * transform, size, anchorRatio, srcRect, color, baseDirection, billboardType, flipFlags);
		}
	};

	// old 保持して戻すのではなく、Stage 確定時にオーバーライドして直接設定するような仕組みにできないか？
	auto* oldMat = m_builder->material();

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	m_builder->setMaterial(material);
	auto* element = m_builder->addNewDrawElement<DrawSprite>(m_manager->spriteRenderFeature2());
	element->transform = transform;
	element->size.set(size.width, size.height);
	element->anchorRatio = anchor;
	element->srcRect = srcRect;
	element->color = color;
	element->baseDirection = baseDirection;
	element->billboardType = billboardType;
	element->flipFlags = flipFlags;

	m_builder->setMaterial(oldMat);
	// TODO: bounding
}

void CommandList::drawPrimitive(VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveTopology topology, int startVertex, int primitiveCount)
{
	if (primitiveCount <= 0) return;

	class DrawPrimitive : public detail::RenderDrawElement
	{
	public:
		Ref<VertexLayout> vertexLayout;
		Ref<VertexBuffer> vertexBuffer;
		int startVertex;
		int primitiveCount;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::PrimitiveRenderFeature*>(renderFeature)->drawPrimitive(batchList, *state, vertexLayout, vertexBuffer, startVertex, primitiveCount);
		}
	};

	m_builder->setPrimitiveTopology(topology);
	auto* element = m_builder->addNewDrawElement<DrawPrimitive>(m_manager->primitiveRenderFeature());
	element->vertexLayout = vertexDeclaration;
	element->vertexBuffer = vertexBuffer;
	element->startVertex = startVertex;
	element->primitiveCount = primitiveCount;
}

// LOD なし。というか直接描画
void CommandList::drawMesh(MeshResource* meshResource, int sectionIndex)
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
		//            if (MeshModel* model = container->meshModel()) {
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

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			// TODO: boneTexture を送る仕組み
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, *state, context, meshResource, sectionIndex);
		}
	};

	if (meshResource->isInitialEmpty()) return;

	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<DrawMesh>(m_manager->meshRenderFeature());
	element->meshResource = meshResource;
	element->sectionIndex = sectionIndex;

	// TODO: bounding
}

void CommandList::drawMesh(MeshPrimitive* mesh, int sectionIndex)
{
	class DrawMesh : public detail::RenderDrawElement
	{
	public:
		Ref<MeshPrimitive>  mesh;
		int sectionIndex;

		// SkinnedMesh の場合に、親インスタンスが破棄されないように参照を保持しておく
		//Ref<SkinnedMeshModel> skinnedMeshModel;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			// TODO: boneTexture を送る仕組み
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, *state, context, mesh, sectionIndex, nullptr, nullptr);
		}
	};

	//if (meshResource->isInitialEmpty()) return;

	m_builder->setPrimitiveTopology(mesh->sections()[sectionIndex].topology);
	auto* element = m_builder->addNewDrawElement<DrawMesh>(m_manager->meshRenderFeature());
	element->mesh = mesh;
	element->sectionIndex = sectionIndex;

	// TODO: bounding
}

void CommandList::drawSkinnedMesh(MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton, detail::MorphInstance* morph)
{
	class DrawSkinnedMesh : public detail::RenderDrawElement
	{
	public:
		Ref<MeshPrimitive>  mesh;
		int sectionIndex;

		// SkinnedMesh の場合に、親インスタンスが破棄されないように参照を保持しておく
		Ref<detail::SkeletonInstance> skeleton;
		Ref<detail::MorphInstance> morph;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMesh(batchList, *state, context, mesh, sectionIndex, skeleton, morph);
		}
	};

	m_builder->setPrimitiveTopology(mesh->sections()[sectionIndex].topology);
	auto* element = m_builder->addNewDrawElement<DrawSkinnedMesh>(m_manager->meshRenderFeature());
	element->mesh = mesh;
	element->sectionIndex = sectionIndex;
	element->skeleton = skeleton;
	element->morph = morph;

	// TODO: bounding
}

void CommandList::drawMeshInstanced(InstancedMeshList* list)
{
	class DrawMeshInstanced : public detail::RenderDrawElement
	{
	public:
		Ref<InstancedMeshList> list;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshRenderFeature*>(renderFeature)->drawMeshInstanced(batchList, *state, context, list);
		}
	};

	if (list->instanceCount() <= 0) return;

	m_builder->setPrimitiveTopology(list->mesh()->sections()[list->sectionIndex()].topology);
	auto* element = m_builder->addNewDrawElement<DrawMeshInstanced>(m_manager->meshRenderFeature());
	element->list = list;
}

void CommandList::drawTextSprite(const StringView& text, const Color& color, const Vector2& anchor, SpriteBaseDirection baseDirection, detail::FontRequester* font)
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
	auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(m_manager->spriteTextRenderFeature());
	element->formattedText = formattedText;
	element->anchor = anchor;
	element->baseDirection = baseDirection;

	if (baseDirection != SpriteBaseDirection::Basic2D) {
		// is 3D.
		element->samplerState = detail::GraphicsManager::instance()->linearSamplerState();
	}
}

void CommandList::drawText(const StringView& text, const Rect& area, TextAlignment alignment/*, TextCrossAlignment crossAlignment*//*, const Color& color, Font* font*/)
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
	auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(m_manager->spriteTextRenderFeature());
	element->formattedText = formattedText;
	element->baseDirection = SpriteBaseDirection::Basic2D;

	// TODO: bounding
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void CommandList::drawChar(uint32_t codePoint, const Color& color, Font* font, const Matrix& transform)
{
	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawCharElement>(m_manager->spriteTextRenderFeature());
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

void CommandList::drawFlexGlyphRun(detail::FlexGlyphRun* glyphRun)
{
	m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	auto* element = m_builder->addNewDrawElement<detail::DrawTextElement>(m_manager->spriteTextRenderFeature());
	element->glyphRun = glyphRun;
	//element->flexText = makeRef<detail::FlexText>();	// TODO: cache
	//element->flexText->copyFrom(text);

	// TODO
	//detail::Sphere sphere;
	//detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &sphere);
	//ptr->setLocalBoundingSphere(sphere);
}

void CommandList::drawPath(CanvasContext* context)
{
	class DrawMeshInstanced : public detail::RenderDrawElement
	{
	public:
		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::PathRenderFeature*>(renderFeature)->draw(batchList, *state, context);
		}
	};

	auto* element = m_builder->addNewDrawElement<DrawMeshInstanced>(m_manager->pathRenderFeature());
}

//CanvasContext* CommandList::beginPath()
//{
//	if (LN_REQUIRE(!m_pathBegan)) return nullptr;
//	m_pathBegan = true;
//	return m_pathContext;
//}
//
//void CommandList::endPath()
//{
//	if (LN_REQUIRE(m_pathBegan)) return;
//	m_pathBegan = false;
//	m_commandList->drawPath(m_pathContext);
//}

void CommandList::invokeExtensionRendering(INativeGraphicsExtension* extension)
{
	class InvokeExtensionRendering : public detail::RenderDrawElement
	{
	public:
		INativeGraphicsExtension* extension;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::ExtensionRenderFeature*>(renderFeature)->invoke(context, batchList, extension);
			//context->drawExtension(extension);
		}
	};

	auto* element = m_builder->addNewDrawElement<InvokeExtensionRendering>(m_manager->extensionRenderFeature());
	element->extension = extension;

	// TODO: bounding
}

void CommandList::drawRegularPolygonPrimitive(int vertexCount, float radius, const Color& color, bool fill, const Matrix& localTransform)
{
	class DrawRegularPolygon2D : public detail::RenderDrawElement
	{
	public:
		detail::RegularPolygon2DGenerater data;

		virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const detail::RLIBatchState* state) override
		{
			return static_cast<detail::MeshGeneraterRenderFeature*>(renderFeature)->drawMeshGenerater(batchList, *state, &data);
		}
	};

	m_builder->setPrimitiveTopology(fill ? PrimitiveTopology::TriangleFan : PrimitiveTopology::LineStrip);
	auto* element = m_builder->addNewDrawElement<DrawRegularPolygon2D>(m_manager->meshGeneraterRenderFeature());
	element->data.vertices = vertexCount;
	element->data.radius = radius;
	element->data.color = color;
	element->data.fill = fill;
	element->data.setTransform(localTransform);

	// TODO: bouding box
}

RenderViewPoint* CommandList::viewPoint() const
{
	return m_builder->viewPoint();
}

void CommandList::setViewPoint(RenderViewPoint* value)
{
	m_builder->setViewPoint(value);
}

void CommandList::setBaseTransfrom(const Optional<Matrix>& value)
{
	m_builder->setBaseTransfrom(value);
}

const Matrix& CommandList::baseTransform() const
{
	return m_builder->baseTransform();
}

void CommandList::setRenderPriority(int value)
{
	m_builder->setRenderPriority(value);
}

void CommandList::setBaseBuiltinEffectData(const Optional<detail::BuiltinEffectData>& value)
{
	m_builder->setBaseBuiltinEffectData(value);
}

void CommandList::setAdditionalElementFlags(detail::RenderDrawElementTypeFlags value)
{
	m_builder->setAdditionalElementFlags(value);
}

void CommandList::setObjectId(int value)
{
	m_builder->setObjectId(value);
}



} // namespace ln

