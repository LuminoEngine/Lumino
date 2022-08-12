
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Asset/AssetModel.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoGraphics/Rendering/Common.hpp>

template <class T>
void staticFactory2();

namespace ln {
class Texture;
class World;
class WorldObject;
class RenderingContext;
class Level;
namespace detail {
	class SceneManager; class SceneConductor;
}

class LevelRenderParameters
	: public Object
{
public:
	void serialize_deprecated(Serializer2_deprecated& ar) override;
	void mergeToRenderParams(detail::SceneGlobalRenderParams* params) const;

	Optional_deprecated<float> m_fogStartDistance;
	Optional_deprecated<Color> m_fogColor;
	Optional_deprecated<float> m_fogDensity;
	Optional_deprecated<float> m_fogHeightDensity;
	Optional_deprecated<float> m_fogLowerHeight;
	Optional_deprecated<float> m_fogUpperHeight;

	Optional_deprecated<Color> m_skydomeSkyColor;
	Optional_deprecated<Color> m_skydomeHorizonColor;
	Optional_deprecated<Color> m_skydomeCloudColor;
	Optional_deprecated<Color> m_skydomeOverlayColor;
};

enum class LevelUpdateMode
{
	PauseWhenInactive,
	Always,
};

class System
	: public Object
{
public:

protected:
	virtual void onUpdate();
	
LN_CONSTRUCT_ACCESS:
	System();
	virtual ~System();
	Result init();

	friend class Level;
};

/**
 * シーンのベースクラスです。
 */
// Note: Level はひとつの World に属し、World からの通知により onUpdate() や m_rootWorldObjectList の Object へさらに通知したりする。
LN_CLASS()
class Level
	: public AssetObject
{
	LN_OBJECT;
public:
	World* world() const { return m_ownerWorld; }

	/** 指定した WorldObject を、この Level 子オブジェクトとして追加します。 */
	LN_METHOD()
	void addObject(WorldObject* obj);

	/** 指定した WorldObject を、この Level 子オブジェクトか除外します。 */
	LN_METHOD()
	void removeObject(WorldObject* obj);

	/** 指定した Level を、この Level の Sub-Level として追加します。 */
	LN_METHOD()
	void addSubLevel(Level* sublevel);

	/** 指定した Level を、この Level の Sub-Level から除外します。 */
	LN_METHOD()
	void removeSubLevel(Level* sublevel);

	/** すべての Sub-Level を除外します。 */
	LN_METHOD()
	void removeAllSubLevels();

	void setUpdateMode(LevelUpdateMode value) { m_updateMode = value; }
	LevelUpdateMode updateMode() const { return m_updateMode; }

	void addSystem(System* system);
	void removeSystem(System* system);

protected:
	/** 開始処理 */
	LN_METHOD()
	virtual void onStart();

	/** 終了処理 */
	LN_METHOD()
	virtual void onStop();

	/** Called when deactivated. */
	LN_METHOD()
	virtual void onPause();

	/** Called when activated. */
	LN_METHOD()
	virtual void onResume();

	/** フレーム更新 */
	LN_METHOD()
	virtual void onUpdate();
    virtual void onPostUpdate(float elapsedSeconds);


public:	// TODO: Editor integration
	void setup(const ln::Path& filePath);
	void clear();
	void save();
	void load();
    WorldObject* findObjectByComponentType(const TypeInfo* type) const;
	void updateObjectsWorldMatrix() const;
    virtual void onPreUpdate(float elapsedSeconds);
	void collectActiveObjects(World* world);
    void collectRenderObjects(World* world, RenderingContext* context);
    void renderGizmos(RenderingContext* context);
	bool traverse(detail::IWorldObjectVisitor* visitor) const;
	//void updateObjectsWorldMatrix() const;

protected:
	void serialize_deprecated(Serializer2_deprecated& ar) override;


LN_CONSTRUCT_ACCESS:
	Level();
	virtual ~Level();

	/** Initialize */
	LN_METHOD()
	Result init();

public: // TODO: internal
	class SubLevelManager
	{
	public:
		void addSubLevel(Level* level);
		void removeSubLevel(Level* level);
		void removeAllSubLevels();
		void executeCommands();
		const List<Ref<Level>>& subLevels() const { return m_subLevels; }

	private:
		enum class CommandType : uint8_t
		{
			AddSubLevel,
			RemoveSubLevel,
			RemoveAllSubLevels,
		};

		struct Command
		{
			CommandType type;
			Ref<Level> level;
		};

		std::vector<Command> m_commands;
		List<Ref<Level>> m_subLevels;
	};

	void update(float elapsedSeconds);
    void removeRootObject(WorldObject* obj);
    void removeAllObjects();
	void mergeToRenderParams(detail::SceneGlobalRenderParams* params) const;
	LevelRenderParameters* acquireRenderParameters();
	SubLevelManager* acquireSubLevelManager();

	World* m_ownerWorld;
	Level* m_parentLevel = nullptr;
    Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
    //List<WorldObject*> m_destroyList;
	std::unique_ptr<SubLevelManager> m_subLevelManager;
	//List<Ref<Level>> m_subLevels;
	List<Ref<System>> m_systems;

	// TODO: Master-scene と Sub-scene の値を統合したうえで、SceneRenderer に流したい。
	//detail::SceneGlobalRenderParams m_sceneGlobalRenderParams;

	// TODO: Editor integration
	ln::Path m_filePath;

	LevelUpdateMode m_updateMode;

	bool m_initialUpdate;

	Ref<LevelRenderParameters> m_levelRenderParameters;

	friend class detail::SceneManager;
	friend class detail::SceneConductor;
    friend class World;
};

} // namespace ln
