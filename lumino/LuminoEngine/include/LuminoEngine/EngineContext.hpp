#pragma once
//#include <LuminoEngine/Reflection/Property.hpp>
#include "../../../Engine/include/LuminoEngine/Reflection/Property.hpp"

namespace ln {
namespace detail {
class RuntimeManager;
class EngineManager;
}

class EngineContext : public RefObject
{
public:
	static EngineContext* current();

	// EngineManager の初期化が行われる直前に呼び出される。
	// Binding で、ラインタイム情報をもとに EngineSetting を最終調整するために使用する。
	std::function<void()> engineManagerPreInit;

	// EngineManager の初期化直後に呼び出される。
	std::function<void()> engineManagerPostInit;

    EngineContext();
	virtual ~EngineContext();
	void internalInit();

   

	const Ref<detail::RuntimeManager>& runtimeManager() const { return m_runtimeManager; }
	const Ref<detail::EngineManager>& engineManager() const { return m_engineManager; }

// TODO: internal
	void initializeRuntimeManager();
	void initializeEngineManager();
	void disposeRuntimeManager();
	void disposeEngineManager();

private:
	Ref<detail::RuntimeManager> m_runtimeManager;
	Ref<detail::EngineManager> m_engineManager;

	bool m_init = false;
};

} // namespace ln

