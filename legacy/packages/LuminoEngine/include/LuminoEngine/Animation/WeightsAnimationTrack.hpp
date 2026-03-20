
#pragma once
#include "AnimationTrack.hpp"

namespace ln {
//
///**  */
//class WeightsAnimationTrack
//	: public AnimationTrack
//{
//public:
//	enum class Interpolation
//	{
//		Step,
//		Linear,
//		CubicSpline,
//	};
//
//LN_CONSTRUCT_ACCESS:
//	WeightsAnimationTrack();
//	bool init(int frameCount, int weightCount, Interpolation interpolation);
//
//protected:
//	void evaluate(float time, AnimationValue* outResult) override;
//	float lastFrameTime() const override { return m_lastTime; }
//
//public:	// TODO: internal
//	Interpolation m_interpolation;
//	
//	
//
//	std::vector<Vector3Key> m_translationKeys;
//	std::vector<QuaternionKey> m_rotationKeys;
//	std::vector<Vector3Key> m_scaleKeys;
//	Interpolation m_scaleInterpolation;
//	float m_lastTime;
//
//	// exp
//	std::vector<Vector3Key> m_rotationKeys2;
//};

} // namespace ln
