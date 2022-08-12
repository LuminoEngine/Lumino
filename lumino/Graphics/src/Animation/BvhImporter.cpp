
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoGraphics/Animation/AnimationClip.hpp>
#include <LuminoGraphics/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include "BvhImporter.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AsciiLineReader

AsciiLineReader::AsciiLineReader()
	: m_stream(nullptr)
	, m_bufferPos(0)
	, m_bufferLength(0)
    , m_lineHead(nullptr)
    , m_lineLength(0)
{
    m_lineCache.resize(1024);
}

void AsciiLineReader::reset(Stream* stream)
{
    m_stream = stream;
    m_bufferPos = 0;
    m_bufferLength = 0;
    m_lineHead = nullptr;
    m_lineLength = 0;
}

bool AsciiLineReader::readLine()
{
    bool result = readLineRaw();
    if (result) {
        StringHelper::trim(m_lineCache.data(), m_lineCache.size(), &m_lineHead, &m_lineLength);
        splitLineTokens();
        m_lineTokenIndex = -1;
        return true;
    }
    else {
        return false;
    }
}

bool AsciiLineReader::readToken()
{
    m_lineTokenIndex++;
    return m_lineTokenIndex < m_lineTokens.size();
}

bool AsciiLineReader::readLineRaw()
{
    if (m_bufferPos == m_bufferLength) {
        if (readBuffer() == 0) {
            return false; // EOF
        }
    }

    m_lineCache.clear();

    do {
        int i = m_bufferPos;
        do {
            const char* buf = m_buffer.data();
            Char ch = m_buffer[i];
            if (ch == '\r' || ch == '\n') {
                std::copy(buf + m_bufferPos, buf + i, std::back_inserter(m_lineCache));
                m_bufferPos = i + 1;

                if (ch == '\r' && (m_bufferPos < m_bufferLength || readBuffer())) {
                    if (buf[m_bufferPos] == '\n') {
                        m_bufferPos++;    // skip LF
                    }
                }
                return true;
            }
            i++;
        } while (i < m_bufferLength);

        // ここに来るのは、charBuffer の現在位置 ～ 終端までに改行が無かったとき。
        // 現在の残りバッファを結合して、次のバッファを ReadBuffer() で読み出す。
        const char* buf = m_buffer.data();
        std::copy(buf + m_bufferPos, buf + m_bufferLength, std::back_inserter(m_lineCache));

    } while (readBuffer() > 0);

    return true;
}

bool AsciiLineReader::readBuffer()
{
    m_bufferPos = 0;
	m_bufferLength = m_stream->read(m_buffer.data(), m_buffer.size());
	if (m_bufferLength == 0) {
		return false;
	}
	else {

        return true;
	}
}

void AsciiLineReader::splitLineTokens()
{
    m_lineTokens.clear();
    const char* pos = lineHead();
    const char* end = lineEnd();
    const char* tokenBegin = nullptr;
    for (; pos < end; pos++) {
        if (!tokenBegin) {
            if (!StringHelper::isSpace(*pos)) {
                tokenBegin = pos;
            }
        }
        else {
            if (StringHelper::isSpace(*pos)) {
                m_lineTokens.push_back({ tokenBegin, static_cast<int>(pos - tokenBegin) });
                tokenBegin = nullptr;
            }
        }
    }

    if (tokenBegin) {
        m_lineTokens.push_back({ tokenBegin, static_cast<int>(pos - tokenBegin) });
    }
}

//==============================================================================
// BvhImporter
// http://www.dcs.shef.ac.uk/intranet/research/public/resmes/CS0111.pdf
// https://sites.google.com/a/cgspeed.com/cgspeed/motion-capture/cmu-bvh-conversion
// motion.hahasoha.netBVH
/*
    [2020/9/9] Blender メモ
    ----------
    Blender は Z が上、Y が前方だが、Blender からエクスポートした BVH はこのままの座標系で出てくる。
    オプションで変更したりできないので、Impoter 側でケアするしかない。

    [2020/9/9] Mixamo からダウンロードした FBX から、Blender を使って BVH をエクスポートしたとき、モーションが崩れる
    ----------
    Blender はボーンを、Head Tail の2点で表現し、さらに回転軸も個々に設定できる。
    これらの要素が、モーションをアタッチしたいモデルと異なっていると崩れてしまう。

    例えば、Bone の Tail を変えるだけで異なる回転角度がエクスポートされる。

    - Blender 上で Bone の Tailを変えても、BVH の HIERARCHY に変化は無し
    - MOTION の回転角度が変わる

    HC4-10で、右腕を頭上に向けるように曲げるとき、

    右腕のボーンを横方向にすると、
    Y: 90 (Blender座標系)

    右腕のボーンを上方向にすると、
    Z: 90 (Blender座標系)

    右腕のボーンを横方向にすると、rotOrg
            x	6.09999988e-05	float
            y	92.5882874	float
            z	-2.20000002e-05	float

    右腕のボーンを上方向にすると、rotOrg
            x	-99.5123901	float
            y	76.8954086	float
            z	91.7683716	float



*/

BvhImporter::BvhImporter(AssetManager* assetManager, DiagnosticsManager* diag)
	: m_assetManager(assetManager)
	, m_diag(diag)
    , m_flipZ(false)    // BVH の座標系は右手、Y up。それと、Z+ を正面とする。もし VRM モデルに適用したい場合は反転が必要
    , m_flipX(true)
    , m_minOffsetY(std::numeric_limits<float>::max())
    // https://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html
    // BVH階層に関する最後の注意点として、ワールド空間は、Y軸をワールドアップベクトルとする右手系の座標系として定義されます。
    // したがって、通常、BVHの骨格セグメントがY軸または負のY軸に沿って整列していることがわかります（キャラクターは多くの場合、キャラクターがまっすぐに立ち、腕を横にまっすぐに伸ばした状態でゼロポーズをとります）。
{
}

bool BvhImporter::import(AnimationClip* clip, const AssetPath& assetPath, const AnimationClipImportSettings* settings)
{
    if (settings->requiredStandardCoordinateSystem) {
        // BVH データは Y+Top, R-Hand であることを前提とし、
        // Z+ を正面とするスキンメッシュモデル用のアニメーションに変換する
        m_flipZ = false;
        m_flipX = true;
    }

	auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
    m_reader.reset(stream);

    while (m_reader.readLine())
    {
        if (m_reader.startWith("HIERARCHY", 9)) {
            if (!readHierarchy()) {
                return false;
            }
        }
        else if (m_reader.startWith("MOTION", 6)) {
            if (!readMotion()) {
                return false;
            }
        }
    }

    // BVH のフレームデータは offset を持っているが、基本的にすべて読み捨ててよい。
    // ただし、ROOT の offset は適用しないと、モーションが正しく再生できないことがある。
    // しかし一方で、モーションの種類によっては、ゲームで使うには不要な場合もある。
    // - 歩行モーションに含まれている offset は不要。
    // - 待機モーションに含まれている腰の重心微調整用の offset は必要。
    // 
    // そういうわけで何とか offset を正しく読み取る必要がある。
    // 
    // 問題なのは、offset はすべて絶対座標で格納されている点。
    // ターゲットモデルのサイズは BVH によってことなるので、1.0=1m と考えた Lumino の標準構造にはそのまま適用できない。
    // (Mixamo のものは 200.0 くらいの高さのモデル)
    //
    // 正式な仕様かは不明だが、手元の BVH はすべて Hips が ROOT となってる。
    // Lumino の HumanoidBones や、これの参考元の Unity もこれが実質の Root.
    // そこで、地面~Hipsまでの高さを 1.0 とした、平行移動"率" で offset を表すことにしてみる。
    //
    // ただし、BVH の中には ROOT を原点(0,0,0) に置いているものもあるので、Hips の高さをそのまま使うことはできない。
    // すべての Joint のうち一番下にあるものとの差をとる。
    //
    // [2020/9/13] TODO: BVH の OFFSET は相対座標なので、再帰的に更新が必要。
    // とりあえず Mixamo のモーションは下端 0 が基準になっているようなので、それに合わせておく。
    //const float offsetBasis = std::abs(m_joints[m_rootJointIndex]->offset.y - m_minOffsetY);
    const float offsetBasis = std::abs(m_joints[m_rootJointIndex]->offset.y);
    const float offsetScale = (1.0f / offsetBasis) * 2.5f;

    // [2020/9/13] TODO: HierarchicalAnimationMode を作ったけど、それだけだと足りない。
    // モーションによっては歩行時の bobbing のため Y オフセットだけ有効にしたいものもある。


    for (const auto& joint : m_joints) {
        auto track = makeObject_deprecated<TransformAnimationTrack>(TranslationClass::Ratio);
        track->m_root = joint->isRoot;

        const auto name = String::fromStdString(joint->name);
        track->setTargetName(name);
        track->setTargetHumanoidBone(mapHumanoidBonesMixamoUnity(name));

        track->resizeFramesTQ(m_frames);
        for (int iFrame = 0; iFrame < m_frames; iFrame++) {
            auto posOrg = Vector3(
                (joint->channels[X_POSITION] >= 0) ? rameData(iFrame, joint->channels[X_POSITION]) : 0.0f,
                (joint->channels[Y_POSITION] >= 0) ? rameData(iFrame, joint->channels[Y_POSITION]) : 0.0f,
                (joint->channels[Z_POSITION] >= 0) ? rameData(iFrame, joint->channels[Z_POSITION]) : 0.0f);// *0.00f;
            //pos.x += joint->offset.x;
            //pos.y += joint->offset.y;
            //pos.z += joint->offset.z;
            //auto pos = Vector3(posOrg.x, posOrg.z, posOrg.y);   // Blender
            //posOrg *= 0.0f; // 無効化。bvh_player でも使っていなかった。
            if (!joint->isRoot) {   // bvh_player ではルート要素だけオフセットをつけていた
                posOrg *= 0.0f;
            }
            else {
                //posOrg *= 1.0f / 20.0f;
                //posOrg *= 10.0f / 80.0f;    // HC4 Model の Hips 位置は y=10. 
                posOrg.y -= offsetBasis;
                posOrg *= offsetScale;

                //posOrg.x = 1;
            }
            auto pos = Vector3(posOrg.x, posOrg.y, posOrg.z);

            if (m_flipX) {
                pos.x *= -1.0f;
            }
#if 1
            auto rotOrg = Vector3(
                (joint->channels[X_ROTATION] >= 0) ? rameData(iFrame, joint->channels[X_ROTATION]) : 0.0f,
                (joint->channels[Y_ROTATION] >= 0) ? rameData(iFrame, joint->channels[Y_ROTATION]) : 0.0f,
                (joint->channels[Z_ROTATION] >= 0) ? rameData(iFrame, joint->channels[Z_ROTATION]) : 0.0f);
            //auto rot = Vector3(rotOrg.x, rotOrg.z, rotOrg.y);   // Blender
            auto rot = Vector3(rotOrg.x, rotOrg.y, rotOrg.z);
            rot.x = Math::degreesToRadians(rot.x);
            rot.y = Math::degreesToRadians(rot.y);
            rot.z = Math::degreesToRadians(rot.z);

            // インポートした各値は　bvh_player と等しかった
            //std::cout << name << std::endl;
            //printf("  pos: %f %f %f\n", posOrg.x, posOrg.y, posOrg.z);
            //printf("  rot: %f %f %f\n", rotOrg.x, rotOrg.y, rotOrg.z);

            //if (name == _TT("Arm_R") {
            //    printf("");
            //}
            if (m_flipZ) {

                //rot.x *= -1;
                rot.y *= -1;
                rot.z *= -1;
            }
            if (m_flipX) {
                //pos.x *= -1.0f;
                //rot.x *= -1.0f;
                //rot.z *= -1.0f;


                rot.y *= -1.0f;
                rot.z *= -1.0f;
                //rot.x *= -1.0f;   // これやると膝が逆を向く


                //rot.y *= -1;
                //rot.z *= -1;
            }
            track->setDataTQ(iFrame, m_frameTime * iFrame, pos, rot);
#else
            const auto rot = Quaternion::makeFromEulerAngles(
                (joint->channels[X_ROTATION] >= 0) ? rameData(iFrame, joint->channels[X_ROTATION]) : 0.0f,
                (joint->channels[Y_ROTATION] >= 0) ? rameData(iFrame, joint->channels[Y_ROTATION]) : 0.0f,
                (joint->channels[Z_ROTATION] >= 0) ? rameData(iFrame, joint->channels[Z_ROTATION]) : 0.0f,
                RotationOrder::ZXY);    // https://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html
                //RotationOrder::XYZ);
                //RotationOrder::YZX);
            track->setDataTQ(iFrame, m_frameTime * iFrame, pos, rot);
#endif
        }

        clip->addTrack(track);
    }

	return true;
}

bool BvhImporter::readHierarchy()
{
    int level = 0;
    bool is_site = false;
    bool blockClosed = false;

    while (m_reader.readLine()) {
        if (!m_reader.readToken()) {
            return false;
        }

        if (m_reader.token().is("ROOT", 4) ||
            m_reader.token().is("JOINT", 5)) {
            bool isRoot = m_reader.token().is("ROOT", 4);

            if (!m_reader.readToken()) {
                m_diag->reportError(_TT("Expected joint name."));
                return false;
            }

            auto joint = std::make_shared<Joint>();
            joint->name = std::string(m_reader.token().begin, m_reader.token().length);
            joint->index = m_joints.size();
            joint->isRoot = isRoot;
            std::fill(joint->channels.begin(), joint->channels.end(), -1);

            m_joints.push_back(joint);

            if (m_rootJointIndex < 0) {
                m_rootJointIndex = m_joints.size() - 1;
            }
            else {
                LN_WARNING(_TT("Multiple root joints found."));
            }
        }
        else if (m_reader.token().is("End", 3)) {
            is_site = true;
        }
        else if (m_reader.token().is("OFFSET", 6)) {
            if (!m_reader.readToken()) {
                m_diag->reportError(_TT("Expected offset value x."));
                return false;
            }
            float x = m_reader.token().toFloat();

            if (!m_reader.readToken()) {
                m_diag->reportError(_TT("Expected offset value y."));
                return false;
            }
            float y = m_reader.token().toFloat();

            if (!m_reader.readToken()) {
                m_diag->reportError(_TT("Expected offset value z."));
                return false;
            }
            float z = m_reader.token().toFloat();

            Joint* joint = currentJoint();
            if (is_site) {
                joint->hasSite = true;
                joint->site.x = x;
                joint->site.y = y;
                joint->site.z = z;
            }
            else {
                joint->offset.x = x;
                joint->offset.y = y;
                joint->offset.z = z;
                m_minOffsetY = std::min(m_minOffsetY, y);
            }
        }
        else if (m_reader.token().is("CHANNELS", 8)) {
            if (!m_reader.readToken()) {
                m_diag->reportError(_TT("Expected channels count."));
                return false;
            }

            int count = m_reader.token().toInt();
            Joint* joint = currentJoint();
            for (int i = 0; i < count; i++) {
                auto channel = std::make_shared<Channel>();
                channel->index = m_channels.size();
                m_channels.push_back(channel);

                if (!m_reader.readToken()) {
                    m_diag->reportError(_TT("Expected channel."));
                    return false;
                }

                if (m_reader.token().is("Xrotation", 9))
                    joint->channels[X_ROTATION] = channel->index;
                else if (m_reader.token().is("Yrotation", 9))
                    joint->channels[Y_ROTATION] = channel->index;
                else if (m_reader.token().is("Zrotation", 9))
                    joint->channels[Z_ROTATION] = channel->index;
                else if (m_reader.token().is("Xposition", 9))
                    joint->channels[X_POSITION] = channel->index;
                else if (m_reader.token().is("Yposition", 9))
                    joint->channels[Y_POSITION] = channel->index;
                else if (m_reader.token().is("Zposition", 9))
                    joint->channels[Z_POSITION] = channel->index;
            }
        }
        else if (m_reader.token().is("{", 1)) {
            level++;
        }
        else if (m_reader.token().is("}", 1)) {
            is_site = false;
            level--;
            if (level == 0) {
                // End "HIERARCHY"
                blockClosed = true;
                break;
            }
        }
    }

    if (!blockClosed) {
        m_diag->reportError(_TT("HIERARCHY block is not closed."));
        return false;
    }

#if 0 // Debug
    for (auto& joint : m_joints) {
        std::cout << joint->name << std::endl;
        std::cout << "  " << joint->offset.x << ", " << joint->offset.y << ", " << joint->offset.z << std::endl;
    }
#endif

    return true;
}

bool BvhImporter::readMotion()
{
    // Frames:
    if (!m_reader.readLine()) {
        m_diag->reportError(_TT("Expected Frames line."));
        return false;
    }
    if (m_reader.tokenCount() != 2) {
        m_diag->reportError(_TT("Expected Frames token."));
        return false;
    }
    if (!m_reader.token(0).is("Frames:", 7)) {
        m_diag->reportError(_TT("Expected Frames."));
        return false;
    }
    m_frames = m_reader.token(1).toInt();

    // Frame Time:
    if (!m_reader.readLine()) {
        m_diag->reportError(_TT("Expected Frame Time line."));
        return false;
    }
    if (m_reader.tokenCount() != 3) {
        m_diag->reportError(_TT("Expected Frame Time token."));
        return false;
    }
    if (!m_reader.startWith("Frame Time:", 11)) {
        m_diag->reportError(_TT("Expected Frame Time."));
        return false;
    }
    m_frameTime = m_reader.token(2).toFloat();

    m_frameData.resize(m_frames * m_channels.size());
    for (int iFrame = 0; iFrame < m_frames; iFrame++) {
        if (!m_reader.readLine()) {
            m_diag->reportError(_TT("Expected frame data line."));
            return false;
        }

        if (m_reader.tokenCount() != m_channels.size()) {
            m_diag->reportError(_TT("Insufficient frame data."));
            return false;
        }

        for (int iToken = 0; iToken < m_reader.tokenCount(); iToken++) {
            setFrameData(iFrame, iToken, m_reader.token(iToken).toFloat());
        }
    }

    return true;
}

HumanoidBones BvhImporter::mapHumanoidBonesMixamoUnity(const String& name)
{
    static const std::unordered_map<String, HumanoidBones> table = {
        { _TT("Hips"), HumanoidBones::Hips },
        { _TT("Spine"), HumanoidBones::Spine },
        { _TT("Spine1"), HumanoidBones::Chest },
        { _TT("Spine2"), HumanoidBones::UpperChest },

        { _TT("LeftShoulder"), HumanoidBones::LeftShoulder },
        { _TT("LeftArm"), HumanoidBones::LeftUpperArm },
        { _TT("LeftForeArm"), HumanoidBones::LeftLowerArm },
        { _TT("LeftHand"), HumanoidBones::LeftHand },

        { _TT("RightShoulder"), HumanoidBones::RightShoulder },
        { _TT("RightArm"), HumanoidBones::RightUpperArm },
        { _TT("RightForeArm"), HumanoidBones::RightLowerArm },
        { _TT("RightHand"), HumanoidBones::RightHand },
        //{ _TT("LeftShoulder", HumanoidBones::RightShoulder },
        //{ _TT("LeftArm", HumanoidBones::RightUpperArm },
        //{ _TT("LeftForeArm", HumanoidBones::RightLowerArm },
        //{ _TT("LeftHand", HumanoidBones::RightHand },

        //{ _TT("RightShoulder", HumanoidBones::LeftShoulder },
        //{ _TT("RightArm", HumanoidBones::LeftUpperArm },
        //{ _TT("RightForeArm", HumanoidBones::LeftLowerArm },
        //{ _TT("RightHand", HumanoidBones::LeftHand },

        { _TT("LeftUpLeg"), HumanoidBones::LeftUpperLeg },
        { _TT("LeftLeg"), HumanoidBones::LeftLowerLeg },
        { _TT("LeftFoot"), HumanoidBones::LeftFoot },
        { _TT("LeftToeBase"), HumanoidBones::LeftToes },

        { _TT("RightUpLeg"), HumanoidBones::RightUpperLeg },
        { _TT("RightLeg"), HumanoidBones::RightLowerLeg },
        { _TT("RightFoot"), HumanoidBones::RightFoot },
        { _TT("RightToeBase"), HumanoidBones::RightToes },
        //{ _TT("LeftUpLeg", HumanoidBones::RightUpperLeg },
        //{ _TT("LeftLeg", HumanoidBones::RightLowerLeg },
        //{ _TT("LeftFoot", HumanoidBones::RightFoot },
        //{ _TT("LeftToeBase", HumanoidBones::RightToes },

        //{ _TT("RightUpLeg", HumanoidBones::LeftUpperLeg },
        //{ _TT("RightLeg", HumanoidBones::LeftLowerLeg },
        //{ _TT("RightFoot", HumanoidBones::LeftFoot },
        //{ _TT("RightToeBase", HumanoidBones::LeftToes },



        { _TT("Neck"), HumanoidBones::Neck },
        { _TT("Head"), HumanoidBones::Head },
        { _TT("LeftEye"), HumanoidBones::LeftEye },
        { _TT("RightEye"), HumanoidBones::RightEye },


        // VRM Bone
        { _TT("shoulder_L"), HumanoidBones::LeftShoulder },
        { _TT("Arm_L"), HumanoidBones::LeftUpperArm },
        { _TT("forearm_L"), HumanoidBones::LeftLowerArm },
        { _TT("hand_L"), HumanoidBones::LeftHand },
        { _TT("shoulder_R"), HumanoidBones::RightShoulder },
        { _TT("Arm_R"), HumanoidBones::RightUpperArm },
        { _TT("forearm_R"), HumanoidBones::RightLowerArm },
        { _TT("hand_R"), HumanoidBones::RightHand },
    };

    int index = name.indexOf(_TT("mixamorig:"));
    if (index < 0) return HumanoidBones::None;

    const auto n = name.substr(10);

    const auto itr = table.find(n);
    if (itr != table.end())
        return itr->second;
    else
        return HumanoidBones::None;
}

} // namespace detail
} // namespace ln

