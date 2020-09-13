
#pragma once
#include "../Base/Promise.hpp"
#include "../Asset/AssetObject.hpp"
#include "Common.hpp"

namespace ln {
struct AnimationKeyFrame;
class AnimationTrack;
class AnimationClip;

/**
 * AnimationClipPromise
 */
LN_PROMISE()
using AnimationClipPromise = Promise<Ref<AnimationClip>>;

/**
 * オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
 *
 * スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
 */
class AnimationClip
	: public AssetObject
{
public:
    /** ひとつの AnimationTrack を持つ AnimationClip を作成します。 */
	static Ref<AnimationClip> create(/*const StringRef& name, */const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);

	static Ref<AnimationClip> load(const StringRef& filePath);

	/** アニメーションの繰り返しの動作を取得します。(default: Loop) */
	void setWrapMode(AnimationWrapMode value) { m_wrapMode = value; }

	/** アニメーションの繰り返しの動作を取得します。 */
	AnimationWrapMode wrapMode() const { return m_wrapMode; }

	/** アニメーションの繰り返しの動作を取得します。(default: AllowTranslationOnlyRoot) */
	void setHierarchicalAnimationMode(HierarchicalAnimationMode value) { m_hierarchicalAnimationMode = value; }

	/** アニメーションの繰り返しの動作を取得します。 */
	HierarchicalAnimationMode hierarchicalAnimationMode() const { return m_hierarchicalAnimationMode; }

    void addTrack(AnimationTrack* track);
	const List<Ref<AnimationTrack>>& tracks() const { return m_tracks; }

	float lastFrameTime() const { return m_lastFrameTime; }

	void setName(const String& value) { m_name = value; }
	const String& name() const { return m_name; }

protected:
	void onLoadSourceFile() override;

LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();
	void init();
	bool init(const detail::AssetPath& assetSourcePath);
	void init(/*const StringRef& name, */const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);

protected:
	String m_name;
	List<Ref<AnimationTrack>> m_tracks;
	Ref<RefObject> m_srcData;
	float m_lastFrameTime;
	AnimationWrapMode m_wrapMode;
	HierarchicalAnimationMode m_hierarchicalAnimationMode;
	detail::AssetPath m_assetSourcePath;

	friend class detail::AnimationManager;
};

///**
//	@brief		Vocaloid Motion Data (.vmd) ファイル
//*/
//// TODO: 継承ではなく、Importer として、AnimationClip を構築
//class VmdAnimationClip
//	: public AnimationClip
//{
//public:
//	static Ref<VmdAnimationClip> create(const Path& filePath);
//
//LN_CONSTRUCT_ACCESS:
//	VmdAnimationClip();
//	virtual ~VmdAnimationClip();
//	void init(const Path& filePath);
//};

} // namespace ln
