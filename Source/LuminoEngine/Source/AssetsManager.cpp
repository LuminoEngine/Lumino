
#include "Internal.h"
#include <Lumino/Assets.h>
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

	addAssetsDirectory(PathName::getCurrentDirectory());

	m_textureCache = RefPtr<CacheManager>::makeRef(32, 0);	// TODO
	m_fontCache = RefPtr<CacheManager>::makeRef(32, 0);	// TODO

	if (g_managerInstance == nullptr)
	{
		g_managerInstance = this;
	}
}

//------------------------------------------------------------------------------
void AssetsManager::Finalize()
{
	m_textureCache->finalizeCache();
	m_fontCache->finalizeCache();
}

//------------------------------------------------------------------------------
void AssetsManager::addAssetsDirectory(const StringRef& directoryPath)
{
	AssetsDirectory dir;
	dir.path = PathName(directoryPath);
	m_assetsDirectories.add(dir);
}

//------------------------------------------------------------------------------
Texture2DPtr AssetsManager::loadTexture(const StringRef& filePath)
{
	// TODO: 暫定処置
	makeSearchPath(filePath);
	auto* path = findLocalFilePath();

	CacheKey key(*path);
	Texture2D* ptr = static_cast<Texture2D*>(m_textureCache->findObjectAddRef(key));
	if (ptr != nullptr) { return Texture2DPtr(ptr, false); }

	// TODO: mipmap ON にしたい。ただ、サンプラステートを変更しないとならないようなので動作確認してから。
	auto ref = newObject<Texture2D>(*path, TextureFormat::R8G8B8A8, false);

	m_textureCache->registerCacheObject(key, ref);
	return ref;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> AssetsManager::loadMeshModel(const StringRef& filePath)
{
	// TODO: 暫定処置
	makeSearchPath(filePath);
	auto* path = findLocalFilePath();

	// TODO: キャッシュ
	return detail::EngineDomain::getModelManager()->createStaticMeshModel(*path);
}

//------------------------------------------------------------------------------
String AssetsManager::loadText(const StringRef& filePath)
{
	makeSearchPath(filePath);
	auto* path = findLocalFilePath();
	return FileSystem::readAllText(*path);
}

//------------------------------------------------------------------------------
RefPtr<Stream> AssetsManager::openFile(const StringRef& filePath)
{
	makeSearchPath(filePath);
	auto* path = findLocalFilePath();
	//if (path == nullptr)
	//{
	//	LN_LOG_ERROR << _T("file not found: ") << filePath;
	//	return nullptr;
	//}

	return FileStream::create(*path, FileOpenMode::read);
}

//------------------------------------------------------------------------------
void AssetsManager::makeSearchPath(const StringRef& path)
{
	if (PathTraits::isAbsolutePath(path.getBegin(), path.getLength()))
	{
		for (AssetsDirectory& dir : m_assetsDirectories)
		{
			dir.searchPath = PathName(path);
		}
	}
	else
	{
		for (AssetsDirectory& dir : m_assetsDirectories)
		{
			dir.searchPath = PathName(dir.path, path);
		}
	}
}

//------------------------------------------------------------------------------
const PathName* AssetsManager::findLocalFilePath()
{
	for (AssetsDirectory& dir : m_assetsDirectories)
	{
		if (dir.searchPath.existsFile())
		{
			return &dir.searchPath;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
// Note: isDeferring は今のところ Sound の遅延読み込み用のもの。
// ディクス上のファイルから FileStream を作るときに使用する。
//Stream* AssetsManager::createFileStream(const StringRef& filePath, bool isDeferring)
//{
//	// TODO: this は const にしたい。String::Replace とかと同じ。
//	PathName absPath = filePath;
//	absPath.CanonicalizePath();
//
//	RefPtr<Stream> stream;
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
//		auto ftFont = RefPtr<FreeTypeFont>::MakeRef(m_engineManager->getGraphicsManager()->getFontManager());
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
RefPtr<StaticMeshModel> Assets::loadMeshModel(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadMeshModel(filePath);
}

//------------------------------------------------------------------------------
String Assets::loadText(const StringRef& filePath)
{
	return AssetsManager::getInstance()->loadText(filePath);
}

LN_NAMESPACE_END
