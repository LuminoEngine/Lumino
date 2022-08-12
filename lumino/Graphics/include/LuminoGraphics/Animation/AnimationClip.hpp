
#pragma once
#include <LuminoEngine/Base/Promise.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>
#include "Common.hpp"

namespace ln {
struct AnimationKeyFrame;
class AnimationTrack;
class AnimationClip;

/**
 * AnimationClipPromise
 */
//LN_PROMISE()
using AnimationClipPromise = Promise<Ref<AnimationClip>>;


/**
 */
class AnimationClipImportSettings
	: public Object
{
public:
	bool requiredStandardCoordinateSystem = true;
};

/**
 * オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
 *
 * スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
 */
LN_CLASS()
class AnimationClip
	: public AssetObject
{
public:
    /** ひとつの AnimationTrack を持つ AnimationClip を作成します。 */
	static Ref<AnimationClip> create(/*const StringView& name, */const StringView& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);

	/** load */
	LN_METHOD()
	static Ref<AnimationClip> load(const StringView& filePath);

	static Ref<AnimationClipPromise> loadAsync(const StringView& filePath);


	/** アニメーションの繰り返しの動作を取得します。(default: Loop) */
	LN_METHOD(Property)
	void setWrapMode(AnimationWrapMode value) { m_wrapMode = value; }

	/** アニメーションの繰り返しの動作を取得します。 */
	LN_METHOD(Property)
	AnimationWrapMode wrapMode() const { return m_wrapMode; }

	/** 階層構造を持つアニメーションデータの動作モード。(default: AllowTranslationOnlyRoot) */
	LN_METHOD(Property)
	void setHierarchicalAnimationMode(HierarchicalAnimationMode value) { m_hierarchicalAnimationMode = value; }

	/** 階層構造を持つアニメーションデータの動作モード。 */
	LN_METHOD(Property)
	HierarchicalAnimationMode hierarchicalAnimationMode() const { return m_hierarchicalAnimationMode; }

    void addTrack(AnimationTrack* track);
	const List<Ref<AnimationTrack>>& tracks() const { return m_tracks; }

	float lastFrameTime() const { return m_lastFrameTime; }

	void setName(const String& value) { m_name = value; }
	const String& name() const { return m_name; }

protected:


LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();
	Result init();
	//bool init(const Path& assetPath);
	//bool init(const detail::AssetPath& assetSourcePath);
	void init(/*const StringView& name, */const StringView& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);

protected:	// TODO:
	String m_name;
	List<Ref<AnimationTrack>> m_tracks;
	Ref<RefObject> m_srcData;
	float m_lastFrameTime;
	AnimationWrapMode m_wrapMode;
	HierarchicalAnimationMode m_hierarchicalAnimationMode;
	//detail::AssetPath m_assetSourcePath;

private:
	const std::vector<const Char*>& resourceExtensions() const override;
	void onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath) override;

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
