
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../Asset/AssetManager.hpp"
#include "GLTFImporter.hpp"
#include "FbxImporter.hpp"
#include "ObjMeshImporter.hpp"
#include "PmxImporter.hpp"
#include "MeshManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshManager
	
static const unsigned char toon01Data[] =
{
#include "Resource/toon01.png.h"
};
static const size_t toon01DataLen = LN_ARRAY_SIZE_OF(toon01Data);

static const unsigned char toon02Data[] =
{
#include "Resource/toon02.png.h"
};
static const size_t toon02DataLen = LN_ARRAY_SIZE_OF(toon02Data);

static const unsigned char toon03Data[] =
{
#include "Resource/toon03.png.h"
};
static const size_t toon03DataLen = LN_ARRAY_SIZE_OF(toon03Data);

static const unsigned char toon04Data[] =
{
#include "Resource/toon04.png.h"
};
static const size_t toon04DataLen = LN_ARRAY_SIZE_OF(toon04Data);

static const unsigned char toon05Data[] =
{
#include "Resource/toon05.png.h"
};
static const size_t toon05DataLen = LN_ARRAY_SIZE_OF(toon05Data);

static const unsigned char toon06Data[] =
{
#include "Resource/toon06.png.h"
};
static const size_t toon06DataLen = LN_ARRAY_SIZE_OF(toon06Data);

static const unsigned char toon07Data[] =
{
#include "Resource/toon07.png.h"
};
static const size_t toon07DataLen = LN_ARRAY_SIZE_OF(toon07Data);

static const unsigned char toon08Data[] =
{
#include "Resource/toon08.png.h"
};
static const size_t toon08DataLen = LN_ARRAY_SIZE_OF(toon08Data);

static const unsigned char toon09Data[] =
{
#include "Resource/toon09.png.h"
};
static const size_t toon09DataLen = LN_ARRAY_SIZE_OF(toon09Data);

static const unsigned char toon10Data[] =
{
#include "Resource/toon10.png.h"
};
static const size_t toon10DataLen = LN_ARRAY_SIZE_OF(toon10Data);

MeshManager::MeshManager()
	: m_graphicsManager(nullptr)
	//, m_predefinedVertexLayouts{}
{
}

void MeshManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "MeshManager Initialization started.";

	m_graphicsManager = settings.graphicsManager;
	m_assetManager = settings.assetManager;

	//{
	//	static VertexElement elements[] =
	//	{
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
	//		{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
	//		{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
	//	};
	//	m_predefinedVertexLayouts[PredefinedVertexLayout_Standard] = makeObject<VertexLayout>(elements, 4);
	//}

	//{
	//	static VertexElement elements[] =
	//	{
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
	//		{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
	//		{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
	//		{ 1, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0 },
	//		{ 1, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0 },
	//	};
	//	m_predefinedVertexLayouts[PredefinedVertexLayout_Standard_BlendWeight] = makeObject<VertexLayout>(elements, 4);
	//}

#define LN_CREATE_MMD_TOON_TEXTURE(index, toonData, toonDataLen) \
    { \
        MemoryStream data(toonData, toonDataLen); \
        auto bmp = makeObject<Bitmap2D>(); \
        bmp->load(&data); \
        m_mmdDefaultToonTexture[index] = makeObject<Texture2D>(bmp, TextureFormat::RGBA8); \
    }
    LN_CREATE_MMD_TOON_TEXTURE(0, toon01Data, toon01DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(1, toon02Data, toon02DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(2, toon03Data, toon03DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(3, toon04Data, toon04DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(4, toon05Data, toon05DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(5, toon06Data, toon06DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(6, toon07Data, toon07DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(7, toon08Data, toon08DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(8, toon09Data, toon09DataLen);
    LN_CREATE_MMD_TOON_TEXTURE(9, toon10Data, toon10DataLen);
#undef LN_CREATE_MMD_TOON_TEXTURE

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();

    LN_LOG_DEBUG << "MeshManager Initialization ended.";
}

void MeshManager::dispose()
{
    for (auto tex : m_mmdDefaultToonTexture)
    {
        tex.reset();
    }

	m_predefinedVertexLayouts.clear();
}

VertexLayout* MeshManager::getPredefinedVertexLayout(PredefinedVertexLayoutFlags flags)
{
	auto itr = m_predefinedVertexLayouts.find(flags);
	if (itr != m_predefinedVertexLayouts.end()) {
		return itr->second;
	}
	else {
		auto vertexLayout = makeObject<VertexLayout>();
		int stream = 0;

		// BasicVertices
		if ((flags & PredefinedVertexLayoutFlags_Geometry) != 0)
		{
			vertexLayout->addElement(stream, VertexElementType::Float3, VertexElementUsage::Position, 0);
			vertexLayout->addElement(stream, VertexElementType::Float3, VertexElementUsage::Normal, 0);
			vertexLayout->addElement(stream, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::Color, 0);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::Tangent, 0);
			++stream;
		}

		// BlendWeights
		if ((flags & PredefinedVertexLayoutFlags_BlendWeights) != 0)
		{
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0);
			++stream;
		}

		// AdditionalUVs
		if ((flags & PredefinedVertexLayoutFlags_AdditionalUV) != 0)
		{
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 1);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 2);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 3);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 4);
			++stream;
		}

		// SdefInfo
		if ((flags & PredefinedVertexLayoutFlags_SdefInfo) != 0)
		{
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 5);
			vertexLayout->addElement(stream, VertexElementType::Float3, VertexElementUsage::TexCoord, 6);
			vertexLayout->addElement(stream, VertexElementType::Float3, VertexElementUsage::TexCoord, 7);
			++stream;
		}

		// MmdExtra
		if ((flags & PredefinedVertexLayoutFlags_MmdExtra) != 0)
		{
			vertexLayout->addElement(stream, VertexElementType::Float1, VertexElementUsage::TexCoord, 8);
			vertexLayout->addElement(stream, VertexElementType::Float1, VertexElementUsage::PointSize, 15);
			++stream;
		}

		m_predefinedVertexLayouts.insert({ flags, vertexLayout });

		return vertexLayout;
	}
}

Ref<StaticMeshModel> MeshManager::createStaticMeshModel(const Path& filePath, float scale)
{
	static const Char* candidateExts[] = { u".gltf", u".glb" };
	auto path = m_assetManager->findAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts));
	if (path) {

		Ref<StaticMeshModel> mesh;

		{
			auto diag = makeObject<DiagnosticsManager>();

			GLTFImporter importer;
			mesh = importer.import(m_assetManager, *path, diag);

			//ObjMeshImporter importer;
			//mesh = importer.import(filePath, scale, diag);

			diag->dumpToLog();
		}

		return mesh;
	}
	else {
		LN_WARNING(u"Asset not found: " + String(filePath));    // TODO: operator
		return nullptr;
	}

}

Ref<SkinnedMeshModel> MeshManager::createSkinnedMeshModel(const Path& filePath, float scale)
{
	static const Char* candidateExts[] = { u".gltf", u".glb", u".fbx" };
	auto path = m_assetManager->findAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts));
	if (path) {

		Ref<SkinnedMeshModel> mesh;
		auto diag = makeObject<DiagnosticsManager>();

		if (path->path().hasExtension(u".fbx")) {
#ifdef LN_USE_FBX_IMPORTER
			FbxImporter importer;
			mesh = importer.importSkinnedMesh(m_assetManager, *path, diag);
#else
			LN_ERROR(u"FBX not supported.");
			return nullptr;
#endif
		}
		else
		{

			GLTFImporter importer;
			mesh = importer.importSkinnedMesh(m_assetManager, *path, diag);

		}

		diag->dumpToLog();
		return mesh;
	}
	else {
		LN_WARNING(u"Asset not found: " + String(filePath));    // TODO: operator
		return nullptr;
		// TODO: deprecated

		//Ref<MeshResource> mesh;

		//PmxLoader  importer;
		//auto file = FileStream::create(filePath);
		//auto mesh = importer.load(this, file, filePath.parent(), false);

		//return mesh->createSkinnedMeshModel();
	}
}

Ref<Texture> MeshManager::createTexture(const Path& parentDir, const StringRef& filePath, DiagnosticsManager* diag)
{
	Path path(parentDir, filePath);

	if (!m_assetManager->existsFile(path)) {
		diag->reportWarning(String::format(u"Texture file not found: {0}", path));
		return nullptr;
	}

    return Texture2D::load(path);
	//return m_assetManager->loadTexture(path);
}

} // namespace detail
} // namespace ln

