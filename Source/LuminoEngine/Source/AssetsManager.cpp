
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
AssetsManager* AssetsManager::GetInstance(AssetsManager* priority)
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

	AddAssetsDirectory(PathName::getCurrentDirectory());

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
void AssetsManager::AddAssetsDirectory(const StringRef& directoryPath)
{
	AssetsDirectory dir;
	dir.path = PathName(directoryPath);
	m_assetsDirectories.add(dir);
}

//------------------------------------------------------------------------------
Texture2DPtr AssetsManager::LoadTexture(const StringRef& filePath)
{
	// TODO: 暫定処置
	MakeSearchPath(filePath);
	auto* path = FindLocalFilePath();

	CacheKey key(*path);
	Texture2D* ptr = static_cast<Texture2D*>(m_textureCache->findObjectAddRef(key));
	if (ptr != nullptr) { return Texture2DPtr(ptr, false); }

	// TODO: mipmap ON にしたい。ただ、サンプラステートを変更しないとならないようなので動作確認してから。
	auto ref = NewObject<Texture2D>(*path, TextureFormat::R8G8B8A8, false);

	m_textureCache->registerCacheObject(key, ref);
	return ref;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> AssetsManager::LoadMeshModel(const StringRef& filePath)
{
	// TODO: 暫定処置
	MakeSearchPath(filePath);
	auto* path = FindLocalFilePath();

	// TODO: キャッシュ
	return detail::EngineDomain::GetModelManager()->CreateStaticMeshModel(*path);
}

//------------------------------------------------------------------------------
String AssetsManager::LoadText(const StringRef& filePath)
{
	MakeSearchPath(filePath);
	auto* path = FindLocalFilePath();
	return FileSystem::readAllText(*path);
}

//------------------------------------------------------------------------------
RefPtr<Stream> AssetsManager::OpenFile(const StringRef& filePath)
{
	MakeSearchPath(filePath);
	auto* path = FindLocalFilePath();
	//if (path == nullptr)
	//{
	//	LN_LOG_ERROR << _T("file not found: ") << filePath;
	//	return nullptr;
	//}

	return FileStream::create(*path, FileOpenMode::read);
}

//------------------------------------------------------------------------------
void AssetsManager::MakeSearchPath(const StringRef& path)
{
	if (PathTraits::IsAbsolutePath(path.getBegin(), path.getLength()))
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
const PathName* AssetsManager::FindLocalFilePath()
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
//Stream* AssetsManager::CreateFileStream(const StringRef& filePath, bool isDeferring)
//{
//	// TODO: this は const にしたい。String::Replace とかと同じ。
//	PathName absPath = filePath;
//	absPath.CanonicalizePath();
//
//	RefPtr<Stream> stream;
//	for (IArchive* archive : m_archiveList)
//	{
//		if (archive->TryCreateStream(absPath, &stream, isDeferring)) {
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
//		ref = m_engineManager->GetGraphicsManager()->GetFontManager()->GetDefaultFont()->Copy();
//	}
//	else
//	{
//		auto ftFont = RefPtr<FreeTypeFont>::MakeRef(m_engineManager->GetGraphicsManager()->GetFontManager());
//		ref = ftFont;
//	}
//
//	ref->SetName(name);
//	ref->SetSize(size);
//	ref->SetBold(isBold);
//	ref->SetItalic(isItalic);
//	ref->SetAntiAlias(isAntiAlias);
//
//	m_fontCache->RegisterCacheObject(key, ref);
//	return RawFontPtr::StaticCast(ref);
//}

//==============================================================================
// Assets
//==============================================================================

//------------------------------------------------------------------------------
void Assets::AddAssetsDirectory(const StringRef& directoryPath)
{
	AssetsManager::GetInstance()->AddAssetsDirectory(directoryPath);
}

//------------------------------------------------------------------------------
Texture2DPtr Assets::LoadTexture(const StringRef& filePath)
{
	return AssetsManager::GetInstance()->LoadTexture(filePath);
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> Assets::LoadMeshModel(const StringRef& filePath)
{
	return AssetsManager::GetInstance()->LoadMeshModel(filePath);
}

//------------------------------------------------------------------------------
String Assets::LoadText(const StringRef& filePath)
{
	return AssetsManager::GetInstance()->LoadText(filePath);
}

LN_NAMESPACE_END
