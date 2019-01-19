
#pragma once
#include "Common.hpp"

namespace ln {
namespace detail { class SceneManager; }

/**
 * シーンのベースクラスです。シーンはタイトル画面などの画面遷移の単位です。
 */
LN_CLASS()
class Scene
	: public Object
{
	LN_OBJECT;
protected:
	/** 開始処理 */
	LN_METHOD()
	virtual void onCreated();

	/** 終了処理 */
	LN_METHOD()
	virtual void onClosed();

	/** Called when activated. */
	LN_METHOD()
	virtual void onActivated();

	/** Called when deactivated. */
	LN_METHOD()
	virtual void onDeactivated();

	/** フレーム更新 */
	LN_METHOD()
	virtual void onUpdate();

LN_CONSTRUCT_ACCESS:
	Scene();
	virtual ~Scene();

	/** Initialize */
	LN_METHOD()
	void init();

private:
	void update();

	friend class detail::SceneManager;
};

} // namespace ln
