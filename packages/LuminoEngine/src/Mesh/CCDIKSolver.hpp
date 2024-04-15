#pragma once

#if 1

namespace ln {
class MeshModel;
//class SkinnedMeshBone;
namespace detail {

class CCDIKSolver
{
public:

	MeshModel* owner;
	MeshSkeleton* m_skeleton;

	void UpdateTransform()
	{
		for (const auto& ik : owner->m_iks)
		{
			UpdateEachIKBoneTransform(ik);
		}
	}

	void UpdateEachIKBoneTransform(const MeshBoneIK* ik)
	{
		for (int iCalc = 0; iCalc < ik->LoopCount; iCalc++)
		{
			MeshBone* ikBone = m_skeleton->bone(ik->IKBoneIndex);			// IK ボーン (IK 情報を持つボーン。目標地点)
			MeshBone* effector = m_skeleton->bone(ik->IKTargetBoneIndex);	// IK ターゲットボーン (エフェクタ。IKに向くべきボーンたちの中の先頭ボーン)

			IKloop(ik, ikBone, effector);
		}
	}
	void IKloop(const MeshBoneIK* ik, MeshBone* ikBone, MeshBone* effector)
	{
		MeshNode* ikBoneNode = ikBone->node();
		MeshNode* effectorNode = effector->node();
		//Vector3 TargetGlobalPos = Vector3::transformCoord(ikBone->GetCore()->OrgPosition, ikBone->getCombinedMatrix());

		// IKボーンのグローバル位置
		const Vector3& targetPos = ikBoneNode->globalMatrix().position();

		for (int iLink = 0; iLink < ik->IKLinks.size(); ++iLink)
		{
			const auto& ikLink = ik->IKLinks[iLink];
			MeshBone* ikLinkBone = m_skeleton->bone(ikLink->LinkBoneIndex);
			MeshNode* ikLinkBoneNode = ikLinkBone->node();

			// ワールド座標系から注目ノードの局所座標系への変換
			// (IKリンク基準のローカル座標系へ変換する行列)
			Matrix toLinkLocal = Matrix::makeInverse(ikLinkBoneNode->globalMatrix());

			//Vector3 effectorPos = Vector3::transformCoord(effector->GetCore()->OrgPosition, effector->getCombinedMatrix() * toLinkLocal);
			//Vector3 link2Effector = Vector3::safeNormalize(effectorPos - ikLinkBone->GetCore()->OrgPosition);
		
			//Vector3 targetPos = Vector3::transformCoord(TargetGlobalPos, toLinkLocal);
			//Vector3 link2Target = Vector3::safeNormalize(targetPos - ikLinkBone->GetCore()->OrgPosition);

			// エフェクタのグローバル位置
			const Vector3& effPos = effectorNode->globalMatrix().position();

			// 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
			// (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
			Vector3 localEffPos = Vector3::transformCoord(effPos, toLinkLocal);

			// (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
			Vector3 localTargetPos = Vector3::transformCoord(targetPos, toLinkLocal);

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			Vector3 link2Effector = Vector3::safeNormalize(localEffPos);
			// (2) 基準関節→目標位置への方向ベクトル
			Vector3 link2Target = Vector3::safeNormalize(localTargetPos);

			IKLinkCalc(ikLink, ikLinkBone, link2Effector, link2Target, ik->IKRotateLimit);
		}
	}

	void IKLinkCalc(const MeshBoneIKChain* ikLink, MeshBone* ikLinkBone, const Vector3& link2Effector, const Vector3& link2Target, float RotationLimited)
	{
		MeshNode* ikLinkBoneNode = ikLinkBone->node();

		// 回転角度を求める
		float dot = Vector3::dot(link2Effector, link2Target);
		if (dot > 1.0f) dot = 1.0f;
		float rotationAngle = Math::clamp(acosf(dot), -RotationLimited, RotationLimited);
		if (Math::isNaN(rotationAngle)) return;
		if (rotationAngle <= 1.0e-3f) return;

		// 回転軸を求める
		Vector3 rotationAxis = Vector3::cross(link2Effector, link2Target);

		// 軸を中心として回転する行列を作成する
		Quaternion rotation = Quaternion(rotationAxis, rotationAngle);
		rotation.normalize();
		ikLinkBoneNode->setRotation(rotation * ikLinkBoneNode->localTransform().rotation);

		// 回転量制限 TODO: ↑と同じ？
		//ikLinkBoneNode->localTransform()->rotation = RestrictRotation(ikLink, ikLinkBoneNode->localTransform()->rotation);
		ikLinkBoneNode->setRotation(RestrictRotation(ikLink, ikLinkBoneNode->localTransform().rotation));

		ikLinkBoneNode->updateGlobalTransform(true);
	}

	Quaternion RestrictRotation(const MeshBoneIKChain* ikLink, const Quaternion& localRot)
	{
		if (!ikLink->IsRotateLimit) return localRot;

		RotationOrder type;
		bool locked;
		Vector3 euler;

		// まずオイラー角に分解する。
		// 分解の試行順序は XYZ が一番最初でなければならない (Love&Joy モーションで破綻する)
		type = RotationOrder::XYZ;
		euler = localRot.toEulerAngles(type, &locked);
		if (locked)
		{
			type = RotationOrder::YZX;
			euler = localRot.toEulerAngles(type, &locked);
			if (locked)
			{
				type = RotationOrder::ZXY;
				euler = localRot.toEulerAngles(type, &locked);
				if (locked)
				{
					// localRotの要素がすべて 0 だった
					euler = Vector3::Zero;
				}
			}
		}

		// 角度修正
		NormalizeEular(&euler);
		euler.clamp(ikLink->MinLimit, ikLink->MaxLimit);

		// 戻す
		return Quaternion::makeFromRotationMatrix(Matrix::makeRotationEulerAngles(euler, type));
	}

    static void NormalizeEular(Vector3* eulers)
    {
        float& x = eulers->x;
        float& y = eulers->y;
        float& z = eulers->z;
        if (x < -Math::PI || Math::PI < x)
        {
            if (x > 0)
            {
                x -= Math::PI * 2;
            }
            else
            {
                x += Math::PI * 2;
            }
        }
        if (y < -Math::PI*0.5f || Math::PI*0.5f < y)
        {
            if (y > 0)
            {
                y -= Math::PI * 2;
            }
            else
            {
                y += Math::PI * 2;
            }
        }
        if (z < -Math::PI || Math::PI < z)
        {
            if (z > 0)
            {
                z -= Math::PI * 2;
            }
            else
            {
                z += Math::PI * 2;
            }
        }
    }
};

} // namespace detail
} // namespace ln

#endif
