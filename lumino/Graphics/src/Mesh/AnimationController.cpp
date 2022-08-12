#include "Internal.hpp"
#include <LuminoGraphics/Mesh/SkinnedMeshModel.hpp>
#include <LuminoGraphics/Mesh/AnimationController.hpp>

namespace ln {

//==============================================================================
// AnimationController
	
LN_OBJECT_IMPLEMENT(AnimationController, Object) {}

AnimationController::AnimationController()
{
}

bool AnimationController::init() {
    LN_NOTIMPLEMENTED();
    return false;
}

bool AnimationController::init(SkinnedMeshModel* model)
{
	if (!Object::init()) return false;
	m_model = model;

	m_core = makeObject_deprecated<AnimationMixerCore>(this);
	m_core->addLayer(makeObject_deprecated<AnimationLayer>(m_core));
	return true;
}

void AnimationController::advanceTime(float elapsedTime)
{
	m_core->advanceTime(elapsedTime);
}

detail::AnimationTargetElementBlendLink* AnimationController::onRequireBinidng(const AnimationTrackTargetKey& key)
{
	auto tb = m_bindings.findIf([&](const auto& x) { return AnimationTrackTargetKey::equals(x->key, key); });
	if (tb) {
		return *tb;
	}

	int index = -1;
	
	// まず HumanoidBones を検索
	if (key.bone != HumanoidBones::None) {
		index = m_model->humanoidBoneIndex(key.bone);
		if (index >= 0) {
			std::cout << "map:" << (int)key.bone << std::endl;
		}
	}

	// 無ければ名前検索
	if (index < 0) {
		index = m_model->findNodeIndex(key.name);
	}

	if (index >= 0) {
		auto binding = makeRef<detail::AnimationTargetElementBlendLink>(AnimationValueType::Transform);
		binding->key = key;
		binding->targetIndex = index;
		m_bindings.add(binding);
		return binding;
	}
	else {
		return nullptr;
	}
}

void AnimationController::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding)
{
	const auto& node = m_model->meshNodes()[binding->targetIndex];
	node->setTransform(binding->rootValue.getTransform());

	//if (binding->targetIndex == 2) {
	//	auto v = binding->rootValue.getTransform().rotation.toEulerAngles();
	//	v.print();
	//}

	//auto& s = binding->rootValue.getTransform().scale;
	//if (s.x != 1 || s.y != 1 || s.z != 1) {
	//	printf("");
	//}

	//if (binding->name == u"左腕") {
	//	m_model->meshNodes()[binding->targetIndex]->resetLocalTransform();
	//	//std::cout << binding->name << std::endl;
	//}
	//if (binding->name == u"左腕") {
	//	Matrix m = Matrix::makeRotationQuaternion(binding->rootValue.getTransform().rotation);
	//	auto v = Vector3::transformCoord(Vector3::UnitZ, m);
	//	printf("%f\t%f\t%f\n", v.x, v.y, v.z);
	//	//result.rotation = Quaternion::makeFromEulerAngles(Vector3(0, 0, time));
	//}

}

} // namespace ln

