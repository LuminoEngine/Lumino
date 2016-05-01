
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/Effect/Common.h>
#include "../Graphics/RenderingCommand.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class EffectManager;

// Effect の管理
class EffectEngine
	: public RefObject
{
public:
	EffectEngine() = default;
	virtual ~EffectEngine() = default;

	void Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize);
	virtual void Finalize();
	virtual VisualEffect* CreateEffectCore(const PathName& filePath) = 0;
	virtual void SetViewProjectin(const Matrix& view, const Matrix& proj) = 0;
	virtual void UpdateFrame(float elapsedTime) = 0;
	virtual void Render() = 0;

	// Render() は、
	// Deferred 描画		: 描画コマンドから実行される
	// Immediate 描画	: 直接実行される

protected:
	EffectManager*	m_manager = nullptr;
	CacheManager*	m_effectCoreCache = nullptr;

public:
	// 一括描画の描画コマンド
	struct RenderCommand : public RenderingCommand
	{
		EffectEngine*	m_engine;
		tr::Task* m_updateTask;
		void Create(EffectEngine* engine, tr::Task* updateTask)
		{
			m_engine = engine;
			m_updateTask = updateTask;
		}
		void Execute()
		{
			m_updateTask->Wait();
			m_engine->Render();
		}
	};
};

//// エフェクトオブジェクト Wrapper。キャッシュ管理される。
//class EffectCore
//	: public RefObject
//	, public ICacheObject
//{
//	LN_CACHE_OBJECT_DECL;
//public:
//	EffectCore();
//	virtual ~EffectCore() = default;
//};
//
//// エフェクトインスタンス Wrapper
//class EffectInstance
//	: public RefObject
//{
//public:
//	EffectInstance() = default;
//	virtual ~EffectInstance() = default;
//
//	void Initialize(EffectManager* manager);
//	void SetWorldMatrix(const Matrix& matrix) { m_worldMatrix = matrix; }
//	//void AdvanceTime(float deltaTime);
//
//	//virtual void Play(bool overlap) = 0;
//	virtual void Stop() = 0;
//	virtual bool IsPlaying() = 0;
//
//	virtual void UpdateFrame() = 0;	// 更新スレッド
//	virtual void Draw() = 0;	// 描画スレッド
//
//protected:
//	EffectManager*	m_manager = nullptr;
//	Matrix			m_worldMatrix;
//	//float			m_deltaTime = 0;
//};


} // namespace detail
LN_NAMESPACE_END
