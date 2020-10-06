
#pragma once
#include "Common.hpp"
#include "../Rendering/Common.hpp"
#include "../Asset/AssetModel.hpp"
#include "../Asset/AssetObject.hpp"

namespace ln {
class Texture;
class World;
class WorldObject;

/** ワールド 及び レベル 管理のユーティリティです。 */
LN_CLASS(Static)
class Scene
{
public:

	//----------------------------------------------------------------------------
	/** @name Background and Sky */
	/** @{ */

	/** シーン背景のクリア方法を設定します。 */
	LN_METHOD()
	static void setClearMode(SceneClearMode value);

	/** ClearMode が SkyDome であるときに使用する、空の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	LN_METHOD()
	static void setSkyColor(const Color& value);

	/** ClearMode が SkyDome であるときに使用する、地平の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	LN_METHOD()
	static void setSkyHorizonColor(const Color& value);

	/** ClearMode が SkyDome であるときに使用する、雲の基本色を設定します。アルファ値は、設定した色の適用率です。 */
	LN_METHOD()
	static void setSkyCloudColor(const Color& value);

	/** ClearMode が SkyDome であるときに使用する、空全体に影響する色を設定します。アルファ値は、設定した色の適用率です。 */
	LN_METHOD()
	static void setSkyOverlayColor(const Color& value);

	/** @} */

	//----------------------------------------------------------------------------
	/** @name Level Transition */
	/** @{ */

	/** 指定したレベルへ遷移します。既存の全てのレベルは非アクティブ化または削除されます。 */
	LN_METHOD()
	static void gotoLevel(Level* level, bool withEffect = true);

	/** 現在のレベルを非アクティブ化し、指定したレベルへ遷移します。 */
	LN_METHOD()
	static void callLevel(Level* level, bool withEffect = true);

	/** 現在のレベルを終了し、ひとつ前のレベルへ遷移します。 */
	LN_METHOD()
	static void returnLevel(bool withEffect = true);

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

	///** クロスフェードエフェクトを開始します。 */
	//LN_METHOD()
	//static void startCrossFade();

	/** @} */

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
	/** @name PostEffect */
	/** @{ */

	static void setScreenBlendColor(const Color& value);

	static const Color& screenBlendColor();

	// ※ RGB は「加算」される
	static void setScreenColorTone(const ColorTone& value);

	static const ColorTone& screenColorTone();

	/** @} */
};

} // namespace ln
