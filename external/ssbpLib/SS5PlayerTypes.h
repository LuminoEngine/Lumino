/**
*  SS5PlayerTypes.h
*/

#ifndef SS5PlayerTypes_h
#define SS5PlayerTypes_h

#include <float.h>
#include <math.h>
#include <assert.h>

namespace ss
{
class SSSize;
class SSPoint;


#ifdef _DEBUG
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)    assert(cond)
	#define SS_ASSERT2(cond, msg) SS_ASSERT(cond)
	#define SSLOGERROR(format,...)  do {} while (0)
#else
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)
	#define SS_ASSERT2(cond, msg) ((void)(cond))
	#define SSLOGERROR(format,...)  do {} while (0)
#endif

/**
* 座標クラス
*/
class SSPoint
{
public:
	float x;
	float y;

public:
	SSPoint(void) : x(0), y(0)
	{
	}

	SSPoint(float x, float y) : x(x), y(y)
	{
	}

	SSPoint(const SSPoint& other) : x(other.x), y(other.y)
	{
	}
/*
	SSPoint(const SSSize& size) : x(size.width), y(size.height)
	{
	}
*/
	SSPoint& operator= (const SSPoint& other)
	{
		setPoint(other.x, other.y);
		return *this;
	}
/*
	SSPoint& operator= (const SSSize& size)
	{
		setPoint(size.width, size.height);
		return *this;
	}
*/
	SSPoint operator+(const SSPoint& right) const
	{
		return SSPoint(this->x + right.x, this->y + right.y);
	}

	SSPoint operator-(const SSPoint& right) const
	{
		return SSPoint(this->x - right.x, this->y - right.y);
	}

	SSPoint operator-() const
	{
		return SSPoint(-x, -y);
	}

	SSPoint operator*(float a) const
	{
		return SSPoint(this->x * a, this->y * a);
	}

	SSPoint operator/(float a) const
	{
		SS_ASSERT2(a, "SSPoint division by 0.");
		return SSPoint(this->x / a, this->y / a);
	}

	void setPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	bool equals(const SSPoint& target) const
	{
		return (fabs(this->x - target.x) < FLT_EPSILON)
			&& (fabs(this->y - target.y) < FLT_EPSILON);
	}

	bool fuzzyEquals(const SSPoint& b, float var) const
	{
		if (x - var <= b.x && b.x <= x + var)
			if (y - var <= b.y && b.y <= y + var)
				return true;
		return false;
	}
/*
	float getAngle(const SSPoint& other) const
	{
		SSPoint a2 = normalize();
		SSPoint b2 = other.normalize();
		float angle = atan2f(a2.cross(b2), a2.dot(b2));
		if (fabs(angle) < FLT_EPSILON) return 0.f;
		return angle;
	}

	SSPoint rotateByAngle(const SSPoint& pivot, float angle) const
	{
		return pivot + (*this - pivot).rotate(SSPoint::forAngle(angle));
	}
*/

	inline float getLength() const {
		return sqrtf(x*x + y*y);
	};
	inline float getLengthSq() const {
		return dot(*this); //x*x + y*y;
	};
	inline float getDistanceSq(const SSPoint& other) const {
		return (*this - other).getLengthSq();
	};
	inline float getDistance(const SSPoint& other) const {
		return (*this - other).getLength();
	};
	inline float getAngle() const {
		return atan2f(y, x);
	};
	float getAngle(const SSPoint& other) const;
	inline float dot(const SSPoint& other) const {
		return x*other.x + y*other.y;
	};
	inline float cross(const SSPoint& other) const {
		return x*other.y - y*other.x;
	};
	inline SSPoint getPerp() const {
		return SSPoint(-y, x);
	};
	inline SSPoint getRPerp() const {
		return SSPoint(y, -x);
	};
	inline SSPoint project(const SSPoint& other) const {
		return other * (dot(other) / other.dot(other));
	};
	inline SSPoint rotate(const SSPoint& other) const {
		return SSPoint(x*other.x - y*other.y, x*other.y + y*other.x);
	};
	inline SSPoint unrotate(const SSPoint& other) const {
		return SSPoint(x*other.x + y*other.y, y*other.x - x*other.y);
	};
	inline SSPoint normalize() const {
		float length = getLength();
		if (length == 0.) return SSPoint(1.f, 0);
		return *this / getLength();
	};
	inline SSPoint lerp(const SSPoint& other, float alpha) const {
		return *this * (1.f - alpha) + other * alpha;
	};
	SSPoint rotateByAngle(const SSPoint& pivot, float angle) const;

	static inline SSPoint forAngle(const float a)
	{
		return SSPoint(cosf(a), sinf(a));
	}
};

/**
* サイズクラス
*/
class SSSize
{
public:
	float width;
	float height;

public:
	SSSize(void) : width(0), height(0)
	{
	}

	SSSize(float width, float height) : width(width), height(height)
	{
	}

	SSSize(const SSSize& other) : width(other.width), height(other.height)
	{
	}

	SSSize(const SSPoint& point) : width(point.x), height(point.y)
	{
	}

	SSSize& operator= (const SSSize& other)
	{
		setSize(other.width, other.height);
		return *this;
	}

	SSSize& operator= (const SSPoint& point)
	{
		setSize(point.x, point.y);
		return *this;
	}

	SSSize operator+(const SSSize& right) const
	{
		return SSSize(this->width + right.width, this->height + right.height);
	}

	SSSize operator-(const SSSize& right) const
	{
		return SSSize(this->width - right.width, this->height - right.height);
	}

	SSSize operator*(float a) const
	{
		return SSSize(this->width * a, this->height * a);
	}

	SSSize operator/(float a) const
	{
		SS_ASSERT2(a, "SSSize division by 0.");
		return SSSize(this->width / a, this->height / a);
	}

	void setSize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	bool equals(const SSSize& target) const
	{
		return (fabs(this->width - target.width)  < FLT_EPSILON)
			&& (fabs(this->height - target.height) < FLT_EPSILON);
	}
};

/**
* 矩形クラス
*/
class SSRect
{
public:
	SSPoint origin;
	SSSize  size;

public:

	SSRect(void)
	{
		setRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	SSRect(float x, float y, float width, float height)
	{
		setRect(x, y, width, height);
	}

	SSRect(const SSRect& other)
	{
		setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
	}

	SSRect& operator= (const SSRect& other)
	{
		setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
		return *this;
	}

	void setRect(float x, float y, float width, float height)
	{
		// CGRect can support width<0 or height<0
		SS_ASSERT2(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

		origin.x = x;
		origin.y = y;

		size.width = width;
		size.height = height;
	}

	bool equals(const SSRect& rect) const
	{
		return (origin.equals(rect.origin) &&
			size.equals(rect.size));
	}

	float getMaxX() const
	{
		return (float)(origin.x + size.width);
	}

	float getMidX() const
	{
		return (float)(origin.x + size.width / 2.0);
	}

	float getMinX() const
	{
		return origin.x;
	}

	float getMaxY() const
	{
		return origin.y + size.height;
	}

	float getMidY() const
	{
		return (float)(origin.y + size.height / 2.0);
	}

	float getMinY() const
	{
		return origin.y;
	}

	bool containsPoint(const SSPoint& point) const
	{
		bool bRet = false;

		if (point.x >= getMinX() && point.x <= getMaxX()
			&& point.y >= getMinY() && point.y <= getMaxY())
		{
			bRet = true;
		}

		return bRet;
	}

	bool intersectsRect(const SSRect& rect) const
	{
		return !(getMaxX() < rect.getMinX() ||
			rect.getMaxX() <      getMinX() ||
			getMaxY() < rect.getMinY() ||
			rect.getMaxY() <      getMinY());
	}

};
	
/**
* カラー構造体
*/
typedef struct _SSColor4B
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} SSColor4B;

/**
* 頂点座標構造体
*/
typedef struct _SSVertex3F
{
	float x;
	float y;
	float z;
} SSVertex3F;

/**
* ４頂点座標構造体
*/
typedef struct _SSQuad3 {
	SSVertex3F        bl;
	SSVertex3F        br;
	SSVertex3F        tl;
	SSVertex3F        tr;
} SSQuad3;

/**
* UV構造体
*/
typedef struct _SSTex2F {
	float u;
	float v;
} SSTex2F;

/**
* 頂点構造体
*/
typedef struct _ccV3F_C4B_T2F
{
	SSVertex3F			vertices;	/// 座標
	SSColor4B			colors;		/// カラー
	SSTex2F				texCoords;	/// UV
} SSV3F_C4B_T2F;

/**
* ４頂点構造体
*/
typedef struct _SSV3F_C4B_T2F_Quad
{
	SSV3F_C4B_T2F    tl;
	SSV3F_C4B_T2F    bl;
	SSV3F_C4B_T2F    tr;
	SSV3F_C4B_T2F    br;
} SSV3F_C4B_T2F_Quad;

//テクスチャデータ
struct TextuerData
{
	long handle;
	int size_w;
	int size_h;
};


};	// namespace ss

#endif
