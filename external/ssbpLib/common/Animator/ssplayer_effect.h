#ifndef __SSPLAYER_EFFECT__
#define __SSPLAYER_EFFECT__

#include <list>
#include "../../SS5Player.h"
#include "../loader/ssloader.h"
#include "MersenneTwister.h"
#include "ssplayer_cellmap.h"
#include "ssplayer_PartState.h"
#include "../Helper/DebugPrint.h"

namespace ss
{

class SsEffectModel;
class SsRenderEffectBase;
class SsEffectNode;

struct SsPartState;
class SsEffectRenderAtom;

class SsEffectBehavior;
class SsEffectRenderer;
class CustomSprite;

#define PFMEM_TEST ( 1 )

namespace SsRenderType
{
	enum _enum{
		BaseNode,
		EmmiterNode,
		ParticleNode,
	};
};


namespace EmmiterType
{
	enum _enum{
		EmmiterTypeNormal,
		EmmiterTypeRibbon,
	};
};



//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
class SsEffectRenderAtom
{
public:
	SsVector3  			position;
	float				rotation;
	SsVector2	  		scale;

	SsEffectRenderAtom*	parent;
	SsEffectNode*		data;
    bool				m_isLive;
	bool				m_isInit;
    bool				m_isCreateChild;

	float				_lifetime;		//オブジェクトの最大生存時間
	float				_exsitTime;		//存在した時間
	float				_life;          //寿命 = 0で死

	bool                undead;

	float				alpha;

public:

	SsEffectRenderAtom() :
		parent(0),
		m_isInit(false),
		m_isLive(true),
		_lifetime(10.0f),
		_life(1.0f),
		rotation(0),
		position(0,0,0),
		scale(1.0f,1.0f),
		m_isCreateChild(false)
		{
		}

	SsEffectRenderAtom( SsEffectNode* refdata , SsEffectRenderAtom* _p){
        data = refdata;
		setParent(_p);

		_lifetime = 0;
		position = SsVector3(0,0,0);
		scale = SsVector2(0,0);
		rotation = 0.0f;
	}

	virtual ~SsEffectRenderAtom(){}

	void	setParent( SsEffectRenderAtom* _p ){ parent = _p; }
    SsRenderType::_enum	getMyType(){ return SsRenderType::BaseNode;}

	bool	   	isInit(){ return m_isInit; }

	virtual void	Initialize()
	{
		parent = 0;
		m_isInit = false;
		m_isLive = true;
		_lifetime = 10.0f;
		_life = 1.0f;
		rotation = 0;
		position = SsVector3(0,0,0);
		scale = SsVector2(1,1);
		m_isCreateChild = false;
		m_isInit = false;
	}
	virtual bool	genarate( SsEffectRenderer* render ){return true;}


    virtual void	update(float delta){}
	virtual void	draw(SsEffectRenderer* render){}

	virtual void	debugdraw(){}

	SsVector3	getPosition() const
	{
		return position;
	}

	void	setPosistion( float x , float y , float z )
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}

	void	setScale( float x , float y )
	{
		scale.x = x;
		scale.y = y;
	}
	void	setRotation( float z  )
	{
		rotation = fmod( z , 360 ) ;
	}

	float		getRotation() const { return rotation; }
	SsVector2	getScale() const { return scale; }
 	virtual void	count(){}

};

class SsEffectDrawBatch;
class SsEffectDrawBatch
{
public:
	int	priority;
	SsCellValue*			dispCell;
	SsEffectNode*			targetNode;

	SsRenderBlendType::_enum       blendType;


	std::list<SsEffectRenderAtom*> drawlist;


	SsEffectDrawBatch() : priority(0), dispCell(0), targetNode(0) {}
	~SsEffectDrawBatch(){}

	void	drawSetting();


};


//--------------------------------------------------------------------------
//パーティクル生成能力を持つオブジェクト
//--------------------------------------------------------------------------
class  SsEffectRenderEmitter : public SsEffectRenderAtom
{
public:
	unsigned int myseed;
	SsCellValue				dispCell;
	//エミッターパラメータ

	//パーティクルパラメータ
    SsEffectNode*		param_particle;

	CMersenneTwister*	     MT;

	//以前からの移植
	int				maxParticle;    //
	int				delay;
	float				interval;
	float				intervalleft;
	float   		frame;
	float			frameDelta;
    int				burst;

	EmmiterType::_enum		type;

	SsString		MyName;
    size_t 			particleCount;


    bool			generate_ok;
	int				drawPriority;


public:

	SsEffectDrawBatch*  myBatchList;


public:
	void	InitParameter()
	{
		if (MT == 0) MT = new CMersenneTwister();
		SsEffectRenderAtom::Initialize();
		delay = 0;
		interval = 0;
		intervalleft = 0;
		frame = 0;
		frameDelta = 0;
		particleCount = 0;
		_exsitTime = 0;

		generate_ok = true;

		param_particle = 0;
		type = EmmiterType::EmmiterTypeNormal;

	}

	SsEffectRenderEmitter() : MT(0){}
	SsEffectRenderEmitter(SsEffectNode* refdata, SsEffectRenderAtom* _p){
		data = refdata;
		parent = _p;
		InitParameter();
	}

	virtual ~SsEffectRenderEmitter()
	{
		if (MT)
		{
			delete MT;
		}
	}
	SsRenderType::_enum		getMyType(){ return SsRenderType::EmmiterNode; }
	void			setMySeed( unsigned int seed );
	void			TrushRandom(int loop)
	{
		for ( int i = 0 ; i < loop ; i++ )
			MT->genrand_uint32();
	}

	virtual void	Initialize();
	virtual bool	genarate( SsEffectRenderer* render );

	virtual void	update(float delta);
	virtual void	count(){ particleCount = 0 ; }

};





//--------------------------------------------------------------------------
//パーティクルオブジェクト
//--------------------------------------------------------------------------
class  SsEffectRenderParticle : public SsEffectRenderAtom
{
public:
	SsCellValue*			dispCell;

	float   				size;
	SsEffectRenderEmitter*  parentEmitter;
    SsEffectBehavior*		refBehavior;

	SsVector2   _baseEmiterPosition;   //もしかしてもう使ってないかも
	SsVector2   _backposition;         //force計算前のポジション
	SsVector2   _position;             //描画用ポジション

	float		_rotation;
	float		_rotationAdd;
	float		_rotationAddDst;
	float		_rotationAddOrg;

	SsVector2   _size;
	SsVector2  	_startsize;
	SsVector2   _divsize;

	SsU8Color	_color;
	SsU8Color	_startcolor;
	SsU8Color	_endcolor;


	float		speed;		//現在持っている速度
	float		firstspeed;
    float		lastspeed;
	SsVector2   vector;

	SsVector2   _force;
	SsVector2   _gravity;
//	SsVector2   _orggravity;

	float 		_radialAccel;
	float 		_tangentialAccel;
	float		direction;
	bool		isTurnDirection;

	SsVector2   _execforce;				//処理中の力 最終的には単位当たりの力に変換




public:
	void	InitParameter()
	{

		SsEffectRenderAtom::Initialize();

		_position = SsVector2(0,0);
		_baseEmiterPosition = SsVector2(0,0);
		_backposition = SsVector2(0,0);
		_rotation = 0;
		_size = SsVector2( 1.0f , 1.0f );
		_startsize = SsVector2( 1.0f , 1.0f );
		_divsize = SsVector2( 0.0f , 0.0f );
		_force = SsVector2(0,0);
		_gravity = SsVector2(0,0);
		_radialAccel = 0;
		_tangentialAccel = 0;
		_color = SsU8Color(255,255,255,255) ;
        _startcolor = _color;
        _exsitTime = 0;
		_execforce = SsVector2(0,0);
		parentEmitter = 0;
		dispCell = 0;
	}



	SsEffectRenderParticle(): parentEmitter(0){}
	SsEffectRenderParticle( SsEffectNode* refdata , SsEffectRenderAtom* _p){
		data = refdata;
		parent = _p;
        InitParameter();
	}

	virtual ~SsEffectRenderParticle(){}
    SsRenderType::_enum	getMyType(){ return SsRenderType::ParticleNode;}

	//生成フェーズ
	virtual void	Initialize();

	virtual bool	genarate( SsEffectRenderer* render );

    virtual void	update(float delta);
	virtual void	draw(SsEffectRenderer* render);


	virtual void	count()
	{
		if ( parentEmitter )
			parentEmitter->particleCount++;
	}
	virtual void	updateDelta(float delta);
	virtual void 	updateForce(float delta);
};



//----------------------------------------------------------------
// 画面に存在できるパーティクルバッファのサイズを指定します。
// 画面に存在できるエミッタバッファのサイズを指定します。
//
// １つのパーツに対してこれらのバッファが生成されます。
// パーツ単位で画面に生成できる最大数になります。
//
// パーティクルにエミッターを付加した場合に複数のエミッターが同時に作成され
// エミッターバッファが消費されます。
//
// アプリケーションの仕様に合わせて数を調整してください。
// パーツ単位でバッファを作成するので、バッファ数を多く設定すると
// アニメーション再生時にバッファ確保の時間が長くなります。
//----------------------------------------------------------------
#define SSEFFECTRENDER_EMMITER_MAX (128)		//エミッターバッファ数
#define SSEFFECTRENDER_PARTICLE_MAX (512)	//パーティクルバッファ数
#define SSEFFECTRENDER_BACTH_MAX (16)		//ノード階層最大値（固定）




//--------------------------------------------------------------------------
//エフェクトの描画処理メイン
//--------------------------------------------------------------------------
class SsEffectRenderer
{
private:
	SsEffectModel*		effectData;


	bool			m_isPlay;
	bool			m_isPause;
	bool			m_isLoop;
	u32				mySeed;


	SsVector3		layoutPosition;

//	SsCellMapList*	curCellMapManager;/// セルマップのリスト（アニメデコーダーからもらう


#if PFMEM_TEST
	SsEffectRenderEmitter    em_pool[SSEFFECTRENDER_EMMITER_MAX + 1];
	SsEffectRenderParticle   pa_pool[SSEFFECTRENDER_PARTICLE_MAX + 1];

	SsEffectDrawBatch		 drawPr_pool[SSEFFECTRENDER_BACTH_MAX + 1];


	int						em_pool_count;
	int						pa_pool_count;
	int						dpr_pool_count;
#endif

public:
	//アップデート物のリスト
	SsEffectRenderAtom* render_root;

	bool			usePreMultiTexture;
	u32				parentAnimeStartFrame;
	bool			renderTexture;
	float           frameDelta;
	SsPartState*		parentState;


 	std::vector<SsEffectRenderAtom*> updatelist;
	std::vector<SsEffectRenderAtom*> createlist;

    std::list<SsEffectDrawBatch*>  drawBatchList;

	//cocos2d-x用エフェクトスプライト
	bool _isContentScaleFactorAuto;
	CustomSprite						*_parentSprite;

public:
	SsEffectRenderer() : effectData(0) , parentState(0) ,mySeed(0) , render_root(0),parentAnimeStartFrame(0) , m_isLoop(false)
#if PFMEM_TEST
	,em_pool_count(0)
	,pa_pool_count(0)
	,dpr_pool_count(0)
	,usePreMultiTexture(true)
	,renderTexture(false)
	,frameDelta(0)
	,_isContentScaleFactorAuto(true)
	,_parentSprite(0)
#endif
	{}


	virtual ~SsEffectRenderer();

    void	clearUpdateList();


public:


	void	setSeed( u32 seed ){  mySeed = seed; }
	virtual void	update(float delta);
    virtual void	draw();
	virtual void    reload();

    //操作
	void    play();
	void	stop();
	void    pause();
	void	setLoop(bool flag);
	bool	getPlayStatus(void);	//追加

	int	getCurrentFPS(){
		if (effectData)
		{
        	if ( effectData->fps == 0 ) return 30;

        	return effectData->fps;
		}
		return 30;
	}

	SsEffectModel* getEffectData()
	{
		return effectData;
	}

	//データセット
	void	setEffectData(SsEffectModel* data){
					stop();
                    clearUpdateList();
					effectData = data;
			}
	void	setParentAnimeState( SsPartState* state ){ parentState = state; }

	SsEffectRenderAtom* CreateAtom( unsigned int seed , SsEffectRenderAtom* parent , SsEffectNode* node );


	//cocos側のエフェクトスプライトを設定する
	void setContentScaleEneble(bool eneble){ _isContentScaleFactorAuto = eneble; }
	void setParentSprite(CustomSprite* sprite){ _parentSprite = sprite; }

};



};
#endif