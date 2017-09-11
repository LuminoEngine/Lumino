
#include "../Internal.h"
#include <Lumino/Animation/AnimationCurve.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationCurve
//==============================================================================

//------------------------------------------------------------------------------
AnimationCurve::AnimationCurve()
	: m_wrapMode(WrapMode_Once)
{
}

//------------------------------------------------------------------------------
AnimationCurve::~AnimationCurve()
{
}

//------------------------------------------------------------------------------
void AnimationCurve::getTime(double time)
{
	if (m_wrapMode == WrapMode_Once)
	{
		updateValue(time);
	}
	else if (m_wrapMode == WrapMode_Loop) 
	{
		double lastTime = getLastFrameTime();
		if (time > lastTime)
		{
			time = fmod(time, lastTime);
		}
		updateValue(time);
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}

	//mTime = time;

	//time *= mTimeTick;	// フレーム位置へ変換

	//double last_frame_pos = getLastFramePos();
	//if (last_frame_pos == 0)
	//{
	//	time = 0;
	//}
	//else if (mLoopMode == LN_ANIMATIONLOOPMODE_REPEAT_LOOP)
	//{
	//	// ループ位置未設定
	//	if (mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f)
	//	{
	//		// 終端のキーフレームの時間を使ってループ
	//		if (time > last_frame_pos)
	//		{
	//			time = fmod(time, last_frame_pos);
	//		}
	//	}
	//	// ループ位置設定済み
	//	else
	//	{
	//		// time_ がループ領域を超えていた場合は開始時間からループ
	//		if (time > mLoopEndPos)
	//		{
	//			time -= mLoopBeginPos;
	//			time = fmod(time, (mLoopEndPos - mLoopBeginPos));
	//			time = mLoopBeginPos + time;
	//		}
	//	}
	//}
	//else if (mLoopMode == LN_ANIMATIONLOOPMODE_REPEAT_LOOP)
	//{
	//	LN_PRINT_NOT_IMPL_FUNCTION;
	//}

	//mCurrentFramePos = time;
}


//==============================================================================
// VMDBezierTable
//==============================================================================

//------------------------------------------------------------------------------
FloatAnimationCurve::FloatAnimationCurve()
	: m_defaultValue(0.0f)
	, m_value(0.0f)
{
}

//------------------------------------------------------------------------------
FloatAnimationCurve::~FloatAnimationCurve()
{
}

//------------------------------------------------------------------------------
void FloatAnimationCurve::addKeyFrame(const FloatKeyFrame& keyFrame)
{
	// そのまま追加できる
	if (m_keyFrameList.isEmpty() || m_keyFrameList.getLast().FrameTime <= keyFrame.FrameTime)
	{
		m_keyFrameList.add(keyFrame);
	}
	// 追加後のソートが必要
	else
	{
		// 比較用ファンクタ
		struct compare
		{
			bool operator()(const FloatKeyFrame& l, const FloatKeyFrame& r)
			{
				return l.FrameTime < r.FrameTime;
			}
		};

		m_keyFrameList.add(keyFrame);
		std::stable_sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare());
	}
}

//------------------------------------------------------------------------------
void FloatAnimationCurve::addKeyFrame(double frame_pos, float value, InterpolationMode mode)
{
	FloatKeyFrame key;
	key.FrameTime = frame_pos;
	key.Value = value;
	key.Mode = mode;
	addKeyFrame(key);
}

static int _cmpKey(const void* a_, const void* b_)
{
	FloatKeyFrame* l = (FloatKeyFrame*)a_;
	FloatKeyFrame* r = (FloatKeyFrame*)b_;

	if ((l)->FrameTime < (r)->FrameTime)
		return -1;
	else if ((l)->FrameTime >= (r)->FrameTime && (l)->FrameTime < ((r + 1))->FrameTime)
		return 0;
	else
		return 1;
}

//------------------------------------------------------------------------------
void FloatAnimationCurve::updateValue(double time)
{
	if (!m_keyFrameList.isEmpty())
	{
		// time_ が最初のフレーム位置より前の場合はデフォルト値
		if (time < m_keyFrameList.getFront().FrameTime)
		{
			m_value = m_defaultValue;
		}
		// キーがひとつだけの場合はそのキーの値
		else if (m_keyFrameList.getCount() == 1)
		{
			m_value = m_keyFrameList.getFront().Value;
		}
		// time_ が終端以降の場合は終端の値
		else if (time >= m_keyFrameList.getLast().FrameTime)
		{
			m_value = m_keyFrameList.getLast().Value;
		}
		// 以上以外の場合は補間する
		else
		{
			const FloatKeyFrame* key0 = (FloatKeyFrame*)bsearch(&time, &(m_keyFrameList[0]), m_keyFrameList.getCount(), sizeof(FloatKeyFrame), _cmpKey);
			const FloatKeyFrame* key1 = key0 + 1;

			float p0 = key0->Value;
			float p1 = key1->Value;
			float t0 = static_cast< float >(key0->FrameTime);
			float t1 = static_cast< float >(key1->FrameTime);
			float t = static_cast< float >(time - t0) / (t1 - t0);

			switch (key0->Mode)
			{
				// 補間無し
			case InterpolationMode_None:
			{
				m_value = p0;
				break;
			}
				// 線形
			case InterpolationMode_Linear:
			{
				m_value = p0 + (p1 - p0) * t;
				break;
			}
				// 等加速度
			case InterpolationMode_QuadAccel:
			{
				m_value = Math::quadAccel(p0, key0->Velocity, key0->Accel, static_cast< float >(time - key0->FrameTime));
				break;
			}
				// 三次補間
			case InterpolationMode_Hermite:
			{
				m_value = Math::hermite(
					p0, p1,
					key0->RightSlope,
					key1->LeftSlope,
					t);
				break;
			}
				// Catmull-Rom
			case InterpolationMode_CatmullRom:
			{
				// ループ再生で time が終端を超えている場合、
				// この時点でkey の値は ループ開始位置のひとつ前のキーを指している

				const FloatKeyFrame& begin = m_keyFrameList.getFront();
				const FloatKeyFrame& end = m_keyFrameList.getLast();

				// この補間には、begin のひとつ前と end のひとつ後の値が必要。
				// それぞれが始点、終点の場合はループするように補間する
				m_value = Math::catmullRom(
					((key0->FrameTime == begin.FrameTime) ? end.Value : (key0 - 1)->Value),
					p0,
					p1,
					((key1->FrameTime == end.FrameTime) ? begin.Value : (key0 + 2)->Value),
					t);
				break;
			}
			}
		}
	}
	else
	{
		m_value = m_defaultValue;
	}
}

//------------------------------------------------------------------------------
double FloatAnimationCurve::getLastFrameTime() const
{
	if (m_keyFrameList.isEmpty()) return 0;
	return m_keyFrameList.getLast().FrameTime;
}


//==============================================================================
// VMDBezierTable
//==============================================================================

//------------------------------------------------------------------------------
	void VMDBezierTable::initialize(float fPointX1, float fPointY1, float fPointX2, float fPointY2)
{
	if (fPointX1 == fPointY1 && fPointX2 == fPointY2)
	{
		m_isLinear = true;
	}
	else
	{
		fPointX1 = (fPointX1 / 127.0f) * 3.0f;
		fPointY1 = (fPointY1 / 127.0f) * 3.0f;

		fPointX2 = (fPointX2 / 127.0f) * 3.0f;
		fPointY2 = (fPointY2 / 127.0f) * 3.0f;

		// 高速化のためテーブル化しておく
		m_yValue[0] = 0.0f;
		m_yValue[YVAL_DIV_NUM] = 1.0f;

		float	fAddX = 1.0f / (float)YVAL_DIV_NUM;

		for (int i = 1; i < YVAL_DIV_NUM; i++)
		{
			m_yValue[i] = getYVal(fAddX * i, fPointX1, fPointY1, fPointX2, fPointY2);
		}

		m_isLinear = false;
	}
}

//------------------------------------------------------------------------------
	float VMDBezierTable::getInterValue(float fX)
{
	if (m_isLinear)	return fX;	// 線形補間

	// テーブルを線形補間する
	fX *= (float)YVAL_DIV_NUM;

	int		iIdx = (int)fX;

	fX -= iIdx;

	return m_yValue[iIdx] * (1.0f - fX) + m_yValue[iIdx + 1] * fX;
}

//------------------------------------------------------------------------------
float VMDBezierTable::getYVal(float fX, float fX1, float fY1, float fX2, float fY2)
{
	float	fT = fX;
	float	fInvT = 1.0f - fT;

	for (int i = 0; i < 32; i++)
	{
		float	fTempX = fInvT*fInvT*fT*fX1 + fInvT*fT*fT*fX2 + fT*fT*fT;

		fTempX -= fX;
		if (fabsf(fTempX) < 0.0001f)
		{
			break;
		}
		else
		{
			fT -= fTempX * 0.5f;
			fInvT = 1.0f - fT;
		}
	}

	return fInvT*fInvT*fT*fY1 + fInvT*fT*fT*fY2 + fT*fT*fT;
}

//==============================================================================
// VMDBezierAttitudeTransformAnimation
//==============================================================================

//------------------------------------------------------------------------------
VMDBezierAttitudeTransformAnimation::VMDBezierAttitudeTransformAnimation()
{
}

//------------------------------------------------------------------------------
VMDBezierAttitudeTransformAnimation::~VMDBezierAttitudeTransformAnimation()
{
}

//------------------------------------------------------------------------------
void VMDBezierAttitudeTransformAnimation::addKeyFrame(
	double framePos, const Vector3& pos, const Quaternion& rot,
	char* interpolation_x,
	char* interpolation_y,
	char* interpolation_z,
	char* interpolation_rot)
{
	m_keyFrameList.add(KeyFrame());
	KeyFrame& key = m_keyFrameList.getLast();

	key.Time = framePos;
	key.Position = pos;
	key.Rotation = rot;
	key.Rotation.normalize();

	key.PosXInterBezier.initialize(interpolation_x[0], interpolation_x[4], interpolation_x[8], interpolation_x[12]);
	key.PosYInterBezier.initialize(interpolation_y[0], interpolation_y[4], interpolation_y[8], interpolation_y[12]);
	key.PosZInterBezier.initialize(interpolation_z[0], interpolation_z[4], interpolation_z[8], interpolation_z[12]);
	key.RotInterBezier.initialize(interpolation_rot[0], interpolation_rot[4], interpolation_rot[8], interpolation_rot[12]);
}

//------------------------------------------------------------------------------
void VMDBezierAttitudeTransformAnimation::sortKeyFrame()
{
	struct
	{
		bool operator()(const KeyFrame& l, const KeyFrame& r) const
		{
			return l.Time < r.Time;
		}
	} compare;

	std::stable_sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
	//std::sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
}

//------------------------------------------------------------------------------
void VMDBezierAttitudeTransformAnimation::updateValue(double time)
{
	// フレーム数 1 個
	if (m_keyFrameList.getCount() == 1)
	{
		m_transform.rotation = m_keyFrameList.getFront().Rotation;
		m_transform.translation = m_keyFrameList.getFront().Position;
		return;
	}

	// 最初のフレーム以前であれば最初のフレームの値を返す
	if (time <= m_keyFrameList.getFront().Time)
	{
		m_transform.rotation = m_keyFrameList.getFront().Rotation;
		m_transform.translation = m_keyFrameList.getFront().Position;
		return;
	}

	// 最後のフレーム以降であれば最後のフレームの値を返す
	if (time >= m_keyFrameList.getLast().Time)
	{
		m_transform.rotation = m_keyFrameList.getLast().Rotation;
		m_transform.translation = m_keyFrameList.getLast().Position;
		return;
	}

	// あるフレーム位置直前のキーフレームを検索するための関数オブジェクト
	struct GreaterEqual
	{
		double FramePos;
		bool operator()(const KeyFrame& key) const
		{
			return FramePos <= key.Time;
		}
	} compare;
	compare.FramePos = time;

	// キー検索
	KeyFrameList::iterator itr = std::find_if(m_keyFrameList.begin(), m_keyFrameList.end(), compare);

	size_t k1idx = itr - m_keyFrameList.begin();
	size_t k0idx = k1idx - 1;

	KeyFrame& k0 = m_keyFrameList[k0idx];
	KeyFrame& k1 = m_keyFrameList[k1idx];

	double t0 = k0.Time;
	double t1 = k1.Time;

	// float ⇔ double の型変換を抑えるため、以下は float で計算を行う

	float rate = static_cast< float >((time - t0) / (t1 - t0));
	float inter;

	inter = k1.PosXInterBezier.getInterValue(rate);
	m_transform.translation.x = k0.Position.x * (1.0f - inter) + k1.Position.x * inter;

	inter = k1.PosYInterBezier.getInterValue(rate);
	m_transform.translation.y = k0.Position.y * (1.0f - inter) + k1.Position.y * inter;

	inter = k1.PosZInterBezier.getInterValue(rate);
	m_transform.translation.z = k0.Position.z * (1.0f - inter) + k1.Position.z * inter;

	inter = k1.RotInterBezier.getInterValue(rate);
	m_transform.rotation = Quaternion::slerp(k0.Rotation, k1.Rotation, inter);
}

//------------------------------------------------------------------------------
double VMDBezierAttitudeTransformAnimation::getLastFrameTime() const
{
	if (m_keyFrameList.isEmpty()) return 0;
	return m_keyFrameList.getLast().Time;
}



//==============================================================================
// VMDBezierAttitudeTransformAnimation
//==============================================================================
VMDBezierSQTTransformAnimation2::VMDBezierSQTTransformAnimation2()
{

}
VMDBezierSQTTransformAnimation2::~VMDBezierSQTTransformAnimation2()
{

}
void VMDBezierSQTTransformAnimation2::sortKeyFrame()
{
	struct
	{
		bool operator()(const BoneFrameData& l, const BoneFrameData& r) const
		{
			return l.Time < r.Time;
		}
	} compare;

	//std::stable_sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
	std::sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
}
void VMDBezierSQTTransformAnimation2::updateValue(double time)
{
	// フレーム数 1 個
	if (m_keyFrameList.getCount() == 1)
	{
		m_transform.rotation = m_keyFrameList.getFront().Rotation;
		m_transform.translation = m_keyFrameList.getFront().Position;
		return;
	}

	// 最初のフレーム以前であれば最初のフレームの値を返す
	if (time <= m_keyFrameList.getFront().Time)
	{
		m_transform.rotation = m_keyFrameList.getFront().Rotation;
		m_transform.translation = m_keyFrameList.getFront().Position;
		return;
	}

	// 最後のフレーム以降であれば最後のフレームの値を返す
	if (time >= m_keyFrameList.getLast().Time)
	{
		m_transform.rotation = m_keyFrameList.getLast().Rotation;
		m_transform.translation = m_keyFrameList.getLast().Position;
		return;
	}

	// あるフレーム位置直前のキーフレームを検索するための関数オブジェクト
	struct GreaterEqual
	{
		double FramePos;
		bool operator()(const BoneFrameData& key) const
		{
			return FramePos <= key.Time;
		}
	} compare;
	compare.FramePos = time;

	// キー検索
	KeyFrameList::iterator itr = std::find_if(m_keyFrameList.begin(), m_keyFrameList.end(), compare);

	size_t k1idx = itr - m_keyFrameList.begin();
	size_t k0idx = k1idx - 1;

	BoneFrameData& k0 = m_keyFrameList[k0idx];
	BoneFrameData& k1 = m_keyFrameList[k1idx];

	// 現在のフレームの前後キーフレーム間での進行度を求めてペジェ関数で変換する
	float s = (k1.Time == k0.Time) ? 0 :
		(float)(time - k0.Time) / (float)(k1.Time - k0.Time); // 進行度
	
	float ss[4];
	for (int i = 0; i < 4; ++i) ss[i] = k0.Curves[i].evaluate(s);

	// ボーンを更新する
	Vector3 progress(ss[0], ss[1], ss[2]);
	m_transform.translation =
		Vector3(Math::lerp(k0.Position.x, k1.Position.x, progress.x),
			Math::lerp(k0.Position.y, k1.Position.y, progress.y),
			Math::lerp(k0.Position.z, k1.Position.z, progress.z));
	
	m_transform.rotation =
		Quaternion::slerp(k0.Rotation, k1.Rotation, ss[3]);
		//CGHelper.ComplementRotateQuaternion(k0, k1, ss[3]);
}

//------------------------------------------------------------------------------
double VMDBezierSQTTransformAnimation2::getLastFrameTime() const
{
	if (m_keyFrameList.isEmpty()) return 0;
	return m_keyFrameList.getLast().Time;
}


LN_NAMESPACE_END
