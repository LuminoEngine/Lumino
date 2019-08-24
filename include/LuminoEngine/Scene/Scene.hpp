
#pragma once
#include "Common.hpp"
#include "../Asset/AssetObject.hpp"

template <class T>
void staticFactory2();

namespace ln {
class World;
class WorldObject;
class RenderingContext;
namespace ed { class SceneAsset; }
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
	virtual void onCreated(); // TODO: onStart かな。

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
    virtual void onPostUpdate(float elapsedSeconds);

public:	// TODO: Editor integration
	void setup(const ln::Path& filePath);
	void clear();
	void save();
	void load();
	void addObject(WorldObject* obj);
	void removeObject(WorldObject* obj);
    WorldObject* findObjectByComponentType(const TypeInfo* type) const;
    void updateObjectsWorldMatrix();
    virtual void onPreUpdate(float elapsedSeconds);
    void renderObjects(RenderingContext* context);

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(Archive& ar);


LN_CONSTRUCT_ACCESS:
	Scene();
	virtual ~Scene();

	/** Initialize */
	LN_METHOD()
	void init();

public: // TODO: internal
	void update(float elapsedSeconds);
    void removeRootObject(WorldObject* obj);
    void removeAllObjects();

    Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
    List<WorldObject*> m_destroyList;

	// TODO: Editor integration
	ln::Path m_filePath;

    friend class ed::SceneAsset;
	friend class detail::SceneManager;
    friend class World;
};

//namespace ed {
//class WorldObjectAsset;

//class SceneAsset
//    : public AssetModel
//{
//	LN_OBJECT;
//public:
//	// Lifecycle management
//	void setup(const ln::Path& filePath);
//	void clear();
//	void save();
//	void load();
//
//	// Edit operations (undo, redo)
//	void addNewWorldObject();
//
//protected:
//	LN_SERIALIZE_CLASS_VERSION(1);
//	void serialize(Archive& ar);
//
//LN_CONSTRUCT_ACCESS:
//    SceneAsset();
//    void init();
//
//private:
//	ln::Path m_filePath;
//	List<Ref<WorldObjectAsset>> m_rootWorldObjects;
//
//	template <class T>
//	friend
//	void staticFactory2();
//
//};

//} // namespace ed
} // namespace ln
