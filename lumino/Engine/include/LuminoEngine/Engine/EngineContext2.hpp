#pragma once
#include "Common2.hpp"

namespace ln {

class EngineContext2
{
	/*
	 * [2021/9/10] なぜモジュールの細分化を行うのか？
	 * ----------
	 * - 移植やテストを段階的に進められるようにするため。
	 *   - 全部まとめて Web に移植！とかだとつらい。
	 *     動作確認大変な他、全部モジュールのビルドを通さないと動かすこともできない。
	 * - 依存関係の違反を検出しやすくするため。
	 *     - 一部だけモジュールを使いたいというときに対応できなくなる。
	 * - 実験的な機能を分離しやすくするため。
	 * 
	 * ユーザーのためというより、検証をやりやすくするための対策。
	 */
public:
	static EngineContext2* instance() { return s_instance.get(); };

	/** Initialize context. */
	static bool initialize(EngineContext2* sharedContext = nullptr);

	/** Terminate context. */
	static void terminate();

	/** Register module to this context. */
	void registerModule(Module* mod);

	/** Unregister module from this context. */
	void unregisterModule(Module* mod);

	// TODO:
	RefObject* platformManager = nullptr;

private:
	EngineContext2();
	bool init();
	void dispose();

	static std::unique_ptr<EngineContext2> s_instance;
	
	List<Ref<Module>> m_modules;
};

} // namespace ln
