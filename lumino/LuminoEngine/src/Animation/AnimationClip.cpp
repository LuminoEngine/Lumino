
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Animation/AnimationClip.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include "AnimationManager.hpp"
#include "VmdLoader.hpp"
#include "BvhImporter.hpp"

namespace ln {

//==============================================================================
// AnimationClip

Ref<AnimationClip> AnimationClip::create(/*const StringView& name, */const StringView& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes)
{
	return makeObject<AnimationClip>(/*name, */targetPath, keyframes);
}

Ref<AnimationClip> AnimationClip::load(const StringView& filePath)
{
	//return makeObject<AnimationClip>(filePath);

	return detail::EngineDomain::animationManager()->loadAnimationClip(filePath)->result();

	//const Char* candidateExts[] = { _TT(".vmd") };
	//if (const auto assetPath = detail::AssetPath::resolveAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts)))
	//	return detail::EngineDomain::animationManager()->acquireAnimationClip(assetPath);
	//else
	//	return nullptr;
}

Ref<AnimationClipPromise> AnimationClip::loadAsync(const StringView& filePath)
{
	auto task = detail::EngineDomain::animationManager()->loadAnimationClip(filePath);
	return AnimationClipPromise::continueWith(task);
}

AnimationClip::AnimationClip()
    : m_lastFrameTime(0)
	, m_wrapMode(AnimationWrapMode::Loop)
	, m_hierarchicalAnimationMode(HierarchicalAnimationMode::AllowTranslationOnlyRoot)
{
}

AnimationClip::~AnimationClip()
{
}

bool AnimationClip::init()
{
	return AssetObject::init();
}

//bool AnimationClip::init(const Path& assetPath)
//{
//	// TODO: 2回initやめたほうがいい
//	if (!init()) return false;
//	if (!initLoad(assetPath)) return false;
//	return true;
//}

//bool AnimationClip::init(const detail::AssetPath& assetSourcePath)
//{
//	if (!AssetObject::init()) return false;
//	m_assetSourcePath = assetSourcePath;
//	reload();
//	return true;
//}

void AnimationClip::init(/*const StringView& name, */const StringView& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes)
{
	//if (LN_REQUIRE(!name.isEmpty())) return;

	init();

	//m_name = name;

	auto curve = makeObject<KeyFrameAnimationCurve>();
	for (auto& key : keyframes) {
		curve->addKeyFrame(key);
	}

	auto track = ScalarAnimationTrack::create();
	track->setTargetName(targetPath);
	track->setCurve(curve);
	addTrack(track);
}

void AnimationClip::addTrack(AnimationTrack* track)
{
	if (LN_REQUIRE(!track->m_owner)) return;

    m_tracks.add(track);
	track->m_owner = this;

    // TODO: とりあえず
    m_lastFrameTime = track->lastFrameTime();
}

const std::vector<const Char*>& AnimationClip::resourceExtensions() const
{
	static const std::vector<const Char*> exts = { _TT(".bvh"), _TT(".vmd") };
	return exts;
}

void AnimationClip::onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath)
{
	// TODO: ちゃんと stream から読みたい
	auto assetManager = detail::AssetManager::instance();
	auto diag = makeObject<DiagnosticsManager>();
	detail::BvhImporter importer(assetManager, diag);
	importer.import(this, assetPath, detail::EngineDomain::animationManager()->defaultAnimationClipImportSettings());
	diag->dumpToLog();

	setName(assetPath.path().fileNameWithoutExtension());
}

//void AnimationClip::onLoadResourceFile()
//{
//	if (LN_REQUIRE(!m_assetSourcePath.isNull())) return;
//
//	auto assetManager = detail::EngineDomain::assetManager();
//
//	if (m_assetSourcePath.path().hasExtension(_TT(".bvh"))) {
//		auto diag = makeObject<DiagnosticsManager>();
//		detail::BvhImporter importer(assetManager, diag);
//		importer.import(this, m_assetSourcePath);
//		diag->dumpToLog();
//	}
//	else if (m_assetSourcePath.path().hasExtension(_TT(".vmd"))) {
//		LN_NOTIMPLEMENTED();
//	}
//	else {
//		// TODO: 拡張子省略時の対策
//		LN_NOTIMPLEMENTED();
//	}
//}

////==============================================================================
//// VmdAnimationClip
//
//Ref<VmdAnimationClip> VmdAnimationClip::create(const Path& filePath)
//{
//	return makeObject<VmdAnimationClip>(filePath);
//}
//
//VmdAnimationClip::VmdAnimationClip()
//{
//}
//
//VmdAnimationClip::~VmdAnimationClip()
//{
//}
//
//void VmdAnimationClip::init(const Path& filePath)
//{
//	AnimationClip::init();
//
//	detail::VmdFile vmdFile;
//	auto vmdData = vmdFile.load(FileStream::create(filePath.c_str(), FileOpenMode::Read));
//	if (vmdData)
//	{
//		for (auto& track : vmdData->MotionData)
//		{
//			m_tracks.add(makeObject<VMDBezierTransformAnimationTrack>(track));
//		}
//
//		m_lastFrameTime = vmdData->lastFrameTime;
//
//		// VmdData が持っているトラックの情報を後で使いたいので、参照を持っておく
//		m_srcData = vmdData;
//	}
//}
//
} // namespace ln