
#pragma once
#include <Lumino/Math/Vector3.h>

LN_NAMESPACE_BEGIN

/**
	@brief	2次元上の点を表します。
*/
template<typename T>
struct GenericPoint
{
public:
	static const GenericPoint<T>	Zero;	/**< (0, 0) */

public:
	T		x;			/**< X 座標 */
	T		y;			/**< Y 座標 */

public:

	/** すべての要素を 0 で初期化します。*/
	GenericPoint() { set(0, 0); }
	
	/** 各要素を指定して初期化します。*/
	GenericPoint(T x, T y) { set(x, y); }
	
	/** 各要素を設定します。 */
	void set(T x_, T y_) { x = x_; y = y_; }

	/** 要素がすべて 0 であるかを判定します。*/
	bool isZero() const { return (x == 0 && y == 0); }

public:
	bool operator == (const GenericPoint<T>& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const GenericPoint<T>& obj) const { return !operator==(obj); }
};

//using PointF = GenericPoint<float>;
using PointI = GenericPoint<int>;


struct PointF : public GenericPoint<float>
{
public:
	PointF() {}
	PointF(const Vector2& v) { set(v.x, v.y); }
	PointF(float x, float y) { set(x, y); }
	operator const Vector2&() const { return *reinterpret_cast<const Vector2*>(this); }
};

/** 2次元上のオブジェクトサイズを表します。*/
LN_STRUCT()
struct Size
{
public:
	static const Size	Zero;		/**< (0, 0) */
	static const Size	MinValue;	/**< */
	static const Size	MaxValue;	/**< */

public:

	/** 横幅 */
	LN_FIELD()
	float width;

	/** 縦幅 */
	LN_FIELD()
	float height;

public:

	/** すべての要素を 0 で初期化します。*/
	Size() { set(0, 0); }

	/** 幅と高さを指定して初期化します。*/
	Size(float w, float h) { set(w, h); }

	/** 各要素を設定します。*/
	void set(float w, float h) { width = w; height = h; }

	/** 要素がすべて 0 かを判定します。*/
	bool isZero() const { return (width == 0 && height == 0); }

	/** いずれかの要素が 0 かを判定します。*/
	bool isAnyZero() const { return (width == 0 || height == 0); }

public:
	static Size min(const Size& size1, const Size& size2)
	{
		return Size(
			(size1.width < size2.width) ? size1.width : size2.width,
			(size1.height < size2.height) ? size1.height : size2.height);
	}

	static Size max(const Size& size1, const Size& size2)
	{
		return Size(
			(size1.width > size2.width) ? size1.width : size2.width,
			(size1.height > size2.height) ? size1.height : size2.height);
	}

public:
	bool operator == (const Size& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const Size& obj) const { return !operator==(obj); }
};

// 内部用
struct SizeI
{
public:
	static const SizeI	Zero;

	int		width;
	int		height;

	SizeI() { set(0, 0); }
	SizeI(int w, int h) { set(w, h); }
	
	void set(int w, int h) { width = w; height = h; }
	bool isZero() const { return (width == 0 && height == 0); }
	bool isAnyZero() const { return (width == 0 || height == 0); }

	Size toFloatSize() const { return Size((float)width, (float)height); }
	static SizeI fromFloatSize(const Size& size);

	bool operator == (const SizeI& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const SizeI& obj) const { return !operator==(obj); }
};



class GeometryStructsHelper
{
public:

	template<typename TRect>
	static void clip(TRect* this_, const TRect& rect)
	{
		auto l = (this_->x < rect.x) ? rect.x : this_->x;
		auto t = (this_->y < rect.y) ? rect.y : this_->y;
		auto r = (this_->getRight() > rect.getRight()) ? rect.getRight() : this_->getRight();
		auto b = (this_->getBottom() > rect.getBottom()) ? rect.getBottom() : this_->getBottom();
		this_->x = l;
		this_->y = t;
		this_->width = r - l;
		this_->height = b - t;
	}
};

/** 2次元の矩形を表すクラスです。*/
LN_STRUCT()
struct Rect
{
public:
	static const Rect	Zero;	/**< (0, 0, 0, 0) */
	static const Rect	Empty;	/**< (0, 0, -1, -1) */

public:

	/** 左辺の X 座標 */
	LN_FIELD()
	float x;

	/** 上辺の Y 座標 */
	LN_FIELD()
	float y;

	/** 幅 */
	LN_FIELD()
	float width;

	/** 高さ */
	LN_FIELD()
	float height;

public:

	/** すべての要素を 0 で初期化します。*/
	LN_METHOD()
	Rect() { set(0, 0, 0, 0); }

	/** 位置とサイズを指定して初期化します。*/
	LN_METHOD()
	Rect(float x, float y, float width, float height) { set(x, y, width, height); }
	
	/** 位置とサイズを指定して初期化します。*/
	Rect(const PointF& point, const Size& size) { set(point.x, point.y, size.width, size.height); }

	/** 位置とサイズを指定して初期化します。*/
	Rect(float x, float y, const Size& size) { set(x, y, size.width, size.height); }

	/** 位置とサイズを指定して初期化します。 */
	Rect(const PointF& point, float width, float height) { set(point.x, point.y, width, height); }

	/** 指定した矩形をコピーして初期化します。*/
	Rect(const Rect& rect) { set(rect.x, rect.y, rect.width, rect.height); }

public:

	/** 各要素を設定します。*/
	void set(float x_, float y_, float width_, float height_) { x = x_; y = y_; width = width_; height = height_; }

	/** 各要素を設定します。*/
	void set(float x_, float y_, const Size& size) { x = x_; y = y_; width = size.width; height = size.height; }

	/** 左辺の x 軸の値を取得します。*/
	LN_METHOD()
	float getLeft() const { return x; }

	/** 上辺の y 軸の位置を取得します。*/
	float getTop() const { return y; }

	/** 右辺の x 軸の値を取得します。*/
	float getRight() const { return x + width; }

	/** 底辺の y 軸の値を取得します。*/
	float getBottom() const { return y + height; }

	/** 左上隅の位置を取得します。*/
	PointF getTopLeft() const { return PointF(getLeft(), getTop()); }
	
	/** 右上隅の位置を取得します。*/
	PointF getTopRight() const { return PointF(getRight(), getTop()); }
	
	/** 左下隅の位置を取得します。*/
	PointF getBottomLeft() const { return PointF(getLeft(), getBottom()); }

	/** 右下隅の位置を取得します。*/
	PointF getBottomRight() const { return PointF(getRight(), getBottom()); }

	/** 左上隅の位置を設定します。*/
	void setLocation(const PointF& pt) { x = pt.x; y = pt.y; }	// TODO: Pos?

	/** 左上隅の位置を取得します。*/
	PointF setLocation() const { return PointF(x, y); }
	
	/** 幅と高さを設定します。*/
	LN_METHOD(Property)
	void setSize(const Size& size) { width = size.width; height = size.height; }
	
	/** 幅と高さを取得します。*/
	LN_METHOD(Property)
	Size getSize() const { return Size(width, height); }

	/** 幅または高さを持たないかを判定します。*/
	bool isEmpty() const { return (width < 0 || height < 0); }

	/** 要素がすべて 0 かを判定します。*/
	bool isZero() const { return (x == 0 && y == 0 && width == 0 && height == 0); }

	/** 矩形内に指定した点が含まれているかどうかを判定します。*/
	bool contains(const PointF point) const { return contains(point.x, point.y); }
	
	/** 矩形内に指定した点が含まれているかどうかを判定します。*/
	bool contains(float x_, float y_) const
	{
		if (isEmpty()) {
			return false;
		}
		return ((x_ >= x) && (x_ - width <= x) && (y_ >= y) && (y_ - height <= y));
	}
	
	/**
		@brief	指定した矩形全体が、この矩形内部に含まれているかを判定します。
	*/
	bool contains(const Rect& rect) const
	{
		if (isEmpty() || rect.isEmpty()) {
			return false;
		}
		return (
			x <= rect.x &&
			y <= rect.y &&
			x + width >= rect.x + rect.width &&
			y + height >= rect.y + rect.height);
	}

	/**
		@brief	指定した矩形に収まるように、この矩形をクリッピングします。
	*/
	void clip(const Rect& rect)
	{
		GeometryStructsHelper::clip(this, rect);
	}

	/**
		@brief	四角形を拡大または縮小します。
	*/
	void inflate(float width_, float height_)
	{
		x -= width_;
		y -= height_;
		width += width_ * 2;
		height += height_ * 2;
		if (width < 0) width = 0;
		if (height < 0) height = 0;
	}

public:
	bool operator == (const Rect& obj) const { return (x == obj.x && y == obj.y && width == obj.width && height == obj.height); }
	bool operator != (const Rect& obj) const { return !operator==(obj); }
};

// 内部用
class RectI
{
public:
	static const RectI	Zero;

	int		x;
	int		y;
	int		width;
	int		height;

	RectI() { set(0, 0, 0, 0); }
	RectI(int x, int y, int width, int height) { set(x, y, width, height); }
	RectI(int x, int y, const SizeI& size) { set(x, y, size.width, size.height); }
	RectI(const RectI& rect) { set(rect.x, rect.y, rect.width, rect.height); }

	void set(int x_, int y_, int width_, int height_) { x = x_; y = y_; width = width_; height = height_; }
	void set(int x_, int y_, const SizeI& size) { x = x_; y = y_; width = size.width; height = size.height; }

	bool isEmpty() const { return (width < 0 || height < 0); }
	int getLeft() const { return x; }
	int getTop() const { return y; }
	int getRight() const { return x + width; }
	int getBottom() const { return y + height; }
	SizeI getSize() const { return SizeI(width, height); }

	void clip(const RectI& rect) { GeometryStructsHelper::clip(this, rect); }

	static RectI fromFloatRect(const Rect& rect) { return RectI((float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height); }

	bool operator == (const RectI& right) const { return (x == right.x && y == right.y && width == right.width && height == right.height); }
	bool operator != (const RectI& right) const { return !operator==(right); }
};

// 内部用
class Box32
{
public:
	static const Box32 Zero;

public:
	int		x;
	int		y;
	int		z;
	int		width;
	int		height;
	int		depth;

public:
	Box32()
		: x(0)
		, y(0)
		, z(0)
		, width(0)
		, height(0)
		, depth(0)
	{
	}

	Box32(int x_, int y_, int z_, int width_, int height_, int depth_)
		: x(x_)
		, y(y_)
		, z(z_)
		, width(width_)
		, height(height_)
		, depth(depth_)
	{
	}
};

class Thickness	// TODO: struct
{
public:
	int		Left;		/**< 四角形の左辺の幅 */
	int		Top;		/**< 四角形の上辺の幅 */
	int		Right;		/**< 四角形の右辺の幅 */
	int		Bottom;		/**< 四角形の底辺の幅 */

};

/**
	@brief		四角形の枠の太さを表すクラスです。(浮動小数点形式)
*/
class ThicknessF
{
public:
	static const ThicknessF	Zero;	///< ThicknessF(0, 0, 0, 0)

public:
	float		Left;		/**< 四角形の左辺の幅 */
	float		Top;		/**< 四角形の上辺の幅 */
	float		Right;		/**< 四角形の右辺の幅 */
	float		Bottom;		/**< 四角形の底辺の幅 */

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	ThicknessF() { set(0, 0, 0, 0); }

	/** すべての要素を uniformLength で初期化します。 */
	ThicknessF(float uniformLength) { set(uniformLength, uniformLength, uniformLength, uniformLength); }

	/**
		@brief	左右共通の幅及び上下共通の幅を指定して初期化します。
	*/
	ThicknessF(float leftAndRight, float topAndbottom) { set(leftAndRight, topAndbottom, leftAndRight, topAndbottom); }
	
	/**
		@brief	各辺の幅を指定して初期化します。
	*/
	ThicknessF(float left, float top, float right, float bottom) { set(left, top, right, bottom); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void set(float left, float top, float right, float bottom) { Left = left; Top = top; Right = right; Bottom = bottom; }

	/** 要素がすべて 0 かを判定します。*/
	bool isZero() const { return (Left == 0 && Top == 0 && Right == 0 && Bottom == 0); }

	void toArray(float* buf) const { buf[0] = Left; buf[1] = Top; buf[2] = Right; buf[3] = Bottom; }
	static ThicknessF fromArray(const float* buf) { return ThicknessF(buf[0], buf[1], buf[2], buf[3]); }

public:
	bool operator == (const ThicknessF& obj) const { return (Left == obj.Left && Top == obj.Top && Right == obj.Right && Bottom == obj.Bottom); }
	bool operator != (const ThicknessF& obj) const { return !operator==(obj); }
};


/**
	@brief	四角形の角の半径を表します。
*/
struct CornerRadius
{
public:

	/** 左上角の半径 */
	float topLeft;

	/** 右上角の半径 */
	float topRight;

	/** 右下角の半径 */
	float bottomRight;

	/** 左下角の半径 */
	float bottomLeft;

public:
	/** すべての要素を 0 で初期化します。 */
	CornerRadius();

	/** すべての要素を uniformRadius で初期化します。 */
	CornerRadius(float uniformRadius);

	/** 各要素の値を指定して初期化します。 */
	CornerRadius(float topLeft, float topRight, float bottomRight, float bottomLeft);

	bool operator == (const CornerRadius& rhs) const { return topLeft == rhs.topLeft && topRight == rhs.topRight && bottomRight == rhs.bottomRight && bottomLeft == rhs.bottomLeft; }
	bool operator != (const CornerRadius& rhs) const { return !operator==(rhs); }
};

namespace detail
{

struct Sphere
{
	Vector3		center;
	float		radius;
};

} // namespace detail
LN_NAMESPACE_END
