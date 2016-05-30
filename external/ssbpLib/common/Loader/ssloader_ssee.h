#ifndef __SSLOADER_SSEE__
#define __SSLOADER_SSEE__

#include "sstypes.h"

#include "SsEffectBehavior.h"

namespace ss
{


class SimpleTree
{
public:
	SimpleTree *parent;
	SimpleTree *ctop;
	SimpleTree *prev;
	SimpleTree *next;

public:
	SimpleTree()
		: parent(0), ctop(0), prev(0), next(0)
	{}
	virtual ~SimpleTree()
	{
		destroy();
	}

	void	addChildEnd(SimpleTree* c){
		if (ctop == 0)
		{
			ctop = c;
		}
		else{
			ctop->addSiblingEnd(c);
		}
		c->parent = this;
	}
	void	addSiblingEnd(SimpleTree* c)
	{
		if (next == 0)
		{
			c->prev = this;
			next = c;
		}
		else{
			next->addSiblingEnd(c);
		}

		c->parent = this->parent;
	}

	void destroysub(SimpleTree *t)
	{
		if (t->ctop)
		{
			destroysub(t->ctop);
		}
		if (t->next)
		{
			destroysub(t->next);
		}

		t->ctop = 0;
		t->next = 0;
		t->prev = 0;
		delete t;
	}
	void destroy()
	{
		if (this->ctop)
			destroysub(this->ctop);
	}
};






class SsEffectNode : public SimpleTree
{
public:
	int						arrayIndex;
	int						parentIndex;
	SsEffectNodeType::_enum	type;
	bool					visible;

	SsEffectBehavior		behavior;

public:
	SsEffectNode() :
		arrayIndex(0), parentIndex(0),
		type(SsEffectNodeType::invalid)
	{}
	~SsEffectNode(){}

	SsEffectNodeType::_enum	GetType(){ return type; }

	SsEffectBehavior*	GetMyBehavior(){ return &behavior; }

};


class SsEffectModel
{
private:

public:
	SsEffectNode* root;
	std::vector<SsEffectNode*> nodeList;
	int			lockRandSeed; 	 // ランダムシード固定値
	bool    	isLockRandSeed;  // ランダムシードを固定するか否か
	int			fps;             //
	SsString	   bgcolor;
	SsString			effectName;

public:
	SsEffectModel() : root(0)
	{}

	virtual ~SsEffectModel(){
		if (root)
		{
			delete root;
		}
		root = 0;
	}

	//アクセス
	SsEffectNode* GetRoot(){ return root; }

};


class SsEffectFile
{
public:
	SsEffectModel	   effectData;  //親子構造＋各アトリビュート
	SsString		   name;

	SsEffectFile(){}
	virtual ~SsEffectFile(){}
};



};

#endif
