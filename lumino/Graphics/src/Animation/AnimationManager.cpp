
#include "Internal.hpp"
//#include <LuminoEngine/Scene/World.hpp>
#include <LuminoGraphics/Animation/AnimationTrack.hpp>
#include <LuminoGraphics/Animation/AnimationClip.hpp>
#include <LuminoGraphics/Animation/AnimationContext.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
//#include "../Scene/SceneManager.hpp"
#include "VmdLoader.hpp"
#include "AnimationManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VMDBezierTransformAnimationTrack

// ボーン単位
class VMDBezierTransformAnimationTrack
    : public AnimationTrack {
public:
    VMDBezierTransformAnimationTrack();
    virtual ~VMDBezierTransformAnimationTrack();
    void init(const detail::MotionFrameTrack& track);

    virtual void evaluate(float time, AnimationValue* outResult) override;

    static HumanoidBones mapHumanoidBone(const String& boneName);

private:
    const detail::MotionFrameTrack* m_vmdTrack;
};

VMDBezierTransformAnimationTrack::VMDBezierTransformAnimationTrack()
    : AnimationTrack(AnimationValueType::Transform) {
}

VMDBezierTransformAnimationTrack::~VMDBezierTransformAnimationTrack() {
}

void VMDBezierTransformAnimationTrack::init(const detail::MotionFrameTrack& track) {
    AnimationTrack::init();
    m_vmdTrack = &track;
    setTargetName(m_vmdTrack->boneName);
    setTargetHumanoidBone(mapHumanoidBone(m_vmdTrack->boneName));
    // std::cout << m_vmdTrack->boneName << std::endl;
}

void VMDBezierTransformAnimationTrack::evaluate(float time, AnimationValue* outResult) {
    AttitudeTransform result;

    if (!m_vmdTrack->frames.isEmpty()) {
        int keyIndex = 0;
        for (; keyIndex < m_vmdTrack->frames.size() && m_vmdTrack->frames[keyIndex].frameTime < time; keyIndex++)
            ;
        keyIndex = (keyIndex > 0) ? keyIndex - 1 : keyIndex;

        if (keyIndex + 1 < m_vmdTrack->frames.size()) {
            auto& lk = m_vmdTrack->frames[keyIndex];
            auto& rk = m_vmdTrack->frames[keyIndex + 1];

            float diffTime = rk.frameTime - lk.frameTime;
            float t = (time - lk.frameTime) / (float)diffTime;

            Quaternion rot;
            Vector3 pos;
            const Vector4 motionPositions = Vector4::Zero;
            float dummy, gt;

            rot = detail::VmdData::interpolateQuaternionStrict(lk, rk, t, motionPositions.w, diffTime, &dummy);

            gt = detail::VmdData::interpolateStrict(rk.interpolXA, rk.interpolXB, t, motionPositions.x, diffTime, &dummy);
            pos.x = lk.position.x + (rk.position.x - lk.position.x) * gt;
            gt = detail::VmdData::interpolateStrict(rk.interpolYA, rk.interpolYB, t, motionPositions.y, diffTime, &dummy);
            pos.y = lk.position.y + (rk.position.y - lk.position.y) * gt;
            gt = detail::VmdData::interpolateStrict(rk.interpolZA, rk.interpolZB, t, motionPositions.z, diffTime, &dummy);
            pos.z = lk.position.z + (rk.position.z - lk.position.z) * gt;

            result.rotation = rot;
            result.translation = pos;
        }
        else {
            result.rotation = m_vmdTrack->frames.back().quaternion;
            result.translation = m_vmdTrack->frames.back().position;
        }
    }
    else {
        // no key frame
        result.rotation = Quaternion::Identity;
        result.translation = Vector3::Zero;
    }

    // if (targetName() == _TT("左腕") {
    //	Matrix m = Matrix::makeRotationQuaternion(result.rotation);
    //	auto v = Vector3::transformCoord(Vector3::UnitZ, m);
    //	printf("%f\t%f\t%f\n", v.x, v.y, v.z);
    //	//result.rotation = Quaternion::makeFromEulerAngles(Vector3(0, 0, time));
    // }

    outResult->setTransform(result);
}

HumanoidBones VMDBezierTransformAnimationTrack::mapHumanoidBone(const String& boneName) {
    static const std::unordered_map<String, HumanoidBones> table = {
        { _TT("下半身"), HumanoidBones::Hips },
        { _TT("上半身"), HumanoidBones::Spine },
        { _TT("上半身2"), HumanoidBones::Chest },
        //{ _TT("", HumanoidBones::UpperChest },

        { _TT("左肩"), HumanoidBones::LeftShoulder },
        { _TT("左腕"), HumanoidBones::LeftUpperArm },
        { _TT("左ひじ"), HumanoidBones::LeftLowerArm },
        { _TT("左手首"), HumanoidBones::LeftHand },

        { _TT("右肩"), HumanoidBones::RightShoulder },
        { _TT("右腕"), HumanoidBones::RightUpperArm },
        { _TT("右ひじ"), HumanoidBones::RightLowerArm },
        { _TT("右手首"), HumanoidBones::RightHand },

        { _TT("左足"), HumanoidBones::LeftUpperLeg },
        { _TT("左ひざ"), HumanoidBones::LeftLowerLeg },
        { _TT("左足首"), HumanoidBones::LeftFoot },
        { _TT("左つま先ＩＫ"), HumanoidBones::LeftToes },

        { _TT("右足"), HumanoidBones::RightUpperLeg },
        { _TT("右ひざ"), HumanoidBones::RightLowerLeg },
        { _TT("右足首"), HumanoidBones::RightFoot }, // FIXME: "右足ＩＫ" にしたほうがいいかも。
        { _TT("右つま先ＩＫ"), HumanoidBones::RightToes },

        { _TT("首"), HumanoidBones::Neck },
        { _TT("頭"), HumanoidBones::Head },
        { _TT("左目"), HumanoidBones::LeftEye },
        { _TT("右目"), HumanoidBones::RightEye },
        { _TT("あご"), HumanoidBones::Jaw },

        { _TT("左親指０"), HumanoidBones::LeftThumbProximal },
        { _TT("左親指１"), HumanoidBones::LeftThumbIntermediate },
        { _TT("左親指２"), HumanoidBones::LeftThumbDistal },
        { _TT("左人指１"), HumanoidBones::LeftIndexProximal },
        { _TT("左人指２"), HumanoidBones::LeftIndexIntermediate },
        { _TT("左人指３"), HumanoidBones::LeftIndexDistal },
        { _TT("左中指１"), HumanoidBones::LeftMiddleProximal },
        { _TT("左中指２"), HumanoidBones::LeftMiddleIntermediate },
        { _TT("左中指３"), HumanoidBones::LeftMiddleDistal },
        { _TT("左薬指１"), HumanoidBones::LeftRingProximal },
        { _TT("左薬指２"), HumanoidBones::LeftRingIntermediate },
        { _TT("左薬指３"), HumanoidBones::LeftRingDistal },
        { _TT("左小指１"), HumanoidBones::LeftLittleProximal },
        { _TT("左小指２"), HumanoidBones::LeftLittleIntermediate },
        { _TT("左小指３"), HumanoidBones::LeftLittleDistal },

        { _TT("右親指０"), HumanoidBones::RightThumbProximal },
        { _TT("右親指１"), HumanoidBones::RightThumbIntermediate },
        { _TT("右親指２"), HumanoidBones::RightThumbDistal },
        { _TT("右人指１"), HumanoidBones::RightIndexProximal },
        { _TT("右人指２"), HumanoidBones::RightIndexIntermediate },
        { _TT("右人指３"), HumanoidBones::RightIndexDistal },
        { _TT("右中指１"), HumanoidBones::RightMiddleProximal },
        { _TT("右中指２"), HumanoidBones::RightMiddleIntermediate },
        { _TT("右中指３"), HumanoidBones::RightMiddleDistal },
        { _TT("右薬指１"), HumanoidBones::RightRingProximal },
        { _TT("右薬指２"), HumanoidBones::RightRingIntermediate },
        { _TT("右薬指３"), HumanoidBones::RightRingDistal },
        { _TT("右小指１"), HumanoidBones::RightLittleProximal },
        { _TT("右小指２"), HumanoidBones::RightLittleIntermediate },
        { _TT("右小指３"), HumanoidBones::RightLittleDistal },
    };

    const auto itr = table.find(boneName);
    if (itr != table.end())
        return itr->second;
    else
        return HumanoidBones::None;
}

//==============================================================================
// AnimationManager

Ref<AnimationManager> AnimationManager::s_instance = nullptr;

AnimationManager* AnimationManager::initialize(const Settings& settings) {
    if (s_instance) return s_instance;

    auto m = Ref<AnimationManager>(LN_NEW detail::AnimationManager(), false);
    s_instance = m;
    if (!m->init(settings)) return nullptr;

    // FIXME: Animation を Module にするなら
    //EngineContext2::instance()->registerModule(m);
    return m;
}

void AnimationManager::terminate() {
    if (s_instance) {
        s_instance->dispose();
        // FIXME: Animation を Module にするなら
        //EngineContext2::instance()->unregisterModule(s_instance);
        s_instance = nullptr;
    }
}

AnimationManager::AnimationManager()
    : m_assetManager(nullptr)
//, m_sceneManager(nullptr)
{
}

AnimationManager::~AnimationManager() {
}

Result<> AnimationManager::init(const Settings& settings) {
    LN_LOG_DEBUG("AnimationManager Initialization started.");

    m_assetManager = settings.assetManager;
    m_animationClipCache.init(64, 0);

    m_defaultAnimationClipImportSettings = makeObject_deprecated<AnimationClipImportSettings>();

    LN_LOG_DEBUG("AnimationManager Initialization ended.");
    return ok();
}

void AnimationManager::dispose() {
}

void AnimationManager::addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation) {
    if (LN_REQUIRE(clock)) return;

    switch (affiliation) {
        case AnimationClockAffiliation::Standalone:
            break;
        case AnimationClockAffiliation::ActiveWorld:
            LN_NOTIMPLEMENTED();
            //m_sceneManager->activeWorld()->animationContext()->addAnimationClock(clock);
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

Ref<GenericTask<Ref<AnimationClip>>> AnimationManager::loadAnimationClip(const StringView& filePath) {
    static const std::vector<const Char*> exts = { _TT(".bvh"), _TT(".vmd") };
    return AssetManager::loadObjectWithCacheHelperAsync<AnimationClip>(&m_animationClipCache, nullptr, exts, filePath);
    // return task->result();

    // return AssetManager::loadObjectWithCacheHelper<AnimationClip>(&m_animationClipCache, nullptr, exts, filePath, nullptr);

    //// TODO: find cache

    //// TODO: やっぱり拡張子は本当のリロード時に解決したい。
    //// なので、AssetPath 自体の仕様として、拡張子無し（未解決）を許可するようにしたい。
    //// → でもそれなら Path で持っておけばいいだけか。ちゃんとドキュメントに書いておこう
    // const Char* exts[] = { _TT(".bvh", _TT(".vmd" };
    // auto assetPath = detail::AssetPath::resolveAssetPath(filePath, exts);

    // auto obj = makeObject_deprecated<AnimationClip>(assetPath);

    // detail::AssetObjectInternal::setAssetPath(obj, filePath);

    // return obj;
}

// Ref<AnimationClipPromise> AnimationManager::loadAnimationClipAsync(const StringView& filePath)
//{
//	LN_NOTIMPLEMENTED();
//	return nullptr;
// }

// Ref<AnimationClip> AnimationManager::acquireAnimationClip(const AssetPath& assetPath)
//{
//     uint64_t key = assetPath.calculateHash();
//     auto obj = m_animationClipCache.findObject(key);
//     if (obj) {
//         return obj;
//     }
//     else {
//         obj = makeObject_deprecated<AnimationClip>();
//         loadAnimationClip(obj, assetPath);
//         m_animationClipCache.registerObject(key, obj, 0);
//         return obj;
//     }
// }
//
// void AnimationManager::loadAnimationClip(AnimationClip* clip, const AssetPath& assetPath)
//{
//     auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
//
//     detail::VmdFile vmdFile;
//     auto vmdData = vmdFile.load(stream);
//     if (vmdData)
//     {
//         for (auto& track : vmdData->MotionData)
//         {
//             clip->m_tracks.add(makeObject_deprecated<VMDBezierTransformAnimationTrack>(track));
//         }
//
//		clip->m_lastFrameTime = vmdData->lastFrameTime;
//
//         // VmdData が持っているトラックの情報を後で使いたいので、参照を持っておく
//		clip->m_srcData = vmdData;
//     }
// }

void AnimationManager::updateFrame(float elapsedSeconds) {
}

} // namespace detail
} // namespace ln
