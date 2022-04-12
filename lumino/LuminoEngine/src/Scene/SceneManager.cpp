
#include "Internal.hpp"
#include "SceneManager.hpp"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoEngine/Scene/Raycaster.hpp>
#include "../Rendering/RenderingManager.hpp"
#include "Shapes/PathShapeContext.hpp"

// for registerType
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/Tilemap/Tilemap.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMesh.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitives.hpp>

namespace ln {
namespace detail {

//==============================================================================
// SceneManager

SceneManager::SceneManager()
    : m_activeWorld(nullptr)
	, m_raycasterCache(makeObject<Raycaster>())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
    LN_LOG_DEBUG("SceneManager Initialization started.");


	// SkyFromAtmosphere.lcfx.h
	{
		static const unsigned char data[] =
		{
#include "Resource/SkyFromAtmosphere.lcfx.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_atmosphereShader = makeObject<Shader>(_TT("SkyFromAtmosphere"), &stream);
	}


	auto createTexture = [](const uint8_t* data, size_t len) {
		MemoryStream stream(data, len);
		auto bmp = makeObject<Bitmap2D>();
		bmp->load(&stream);
		return makeObject<Texture2D>(bmp, TextureFormat::RGBA8);
	};

	// SkydomeCloudA
	{
		static const unsigned char data[] = {
#include "Resource/SkydomeCloudA.png.inl"
		};
		SkydomeCloudA = createTexture(data, LN_ARRAY_SIZE_OF(data));
	}
	// SkydomeCloudB
	{
		static const unsigned char data[] = {
#include "Resource/SkydomeCloudB.png.inl"
		};
		SkydomeCloudB = createTexture(data, LN_ARRAY_SIZE_OF(data));
	}
	// SkydomeCloudC
	{
		static const unsigned char data[] = {
#include "Resource/SkydomeCloudC.png.inl"
		};
		SkydomeCloudC = createTexture(data, LN_ARRAY_SIZE_OF(data));
	}
	// SkydomeCloudR
	{
		static const unsigned char data[] = {
#include "Resource/SkydomeCloudR.png.inl"
		};
		SkydomeCloudR = createTexture(data, LN_ARRAY_SIZE_OF(data));
	}

	m_primitiveMeshDefaultMaterial = makeObject<Material>();
	m_primitiveMeshDefaultMaterial->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_primitiveMeshDefaultMaterial->setRoughness(0.5f);
	m_primitiveMeshDefaultMaterial->setMetallic(0.0f);

	m_pathContext = PathShape_NVGcontext::nvgCreate(0);

    auto* context = EngineContext2::instance();
    context->registerType<World>();
    context->registerType<Level>();
    context->registerType<WorldObject>();
    context->registerType<VisualObject>();
    context->registerType<Sprite>();

    context->registerType<Tileset>();
    context->registerType<TilemapLayer>();
    context->registerType<TilemapModel>();
    context->registerType<TilemapComponent>();
    context->registerType<Tilemap>();

	context->registerType<StaticMesh>();

	context->registerType<PlaneMesh>();
	context->registerType<PlaneMeshComponent>();

    LN_LOG_DEBUG("SceneManager Initialization ended.");
}

void SceneManager::dispose()
{
	if (m_pathContext) {
		PathShape_NVGcontext::nvgDelete(m_pathContext.get());
		m_pathContext = nullptr;
	}
}

Shader* SceneManager::skydomeShader() const
{
    return detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::SkyDome);
}

Level* SceneManager::loadScene(const StringView& sceneAssetFilePath)
{
    auto scene = dynamic_pointer_cast<Level>(Assets::loadAsset(sceneAssetFilePath));
    activeWorld()->addScene(scene);
    return nullptr;
}

void SceneManager::unloadScene(Level* scene)
{
    activeWorld()->m_sceneList->remove(scene);
}

void SceneManager::updateFrame()
{
	//executeCommands();
	//if (m_activeScene != nullptr)
	//{
	//	m_activeScene->update();
	//}
}

int SceneManager::getWorldObjectId()
{
	if (!m_objectIndexStack.empty()) {
		int newId = m_objectIndexStack.top();
		m_objectIndexStack.pop();
		return newId;
	}
	else {
		m_objectIndexCount++;
		return m_objectIndexCount;
	}
}

void SceneManager::releaseWorldObjectId(int id)
{
	m_objectIndexStack.push(id);
}

Ref<WorldObject> SceneManager::instantiateObjectFromAnyFile(const Path& filePath) const
{
	const StringView& ext = filePath.extension();
	
	if (std::find_if(
		MeshHelper::CandidateExtensions_MeshModel.begin(), MeshHelper::CandidateExtensions_MeshModel.end(),
		[&](const Char* x) { return ext.endsWith(x, CaseSensitivity::CaseInsensitive); }) != MeshHelper::CandidateExtensions_MeshModel.end()) {
		return StaticMesh::load(filePath);
	}

	return nullptr;
}

} // namespace detail
} // namespace ln
