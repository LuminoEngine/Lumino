/*
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
#include "Effekseer\EffekseerDriver.h"
#include "EffectManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// Internal
//=============================================================================
static EffectManager* g_managerInstance = nullptr;

EffectManager* GetEffectManager(EffectManager* priority)
{
	if (priority != NULL) return priority;
	return g_managerInstance;
}

//=============================================================================
// EffectManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectManager::EffectManager()
	: m_fileManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_audioManager(nullptr)
	, m_engine(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectManager::~EffectManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Initialize(const Settings& settings)
{
	m_fileManager = settings.fileManager;
	m_graphicsManager = settings.graphicsManager;
	m_audioManager = settings.audioManager;

	auto engine = RefPtr<EffekseerEffectEngine>::Create();
	engine->Initialize(this, 32, 0, 2000);
	m_engine = engine.DetachAddRef();

	if (g_managerInstance == nullptr) {
		g_managerInstance = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Finalize()
{
	if (m_engine != nullptr) {
		m_engine->Finalize();
		LN_SAFE_RELEASE(m_engine);
	}

	if (g_managerInstance == this) {
		g_managerInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::PreRender()
{
	m_engine->UpdateFrame(0.016);	// TODO: time
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Render()
{
	m_engine->Render();
}

} // namespace detail
LN_NAMESPACE_END
