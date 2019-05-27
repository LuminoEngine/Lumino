
#pragma once
#include "Common.hpp"

namespace ln {
class WorldObject;
namespace detail { class SceneManager; }

/**
 * シーンのベースクラスです。
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

public:	// TODO: Editor integration
	void setup(const ln::Path& filePath);
	void clear();
	void save();
	void load();
	void addObject(WorldObject* obj);
	void removeObject(WorldObject* obj);
protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(Archive& ar);


LN_CONSTRUCT_ACCESS:
	Scene();
	virtual ~Scene();

	/** Initialize */
	LN_METHOD()
	void init();

private:
	void update();

	List<Ref<WorldObject>> m_rootWorldObjects;

	// TODO: Editor integration
	ln::Path m_filePath;

	friend class detail::SceneManager;
};

} // namespace ln
