
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
namespace ed { class SceneAsset; }
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
	void updateObjectsWorldMatrix();
    virtual void onPreUpdate(float elapsedSeconds);
    void collectRenderObjects(World* world, RenderingContext* context);
    void renderGizmos(RenderingContext* context);

protected:
	void serialize2(Serializer2& ar) override;


LN_CONSTRUCT_ACCESS:
	Level();
	virtual ~Level();

	/** Initialize */
	LN_METHOD()
	bool init();

public: // TODO: internal
	void update(float elapsedSeconds);
    void removeRootObject(WorldObject* obj);
    void removeAllObjects();
	void mergeToRenderParams(detail::SceneGlobalRenderParams* params) const;
	LevelRenderParameters* acquireRenderParameters();

	World* m_ownerWorld;
    Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
    List<WorldObject*> m_destroyList;

	// TODO: Master-scene と Sub-scene の値を統合したうえで、SceneRenderer に流したい。
	//detail::SceneGlobalRenderParams m_sceneGlobalRenderParams;

	// TODO: Editor integration
	ln::Path m_filePath;

	bool m_initialUpdate;

	Ref<LevelRenderParameters> m_levelRenderParameters;

    friend class ed::SceneAsset;
	friend class detail::SceneManager;
	friend class detail::SceneConductor;
    friend class World;
};


/** ワールド 及び レベル 管理のユーティリティです。 */
LN_CLASS(Static)
class Scene
{
public:
	/** 指定したレベルへ遷移します。既存の全てのレベルは非アクティブ化または削除されます。 */
	LN_METHOD()
	static void gotoLevel(Level* level);

	static void callLevel(Level* level);

	static void returnLevel();

	/** 現在のアクティブなレベルを取得します。 */
	LN_METHOD()
	static Level* activeLevel();

	/** レベルの遷移エフェクトを実行中であるかを確認します。 */
	LN_METHOD()
	static bool isTransitionEffectRunning();

	/** レベル遷移時のエフェクトの種類を設定します。 */
	LN_METHOD()
	static void setTransitionEffectMode(LevelTransitionEffectMode value);

	/** レベル遷移時のエフェクトの種類を取得します。 */
	LN_METHOD()
	static LevelTransitionEffectMode transitionEffectMode();

	/** レベルの遷移にかける時間を設定します。(Unit: 秒) */
	LN_METHOD()
	static void setTransitionDuration(float value);

	/** レベルの遷移にかける時間を取得します。(Unit: 秒) */
	LN_METHOD()
	static float transitionDuration();

	/** レベルの遷移モードが FadeInOut である場合に使用する色を設定します。 */
	LN_METHOD()
	static void setTransitionEffectColor(const Color& value);

	/** レベルの遷移モードが FadeInOut である場合に使用する色を取得します。 */
	LN_METHOD()
	static const Color& transitionEffectColor();

	/** レベルの遷移エフェクトで使用するマスクテクスチャを設定します。 */
	LN_METHOD()
	static void setTransitionEffectMaskTexture(Texture* value);

	/** レベルの遷移エフェクトで使用するマスクテクスチャを取得します。 */
	LN_METHOD()
	static Texture* transitionEffectMaskTexture();

	/** レベルの遷移エフェクトの境界のあいまいさを設定します。 */
	LN_METHOD()
	static void setTransitionEffectVague(float value);

	/** レベルの遷移エフェクトの境界のあいまいさを取得します。 */
	LN_METHOD()
	static float transitionEffectVague();

	/** フェードアウトエフェクトを開始します。 */
	LN_METHOD()
	static void startFadeOut();

	/** フェードインエフェクトを開始します。 */
	LN_METHOD()
	static void startFadeIn();

	/** クロスフェードエフェクトを開始します。 */
	LN_METHOD()
	static void startCrossFade();

	//----------------------------------------------------------------------------
	/** @name Fog */
	/** @{ */

	// 指数関数的高さフォグ

	/** フォグを開始するカメラからの距離を設定します。 */
	static void setFogStartDistance(float value);

	/** フォグのメインカラーを設定します。 */
	static void setFogColor(const Color& value);

	/** フォグの濃さを設定します。 */
	static void setFogDensity(float value);

	/** 高さフォグの濃さを設定します。 */
	static void setFogHeightDensity(float value);

	/** フォグの高さの下限を設定します。 */
	static void setFogLowerHeight(float value);

	/** フォグの高さの上限を設定します。 */
	static void setFogUpperHeight(float value);

	/** @} */

	//----------------------------------------------------------------------------
	/** @name Skydome */
	/** @{ */

	/** Skydome の空の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	static void setSkydomeSkyColor(const Color& value);

	/** Skydome の地平の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	static void setSkydomeHorizonColor(const Color& value);

	/** Skydome の雲の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	static void setSkydomeCloudColor(const Color& value);

	/** Skydome 全体に影響する色を設定します。アルファ値は、設定した色の適用率です。 */
	static void setSkydomeOverlayColor(const Color& value);

	/** @} */
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
