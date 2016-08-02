
#pragma once
#include "Point.h"
#include "Size.h"

LN_NAMESPACE_BEGIN

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
	Rect(const Point& point, const Size& size) { Set(point.x, point.y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	Rect(int x, int y, const Size& size) { Set(x, y, size.width, size.height); }

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
	void Set(int x_, int y_, const Size& size) { x = x_; y = y_; width = size.width; height = size.height; }

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
	void SetSize(const Size& size) { width = size.width; height = size.height; }
	
	/**
		@brief	幅と高さを取得します。
	*/
	Size GetSize() const { return Size(width, height); }

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

LN_NAMESPACE_END
