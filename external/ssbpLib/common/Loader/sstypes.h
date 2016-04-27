#ifndef __SSTYPES__
#define __SSTYPES__

#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

namespace ss
{

//===============================================================
//Macros 
//===============================================================
#define	SS_DECLARE_ENUM_STRING_DEF(type) \
	SsString	__EnumToString_( type::_enum n );\
	void	__StringToEnum_( SsString n , type::_enum& out);\




//===============================================================
// Declare Type
//===============================================================

//文字列の設定
typedef std::string SsString;



///２次元座標を表現するためのクラスです。
struct SsPoint2
{
public:
	float	x;
	float	y;

public:
	SsPoint2( float _x , float _y )
	{
		x = _x ; y = _y;
	}
	SsPoint2() : x(0) , y(0){}

	static	float	distance_sq(const SsPoint2 & l, const SsPoint2 & r)
	{
		float x = l.x - r.x;
		float y = l.y - r.y;
		float sq = x * x + y * y;
		return sq;		
	}

	static	float	distance(const SsPoint2 & l, const SsPoint2 & r)
	{
		return  sqrt( distance_sq(l, r) );
	}

	SsPoint2 operator +( const SsPoint2& r) const
	{
		return SsPoint2(x + r.x, y + r.y);
	}

	SsPoint2 operator -( const SsPoint2& r) const
	{
		return SsPoint2(x - r.x, y - r.y);
	}

	SsPoint2 operator *( float r) const
	{
		return SsPoint2(x * r, y * r);
	}

	SsPoint2 operator /( float r) const
	{
		return SsPoint2(x / r, y / r);
	}

	float	length_sq() const
	{
		return (x * x) + (y * y);
	}

	float	length() const
	{
#ifdef _WIN32
		float r = length_sq();
		if ( r < 0.0001f && r > -0.0001f ) return 0;

		return (float)std::sqrtf( r );
#else
        return (float)sqrt( length_sq() );
#endif
	}

	static void	normalize(const SsPoint2& in, SsPoint2* out)
	{

		float len = in.length();
		float div = 0;

		if ( len == 0 )
		{
			div = 0;
		}else{
			div = (float)1 / in.length();
		}

		out->x = in.x * div;
		out->y = in.y * div;
	}

	void		normalize()
	{
		normalize(*this, this);
	}

	//----------------------------------------------------------------------------
	static	float	dot(const SsPoint2& l, const SsPoint2 r)
	{
		return (l.x * r.x) + (l.y * r.y);
	}
	static	float	cross(const SsPoint2& l, const SsPoint2& r)
	{
		return (l.x * r.y) - (l.y * r.x);
	}

	//----------------------------------------------------------------------------
	/**
		２つのベクトルが織りなす角度を求める
		入力は単位ベクトルでなくてはいけない。
	*/
	//----------------------------------------------------------------------------
	static	float 	get_angle_unit(const SsPoint2& v0, const SsPoint2 v1)
	{
		float ip = dot(v0, v1);
		if (ip > 1.0f) ip = 1.0f;
		if (ip < -1.0f) ip = -1.0f;
		float f = acos(ip);
		return f;		
		
	}
	//----------------------------------------------------------------------------
	/**
		２つのベクトルが織りなす角度を求める
	*/
	//----------------------------------------------------------------------------
	static	float 	get_angle(const SsPoint2& v0, const SsPoint2& v1)
	{
		SsPoint2 uv0(v0), uv1(v1);
		uv0.normalize();
		uv1.normalize();
		return get_angle_unit(uv0, uv1);
	}

	// v0 から v1 への左回りの角度を返す
	static	float 	get_angle_360_unit(const SsPoint2& v0, const SsPoint2 v1)
	{
		float ang = get_angle_unit(v0, v1);
		float c = cross(v0, v1);

		if (c < 0)
		{
			ang = (3.1415926535897932385f)*2.0f - ang;
		}
		return ang;
	}

	static	float 	get_angle_360(const SsPoint2& v0, const SsPoint2 v1)
	{
		SsPoint2 uv0(v0), uv1(v1);
		uv0.normalize();
		uv1.normalize();
		return get_angle_360_unit(uv0, uv1);
	}

};

///３次元座標を表現するためのクラスです。
struct SsPoint3
{
public:
	float	x;
	float	y;
	float	z;

public:
	SsPoint3( float _x , float _y , float _z)
	{
		x = _x ; y = _y;z = _z;
	}
	SsPoint3() : x(0) , y(0) ,z(0){}
};

typedef SsPoint2 SsVector2;
typedef SsPoint3 SsVector3;
typedef unsigned int u32;
typedef unsigned char u8;




/// 矩形
template <typename T>
class SsTRect
{
public:
	T	x, y, w, h;

	SsTRect(): x(0), y(0), w(0), h(0)  {}
	SsTRect(T ax, T ay, T aw, T ah): x(ax), y(ay), w(aw), h(ah) {}
	SsTRect(const SsTRect& r): x(r.x), y(r.y), w(r.w), h(r.h) {}

	bool	operator ==(const SsTRect& r) const {return x == r.x && y == r.y && w == r.w && h == r.h;}
	bool	operator !=(const SsTRect& r) const {return !(*this == r);}
private:
};


typedef SsTRect<int>	SsIRect;


///カラー値を定義するテンプレートクラスです。
template <typename T>
class SsTColor
{
public:
	T	r, g, b, a;

	SsTColor(): r(0), g(0), b(0), a(0) {}
	SsTColor(T ar, T ag, T ab, T aa): r(ar), g(ag), b(ab), a(aa) {}
	SsTColor(const SsTColor& s): r(s.r), g(s.g), b(s.b), a(s.a) {}

	void	fromARGB(u32 c);
	void	fromBGRA(u32 c);

	u32		toARGB() const;

	bool	operator ==(const SsTColor& rhs) const
	{
		return r == rhs.r
			&& g == rhs.g
			&& b == rhs.b
			&& a == rhs.a;
	}

private:
};


/// rgba 小数版
template<> inline SsTColor<float>::SsTColor(): r(0.5f), g(0.5f), b(0.5f), a(1.f) {}
template<> inline void SsTColor<float>::fromARGB(u32 c)
{
	a = (float)(c >> 24) / 255.f;
	r = (float)((c >> 16) & 0xff) / 255.f;
	g = (float)((c >> 8) & 0xff) / 255.f;
	b = (float)(c & 0xff) / 255.f;
}
template<> inline void SsTColor<float>::fromBGRA(u32 c)
{
	b = (float)(c >> 24) / 255.f;
	g = (float)((c >> 16) & 0xff) / 255.f;
	r = (float)((c >> 8) & 0xff) / 255.f;
	a = (float)(c & 0xff) / 255.f;
}
template<> inline u32 SsTColor<float>::toARGB() const
{
	u32 c = (u8)(a * 255) << 24 | (u8)(r * 255) << 16 | (u8)(g * 255) << 8 | (u8)(b * 255);
	return c;
}




template<> inline SsTColor<u32>::SsTColor(): r(255), g(255), b(255), a(255) {}
template<> inline void SsTColor<u32>::fromARGB(u32 c)
{
	a = (c >> 24);
	r = ((c >> 16) & 0xff);
	g = ((c >> 8) & 0xff);
	b = (c & 0xff);
}
template<> inline void SsTColor<u32>::fromBGRA(u32 c)
{
	b = (c >> 24) ;
	g = ((c >> 16) & 0xff) ;
	r = ((c >> 8) & 0xff) ;
	a = (c & 0xff) ;
}
template<> inline u32 SsTColor<u32>::toARGB() const
{
	u32 c = (u8)(a) << 24 | (u8)(r) << 16 | (u8)(g) << 8 | (u8)(b);
	return c;
}



template<> inline SsTColor<u8>::SsTColor(): r(255), g(255), b(255), a(255) {}
template<> inline void SsTColor<u8>::fromARGB(u32 c)
{
	a = (c >> 24);
	r = ((c >> 16) & 0xff);
	g = ((c >> 8) & 0xff);
	b = (c & 0xff);
}
template<> inline void SsTColor<u8>::fromBGRA(u32 c)
{
	b = (c >> 24) ;
	g = ((c >> 16) & 0xff) ;
	r = ((c >> 8) & 0xff) ;
	a = (c & 0xff) ;
}
template<> inline u32 SsTColor<u8>::toARGB() const
{
	u32 c = (u8)(a) << 24 | (u8)(r) << 16 | (u8)(g) << 8 | (u8)(b);
	return c;
}


///floatでのカラー値定義
typedef SsTColor<float> SsFColor;


///unsigned intでのカラー値定義
typedef SsTColor<u32> SsColor;


typedef SsTColor<u8> SsU8Color;

struct ToLower {
    char operator()(char c) { return tolower(c); }
};
///与えられた文字列をカラー値に変換するための関数
inline void	ConvertStringToSsColor( const std::string& str , SsColor& out)
{
	char *endptr;
	unsigned long x;

	std::string temp = "0x";
	temp+=str;
	
	transform(temp.begin(), temp.end(), temp.begin(), ToLower());
	x = strtoul(temp.c_str(), &endptr, 16);
	out.fromARGB( x );
}



/// 曲線補間計算用パラメータ
class SsCurve
{
public:
	float	startTime;		///< 始点キーの時間から制御点の時間へのオフセット値。Ｘ軸に当たる。
	float	startValue;		///< 始点キーの値から	〃	。Ｙ軸	〃
	float	endTime;		///< 終点キーの時間から制御点の時間へのオフセット値。Ｘ軸に当たる。
	float	endValue;		///< 終点キーの値から	〃	。Ｙ軸	〃

	float	startKeyTime;	///< [ワークパラメータ] 始点キーの時間 計算時のみ使用
	float	endKeyTime;		///< [ワークパラメータ] 終点キーの時間 計算時のみ使用

	bool	syncStartEnd;	///< [編集用パラメータ]カーブエディタでの編集時に始点・終点ハンドルを同期して動かすか？

	SsCurve() : startTime(0.f), startValue(0.f), endTime(0.f), endValue(0.f), startKeyTime(0.f), endKeyTime(0.f){}
	~SsCurve(){}

};



//---------------------------------------------------------------
/// ソートモード
namespace SsPartsSortMode
{
	enum _enum
	{
		invalid = -1, 
		prio,			///< 描画順は優先度で制御する。優先度を表示し、Ｚ座標を隠す。
		z,				///< 描画順はＺ座標で制御する。Ｚ座標を表示し、優先度を隠す。
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsPartsSortMode );

//---------------------------------------------------------------
/// Animation Part Type
namespace SsPartType
{
	enum _enum
	{
		invalid = -1,
		null,			///< null。領域を持たずSRT情報のみ。ただし円形の当たり判定は設定可能。
		normal,			///< 通常パーツ。領域を持つ。画像は無くてもいい。
		text,			///< テキスト(予約　未実装）
		instance,		///< インスタンス。他アニメ、パーツへの参照。シーン編集モードの代替になるもの
		effect,
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsPartType );


//---------------------------------------------------------------
/// 当たり判定形状
namespace SsBoundsType
{
	enum _enum 
	{
		invalid = -1,
		none,			///< 当たり判定として使わない。
		quad,			///< 自在に変形する四辺形。頂点変形など適用後の４角を結んだ領域。最も重い。
		aabb,			///< 回転しない全体を囲む矩形で交差判定
		circle,			///< 真円の半径で距離により判定する
		circle_smin,	///< 真円の半径で距離により判定する (スケールはx,yの最小値をとる）
		circle_smax,	///< 真円の半径で距離により判定する (スケールはx,yの最大値をとる）
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsBoundsType );


//---------------------------------------------------------------
/// 継承タイプ
namespace SsInheritType
{
	enum _enum
	{
		invalid = -1,
		parent,			///< 親の継承方法をそのまま引き継ぐ
		self,			///< 自身がアトリビュート別に持つ継承方法を使う
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsInheritType );

//---------------------------------------------------------------
/// ブレンドタイプ
namespace SsBlendType
{
	enum _enum{
		invalid=-1,
		mix,			///< 0 ブレンド（ミックス）
		mul,			///< 1 乗算
		add,			///< 2 加算
		sub,			///< 3 減算
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsBlendType );


///カラーブレンドキーが使用されている際のカラー適用範囲の定義
namespace SsColorBlendTarget
{
	enum _enum{
		invalid = -1,
		whole,	///< 単色。全体にかける。
		vertex,	///< 頂点単位
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsColorBlendTarget );



///補間モードの定義
namespace SsInterpolationType
{
	enum _enum 
	{
		invalid = -1,
		none,			///< なし
		linear,			///< 線形
		hermite,		///< エルミート
		bezier,			///< ベジェ
		acceleration,	///< 加速度
		deceleration,	///< 減速度
		num,
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsInterpolationType );


/// テクスチャラップモード
namespace SsTexWrapMode
{
	enum _enum
	{
		invalid = -1,	/// なし
		clamp,			/// クランプする
		repeat,			/// リピート
		mirror,			/// ミラー
		num
	};
};

SS_DECLARE_ENUM_STRING_DEF(SsTexWrapMode);

/// テクスチャフィルターモード 画素補間方法
namespace SsTexFilterMode
{
	enum _enum
	{
		invalid = -1,
		nearlest,	///< ニアレストネイバー
		linear,		///< リニア、バイリニア
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF(SsTexFilterMode);




/// アトリビュートの種類
namespace SsAttributeKind
{
	enum _enum
	{
		invalid=-1,	///< 無効値。旧データからの変換時など
		cell=0,		///< [CELL]参照セル
		posx,		///< [POSX]位置.X
		posy,		///< [POSY]位置.Y
		posz,		///< [POSZ]位置.Z
		rotx,		///< [ROTX]回転.X
		roty,		///< [ROTY]回転.Y
		rotz,		///< [ROTZ]回転.Z
		sclx,		///< [SCLX]スケール.X
		scly,		///< [SCLY]スケール.Y
		alpha,		///< [ALPH]不透明度
		prio,		///< [PRIO]優先度
		fliph,		///< [FLPH]左右反転(セルの原点を軸にする)
		flipv,		///< [FLPV]上下反転(セルの原点を軸にする)
		hide,		///< [HIDE]非表示
		color,		///< [VCOL]カラーブレンド
		vertex,		///< [VERT]頂点変形
		pivotx,		///< [PVTX]原点オフセット.X
		pivoty,		///< [PVTY]原点オフセット.Y
		anchorx,	///< [ANCX]アンカーポイント.X
		anchory,	///< [ANCY]アンカーポイント.Y
		sizex,		///< [SIZX]表示サイズ.X
		sizey,		///< [SIZY]表示サイズ.Y
		imgfliph,	///< [IFLH]イメージ左右反転(常にイメージの中央を原点とする)
		imgflipv,	///< [IFLV]イメージ上下反転(常にイメージの中央を原点とする)
		uvtx,		///< [UVTX]UVアニメ.移動.X
		uvty,		///< [UVTY]UVアニメ.移動.Y
		uvrz,		///< [UVRZ]UVアニメ.回転
		uvsx,		///< [UVSX]UVアニメ.スケール.X
		uvsy,		///< [UVSY]UVアニメ.スケール.Y
		boundr,		///< [BNDR]当たり判定用の半径
		user,		///< [USER]ユーザーデータ
		instance,	///< [IPRM]インスタンスパーツパラメータ
		num,
	};
};


SS_DECLARE_ENUM_STRING_DEF(SsAttributeKind);

namespace SsKeyValueType
{
	enum _enum
	{
		_unkown = -1,
		_bool = 0,
		_float,
		_int,
		_string,
		_cellmap,
		_vertexAnime,
		_colorAnime,
		_userData,
		_instance,
	};
};






///カラーブレンドキーのカラー値
struct SsColorBlendValue
{
	SsColor		rgba;	///カラー値
	float		rate;	///反映率

	SsColorBlendValue(): rate(0){}

};


///頂点変形キーの４頂点変形値
struct SsVertexAnime
{
	SsPoint2	offsets[4];	///< 各頂点の移動オフセット値
	SsPoint2&	getOffsets(int index){ return offsets[index];}
};




class ISSTexture;
class SsCell;



///カラーブレンド使用時のブレンドタイプとカラー値
struct SsColorAnime
{
	SsColorBlendTarget::_enum	target;		//ブレンドの適用方法  単色(全体) , 頂点単位 
	SsBlendType::_enum			blendType;	//ブレンド種別 (mix　乗算　加算　減算）
	SsColorBlendValue			color;		//単色。全体の場合に使用されるカラー値
	SsColorBlendValue			colors[4];	//頂点単位の場合使用されるカラー値

	SsColorBlendValue&			getColors(int index){ return colors[index];}
	int							getTargetToInt(){ return (int)target;}
	int							getBlendTypeToInt(){ return (int)blendType;}
	SsColorAnime() : 
			target( SsColorBlendTarget::invalid ) ,
				blendType( SsBlendType::invalid ){}

};


//エフェクト関連の定義
//エフェクトのノードタイプ
namespace SsEffectNodeType
{
	enum _enum{
		invalid=-1,
		root,
		emmiter,
		particle,
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsEffectNodeType );



//エフェクト関連の定義
//エフェクトのブレンドタイプ
namespace SsRenderBlendType
{
	enum _enum{
		invalid=-1,
		Mix,
		Add,
		num
	};
};
SS_DECLARE_ENUM_STRING_DEF( SsRenderBlendType );





//参照セル値
struct SsRefCell
{
	int			mapid;
	std::string	name;

	
};
class SsUserDataAnime
{
public:
	bool			useInteger;	///<整数が使用されているか
	bool			usePoint;	///<座標データが使用されているか
	bool			useRect;	///<矩形データが使用されているか
	bool			useString;	///<文字列データが使用されているか 

	int				integer;	///< 整数
	SsPoint2		point;		///< 座標
	SsIRect			rect;		///< 矩形
	SsString		string;		///< 文字列

	SsUserDataAnime() : 
		useInteger(false),
		usePoint(false),
		useRect(false),
		useString(false){}
};

class SsInstanceAttr
{
public:
    bool   		infinity;		///無限ループフラグ
    bool   		reverse;		///逆再生フラグ
    bool   		pingpong;		///往復再生フラグ
	bool		independent;	///独立動作フラグ
    int			loopNum;		///ループ回数　無限ループフラグ=trueの時には無効
    SsString	startLabel;		///再生開始位置 ラベル名称
    int			startOffset;	///再生開始位置 ラベル名称からのオフセット
    SsString	endLabel;		///再生終了位置 ラベル名称
    int			endOffset;		///再生終了位置 ラベル名称からのオフセット
    float       speed;			///再生スピード
    int			startFrame;		///ラベル位置とオフセット位置を加えた実際のフレーム数
    int			endFrame;		///ラベル位置とオフセット位置を加えた実際のフレーム数


    //テンポラリ <-エディタ用計算値の可能性もあるので後で整理
    int         curKeyframe; //この値があるキーフレーム値 (計算値）
    float		liveFrame;	//再生時間の累積

	SsInstanceAttr():
    	infinity( false ),
    	reverse( false ),
    	pingpong( false ),
    	independent( false ),
		loopNum( 1 ),
     	startLabel("_start"),
     	startOffset(0),
     	endLabel("_end"),
     	endOffset(0),
		curKeyframe( 0 ),
		speed(1.0f),
		startFrame(0),
		endFrame(0),
		liveFrame(0.0f)
   {}

};
};

#endif
