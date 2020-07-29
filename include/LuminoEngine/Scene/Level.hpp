
#pragma once
#include "Common.hpp"
#include "../Rendering/Common.hpp"
#include "../Asset/AssetModel.hpp"
#include "../Asset/AssetObject.hpp"

template <class T>
void staticFactory2();

namespace ln {
class Texture;
class World;
class WorldObject;
class RenderingContext;
namespace detail {
	class SceneManager; class SceneConductor;
}

class LevelRenderParameters
	: public Object
{
public:
	void serialize2(Serializer2& ar) override;
	void mergeToRenderParams(detail::SceneGlobalRenderParams* params) const;

	Optional<float> m_fogStartDistance;
	Optional<Color> m_fogColor;
	Optional<float> m_fogDensity;
	Optional<float> m_fogHeightDensity;
	Optional<float> m_fogLowerHeight;
	Optional<float> m_fogUpperHeight;

	Optional<Color> m_skydomeSkyColor;
	Optional<Color> m_skydomeHorizonColor;
	Optional<Color> m_skydomeCloudColor;
	Optional<Color> m_skydomeOverlayColor;
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

	void addSubLevel(Level* level);
	void removeSubLevel(Level* level);
	void removeAllSubLevels();

protected:
	/** 開始処理 */
	LN_METHOD()
	virtual void onStart();

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
	void updateObjectsWorldMatrix() const;
    virtual void onPreUpdate(float elapsedSeconds);
    void collectRenderObjects(World* world, RenderingContext* context);
    void renderGizmos(RenderingContext* context);
	bool traverse(detail::IWorldObjectVisitor* visitor) const;
	//void updateObjectsWorldMatrix() const;

protected:
	void serialize2(Serializer2& ar) override;


LN_CONSTRUCT_ACCESS:
	Level();
	virtual ~Level();

	/** Initialize */
	LN_METHOD()
	bool init();

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
    List<WorldObject*> m_destroyList;
	std::unique_ptr<SubLevelManager> m_subLevelManager;
	//List<Ref<Level>> m_subLevels;

	// TODO: Master-scene と Sub-scene の値を統合したうえで、SceneRenderer に流したい。
	//detail::SceneGlobalRenderParams m_sceneGlobalRenderParams;

	// TODO: Editor integration
	ln::Path m_filePath;

	bool m_initialUpdate;

	Ref<LevelRenderParameters> m_levelRenderParameters;

	friend class detail::SceneManager;
	friend class detail::SceneConductor;
    friend class World;
};

} // namespace ln
