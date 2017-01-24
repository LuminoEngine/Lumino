#include "../loader/ssloader.h"
//#include "ssplayer_animedecode.h"
#include "ssplayer_PartState.h"

namespace ss
{

SsPartState::SsPartState() : alphaBlendType(SsBlendType::invalid) {init();}

SsPartState::~SsPartState(){
	destroy();
}


void	SsPartState::destroy()
{
}

void	SsPartState::init()
{
	memset( vertices , 0 , sizeof( vertices ) );
	memset( colors , 0 , sizeof( colors ) );
	memset( uvs , 0 , sizeof( uvs ) );
	memset( matrix , 0 , sizeof( matrix ) );
	alpha = 1.0f;

}

};
