
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../../../Engine/src/Asset/AssetManager.hpp"
#include "MeshImporters/GLTFImporter.hpp"
#include "MeshImporters/VRMImporter.hpp"
#include "MeshImporters/FbxImporter.hpp"
#include "MeshImporters/ObjMeshImporter.hpp"
#include "MeshImporters/PmxImporter.hpp"
#include "MeshBoneMapper.hpp"
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

	m_meshModelCache.init(64, 0);

    LN_LOG_DEBUG << "MeshManager Initialization ended.";
}

void MeshManager::dispose()
{
	m_meshModelCache.dispose();

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
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::Position, 0);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::Normal, 0);
			vertexLayout->addElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 0);
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

Ref<MeshModel> MeshManager::acquireStaticMeshModel(const Path& filePath, float scale)
{
	static const Char* candidateExts[] = { _TT(".gltf"), _TT(".glb"), _TT(".pmx") };
	auto path = m_assetManager->findAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts));
	if (path) {
		return acquireStaticMeshModel(*path, scale);
	}
	else {
		LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
		return nullptr;
	}
}

Ref<MeshModel> MeshManager::acquireStaticMeshModel(const AssetPath& assetPath, float scale)
{
	uint64_t key = assetPath.calculateHash();
	auto mesh = m_meshModelCache.findObject(key);
	if (mesh) {
		return mesh;
	}
	else {
		mesh = makeObject<MeshModel>();
		loadStaticMeshModel(mesh, assetPath, scale);
		m_meshModelCache.registerObject(key, mesh, 0);
		return mesh;
	}
}

// TODO: deprecaed
//void MeshManager::loadStaticMeshModel(MeshModel* model, const Path& filePath, float scale)
//{
//	static const Char* candidateExts[] = { _TT(".gltf", _TT(".glb" };
//	auto path = m_assetManager->findAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts));
//	if (path) {
//		loadStaticMeshModel(model, *path, scale);
//	}
//	else {
//		LN_WARNING(_TT("Asset not found: " + String(filePath));    // TODO: operator
//	}
//}

void MeshManager::loadStaticMeshModel(MeshModel* model, const AssetPath& assetPath, float scale)
{
	LN_NOTIMPLEMENTED();
	{
		auto diag = makeObject<DiagnosticsManager>();
	 {

			GLTFImporter importer;
			importer.prepare(this, diag, nullptr);
			bool result = importer.onImportAsStaticMesh(model, assetPath);

			//ObjMeshImporter importer;
			//mesh = importer.import(filePath, scale, diag);

		}



		diag->dumpToLog();
	}

	model->m_filePath = assetPath;
	model->m_scale = scale;
}

Ref<SkinnedMeshModel> MeshManager::createSkinnedMeshModel(const Path& filePath, MeshImportSettings* settings)
{
	static const Char* candidateExts[] = { _TT(".gltf"), _TT(".glb"), _TT(".fbx") };
	auto path = m_assetManager->findAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts));
	if (path) {

		Ref<SkinnedMeshModel> mesh = makeObject<SkinnedMeshModel>();
		mesh->m_name = filePath.fileNameWithoutExtension();
		auto diag = makeObject<DiagnosticsManager>();

		if (path->path().hasExtension(_TT(".pmx"))) {
			PmxLoader importer(this, diag);
			bool result = importer.load(mesh, *path, false);
			mesh->m_animationController = makeObject<AnimationController>(mesh);
		}
		else if (path->path().hasExtension(_TT(".fbx"))) {
#ifdef LN_USE_FBX_IMPORTER
			FbxImporter importer;
			mesh = importer.importSkinnedMesh(m_assetManager, *path, diag);
#else
			LN_ERROR(_TT("FBX not supported."));
			return nullptr;
#endif
		}
		else
		{

			//GLTFImporter importer;
			VRMImporter importer;
			importer.prepare(this, diag, settings);
			bool result = importer.onImportAsSkinnedMesh(mesh, *path);


			if (!mesh->skeletons().isEmpty()) {
				mesh->m_animationController = makeObject<AnimationController>(mesh);

				for (auto& clip : importer.animationClips()) {
					mesh->m_animationController->addClip(clip);
				}
			}

			if (importer.m_applyBoneTransformationsEnabled) {
				MeshDiag::clearBoneInitialRotations(mesh);
			}

		}

		mesh->calculateBoundingBox();

		MeshBoneMapper boneMapper;
		boneMapper.map(mesh);

		if (mesh->m_animationController) {
			

			if (MeshNode* root = mesh->findHumanoidBone(HumanoidBones::Hips)) {
				mesh->m_animationController->core()->m_animationTranslationBasis = root->initialLocalTransform().position().y;
			}
		}

		diag->dumpToLog();
		return mesh;
	}
	else {
		LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
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
		diag->reportWarning(String::format(_TT("Texture file not found: {0}"), path.str()));
		return nullptr;
	}

    return Texture2D::load(path);
	//return m_assetManager->loadTexture(path);
}

void MeshManager::collectUnreferenceObjects()
{
	m_meshModelCache.collectUnreferenceObjects(false);
}

} // namespace detail
} // namespace ln

