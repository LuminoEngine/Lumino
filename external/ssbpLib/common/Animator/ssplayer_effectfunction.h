#ifndef __SSPLAYER_EFFECTFUNCTION__
#define __SSPLAYER_EFFECTFUNCTION__

#include "ssplayer_effect.h"

namespace ss
{

class	SsEffectFunctionExecuter
{
public:
	SsEffectFunctionExecuter(){}
	virtual ~SsEffectFunctionExecuter(){}

	static void	initalize( SsEffectBehavior* beh , SsEffectRenderEmitter* emmiter);
	static void	updateEmmiter( SsEffectBehavior* beh , SsEffectRenderEmitter* emmiter);
	static void	initializeParticle( SsEffectBehavior* beh , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	static void	updateParticle( SsEffectBehavior* beh , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
};


};



#endif
