
#pragma once
#include "Common.hpp"
#include "../Rendering/Common.hpp"
#include "../Asset/AssetModel.hpp"
#include "../Asset/AssetObject.hpp"

//template <class T>
//void staticFactory2();

namespace ln {
class Texture;
class World;
class WorldObject;
class RenderingContext;
namespace detail {
	class SceneManager; class SceneConductor;
}

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
