/*
	現状 Effekseer だけ対応しているが、今後複数のツールを同時使用とか考えると
	キャッシュは Manager ではなくこちらに持たせておいた方が都合がいい。
*/
#include "Internal.h"
#include "EffectDriver.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// EffectEngine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectEngine::Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize)
{
	m_manager = manager;
	m_effectCoreCache = LN_NEW CacheManager(cacheObjectCount, cacheMemorySize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectEngine::Finalize()
{
	if (m_effectCoreCache != NULL) {
		m_effectCoreCache->Finalize();
		LN_SAFE_RELEASE(m_effectCoreCache);
	}
}

//-----------------------------------------------------------------------------
// Deferred 描画		: 描画コマンドから実行される
// Immediate 描画	: 直接実行される
//-----------------------------------------------------------------------------
void EffectEngine::Render()
{

}

//=============================================================================
// EffectInstance
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectInstance::Initialize(EffectManager* manager)
{
	m_manager = manager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void EffectInstance::AdvanceTime(float deltaTime)
//{
//	m_deltaTime += deltaTime;
//}

} // namespace detail
LN_NAMESPACE_END

