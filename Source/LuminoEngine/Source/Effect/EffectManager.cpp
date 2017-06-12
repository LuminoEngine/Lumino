/*
	[2015/12/14] 公開クラスは PImpl? abstruct?
		EffectInstance を new するのは内部なので、今のところはどっちにしても abstruct になる。
		Effect モジュール的には現時点では拡張を考慮していないのでこれでもいい。
		（イベントコールバックが必要になったら考える必要がある）

エフェクトの構成について
	Core … 単なるデータクラス。

	Instance
		実際に更新・子生成等を行う。
		最終目的は描画のための情報の生成。(行列、色etc...)
		描画を行うのは EmitterCore。

	親子関係について
		update() で親から影響を受けるパラメータは行列のみ。(生成タイミングは別として、更新中の要素)
		これ以外のパラメータ(例えばRing の径とか)は、NodeInstance が EmitterCore に
		描画時に現在の時間を伝えて、そこで値を更新する。

	乱数シードについて
		NodeInstance ごとに固有の値を持たなければならない。
		本来であればエディタから設定できるようにするべきだが、
		とりあえず this を使っている。

*/
#include "Internal.h"
#include "../Graphics/GraphicsManager.h"
#include "Effekseer/EffekseerDriver.h"
#include "EffectManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// Internal
//==============================================================================
static EffectManager* g_managerInstance = nullptr;

EffectManager* GetEffectManager(EffectManager* priority)
{
	if (priority != NULL) return priority;
	return g_managerInstance;
}

//==============================================================================
// EffectManager
//==============================================================================

//------------------------------------------------------------------------------
EffectManager::EffectManager()
	: m_fileManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_audioManager(nullptr)
{
}

//------------------------------------------------------------------------------
EffectManager::~EffectManager()
{
}

//------------------------------------------------------------------------------
void EffectManager::initialize(const Settings& settings)
{
	m_fileManager = settings.fileManager;
	m_graphicsManager = settings.graphicsManager;
	m_audioManager = settings.audioManager;


	//m_threadUpdateFrame.Start(CreateDelegate(this, &EffectManager::Thread_UpdateFrame));
	//m_taskUpdateFrame = tr::Task::Create(CreateDelegate(this, &EffectManager::Thread_UpdateFrame));

	if (g_managerInstance == nullptr) {
		g_managerInstance = this;
	}
}

//------------------------------------------------------------------------------
void EffectManager::Finalize()
{
	if (g_managerInstance == this) {
		g_managerInstance = nullptr;
	}
}

//------------------------------------------------------------------------------
EffectEngine* EffectManager::CreateEffectWorld()
{
#ifdef LN_USE_EXTERNAL_Effekseer
	auto engine = RefPtr<EffekseerEffectEngine>::MakeRef();
	engine->initialize(this, 32, 0, 2000);	// TODO

	auto basePtr = RefPtr<EffectEngine>::StaticCast(engine);
	m_effectWorldList.Add(basePtr);
	return basePtr;
#else
	LN_NOTIMPLEMENTED();
	return nullptr;
#endif
}

//------------------------------------------------------------------------------
void EffectManager::ReleaseEffectWorld(EffectEngine* world)
{
	assert(world != nullptr);
	world->Finalize();
	m_effectWorldList.Remove(RefPtr<EffectEngine>(world));
}

////------------------------------------------------------------------------------
//void EffectManager::PreRender()
//{
//	m_taskUpdateFrame->Start();
//}
//
////------------------------------------------------------------------------------
//void EffectManager::Render()
//{
//	if (m_engine == nullptr) return;
//
//	if (m_graphicsManager->GetRenderingType() == GraphicsRenderingType::Threaded) {
//		m_graphicsManager->GetPrimaryRenderingCommandList()->AddCommand<EffectEngine::RenderCommand>(m_engine, m_taskUpdateFrame);
//	}
//	else {
//		m_taskUpdateFrame->Wait();
//		m_engine->Render();
//	}
//}

//------------------------------------------------------------------------------
void EffectManager::OnLostDevice()
{
	for (EffectEngine* e : m_effectWorldList)
	{
		e->OnLostDevice();
	}
}

//------------------------------------------------------------------------------
void EffectManager::OnResetDevice()
{
	for (EffectEngine* e : m_effectWorldList)
	{
		e->OnResetDevice();
	}
}

//------------------------------------------------------------------------------
//void EffectManager::Thread_UpdateFrame()
//{
//	if (m_engine == nullptr) return;
//	m_engine->UpdateFrame(0.016f);	// TODO: time
//}

} // namespace detail
LN_NAMESPACE_END
