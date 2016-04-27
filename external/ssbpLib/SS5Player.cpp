// 
//  SS5Player.cpp
//
#include "SS5Player.h"
#include "SS5PlayerData.h"
#include "SS5PlayerTypes.h"
#include "common/Animator/ssplayer_matrix.h"


namespace ss
{

/**
 * definition
 */

static const ss_u32 DATA_ID = 0x42505353;
static const ss_u32 DATA_VERSION = 3;


/**
 * utilites
 */
static void splitPath(std::string& directoty, std::string& filename, const std::string& path)
{
    std::string f = path;
    std::string d = "";

    size_t pos = path.find_last_of("/");
	if (pos == std::string::npos) pos = path.find_last_of("\\");	// for win

    if (pos != std::string::npos)
    {
        d = path.substr(0, pos+1);
        f = path.substr(pos+1);
    }

	directoty = d;
	filename = f;
}

// printf 形式のフォーマット
static std::string Format(const char* format, ...){

	static std::vector<char> tmp(1000);

	va_list args, source;
	va_start(args, format);
	va_copy( source , args );

	while (1)
	{
		va_copy( args , source );
#ifdef _WIN32		//Windows
		if (_vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
		{
			tmp.resize(tmp.size() * 2);
		}
		else
		{
			break;
		}
#else
        if (vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
        {
            tmp.resize(tmp.size() * 2);
        }
        else
        {
            break;
        }

#endif
	}
	tmp.push_back('\0');
	std::string ret = &(tmp[0]);
	va_end(args);
	return ret;
}

//座標回転処理
//指定した座標を中心に回転後した座標を取得します
void get_uv_rotation(float *u, float *v, float cu, float cv, float deg)
{
	float dx = *u - cu; // 中心からの距離(X)
	float dy = *v - cv; // 中心からの距離(Y)

	float tmpX = (dx * cosf(SSRadianToDegree(deg))) - (dy * sinf(SSRadianToDegree(deg))); // 回転
	float tmpY = (dx * sinf(SSRadianToDegree(deg))) + (dy * cosf(SSRadianToDegree(deg)));

	*u = (cu + tmpX); // 元の座標にオフセットする
	*v = (cv + tmpY);

}

//乱数シードに利用するユニークIDを作成します。
//この値は全てのSS5プレイヤー共通で使用します
int seedMakeID = 123456;
//エフェクトに与えるシードを取得する関数
unsigned int getRandomSeed()
{
	seedMakeID++;	//ユニークIDを更新します。
	//時間＋ユニークIDにする事で毎回シードが変わるようにします。
	unsigned int rc = (unsigned int)time(0) + (seedMakeID);

	return(rc);
}



/**
 * ToPointer
 */
class ToPointer
{
public:
	explicit ToPointer(const void* base)
		: _base(static_cast<const char*>(base)) {}
	
	const void* operator()(ss_offset offset) const
	{
		return (_base + offset);
	}

private:
	const char*	_base;
};


/**
 * DataArrayReader
 */
class DataArrayReader
{
public:
	DataArrayReader(const ss_u16* dataPtr)
		: _dataPtr(dataPtr)
	{}

	ss_u16 readU16() { return *_dataPtr++; }
	ss_s16 readS16() { return static_cast<ss_s16>(*_dataPtr++); }

	unsigned int readU32()
	{
		unsigned int l = readU16();
		unsigned int u = readU16();
		return static_cast<unsigned int>((u << 16) | l);
	}

	int readS32()
	{
		return static_cast<int>(readU32());
	}

	float readFloat()
	{
		union {
			float			f;
			unsigned int	i;
		} c;
		c.i = readU32();
		return c.f;
	}
	
	void readColor(SSColor4B& color)
	{
		unsigned int raw = readU32();
		color.a = static_cast<unsigned char>(raw >> 24);
		color.r = static_cast<unsigned char>(raw >> 16);
		color.g = static_cast<unsigned char>(raw >> 8);
		color.b = static_cast<unsigned char>(raw);
	}
	
	ss_offset readOffset()
	{
		return static_cast<ss_offset>(readS32());
	}

private:
	const ss_u16*	_dataPtr;
};


/**
 * CellRef
 */
struct CellRef
{
	const Cell* cell;
	TextuerData texture;
	SSRect rect;
	std::string texname;
};


/**
 * CellCache
 */
class CellCache
{
public:
	CellCache()
	{
	}
	~CellCache()
	{
		releseReference();
	}

	static CellCache* create(const ProjectData* data, const std::string& imageBaseDir)
	{
		CellCache* obj = new CellCache();
		if (obj)
		{
			obj->init(data, imageBaseDir);
		}
		return obj;
	}

	CellRef* getReference(int index)
	{
		if (index < 0 || index >= (int)_refs.size())
		{
			SSLOGERROR("Index out of range > %d", index);
			SS_ASSERT(0);
		}
		CellRef* ref = _refs.at(index);
		return ref;
	}

	//指定した名前のセルの参照テクスチャを変更する
	bool setCellRefTexture(const ProjectData* data, const char* cellName, long texture)
	{
		bool rc = false;

		ToPointer ptr(data);
		const Cell* cells = static_cast<const Cell*>(ptr(data->cells));

		//名前からインデックスの取得
		int cellindex = -1;
		for (int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
			const char* name = static_cast<const char*>(ptr(cellMap->name));
			if (strcmp(cellName, name) == 0)
			{
				CellRef* ref = getReference(i);
				ref->texture.handle = texture;
				rc = true;
			}
		}

		return(rc);
	}

	//指定したデータのテクスチャを破棄する
	bool releseTexture(const ProjectData* data)
	{
		bool rc = false;

		ToPointer ptr(data);
		const Cell* cells = static_cast<const Cell*>(ptr(data->cells));
		for (int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
			{
				CellRef* ref = _refs.at(i);
				if (ref->texture.handle != -1 )
				{
					SSTextureRelese(ref->texture.handle);
					ref->texture.handle = -1;
					rc = true;
				}
			}
		}
		return(rc);
	}

protected:
	void init(const ProjectData* data, const std::string& imageBaseDir)
	{

		SS_ASSERT2(data != NULL, "Invalid data");
		
		_textures.clear();
		_refs.clear();
		_texname.clear();

		ToPointer ptr(data);
		const Cell* cells = static_cast<const Cell*>(ptr(data->cells));

		for (int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
			
			if (cellMap->index >= (int)_textures.size())
			{
				const char* imagePath = static_cast<const char*>(ptr(cellMap->imagePath));
				addTexture(imagePath, imageBaseDir, (SsTexWrapMode::_enum)cellMap->wrapmode, (SsTexFilterMode::_enum)cellMap->filtermode);
			}

			//セル情報だけ入れておく
			//テクスチャの読み込みはゲーム側に任せる
			CellRef* ref = new CellRef();
			ref->cell = cell;
			ref->texture = _textures.at(cellMap->index);
			ref->texname = _texname.at(cellMap->index);
			ref->rect = SSRect(cell->x, cell->y, cell->width, cell->height);
			_refs.push_back(ref);
		}

	}
	//キャッシュの削除
	void releseReference(void)
	{
		for (int i = 0; i < _refs.size(); i++)
		{
			CellRef* ref = _refs.at(i);
			if (ref->texture.handle != -1 )
			{
				SSTextureRelese(ref->texture.handle);
				ref->texture.handle = -1;
			}
			delete ref;
		}
		_refs.clear();
	}

	void addTexture(const std::string& imagePath, const std::string& imageBaseDir, SsTexWrapMode::_enum  wrapmode, SsTexFilterMode::_enum filtermode)
	{
		std::string path = "";
		
		if (isAbsolutePath(imagePath))
		{
			// 絶対パスのときはそのまま扱う
			path = imagePath;
		}
		else
		{
			// 相対パスのときはimageBaseDirを付与する
			path.append(imageBaseDir);
			size_t pathLen = path.length();
			if (pathLen && path.at(pathLen-1) != '/' && path.at(pathLen-1) != '\\')
			{
				path.append("/");
			}
			path.append(imagePath);
		}

		//テクスチャの読み込み
		long tex = SSTextureLoad(path.c_str(), wrapmode, filtermode);
		SSLOG("load: %s", path.c_str());
		TextuerData texdata;
		texdata.handle = tex;
		int w;
		int h;
		SSGetTextureSize(texdata.handle, w, h);
		texdata.size_w = w;
		texdata.size_h = h;

		_textures.push_back(texdata);
		_texname.push_back(path);

	}

protected:
	std::vector<std::string>			_texname;
	std::vector<TextuerData>			_textures;
	std::vector<CellRef*>				_refs;
};


/**
* EffectCache
*/
class EffectCache
{
public:
	EffectCache()
	{
	}
	~EffectCache()
	{
		releseReference();
	}

	static EffectCache* create(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache)
	{
		EffectCache* obj = new EffectCache();
		if (obj)
		{
			obj->init(data, imageBaseDir, cellCache);
			//			obj->autorelease();
		}
		return obj;
	}

	/**
	* エフェクトファイル名を指定してEffectRefを得る
	*/
	SsEffectModel* getReference(const std::string& name)
	{
		SsEffectModel* ref = _dic.at(name);
		return ref;
	}

	void dump()
	{
		std::map<std::string, SsEffectModel*>::iterator it = _dic.begin();
		while (it != _dic.end())
		{
			SSLOG("%s", (*it).second);
			++it;
		}
	}
protected:
	void init(const ProjectData* data, const std::string& imageBaseDir, CellCache* cellCache)
	{
		SS_ASSERT2(data != NULL, "Invalid data");

		ToPointer ptr(data);

		//ssbpからエフェクトファイル配列を取得
		const EffectFile* effectFileArray = static_cast<const EffectFile*>(ptr(data->effectFileList));

		for (int listindex = 0; listindex < data->numEffectFileList; listindex++)
		{
			//エフェクトファイル配列からエフェクトファイルを取得
			const EffectFile* effectFile = &effectFileArray[listindex];

			//保持用のエフェクトファイル情報を作成
			SsEffectModel *effectmodel = new SsEffectModel();
			std::string effectFileName = static_cast<const char*>(ptr(effectFile->name));

			//エフェクトファイルからエフェクトノード配列を取得
			const EffectNode* effectNodeArray = static_cast<const EffectNode*>(ptr(effectFile->effectNode));
			for (int nodeindex = 0; nodeindex < effectFile->numNodeList; nodeindex++)
			{
				const EffectNode* effectNode = &effectNodeArray[nodeindex];		//エフェクトノード配列からエフェクトノードを取得

				SsEffectNode *node = new SsEffectNode();
				node->arrayIndex = effectNode->arrayIndex;
				node->parentIndex = effectNode->parentIndex;
				node->type = (SsEffectNodeType::_enum)effectNode->type;
				node->visible = true;

				SsEffectBehavior behavior;
				//セル情報を作成
				behavior.CellIndex = effectNode->cellIndex;
				CellRef* cellRef = behavior.CellIndex >= 0 ? cellCache->getReference(behavior.CellIndex) : NULL;
				if (cellRef)
				{
					behavior.refCell.pivot_X = cellRef->cell->pivot_X;
					behavior.refCell.pivot_Y = cellRef->cell->pivot_Y;
					behavior.refCell.texture = cellRef->texture;
					behavior.refCell.texname = cellRef->texname;
					behavior.refCell.rect = cellRef->rect;
					behavior.refCell.cellIndex = behavior.CellIndex;
					std::string name = static_cast<const char*>(ptr(cellRef->cell->name));
					behavior.refCell.cellName = name;

				}
				//				behavior.CellName;
				//				behavior.CellMapName;
				behavior.blendType = (SsRenderBlendType::_enum)effectNode->blendType;

				//エフェクトノードからビヘイビア配列を取得
				const ss_offset* behaviorArray = static_cast<const ss_offset*>(ptr(effectNode->Behavior));
				for (int behaviorindex = 0; behaviorindex < effectNode->numBehavior; behaviorindex++)
				{
					//ビヘイビア配列からビヘイビアを取得
					const ss_u16* behavior_adr = static_cast<const ss_u16*>(ptr(behaviorArray[behaviorindex]));
					DataArrayReader reader(behavior_adr);

					//パラメータを作ってpush_backで登録していく
					int type = reader.readS32();
					switch (type)
					{
					case SsEffectFunctionType::Basic:
					{
						//基本情報
						EffectParticleElementBasic readparam;
						readparam.priority = reader.readU32();			//表示優先度
						readparam.maximumParticle = reader.readU32();		//最大パーティクル数
						readparam.attimeCreate = reader.readU32();		//一度に作成するパーティクル数
						readparam.interval = reader.readU32();			//生成間隔
						readparam.lifetime = reader.readU32();			//エミッター生存時間
						readparam.speedMinValue = reader.readFloat();		//初速最小
						readparam.speedMaxValue = reader.readFloat();		//初速最大
						readparam.lifespanMinValue = reader.readU32();	//パーティクル生存時間最小
						readparam.lifespanMaxValue = reader.readU32();	//パーティクル生存時間最大
						readparam.angle = reader.readFloat();				//射出方向
						readparam.angleVariance = reader.readFloat();		//射出方向範囲

						ParticleElementBasic *effectParam = new ParticleElementBasic();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->priority = readparam.priority;							//表示優先度
						effectParam->maximumParticle = readparam.maximumParticle;			//最大パーティクル数
						effectParam->attimeCreate = readparam.attimeCreate;					//一度に作成するパーティクル数
						effectParam->interval = readparam.interval;							//生成間隔
						effectParam->lifetime = readparam.lifetime;							//エミッター生存時間
						effectParam->speed.setMinMax(readparam.speedMinValue, readparam.speedMaxValue);				//初速
						effectParam->lifespan.setMinMax(readparam.lifespanMinValue, readparam.lifespanMaxValue);	//パーティクル生存時間
						effectParam->angle = readparam.angle;								//射出方向
						effectParam->angleVariance = readparam.angleVariance;				//射出方向範囲

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::RndSeedChange:
					{
						//シード上書き
						EffectParticleElementRndSeedChange readparam;
						readparam.Seed = reader.readU32();				//上書きするシード値

						ParticleElementRndSeedChange *effectParam = new ParticleElementRndSeedChange();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Seed = readparam.Seed;							//上書きするシード値

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::Delay:
					{
						//発生：タイミング
						EffectParticleElementDelay readparam;
						readparam.DelayTime = reader.readU32();			//遅延時間

						ParticleElementDelay *effectParam = new ParticleElementDelay();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->DelayTime = readparam.DelayTime;			//遅延時間

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::Gravity:
					{
						//重力を加える
						EffectParticleElementGravity readparam;
						readparam.Gravity_x = reader.readFloat();			//X方向の重力
						readparam.Gravity_y = reader.readFloat();			//Y方向の重力

						ParticleElementGravity *effectParam = new ParticleElementGravity();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Gravity.x = readparam.Gravity_x;			//X方向の重力
						effectParam->Gravity.y = readparam.Gravity_y;			//Y方向の重力

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::Position:
					{
						//座標：生成時
						EffectParticleElementPosition readparam;
						readparam.OffsetXMinValue = reader.readFloat();	//X座標に加算最小
						readparam.OffsetXMaxValue = reader.readFloat();	//X座標に加算最大
						readparam.OffsetYMinValue = reader.readFloat();	//X座標に加算最小
						readparam.OffsetYMaxValue = reader.readFloat();	//X座標に加算最大

						ParticleElementPosition *effectParam = new ParticleElementPosition();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->OffsetX.setMinMax(readparam.OffsetXMinValue, readparam.OffsetXMaxValue); 	//X座標に加算最小
						effectParam->OffsetY.setMinMax(readparam.OffsetYMinValue, readparam.OffsetYMaxValue);	//X座標に加算最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::Rotation:
					{
						//Z回転を追加
						EffectParticleElementRotation readparam;
						readparam.RotationMinValue = reader.readFloat();		//角度初期値最小
						readparam.RotationMaxValue = reader.readFloat();		//角度初期値最大
						readparam.RotationAddMinValue = reader.readFloat();	//角度初期加算値最小
						readparam.RotationAddMaxValue = reader.readFloat();	//角度初期加算値最大

						ParticleElementRotation *effectParam = new ParticleElementRotation();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Rotation.setMinMax(readparam.RotationMinValue, readparam.RotationMaxValue);		//角度初期値最小
						effectParam->RotationAdd.setMinMax(readparam.RotationAddMinValue, readparam.RotationAddMaxValue);	//角度初期加算値最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TransRotation:
					{
						//Z回転速度変更
						EffectParticleElementRotationTrans readparam;
						readparam.RotationFactor = reader.readFloat();		//角度目標加算値
						readparam.EndLifeTimePer = reader.readFloat();		//到達時間

						ParticleElementRotationTrans *effectParam = new ParticleElementRotationTrans();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->RotationFactor = readparam.RotationFactor;		//角度目標加算値
						effectParam->EndLifeTimePer = readparam.EndLifeTimePer;		//到達時間

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TransSpeed:
					{
						//速度：変化
						EffectParticleElementTransSpeed readparam;
						readparam.SpeedMinValue = reader.readFloat();			//速度目標値最小
						readparam.SpeedMaxValue = reader.readFloat();			//速度目標値最大

						ParticleElementTransSpeed *effectParam = new ParticleElementTransSpeed();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Speed.setMinMax(readparam.SpeedMinValue, readparam.SpeedMaxValue);			//速度目標値最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TangentialAcceleration:
					{
						//接線加速度
						EffectParticleElementTangentialAcceleration readparam;
						readparam.AccelerationMinValue = reader.readFloat();	//設定加速度最小
						readparam.AccelerationMaxValue = reader.readFloat();	//設定加速度最大

						ParticleElementTangentialAcceleration *effectParam = new ParticleElementTangentialAcceleration();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Acceleration.setMinMax(readparam.AccelerationMinValue, readparam.AccelerationMaxValue);	//設定加速度最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::InitColor:
					{
						//カラーRGBA：生成時
						EffectParticleElementInitColor readparam;
						readparam.ColorMinValue = reader.readU32();			//設定カラー最小
						readparam.ColorMaxValue = reader.readU32();			//設定カラー最大

						ParticleElementInitColor *effectParam = new ParticleElementInitColor();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

						int a = (readparam.ColorMinValue & 0xFF000000) >> 24;
						int r = (readparam.ColorMinValue & 0x00FF0000) >> 16;
						int g = (readparam.ColorMinValue & 0x0000FF00) >> 8;
						int b = (readparam.ColorMinValue & 0x000000FF) >> 0;
						SsU8Color mincol(r, g, b, a);
						a = (readparam.ColorMaxValue & 0xFF000000) >> 24;
						r = (readparam.ColorMaxValue & 0x00FF0000) >> 16;
						g = (readparam.ColorMaxValue & 0x0000FF00) >> 8;
						b = (readparam.ColorMaxValue & 0x000000FF) >> 0;
						SsU8Color maxcol(r, g, b, a);
						effectParam->Color.setMinMax(mincol, maxcol);			//設定カラー最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TransColor:
					{
						//カラーRGB：変化
						EffectParticleElementTransColor readparam;
						readparam.ColorMinValue = reader.readU32();			//設定カラー最小
						readparam.ColorMaxValue = reader.readU32();			//設定カラー最大

						ParticleElementTransColor *effectParam = new ParticleElementTransColor();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

						int a = (readparam.ColorMinValue & 0xFF000000) >> 24;
						int r = (readparam.ColorMinValue & 0x00FF0000) >> 16;
						int g = (readparam.ColorMinValue & 0x0000FF00) >> 8;
						int b = (readparam.ColorMinValue & 0x000000FF) >> 0;
						SsU8Color mincol(r, g, b, a);
						a = (readparam.ColorMaxValue & 0xFF000000) >> 24;
						r = (readparam.ColorMaxValue & 0x00FF0000) >> 16;
						g = (readparam.ColorMaxValue & 0x0000FF00) >> 8;
						b = (readparam.ColorMaxValue & 0x000000FF) >> 0;
						SsU8Color maxcol(r, g, b, a);
						effectParam->Color.setMinMax(mincol, maxcol);			//設定カラー最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::AlphaFade:
					{
						//フェード
						EffectParticleElementAlphaFade readparam;
						readparam.disprangeMinValue = reader.readFloat();		//表示区間開始
						readparam.disprangeMaxValue = reader.readFloat();		//表示区間終了

						ParticleElementAlphaFade *effectParam = new ParticleElementAlphaFade();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->disprange.setMinMax(readparam.disprangeMinValue, readparam.disprangeMaxValue);		//表示区間開始

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::Size:
					{
						//スケール：生成時
						EffectParticleElementSize readparam;
						readparam.SizeXMinValue = reader.readFloat();			//幅倍率最小
						readparam.SizeXMaxValue = reader.readFloat();			//幅倍率最大
						readparam.SizeYMinValue = reader.readFloat();			//高さ倍率最小
						readparam.SizeYMaxValue = reader.readFloat();			//高さ倍率最大
						readparam.ScaleFactorMinValue = reader.readFloat();		//倍率最小
						readparam.ScaleFactorMaxValue = reader.readFloat();		//倍率最大

						ParticleElementSize *effectParam = new ParticleElementSize();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->SizeX.setMinMax(readparam.SizeXMinValue, readparam.SizeXMaxValue);			//幅倍率最小
						effectParam->SizeY.setMinMax(readparam.SizeYMinValue, readparam.SizeYMaxValue);			//高さ倍率最小
						effectParam->ScaleFactor.setMinMax(readparam.ScaleFactorMinValue, readparam.ScaleFactorMaxValue);		//倍率最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TransSize:
					{
						//スケール：変化
						EffectParticleElementTransSize readparam;
						readparam.SizeXMinValue = reader.readFloat();			//幅倍率最小
						readparam.SizeXMaxValue = reader.readFloat();			//幅倍率最大
						readparam.SizeYMinValue = reader.readFloat();			//高さ倍率最小
						readparam.SizeYMaxValue = reader.readFloat();			//高さ倍率最大
						readparam.ScaleFactorMinValue = reader.readFloat();		//倍率最小
						readparam.ScaleFactorMaxValue = reader.readFloat();		//倍率最大

						ParticleElementTransSize *effectParam = new ParticleElementTransSize();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->SizeX.setMinMax(readparam.SizeXMinValue, readparam.SizeXMaxValue);			//幅倍率最小
						effectParam->SizeY.setMinMax(readparam.SizeYMinValue, readparam.SizeYMaxValue);			//高さ倍率最小
						effectParam->ScaleFactor.setMinMax(readparam.ScaleFactorMinValue, readparam.ScaleFactorMaxValue);		//倍率最小

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::PointGravity:
					{
						//重力点の追加
						EffectParticlePointGravity readparam;
						readparam.Position_x = reader.readFloat();				//重力点X
						readparam.Position_y = reader.readFloat();				//重力点Y
						readparam.Power = reader.readFloat();					//パワー

						ParticlePointGravity *effectParam = new ParticlePointGravity();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類
						effectParam->Position.x = readparam.Position_x;				//重力点X
						effectParam->Position.y = readparam.Position_y;				//重力点Y
						effectParam->Power = readparam.Power;					//パワー

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					case SsEffectFunctionType::TurnToDirectionEnabled:
					{
						//進行方向に向ける
						EffectParticleTurnToDirectionEnabled readparam;
						readparam.flag = reader.readS32();					//フラグ

						ParticleTurnToDirectionEnabled *effectParam = new ParticleTurnToDirectionEnabled();
						effectParam->setType((SsEffectFunctionType::enum_)type);				//コマンドの種類

						behavior.plist.push_back(effectParam);												//パラメータを追加
						break;
					}
					default:
						break;
					}
				}
				node->behavior = behavior;
				effectmodel->nodeList.push_back(node);
				if (nodeindex == 0)
				{
				}
			}
			//ツリーの構築
			if (effectmodel->nodeList.size() > 0)
			{
				effectmodel->root = effectmodel->nodeList[0];	//rootノードを追加
				for (size_t i = 1; i < effectmodel->nodeList.size(); i++)
				{
					int pi = effectmodel->nodeList[i]->parentIndex;
					if (pi >= 0)
					{
						effectmodel->nodeList[pi]->addChildEnd(effectmodel->nodeList[i]);
					}
				}
			}
			effectmodel->lockRandSeed = effectFile->lockRandSeed; 	 // ランダムシード固定値
			effectmodel->isLockRandSeed = effectFile->isLockRandSeed;  // ランダムシードを固定するか否か
			effectmodel->fps = effectFile->fps;             //
			effectmodel->effectName = effectFileName;


			SSLOG("effect key: %s", effectFileName.c_str());
			_dic.insert(std::map<std::string, SsEffectModel*>::value_type(effectFileName, effectmodel));
		}
	}
	//エフェクトファイル情報の削除
	void releseReference(void)
	{
		std::map<std::string, SsEffectModel*>::iterator it = _dic.begin();
		while (it != _dic.end())
		{
			SsEffectModel* effectmodel = it->second;

			if (effectmodel)
			{
				for (int nodeindex = 0; nodeindex < effectmodel->nodeList.size(); nodeindex++)
				{
					SsEffectNode* node = effectmodel->nodeList.at(nodeindex);
					for (int behaviorindex = 0; behaviorindex < node->behavior.plist.size(); behaviorindex++)
					{
						SsEffectElementBase* eb = node->behavior.plist.at(behaviorindex);
						delete eb;
					}
					node->behavior.plist.clear();
				}
				if (effectmodel->nodeList.size() > 0)
				{
					SsEffectNode* node = effectmodel->nodeList.at(0);
					delete node;
					effectmodel->nodeList.clear();
				}
				effectmodel->root = 0;

			}
			delete effectmodel;
			it++;
		}
		_dic.clear();
	}
protected:
	std::map<std::string, SsEffectModel*>		_dic;
};



/**
 * AnimeRef
 */
struct AnimeRef
{
	std::string				packName;
	std::string				animeName;
	const AnimationData*	animationData;
	const AnimePackData*	animePackData;
};


/**
 * AnimeCache
 */
class AnimeCache
{
public:
	AnimeCache()
	{
	}
	~AnimeCache()
	{
		releseReference();
	}
	static AnimeCache* create(const ProjectData* data)
	{
		AnimeCache* obj = new AnimeCache();
		if (obj)
		{
			obj->init(data);
		}
		return obj;
	}

	/**
	 * packNameとanimeNameを指定してAnimeRefを得る
	 */
	AnimeRef* getReference(const std::string& packName, const std::string& animeName)
	{
		std::string key = toPackAnimeKey(packName, animeName);
		AnimeRef* ref = _dic.at(key);
		return ref;
	}

	/**
	 * animeNameのみ指定してAnimeRefを得る
	 */
	AnimeRef* getReference(const std::string& animeName)
	{
		AnimeRef* ref = _dic.at(animeName);
		return ref;
	}
	
	void dump()
	{
		std::map<std::string, AnimeRef*>::iterator it = _dic.begin();
		while (it != _dic.end())
		{
			SSLOG("%s", (*it).second);
			++it;
		}
	}

protected:
	void init(const ProjectData* data)
	{
		SS_ASSERT2(data != NULL, "Invalid data");
		
		ToPointer ptr(data);
		const AnimePackData* animePacks = static_cast<const AnimePackData*>(ptr(data->animePacks));

		for (int packIndex = 0; packIndex < data->numAnimePacks; packIndex++)
		{
			const AnimePackData* pack = &animePacks[packIndex];
			const AnimationData* animations = static_cast<const AnimationData*>(ptr(pack->animations));
			const char* packName = static_cast<const char*>(ptr(pack->name));
			
			for (int animeIndex = 0; animeIndex < pack->numAnimations; animeIndex++)
			{
				const AnimationData* anime = &animations[animeIndex];
				const char* animeName = static_cast<const char*>(ptr(anime->name));
				
				AnimeRef* ref = new AnimeRef();
				ref->packName = packName;
				ref->animeName = animeName;
				ref->animationData = anime;
				ref->animePackData = pack;

				// packName + animeNameでの登録
				std::string key = toPackAnimeKey(packName, animeName);
				SSLOG("anime key: %s", key.c_str());
				_dic.insert(std::map<std::string, AnimeRef*>::value_type(key, ref));

				// animeNameのみでの登録
//				_dic.insert(std::map<std::string, AnimeRef*>::value_type(animeName, ref));
				
			}
		}
	}

	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName)
	{
		return Format("%s/%s", packName.c_str(), animeName.c_str());
	}

	//キャッシュの削除
	void releseReference(void)
	{
		std::map<std::string, AnimeRef*>::iterator it = _dic.begin();
		while (it != _dic.end())
		{
			AnimeRef* ref = it->second;
			if (ref)
			{
				delete ref;
				it->second = 0;
			}
			it++;
		}
		_dic.clear();
	}

protected:
	std::map<std::string, AnimeRef*>	_dic;
};





/**
 * ResourceSet
 */
struct ResourceSet
{
	const ProjectData* data;
	bool isDataAutoRelease;
	EffectCache* effectCache;
	CellCache* cellCache;
	AnimeCache* animeCache;

	virtual ~ResourceSet()
	{
		if (isDataAutoRelease)
		{
			delete data;
			data = NULL;
		}
		if (animeCache)
		{
			delete animeCache;
			animeCache = NULL;
		}
		if (cellCache)
		{
			delete cellCache;
			cellCache = NULL;
		}
		if (effectCache)
		{
			delete effectCache;
			effectCache = NULL;
		}
	}
};


/**
 * ResourceManager
 */

static ResourceManager* defaultInstance = NULL;
const std::string ResourceManager::s_null;

ResourceManager* ResourceManager::getInstance()
{
	if (!defaultInstance)
	{
		defaultInstance = ResourceManager::create();
	}
	return defaultInstance;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager()
{
	removeAllData();
}

ResourceManager* ResourceManager::create()
{
	ResourceManager* obj = new ResourceManager();
	return obj;
}

ResourceSet* ResourceManager::getData(const std::string& dataKey)
{
	ResourceSet* rs = _dataDic.at(dataKey);
	return rs;
}

std::string ResourceManager::addData(const std::string& dataKey, const ProjectData* data, const std::string& imageBaseDir)
{
	SS_ASSERT2(data != NULL, "Invalid data");
	SS_ASSERT2(data->dataId == DATA_ID, "Not data id matched");
	SS_ASSERT2(data->version == DATA_VERSION, "Version number of data does not match");
	
	// imageBaseDirの指定がないときコンバート時に指定されたパスを使用する
	std::string baseDir = imageBaseDir;
	if (imageBaseDir == s_null && data->imageBaseDir)
	{
		ToPointer ptr(data);
		const char* dir = static_cast<const char*>(ptr(data->imageBaseDir));
		baseDir = dir;
	}

	//アニメはエフェクトを参照し、エフェクトはセルを参照するのでこの順番で生成する必要がある
	CellCache* cellCache = CellCache::create(data, baseDir);

	EffectCache* effectCache = EffectCache::create(data, baseDir, cellCache);	//

	AnimeCache* animeCache = AnimeCache::create(data);

	ResourceSet* rs = new ResourceSet();
	rs->data = data;
	rs->isDataAutoRelease = false;
	rs->cellCache = cellCache;
	rs->animeCache = animeCache;
	rs->effectCache = effectCache;
	_dataDic.insert(std::map<std::string, ResourceSet*>::value_type(dataKey, rs));

	return dataKey;
}

std::string ResourceManager::addDataWithKey(const std::string& dataKey, const std::string& ssbpFilepath, const std::string& imageBaseDir)
{

	std::string fullpath = ssbpFilepath;

	unsigned long nSize = 0;
	void* loadData = SSFileOpen(fullpath.c_str(), "rb", &nSize);
	if (loadData == NULL)
	{
		std::string msg = "Can't load project data > " + fullpath;
		SS_ASSERT2(loadData != NULL, msg.c_str());
	}
	
	const ProjectData* data = static_cast<const ProjectData*>(loadData);
	SS_ASSERT2(data->dataId == DATA_ID, "Not data id matched");
	SS_ASSERT2(data->version == DATA_VERSION, "Version number of data does not match");
	
	std::string baseDir = imageBaseDir;
	if (imageBaseDir == s_null)
	{
		// imageBaseDirの指定がないとき
		if (data->imageBaseDir)
		{
			// コンバート時に指定されたパスを使用する
			ToPointer ptr(data);
			const char* dir = static_cast<const char*>(ptr(data->imageBaseDir));
			baseDir = dir;
		}
		else
		{
			// プロジェクトファイルと同じディレクトリを指定する
			std::string directory;
			std::string filename;
			splitPath(directory, filename, ssbpFilepath);
			baseDir = directory;
		}
		//SSLOG("imageBaseDir: %s", baseDir.c_str());
	}

	addData(dataKey, data, baseDir);
	
	// リソースが破棄されるとき一緒にロードしたデータも破棄する
	ResourceSet* rs = getData(dataKey);
	SS_ASSERT2(rs != NULL, "");
	rs->isDataAutoRelease = true;
	
	return dataKey;
}

std::string ResourceManager::addData(const std::string& ssbpFilepath, const std::string& imageBaseDir)
{
	// ファイル名を取り出す
	std::string directory;
    std::string filename;
	splitPath(directory, filename, ssbpFilepath);
	
	// 拡張子を取る
	std::string dataKey = filename;
	size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos)
    {
        dataKey = filename.substr(0, pos);
    }

	//登録されている名前か判定する
	std::map<std::string, ResourceSet*>::iterator it = _dataDic.find(dataKey);
	if (it != _dataDic.end())
	{
		//登録されている場合は処理を行わない
		std::string str = "";
		return str;
	}

	return addDataWithKey(dataKey, ssbpFilepath, imageBaseDir);
}

void ResourceManager::removeData(const std::string& dataKey)
{
	ResourceSet* rs = getData(dataKey);

	//バイナリデータの削除
	delete rs;
	_dataDic.erase(dataKey);
}

void ResourceManager::removeAllData()
{
	//全リソースの解放
	while (!_dataDic.empty())
	{
		std::map<std::string, ResourceSet*>::iterator it = _dataDic.begin();
		std::string ssbpName = it->first;
		removeData(ssbpName);
	}
	_dataDic.clear();
}

//データ名、セル名を指定して、セルで使用しているテクスチャを変更する
bool ResourceManager::changeTexture(char* ssbpName, char* ssceName, long texture)
{
	bool rc = false;

	ResourceSet* rs = getData(ssbpName);
	rc = rs->cellCache->setCellRefTexture(rs->data, ssceName, texture);

	return(rc);
}

//指定したデータのテクスチャを破棄します
bool ResourceManager::releseTexture(char* ssbpName)
{

	ResourceSet* rs = getData(ssbpName);
	bool rc = rs->cellCache->releseTexture(rs->data);

	return(rc);
}

//アニメーションの総フレーム数を取得する
int ResourceManager::getMaxFrame(std::string ssbpName, std::string animeName)
{
	int rc = -1;

	ResourceSet* rs = getData(ssbpName);
	AnimeRef* animeRef = rs->animeCache->getReference(animeName);
	if (animeRef == NULL)
	{
		std::string msg = Format("Not found animation > anime=%s", animeName.c_str());
		SS_ASSERT2(animeRef != NULL, msg.c_str());
	}
	rc = animeRef->animationData->numFrames;

	return(rc);
}

/**
 * Player
 */

static const std::string s_nullString;

Player::Player(void)
	: _resman(NULL)
	, _currentRs(NULL)
	, _currentAnimeRef(NULL)

	, _frameSkipEnabled(true)
	, _playingFrame(0.0f)
	, _step(1.0f)
	, _loop(0)
	, _loopCount(0)
	, _isPlaying(false)
	, _isPausing(false)
	, _prevDrawFrameNo(-1)
	, _InstanceAlpha(255)
	, _InstanceRotX(0.0f)
	, _InstanceRotY(0.0f)
	, _InstanceRotZ(0.0f)
	, _gamefps(1000.0f/60.0f)	//60fps
	, _col_r(255)
	, _col_g(255)
	, _col_b(255)
	, _instanceOverWrite(false)
{
	int i;
	for (i = 0; i < PART_VISIBLE_MAX; i++)
	{
		_partVisible[i] = true;
		_partIndex[i] = -1;
		_cellChange[i] = -1;
	}
	_state.init();
}

Player::~Player()
{
	releaseParts();
	releaseData();
	releaseResourceManager();
	releaseAnime();
}

Player* Player::create(ResourceManager* resman)
{
	Player* obj = new Player();
	if (obj && obj->init())
	{
		obj->setResourceManager(resman);
		return obj;
	}
	SS_SAFE_DELETE(obj);
	return NULL;
}

bool Player::init()
{
	return true;
}

void Player::releaseResourceManager()
{
}

void Player::setResourceManager(ResourceManager* resman)
{
	if (_resman) releaseResourceManager();
	
	if (!resman)
	{
		// nullのときはデフォルトを使用する
		resman = ResourceManager::getInstance();
	}
	
	_resman = resman;
}

int Player::getMaxFrame() const
{
	if (_currentAnimeRef )
	{
		return(_currentAnimeRef->animationData->numFrames);
	}
	else
	{
		return(0);
	}

}

int Player::getFrameNo() const
{
	return static_cast<int>(_playingFrame);
}

void Player::setFrameNo(int frameNo)
{
	_playingFrame = (float)frameNo;
}

float Player::getStep() const
{
	return _step;
}

void Player::setStep(float step)
{
	_step = step;
}

int Player::getLoop() const
{
	return _loop;
}

void Player::setLoop(int loop)
{
	if (loop < 0) return;
	_loop = loop;
}

int Player::getLoopCount() const
{
	return _loopCount;
}

void Player::clearLoopCount()
{
	_loopCount = 0;
}

void Player::setFrameSkipEnabled(bool enabled)
{
	_frameSkipEnabled = enabled;
	_playingFrame = (float)((int)_playingFrame);
}

bool Player::isFrameSkipEnabled() const
{
	return _frameSkipEnabled;
}

void Player::setData(const std::string& dataKey)
{
	ResourceSet* rs = _resman->getData(dataKey);
	_currentdataKey = dataKey;
	if (rs == NULL)
	{
		std::string msg = Format("Not found data > %s", dataKey.c_str());
		SS_ASSERT2(rs != NULL, msg.c_str());
	}
	
	if (_currentRs != rs)
	{
//		releaseData();
		_currentRs = rs;
	}
}

void Player::releaseData()
{
	releaseAnime();
}


void Player::releaseAnime()
{
	releaseParts();
}

void Player::play(const std::string& ssaeName, const std::string& motionName, int loop, int startFrameNo)
{
	std::string animeName = Format("%s/%s", ssaeName.c_str(), motionName.c_str());
	play(animeName, loop, startFrameNo);
}

void Player::play(const std::string& animeName, int loop, int startFrameNo)
{
	SS_ASSERT2(_currentRs != NULL, "Not select data");

	AnimeRef* animeRef = _currentRs->animeCache->getReference(animeName);
	if (animeRef == NULL)
	{
		std::string msg = Format("Not found animation > anime=%s", animeName.c_str());
		SS_ASSERT2(animeRef != NULL, msg.c_str());
	}
	_currentAnimename = animeName;

	play(animeRef, loop, startFrameNo);
}

void Player::play(AnimeRef* animeRef, int loop, int startFrameNo)
{
	if (_currentAnimeRef != animeRef)
	{
		_currentAnimeRef = animeRef;
		
		allocParts(animeRef->animePackData->numParts, false);
		setPartsParentage();
	}
	_playingFrame = static_cast<float>(startFrameNo);
	_step = 1.0f;
	_loop = loop;
	_loopCount = 0;
	_isPlaying = true;
	_isPausing = false;
	_prevDrawFrameNo = -1;
	_isPlayFirstUserdataChack = true;
	_animefps = _currentAnimeRef->animationData->fps;

	setFrame((int)_playingFrame);
}

void Player::animePause()
{
	_isPausing = true;
}

void Player::animeResume()
{
	_isPausing = false;
}

void Player::stop()
{
	_isPlaying = false;
}

const std::string& Player::getPlayPackName() const
{
	return _currentAnimeRef != NULL ? _currentAnimeRef->packName : s_nullString;
}

const std::string& Player::getPlayAnimeName() const
{
	return _currentAnimeRef != NULL ? _currentAnimeRef->animeName : s_nullString;
}


void Player::update(float dt)
{
	updateFrame(dt);
}

void Player::updateFrame(float dt)
{
	if (!_currentAnimeRef) return;
	if (!_currentRs->data) return;

	bool playEnd = false;
	bool toNextFrame = _isPlaying && !_isPausing;
	if (toNextFrame && (_loop == 0 || _loopCount < _loop))
	{
		// フレームを進める.
		// forward frame.
		const int numFrames = _currentAnimeRef->animationData->numFrames;

		float fdt = dt;
		float s = fdt / (1.0f / _currentAnimeRef->animationData->fps);
		
		//if (!m_frameSkipEnabled) SSLOG("%f", s);
		
		float next = _playingFrame + (s * _step);

		int nextFrameNo = static_cast<int>(next);
		float nextFrameDecimal = next - static_cast<float>(nextFrameNo);
		int currentFrameNo = static_cast<int>(_playingFrame);

		//playを行って最初のupdateでは現在のフレームのユーザーデータを確認する
		if (_isPlayFirstUserdataChack == true)
		{
			checkUserData(currentFrameNo);
			_isPlayFirstUserdataChack = false;
		}

		if (_step >= 0)
		{
			// 順再生時.
			// normal plays.
			for (int c = nextFrameNo - currentFrameNo; c; c--)
			{
				int incFrameNo = currentFrameNo + 1;
				if (incFrameNo >= numFrames)
				{
					// アニメが一巡
					// turned animation.
					_loopCount += 1;
					if (_loop && _loopCount >= _loop)
					{
						// 再生終了.
						// play end.
						playEnd = true;
						break;
					}
					
					incFrameNo = 0;
				}
				currentFrameNo = incFrameNo;

				// このフレームのユーザーデータをチェック
				// check the user data of this frame.
				checkUserData(currentFrameNo);
			}
		}
		else
		{
			// 逆再生時.
			// reverse play.
			for (int c = currentFrameNo - nextFrameNo; c; c--)
			{
				int decFrameNo = currentFrameNo - 1;
				if (decFrameNo < 0)
				{
					// アニメが一巡
					// turned animation.
					_loopCount += 1;
					if (_loop && _loopCount >= _loop)
					{
						// 再生終了.
						// play end.
						playEnd = true;
						break;
					}
				
					decFrameNo = numFrames - 1;
				}
				currentFrameNo = decFrameNo;
				
				// このフレームのユーザーデータをチェック
				// check the user data of this frame.
				checkUserData(currentFrameNo);
			}
		}
		
		_playingFrame = static_cast<float>(currentFrameNo) + nextFrameDecimal;
	}
	else
	{
		//アニメを手動で更新する場合
		checkUserData(getFrameNo());
	}

	setFrame(getFrameNo());
	
	if (playEnd)
	{
		stop();
	
		// 再生終了コールバックの呼び出し
		SSPlayEnd(this);
	}
}


void Player::allocParts(int numParts, bool useCustomShaderProgram)
{
	_parts.clear();	//すべてのパーツを消す
	{
		// パーツ数だけCustomSpriteを作成する
//		// create CustomSprite objects.
		for (int i = 0; i < numParts; i++)
		{
			CustomSprite* sprite =  CustomSprite::create();
			sprite->_ssplayer = NULL;
			sprite->changeShaderProgram(useCustomShaderProgram);

			_parts.push_back(sprite);
		}
	}
}

void Player::releaseParts()
{
	// パーツの子CustomSpriteを全て削除
	// remove children CCSprite objects.
	if (_currentRs)
	{
		if (_currentAnimeRef)
		{

			ToPointer ptr(_currentRs->data);
			const AnimePackData* packData = _currentAnimeRef->animePackData;
			const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));
			if (_parts.size() > 0)
			{
				for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
				{
					CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
					SS_SAFE_DELETE(sprite->_ssplayer);
				}
			}
		}
	}

	_parts.clear();
}

void Player::setPartsParentage()
{
	if (!_currentAnimeRef) return;

	ToPointer ptr(_currentRs->data);
	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	//親子関係を設定
	for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		
		if (partIndex > 0)
		{
			CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
			sprite->_parent = parent;
		}
		else
		{
			sprite->_parent = NULL;
		}

		//インスタンスパーツの生成
		std::string refanimeName = static_cast<const char*>(ptr(partData->refname));

		SS_SAFE_DELETE(sprite->_ssplayer);
		if (refanimeName != "")
		{
			//インスタンスパーツが設定されている
			sprite->_ssplayer = ss::Player::create();
			sprite->_ssplayer->setData(_currentdataKey);
			sprite->_ssplayer->play(refanimeName);				 // アニメーション名を指定(ssae名/アニメーション名も可能、詳しくは後述)
			sprite->_ssplayer->animePause();
		}

		//エフェクトパーツの生成
		if (sprite->refEffect)
		{
			delete sprite->refEffect;
			sprite->refEffect = 0;
		}

		std::string refeffectName = static_cast<const char*>(ptr(partData->effectfilename));
		if (refeffectName != "")
		{
			SsEffectModel* effectmodel = _currentRs->effectCache->getReference(refeffectName);
			if (effectmodel)
			{
				//エフェクトクラスにパラメータを設定する
				SsEffectRenderer* er = new SsEffectRenderer();
				sprite->refEffect = er;
				sprite->refEffect->setParentAnimeState(&sprite->partState);
				sprite->refEffect->setEffectData(effectmodel);
//				sprite->refEffect->setEffectSprite(&_effectSprite);	//エフェクトクラスに渡す都合上publicにしておく
//				sprite->refEffect->setEffectSpriteCount(&_effectSpriteCount);	//エフェクトクラスに渡す都合上publicにしておく
				sprite->refEffect->setSeed(getRandomSeed());
				sprite->refEffect->reload();
				sprite->refEffect->stop();
			}
		}
	}
}

//indexからパーツ名を取得
const char* Player::getPartName(int partId) const
{
	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	SS_ASSERT2(partId >= 0 && partId < packData->numParts, "partId is out of range.");

	const PartData* partData = static_cast<const PartData*>(ptr(packData->parts));
	const char* name = static_cast<const char*>(ptr(partData[partId].name));
	return name;
}

//パーツ名からindexを取得
int Player::indexOfPart(const char* partName) const
{
	const AnimePackData* packData = _currentAnimeRef->animePackData;
	for (int i = 0; i < packData->numParts; i++)
	{
		const char* name = getPartName(i);
		if (strcmp(partName, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

/*
 パーツ名から指定フレームのパーツステータスを取得します。
 必要に応じて　ResluteState　を編集しデータを取得してください。

 指定したフレームの状態にすべてのパーツのステータスを更新します。
 描画を行う前にupdateを呼び出し、パーツステータスを表示に状態に戻してからdrawしてください。
*/
bool Player::getPartState(ResluteState& result, const char* name, int frameNo)
{
	bool rc = false;
	if (_currentAnimeRef)
	{
		{
			//カレントフレームのパーツステータスを取得する
			if (frameNo == -1)
			{
				//フレームの指定が省略された場合は現在のフレームを使用する
				frameNo = getFrameNo();
			}

			if (frameNo != getFrameNo())
			{
				//取得する再生フレームのデータが違う場合プレイヤーを更新する
				//パーツステータスの更新
				setFrame(frameNo);
			}

			ToPointer ptr(_currentRs->data);

			const AnimePackData* packData = _currentAnimeRef->animePackData;
			const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

			for (int index = 0; index < packData->numParts; index++)
			{
				int partIndex = _partIndex[index];

				const PartData* partData = &parts[partIndex];
				const char* partName = static_cast<const char*>(ptr(partData->name));
				if (strcmp(partName, name) == 0)
				{
					//必要に応じて取得するパラメータを追加してください。
					//当たり判定などのパーツに付属するフラグを取得する場合は　partData　のメンバを参照してください。
					//親から継承したスケールを反映させる場合はxスケールは_mat.m[0]、yスケールは_mat.m[5]をかけて使用してください。
					CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
					result.x = sprite->_state.mat[12];
					result.y = sprite->_state.mat[13];

					//パーツアトリビュート
//					sprite->_state;												//SpriteStudio上のアトリビュートの値は_stateから取得してください
					result.flags = sprite->_state.flags;						// このフレームで更新が行われるステータスのフラグ
					result.cellIndex = sprite->_state.cellIndex;				// パーツに割り当てられたセルの番号
					result.x = sprite->_state.mat[12];
					result.y = sprite->_state.mat[13];
					result.z = sprite->_state.z;
					result.pivotX = sprite->_state.pivotX;						// 原点Xオフセット＋セルに設定された原点オフセットX
					result.pivotY = sprite->_state.pivotY;						// 原点Yオフセット＋セルに設定された原点オフセットY
					result.rotationX = sprite->_state.rotationX;				// X回転（親子関係計算済）
					result.rotationY = sprite->_state.rotationY;				// Y回転（親子関係計算済）
					result.rotationZ = sprite->_state.rotationZ;				// Z回転（親子関係計算済）
					result.scaleX = sprite->_state.scaleX;						// Xスケール（親子関係計算済）
					result.scaleY = sprite->_state.scaleY;						// Yスケール（親子関係計算済）
					result.opacity = sprite->_state.opacity;					// 不透明度（0～255）（親子関係計算済）
					result.size_X = sprite->_state.size_X;						// SS5アトリビュート：Xサイズ
					result.size_Y = sprite->_state.size_Y;						// SS5アトリビュート：Xサイズ
					result.uv_move_X = sprite->_state.uv_move_X;				// SS5アトリビュート：UV X移動
					result.uv_move_Y = sprite->_state.uv_move_Y;				// SS5アトリビュート：UV Y移動
					result.uv_rotation = sprite->_state.uv_rotation;			// SS5アトリビュート：UV 回転
					result.uv_scale_X = sprite->_state.uv_scale_X;				// SS5アトリビュート：UV Xスケール
					result.uv_scale_Y = sprite->_state.uv_scale_Y;				// SS5アトリビュート：UV Yスケール
					result.boundingRadius = sprite->_state.boundingRadius;		// SS5アトリビュート：当たり半径
					result.colorBlendFunc = sprite->_state.colorBlendFunc;		// SS5アトリビュート：カラーブレンドのブレンド方法
					result.colorBlendType = sprite->_state.colorBlendType;		// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
					result.flipX = sprite->_state.flipX;						// 横反転（親子関係計算済）
					result.flipY = sprite->_state.flipY;						// 縦反転（親子関係計算済）
					result.isVisibled = sprite->_state.isVisibled;				// 非表示（親子関係計算済）

					//パーツ設定
					result.part_type = partData->type;							//パーツ種別
					result.part_boundsType = partData->boundsType;				//当たり判定種類
					result.part_alphaBlendType = partData->alphaBlendType;		// BlendType
					//ラベルカラー
					std::string colorName = static_cast<const char*>(ptr(partData->colorLabel));
					if (colorName == COLORLABELSTR_NONE)
					{
						result.part_labelcolor = COLORLABEL_NONE;
					}
					if (colorName == COLORLABELSTR_RED)
					{
						result.part_labelcolor = COLORLABEL_RED;
					}
					if (colorName == COLORLABELSTR_ORANGE)
					{
						result.part_labelcolor = COLORLABEL_ORANGE;
					}
					if (colorName == COLORLABELSTR_YELLOW)
					{
						result.part_labelcolor = COLORLABEL_YELLOW;
					}
					if (colorName == COLORLABELSTR_GREEN)
					{
						result.part_labelcolor = COLORLABEL_GREEN;
					}
					if (colorName == COLORLABELSTR_BLUE)
					{
						result.part_labelcolor = COLORLABEL_BLUE;
					}
					if (colorName == COLORLABELSTR_VIOLET)
					{
						result.part_labelcolor = COLORLABEL_VIOLET;
					}
					if (colorName == COLORLABELSTR_GRAY)
					{
						result.part_labelcolor = COLORLABEL_GRAY;
					}

					rc = true;
					break;
				}
			}
			//パーツステータスを表示するフレームの内容で更新
			if (frameNo != getFrameNo())
			{
				//取得する再生フレームのデータが違う場合プレイヤーの状態をもとに戻す
				//パーツステータスの更新
				setFrame(getFrameNo());
			}
		}
	}
	return rc;
}


//ラベル名からラベルの設定されているフレームを取得
//ラベルが存在しない場合は戻り値が-1となります。
//ラベル名が全角でついていると取得に失敗します。
int Player::getLabelToFrame(char* findLabelName)
{
	int rc = -1;

	ToPointer ptr(_currentRs->data);
	const AnimationData* animeData = _currentAnimeRef->animationData;

	if (!animeData->labelData) return -1;
	const ss_offset* labelDataIndex = static_cast<const ss_offset*>(ptr(animeData->labelData));


	int idx = 0;
	for (idx = 0; idx < animeData->labelNum; idx++ )
	{
		if (!labelDataIndex[idx]) return -1;
		const ss_u16* labelDataArray = static_cast<const ss_u16*>(ptr(labelDataIndex[idx]));

		DataArrayReader reader(labelDataArray);

		LabelData ldata;
		ss_offset offset = reader.readOffset();
		const char* str = static_cast<const char*>(ptr(offset));
		int labelFrame = reader.readU16();
		ldata.str = str;
		ldata.frameNo = labelFrame;

		if (ldata.str.compare(findLabelName) == 0 )
		{
			//同じ名前のラベルが見つかった
			return (ldata.frameNo);
		}
	}

	return (rc);
}

//特定パーツの表示、非表示を設定します
//パーツ番号はスプライトスタジオのフレームコントロールに配置されたパーツが
//プライオリティでソートされた後、上に配置された順にソートされて決定されます。
void Player::setPartVisible(std::string partsname, bool flg)
{
	bool rc = false;
	if (_currentAnimeRef)
	{
		ToPointer ptr(_currentRs->data);

		const AnimePackData* packData = _currentAnimeRef->animePackData;
		const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

		for (int index = 0; index < packData->numParts; index++)
		{
			int partIndex = _partIndex[index];

			const PartData* partData = &parts[partIndex];
			const char* partName = static_cast<const char*>(ptr(partData->name));
			if (strcmp(partName, partsname.c_str()) == 0)
			{
				_partVisible[index] = flg;
				break;
			}
		}
	}
}

//パーツに割り当たるセルを変更します
void Player::setPartCell(std::string partsname, std::string sscename, std::string cellname)
{
	bool rc = false;
	if (_currentAnimeRef)
	{
		ToPointer ptr(_currentRs->data);

		int changeCellIndex = -1;
		if ((sscename != "") && (cellname != ""))
		{
			//セルマップIDを取得する
			//必要あり


			const Cell* cells = static_cast<const Cell*>(ptr(_currentRs->data->cells));

			//名前からインデックスの取得
			int cellindex = -1;
			for (int i = 0; i < _currentRs->data->numCells; i++)
			{
				const Cell* cell = &cells[i];
				const char* name1 = static_cast<const char*>(ptr(cell->name));
				const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
				const char* name2 = static_cast<const char*>(ptr(cellMap->name));
				if (strcmp(cellname.c_str(), name1) == 0)
				{
					if (strcmp(sscename.c_str(), name2) == 0)
					{
						changeCellIndex = i;
						break;
					}
				}
			}
		}

		const AnimePackData* packData = _currentAnimeRef->animePackData;
		const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

		for (int index = 0; index < packData->numParts; index++)
		{
			int partIndex = _partIndex[index];

			const PartData* partData = &parts[partIndex];
			const char* partName = static_cast<const char*>(ptr(partData->name));
			if (strcmp(partName, partsname.c_str()) == 0)
			{
				//セル番号を設定
				_cellChange[index] = changeCellIndex;	//上書き解除
				break;
			}
		}
	}
}

// インスタンスパーツが再生するアニメを変更します。
bool Player::changeInstanceAnime(std::string partsname, std::string animename, bool overWrite, Instance keyParam)
{
	//名前からパーツを取得
	bool rc = false;
	if (_currentAnimeRef)
	{
		ToPointer ptr(_currentRs->data);

		const AnimePackData* packData = _currentAnimeRef->animePackData;
		const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

		for (int index = 0; index < packData->numParts; index++)
		{
			int partIndex = _partIndex[index];

			const PartData* partData = &parts[partIndex];
			const char* partName = static_cast<const char*>(ptr(partData->name));
			if (strcmp(partName, partsname.c_str()) == 0)
			{
				CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
				if (sprite->_ssplayer)
				{
					//パーツがインスタンスパーツの場合は再生するアニメを設定する
					//アニメが入れ子にならないようにチェックする
					if (_currentAnimename != animename)
					{
						sprite->_ssplayer->play(animename);
						setInstanceParam(overWrite, keyParam);	//インスタンスパラメータの設定
						sprite->_ssplayer->animeResume();		//アニメ切り替え時にがたつく問題の対応
						sprite->_liveFrame = 0;					//独立動作の場合再生位置をリセット
						rc = true;
					}
				}

				break;
			}
		}
	}

	return (rc);
}
//インスタンスパラメータを設定します
void Player::setInstanceParam(bool overWrite, Instance keyParam)
{
	_instanceOverWrite = overWrite;		//インスタンス情報を上書きするか？
	_instanseParam = keyParam;			//インスタンスパラメータ

}
//インスタンスパラメータを取得します
void Player::getInstanceParam(bool *overWrite, Instance *keyParam)
{
	*overWrite = _instanceOverWrite;		//インスタンス情報を上書きするか？
	*keyParam = _instanseParam;			//インスタンスパラメータ
}

//アニメーションの色成分を変更します
void Player::setColor(int r, int g, int b)
{
	_col_r = r;
	_col_g = g;
	_col_b = b;
}

void Player::setFrame(int frameNo)
{
	if (!_currentAnimeRef) return;
	if (!_currentRs->data) return;

	bool forceUpdate = false;
	{
		// フリップに変化があったときは必ず描画を更新する
		CustomSprite* root = static_cast<CustomSprite*>(_parts.at(0));
		float scaleX = root->isFlippedX() ? -1.0f : 1.0f;
		float scaleY = root->isFlippedY() ? -1.0f : 1.0f;
		root->setStateValue(root->_state.x, scaleX);
		root->setStateValue(root->_state.y, scaleY);
		forceUpdate = root->_isStateChanged;
	}
	
	// 前回の描画フレームと同じときはスキップ
	//インスタンスアニメがあるので毎フレーム更新するためコメントに変更
	//	if (!forceUpdate && frameNo == _prevDrawFrameNo) return;

	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	const AnimationData* animeData = _currentAnimeRef->animationData;
	const ss_offset* frameDataIndex = static_cast<const ss_offset*>(ptr(animeData->frameData));
	
	const ss_u16* frameDataArray = static_cast<const ss_u16*>(ptr(frameDataIndex[frameNo]));
	DataArrayReader reader(frameDataArray);
	
	const AnimationInitialData* initialDataList = static_cast<const AnimationInitialData*>(ptr(animeData->defaultData));


	State state;

	for (int index = 0; index < packData->numParts; index++)
	{
		int partIndex = reader.readS16();
		const PartData* partData = &parts[partIndex];
		const AnimationInitialData* init = &initialDataList[partIndex];

		// optional parameters
		int flags      = reader.readU32();
		int cellIndex  = flags & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;
		float x        = flags & PART_FLAG_POSITION_X ? (float)reader.readS16() : (float)init->positionX;
#ifdef UP_MINUS
		float y        = flags & PART_FLAG_POSITION_Y ? (float)-reader.readS16() : (float)-init->positionY;		//上がマイナスなので反転させる
#else
		float y        = flags & PART_FLAG_POSITION_Y ? (float)reader.readS16() : (float)init->positionY;
#endif
		float z        = flags & PART_FLAG_POSITION_Z ? (float)reader.readS16() : (float)init->positionZ;
		float pivotX   = flags & PART_FLAG_PIVOT_X ? reader.readFloat() : init->pivotX;
#ifdef UP_MINUS
		float pivotY = flags & PART_FLAG_PIVOT_Y ? -reader.readFloat() : -init->pivotY;
#else
		float pivotY = flags & PART_FLAG_PIVOT_Y ? reader.readFloat() : init->pivotY;
#endif
#ifdef UP_MINUS
		float rotationX = flags & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;
		float rotationY = flags & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;
		float rotationZ = flags & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;
#else
		float rotationX = flags & PART_FLAG_ROTATIONX ? reader.readFloat() : init->rotationX;
		float rotationY = flags & PART_FLAG_ROTATIONY ? reader.readFloat() : init->rotationY;
		float rotationZ = flags & PART_FLAG_ROTATIONZ ? reader.readFloat() : init->rotationZ;
#endif
		float scaleX = flags & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;
		float scaleY   = flags & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;
		int opacity    = flags & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;
		float size_X   = flags & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;
		float size_Y   = flags & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;
		float uv_move_X   = flags & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;
		float uv_move_Y   = flags & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;
		float uv_rotation = flags & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;
		float uv_scale_X  = flags & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;
		float uv_scale_Y  = flags & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;
		float boundingRadius = flags & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;
		bool flipX = (bool)(flags & PART_FLAG_FLIP_H);
		bool flipY = (bool)(flags & PART_FLAG_FLIP_V);

		bool isVisibled = !(flags & PART_FLAG_INVISIBLE);

		if (_partVisible[index] == false)
		{
			//ユーザーが任意に非表示としたパーツは非表示に設定
			isVisibled = false;
		}
		if (_cellChange[index] != -1)
		{
			//ユーザーがセルを上書きした
			cellIndex = _cellChange[index];
		}

		//固定少数を少数へ戻す
		x = x / DOT;
		y = y / DOT;
		z = z / DOT;

		_partIndex[index] = partIndex;

		if ( _state.flipX == true )
		{
			//プレイヤーのXフリップ
			flipX = !flipX;	//フラグ反転
		}
		if (_state.flipY == true)
		{
			//プレイヤーのYフリップ
			flipY = !flipY;	//フラグ反転
		}

		//セルの原点設定を反映させる
		CellRef* cellRef = cellIndex >= 0 ? _currentRs->cellCache->getReference(cellIndex) : nullptr;
		if (cellRef)
		{
			float cpx = 0;
			float cpy = 0;

			cpx = cellRef->cell->pivot_X;
			if (flipX) cpx = -cpx;	// 水平フリップによって原点を入れ替える
			cpy = cellRef->cell->pivot_Y;
			if (flipY) cpy = -cpy;	// 垂直フリップによって原点を入れ替える

			pivotX += cpx;
			pivotY += cpy;

		}
		pivotX += 0.5f;
		pivotY += 0.5f;


		//ステータス保存
		state.flags = flags;
		state.cellIndex = cellIndex;
		state.x = x;
		state.y = y;
		state.z = z;
		state.pivotX = pivotX;
		state.pivotY = pivotY;
		state.rotationX = rotationX;
		state.rotationY = rotationY;
		state.rotationZ = rotationZ;
		state.scaleX = scaleX;
		state.scaleY = scaleY;
		state.opacity = opacity;
		state.size_X = size_X;
		state.size_Y = size_Y;
		state.uv_move_X = uv_move_X;
		state.uv_move_Y = uv_move_Y;
		state.uv_rotation = uv_rotation;
		state.uv_scale_X = uv_scale_X;
		state.uv_scale_Y = uv_scale_Y;
		state.boundingRadius = boundingRadius;
		state.isVisibled = isVisibled;
		state.flipX = flipX;
		state.flipY = flipY;
		state.instancerotationX = _InstanceRotX;
		state.instancerotationY = _InstanceRotY;
		state.instancerotationZ = _InstanceRotZ;

		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));

		//反転
		//反転はUVにも反映させておくので使いやすい方で反転してください。
		sprite->setFlippedX(flipX);
		sprite->setFlippedY(flipY);

		bool setBlendEnabled = true;

		if (cellRef)
		{
			//各パーツのテクスチャ情報を設定
			state.texture = cellRef->texture;
			state.rect = cellRef->rect;
			state.blendfunc = partData->alphaBlendType;

			if (setBlendEnabled)
			{
				if (flags & PART_FLAG_COLOR_BLEND)
				{
					//カラーブレンドを行うときはカスタムシェーダーを使用する
					sprite->changeShaderProgram(true);
				}
				else
				{
					sprite->changeShaderProgram(false);
				}
			}
		}
		else
		{
			state.texture.handle = -1;
			//セルが無く通常パーツ、ヌルパーツの時は非表示にする
			if ((partData->type == PARTTYPE_NORMAL) || (partData->type == PARTTYPE_NULL))
			{
				state.isVisibled = false;
			}
		}
		sprite->setOpacity(opacity);

		//頂点データの設定
		//quadにはプリミティブの座標（頂点変形を含む）、UV、カラー値が設定されます。
		SSV3F_C4B_T2F_Quad quad;
		memset(&quad, 0, sizeof(quad));
		if (cellRef)
		{
			//頂点を設定する
			float width_h = cellRef->rect.size.width / 2;
			float height_h = cellRef->rect.size.height / 2;
			float x1 = -width_h;
			float y1 = -height_h;
			float x2 = width_h;
			float y2 = height_h;

#ifdef UP_MINUS
			quad.tl.vertices.x = x1;
			quad.tl.vertices.y = y1;
			quad.tr.vertices.x = x2;
			quad.tr.vertices.y = y1;
			quad.bl.vertices.x = x1;
			quad.bl.vertices.y = y2;
			quad.br.vertices.x = x2;
			quad.br.vertices.y = y2;
#else
			quad.tl.vertices.x = x1;
			quad.tl.vertices.y = y2;
			quad.tr.vertices.x = x2;
			quad.tr.vertices.y = y2;
			quad.bl.vertices.x = x1;
			quad.bl.vertices.y = y1;
			quad.br.vertices.x = x2;
			quad.br.vertices.y = y1;
#endif
			//UVを設定する
			int atlasWidth = state.texture.size_w;
			int atlasHeight = state.texture.size_h;
			float left, right, top, bottom;
			left = cellRef->rect.origin.x / (float)atlasWidth;
			right = (cellRef->rect.origin.x + cellRef->rect.size.width) / (float)atlasWidth;
			top = cellRef->rect.origin.y / (float)atlasHeight;
			bottom = (cellRef->rect.origin.y + cellRef->rect.size.height) / (float)atlasHeight;

			quad.tl.texCoords.u = left;
			quad.tl.texCoords.v = top;
			quad.tr.texCoords.u = right;
			quad.tr.texCoords.v = top;
			quad.bl.texCoords.u = left;
			quad.bl.texCoords.v = bottom;
			quad.br.texCoords.u = right;
			quad.br.texCoords.v = bottom;
		}

		//サイズ設定
		//頂点をサイズに合わせて変形させる
		if (flags & PART_FLAG_SIZE_X)
		{
			float w = 0;
			float center = 0;
			w = (quad.tr.vertices.x - quad.tl.vertices.x) / 2.0f;
			if (w!= 0.0f)
			{
				center = quad.tl.vertices.x + w;
				float scale = (size_X / 2.0f) / w;

				quad.bl.vertices.x = center - (w * scale);
				quad.br.vertices.x = center + (w * scale);
				quad.tl.vertices.x = center - (w * scale);
				quad.tr.vertices.x = center + (w * scale);
			}
		}
		if (flags & PART_FLAG_SIZE_Y)
		{
			float h = 0;
			float center = 0;
			h = (quad.bl.vertices.y - quad.tl.vertices.y) / 2.0f;
			if (h != 0.0f)
			{
				center = quad.tl.vertices.y + h;
				float scale = (size_Y / 2.0f) / h;

				quad.bl.vertices.y = center - (h * scale);
				quad.br.vertices.y = center - (h * scale);
				quad.tl.vertices.y = center + (h * scale);
				quad.tr.vertices.y = center + (h * scale);
			}
		}
		// 頂点変形のオフセット値を反映
		if (flags & PART_FLAG_VERTEX_TRANSFORM)
		{
			int vt_flags = reader.readU16();
			if (vt_flags & VERTEX_FLAG_LT)
			{
				quad.tl.vertices.x += reader.readS16();
				quad.tl.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_RT)
			{
				quad.tr.vertices.x += reader.readS16();
				quad.tr.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_LB)
			{
				quad.bl.vertices.x += reader.readS16();
				quad.bl.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_RB)
			{
				quad.br.vertices.x += reader.readS16();
				quad.br.vertices.y += reader.readS16();
			}
		}
		
		
		//頂点情報の取得
		unsigned char alpha = (unsigned char)opacity;
		SSColor4B color4 = { 0xff, 0xff, 0xff, 0xff };

		color4.r = color4.r * _col_r / 255;
		color4.g = color4.g * _col_g / 255;
		color4.b = color4.b * _col_b / 255;

		quad.tl.colors =
		quad.tr.colors =
		quad.bl.colors =
		quad.br.colors = color4;


		// カラーブレンドの反映
		if (flags & PART_FLAG_COLOR_BLEND)
		{

			int typeAndFlags = reader.readU16();
			int funcNo = typeAndFlags & 0xff;
			int cb_flags = (typeAndFlags >> 8) & 0xff;
			float blend_rate = 1.0f;

			sprite->setColorBlendFunc(funcNo);
			sprite->_state.colorBlendFunc = funcNo;
			sprite->_state.colorBlendType = cb_flags;

			//ssbpではカラーブレンドのレート（％）は使用できません。
			//制限となります。
			if (cb_flags & VERTEX_FLAG_ONE)
			{
				blend_rate = reader.readFloat();
				reader.readColor(color4);


				color4.r = color4.r * _col_r / 255;
				color4.g = color4.g * _col_g / 255;
				color4.b = color4.b * _col_b / 255;
				color4.a = color4.a * alpha / 255;

				quad.tl.colors =
				quad.tr.colors =
				quad.bl.colors =
				quad.br.colors = color4;
			}
			else
			{
				if (cb_flags & VERTEX_FLAG_LT)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					quad.tl.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_RT)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					quad.tr.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_LB)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					quad.bl.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_RB)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					quad.br.colors = color4;
				}
			}
		}
		//uvスクロール
		if (flags & PART_FLAG_U_MOVE)
		{
			quad.tl.texCoords.u += uv_move_X;
			quad.tr.texCoords.u += uv_move_X;
			quad.bl.texCoords.u += uv_move_X;
			quad.br.texCoords.u += uv_move_X;
		}
		if (flags & PART_FLAG_V_MOVE)
		{
			quad.tl.texCoords.v += uv_move_Y;
			quad.tr.texCoords.v += uv_move_Y;
			quad.bl.texCoords.v += uv_move_Y;
			quad.br.texCoords.v += uv_move_Y;
		}


		float u_wide = 0;
		float v_height = 0;
		float u_center = 0;
		float v_center = 0;
		float u_code = 1;
		float v_code = 1;

		//UVを作成、反転の結果UVが反転する
		u_wide = (quad.tr.texCoords.u - quad.tl.texCoords.u) / 2.0f;
		u_center = quad.tl.texCoords.u + u_wide;
		if (flags & PART_FLAG_FLIP_H)
		{
			//左右反転を行う場合は符号を逆にする
			u_code = -1;
		}
		v_height = (quad.bl.texCoords.v - quad.tl.texCoords.v) / 2.0f;
		v_center = quad.tl.texCoords.v + v_height;
		if (flags & PART_FLAG_FLIP_V)
		{
			//上下反転を行う場合はテクスチャUVを逆にする
			v_code = -1;
		}
		//UV回転
		if (flags & PART_FLAG_UV_ROTATION)
		{
			//頂点位置を回転させる
			get_uv_rotation(&quad.tl.texCoords.u, &quad.tl.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.tr.texCoords.u, &quad.tr.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.bl.texCoords.u, &quad.bl.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.br.texCoords.u, &quad.br.texCoords.v, u_center, v_center, uv_rotation);
		}

		//UVスケール || 反転
		if ((flags & PART_FLAG_U_SCALE) || (flags & PART_FLAG_FLIP_H))
		{
			quad.tl.texCoords.u = u_center - (u_wide * uv_scale_X * u_code);
			quad.tr.texCoords.u = u_center + (u_wide * uv_scale_X * u_code);
			quad.bl.texCoords.u = u_center - (u_wide * uv_scale_X * u_code);
			quad.br.texCoords.u = u_center + (u_wide * uv_scale_X * u_code);
		}
		if ((flags & PART_FLAG_V_SCALE) || (flags & PART_FLAG_FLIP_V))
		{
			quad.tl.texCoords.v = v_center - (v_height * uv_scale_Y * v_code);
			quad.tr.texCoords.v = v_center - (v_height * uv_scale_Y * v_code);
			quad.bl.texCoords.v = v_center + (v_height * uv_scale_Y * v_code);
			quad.br.texCoords.v = v_center + (v_height * uv_scale_Y * v_code);
		}
		state.quad = quad;




		//インスタンスパーツの場合
		if (partData->type == PARTTYPE_INSTANCE)
		{
			bool overWrite;
			Instance keyParam;
			getInstanceParam(&overWrite, &keyParam);
			//描画
			int refKeyframe = 0;
			int refStartframe = 0;
			int refEndframe = 0;
			float refSpeed = 0;
			int refloopNum = 0;
			bool infinity = false;
			bool reverse = false;
			bool pingpong = false;
			bool independent = false;

			if (flags & PART_FLAG_INSTANCE_KEYFRAME)
			{
				refKeyframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_START)
			{
				refStartframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_END)
			{
				refEndframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_SPEED)
			{
				refSpeed = reader.readFloat();
			}
			if (flags & PART_FLAG_INSTANCE_LOOP)
			{
				refloopNum = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_LOOP_FLG)
			{
				int lflags = reader.readS16();
				if (lflags & INSTANCE_LOOP_FLAG_INFINITY )
				{
					//無限ループ
					infinity = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_REVERSE)
				{
					//逆再生
					reverse = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_PINGPONG)
				{
					//往復
					pingpong = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_INDEPENDENT)
				{
					//独立
					independent = true;
				}
			}
			//インスタンスパラメータを上書きする
			if (overWrite == true)
			{
				refStartframe = keyParam.refStartframe;		//開始フレーム
				refEndframe = keyParam.refEndframe;			//終了フレーム
				refSpeed = keyParam.refSpeed;				//再生速度
				refloopNum = keyParam.refloopNum;			//ループ回数
				infinity = keyParam.infinity;				//無限ループ
				reverse = keyParam.reverse;					//逆再選
				pingpong = keyParam.pingpong;				//往復
				independent = keyParam.independent;			//独立動作
			}

			//タイムライン上の時間 （絶対時間）
			int time = frameNo;

			//独立動作の場合
			if (independent)
			{
				float fdt = 1.0f / _gamefps;	//ゲームFPSからアニメーション時間を求める
				float delta = fdt / (1.0f / _animefps);						//	独立動作時は親アニメのfpsを使用する
//				float delta = fdt / (1.0f / sprite->_ssplayer->_animefps);

				sprite->_liveFrame += delta;
				time = (int)sprite->_liveFrame;
			}

			//このインスタンスが配置されたキーフレーム（絶対時間）
			int	selfTopKeyframe = refKeyframe;


			int	reftime = (int)((float)time * refSpeed) - selfTopKeyframe; //開始から現在の経過時間
			if (reftime < 0) continue;							//そもそも生存時間に存在していない

			int inst_scale = (refEndframe - refStartframe) + 1; //インスタンスの尺


			//尺が０もしくはマイナス（あり得ない
			if (inst_scale <= 0) continue;
			int	nowloop = (reftime / inst_scale);	//現在までのループ数

			int checkloopnum = refloopNum;

			//pingpongの場合では２倍にする
			if (pingpong) checkloopnum = checkloopnum * 2;

			//無限ループで無い時にループ数をチェック
			if (!infinity)   //無限フラグが有効な場合はチェックせず
			{
				if (nowloop >= checkloopnum)
				{
					reftime = inst_scale - 1;
					nowloop = checkloopnum - 1;
				}
			}

			int temp_frame = reftime % inst_scale;  //ループを加味しないインスタンスアニメ内のフレーム

			//参照位置を決める
			//現在の再生フレームの計算
			int _time = 0;
			if (pingpong && (nowloop % 2 == 1))
			{
				if (reverse)
				{
					reverse = false;//反転
				}
				else
				{
					reverse = true;//反転
				}
			}

			if (reverse)
			{
				//リバースの時
				_time = refEndframe - temp_frame;
			}
			else{
				//通常時
				_time = temp_frame + refStartframe;
			}

			//インスタンスパラメータを設定
			sprite->_ssplayer->set_InstanceAlpha(opacity);
			sprite->_ssplayer->set_InstanceRotation(rotationX, rotationY, rotationZ);
			sprite->_ssplayer->setColor(_col_r, _col_g, _col_b);

			//インスタンス用SSPlayerに再生フレームを設定する
			sprite->_ssplayer->setFrameNo(_time);
		}

		//スプライトステータスの保存
		sprite->setState(state);

	}


	// 親に変更があるときは自分も更新するようフラグを設定する
	for (int partIndex = 1; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
		
		if (parent->_isStateChanged)
		{
			sprite->_isStateChanged = true;
		}
	}

	// 行列の更新
	float mat[16];
	float t[16];
	for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));

		if (sprite->_isStateChanged)
		{
			if (partIndex > 0)
			{
				//親のマトリクスを適用
				CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
				memcpy(mat, parent->_mat, sizeof(float)* 16);
			}
			else
			{
				IdentityMatrix( mat );
				//親がいない場合、プレイヤーの値とインスタンスパーツの値を初期値とする
				TranslationMatrix(t, _state.x, _state.y, 0.0f);
				MultiplyMatrix(t, mat, mat);

				Matrix4RotationX(t, SSRadianToDegree(sprite->_state.instancerotationX ));
				MultiplyMatrix(t, mat, mat);

				Matrix4RotationY(t, SSRadianToDegree(sprite->_state.instancerotationY ));
				MultiplyMatrix(t, mat, mat);

				Matrix4RotationZ(t, SSRadianToDegree(sprite->_state.instancerotationZ ));
				MultiplyMatrix(t, mat, mat);

				//rootパーツはプレイヤーからステータスを引き継ぐ
				sprite->_state.rotationX += _state.rotationX + sprite->_state.instancerotationX;
				sprite->_state.rotationY += _state.rotationY + sprite->_state.instancerotationY;
				sprite->_state.rotationZ += _state.rotationZ + sprite->_state.instancerotationZ;
				sprite->_state.scaleX *= _state.scaleX;
				sprite->_state.scaleY *= _state.scaleY;
				if (_state.flipX == true)
				{
					//プレイヤーのXフリップ
					sprite->_state.scaleX = -sprite->_state.scaleX;	//フラグ反転
				}
				if (_state.flipY == true)
				{
					sprite->_state.scaleY = -sprite->_state.scaleY;	//フラグ反転
				}
			}
			TranslationMatrix(t, sprite->_state.x, sprite->_state.y, 0.0f);
			MultiplyMatrix(t, mat, mat);

			Matrix4RotationX(t, SSRadianToDegree(sprite->_state.rotationX));
			MultiplyMatrix(t, mat, mat);

			Matrix4RotationY(t, SSRadianToDegree(sprite->_state.rotationY));
			MultiplyMatrix(t, mat, mat);

			Matrix4RotationZ(t, SSRadianToDegree(sprite->_state.rotationZ));
			MultiplyMatrix(t, mat, mat);

			ScaleMatrix(t, sprite->_state.scaleX, sprite->_state.scaleY, 1.0f);
			MultiplyMatrix(t, mat, mat);

			memcpy(sprite->_mat, mat, sizeof(float)* 16);
			memcpy(sprite->_state.mat, mat, sizeof(float)* 16);

			if (partIndex > 0)
			{
				CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
				//子供は親のステータスを引き継ぐ
				//座標はマトリクスから取得する
				if ((parent->_state.scaleX * parent->_state.scaleY) < 0)	//スケールのどちらかが-の場合は回転方向を逆にする
				{
					sprite->_state.rotationZ = -sprite->_state.rotationZ;
				}
				sprite->_state.rotationX += parent->_state.rotationX;
				sprite->_state.rotationY += parent->_state.rotationY;
				sprite->_state.rotationZ += parent->_state.rotationZ;

				sprite->_state.scaleX *= parent->_state.scaleX;
				sprite->_state.scaleY *= parent->_state.scaleY;

				//ルートパーツのアルファ値を反映させる
				sprite->_state.opacity = (sprite->_state.opacity * _state.opacity * _InstanceAlpha) / 255 / 255;
				//インスタンスパーツの親を設定
				if (sprite->_ssplayer)
				{
					sprite->_ssplayer->setPosition(sprite->_mat[12], sprite->_mat[13]);
					sprite->_ssplayer->setScale(sprite->_state.scaleX, sprite->_state.scaleY);
					sprite->_ssplayer->setRotation(sprite->_state.rotationX, sprite->_state.rotationY, sprite->_state.rotationZ);
				}

			}
			//原点計算を行う
			float px = 0;
			float py = 0;
			float cx = ((sprite->_state.rect.size.width * sprite->_state.scaleX) * -(sprite->_state.pivotX - 0.5f));
#ifdef UP_MINUS
			float cy = ((sprite->_state.rect.size.height * sprite->_state.scaleY) * -(sprite->_state.pivotY - 0.5f));
#else
			float cy = ((sprite->_state.rect.size.height * sprite->_state.scaleY) * +(sprite->_state.pivotY - 0.5f));
#endif
			get_uv_rotation(&cx, &cy, 0, 0, sprite->_state.rotationZ);

			sprite->_state.mat[12] += cx;
			sprite->_state.mat[13] += cy;
			sprite->_isStateChanged = false;
		}
	}
	// エフェクトのアップデート
	for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));

		//エフェクトのアップデート
		if (sprite->refEffect)
		{
			sprite->refEffect->setParentSprite(sprite);
			if (sprite->_state.isVisibled == false)
			{
				//パーツが非表示の場合はエフェクトをリセットする
				if (sprite->refEffect->getPlayStatus() == true)
				{
					//毎回行うと負荷がかかるので、前回が再生中であればリセット
					sprite->refEffect->setSeed(getRandomSeed());
					sprite->refEffect->reload();
					sprite->refEffect->stop();
				}
			}
			else{
				//パーツのステータスの更新
				sprite->partState.alpha = sprite->_state.opacity / 255.0f;
				int matindex = 0;
				for (matindex = 0; matindex < 16; matindex++)
				{
					sprite->partState.matrix[matindex] = sprite->_mat[matindex];
				}

				//エフェクトアップデート
				if (frameNo != _prevDrawFrameNo)
				{
					sprite->refEffect->setLoop(false);
					int fdt = 1;
					if (_prevDrawFrameNo < frameNo)			//差分フレームを計算
					{
						fdt = frameNo - _prevDrawFrameNo;
						if (sprite->refEffect->getPlayStatus() == false)
						{
							sprite->refEffect->play();
							//前回エフェクトの更新をしていない場合は初回を0でアップデートする
							sprite->refEffect->update(0); //先頭フレームは0でアップデートする
							fdt = fdt - 1;
						}
					}
					else
					{
						//アニメーションループ時
						sprite->refEffect->setSeed(getRandomSeed());
						sprite->refEffect->reload();
						sprite->refEffect->play();
						sprite->refEffect->update(0); //先頭フレームは0でアップデートする
						if (frameNo > 0)
						{
							fdt = frameNo - 1;
						}
						else
						{
							fdt = 0;
						}

					}
					sprite->refEffect->play();
					int f = 0;
					for (f = 0; f < fdt; f++)
					{
						sprite->refEffect->update(1); //先頭から今のフレーム
					}
				}
			}
		}
	}
	_prevDrawFrameNo = frameNo;	//再生したフレームを保存
}

//プレイヤーの描画
void Player::draw()
{
	if (!_currentAnimeRef) return;

	ToPointer ptr(_currentRs->data);
	const AnimePackData* packData = _currentAnimeRef->animePackData;

	for (int index = 0; index < packData->numParts; index++)
	{
		int partIndex = _partIndex[index];
		//スプライトの表示
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		if (sprite->_ssplayer)
		{
			//インスタンスパーツの場合は子供のプレイヤーを再生
			sprite->_ssplayer->update(0);
			sprite->_ssplayer->draw();
		}
		else
		{
			if (sprite->refEffect)
			{ 
				//エフェクトパーツ
				sprite->refEffect->draw();
			}
			else
			{
				if ((sprite->_state.texture.handle != -1) && (sprite->_state.isVisibled == true))
				{
					SSDrawSprite(sprite->_state);
				}
			}
		}
	}
}

void Player::checkUserData(int frameNo)
{
	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const AnimationData* animeData = _currentAnimeRef->animationData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	if (!animeData->userData) return;
	const ss_offset* userDataIndex = static_cast<const ss_offset*>(ptr(animeData->userData));

	if (!userDataIndex[frameNo]) return;
	const ss_u16* userDataArray = static_cast<const ss_u16*>(ptr(userDataIndex[frameNo]));
	
	DataArrayReader reader(userDataArray);
	int numUserData = reader.readU16();

	for (int i = 0; i < numUserData; i++)
	{
		int flags = reader.readU16();
		int partIndex = reader.readU16();

		_userData.flags = 0;

		if (flags & UserData::FLAG_INTEGER)
		{
			_userData.flags |= UserData::FLAG_INTEGER;
			_userData.integer = reader.readS32();
		}
		else
		{
			_userData.integer = 0;
		}
		
		if (flags & UserData::FLAG_RECT)
		{
			_userData.flags |= UserData::FLAG_RECT;
			_userData.rect[0] = reader.readS32();
			_userData.rect[1] = reader.readS32();
			_userData.rect[2] = reader.readS32();
			_userData.rect[3] = reader.readS32();
		}
		else
		{
			_userData.rect[0] =
			_userData.rect[1] =
			_userData.rect[2] =
			_userData.rect[3] = 0;
		}
		
		if (flags & UserData::FLAG_POINT)
		{
			_userData.flags |= UserData::FLAG_POINT;
			_userData.point[0] = reader.readS32();
			_userData.point[1] = reader.readS32();
		}
		else
		{
			_userData.point[0] =
			_userData.point[1] = 0;
		}
		
		if (flags & UserData::FLAG_STRING)
		{
			_userData.flags |= UserData::FLAG_STRING;
			int size = reader.readU16();
			ss_offset offset = reader.readOffset();
			const char* str = static_cast<const char*>(ptr(offset));
			_userData.str = str;
			_userData.strSize = size;
		}
		else
		{
			_userData.str = 0;
			_userData.strSize = 0;
		}
		
		_userData.partName = static_cast<const char*>(ptr(parts[partIndex].name));
		_userData.frameNo = frameNo;
		
		SSonUserData(this, &_userData);
	}

}


//インスタンスパーツのアルファ値を設定
void  Player::set_InstanceAlpha(int alpha)
{
	_InstanceAlpha = alpha;
}

//インスタンスパーツの回転値を設定
void  Player::set_InstanceRotation(float rotX, float rotY, float rotZ)
{
	_InstanceRotX = rotX;
	_InstanceRotY = rotY;
	_InstanceRotZ = rotZ;
}


void  Player::setPosition(float x, float y)
{
	_state.x = x;
	_state.y = y;
}
void  Player::setRotation(float x, float y, float z)
{
	_state.rotationX = x;
	_state.rotationY = y;
	_state.rotationZ = z;
}

void  Player::setScale(float x, float y)
{
	_state.scaleX = x;
	_state.scaleY = y;
}

void  Player::setAlpha(int a)
{
	_state.opacity = a;
}

void  Player::setFlip(bool flipX, bool flipY)
{
	_state.flipX = flipX;
	_state.flipY = flipY;
}

void Player::setGameFPS(float fps)
{
	_gamefps = fps;
}


/**
 * CustomSprite
 */
 //カラーブレンド用のシェーダー処理は汎用的に使用する事ができないためすべてコメントにしてあります。
 //カラーブレンドを再現するための参考にしてください。

unsigned int CustomSprite::ssSelectorLocation = 0;
unsigned int CustomSprite::ssAlphaLocation = 0;
unsigned int CustomSprite::sshasPremultipliedAlpha = 0;

//static const GLchar * ssPositionTextureColor_frag =
//#include "ssShader_frag.h"

CustomSprite::CustomSprite():
//	: _defaultShaderProgram(NULL)
	  _useCustomShaderProgram(false)
	, _opacity(1.0f)
	, _colorBlendFuncNo(0)
	, _liveFrame(0.0f)
	, _hasPremultipliedAlpha(0)
	, refEffect(0)
	, _ssplayer(0)
{}

CustomSprite::~CustomSprite()
{}

/*
CCGLProgram* CustomSprite::getCustomShaderProgram()
{
	using namespace cocos2d;

	static CCGLProgram* p = NULL;
	static bool constructFailed = false;
	if (!p && !constructFailed)
	{
		p = new CCGLProgram();
		p->initWithVertexShaderByteArray(
			ccPositionTextureColor_vert,
			ssPositionTextureColor_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		if (!p->link())
		{
			constructFailed = true;
			return NULL;
		}
		
		p->updateUniforms();
		
		ssSelectorLocation = glGetUniformLocation(p->getProgram(), "u_selector");
		ssAlphaLocation = glGetUniformLocation(p->getProgram(), "u_alpha");
		sshasPremultipliedAlpha = glGetUniformLocation(p->getProgram(), "u_hasPremultipliedAlpha");
		if (ssSelectorLocation == GL_INVALID_VALUE
		 || ssAlphaLocation == GL_INVALID_VALUE)
		{
			delete p;
			p = NULL;
			constructFailed = true;
			return NULL;
		}

		glUniform1i(ssSelectorLocation, 0);
		glUniform1f(ssAlphaLocation, 1.0f);
		glUniform1i(sshasPremultipliedAlpha, 0);
	}
	return p;
}
*/

CustomSprite* CustomSprite::create()
{
	CustomSprite *pSprite = new CustomSprite();
	if (pSprite)
	{
		pSprite->initState();
//		pSprite->_defaultShaderProgram = pSprite->getShaderProgram();
//		pSprite->autorelease();
		return pSprite;
	}
	SS_SAFE_DELETE(pSprite);
	return NULL;
}

void CustomSprite::changeShaderProgram(bool useCustomShaderProgram)
{
/*
	if (useCustomShaderProgram != _useCustomShaderProgram)
	{
		if (useCustomShaderProgram)
		{
			CCGLProgram *shaderProgram = getCustomShaderProgram();
			if (shaderProgram == NULL)
			{
				// Not use custom shader.
				shaderProgram = _defaultShaderProgram;
				useCustomShaderProgram = false;
			}
			this->setShaderProgram(shaderProgram);
			_useCustomShaderProgram = useCustomShaderProgram;
		}
		else
		{
			this->setShaderProgram(_defaultShaderProgram);
			_useCustomShaderProgram = false;
		}
	}
*/
}

void CustomSprite::sethasPremultipliedAlpha(int PremultipliedAlpha)
{
	_hasPremultipliedAlpha = PremultipliedAlpha;
}

bool CustomSprite::isCustomShaderProgramEnabled() const
{
	return _useCustomShaderProgram;
}

void CustomSprite::setColorBlendFunc(int colorBlendFuncNo)
{
	_colorBlendFuncNo = colorBlendFuncNo;
}

SSV3F_C4B_T2F_Quad& CustomSprite::getAttributeRef()
{
	return _sQuad;
}

void CustomSprite::setOpacity(unsigned char opacity)
{
//	CCSprite::setOpacity(opacity);
	_opacity = static_cast<float>(opacity) / 255.0f;
}


#if 1
void CustomSprite::draw(void)
{
/*
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "SSSprite - draw");


	if (!_useCustomShaderProgram)
	{
		CCSprite::draw();
		return;
	}


	SS_ASSERT2(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	if (m_pobTexture != NULL)
	{
		ccGLBindTexture2D(m_pobTexture->getName());
	}
	else
	{
		ccGLBindTexture2D(0);
	}

	glUniform1i(ssSelectorLocation, _colorBlendFuncNo);
	glUniform1f(ssAlphaLocation, _opacity);
	glUniform1i(sshasPremultipliedAlpha, _hasPremultipliedAlpha);

	//
	// Attributes
	//

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof(ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4] = {
		ccp(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

	CC_INCREMENT_GL_DRAWS(1);

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
*/
}
#endif

void CustomSprite::setFlippedX(bool flip)
{
	_flipX = flip;
}
void CustomSprite::setFlippedY(bool flip)
{
	_flipY = flip;
}
bool CustomSprite::isFlippedX()
{
	return (_flipX);
}
bool CustomSprite::isFlippedY()
{
	return (_flipY);
}


};
