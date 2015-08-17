
#include "../Internal.h"
#include "PMXLoader.h"
#include "VMDLoader.h"
#include "ModelManager.h"

namespace Lumino
{
namespace Modeling
{

//=============================================================================
// ModelManager
//=============================================================================

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelManager::ModelManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelManager::~ModelManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelManager::Initialize(const ConfigData& configData)
{
	m_fileManager = configData.FileManager;
	m_physicsManager = configData.PhysicsManager;
	m_graphicsManager = configData.GraphicsManager;

	m_mmdDefaultToonTexture[0].Attach(Graphics::Texture::Create(toon01Data, toon01DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[1].Attach(Graphics::Texture::Create(toon02Data, toon02DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[2].Attach(Graphics::Texture::Create(toon03Data, toon03DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[3].Attach(Graphics::Texture::Create(toon04Data, toon04DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[4].Attach(Graphics::Texture::Create(toon05Data, toon05DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[5].Attach(Graphics::Texture::Create(toon06Data, toon06DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[6].Attach(Graphics::Texture::Create(toon07Data, toon07DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[7].Attach(Graphics::Texture::Create(toon08Data, toon08DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[8].Attach(Graphics::Texture::Create(toon09Data, toon09DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
	m_mmdDefaultToonTexture[9].Attach(Graphics::Texture::Create(toon10Data, toon10DataLen, Graphics::TextureFormat_B8G8R8A8, 1, m_graphicsManager));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelManager::Finalize()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Graphics::Texture* ModelManager::GetMMDDefaultToonTexture(int index)
{ 
	LN_ASSERT(0 <= index && index < 10);
	return m_mmdDefaultToonTexture[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelCore* ModelManager::CreateModelCore(const PathName& filePath)
{
	RefPtr<Stream> stream(m_fileManager->CreateFileStream(filePath));

	PMXLoader loader;
	RefPtr<ModelCore> modelCore(loader.Load(this, stream, filePath.GetParent(), true));
	
	modelCore->RefreshInitialValues();
	modelCore.SafeAddRef();
	return modelCore;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Animation::AnimationClip* ModelManager::CreateMotion(const PathName& filePath)
{
	RefPtr<Stream> stream(m_fileManager->CreateFileStream(filePath));
	VMDLoader loader;
	if (!loader.Load(stream)) {
		LN_THROW(0, InvalidFormatException);
	}

	// TODO: ボーンと表情のアニメーションは分けた方が良い気がする…
	RefPtr<Animation::AnimationClip> clip(LN_NEW Animation::AnimationClip());
	LN_FOREACH(VMDLoader::BoneAnimation& anim, loader.GetBoneAnimationList())
	{
		clip->AddAnimationCurve(anim.TargetBoneName, anim.AnimationCurve);
	}
	LN_FOREACH(VMDLoader::FaceAnimation& anim, loader.GetFaceAnimationList())
	{
		clip->AddAnimationCurve(anim.TargetFaceName, anim.AnimationCurve);
	}
	clip.SafeAddRef();
	return clip;
}

} // namespace Modeling
} // namespace Lumino
