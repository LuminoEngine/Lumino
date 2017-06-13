
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

	void initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize);
	virtual void Finalize();
	virtual VisualEffect* CreateEffectCore(const PathName& filePath) = 0;
	virtual void SetViewProjectin(const Matrix& view, const Matrix& proj) = 0;
	virtual void updateFrame(float elapsedTime) = 0;
	virtual void render() = 0;
	virtual void onLostDevice() = 0;
	virtual void onResetDevice() = 0;

	// render() は、
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
		void create(EffectEngine* engine, tr::Task* updateTask)
		{
			m_engine = engine;
			m_updateTask = updateTask;
		}
		void execute()
		{
			m_updateTask->wait();
			m_engine->render();
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
//	void initialize(EffectManager* manager);
//	void SetWorldMatrix(const Matrix& matrix) { m_worldMatrix = matrix; }
//	//void advanceTime(float deltaTime);
//
//	//virtual void play(bool overlap) = 0;
//	virtual void stop() = 0;
//	virtual bool IsPlaying() = 0;
//
//	virtual void updateFrame() = 0;	// 更新スレッド
//	virtual void Draw() = 0;	// 描画スレッド
//
//protected:
//	EffectManager*	m_manager = nullptr;
//	Matrix			m_worldMatrix;
//	//float			m_deltaTime = 0;
//};


} // namespace detail
LN_NAMESPACE_END
