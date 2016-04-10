
#include "Internal.h"
#include <Lumino/Assets.h>
#include "EngineManager.h"
#include "AssetsManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// AssetsManager
//=============================================================================

static AssetsManager* g_managerInstance = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AssetsManager* AssetsManager::GetInstance(AssetsManager* priority)
{
	return (priority == nullptr) ? g_managerInstance : priority;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AssetsManager::AssetsManager()
	: m_engineManager(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AssetsManager::~AssetsManager()
{



	if (g_managerInstance == this)
	{
		g_managerInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AssetsManager::Initialize(EngineManager* manager)
{
	m_engineManager = manager;

	m_textureCache = RefPtr<CacheManager>::MakeRef(32, 0);	// TODO


	if (g_managerInstance == nullptr)
	{
		g_managerInstance = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AssetsManager::Finalize()
{
	m_textureCache->Finalize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture2DPtr AssetsManager::LoadTexture(const StringRef& filePath)
{
	CacheKey key(filePath);
	Texture2D* ptr = (Texture2D*)m_textureCache->FindObjectAddRef(key);
	if (ptr != nullptr) { return Texture2DPtr(ptr, false); }

	Texture2DPtr ref = Texture2DPtr::MakeRef();
	ref->CreateCore(m_engineManager->GetGraphicsManager(), filePath, TextureFormat_R8G8B8A8, 1);

	m_textureCache->RegisterCacheObject(key, ref);
	return ref;
}


//=============================================================================
// Assets
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture2DPtr Assets::LoadTexture(const StringRef& filePath)
{
	return AssetsManager::GetInstance()->LoadTexture(filePath);
}

LN_NAMESPACE_END
