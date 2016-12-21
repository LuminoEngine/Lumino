
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
	GenericPoint() { Set(0, 0); }
	
	/** 各要素を指定して初期化します。*/
	GenericPoint(T x, T y) { Set(x, y); }
	
	/** 各要素を設定します。 */
	void Set(T x_, T y_) { x = x_; y = y_; }

	/** 要素がすべて 0 であるかを判定します。*/
	bool IsZero() const { return (x == 0 && y == 0); }

public:
	bool operator == (const GenericPoint<T>& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const GenericPoint<T>& obj) const { return !operator==(obj); }
};

using PointF = GenericPoint<float>;
using PointI = GenericPoint<int>;

/**
	@brief		2次元上のオブジェクトサイズを表します。
*/
template<typename T>
struct GenericSize
{
public:
	static const GenericSize<T>	Zero;		/**< (0, 0) */
	static const GenericSize<T>	MinValue;	/**< */
	static const GenericSize<T>	MaxValue;	/**< */

public:
	T		width;				/**< X 方向の大きさ*/
	T		height;				/**< Y 方向の大きさ*/

public:

	/** すべての要素を 0 で初期化します。*/
	GenericSize() { Set(0, 0); }
	
	/** 幅と高さを指定して初期化します。*/
	GenericSize(T w, T h) { Set(w, h); }

public:
	
	/** 各要素を設定します。*/
	void Set(T w, T h) { width = w; height = h; }

	/** 要素がすべて 0 かを判定します。*/
	bool IsZero() const { return (width == 0 && height == 0); }

	/** いずれかの要素が 0 かを判定します。*/
	bool IsAnyZero() const { return (width == 0 || height == 0); }

public:
	static GenericSize<T> Min(const GenericSize<T>& size1, const GenericSize<T>& size2)
	{
		return GenericSize<T>(
			(size1.width < size2.width) ? size1.width : size2.width,
			(size1.height < size2.height) ? size1.height : size2.height);
	}

	static GenericSize<T> Max(const GenericSize<T>& size1, const GenericSize<T>& size2)
	{
		return GenericSize<T>(
			(size1.width > size2.width) ? size1.width : size2.width,
			(size1.height > size2.height) ? size1.height : size2.height);
	}

public:
	bool operator == (const GenericSize<T>& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const GenericSize<T>& obj) const { return !operator==(obj); }
};

using Size = GenericSize<float>;
using SizeI = GenericSize<int>;


/**
	@brief		2次元の矩形を表すクラスです。
*/
template<typename T>
class GenericRect
{
public:
	static const GenericRect<T>	Zero;	/**< (0, 0, 0, 0) */
	static const GenericRect<T>	Empty;	/**< (0, 0, -1, -1) */

public:
	T		x;							/**< 左辺の X 座標 */
	T		y;							/**< 上辺の Y 座標 */
	T		width;						/**< 幅 */
	T		height;						/**< 高さ */

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	GenericRect() { Set(0, 0, 0, 0); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	GenericRect(T x, T y, T width, T height) { Set(x, y, width, height); }
	
	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	GenericRect(const GenericPoint<T>& point, const GenericSize<T>& size) { Set(point.x, point.y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	GenericRect(T x, T y, const GenericSize<T>& size) { Set(x, y, size.width, size.height); }

	/**
		@brief	位置とサイズを指定して初期化します。
	*/
	GenericRect(const GenericPoint<T>& point, T width, T height) { Set(point.x, point.y, width, height); }

	/**
		@brief	指定した矩形をコピーして初期化します。
	*/
	GenericRect(const GenericRect<T>& rect) { Set(rect.x, rect.y, rect.width, rect.height); }

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(T x_, T y_, T width_, T height_) { x = x_; y = y_; width = width_; height = height_; }

	/**
		@brief	各要素を設定します。
	*/
	void Set(T x_, T y_, const GenericSize<T>& size) { x = x_; y = y_; width = size.width; height = size.height; }

	/**
		@brief	左辺の x 軸の値を取得します。
	*/
	T GetLeft() const { return x; }

	/**
		@brief	上辺の y 軸の位置を取得します。
	*/
	T GetTop() const { return y; }

	/**
		@brief	右辺の x 軸の値を取得します。
	*/
	T GetRight() const { return x + width; }

	/**
		@brief	底辺の y 軸の値を取得します。
	*/
	T GetBottom() const { return y + height; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	GenericPoint<T> GetTopLeft() const { return GenericPoint<T>(GetLeft(), GetTop()); }
	
	/**
		@brief	右上隅の位置を取得します。
	*/
	GenericPoint<T> GetTopRight() const { return GenericPoint<T>(GetRight(), GetTop()); }
	
	/**
		@brief	左下隅の位置を取得します。
	*/
	GenericPoint<T> GetBottomLeft() const { return GenericPoint<T>(GetLeft(), GetBottom()); }

	/**
		@brief	右下隅の位置を取得します。
	*/
	GenericPoint<T> GetBottomRight() const { return GenericPoint<T>(GetRight(), GetBottom()); }

	/**
		@brief	左上隅の位置を設定します。
	*/
	void SetLocation(const GenericPoint<T>& pt) { x = pt.x; y = pt.y; }

	/**
		@brief	左上隅の位置を取得します。
	*/
	GenericPoint<T> GetLocation() const { return GenericPoint<T>(x, y); }
	
	/**
		@brief	幅と高さを設定します。
	*/
	void SetSize(const GenericSize<T>& size) { width = size.width; height = size.height; }
	
	/**
		@brief	幅と高さを取得します。
	*/
	GenericSize<T> GetSize() const { return GenericSize<T>(width, height); }

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
	bool Contains(GenericPoint<T> point) const { return Contains(point.x, point.y); }
	
	/**
		@brief	矩形内に指定した点が含まれているかどうかを判定します。
	*/
	bool Contains(T x_, T y_) const
	{
		if (IsEmpty()) {
			return false;
		}
		return ((x_ >= x) && (x_ - width <= x) && (y_ >= y) && (y_ - height <= y));
	}
	
	/**
		@brief	指定した矩形全体が、この矩形内部に含まれているかを判定します。
	*/
	bool Contains(const GenericRect<T>& rect) const
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
	void Clip(const GenericRect<T>& rect)
	{
		T l = (x < rect.x) ? rect.x : x;
		T t = (y < rect.y) ? rect.y : y;
		T r = (GetRight() > rect.GetRight()) ? rect.GetRight() : GetRight();
		T b = (GetBottom() > rect.GetBottom()) ? rect.GetBottom() : GetBottom();
		x = l;
		y = t;
		width = r - l;
		height = b - t;
	}

	/**
		@brief	四角形を拡大または縮小します。
	*/
	void Inflate(T width_, T height_)
	{
		x -= width_;
		y -= height_;
		width += width_ * 2;
		height += height_ * 2;
		if (width < 0) width = 0;
		if (height < 0) height = 0;
	}

public:
	bool operator == (const GenericRect<T>& obj) const { return (x == obj.x && y == obj.y && width == obj.width && height == obj.height); }
	bool operator != (const GenericRect<T>& obj) const { return !operator==(obj); }

	static GenericRect<T> MakeFromBounds(T left, T top, T bottom) { return GenericRect<T>(left, top, right - left, bottom - top); }
};

using RectF = GenericRect<float>;
using RectI = GenericRect<int>;


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
