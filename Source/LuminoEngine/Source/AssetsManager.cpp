
#include "Internal.h"
#include <Lumino/Assets.h>
#include "IO/Archive.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Text/FreeTypeFont.h"
#include "Mesh/ModelManager.h"
#include "EngineManager.h"
#include "AssetsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AssetsManager
//==============================================================================

static AssetsManager* g_managerInstance = nullptr;

//------------------------------------------------------------------------------
AssetsManager* AssetsManager::getInstance(AssetsManager* priority)
{
	return (priority == nullptr) ? g_managerInstance : priority;
}

//------------------------------------------------------------------------------
AssetsManager::AssetsManager()
	: m_engineManager(nullptr)
{
}

//------------------------------------------------------------------------------
AssetsManager::~AssetsManager()
{
	if (g_managerInstance == this)
	{
		g_managerInstance = nullptr;
	}
}

//------------------------------------------------------------------------------
void AssetsManager::initialize(EngineManager* manager)
{
	m_engineManager = manager;
	m_archiveManager = m_engineManager->getArchiveManager();

	//addAssetsDirectory(PathName::getCurrentDirectory());

	m_textureCache = Ref<CacheManager>::makeRef(32, 0);
	//m_shaderCache = Ref<CacheManager>::makeRef(32, 0);
	m_fontCache = Ref<CacheManager>::makeRef(32, 0);

	if (g_managerInstance == nullptr)
	{
		g_managerInstance = this;
	}
}

//------------------------------------------------------------------------------
void AssetsManager::dispose()
{
	m_textureCache->finalizeCache();
	//m_shaderCache->finalizeCache();
	m_fontCache->finalizeCache();
}

//------------------------------------------------------------------------------
void AssetsManager::addAssetsDirectory(const StringRef& directoryPath)
{
	m_archiveManager->addAssetsDirectory(directoryPath);
	//AssetsDirectory dir;
	//dir.path = PathName(directoryPath);
	//m_assetsDirectories.add(dir);
}

//------------------------------------------------------------------------------
Texture2DPtr AssetsManager::loadTexture(const StringRef& filePath)
{
	// TODO: Stream 指定
	auto path = m_archiveManager->findLocalFilePath(filePath);

	CacheKey key(path.toString());
	Texture2D* ptr = static_cast<Texture2D*>(m_textureCache->findObjectAddRef(key));
	if (ptr != nullptr) { return Texture2DPtr(ptr, false); }

	// TODO: mipmap ON にしたい。ただ、サンプラステートを変更しないとならないようなので動作確認してから。
	auto ref = newObject<Texture2D>(path, TextureFormat::R8G8B8A8, false);

	m_textureCache->registerCacheObject(key, ref);
	return ref;
}

//------------------------------------------------------------------------------
Ref<Shader> AssetsManager::loadShader(const StringRef& filePath)
{
	// TODO: キャッシュ
	// TODO: Stream 指定
	auto path = m_archiveManager->findLocalFilePath(filePath);
	return Shader::create(path);
}

//------------------------------------------------------------------------------
Ref<StaticMeshModel> AssetsManager::loadMeshModel(const StringRef& filePath)
{
	// TODO: キャッシュ
	// TODO: Stream 指定
	auto path = m_archiveManager->findLocalFilePath(filePath);
	return detail::EngineDomain::getModelManager()->createStaticMeshModel(path);
}

//------------------------------------------------------------------------------
String AssetsManager::loadText(const StringRef& filePath)
{
	auto stream = m_archiveManager->createFileStream(filePath, false);
	return FileSystem::readAllText(stream);
}

//------------------------------------------------------------------------------
Ref<Stream> AssetsManager::openFile(const StringRef& filePath)
{
	return m_archiveManager->createFileStream(filePath, false);
}

////------------------------------------------------------------------------------
//void AssetsManager::makeSearchPath(const StringRef& path)
//{
//	if (PathTraits::isAbsolutePath(path.getBegin(), path.getLength()))
//	{
//		for (AssetsDirectory& dir : m_assetsDirectories)
//		{
//			dir.searchPath = PathName(path);
//		}
//	}
//	else
//	{
//		for (AssetsDirectory& dir : m_assetsDirectories)
//		{
//			dir.searchPath = PathName(dir.path, path);
//		}
//	}
//}
//
////------------------------------------------------------------------------------
//const PathName* AssetsManager::findLocalFilePath()
//{
//	for (AssetsDirectory& dir : m_assetsDirectories)
//	{
//		if (dir.searchPath.existsFile())
//		{
//			return &dir.searchPath;
//		}
//	}
//	return nullptr;
//}

//------------------------------------------------------------------------------
// Note: isDeferring は今のところ Sound の遅延読み込み用のもの。
// ディクス上のファイルから FileStream を作るときに使用する。
//Stream* AssetsManager::createFileStream(const StringRef& filePath, bool isDeferring)
//{
//	// TODO: this は const にしたい。String::Replace とかと同じ。
//	PathName absPath = filePath;
//	absPath.CanonicalizePath();
//
//	Ref<Stream> stream;
//	for (IArchive* archive : m_archiveList)
//	{
//		if (archive->tryCreateStream(absPath, &stream, isDeferring)) {
//			break;
//		}
//	}
//
//	LN_THROW(stream != NULL, FileNotFoundException, absPath);	// ファイルが見つからなかった
//	return stream.DetachMove();
//}

//------------------------------------------------------------------------------
//RawFontPtr AssetsManager::LoadFont(const StringRef& name, int size, bool isBold, bool isItalic, bool isAntiAlias)
//{
//	CacheKey key(String::Format(_T("{0}-{1}{2}{3}{4}"), name, size, isBold, isItalic, isAntiAlias));
//
//	RawFont* ptr = static_cast<RawFont*>(m_fontCache->FindObjectAddRef(key));
//	if (ptr != nullptr) { return RawFontPtr(ptr, false); }
//
//	RawFontPtr ref;
//	if (name.IsEmpty())
//	{
//		ref = m_engineManager->getGraphicsManager()->getFontManager()->getDefaultFont()->Copy();
//	}
//	else
//	{
//		auto ftFont = Ref<FreeTypeFont>::MakeRef(m_engineManager->getGraphicsManager()->getFontManager());
//		ref = ftFont;
//	}
//
//	ref->setName(name);
//	ref->setSize(size);
//	ref->setBold(isBold);
//	ref->setItalic(isItalic);
//	ref->setAntiAlias(isAntiAlias);
//
//	m_fontCache->RegisterCacheObject(key, ref);
//	return RawFontPtr::StaticCast(ref);
//}

//==============================================================================
// Assets
//==============================================================================
const String Assets::standardContentsDirectory = _T("CommonAssets");

//------------------------------------------------------------------------------
void Assets::addAssetsDirectory(const StringRef& directoryPath)
{
	AssetsManager::getInstance()->addAssetsDirectory(directoryPath);
}

//------------------------------------------------------------------------------
Texture2DPtr Assets::loadTexture(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadTexture(filePath);
}

//------------------------------------------------------------------------------
Ref<Shader> Assets::loadShader(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadShader(filePath);
}

//------------------------------------------------------------------------------
Ref<StaticMeshModel> Assets::loadMeshModel(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadMeshModel(filePath);
}

//------------------------------------------------------------------------------
String Assets::loadText(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadText(filePath);
}

LN_NAMESPACE_END
