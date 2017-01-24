#ifndef __SSEFFECTBEHAVIOR__
#define __SSEFFECTBEHAVIOR__

#include "SsEffectElement.h"
#include "../Animator/ssplayer_cellmap.h"
#include "sstypes.h"

namespace ss
{

class SsEffectElementBase;
class SsEffectRenderer;

class SsEffectBehavior
{
public:
	std::vector<SsEffectElementBase*> plist;

public:
	int								CellIndex;	//プレイヤー専用に追加
	SsCell							refCell;
	SsString						CellName;
	SsString						CellMapName;
	SsRenderBlendType::_enum		blendType;

public:
	SsEffectBehavior() : blendType(SsRenderBlendType::invalid) {}
	virtual ~SsEffectBehavior(){}

};


};



#endif
