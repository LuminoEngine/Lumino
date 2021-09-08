#include "Common.hpp"
class Test_Animation_Mixer : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Animation_Mixer, Basic)
{
	auto clip1 = makeObject<AnimationClip>();
	auto curve1 = EasingAnimationCurve::create(0, 10, 1, EasingMode::Linear);
	auto track1 = makeObject<ScalarAnimationTrack>();
	track1->setTargetName(_TT("Bone.1"));
	track1->setCurve(curve1);
	auto track2 = makeObject<ScalarAnimationTrack>();
	track2->setTargetName(_TT("Bone.2"));
	track2->setCurve(curve1);
	clip1->addTrack(track1);
	clip1->addTrack(track2);
	clip1->setWrapMode(AnimationWrapMode::Once);

	class AnimCtrl : public detail::IAnimationMixerCoreHolder
	{
	public:
		Ref<detail::AnimationTargetElementBlendLink> m_bone1Binding;
		float m_bone1Value;

		detail::AnimationTargetElementBlendLink* onRequireBinidng(const AnimationTrackTargetKey& key) override
		{
			if (key.name == _TT("Bone.1")) {
				m_bone1Binding = makeRef<detail::AnimationTargetElementBlendLink>(AnimationValueType::Float);
				return m_bone1Binding;
			}
			else {
				return nullptr;
			}
		}

		void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding) override
		{
			if (LN_REQUIRE(m_bone1Binding == binding)) return;
			m_bone1Value = binding->rootValue.getFloat();
		}
	} ctrl;

	auto mixer1 = makeObject<AnimationMixerCore>(&ctrl);
	mixer1->addLayer(makeObject<AnimationLayer>(mixer1));
	mixer1->addClip(_TT("Idle"), clip1);
	mixer1->play(_TT("Idle"), 0.0f);

	mixer1->advanceTime(0.0f);
	ASSERT_EQ(AnimationValueType::Float, ctrl.m_bone1Binding->rootValue.type());
	ASSERT_FLOAT_EQ(0.0f, ctrl.m_bone1Value);

	mixer1->advanceTime(1.0f);
	ASSERT_FLOAT_EQ(10.0f, ctrl.m_bone1Value);
}

////------------------------------------------------------------------------------
//TEST_F(Test_Animation_Mixer, VMD)
//{
//	auto clip = AnimationClip::load(LN_ASSETFILE("Animation/Move1.vmd"));
//
//	//for (auto& track : clip->tracks()) {
//	//	std::cout << track->targetName() << std::endl;
//	//}
//
//	//printf("");
//}
