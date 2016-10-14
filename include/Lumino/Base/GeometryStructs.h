/**
	@file	Point.h
*/
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief	2次元上の点を表すクラスです。
*/
class Point
{
public:
	static const Point	Zero;	///< Point(0, 0)

public:
	int		x;		///< X 座標
	int		y;		///< Y 座標

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	Point() { Set(0, 0); }
	
	/**
		@brief	座標を指定して初期化します。
	*/
	Point(int x, int y) { Set(x, y); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(int x_, int y_) { x = x_; y = y_; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0 && y == 0); }

public:
	bool operator == (const Point& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const Point& obj) const { return !operator==(obj); }
};

/**
	@brief	2次元上の点を表すクラスです。(浮動小数点形式)
*/
class PointF
{
public:
	static const PointF	Zero;	///< PointF(0, 0)

public:
	float	x;		///< X 座標
	float	y;		///< Y 座標

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	PointF() { Set(0, 0); }
	
	/**
		@brief	座標を指定して初期化します。
	*/
	PointF(float x, float y) { Set(x, y); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float x_, float y_) { x = x_; y = y_; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0.0f && y == 0.0f); }

public:
	bool operator == (const PointF& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const PointF& obj) const { return !operator==(obj); }
};


/**
	@brief		2次元上のオブジェクトサイズを表すクラスです。
*/
class SizeI
{
public:
	static const SizeI	Zero;	/**< Size(0, 0)	*/

public:
	int		width;				/**< X 方向の大きさ*/
	int		height;				/**< Y 方向の大きさ*/

public:

	/** すべての要素を 0 で初期化します。*/
	SizeI() { Set(0, 0); }
	
	/** 幅と高さを指定して初期化します。*/
	SizeI(int w, int h) { Set(w, h); }

public:
	
	/** 各要素を設定します。*/
	void Set(int w, int h) { width = w; height = h; }

	/** 要素がすべて 0 かを判定します。*/
	bool IsZero() const { return (width == 0 && height == 0); }

	/** いずれかの要素が 0 かを判定します。*/
	bool IsAnyZero() const { return (width == 0 || height == 0); }

public:
	bool operator == (const SizeI& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const SizeI& obj) const { return !operator==(obj); }
};

/**
	@brief		2次元上のオブジェクトサイズを表すクラスです。(浮動小数点形式)
*/
class SizeF
{
public:
	static const SizeF	Zero;	///< SizeF(0, 0)
	static const SizeF	NaN;	///< SizeF(NaN, NaN)
	static const SizeF	Max;	///< SizeF(FLT_MAX, FLT_MAX)

public:
	float	width;				///< X 方向の大きさ
	float	height;				///< Y 方向の大きさ

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	SizeF() { Set(0, 0); }
	
	/**
		@brief	幅と高さを指定して初期化します。
	*/
	SizeF(float w, float h) { Set(w, h); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float w, float h) { width = w; height = h; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (width == 0.0f && height == 0.0f); }

	/**
		@brief	いずれかの要素が 0 かを判定します。
	*/
	bool IsAnyZero() const { return (width == 0.0f || height == 0.0f); }

public:
	bool operator == (const SizeF& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const SizeF& obj) const { return !operator==(obj); }
};


/**
	@brief		2次元の矩形を表すクラスです。
*/
class Rect
{
public:
	static const Rect	Zero;	///< RectF(0, 0, 0, 0)
	static const Rect	Empty;	///< RectF(0, 0, -1, -1)

public:
	int		x;			///< 左辺の X 座標
	int		y;			///< 上辺の Y 座標
	int		width;		///< 幅
	int		height;		///< 高さ

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	Rect() { Set(0, 0, 0, 0); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	Rect(int x, int y, int width, int height) { Set(x, y, width, height); }
	
	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	Rect(const Point& point, const SizeI& size) { Set(point.x, point.y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	Rect(int x, int y, const SizeI& size) { Set(x, y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	Rect(const Point& point, int width, int height) { Set(point.x, point.y, width, height); }

	/**
		@brief	指定した矩形をコピーして初期化します。
	*/
	Rect(const Rect& rect) { Set(rect.x, rect.y, rect.width, rect.height); }

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(int x_, int y_, int width_, int height_) { x = x_; y = y_; width = width_; height = height_; }

	/**
		@brief	各要素を設定します。
	*/
	void Set(int x_, int y_, const SizeI& size) { x = x_; y = y_; width = size.width; height = size.height; }

	/**
		@brief	左辺の x 軸の値を取得します。
	*/
	int GetLeft() const { return x; }

	/**
		@brief	上辺の y 軸の位置を取得します。
	*/
	int GetTop() const { return y; }

	/**
		@brief	右辺の x 軸の値を取得します。
	*/
	int GetRight() const { return x + width; }

	/**
		@brief	底辺の y 軸の値を取得します。
	*/
	int GetBottom() const { return y + height; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	Point GetTopLeft() const { return Point(GetLeft(), GetTop()); }
	
	/**
		@brief	右上隅の位置を取得します。
	*/
	Point GetTopRight() const { return Point(GetRight(), GetTop()); }
	
	/**
		@brief	左下隅の位置を取得します。
	*/
	Point GetBottomLeft() const { return Point(GetLeft(), GetBottom()); }

	/**
		@brief	右下隅の位置を取得します。
	*/
	Point GetBottomRight() const { return Point(GetRight(), GetBottom()); }

	/**
		@brief	左上隅の位置を設定します。
	*/
	void SetLocation(const Point& pt) { x = pt.x; y = pt.y; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	Point GetLocation() const { return Point(x, y); }
	
	/**
		@brief	幅と高さを設定します。
	*/
	void SetSize(const SizeI& size) { width = size.width; height = size.height; }
	
	/**
		@brief	幅と高さを取得します。
	*/
	SizeI GetSize() const { return SizeI(width, height); }

	/**
		@brief	幅または高さを持たないかを判定します。
	*/
	bool IsEmpty() const { return (width < 0 || height < 0); }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0 && y == 0 && width == 0 && height == 0); }

	/**
		@brief	矩形内に指定した点が含まれているかどうかを判定します。
	*/
	bool Contains(Point point) const { return Contains(point.x, point.y); }
	
	/**
		@brief	矩形内に指定した点が含まれているかどうかを判定します。
	*/
	bool Contains(int x_, int y_) const
	{
		if (IsEmpty()) {
			return false;
		}
		return ((x_ >= x) && (x_ - width <= x) && (y_ >= y) && (y_ - height <= y));
	}
	
	/**
		@brief	指定した矩形全体が、この矩形内部に含まれているかを判定します。
	*/
	bool Contains(const Rect& rect) const
	{
		if (IsEmpty() || rect.IsEmpty()) {
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
	void Clip(const Rect& rect)
	{
		int l = (x < rect.x) ? rect.x : x;
		int t = (y < rect.y) ? rect.y : y;
		int r = (GetRight() > rect.GetRight()) ? rect.GetRight() : GetRight();
		int b = (GetBottom() > rect.GetBottom()) ? rect.GetBottom() : GetBottom();
		x = l;
		y = t;
		width = r - l;
		height = b - t;
	}

	/**
		@brief	四角形を拡大または縮小します。
	*/
	void Inflate(int width_, int height_)
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

	static Rect MakeFromBounds(int left, int top, int right, int bottom) { return Rect(left, top, right - left, bottom - top); }
};

/**
	@brief		2次元の矩形を表すクラスです。
*/
class RectF
{
public:
	static const RectF Zero;	///< RectF(0, 0, 0, 0)
	static const RectF Empty;	///< RectF(0, 0, -1, -1)

public:
	float		x;			///< 左辺の X 座標
	float		y;			///< 上辺の Y 座標
	float		width;		///< 幅
	float		height;		///< 高さ

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	RectF() { Set(0, 0, 0, 0); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	RectF(float x, float y, float width, float height) { Set(x, y, width, height); }
	
	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	RectF(float x, float y, const SizeF& size) { Set(x, y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	RectF(const PointF& point, const SizeF& size) { Set(point.x, point.y, size.width, size.height); }
	
	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	RectF(const PointF& point, float width, float height) { Set(point.x, point.y, width,height); }

	/**
		@brief	指定した矩形をコピーして初期化します。
	*/
	RectF(const RectF& rect) { Set(rect.x, rect.y, rect.width, rect.height); }

	/**
		@brief	指定した矩形をコピーして初期化します。
	*/
	RectF(const Rect& rect) { Set((float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height); }

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(float x_, float y_, float width_, float height_) { x = x_; y = y_; width = width_; height = height_; }

	/**
		@brief	左辺の x 軸の値を取得します。
	*/
	float GetLeft() const { return x; }

	/**
		@brief	上辺の y 軸の位置を取得します。
	*/
	float GetTop() const { return y; }

	/**
		@brief	右辺の x 軸の値を取得します。
	*/
	float GetRight() const { return x + width; }

	/**
		@brief	底辺の y 軸の値を取得します。
	*/
	float GetBottom() const { return y + height; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	PointF GetTopLeft() const { return PointF(GetLeft(), GetTop()); }
	
	/**
		@brief	右上隅の位置を取得します。
	*/
	PointF GetTopRight() const { return PointF(GetRight(), GetTop()); }
	
	/**
		@brief	左下隅の位置を取得します。
	*/
	PointF GetBottomLeft() const { return PointF(GetLeft(), GetBottom()); }

	/**
		@brief	右下隅の位置を取得します。
	*/
	PointF GetBottomRight() const { return PointF(GetRight(), GetBottom()); }

	/**
		@brief	左上隅の位置を設定します。
	*/
	void SetPosition(const PointF& pt) { x = pt.x; y = pt.y; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	const PointF& GetPosition() const { return *((PointF*)&x); }
	
	/**
		@brief	幅と高さを設定します。
	*/
	void SetSize(const SizeF& size) { width = size.width; height = size.height; }
	
	/**
		@brief	幅と高さを取得します。
	*/
	const SizeF& GetSize() const { return *((SizeF*)&width); }

	/**
		@brief	幅または高さを持たないかを判定します。
	*/
	bool IsEmpty() const { return (width < 0 || height < 0); }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0 && y == 0 && width == 0 && height == 0); }

	/**
		@brief	矩形内に指定した点が含まれているかどうかを判定します。
	*/
	bool Contains(PointF point) const { return Contains(point.x, point.y); }
	
	/**
		@brief	矩形内に指定した点が含まれているかどうかを判定します。
	*/
	bool Contains(float x_, float y_) const
	{
		if (IsEmpty()) {
			return false;
		}
		return ((x_ >= x) && (x_ - width <= x) && (y_ >= y) && (y_ - height <= y));
	}
	
	/**
		@brief	指定した矩形全体が、この矩形内部に含まれているかを判定します。
	*/
	bool Contains(const RectF& rect) const
	{
		if (IsEmpty() || rect.IsEmpty()) {
			return false;
		}
		return (
			x <= rect.x &&
			y <= rect.y &&
			x + width >= rect.x + rect.width &&
			y + height >= rect.y + rect.height);
	}

	/**
		@brief	四角形を拡大または縮小します。
	*/
	void Inflate(float width_, float height_)
	{
		x -= width_;
		y -= height_;
		width += width_ * 2;
		height += height_ * 2;
		if (width < 0) width = 0;
		if (height < 0) height = 0;
	}

public:
	bool operator == (const RectF& obj) const { return (x == obj.x && y == obj.y && width == obj.width && height == obj.height); }
	bool operator != (const RectF& obj) const { return !operator==(obj); }
};

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
	ThicknessF() { Set(0, 0, 0, 0); }
	
	/**
		@brief	左右共通の幅及び上下共通の幅を指定して初期化します。
	*/
	ThicknessF(float leftAndRight, float topAndbottom) { Set(leftAndRight, topAndbottom, leftAndRight, topAndbottom); }
	
	/**
		@brief	各辺の幅を指定して初期化します。
	*/
	ThicknessF(float left, float top, float right, float bottom) { Set(left, top, right, bottom); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float left, float top, float right, float bottom) { Left = left; Top = top; Right = right; Bottom = bottom; }

	void ToArray(float* buf) const { buf[0] = Left; buf[1] = Top; buf[2] = Right; buf[3] = Bottom; }
	static ThicknessF FromArray(const float* buf) { return ThicknessF(buf[0], buf[1], buf[2], buf[3]); }

public:
	bool operator == (const ThicknessF& obj) const { return (Left == obj.Left && Top == obj.Top && Right == obj.Right && Bottom == obj.Bottom); }
	bool operator != (const ThicknessF& obj) const { return !operator==(obj); }
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
