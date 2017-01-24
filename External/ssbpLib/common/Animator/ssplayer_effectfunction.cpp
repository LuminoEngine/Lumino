#include <stdio.h>
#include <cstdlib>

#include "../loader/ssloader.h"
#include "ssplayer_effect.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
#include "ssplayer_effectfunction.h"

namespace ss
{


//二つの値の範囲から値をランダムで得る
static u8 GetRandamNumberRange(SsEffectRenderEmitter* e, u8 a, u8 b)
{
	u8 min = a < b ? a : b;
	u8 max = a < b ? b : a;

	u8 diff = (max - min);


	if (diff == 0) return min;
	return min + (e->MT->genrand_uint32() % diff);

}

static void VarianceCalcColor(SsEffectRenderEmitter* e, SsU8Color& out, SsU8Color  color1, SsU8Color color2)
{

	out.r = GetRandamNumberRange(e, color1.r, color2.r);
	out.g = GetRandamNumberRange(e, color1.g, color2.g);
	out.b = GetRandamNumberRange(e, color1.b, color2.b);
	out.a = GetRandamNumberRange(e, color1.a, color2.a);

}



float frand(unsigned v) {
	unsigned res = (v >> 9) | 0x3f800000;
	return (*(float*)&res) - 1.0f;
}

static float VarianceCalc(SsEffectRenderEmitter* e, float base, float variance)
{

	unsigned long r = e->MT->genrand_uint32();

	float len = variance - base;

	return base + len * frand(r);


}

static float VarianceCalcFin(SsEffectRenderEmitter* e, float base, float variance)
{
	unsigned long r = e->MT->genrand_uint32();

	return base + (-variance + variance* (frand(r) * 2.0f));

}


static u8 blendNumber(u8 a, u8 b, float rate)
{
	return (a + (b - a) * rate);
}


static float blendFloat(float a, float b, float rate)
{
	return   (a + (b - a) * rate);
}


class EffectFuncBase
{
public:
	EffectFuncBase(){}
	virtual ~EffectFuncBase(){}

	virtual void	initalizeEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter){}
	virtual void	updateEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter){}
	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle){}
	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle){}
};




//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementBasic : public EffectFuncBase
{
public:
	FuncParticleElementBasic(){}
	virtual ~FuncParticleElementBasic(){}

	virtual void	initalizeEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* e)
	{
		ParticleElementBasic* source = static_cast<ParticleElementBasic*>(ele);

		e->maxParticle = source->maximumParticle;
		e->interval = source->interval;
		e->_lifetime = source->lifetime;
		e->_life = source->lifetime;
		e->burst = source->attimeCreate;

		e->undead = false;
		e->drawPriority = source->priority;

		if (e->_lifetime == 0) e->undead = true;


	}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementBasic* source = static_cast<ParticleElementBasic*>(ele);
		SsVector3 eVec = e->getPosition();
		float eAngle = 0;

		p->_baseEmiterPosition.x = eVec.x;
		p->_baseEmiterPosition.y = eVec.y;
		p->_position.x = p->_baseEmiterPosition.x;
		p->_position.y = p->_baseEmiterPosition.y;
		p->_size = SsVector2(1.0f, 1.0f);


		p->_color = SsU8Color(255, 255, 255, 255);
		p->_startcolor = SsU8Color(255, 255, 255, 255);
		p->_endcolor = p->_startcolor;


		p->_backposition = p->_position;

		p->_lifetime = VarianceCalc(e, source->lifespan.getMinValue(), source->lifespan.getMaxValue());
		p->_life = source->lifetime;
		float temp_angle = VarianceCalcFin(e, source->angle + eAngle, source->angleVariance / 2.0f);

		float angle_rad = DegreeToRadian((temp_angle + 90.0f));
		float lspeed = VarianceCalc(e, source->speed.getMinValue(), source->speed.getMaxValue());

		p->speed = lspeed;
		p->firstspeed = lspeed;
		p->vector.x = cos(angle_rad);
		p->vector.y = sin(angle_rad);

		p->_force = SsVector2(0, 0);//p->vector * p->speed;
		p->direction = 0;
		p->isTurnDirection = false;

		p->_rotation = 0;
		p->_rotationAdd = 0;
		p->_rotationAddDst = 0;
		p->_rotationAddOrg = 0;

		p->_rotation = 0;
	}

};
static FuncParticleElementBasic		funcBasic;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRndSeedChange : public EffectFuncBase
{
public:
	FuncParticleElementRndSeedChange(){}
	virtual ~FuncParticleElementRndSeedChange(){}

	virtual void	initalizeEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter)
	{
		ParticleElementRndSeedChange* source = static_cast<ParticleElementRndSeedChange*>(ele);
		emmiter->setMySeed(source->Seed);
	}

};
static FuncParticleElementRndSeedChange		funcRndSeedChange;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementDelay : public EffectFuncBase
{
public:
	FuncParticleElementDelay(){}
	virtual ~FuncParticleElementDelay(){}

	virtual void	initalizeEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter)
	{
		ParticleElementDelay* source = static_cast<ParticleElementDelay*>(ele);
		emmiter->delay = source->DelayTime;
		emmiter->_lifetime = emmiter->_lifetime + source->DelayTime;
		emmiter->_life = emmiter->_lifetime;
		emmiter->generate_ok = false;
	}

	virtual void	updateEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter)
	{
		ParticleElementDelay* source = static_cast<ParticleElementDelay*>(ele);
		//既定の時間までストップ？
		if (emmiter->_exsitTime >= source->DelayTime)
		{
			emmiter->generate_ok = true;
		}
	}

};
static FuncParticleElementDelay		funcDelay;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementGravity : public EffectFuncBase
{
public:
	FuncParticleElementGravity(){}
	virtual ~FuncParticleElementGravity(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementGravity* source = static_cast<ParticleElementGravity*>(ele);
		p->_gravity = source->Gravity;
	}
	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle)
	{
		ParticleElementGravity* source = static_cast<ParticleElementGravity*>(ele);
		particle->_gravity = source->Gravity * particle->_exsitTime;
	}

};
static FuncParticleElementGravity		funcGravity;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementPosition : public EffectFuncBase
{
public:
	FuncParticleElementPosition(){}
	virtual ~FuncParticleElementPosition(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementPosition* source = static_cast<ParticleElementPosition*>(ele);
		p->_position.x = p->_baseEmiterPosition.x + VarianceCalc(e, source->OffsetX.getMinValue(), source->OffsetX.getMaxValue());
		p->_position.y = p->_baseEmiterPosition.y + VarianceCalc(e, source->OffsetY.getMinValue(), source->OffsetY.getMaxValue());
	}

	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle){}

};
static FuncParticleElementPosition		funcPosition;


#if 0		//オミット
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransPosition: public EffectFuncBase
{
public:
	FuncParticleElementTransPosition(){}
	virtual ~FuncParticleElementTransPosition(){}

};
static FuncParticleElementPosition		funcTransPosition;
#endif


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRotation : public EffectFuncBase
{
public:
	FuncParticleElementRotation(){}
	virtual ~FuncParticleElementRotation(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementRotation* source = static_cast<ParticleElementRotation*>(ele);

		p->_rotation = VarianceCalc(e, source->Rotation.getMinValue(), source->Rotation.getMaxValue());
		p->_rotationAdd = VarianceCalc(e, source->RotationAdd.getMinValue(), source->RotationAdd.getMaxValue());
		p->_rotationAddDst = p->_rotationAdd;
	}

};
static FuncParticleElementRotation		funcRotation;



//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRotationTrans : public EffectFuncBase
{
public:
	FuncParticleElementRotationTrans(){}
	virtual ~FuncParticleElementRotationTrans(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementRotationTrans* source = static_cast<ParticleElementRotationTrans*>(ele);
		if (p->_lifetime == 0) return;
		if (source->EndLifeTimePer == 0)
		{
			p->_rotationAddDst = p->_rotationAdd * source->RotationFactor;
			p->_rotationAddOrg = p->_rotationAdd;
			return;
		}
		p->_rotationAddDst = p->_rotationAdd * source->RotationFactor;
		p->_rotationAddOrg = p->_rotationAdd;
	}
	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementRotationTrans* source = static_cast<ParticleElementRotationTrans*>(ele);

		if ((p->_lifetime*source->EndLifeTimePer) == 0)
		{
			p->_rotationAdd = blendFloat(p->_rotationAddOrg, p->_rotationAddDst, 1.0f);
			return;
		}
		float per = ((float)p->_exsitTime / ((float)p->_lifetime*(source->EndLifeTimePer / 100.0f)));// * 100.0f;

		if (per > 1.0f) per = 1.0f;

		p->_rotationAdd = blendFloat(p->_rotationAddOrg, p->_rotationAddDst, per);
	}

};
static FuncParticleElementRotationTrans		funcRotationTrans;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransSpeed : public EffectFuncBase
{
public:
	FuncParticleElementTransSpeed(){}
	virtual ~FuncParticleElementTransSpeed(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementTransSpeed* source = static_cast<ParticleElementTransSpeed*>(ele);
		p->lastspeed = VarianceCalc(e, source->Speed.getMinValue(), source->Speed.getMaxValue());
	}

	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		//ParticleElementTransSpeed* source = static_cast<ParticleElementTransSpeed*>(ele);
		float per = ((float)p->_exsitTime / (float)p->_lifetime);
		p->speed = (p->firstspeed + (p->lastspeed - p->firstspeed) * per);
	}

};
static FuncParticleElementTransSpeed		funcTransSpeed;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTangentialAcceleration : public EffectFuncBase
{
public:
	FuncParticleElementTangentialAcceleration(){}
	virtual ~FuncParticleElementTangentialAcceleration(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementTangentialAcceleration* source = static_cast<ParticleElementTangentialAcceleration*>(ele);
		p->_tangentialAccel = VarianceCalc(e, source->Acceleration.getMinValue(), source->Acceleration.getMaxValue());
	}

};
static FuncParticleElementTangentialAcceleration		funcTangentialAcceleration;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementInitColor : public EffectFuncBase
{
public:
	FuncParticleElementInitColor(){}
	virtual ~FuncParticleElementInitColor(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementInitColor* source = static_cast<ParticleElementInitColor*>(ele);
		VarianceCalcColor(e, p->_startcolor, source->Color.getMinValue(), source->Color.getMaxValue());
		p->_color = p->_startcolor;

	}

};
static FuncParticleElementInitColor		funcInitColor;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransColor : public EffectFuncBase
{
public:
	FuncParticleElementTransColor(){}
	virtual ~FuncParticleElementTransColor(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementTransColor* source = static_cast<ParticleElementTransColor*>(ele);
		VarianceCalcColor(e, p->_endcolor, source->Color.getMinValue(), source->Color.getMaxValue());
	}

	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		float per = ((float)p->_exsitTime / (float)p->_lifetime);

		if (per > 1.0f)per = 1.0f;

		p->_color.a = blendNumber(p->_startcolor.a, p->_endcolor.a, per);
		p->_color.r = blendNumber(p->_startcolor.r, p->_endcolor.r, per);
		p->_color.g = blendNumber(p->_startcolor.g, p->_endcolor.g, per);
		p->_color.b = blendNumber(p->_startcolor.b, p->_endcolor.b, per);
	}

};
static FuncParticleElementTransColor		funcTransColor;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementAlphaFade : public EffectFuncBase
{
public:
	FuncParticleElementAlphaFade(){}
	virtual ~FuncParticleElementAlphaFade(){}

	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle)
	{
		ParticleElementAlphaFade* source = static_cast<ParticleElementAlphaFade*>(ele);

		if (particle->_lifetime == 0) return;

		float per = ((float)particle->_exsitTime / (float)particle->_lifetime) * 100.0f;

		float start = source->disprange.getMinValue();
		float end = source->disprange.getMaxValue();

		if (per < start)
		{
			float alpha = (start - per) / start;
			particle->_color.a *= 1.0f - alpha;
			return;
		}

		if (per > end)
		{

			if (end >= 100.0f)
			{
				particle->_color.a = 0;
				return;
			}
			float alpha = (per - end) / (100.0f - end);
			particle->_color.a *= 1.0f - alpha;
			return;
		}

	}

};
static FuncParticleElementAlphaFade		funcAlphaFade;



//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementSize : public EffectFuncBase
{
public:
	FuncParticleElementSize(){}
	virtual ~FuncParticleElementSize(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{

		ParticleElementSize* source = static_cast<ParticleElementSize*>(ele);

		p->_size.x = VarianceCalc(e, source->SizeX.getMinValue(), source->SizeX.getMaxValue());
		p->_size.y = VarianceCalc(e, source->SizeY.getMinValue(), source->SizeY.getMaxValue());
		float sf = VarianceCalc(e, source->ScaleFactor.getMinValue(), source->ScaleFactor.getMaxValue());

		p->_size = p->_size * sf;
		p->_startsize = p->_size;
	}

};
static FuncParticleElementSize		funcSize;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransSize : public EffectFuncBase
{
public:
	FuncParticleElementTransSize(){}
	virtual ~FuncParticleElementTransSize(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticleElementTransSize* source = static_cast<ParticleElementTransSize*>(ele);
		SsVector2 endsize;
		endsize.x = VarianceCalc(e, source->SizeX.getMinValue(), source->SizeX.getMaxValue());
		endsize.y = VarianceCalc(e, source->SizeY.getMinValue(), source->SizeY.getMaxValue());

		float sf = VarianceCalc(e, source->ScaleFactor.getMinValue(), source->ScaleFactor.getMaxValue());

		endsize = endsize * sf;

		p->_divsize = (endsize - p->_startsize) / p->_lifetime;
	}
	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{

		p->_size = p->_startsize + (p->_divsize * (p->_exsitTime));

	}

};
static FuncParticleElementTransSize		funcTransSize;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticlePointGravity : public EffectFuncBase
{
public:
	FuncParticlePointGravity(){}
	virtual ~FuncParticlePointGravity(){}

	virtual void	initalizeEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter){}
	virtual void	updateEmmiter(SsEffectElementBase* ele, SsEffectRenderEmitter* emmiter){}
	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		//p->_orggravity = p->_gravity;	
	}
	virtual void	updateParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* p)
	{
		ParticlePointGravity* source = static_cast<ParticlePointGravity*>(ele);

		SsVector2 Target;
		Target.x = source->Position.x + p->parentEmitter->position.x;
		Target.y = source->Position.y + p->parentEmitter->position.y;

		//現在地点から指定された点に対してのベクトル*パワーを与える
		SsVector2 v2 = Target - p->_position;
		SsVector2 v2_temp = v2;

		SsVector2::normalize(v2, &v2);
		v2 = v2 * source->Power;

		p->_gravity = p->_gravity + v2;

	}

};
static FuncParticlePointGravity		funcPointGravity;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleTurnToDirectionEnabled : public EffectFuncBase
{
public:
	FuncParticleTurnToDirectionEnabled(){}
	virtual ~FuncParticleTurnToDirectionEnabled(){}

	virtual void	initializeParticle(SsEffectElementBase* ele, SsEffectRenderEmitter* e, SsEffectRenderParticle* particle)
	{
		particle->isTurnDirection = true;
	}

};
static FuncParticleTurnToDirectionEnabled		funcTurnToDirectionEnabled;


//-------------------------------------------------------------------
//挙動反映クラスの呼び出しテーブル
//SsEffectFunctionTypeの順に並べること
//-------------------------------------------------------------------
static EffectFuncBase* callTable[] =
{
	0,
	&funcBasic,
	&funcRndSeedChange,
	&funcDelay,
	&funcGravity,
	&funcPosition,
	//&funcTransPosition,
	&funcRotation,
	&funcRotationTrans,
	&funcTransSpeed,
	&funcTangentialAcceleration,
	&funcInitColor,
	&funcTransColor,
	&funcAlphaFade,
	&funcSize,
	&funcTransSize,
	&funcPointGravity,
	&funcTurnToDirectionEnabled,
};






///----------------------------------------------------------------------------------------------------
//
///----------------------------------------------------------------------------------------------------
void	SsEffectFunctionExecuter::initalize(SsEffectBehavior* beh, SsEffectRenderEmitter* emmiter)
{
	foreach(std::vector<SsEffectElementBase* >, beh->plist, e)
	{
		EffectFuncBase* cf = callTable[(*e)->myType];
		cf->initalizeEmmiter((*e), emmiter);
	}
}

void	SsEffectFunctionExecuter::updateEmmiter(SsEffectBehavior* beh, SsEffectRenderEmitter* emmiter)
{
	foreach(std::vector<SsEffectElementBase* >, beh->plist, e)
	{
		EffectFuncBase* cf = callTable[(*e)->myType];
		cf->updateEmmiter((*e), emmiter);
	}
}

void	SsEffectFunctionExecuter::initializeParticle(SsEffectBehavior* beh, SsEffectRenderEmitter* emmiter, SsEffectRenderParticle* particle)
{
	foreach(std::vector<SsEffectElementBase* >, beh->plist, e)
	{
		EffectFuncBase* cf = callTable[(*e)->myType];
		cf->initializeParticle((*e), emmiter, particle);
	}
}

void	SsEffectFunctionExecuter::updateParticle(SsEffectBehavior* beh, SsEffectRenderEmitter* emmiter, SsEffectRenderParticle* particle)
{
	foreach(std::vector<SsEffectElementBase* >, beh->plist, e)
	{
		EffectFuncBase* cf = callTable[(*e)->myType];
		cf->updateParticle((*e), emmiter, particle);
	}
}


};
















