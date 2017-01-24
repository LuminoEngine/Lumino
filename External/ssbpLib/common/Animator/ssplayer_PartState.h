#ifndef __SSPLAYER_PARTSTATE__
#define __SSPLAYER_PARTSTATE__

//#include "../loader/ssloader.h"
//#include "../Helper/ssHelper.h"
namespace ss
{


class SsAnimeDecoder;
class SsEffectRenderer;


///パーツの状態を保持するクラスです。
struct SsPartState
{
	float			vertices[3 * 5];	///< 座標
	float			colors[4 * 4];		///< カラー (４頂点分）
	float			uvs[2 * 5];			///< UV		(４隅＋中央)
	float			matrix[4 * 4];		///< 行列

	SsBlendType::_enum	alphaBlendType;
	float			alpha;			///< 不透明度 0~1

	SsPartState();

	virtual ~SsPartState();
	void	destroy();
	void	init();
//	bool	inherits_(SsAttributeKind::_enum kind) const {return inheritRates[(int)kind] != 0.f;}

};

};

#endif
